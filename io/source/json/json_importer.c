/* File: json_importer.c; Copyright and License: see below */

#include "json/json_importer.h"
#include "ctrl_error.h"
#include "util/string/utf8string.h"
#include "trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void json_importer_init( json_importer_t *this_,
                         data_database_reader_t *db_reader,
                         ctrl_controller_t *controller,
                         data_stat_t *io_stat )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != controller );
    assert( NULL != io_stat );

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).stat = io_stat;

    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void json_importer_destroy( json_importer_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).db_reader );
    assert( NULL != (*this_).controller );
    assert( NULL != (*this_).stat );

    data_rules_destroy ( &((*this_).data_rules) );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).stat = NULL;

    TRACE_END();
}

data_error_t json_importer_import_stream( json_importer_t *this_,
                                                          universal_input_stream_t *json_text,
                                                          data_row_id_t diagram_id,
                                                          uint32_t *out_read_line )
{
    TRACE_BEGIN();
    assert( NULL != json_text );
    assert( NULL != out_read_line );

    data_error_t parse_error = DATA_ERROR_NONE;
    data_row_id_t attach_diagram_id = diagram_id;

    json_deserializer_init( &((*this_).temp_deserializer), json_text );

    /* check if diagram id exists */
    {
        static data_diagram_t dummy_diagram;
        parse_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, &dummy_diagram );
        if ( DATA_ERROR_NONE != parse_error )
        {
            TSLOG_ERROR_INT( "diagram id where to import json data does not exist (anymore)", diagram_id );
        }
    }

    /* read header */
    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_header( &((*this_).temp_deserializer) );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_importer_private_import_views( this_, &attach_diagram_id );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_importer_private_import_nodes( this_, attach_diagram_id );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_importer_private_import_edges( this_,  attach_diagram_id );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_footer( &((*this_).temp_deserializer) );
    }

    json_deserializer_get_read_line ( &((*this_).temp_deserializer), out_read_line );
    json_deserializer_destroy( &((*this_).temp_deserializer) );

    TRACE_END_ERR( parse_error );
    return parse_error;
}

bool json_importer_private_is_feature_focused_in_diagram( json_importer_t *this_, data_row_id_t diagram_id, data_row_id_t feature_id )
{
    TRACE_BEGIN();
    bool is_focused = false;

    /* read the database */
    uint32_t diagramelement_count;
    data_error_t d_err;
    d_err = data_database_reader_get_diagramelements_by_diagram_id ( (*this_).db_reader,
                                                                     diagram_id,
                                                                     JSON_IMPORT_TO_DATABASE_MAX_DIAGELES,
                                                                     &((*this_).temp_diageles),
                                                                     &diagramelement_count
                                                                   );
    if ( d_err == DATA_ERROR_NONE )
    {
        for ( uint_fast32_t idx = 0; idx < diagramelement_count; idx ++ )
        {
            data_diagramelement_t *current_diagele = &((*this_).temp_diageles[idx]);
            if ( feature_id == data_diagramelement_get_focused_feature_row_id( current_diagele ) )
            {
                is_focused = true;
            }
        }
    }
    else
    {
        TSLOG_ERROR_HEX( "data_database_reader_get_diagramelements_by_diagram_id failed.", d_err );
    }

    TRACE_END();
    return is_focused;
}

data_error_t json_importer_prescan( json_importer_t *this_,
                                    universal_input_stream_t *in_stream,
                                    universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    char buf[1024];
    int err = 0;
    int count = 0;
    while( err == 0 )
    {
        size_t length;
        err = universal_input_stream_read( in_stream, &buf, sizeof(buf), &length );
        count ++;
    }
    universal_utf8_writer_write_str( out_english_report, "\nread size: " );
    universal_utf8_writer_write_int( out_english_report, count );
    universal_utf8_writer_write_str( out_english_report, " kB\n" );

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_importer_private_import_views( json_importer_t *this_, data_row_id_t *io_diagram_id )
{
    TRACE_BEGIN();
    assert( NULL != io_diagram_id );

    data_error_t parse_error = DATA_ERROR_NONE;

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &((*this_).temp_deserializer) );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
        bool any_error = ( parse_error != DATA_ERROR_NONE );  /* consolidation of parser errors and writer errors */
        bool is_first = true;  /* is this the first object in the database that is created? if false, database changes are appended to the last undo_redo_set */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_expect_begin_type_of_element( &((*this_).temp_deserializer), &next_object_type );
            if ( DATA_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_DIAGRAM:
                    {
                        data_diagram_t new_diagram;
                        parse_error = json_deserializer_get_next_diagram ( &((*this_).temp_deserializer), &new_diagram );
                        if ( DATA_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            any_error = true;
                        }
                        else
                        {
                            /* create the parsed diagram as child below the current diagram */
                            ctrl_diagram_controller_t *diag_ctrl;
                            diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                            ctrl_error_t write_error3;
                            data_row_id_t new_diag_id;
                            data_diagram_set_parent_row_id( &new_diagram, *io_diagram_id );
                            write_error3 = ctrl_diagram_controller_create_diagram ( diag_ctrl,
                                                                                    &new_diagram,
                                                                                    ( is_first
                                                                                    ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                                    : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                                    &new_diag_id
                                                                                  );
                            data_stat_inc_count ( (*this_).stat,
                                                  DATA_TABLE_DIAGRAM,
                                                  (CTRL_ERROR_NONE==write_error3)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                                );
                            if ( CTRL_ERROR_NONE != write_error3 )
                            {
                                TSLOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagram" );
                                any_error = true;
                            }
                            else
                            {
                                /* insert all consecutive elements to this new diagram */
                                *io_diagram_id = new_diag_id;
                            }
                            is_first = false;
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected token error" );
                        any_error = true;
                    }
                }
                if ( DATA_ERROR_NONE == parse_error )
                {
                    parse_error = json_deserializer_expect_end_type_of_element( &((*this_).temp_deserializer) );
                    /* parser error, break loop: */
                    any_error = ( any_error ||( DATA_ERROR_NONE != parse_error ));
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }

            if ( DATA_ERROR_NONE == parse_error )
            {
                parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
                /* parser error, break loop: */
                any_error = ( any_error ||( DATA_ERROR_NONE != parse_error ));
            }
        }
    }

    TRACE_END_ERR( parse_error );
    return parse_error;
}

data_error_t json_importer_private_import_nodes( json_importer_t *this_, data_row_id_t diagram_id )
{
    TRACE_BEGIN();

    data_error_t parse_error = DATA_ERROR_NONE;

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &((*this_).temp_deserializer) );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
        bool any_error = ( parse_error != DATA_ERROR_NONE );  /* consolidation of parser errors and writer errors */
        bool is_first = true;  /* is this the first object in the database that is created? if false, database changes are appended to the last undo_redo_set */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_expect_begin_type_of_element( &((*this_).temp_deserializer), &next_object_type );
            if ( DATA_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_CLASSIFIER:
                    {
                        data_classifier_t new_classifier;
                        uint32_t feature_count;
                        parse_error = json_deserializer_get_next_classifier ( &((*this_).temp_deserializer),
                                                                              &new_classifier,
                                                                              JSON_IMPORT_TO_DATABASE_MAX_FEATURES,
                                                                              &((*this_).temp_features),
                                                                              &feature_count
                                                                            );
                        if ( DATA_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            any_error = true;
                        }
                        else
                        {
                            /* create classifier if not yet existing */
                            data_row_id_t the_classifier_id;
                            {
                                /* check if the parsed classifier already exists in this database; if not, create it */
                                ctrl_classifier_controller_t *classifier_ctrl;
                                classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

                                data_error_t read_error;
                                data_classifier_t existing_classifier;
                                read_error = data_database_reader_get_classifier_by_name ( (*this_).db_reader,
                                                                                           data_classifier_get_name_const( &new_classifier ),
                                                                                           &existing_classifier
                                                                                         );
                                /* if the name is equal, expect the objects to be equal */
                                const bool classifier_exists = ( DATA_ERROR_NONE == read_error );

                                if ( ! classifier_exists )
                                {
                                    ctrl_error_t write_error;
                                    write_error = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                                                &new_classifier,
                                                                                                ( is_first
                                                                                                ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                                                : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                                                &the_classifier_id
                                                                                              );
                                    data_stat_inc_count ( (*this_).stat,
                                                          DATA_TABLE_CLASSIFIER,
                                                          (CTRL_ERROR_NONE==write_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                                        );

                                    if ( CTRL_ERROR_NONE == write_error )
                                    {
                                        /* create also the features */
                                        for ( int f_index = 0; f_index < feature_count; f_index ++ )
                                        {
                                            data_feature_t *current_feature = &((*this_).temp_features[f_index]);
                                            data_row_id_t new_feature_id;
                                            data_feature_set_classifier_row_id( current_feature, the_classifier_id );
                                            /* filter lifelines */
                                            if ( ! data_rules_feature_is_scenario_cond( &((*this_).data_rules),
                                                                                        data_feature_get_main_type( current_feature ) ) )
                                            {
                                                write_error |= ctrl_classifier_controller_create_feature ( classifier_ctrl,
                                                                                                           current_feature,
                                                                                                           CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                                                           &new_feature_id
                                                                                                         );
                                                data_stat_inc_count ( (*this_).stat,
                                                                      DATA_TABLE_FEATURE,
                                                                      (CTRL_ERROR_NONE==write_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                                                    );
                                            }
                                            else  /* lifeline */
                                            {
                                                data_stat_inc_count ( (*this_).stat,
                                                                      DATA_TABLE_FEATURE,
                                                                      DATA_STAT_SERIES_IGNORED
                                                                    );
                                                TRACE_INFO( "lifeline dropped at json import." );
                                            }
                                        }
                                    }
                                    if ( CTRL_ERROR_NONE != write_error )
                                    {
                                        TSLOG_ERROR( "unexpected error at ctrl_classifier_controller_create_classifier/feature" );
                                        any_error = true;
                                    }
                                    is_first = false;
                                }
                                else
                                {
                                    /* do the statistics */
                                    data_stat_inc_count ( (*this_).stat,
                                                          DATA_TABLE_CLASSIFIER,
                                                          DATA_STAT_SERIES_IGNORED
                                                        );
                                    for ( int f_index = 0; f_index < feature_count; f_index ++ )
                                    {
                                        data_stat_inc_count ( (*this_).stat,
                                                              DATA_TABLE_FEATURE,
                                                              DATA_STAT_SERIES_IGNORED
                                                            );
                                    }
                                    TRACE_INFO( "classifier did already exist, features dropped at json import." );
                                    /* set the the_classifier_id */
                                    the_classifier_id = data_classifier_get_row_id( &existing_classifier );
                                }
                            }

                            if ( ! any_error )  /* no error */
                            {
                                /* link the classifier to the current diagram */
                                ctrl_diagram_controller_t *diag_ctrl;
                                diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                                ctrl_error_t write_error2;
                                data_row_id_t new_element_id;
                                data_diagramelement_t diag_ele;
                                data_diagramelement_init_new ( &diag_ele,
                                                               diagram_id,
                                                               the_classifier_id,
                                                               DATA_DIAGRAMELEMENT_FLAG_NONE,
                                                               DATA_ROW_ID_VOID
                                                             );
                                write_error2 = ctrl_diagram_controller_create_diagramelement ( diag_ctrl,
                                                                                               &diag_ele,
                                                                                               ( is_first
                                                                                               ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                                               : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                                               &new_element_id
                                                                                             );
                                data_stat_inc_count ( (*this_).stat,
                                                      DATA_TABLE_DIAGRAMELEMENT,
                                                      (CTRL_ERROR_NONE==write_error2)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                                    );
                                if ( CTRL_ERROR_NONE != write_error2 )
                                {
                                    TSLOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagramelement" );
                                    any_error = true;
                                }
                                is_first = false;
                            }
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected token error" );
                        any_error = true;
                    }
                }
                if ( DATA_ERROR_NONE == parse_error )
                {
                    parse_error = json_deserializer_expect_end_type_of_element( &((*this_).temp_deserializer) );
                    /* parser error, break loop: */
                    any_error = ( any_error ||( DATA_ERROR_NONE != parse_error ));
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }

            if ( DATA_ERROR_NONE == parse_error )
            {
                parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
                /* parser error, break loop: */
                any_error = ( any_error ||( DATA_ERROR_NONE != parse_error ));
            }
        }
    }

    TRACE_END_ERR( parse_error );
    return parse_error;
}

data_error_t json_importer_private_import_edges( json_importer_t *this_, data_row_id_t diagram_id )
{
    TRACE_BEGIN();

    data_error_t parse_error = DATA_ERROR_NONE;

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &((*this_).temp_deserializer) );
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
        bool any_error = ( parse_error != DATA_ERROR_NONE );  /* consolidation of parser errors and writer errors */
        bool is_first = true;  /* is this the first object in the database that is created? if false, database changes are appended to the last undo_redo_set */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_expect_begin_type_of_element( &((*this_).temp_deserializer), &next_object_type );
            if ( DATA_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_RELATIONSHIP:
                    {
                        data_relationship_t new_relationship;
                        char rel_from_clas_buf[DATA_CLASSIFIER_MAX_NAME_SIZE] = "";
                        utf8stringbuf_t rel_from_clas = UTF8STRINGBUF(rel_from_clas_buf);
                        char rel_from_feat_buf[DATA_FEATURE_MAX_KEY_SIZE] = "";
                        utf8stringbuf_t rel_from_feat = UTF8STRINGBUF(rel_from_feat_buf);
                        char rel_to_clas_buf[DATA_CLASSIFIER_MAX_NAME_SIZE] = "";
                        utf8stringbuf_t rel_to_clas = UTF8STRINGBUF(rel_to_clas_buf);
                        char rel_to_feat_buf[DATA_FEATURE_MAX_KEY_SIZE] = "";
                        utf8stringbuf_t rel_to_feat = UTF8STRINGBUF(rel_to_feat_buf);
                        parse_error = json_deserializer_get_next_relationship ( &((*this_).temp_deserializer),
                                                                                &new_relationship,
                                                                                rel_from_clas,
                                                                                rel_from_feat,
                                                                                rel_to_clas,
                                                                                rel_to_feat
                                                                              );

                        if ( DATA_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            any_error = true;
                        }
                        else
                        {
                            data_row_id_t from_classifier_id = DATA_ROW_ID_VOID;
                            data_row_id_t from_feature_id = DATA_ROW_ID_VOID;
                            data_feature_type_t from_feature_type = DATA_FEATURE_TYPE_VOID;
                            data_row_id_t to_classifier_id = DATA_ROW_ID_VOID;
                            data_row_id_t to_feature_id = DATA_ROW_ID_VOID;
                            data_feature_type_t to_feature_type = DATA_FEATURE_TYPE_VOID;

                            /* determine ids in target database */
                            {
                                data_error_t read_error2;
                                data_classifier_t found_classifier;

                                /* search source classifier id */
                                read_error2 = data_database_reader_get_classifier_by_name ( (*this_).db_reader,
                                                                                            utf8stringbuf_get_string( rel_from_clas ),
                                                                                            &found_classifier
                                                                                          );
                                if ( DATA_ERROR_NONE == read_error2 )
                                {
                                    from_classifier_id = data_classifier_get_row_id( &found_classifier );
                                    TRACE_INFO_STR( "id found for src classifier:", utf8stringbuf_get_string( rel_from_clas ) );

                                    /* search source feature id */
                                    if ( data_relationship_get_from_feature_row_id( &new_relationship ) != DATA_ROW_ID_VOID )
                                    {
                                        uint32_t feature_count;
                                        read_error2 = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                                           from_classifier_id,
                                                                                                           JSON_IMPORT_TO_DATABASE_MAX_FEATURES,
                                                                                                           &((*this_).temp_features),
                                                                                                           &feature_count
                                                                                                         );
                                        if ( DATA_ERROR_NONE == read_error2 )
                                        {
                                            for ( int src_idx=0; src_idx < feature_count; src_idx ++ )
                                            {
                                                const data_feature_t *const current_feature = &((*this_).temp_features[src_idx]);
                                                const data_row_id_t current_feature_id = data_feature_get_row_id( current_feature );
                                                const char *const current_feature_key = data_feature_get_key_const( current_feature );
                                                const data_feature_type_t current_feature_type = data_feature_get_main_type( current_feature );
                                                if ( utf8stringbuf_equals_str( rel_from_feat, current_feature_key ) )
                                                {
                                                    if ( data_rules_feature_is_scenario_cond( &((*this_).data_rules), current_feature_type ) )
                                                    {
                                                        /* lifeline is ok if visible in current diagram */
                                                        bool visible;
                                                        visible = json_importer_private_is_feature_focused_in_diagram ( this_,
                                                                                                                                  diagram_id,
                                                                                                                                  current_feature_id
                                                                                                                                );
                                                        if ( visible )
                                                        {
                                                            from_feature_id = current_feature_id;
                                                            from_feature_type = current_feature_type;
                                                            TRACE_INFO_STR( "id found for scenario src feature:", utf8stringbuf_get_string( rel_from_feat ) );
                                                        }
                                                        else
                                                        {
                                                            /* ignore lifelines of other diagrams */
                                                        }
                                                    }
                                                    else
                                                    {
                                                        from_feature_id = current_feature_id;
                                                        from_feature_type = current_feature_type;
                                                        TRACE_INFO_STR( "id found for unconditional src feature:", utf8stringbuf_get_string( rel_from_feat ) );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                /* search destination classifier id */
                                read_error2 = data_database_reader_get_classifier_by_name ( (*this_).db_reader,
                                                                                            utf8stringbuf_get_string( rel_to_clas ),
                                                                                            &found_classifier
                                                                                          );
                                if ( DATA_ERROR_NONE == read_error2 )
                                {
                                    to_classifier_id = data_classifier_get_row_id( &found_classifier );
                                    TRACE_INFO_STR( "id found for dst classifier:", utf8stringbuf_get_string( rel_to_clas ) );

                                    /* search destination feature id */
                                    if ( data_relationship_get_to_feature_row_id( &new_relationship ) != DATA_ROW_ID_VOID )
                                    {
                                        uint32_t feature_count;
                                        read_error2 = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                                           to_classifier_id,
                                                                                                           JSON_IMPORT_TO_DATABASE_MAX_FEATURES,
                                                                                                           &((*this_).temp_features),
                                                                                                           &feature_count
                                                                                                         );
                                        if ( DATA_ERROR_NONE == read_error2 )
                                        {
                                            for ( int src_idx=0; src_idx < feature_count; src_idx ++ )
                                            {
                                                const data_feature_t *const current_feature = &((*this_).temp_features[src_idx]);
                                                const data_row_id_t current_feature_id = data_feature_get_row_id( current_feature );
                                                const char *const current_feature_key = data_feature_get_key_const( current_feature );
                                                const data_feature_type_t current_feature_type = data_feature_get_main_type( current_feature );
                                                 if ( utf8stringbuf_equals_str( rel_to_feat, current_feature_key ) )
                                                {
                                                    if ( data_rules_feature_is_scenario_cond( &((*this_).data_rules), current_feature_type ) )
                                                    {
                                                        /* lifeline is ok if visible in current diagram */
                                                        bool visible;
                                                        visible = json_importer_private_is_feature_focused_in_diagram ( this_,
                                                                                                                                  diagram_id,
                                                                                                                                  current_feature_id
                                                                                                                                );
                                                        if ( visible )
                                                        {
                                                            to_feature_id = current_feature_id;
                                                            to_feature_type = current_feature_type;
                                                            TRACE_INFO_STR( "id found for scenario dst feature:", utf8stringbuf_get_string( rel_from_feat ) );
                                                        }
                                                        else
                                                        {
                                                            /* ignore lifelines of other diagrams */
                                                        }
                                                    }
                                                    else
                                                    {
                                                        to_feature_id = current_feature_id;
                                                        to_feature_type = current_feature_type;
                                                        TRACE_INFO_STR( "id found for unconditional dst feature:", utf8stringbuf_get_string( rel_from_feat ) );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            /* create relationship */
                            bool dropped=false;
                            if ( from_classifier_id == DATA_ROW_ID_VOID )
                            {
                                TSLOG_ERROR_STR( "A relationship could not be created because the source classifier could not be found.",
                                                 utf8stringbuf_get_string( rel_from_clas )
                                               );
                                dropped = true;
                            }
                            else if ( to_classifier_id == DATA_ROW_ID_VOID )
                            {
                                TSLOG_ERROR_STR( "A relationship could not be created because the destination classifier could not be found.",
                                                 utf8stringbuf_get_string( rel_to_clas )
                                               );
                                dropped = true;
                            }
                            else if (( data_relationship_get_from_feature_row_id( &new_relationship ) != DATA_ROW_ID_VOID )
                                && ( from_feature_id == DATA_ROW_ID_VOID ))
                            {
                                TSLOG_ERROR_STR( "A relationship could not be created because the source feature could not be found.",
                                                 utf8stringbuf_get_string( rel_from_feat )
                                               );
                                dropped = true;
                            }
                            else if (( data_relationship_get_to_feature_row_id( &new_relationship ) != DATA_ROW_ID_VOID )
                                && ( to_feature_id == DATA_ROW_ID_VOID ))
                            {
                                TSLOG_ERROR_STR( "A relationship could not be created because the destination feature could not be found.",
                                                 utf8stringbuf_get_string( rel_to_feat )
                                               );
                                dropped = true;
                            }
                            else
                            {
                                ctrl_error_t write_error4;

                                /* get classifier controller */
                                ctrl_classifier_controller_t *classifier_control4;
                                classifier_control4 = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                                /* update the json-parsed relationship struct */
                                data_relationship_set_row_id ( &new_relationship, DATA_ROW_ID_VOID );
                                data_relationship_set_from_classifier_row_id ( &new_relationship, from_classifier_id );
                                data_relationship_set_from_feature_row_id ( &new_relationship, from_feature_id );
                                data_relationship_set_to_classifier_row_id ( &new_relationship, to_classifier_id );
                                data_relationship_set_to_feature_row_id ( &new_relationship, to_feature_id );

                                /* create relationship */
                                data_row_id_t relationship_id;
                                write_error4 = ctrl_classifier_controller_create_relationship ( classifier_control4,
                                                                                                &new_relationship,
                                                                                                ( is_first
                                                                                                ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                                                : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                                                &relationship_id
                                                                                              );
                                if ( CTRL_ERROR_NONE != write_error4 )
                                {
                                    TSLOG_ERROR( "unexpected error at ctrl_classifier_controller_create_relationship" );
                                    any_error = true;
                                }
                                is_first = false;
                            }
                            /* cleanup */
                            data_relationship_destroy ( &new_relationship );

                            /* update statistics */
                            data_stat_inc_count ( (*this_).stat,
                                                  DATA_TABLE_RELATIONSHIP,
                                                  (!dropped)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                                );
                            if ( dropped )
                            {
                                const bool is_scenario = data_rules_relationship_is_scenario_cond ( &((*this_).data_rules),
                                                                                                    from_feature_type,
                                                                                                    to_feature_type
                                                                                                  );
                                TRACE_INFO( is_scenario ? "relationship in scenario dropped" : "general relationship dropped" );
                            }
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected token error" );
                        any_error = true;
                    }
                }
                if ( DATA_ERROR_NONE == parse_error )
                {
                    parse_error = json_deserializer_expect_end_type_of_element( &((*this_).temp_deserializer) );
                    /* parser error, break loop: */
                    any_error = ( any_error ||( DATA_ERROR_NONE != parse_error ));
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }

            if ( DATA_ERROR_NONE == parse_error )
            {
                parse_error = json_deserializer_check_end_data( &((*this_).temp_deserializer), &set_end );
                /* parser error, break loop: */
                any_error = ( any_error ||( DATA_ERROR_NONE != parse_error ));
            }
        }
    }

    TRACE_END_ERR( parse_error );
    return parse_error;
}


/*
Copyright 2016-2021 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
