/* File: io_import_elements.c; Copyright and License: see below */

#include "io_import_elements.h"
#include "ctrl_error.h"
#include "util/string/utf8string.h"
#include "trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void io_import_elements_init ( io_import_elements_t *this_,
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

    (*this_).mode = IO_IMPORT_MODE_CHECK;
    (*this_).paste_to_diagram = DATA_ROW_ID_VOID;

    (*this_).is_first_undo_action = true;

    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void io_import_elements_init_for_paste( io_import_elements_t *this_,
                                        data_row_id_t paste_to_diagram,
                                        data_database_reader_t *db_reader,
                                        ctrl_controller_t *controller,
                                        data_stat_t *io_stat )
{
    TRACE_BEGIN();

    io_import_elements_init( this_, db_reader, controller, io_stat );
    (*this_).mode = IO_IMPORT_MODE_PASTE;
    (*this_).paste_to_diagram = paste_to_diagram;

    /* check if diagram id exists */
    {
        const int sync_error
            = data_database_reader_get_diagram_by_id( (*this_).db_reader,
                                                      (*this_).paste_to_diagram,
                                                      &((*this_).temp_diagram)
                                                    );
        if ( DATA_ERROR_NONE != sync_error )
        {
            TSLOG_ERROR_INT( "diagram id where to import json data does not exist (anymore)", (*this_).paste_to_diagram );
            (*this_).paste_to_diagram = DATA_ROW_ID_VOID;
        }
        else
        {
            data_diagram_destroy( &((*this_).temp_diagram) );
        }
    }

    TRACE_END();
}

void io_import_elements_destroy ( io_import_elements_t *this_ )
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

void io_import_elements_set_mode( io_import_elements_t *this_, io_import_mode_t mode )
{
    TRACE_BEGIN();

    (*this_).mode = mode;

    TRACE_END();
}

int io_import_elements_sync_diagram( io_import_elements_t *this_,
                                     data_diagram_t *diagram_ptr,
                                     const char *parent_uuid )
{
    TRACE_BEGIN();
    assert( NULL != diagram_ptr );
     /* parent_uuid is NULL if root diagram */
    int sync_error = 0;

    /* determine parent id */
    data_row_id_t parent_row_id = DATA_ROW_ID_VOID;
    if ( parent_uuid != NULL )
    {
        if ( ! utf8string_equals_str( parent_uuid, "" ) )
        {
            const data_error_t try_read
                = data_database_reader_get_diagram_by_uuid( (*this_).db_reader,
                                                            parent_uuid,
                                                            &((*this_).temp_diagram)
                                                          );
            if ( try_read != DATA_ERROR_NONE )
            {
                sync_error = -1;
                TRACE_INFO_STR( "parent diagram not found", parent_uuid );
            }
            else
            {
                parent_row_id = data_diagram_get_row_id( &((*this_).temp_diagram ) );
                data_diagram_destroy( &((*this_).temp_diagram ) );
            }
        }
    }
    data_diagram_set_parent_row_id( diagram_ptr, parent_row_id );

    if (( (*this_).mode == IO_IMPORT_MODE_PASTE )&&( sync_error==0 ))
    {
        if ( (*this_).paste_to_diagram == DATA_ROW_ID_VOID )
        {
            sync_error = -1;
            TRACE_INFO_STR( "in paste-clipboard mode, parent diagram must be valid", parent_uuid );
        }
        else
        {
            data_diagram_set_parent_row_id( diagram_ptr, (*this_).paste_to_diagram );
        }
    }

    if (( (*this_).mode != IO_IMPORT_MODE_CHECK )&&( sync_error==0 ))
    {
        /* create the parsed diagram as child below the current diagram */
        ctrl_diagram_controller_t *diag_ctrl;
        diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

        ctrl_error_t write_error3;
        data_row_id_t new_diag_id;
        write_error3 = ctrl_diagram_controller_create_diagram ( diag_ctrl,
                                                                diagram_ptr,
                                                                ( (*this_).is_first_undo_action
                                                                ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                &new_diag_id
                                                              );
        data_stat_inc_count( (*this_).stat,
                             DATA_TABLE_DIAGRAM,
                             (CTRL_ERROR_NONE==write_error3)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                           );
        if ( CTRL_ERROR_NONE != write_error3 )
        {
            TSLOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagram" );
            sync_error = -1;
        }
        else
        {
            /* insert all consecutive elements to this new diagram */
            (*this_).paste_to_diagram = new_diag_id;

            (*this_).is_first_undo_action = false;
        }
    }

    TRACE_END_ERR( sync_error );
    return sync_error;
}

int io_import_elements_sync_diagramelement( io_import_elements_t *this_,
                                            const data_diagramelement_t *diagramelement_ptr,
                                            const char *diagram_uuid,
                                            const char *node_uuid )
{
    TRACE_BEGIN();
    int sync_error = 0;
    assert( NULL != diagramelement_ptr );
    assert( NULL != diagram_uuid );
    assert( NULL != node_uuid );

    if ( (*this_).mode != IO_IMPORT_MODE_CHECK )
    {
    }

    TRACE_END_ERR( sync_error );
    return sync_error;
}

int io_import_elements_sync_classifier( io_import_elements_t *this_,
                                        const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert( NULL != classifier_ptr );
    int sync_error = 0;
    data_row_id_t the_classifier_id = DATA_ROW_ID_VOID;

    if (( (*this_).mode == IO_IMPORT_MODE_PASTE )&&( sync_error==0 ))
    {
        if ( (*this_).paste_to_diagram == DATA_ROW_ID_VOID )
        {
            sync_error = -1;
            TRACE_INFO( "in paste-clipboard mode, parent diagram must be valid" );
        }
    }

    if (( (*this_).mode != IO_IMPORT_MODE_CHECK )&&( sync_error==0 ))
    {
        /* check if the parsed classifier already exists in this database; if not, create it */
        ctrl_classifier_controller_t *classifier_ctrl;
        classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

        data_error_t read_error;
        data_classifier_t existing_classifier;
        read_error = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                                  data_classifier_get_uuid_const( classifier_ptr ),
                                                                  &existing_classifier
                                                                );
        const bool classifier_exists = ( DATA_ERROR_NONE == read_error );

        if ( ! classifier_exists )
        {
            ctrl_error_t write_error;
            write_error = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                        classifier_ptr,
                                                                        ( (*this_).is_first_undo_action
                                                                        ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                        : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                        &the_classifier_id
                                                                      );
            data_stat_inc_count( (*this_).stat,
                                 DATA_TABLE_CLASSIFIER,
                                 (CTRL_ERROR_NONE==write_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                               );

            if ( CTRL_ERROR_NONE != write_error )
            {
                TSLOG_ERROR( "unexpected error at ctrl_classifier_controller_create_classifier/feature" );
                sync_error = -1;
            }
            else
            {
                (*this_).is_first_undo_action = false;

                sync_error = io_import_elements_private_create_diagramelement( this_, the_classifier_id );
            }
        }
        else
        {
            /* do the statistics */
            data_stat_inc_count( (*this_).stat,
                                 DATA_TABLE_CLASSIFIER,
                                 DATA_STAT_SERIES_IGNORED
                               );
            TRACE_INFO( "classifier did already exist." );
            /* set the the_classifier_id */
            the_classifier_id = data_classifier_get_row_id( &existing_classifier );
        }
    }

    TRACE_END_ERR( sync_error );
    return sync_error;
}

int io_import_elements_private_create_diagramelement( io_import_elements_t *this_, data_row_id_t classifier_id )
{
    TRACE_BEGIN();
    assert( DATA_ROW_ID_VOID != classifier_id );
    int sync_error = 0;

    if (( (*this_).mode != IO_IMPORT_MODE_CHECK )&&( sync_error==0 ))
    {
        /* link the classifier to the current diagram */
        ctrl_diagram_controller_t *diag_ctrl;
        diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

        ctrl_error_t write_error2;
        data_row_id_t new_element_id;
        data_diagramelement_t diag_ele;
        data_diagramelement_init_new( &diag_ele,
                                      (*this_).paste_to_diagram,
                                      classifier_id,
                                      DATA_DIAGRAMELEMENT_FLAG_NONE,
                                      DATA_ROW_ID_VOID
                                    );
        write_error2 = ctrl_diagram_controller_create_diagramelement( diag_ctrl,
                                                                      &diag_ele,
                                                                      ( (*this_).is_first_undo_action
                                                                      ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                      : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                      &new_element_id
                                                                    );
        data_stat_inc_count( (*this_).stat,
                             DATA_TABLE_DIAGRAMELEMENT,
                             (CTRL_ERROR_NONE==write_error2)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                           );
        if ( CTRL_ERROR_NONE != write_error2 )
        {
            TSLOG_ERROR( "unexpected error at ctrl_diagram_controller_create_diagramelement" );
            sync_error = -1;
        }
        else
        {
            (*this_).is_first_undo_action = false;
        }
    }

    TRACE_END_ERR( sync_error );
    return sync_error;
}

int io_import_elements_sync_feature( io_import_elements_t *this_,
                                     data_feature_t *feature_ptr,
                                     const char *classifier_uuid )
{
    TRACE_BEGIN();
    assert( NULL != feature_ptr );
    assert( NULL != classifier_uuid );
    int sync_error = 0;

    /* determine classifier id */
    data_row_id_t classifier_row_id = DATA_ROW_ID_VOID;
    if ( classifier_uuid != NULL )
    {
        if ( ! utf8string_equals_str( classifier_uuid, "" ) )
        {
            const data_error_t try_read
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                               classifier_uuid,
                                                               &((*this_).temp_classifier)
                                                             );
            if ( try_read != DATA_ERROR_NONE )
            {
                sync_error = -1;
                TRACE_INFO_STR( "parent classifier not found", classifier_uuid );
            }
            else
            {
                classifier_row_id = data_classifier_get_row_id( &((*this_).temp_classifier ) );
                data_classifier_destroy( &((*this_).temp_classifier ) );
            }
        }
    }
    data_feature_set_classifier_row_id( feature_ptr, classifier_row_id );

    if (( (*this_).mode != IO_IMPORT_MODE_CHECK )&&( sync_error==0 ))
    {
        data_row_id_t new_feature_id;
        /* filter lifelines */
        if ( ! data_rules_feature_is_scenario_cond( &((*this_).data_rules),
                                                    data_feature_get_main_type( feature_ptr ) ) )
        {
            ctrl_classifier_controller_t *classifier_ctrl
                = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

            const ctrl_error_t write_error
                = ctrl_classifier_controller_create_feature( classifier_ctrl,
                                                             feature_ptr,
                                                             ( (*this_).is_first_undo_action
                                                             ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                             : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                             &new_feature_id
                                                           );
            data_stat_inc_count( (*this_).stat,
                                 DATA_TABLE_FEATURE,
                                 (CTRL_ERROR_NONE==write_error)?DATA_STAT_SERIES_CREATED:DATA_STAT_SERIES_ERROR
                               );
            if ( CTRL_ERROR_NONE != write_error )
            {
                TSLOG_ERROR( "unexpected error at ctrl_classifier_controller_create_feature" );
                sync_error = -1;
            }
            else
            {
                (*this_).is_first_undo_action = false;
            }
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

    TRACE_END_ERR( sync_error );
    return sync_error;
}

int io_import_elements_sync_relationship( io_import_elements_t *this_,
                                          data_relationship_t *relation_ptr,
                                          const char *from_node_uuid,
                                          const char *to_node_uuid )
{
    TRACE_BEGIN();
    assert( NULL != relation_ptr );
    assert( NULL != from_node_uuid );
    assert( NULL != to_node_uuid );
    int sync_error = 0;

    data_row_id_t from_classifier_id = DATA_ROW_ID_VOID;
    data_row_id_t from_feature_id = DATA_ROW_ID_VOID;
    data_feature_type_t from_feature_type = DATA_FEATURE_TYPE_VOID;
    data_row_id_t to_classifier_id = DATA_ROW_ID_VOID;
    data_row_id_t to_feature_id = DATA_ROW_ID_VOID;
    data_feature_type_t to_feature_type = DATA_FEATURE_TYPE_VOID;

    if ( from_node_uuid != NULL )
    {
        if ( ! utf8string_equals_str( from_node_uuid, "" ) )
        {
            /* search source classifier id */
            const data_error_t read_error1
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                               from_node_uuid,
                                                               &((*this_).temp_classifier)
                                                             );
            if ( DATA_ERROR_NONE == read_error1 )
            {
                from_classifier_id = data_classifier_get_row_id( &((*this_).temp_classifier) );
                TRACE_INFO_STR( "id found for src classifier:", from_node_uuid );
                data_classifier_destroy( &((*this_).temp_classifier) );
            }
            else
            {
                /* search source feature id */
                const data_error_t read_error2
                    = data_database_reader_get_feature_by_uuid( (*this_).db_reader,
                                                                from_node_uuid,
                                                                &((*this_).temp_feature)
                                                              );
                if ( DATA_ERROR_NONE == read_error2 )
                {
                    from_classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                    from_feature_id = data_feature_get_row_id( &((*this_).temp_feature) );
                    from_feature_type = data_feature_get_main_type( &((*this_).temp_feature) );
                    TRACE_INFO_STR( "id found for src feature:", from_node_uuid );
                    data_feature_destroy( &((*this_).temp_feature) );
                }
                else
                {
                    sync_error = -1;
                    TRACE_INFO_STR( "relationship source not found", from_node_uuid );
                }
            }
        }
    }

    if ( to_node_uuid != NULL )
    {
        if ( ! utf8string_equals_str( to_node_uuid, "" ) )
        {
            /* search destination classifier id */
            const data_error_t read_error3
                = data_database_reader_get_classifier_by_uuid( (*this_).db_reader,
                                                               to_node_uuid,
                                                               &((*this_).temp_classifier)
                                                             );
            if ( DATA_ERROR_NONE == read_error3 )
            {
                to_classifier_id = data_classifier_get_row_id( &((*this_).temp_classifier) );
                TRACE_INFO_STR( "id found for dst classifier:", to_node_uuid );
                data_classifier_destroy( &((*this_).temp_classifier) );
            }
            else
            {
                /* search dst feature id */
                const data_error_t read_error4
                    = data_database_reader_get_feature_by_uuid( (*this_).db_reader,
                                                                to_node_uuid,
                                                                &((*this_).temp_feature)
                                                              );
                if ( DATA_ERROR_NONE == read_error4 )
                {
                    to_classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                    to_feature_id = data_feature_get_row_id( &((*this_).temp_feature) );
                    to_feature_type = data_feature_get_main_type( &((*this_).temp_feature) );
                    TRACE_INFO_STR( "id found for src feature:", to_node_uuid );
                    data_feature_destroy( &((*this_).temp_feature) );
                }
                else
                {
                    sync_error = -1;
                    TRACE_INFO_STR( "relationship destination not found", to_node_uuid );
                }
            }
        }
    }

    if ( (*this_).mode != IO_IMPORT_MODE_CHECK )
    {
        /* check if the parsed relationship already exists in this database; if not, create it */

        /* create relationship */
        bool dropped=false;
        if ( from_classifier_id == DATA_ROW_ID_VOID )
        {
            TSLOG_ERROR_STR( "A relationship could not be created because the source classifier could not be found.",
                             from_node_uuid
                           );
            dropped = true;
        }
        else if ( to_classifier_id == DATA_ROW_ID_VOID )
        {
            TSLOG_ERROR_STR( "A relationship could not be created because the destination classifier could not be found.",
                             to_node_uuid
                           );
            dropped = true;
        }
        else if (( data_relationship_get_from_feature_row_id( relation_ptr ) != DATA_ROW_ID_VOID )
            && ( from_feature_id == DATA_ROW_ID_VOID ))
        {
            TSLOG_ERROR_STR( "A relationship could not be created because the source feature could not be found.",
                             from_node_uuid
                           );
            dropped = true;
        }
        else if (( data_relationship_get_to_feature_row_id( relation_ptr ) != DATA_ROW_ID_VOID )
            && ( to_feature_id == DATA_ROW_ID_VOID ))
        {
            TSLOG_ERROR_STR( "A relationship could not be created because the destination feature could not be found.",
                             to_node_uuid
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
            data_relationship_set_row_id ( relation_ptr, DATA_ROW_ID_VOID );
            data_relationship_set_from_classifier_row_id ( relation_ptr, from_classifier_id );
            data_relationship_set_from_feature_row_id ( relation_ptr, from_feature_id );
            data_relationship_set_to_classifier_row_id ( relation_ptr, to_classifier_id );
            data_relationship_set_to_feature_row_id ( relation_ptr, to_feature_id );

            /* create relationship */
            data_row_id_t relationship_id;
            write_error4 = ctrl_classifier_controller_create_relationship ( classifier_control4,
                                                                            relation_ptr,
                                                                            ( (*this_).is_first_undo_action
                                                                            ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                            : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND ),
                                                                            &relationship_id
                                                                            );
            if ( CTRL_ERROR_NONE != write_error4 )
            {
                TSLOG_ERROR( "unexpected error at ctrl_classifier_controller_create_relationship" );
                sync_error = -1;
            }
            else
            {
                (*this_).is_first_undo_action = false;
            }
        }

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

    TRACE_END_ERR( sync_error );
    return sync_error;
}


/*
Copyright 2021-2021 Andreas Warnke

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
