/* File: io_import_elements.c; Copyright and License: see below */

#include "io_import_elements.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void io_import_elements_init( io_import_elements_t *this_,
                              data_database_reader_t *db_reader,
                              ctrl_controller_t *controller,
                              data_stat_t *io_stat,
                              utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != controller );
    assert( NULL != io_stat );
    assert( NULL != out_english_report );

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).stat = io_stat;
    (*this_).english_report = out_english_report;

    (*this_).mode = IO_IMPORT_MODE_CHECK;
    (*this_).paste_to_diagram = DATA_ROW_ID_VOID;

    ctrl_multi_step_changer_init( &((*this_).multi_step_changer), controller, db_reader );

    data_rules_init ( &((*this_).data_rules) );

    /* get the id of the root diagram if there is one */
    (*this_).root_diagram = DATA_ROW_ID_VOID;
    {
        data_small_set_t roots;
        data_small_set_init( &roots );
        const u8_error_t read_error
            = data_database_reader_get_diagram_ids_by_parent_id( (*this_).db_reader,
                                                                 DATA_ROW_ID_VOID,
                                                                 &roots
                                                               );
        if ( read_error != U8_ERROR_NONE )
        {
            U8_LOG_ERROR_HEX( "error at reading root", read_error );
        }
        else
        {
            if ( 1 <= data_small_set_get_count( &roots ) )
            {
                data_id_t first_root = data_small_set_get_id( &roots, 0 );
                (*this_).root_diagram = data_id_get_row_id( &first_root );
            }
            else
            {
                U8_LOG_EVENT( "importing to empty database" );
            }
        }
        data_small_set_destroy( &roots );
    }

    U8_TRACE_END();
}

void io_import_elements_init_for_paste( io_import_elements_t *this_,
                                        data_row_id_t paste_to_diagram,
                                        data_database_reader_t *db_reader,
                                        ctrl_controller_t *controller,
                                        data_stat_t *io_stat,
                                        utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();

    io_import_elements_init( this_, db_reader, controller, io_stat, out_english_report );
    (*this_).mode = IO_IMPORT_MODE_PASTE;

    /* check if diagram id exists */
    {
        data_diagram_init_empty( &((*this_).temp_diagram) );
        const u8_error_t read_error1
            = data_database_reader_get_diagram_by_id( (*this_).db_reader,
                                                      paste_to_diagram,
                                                      &((*this_).temp_diagram)
                                                    );
        if ( U8_ERROR_NONE != read_error1 )
        {
            U8_LOG_ERROR_INT( "diagram id where to import json data to does not exist (anymore)", (*this_).paste_to_diagram );
            (*this_).paste_to_diagram = DATA_ROW_ID_VOID;
        }
        else
        {
            (*this_).paste_to_diagram = paste_to_diagram;
        }
        data_diagram_destroy( &((*this_).temp_diagram) );
    }

    U8_TRACE_END();
}

void io_import_elements_destroy( io_import_elements_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( NULL != (*this_).db_reader );
    assert( NULL != (*this_).controller );
    assert( NULL != (*this_).stat );
    assert( NULL != (*this_).english_report );

    data_rules_destroy ( &((*this_).data_rules) );

    ctrl_multi_step_changer_destroy( &((*this_).multi_step_changer) );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).stat = NULL;
    (*this_).english_report = NULL;

    U8_TRACE_END();
}

void io_import_elements_set_mode( io_import_elements_t *this_, io_import_mode_t mode )
{
    U8_TRACE_BEGIN();

    (*this_).mode = mode;

    U8_TRACE_END();
}

u8_error_t io_import_elements_sync_diagram( io_import_elements_t *this_,
                                            const data_diagram_t *diagram_ptr,
                                            const char *parent_uuid )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagram_ptr );
     /* parent_uuid is NULL if root diagram */
    u8_error_t sync_error = U8_ERROR_NONE;

    /* ANY MODE: determine parent id */
    data_row_id_t parent_row_id = (*this_).root_diagram;
    const bool parent_uuid_specified
        = (( parent_uuid != NULL )&&( ! utf8string_equals_str( parent_uuid, "" )));
    if ( parent_uuid_specified )
    {
        data_diagram_init_empty( &((*this_).temp_diagram ) );
        const u8_error_t read_error1
            = data_database_reader_get_diagram_by_uuid( (*this_).db_reader,
                                                        parent_uuid,
                                                        &((*this_).temp_diagram)
                                                        );
        if ( read_error1 == U8_ERROR_NOT_FOUND )
        {
            U8_TRACE_INFO_STR( "no parent found, uuid:", parent_uuid );
        }
        else if ( read_error1 != U8_ERROR_NONE )
        {
            U8_TRACE_INFO_STR( "error at searching for parent diagram:", parent_uuid );
        }
        else
        {
            parent_row_id = data_diagram_get_row_id( &((*this_).temp_diagram ) );
        }
        data_diagram_destroy( &((*this_).temp_diagram ) );
    }

    /* update default parent diagram id */
    if (( (*this_).mode == IO_IMPORT_MODE_PASTE )&&( sync_error == U8_ERROR_NONE ))
    {
        if ( (*this_).paste_to_diagram == DATA_ROW_ID_VOID )
        {
            data_row_id_t parent_row_id = (*this_).root_diagram;
            U8_TRACE_INFO_INT( "in paste-clipboard mode, missing parent diagram set to", parent_row_id );
        }
        else
        {
            /* overwrite the parent diagram id, do not keep the one provided via the clipboard */
            parent_row_id = (*this_).paste_to_diagram;
        }
    }

    /* if PASTE */
    if (( (*this_).mode == IO_IMPORT_MODE_PASTE )&&( sync_error == U8_ERROR_NONE ))
    {
        data_diagram_copy( &((*this_).temp_diagram), diagram_ptr );
        data_diagram_set_parent_row_id( &((*this_).temp_diagram), parent_row_id );
        /* create new uuid for diagram if paste */
        {
            data_uuid_t new_uuid;
            data_uuid_init_new( &new_uuid );
            data_diagram_set_uuid( &((*this_).temp_diagram), data_uuid_get_string( &new_uuid ) );
            data_uuid_destroy( &new_uuid );
        }

        /* create the parsed diagram as child below the current diagram */
        u8_error_t modified_info;
        sync_error = ctrl_multi_step_changer_create_diagram( &((*this_).multi_step_changer),
                                                             &((*this_).temp_diagram),
                                                             &modified_info
                                                           );
        data_stat_inc_count( (*this_).stat,
                             DATA_STAT_TABLE_DIAGRAM,
                             (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                           );
        if ( U8_ERROR_NONE != sync_error )
        {
            U8_LOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagram" );
        }
        else
        {
            /* insert all consecutive elements to this new diagram */
            (*this_).paste_to_diagram = data_diagram_get_row_id( &((*this_).temp_diagram) );
            /* this new diagram is root if it is the first diagram */
            if ( (*this_).root_diagram == DATA_ROW_ID_VOID )
            {
                (*this_).root_diagram = data_diagram_get_row_id( &((*this_).temp_diagram) );
            }
        }
        data_diagram_destroy( &((*this_).temp_diagram) );
    }

    /* if CREATE/LINK */
    if ((( (*this_).mode == IO_IMPORT_MODE_CREATE )||( (*this_).mode == IO_IMPORT_MODE_LINK ))
        &&( sync_error == U8_ERROR_NONE ))
    {
        /* check if the parsed diagram already exists in this database; if not, create it */
        data_diagram_init_empty( &((*this_).temp_diagram) );
        u8_error_t read_error;
        read_error = data_database_reader_get_diagram_by_uuid( (*this_).db_reader,
                                                               data_diagram_get_uuid_const( diagram_ptr ),
                                                               &((*this_).temp_diagram)
                                                             );
        const bool diagram_exists = ( U8_ERROR_NONE == read_error );

        if ( diagram_exists )
        {
            if ( (*this_).mode == IO_IMPORT_MODE_LINK )
            {
                /* if (*this_).temp_diagram is the only valid root, set parent_row_id to DATA_ROW_ID_VOID */
                if ( data_diagram_get_row_id( &((*this_).temp_diagram) ) == (*this_).root_diagram )
                {
                    parent_row_id = DATA_ROW_ID_VOID;
                }

                /* update the parent if wrong parent stored */
                if ( data_diagram_get_parent_row_id( &((*this_).temp_diagram) ) != parent_row_id )
                {
                    sync_error = ctrl_multi_step_changer_update_diagram_parent_id( &((*this_).multi_step_changer),
                                                                                   data_diagram_get_row_id( &((*this_).temp_diagram) ),
                                                                                   parent_row_id
                                                                                 );

                    /* update root diag if this is root */
                    if (( parent_row_id == DATA_ROW_ID_VOID )&&( sync_error == U8_ERROR_NONE ))
                    {
                        (*this_).root_diagram = data_diagram_get_row_id( &((*this_).temp_diagram) );
                    }
                }
            }
            else
            {
                /* do the statistics */
                data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED );
            }
        }
        else
        {
            data_diagram_replace( &((*this_).temp_diagram), diagram_ptr );
            data_diagram_set_parent_row_id( &((*this_).temp_diagram), parent_row_id );

            /* create the parsed diagram as child below the current diagram */
            u8_error_t modified_info;
            sync_error = ctrl_multi_step_changer_create_diagram( &((*this_).multi_step_changer),
                                                                 &((*this_).temp_diagram),
                                                                 &modified_info
                                                               );
            data_stat_inc_count( (*this_).stat,
                                 DATA_STAT_TABLE_DIAGRAM,
                                 (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                               );
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_ID ) )
            {
                /* warn on changed diagram ids. This is important because links in description texts may be affected. */
                data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_WARNING );
            }

            if ( U8_ERROR_NONE != sync_error )
            {
                U8_LOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagram" );
            }
            else
            {
                /* this new diagram is root if it is the first root diagram, further diagrams will not be root */
                if (( (*this_).root_diagram == DATA_ROW_ID_VOID )&&( parent_row_id == DATA_ROW_ID_VOID )
                    &&( ! parent_uuid_specified ))
                {
                    (*this_).root_diagram = data_diagram_get_row_id( &((*this_).temp_diagram) );
                }
            }

            /* write report in case of anomalies */
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_ID ) )
            {
                io_import_elements_private_report_id_differs( this_,
                                                              data_diagram_get_data_id( diagram_ptr ),
                                                              data_diagram_get_data_id( &((*this_).temp_diagram ) )
                                                            );
            }
        }
        data_diagram_destroy( &((*this_).temp_diagram) );
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t io_import_elements_sync_diagramelement( io_import_elements_t *this_,
                                                   const data_diagramelement_t *diagramelement_ptr,
                                                   const char *diagram_uuid,
                                                   const char *node_uuid )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagramelement_ptr );
    assert( NULL != diagram_uuid );
    assert( NULL != node_uuid );
    u8_error_t sync_error = U8_ERROR_NONE;

    /* ANY MODE: determine classifier/feature id */
    data_row_id_t node_classifier_id = DATA_ROW_ID_VOID;
    data_row_id_t node_feature_id = DATA_ROW_ID_VOID;
    data_feature_type_t node_feature_type = DATA_FEATURE_TYPE_VOID;
    if ( node_uuid != NULL )
    {
        if ( ! utf8string_equals_str( node_uuid, "" ) )
        {
            /* search source classifier id */
            data_classifier_init_empty( &((*this_).temp_classifier ) );
            const u8_error_t read_error1
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                               node_uuid,
                                                               &((*this_).temp_classifier)
                                                             );
            if ( U8_ERROR_NONE == read_error1 )
            {
                node_classifier_id = data_classifier_get_row_id( &((*this_).temp_classifier) );
                U8_TRACE_INFO_STR( "id found for classifier:", node_uuid );
            }
            else
            {
                /* search source feature id */
                data_feature_init_empty( &((*this_).temp_feature) );
                const u8_error_t read_error2
                    = data_database_reader_get_feature_by_uuid( (*this_).db_reader,
                                                                node_uuid,
                                                                &((*this_).temp_feature)
                                                              );
                if ( U8_ERROR_NONE == read_error2 )
                {
                    node_classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                    node_feature_id = data_feature_get_row_id( &((*this_).temp_feature) );
                    node_feature_type = data_feature_get_main_type( &((*this_).temp_feature) );
                    U8_TRACE_INFO_STR( "id found for feature:", node_uuid );
                }
                else
                {
                    U8_TRACE_INFO_STR( "diagramelement node not found", node_uuid );
                }
                data_feature_destroy( &((*this_).temp_feature) );
            }
            data_classifier_destroy( &((*this_).temp_classifier ) );
        }
    }

    /* ANY MODE: determine diagram id */
    data_row_id_t diagram_row_id = DATA_ROW_ID_VOID;
    if (( diagram_uuid != NULL )&&( sync_error == U8_ERROR_NONE ))
    {
        if ( ! utf8string_equals_str( diagram_uuid, "" ) )
        {
            data_diagram_init_empty( &((*this_).temp_diagram ) );
            const u8_error_t read_error3
                = data_database_reader_get_diagram_by_uuid( (*this_).db_reader,
                                                            diagram_uuid,
                                                            &((*this_).temp_diagram)
                                                          );
            if ( read_error3 == U8_ERROR_NOT_FOUND )
            {
                U8_TRACE_INFO_STR( "no diagram found, uuid:", diagram_uuid );
            }
            else if ( read_error3 != U8_ERROR_NONE )
            {
                U8_TRACE_INFO_STR( "diagram not found:", diagram_uuid );
            }
            else
            {
                diagram_row_id = data_diagram_get_row_id( &((*this_).temp_diagram ) );
            }
            data_diagram_destroy( &((*this_).temp_diagram ) );
        }
    }

    /* check preconditions */
    if ( (*this_).mode == IO_IMPORT_MODE_LINK )
    {
        if ( node_classifier_id == DATA_ROW_ID_VOID )
        {
            sync_error |= U8_ERROR_VALUE_OUT_OF_RANGE;
            U8_LOG_ERROR( "diagramelement references a non-existing classifier." );
        }
        if (( node_feature_id != DATA_ROW_ID_VOID )&&( node_feature_type != DATA_FEATURE_TYPE_LIFELINE ))
        {
            sync_error |= U8_ERROR_VALUE_OUT_OF_RANGE;
            U8_LOG_ERROR( "diagramelement references a feature which is not of type DATA_FEATURE_TYPE_LIFELINE." );
        }
        if ( diagram_row_id == DATA_ROW_ID_VOID )
        {
            sync_error |= U8_ERROR_VALUE_OUT_OF_RANGE;
            U8_LOG_ERROR( "diagramelement references a non-existing classifier." );
        }
    }

    if (( (*this_).mode == IO_IMPORT_MODE_LINK )&&( sync_error == U8_ERROR_NONE ))
    {
        /* check if the parsed diagramelement already exists in this database; if not, create it */
        data_diagramelement_init_empty( &((*this_).temp_diagramelement ) );
        const u8_error_t read_error4
            = data_database_reader_get_diagramelement_by_uuid( (*this_).db_reader,
                                                               data_diagramelement_get_uuid_const( diagramelement_ptr ),
                                                               &((*this_).temp_diagramelement)
                                                             );
        const bool diagramelement_exists = ( U8_ERROR_NONE == read_error4 );

        if ( diagramelement_exists )
        {
            /* do the statistics */
            data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_IGNORED );
            U8_TRACE_INFO_INT( "diagramelement did already exist:", data_diagramelement_get_row_id( &((*this_).temp_diagramelement) ) );
        }
        else
        {
            /* link the classifier to the current diagram */
            sync_error = data_diagramelement_reinit( &((*this_).temp_diagramelement),
                                                     data_diagramelement_get_row_id( diagramelement_ptr ),
                                                     diagram_row_id,
                                                     node_classifier_id,
                                                     data_diagramelement_get_display_flags( diagramelement_ptr ),
                                                     node_feature_id,  /* focused_feature_id */
                                                     data_diagramelement_get_uuid_const( diagramelement_ptr )
                                                   );
            u8_error_t modified_info;
            sync_error |= ctrl_multi_step_changer_create_diagramelement( &((*this_).multi_step_changer),
                                                                         &((*this_).temp_diagramelement),
                                                                         &modified_info
                                                                       );
            data_stat_inc_count( (*this_).stat,
                                 DATA_STAT_TABLE_DIAGRAMELEMENT,
                                 (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                               );
            if ( U8_ERROR_NONE != sync_error )
            {
                U8_LOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagramelement" );
            }

            /* write report in case of anomalies */
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_ID ) )
            {
                io_import_elements_private_report_id_differs( this_,
                                                              data_diagramelement_get_data_id( diagramelement_ptr ),
                                                              data_diagramelement_get_data_id( &((*this_).temp_diagramelement ) )
                                                            );
            }
        }
        data_diagramelement_destroy( &((*this_).temp_diagramelement ) );
    }

    if ( (*this_).mode == IO_IMPORT_MODE_PASTE )
    {
        /* in paste mode, ignore diagramelements */
        data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_IGNORED );
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t io_import_elements_private_create_diagramelement( io_import_elements_t *this_, data_row_id_t classifier_id )
{
    U8_TRACE_BEGIN();
    assert( DATA_ROW_ID_VOID != classifier_id );
    u8_error_t sync_error = U8_ERROR_NONE;

    if ( (*this_).mode == IO_IMPORT_MODE_PASTE )
    {
        /* link the classifier to the current diagram */
        data_diagramelement_init_new( &((*this_).temp_diagramelement ),
                                      (*this_).paste_to_diagram,
                                      classifier_id,
                                      DATA_DIAGRAMELEMENT_FLAG_NONE,
                                      DATA_ROW_ID_VOID
                                    );
        u8_error_t modified_info;
        sync_error = ctrl_multi_step_changer_create_diagramelement( &((*this_).multi_step_changer),
                                                                    &((*this_).temp_diagramelement ),
                                                                    &modified_info
                                                                  );
        data_stat_inc_count( (*this_).stat,
                             DATA_STAT_TABLE_DIAGRAMELEMENT,
                             (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                           );
        if ( U8_ERROR_NONE != sync_error )
        {
            U8_LOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagramelement" );
        }

        data_diagramelement_destroy( &((*this_).temp_diagramelement ) );
    }
    else
    {
        U8_TRACE_INFO_INT( "no diagramelement created for new classifier:", classifier_id );
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t io_import_elements_sync_classifier( io_import_elements_t *this_,
                                               const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert( NULL != classifier_ptr );
    u8_error_t sync_error = U8_ERROR_NONE;

    if ( (*this_).mode == IO_IMPORT_MODE_PASTE )
    {
        if ( (*this_).paste_to_diagram == DATA_ROW_ID_VOID )
        {
            sync_error = U8_ERROR_FOCUS_EMPTY;
            U8_TRACE_INFO( "in paste-clipboard mode, parent diagram must be valid" );
        }
    }

    if ((( (*this_).mode == IO_IMPORT_MODE_CREATE )||( (*this_).mode == IO_IMPORT_MODE_PASTE ))
        &&( sync_error == U8_ERROR_NONE ))
    {
        /* check if the parsed classifier already exists in this database; if not, create it */
        data_classifier_init_empty( &((*this_).temp_classifier ) );
        u8_error_t read_error;
        read_error = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                                  data_classifier_get_uuid_const( classifier_ptr ),
                                                                  &((*this_).temp_classifier)
                                                                );
        const bool classifier_exists = ( U8_ERROR_NONE == read_error );

        if ( classifier_exists )
        {
            /* do the statistics */
            data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_IGNORED );
            U8_TRACE_INFO_INT( "classifier did already exist:", data_classifier_get_row_id( &((*this_).temp_classifier) ) );
        }
        else
        {
            data_classifier_replace( &((*this_).temp_classifier ), classifier_ptr );

            u8_error_t modified_info;
            sync_error = ctrl_multi_step_changer_create_classifier( &((*this_).multi_step_changer),
                                                                    &((*this_).temp_classifier ),
                                                                    &modified_info
                                                                  );
            data_stat_inc_count( (*this_).stat,
                                 DATA_STAT_TABLE_CLASSIFIER,
                                 (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                               );
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_NAME ) )
            {
                /* warn on changed classifier names. */
                data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING );
            }

            if ( U8_ERROR_NONE != sync_error )
            {
                U8_LOG_ERROR( "unexpected error at ctrl_classifier_controller_create_classifier/feature" );
            }

            /* write report in case of anomalies */
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_ID ) )
            {
                io_import_elements_private_report_id_differs( this_,
                                                              data_classifier_get_data_id( classifier_ptr ),
                                                              data_classifier_get_data_id( &((*this_).temp_classifier ) )
                                                            );
            }
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_NAME ) )
            {
                io_import_elements_private_report_name_differs( this_,
                                                                data_classifier_get_name_const( classifier_ptr ),
                                                                data_classifier_get_name_const( &((*this_).temp_classifier ) )
                                                              );
            }
        }

        if (( (*this_).mode == IO_IMPORT_MODE_PASTE )&&( sync_error == U8_ERROR_NONE ))
        {
            /* in paste mode, create a diagramelement in the current diagram */
            const data_row_id_t classifier_row_id = data_classifier_get_row_id( &((*this_).temp_classifier ) );
            sync_error = io_import_elements_private_create_diagramelement( this_, classifier_row_id );
        }

        data_classifier_destroy( &((*this_).temp_classifier ) );
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t io_import_elements_sync_feature( io_import_elements_t *this_,
                                            const data_feature_t *feature_ptr,
                                            const char *classifier_uuid )
{
    U8_TRACE_BEGIN();
    assert( NULL != feature_ptr );
    assert( NULL != classifier_uuid );
    u8_error_t sync_error = U8_ERROR_NONE;

    /* ANY MODE: determine classifier id */
    data_row_id_t classifier_row_id = DATA_ROW_ID_VOID;
    if ( classifier_uuid != NULL )
    {
        if ( ! utf8string_equals_str( classifier_uuid, "" ) )
        {
            data_classifier_init_empty( &((*this_).temp_classifier ) );
            const u8_error_t read_error1
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                                      classifier_uuid,
                                                                      &((*this_).temp_classifier)
                                                                    );
            if ( read_error1 == U8_ERROR_NOT_FOUND )
            {
                U8_TRACE_INFO_STR( "no classifier found, uuid:", classifier_uuid );
            }
            else if ( read_error1 != U8_ERROR_NONE )
            {
                U8_TRACE_INFO_STR( "parent classifier not found:", classifier_uuid );
            }
            else
            {
                classifier_row_id = data_classifier_get_row_id( &((*this_).temp_classifier ) );
            }
            data_classifier_destroy( &((*this_).temp_classifier ) );
        }
    }

    /* check preconditions */
    if (( (*this_).mode == IO_IMPORT_MODE_CREATE )||( (*this_).mode == IO_IMPORT_MODE_PASTE ))
    {
        if ( classifier_row_id == DATA_ROW_ID_VOID )
        {
            sync_error |= U8_ERROR_VALUE_OUT_OF_RANGE;
            U8_LOG_ERROR( "feature references a non-existing classifier." );
        }
    }

    if ((( (*this_).mode == IO_IMPORT_MODE_CREATE )||( (*this_).mode == IO_IMPORT_MODE_PASTE ))
        &&( sync_error == U8_ERROR_NONE ))
    {
        /* check if the parsed feature already exists in this database; if not, create it */
        data_feature_init_empty( &((*this_).temp_feature ) );
        u8_error_t read_error;
        read_error = data_database_reader_get_feature_by_uuid( (*this_).db_reader,
                                                               data_feature_get_uuid_const( feature_ptr ),
                                                               &((*this_).temp_feature)
                                                             );
        data_feature_destroy( &((*this_).temp_feature ) );
        const bool feature_exists = ( U8_ERROR_NONE == read_error );

        if ( feature_exists )
        {
            /* update statistics */
            const data_feature_type_t feat_type = data_feature_get_main_type( &((*this_).temp_feature) );
            const data_stat_table_t feat_or_lifeline
                = ( feat_type == DATA_FEATURE_TYPE_LIFELINE ) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;
            data_stat_inc_count( (*this_).stat, feat_or_lifeline, DATA_STAT_SERIES_IGNORED );
        }
        else
        {
            /* filter lifelines */
            const bool is_lifeline
                = data_rules_feature_is_scenario_cond( &((*this_).data_rules), data_feature_get_main_type( feature_ptr ) );
            if (( (*this_).mode == IO_IMPORT_MODE_CREATE )||( ! is_lifeline ))
            {
                /* create feature */
                data_feature_copy( &((*this_).temp_feature ), feature_ptr );
                data_feature_set_classifier_row_id( &((*this_).temp_feature ), classifier_row_id );

                u8_error_t modified_info;
                sync_error = ctrl_multi_step_changer_create_feature( &((*this_).multi_step_changer),
                                                                     &((*this_).temp_feature ),
                                                                     &modified_info
                                                                   );

                data_stat_inc_count( (*this_).stat,
                                     is_lifeline ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE,
                                     (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                                   );
                if ( U8_ERROR_NONE != sync_error )
                {
                    U8_LOG_ERROR( "unexpected error at ctrl_classifier_controller_create_feature" );
                }

                /* write report in case of anomalies */
                if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_ID ) )
                {
                    io_import_elements_private_report_id_differs( this_,
                                                                  data_feature_get_data_id( feature_ptr ),
                                                                  data_feature_get_data_id( &((*this_).temp_feature ) )
                                                                );
                }

                data_feature_destroy( &((*this_).temp_feature ) );
            }
            else  /* lifeline in paste mode */
            {
                assert( is_lifeline );
                data_stat_inc_count( (*this_).stat,
                                     DATA_STAT_TABLE_LIFELINE, /* is_lifeline is true */
                                     DATA_STAT_SERIES_IGNORED
                                   );
                U8_TRACE_INFO( "lifeline dropped at json import." );
            }
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

u8_error_t io_import_elements_sync_relationship( io_import_elements_t *this_,
                                                 const data_relationship_t *relation_ptr,
                                                 const char *from_node_uuid,
                                                 const char *to_node_uuid )
{
    U8_TRACE_BEGIN();
    assert( NULL != relation_ptr );
    assert( NULL != from_node_uuid );
    assert( NULL != to_node_uuid );
    u8_error_t sync_error = U8_ERROR_NONE;

    /* ANY MODE: determine from classifier/feature */
    data_row_id_t from_classifier_id = DATA_ROW_ID_VOID;
    data_row_id_t from_feature_id = DATA_ROW_ID_VOID;
    data_feature_type_t from_feature_type = DATA_FEATURE_TYPE_VOID;
    if ( from_node_uuid != NULL )
    {
        if ( ! utf8string_equals_str( from_node_uuid, "" ) )
        {
            /* search source classifier id */
            data_classifier_init_empty( &((*this_).temp_classifier ) );
            const u8_error_t read_error1
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                               from_node_uuid,
                                                               &((*this_).temp_classifier)
                                                             );
            if ( U8_ERROR_NONE == read_error1 )
            {
                from_classifier_id = data_classifier_get_row_id( &((*this_).temp_classifier) );
                U8_TRACE_INFO_STR( "id found for src classifier:", from_node_uuid );
            }
            else
            {
                /* search source feature id */
                data_feature_init_empty( &((*this_).temp_feature) );
                const u8_error_t read_error2
                    = data_database_reader_get_feature_by_uuid( (*this_).db_reader,
                                                                from_node_uuid,
                                                                &((*this_).temp_feature)
                                                              );
                if ( U8_ERROR_NONE == read_error2 )
                {
                    from_classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                    from_feature_id = data_feature_get_row_id( &((*this_).temp_feature) );
                    from_feature_type = data_feature_get_main_type( &((*this_).temp_feature) );
                    U8_TRACE_INFO_STR( "id found for src feature:", from_node_uuid );
                }
                else
                {
                    U8_TRACE_INFO_STR( "relationship source not found", from_node_uuid );
                }
                data_feature_destroy( &((*this_).temp_feature) );
            }
            data_classifier_destroy( &((*this_).temp_classifier ) );
        }
    }

    /* ANY MODE: determine to classifier/feature */
    data_row_id_t to_classifier_id = DATA_ROW_ID_VOID;
    data_row_id_t to_feature_id = DATA_ROW_ID_VOID;
    data_feature_type_t to_feature_type = DATA_FEATURE_TYPE_VOID;
    if ( to_node_uuid != NULL )
    {
        if ( ! utf8string_equals_str( to_node_uuid, "" ) )
        {
            /* search destination classifier id */
            data_classifier_init_empty( &((*this_).temp_classifier ) );
            const u8_error_t read_error3
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                               to_node_uuid,
                                                               &((*this_).temp_classifier)
                                                             );
            if ( U8_ERROR_NONE == read_error3 )
            {
                to_classifier_id = data_classifier_get_row_id( &((*this_).temp_classifier) );
                U8_TRACE_INFO_STR( "id found for dst classifier:", to_node_uuid );
            }
            else
            {
                /* search dst feature id */
                data_feature_init_empty( &((*this_).temp_feature) );
                const u8_error_t read_error4
                    = data_database_reader_get_feature_by_uuid( (*this_).db_reader,
                                                                to_node_uuid,
                                                                &((*this_).temp_feature)
                                                              );
                if ( U8_ERROR_NONE == read_error4 )
                {
                    to_classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                    to_feature_id = data_feature_get_row_id( &((*this_).temp_feature) );
                    to_feature_type = data_feature_get_main_type( &((*this_).temp_feature) );
                    U8_TRACE_INFO_STR( "id found for src feature:", to_node_uuid );
                }
                else
                {
                    U8_TRACE_INFO_STR( "relationship destination not found", to_node_uuid );
                }
                data_feature_destroy( &((*this_).temp_feature) );
            }
            data_classifier_destroy( &((*this_).temp_classifier ) );
        }
    }

    /* check preconditions */
    if (( (*this_).mode == IO_IMPORT_MODE_PASTE )||( (*this_).mode == IO_IMPORT_MODE_LINK ))
    {
        if ( from_classifier_id == DATA_ROW_ID_VOID )
        {
            sync_error |= U8_ERROR_VALUE_OUT_OF_RANGE;
            U8_LOG_ERROR( "A relationship could not be created because the source classifier could not be found." );
        }
        if ( to_classifier_id == DATA_ROW_ID_VOID )
        {
            sync_error |= U8_ERROR_VALUE_OUT_OF_RANGE;
            U8_LOG_ERROR( "A relationship could not be created because the destination classifier could not be found." );
        }
        if ( sync_error != U8_ERROR_NONE )
        {
            const bool is_scenario = data_rules_relationship_is_scenario_cond( &((*this_).data_rules),
                                                                               from_feature_type,
                                                                               to_feature_type
                                                                             );
            U8_TRACE_INFO( is_scenario ? "relationship in interaction scenario dropped" : "general relationship dropped" );
        }
    }

    if ((( (*this_).mode == IO_IMPORT_MODE_PASTE )||( (*this_).mode == IO_IMPORT_MODE_LINK ))
        &&( sync_error == U8_ERROR_NONE ))
    {
        /* check if the parsed relationship already exists in this database; if not, create it */
        data_relationship_init_empty( &((*this_).temp_relationship ) );
        const u8_error_t read_error5
            = data_database_reader_get_relationship_by_uuid( (*this_).db_reader,
                                                             data_relationship_get_uuid_const( relation_ptr ),
                                                             &((*this_).temp_relationship)
                                                           );
        data_relationship_destroy( &((*this_).temp_relationship ) );
        const bool relationship_exists = ( U8_ERROR_NONE == read_error5 );

        if ( relationship_exists )
        {
            /* update statistics */
            data_stat_inc_count( (*this_).stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_IGNORED );
        }
        else
        {
            /* create relationship */
            data_relationship_copy( &((*this_).temp_relationship ), relation_ptr );
            data_relationship_set_row_id( &((*this_).temp_relationship ), data_relationship_get_row_id( relation_ptr ) );
            data_relationship_set_from_classifier_row_id( &((*this_).temp_relationship ), from_classifier_id );
            data_relationship_set_from_feature_row_id( &((*this_).temp_relationship ), from_feature_id );
            data_relationship_set_to_classifier_row_id( &((*this_).temp_relationship ), to_classifier_id );
            data_relationship_set_to_feature_row_id( &((*this_).temp_relationship ), to_feature_id );

            /* create relationship */
            u8_error_t modified_info;
            sync_error = ctrl_multi_step_changer_create_relationship( &((*this_).multi_step_changer),
                                                                      &((*this_).temp_relationship ),
                                                                      &modified_info
                                                                    );
            if ( U8_ERROR_NONE != sync_error )
            {
                U8_LOG_ERROR( "unexpected error at ctrl_classifier_controller_create_relationship" );
            }
            else
            {
            }

            /* update statistics */
            data_stat_inc_count( (*this_).stat,
                                 DATA_STAT_TABLE_RELATIONSHIP,
                                 (U8_ERROR_NONE==sync_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                               );

            /* write report in case of anomalies */
            if ( u8_error_contains( modified_info, U8_ERROR_DUPLICATE_ID ) )
            {
                io_import_elements_private_report_id_differs( this_,
                                                              data_relationship_get_data_id( relation_ptr ),
                                                              data_relationship_get_data_id( &((*this_).temp_relationship ) )
                                                            );
            }

            data_relationship_destroy( &((*this_).temp_relationship ) );
        }
    }

    U8_TRACE_END_ERR( sync_error );
    return sync_error;
}

void io_import_elements_private_report_id_differs( io_import_elements_t *this_, data_id_t req_id, data_id_t act_id )
{
    U8_TRACE_BEGIN();
    u8_error_t report_err = U8_ERROR_NONE;

    report_err |= utf8stream_writer_write_str( (*this_).english_report, "Id changed: " );
    report_err |= data_id_to_utf8_writer( &req_id, (*this_).english_report );
    report_err |= utf8stream_writer_write_str( (*this_).english_report, " -> " );
    report_err |= data_id_to_utf8_writer( &act_id, (*this_).english_report );
    report_err |= utf8stream_writer_write_str( (*this_).english_report, ", " );
    if ( report_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "Could not write report on import, ERR:", report_err );
    }

    U8_TRACE_END();
}

void io_import_elements_private_report_name_differs( io_import_elements_t *this_, const char *req_name, const char *act_name )
{
    U8_TRACE_BEGIN();
    assert( NULL != req_name );
    assert( NULL != act_name );
    u8_error_t report_err = U8_ERROR_NONE;

    report_err |= utf8stream_writer_write_str( (*this_).english_report, "Name changed: \"" );
    report_err |= utf8stream_writer_write_str( (*this_).english_report, req_name );
    report_err |= utf8stream_writer_write_str( (*this_).english_report, "\" -> \"" );
    report_err |= utf8stream_writer_write_str( (*this_).english_report, act_name );
    report_err |= utf8stream_writer_write_str( (*this_).english_report, "\", " );
    if ( report_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "Could not write report on import, ERR:", report_err );
    }

    U8_TRACE_END();
}


/*
Copyright 2021-2023 Andreas Warnke

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
