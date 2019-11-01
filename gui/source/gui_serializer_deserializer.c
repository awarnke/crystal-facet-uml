/* File: gui_serializer_deserializer.c; Copyright and License: see below */

#include "gui_serializer_deserializer.h"
#include "trace.h"
#include "json/json_serializer.h"
#include "json/json_deserializer.h"
#include "ctrl_error.h"
#include "util/string/utf8string.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_serializer_deserializer_init ( gui_serializer_deserializer_t *this_,
                                        GtkClipboard *clipboard,
                                        gui_simple_message_to_user_t *message_to_user,
                                        data_database_reader_t *db_reader,
                                        ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != clipboard );
    assert( NULL != message_to_user );
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).destination_diagram_id = DATA_ID_VOID_ID;
    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).the_clipboard = clipboard;
    (*this_).clipboard_stringbuf = utf8stringbuf_init( sizeof((*this_).private_clipboard_buffer),
                                                       (*this_).private_clipboard_buffer
                                                     );
    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void gui_serializer_deserializer_destroy ( gui_serializer_deserializer_t *this_ )
{
    TRACE_BEGIN();

    data_rules_destroy ( &((*this_).data_rules) );
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).message_to_user = NULL;

    TRACE_END();
}

void gui_serializer_deserializer_copy_set_to_clipboard( gui_serializer_deserializer_t *this_, data_small_set_t *set_to_be_copied )
{
    TRACE_BEGIN();
    data_error_t serialize_error = DATA_ERROR_NONE;
    data_error_t read_error;
    json_serializer_t serializer;

    json_serializer_init( &serializer );
    utf8stringbuf_clear( (*this_).clipboard_stringbuf );

    serialize_error |= json_serializer_begin_set( &serializer, (*this_).clipboard_stringbuf );

    /* first pass: serialize the diagram(s) if there is one/some */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_copied ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_copied, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            break;

            case DATA_TABLE_DIAGRAM:
            {
                data_diagram_t out_diagram;
                read_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader,
                                                                      data_id_get_row_id( &current_id ),
                                                                      &out_diagram
                                                                    );

                if ( read_error == DATA_ERROR_NONE )
                {
                    serialize_error |= json_serializer_append_diagram( &serializer, &out_diagram, (*this_).clipboard_stringbuf );
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                }
            }
            break;

            case DATA_TABLE_FEATURE:
            break;

            case DATA_TABLE_RELATIONSHIP:
            break;

            default:
            break;
        }
    }

    /* second pass: serialize all classifiers, diagram elements and features */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_copied ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_copied, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            {
                data_classifier_t out_classifier;
                read_error = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                         data_id_get_row_id( &current_id ),
                                                                         &out_classifier
                                                                       );

                if ( read_error == DATA_ERROR_NONE )
                {
                    uint32_t out_feature_count;
                    read_error = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                      data_id_get_row_id( &current_id ),
                                                                                      GUI_SERIALIZER_DESERIALIZER_MAX_FEATURES,
                                                                                      &((*this_).temp_features),
                                                                                      &out_feature_count
                                                                                    );

                    if ( read_error == DATA_ERROR_NONE )
                    {
                        serialize_error |= json_serializer_append_classifier( &serializer,
                                                                              &out_classifier,
                                                                              &((*this_).temp_features),
                                                                              out_feature_count,
                                                                              (*this_).clipboard_stringbuf
                                                                            );
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                }
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_classifier_t out_classifier;
                data_diagramelement_t out_diagramelement;
                int64_t classifier_id;

                read_error = data_database_reader_get_diagramelement_by_id ( (*this_).db_reader,
                                                                             data_id_get_row_id( &current_id ),
                                                                             &out_diagramelement
                                                                           );

                if ( read_error == DATA_ERROR_NONE )
                {
                    classifier_id = data_diagramelement_get_classifier_id( &out_diagramelement );

                    read_error = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                             classifier_id,
                                                                             &out_classifier
                                                                           );

                    if ( read_error == DATA_ERROR_NONE )
                    {
                        uint32_t out_feature_count;
                        read_error = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                          classifier_id,
                                                                                          GUI_SERIALIZER_DESERIALIZER_MAX_FEATURES,
                                                                                          &((*this_).temp_features),
                                                                                          &out_feature_count
                                                                                        );

                        if ( read_error == DATA_ERROR_NONE )
                        {
                            serialize_error |= json_serializer_append_classifier( &serializer,
                                                                                  &out_classifier,
                                                                                  &((*this_).temp_features),
                                                                                  out_feature_count,
                                                                                  (*this_).clipboard_stringbuf
                                                                                );
                        }
                        else
                        {
                            /* program internal error */
                            TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                        }
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                }
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                /* diagrams are serialized in first pass */
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                /* intentionally not supported */
                TRACE_INFO( "gui_toolbox_private_copy_set_to_clipboard does not copy single features, only complete classifiers." );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* relationships are serialized in third pass */
            }
            break;

            default:
            {
                serialize_error |= DATA_ERROR_VALUE_OUT_OF_RANGE;
            }
            break;
        }
    }

    /* third pass: serialize all relationships after the source and destination classifiers */
    for ( int index = 0; index < data_small_set_get_count( set_to_be_copied ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_copied, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            break;

            case DATA_TABLE_DIAGRAM:
            break;

            case DATA_TABLE_FEATURE:
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                data_relationship_t out_relation;
                read_error = data_database_reader_get_relationship_by_id ( (*this_).db_reader,
                                                                           data_id_get_row_id( &current_id ),
                                                                           &out_relation
                                                                         );

                if ( read_error == DATA_ERROR_NONE )
                {
                    data_classifier_t from_classifier;
                    data_classifier_t to_classifier;
                    assert ( GUI_SERIALIZER_DESERIALIZER_MAX_FEATURES >= 2 );

                    /* get source */
                    read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                              data_relationship_get_from_classifier_id( &out_relation ),
                                                                              &from_classifier
                                                                            );
                    if ( DATA_ID_VOID_ID == data_relationship_get_from_feature_id( &out_relation ) )
                    {
                        data_feature_init_empty( &((*this_).temp_features[0]) );
                    }
                    else
                    {
                        read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                               data_relationship_get_from_feature_id( &out_relation ),
                                                                               &((*this_).temp_features[0])
                                                                             );
                    }

                    /* get destination */
                    read_error |= data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                              data_relationship_get_to_classifier_id( &out_relation ),
                                                                              &to_classifier
                                                                            );
                    if ( DATA_ID_VOID_ID == data_relationship_get_to_feature_id( &out_relation ) )
                    {
                        data_feature_init_empty( &((*this_).temp_features[1]) );
                    }
                    else
                    {
                        read_error |= data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                               data_relationship_get_to_feature_id( &out_relation ),
                                                                               &((*this_).temp_features[1])
                                                                             );
                    }

                    /* serialize */
                    if ( read_error == DATA_ERROR_NONE )
                    {
                        serialize_error |= json_serializer_append_relationship( &serializer,
                                                                                &out_relation,
                                                                                &from_classifier,
                                                                                &((*this_).temp_features[0]),
                                                                                &to_classifier,
                                                                                &((*this_).temp_features[1]),
                                                                                (*this_).clipboard_stringbuf
                                                                              );
                    }
                    else
                    {
                        /* program internal error */
                        TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all features of the classifier of the set." );
                    }
                }
                else
                {
                    /* program internal error */
                    TSLOG_ERROR( "gui_toolbox_private_copy_set_to_clipboard could not read all data of the set." );
                }
            }
            break;

            default:
            break;
        }
    }

    serialize_error |= json_serializer_end_set( &serializer, (*this_).clipboard_stringbuf );

    if ( serialize_error == DATA_ERROR_NONE )
    {
        gtk_clipboard_set_text ( (*this_).the_clipboard, utf8stringbuf_get_string( (*this_).clipboard_stringbuf ), -1 );
    }
    else
    {
        /* error to be shown to the user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                               );
    }
    TRACE_INFO( utf8stringbuf_get_string( (*this_).clipboard_stringbuf ) );

    json_serializer_destroy( &serializer );

    TRACE_END();
}

void gui_serializer_deserializer_request_clipboard_text( gui_serializer_deserializer_t *this_, int64_t destination_diagram_id )
{
    TRACE_BEGIN();

    utf8stringbuf_clear( (*this_).clipboard_stringbuf );

    (*this_).destination_diagram_id = destination_diagram_id;

    /* this more complicated call (compared to gtk_clipboard_wait_for_text) avoids recursive calls of the gdk main loop */
    gtk_clipboard_request_text ( (*this_).the_clipboard, (GtkClipboardTextReceivedFunc) gui_serializer_deserializer_clipboard_text_received_callback, this_ );

    TRACE_END();
}

void gui_serializer_deserializer_clipboard_text_received_callback ( GtkClipboard *clipboard, const gchar *clipboard_text, gpointer data )
{
    TRACE_BEGIN();
    gui_serializer_deserializer_t *this_ = data;

    if ( clipboard_text != NULL )
    {
        gui_serializer_deserializer_private_copy_clipboard_to_db( this_, clipboard_text );
    }
    else
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA
                                               );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_serializer_deserializer_private_copy_clipboard_to_db( gui_serializer_deserializer_t *this_, const char *json_text )
{
    TRACE_BEGIN();

    data_error_t diag_check_error;
    static data_diagram_t test_diagram;
    diag_check_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader, (*this_).destination_diagram_id, &test_diagram );
    if ( DATA_ERROR_NONE == diag_check_error )
    {
        TRACE_INFO_INT ( "(*this_).destination_diagram_id:", (*this_).destination_diagram_id );
        gui_serializer_deserializer_private_copy_clipboard_to_diagram( this_, json_text, (*this_).destination_diagram_id );
    }
    else
    {
        TSLOG_WARNING_INT ( "(*this_).destination_diagram_id is invalid:", (*this_).destination_diagram_id );
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
                                               );
    }

    TRACE_END();
}

void gui_serializer_deserializer_private_copy_clipboard_to_diagram( gui_serializer_deserializer_t *this_, const char *json_text, int64_t diagram_id )
{
    TRACE_BEGIN();
    json_deserializer_t deserializer;
    data_error_t parse_error = DATA_ERROR_NONE;

    TRACE_INFO ( json_text );

    json_deserializer_init( &deserializer, json_text );
    int64_t current_diagram_id = diagram_id;

    parse_error = json_deserializer_expect_begin_set( &deserializer );

    if ( DATA_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        bool any_error = false;  /* consolidation of parser errors and writer errors */
        bool is_first = true;  /* is this the first object in the database that is created? if false, database changes are appended to the last undo_redo_set */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* do not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( ! any_error ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = json_deserializer_get_type_of_next_element( &deserializer, &next_object_type );
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
                        parse_error = json_deserializer_get_next_classifier ( &deserializer,
                                                                              &new_classifier,
                                                                              GUI_SERIALIZER_DESERIALIZER_MAX_FEATURES,
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
                            int64_t the_classifier_id;
                            {
                                /* check if the parsed classifier already exists in this database; if not, create it */
                                ctrl_classifier_controller_t *classifier_ctrl;
                                classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

                                data_error_t read_error;
                                data_classifier_t existing_classifier;
                                read_error = data_database_reader_get_classifier_by_name ( (*this_).db_reader,
                                                                                           data_classifier_get_name_ptr( &new_classifier ),
                                                                                           &existing_classifier
                                                                                         );
                                /* if the name is equal, expect the objects to be equal */
                                const bool classifier_exists = ( DATA_ERROR_NONE == read_error );

                                if ( ! classifier_exists )
                                {
                                    ctrl_error_t write_error;
                                    write_error = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                                                &new_classifier,
                                                                                                ! is_first,
                                                                                                &the_classifier_id
                                                                                              );
                                    if ( CTRL_ERROR_NONE == write_error )
                                    {
                                        /* create also the features */
                                        for ( int f_index = 0; f_index < feature_count; f_index ++ )
                                        {
                                            data_feature_t *current_feature = &((*this_).temp_features[f_index]);
                                            int64_t new_feature_id;
                                            data_feature_set_classifier_id( current_feature, the_classifier_id );
                                            /* filter lifelines */
                                            /* TODO , remember to adhere to all (*this_).data_rules */
                                            if ( data_feature_get_main_type(current_feature) != DATA_FEATURE_TYPE_LIFELINE )
                                            {
                                                write_error |= ctrl_classifier_controller_create_feature ( classifier_ctrl,
                                                                                                           current_feature,
                                                                                                           true, /* = bool add_to_latest_undo_set */
                                                                                                           &new_feature_id
                                                                                                         );
                                            }
                                            else
                                            {
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
                                    the_classifier_id = data_classifier_get_id( &existing_classifier );
                                }
                            }

                            if ( ! any_error )  /* no error */
                            {
                                /* link the classifier to the current diagram */
                                ctrl_diagram_controller_t *diag_ctrl;
                                diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                                ctrl_error_t write_error2;
                                int64_t new_element_id;
                                data_diagramelement_t diag_ele;
                                data_diagramelement_init_new ( &diag_ele,
                                                               current_diagram_id,
                                                               the_classifier_id,
                                                               DATA_DIAGRAMELEMENT_FLAG_NONE,
                                                               DATA_ID_VOID_ID
                                                             );
                                write_error2 = ctrl_diagram_controller_create_diagramelement ( diag_ctrl,
                                                                                               &diag_ele,
                                                                                               ! is_first,
                                                                                               &new_element_id
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

                    case DATA_TABLE_DIAGRAM:
                    {
                        data_diagram_t new_diagram;
                        parse_error = json_deserializer_get_next_diagram ( &deserializer, &new_diagram );
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
                            int64_t new_diag_id;
                            data_diagram_set_parent_id( &new_diagram, diagram_id );
                            write_error3 = ctrl_diagram_controller_create_diagram ( diag_ctrl,
                                                                                    &new_diagram,
                                                                                    ! is_first,
                                                                                    &new_diag_id
                                                                                  );
                            if ( CTRL_ERROR_NONE != write_error3 )
                            {
                                TSLOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagram" );
                                any_error = true;
                            }
                            else
                            {
                                /* insert all consecutive elements to this new diagram */
                                current_diagram_id = new_diag_id;
                            }
                            is_first = false;
                        }
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
                        parse_error = json_deserializer_get_next_relationship ( &deserializer,
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
                            int64_t from_classifier_id = DATA_ID_VOID_ID;
                            int64_t from_feature_id = DATA_ID_VOID_ID;
                            int64_t to_classifier_id = DATA_ID_VOID_ID;
                            int64_t to_feature_id = DATA_ID_VOID_ID;

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
                                    from_classifier_id = data_classifier_get_id( &found_classifier );
                                    TRACE_INFO_STR( "id found for src classifier:", utf8stringbuf_get_string( rel_from_clas ) );

                                    /* search source feature id */
                                    if ( data_relationship_get_from_feature_id( &new_relationship ) != DATA_ID_VOID_ID )
                                    {
                                        uint32_t feature_count;
                                        read_error2 = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                                           from_classifier_id,
                                                                                                           GUI_SERIALIZER_DESERIALIZER_MAX_FEATURES,
                                                                                                           &((*this_).temp_features),
                                                                                                           &feature_count
                                                                                                         );
                                        if ( DATA_ERROR_NONE == read_error2 )
                                        {
                                            data_feature_t *best_match = NULL;  /* to compare in case multiple features match to the given name */
                                            for ( int src_idx=0; src_idx < feature_count; src_idx ++ )
                                            {
                                                data_feature_t *current_feature = &((*this_).temp_features[src_idx]);
                                                if ( utf8stringbuf_equals_str( rel_from_feat,
                                                                               data_feature_get_key_ptr( current_feature ) ) )
                                                {
                                                    bool better_match = ( best_match == NULL );
                                                    if ( data_rules_feature_is_scenario_cond( &((*this_).data_rules),
                                                                                              data_feature_get_main_type( current_feature ) ) )
                                                    {
                                                        better_match = false;  /* lifelines are no good match */
                                                        /* TODO lifeline would be ok if fitting to current diagram */
                                                    }
                                                    if ( better_match )
                                                    {
                                                        from_feature_id = data_feature_get_id( current_feature );
                                                        best_match = current_feature;
                                                        TRACE_INFO_STR( "id found for src feature:", utf8stringbuf_get_string( rel_from_feat ) );
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
                                    to_classifier_id = data_classifier_get_id( &found_classifier );
                                    TRACE_INFO_STR( "id found for dst classifier:", utf8stringbuf_get_string( rel_to_clas ) );

                                    /* search destination feature id */
                                    if ( data_relationship_get_to_feature_id( &new_relationship ) != DATA_ID_VOID_ID )
                                    {
                                        uint32_t feature_count;
                                        read_error2 = data_database_reader_get_features_by_classifier_id ( (*this_).db_reader,
                                                                                                           to_classifier_id,
                                                                                                           GUI_SERIALIZER_DESERIALIZER_MAX_FEATURES,
                                                                                                           &((*this_).temp_features),
                                                                                                           &feature_count
                                                                                                         );
                                        if ( DATA_ERROR_NONE == read_error2 )
                                        {
                                            data_feature_t *best_match = NULL;  /* to compare in case multiple features match to the given name */
                                            for ( int src_idx=0; src_idx < feature_count; src_idx ++ )
                                            {
                                                data_feature_t *current_feature = &((*this_).temp_features[src_idx]);
                                                if ( utf8stringbuf_equals_str( rel_to_feat,
                                                                               data_feature_get_key_ptr( current_feature ) ) )
                                                {
                                                    bool better_match = ( best_match == NULL );
                                                    if ( data_rules_feature_is_scenario_cond( &((*this_).data_rules),
                                                                                              data_feature_get_main_type( current_feature ) ) )
                                                    {
                                                        better_match = false;  /* lifelines are no good match */
                                                        /* TODO lifeline would be ok if fitting to current diagram */
                                                    }
                                                    if ( better_match )
                                                    {
                                                        to_feature_id = data_feature_get_id( current_feature );
                                                        best_match = current_feature;
                                                        TRACE_INFO_STR( "id found for dst feature:", utf8stringbuf_get_string( rel_to_feat ) );
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            /* create relationship if not yet existing */
                            if ( from_classifier_id == DATA_ID_VOID_ID )
                            {
                                TSLOG_ERROR_STR( "A relationship could not be created because the source classifier could not be found.",
                                                 utf8stringbuf_get_string( rel_from_clas )
                                               );
                            }
                            else if ( to_classifier_id == DATA_ID_VOID_ID )
                            {
                                TSLOG_ERROR_STR( "A relationship could not be created because the destination classifier could not be found.",
                                                 utf8stringbuf_get_string( rel_to_clas )
                                               );
                            }
                            else
                            {
                                ctrl_error_t write_error4;

                                /* get classifier controller */
                                ctrl_classifier_controller_t *classifier_control4;
                                classifier_control4 = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                                /* update the json-parsed relationship struct */
                                data_relationship_set_id ( &new_relationship, DATA_ID_VOID_ID );
                                data_relationship_set_from_classifier_id ( &new_relationship, from_classifier_id );
                                data_relationship_set_from_feature_id ( &new_relationship, from_feature_id );
                                data_relationship_set_to_classifier_id ( &new_relationship, to_classifier_id );
                                data_relationship_set_to_feature_id ( &new_relationship, to_feature_id );

                                /* check preconditions */
                                const bool diagram_ok = true;
                                /* TODO , remember to adhere to (*this_).data_rules */
                                /*
                                const bool is_scenario = data_rules_diagram_is_scenario ( &((*this_).data_rules), diag_type )
                                                        && (( from_feature_id != DATA_ID_VOID_ID )||( to_feature_id != DATA_ID_VOID_ID ));
                                const bool diagram_ok = is_scenario
                                                        ? data_rules_diagram_shows_scenario_relationships ( &((*this_).data_rules), diag_type )
                                                        : data_rules_diagram_shows_uncond_relationships ( &((*this_).data_rules), diag_type );
                                */
                                if ( diagram_ok ) {
                                    /* create relationship */
                                    int64_t relationship_id;
                                    write_error4 = ctrl_classifier_controller_create_relationship ( classifier_control4,
                                                                                                    &new_relationship,
                                                                                                    ! is_first, /*=add_to_latest_undo_set*/
                                                                                                    &relationship_id
                                                                                                  );
                                    if ( CTRL_ERROR_NONE != write_error4 )
                                    {
                                        TSLOG_ERROR( "unexpected error at ctrl_classifier_controller_create_relationship" );
                                        any_error = true;
                                    }
                                    is_first = false;
                                }
                                else
                                {
                                    TSLOG_WARNING( "1 relationship is not allowed in target diagram type" );
                                }
                            }
                            /* cleanup */
                            data_relationship_destroy ( &new_relationship );
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected error" );
                        any_error = true;
                    }
                }
            }
            else
            {
                /* parser error, break loop: */
                any_error = true;
            }
        }
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_end_set( &deserializer );
    }

    json_deserializer_destroy( &deserializer );

    if ( DATA_ERROR_NONE != parse_error )
    {
        uint32_t read_pos;
        json_deserializer_get_read_pos ( &deserializer, &read_pos );
        gui_simple_message_to_user_show_message_with_int ( (*this_).message_to_user,
                                                           GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                           GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA,
                                                           read_pos
                                                         );
    }

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke

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
