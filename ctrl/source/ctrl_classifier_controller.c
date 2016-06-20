/* File: ctrl_classifier_controller.c; Copyright and License: see below */

#include "ctrl_classifier_controller.h"
#include "trace.h"
#include "log.h"

void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_, data_database_t *database, data_database_reader_t *db_reader, data_database_writer_t *db_writer )
{
    TRACE_BEGIN();

    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;
    ctrl_consistency_checker_init ( &((*this_).consistency_checker), db_reader, db_writer );

    TRACE_END();
}

void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ )
{
    TRACE_BEGIN();

    ctrl_consistency_checker_destroy ( &((*this_).consistency_checker) );
    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

ctrl_error_t ctrl_classifier_controller_create_classifier_in_diagram ( ctrl_classifier_controller_t *this_, int64_t diagram_id, data_classifier_type_t classifier_type, const char* classifier_name, int64_t* out_new_id )
{
    TRACE_BEGIN();
    data_classifier_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    /* create a classifier row */

    data_result = data_classifier_init_new( &to_be_created, classifier_type, "", classifier_name, "", 0, 0 );
    result |= (ctrl_error_t) data_result;

    data_result = data_database_writer_create_classifier( (*this_).db_writer, &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result |= (ctrl_error_t) data_result;

    data_classifier_destroy( &to_be_created );

    /* create a diagramelement row */
    if ( DATA_ERROR_NONE == data_result )
    {
        data_diagramelement_t link_to_be_created;
        int64_t new_link_id;

        data_diagramelement_init_new( &link_to_be_created, diagram_id, new_id, DATA_DIAGRAMELEMENT_DISPLAY_FLAG_NONE );

        data_result = data_database_writer_create_diagramelement( (*this_).db_writer, &link_to_be_created, &new_link_id );
        if ( DATA_ERROR_NONE != data_result )
        {
            result |= (ctrl_error_t) data_result;
        }

        data_diagramelement_destroy( &link_to_be_created );
    }

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_stereotype ( ctrl_classifier_controller_t *this_, int64_t classifier_id, const char* new_classifier_stereotype )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_classifier_stereotype( (*this_).db_writer, classifier_id, new_classifier_stereotype );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_description ( ctrl_classifier_controller_t *this_, int64_t classifier_id, const char* new_classifier_description )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_classifier_description( (*this_).db_writer, classifier_id, new_classifier_description );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_name ( ctrl_classifier_controller_t *this_, int64_t classifier_id, const char* new_classifier_name )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_classifier_name( (*this_).db_writer, classifier_id, new_classifier_name );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_update_classifier_main_type ( ctrl_classifier_controller_t *this_, int64_t classifier_id, data_classifier_type_t new_classifier_main_type )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_classifier_main_type( (*this_).db_writer, classifier_id, new_classifier_main_type );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_classifier_controller_delete_set ( ctrl_classifier_controller_t *this_, data_small_set_t objects )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    if ( data_small_set_is_empty( &objects ) )
    {
        result = CTRL_ERROR_INPUT_EMPTY;
    }
    else
    {
        LOG_ERROR( "not yet implemented" );

        int index;
        for ( index = 0; index < data_small_set_get_count( &objects ); index ++ )
        {
            data_id_t current_id;
            current_id = data_small_set_get_id( &objects, index );
            if ( DATA_TABLE_CLASSIFIER == data_id_get_table( &current_id ) )
            {
            }
            else if ( DATA_TABLE_FEATURE == data_id_get_table( &current_id ) )
            {
            }
            else if ( DATA_TABLE_RELATIONSHIP == data_id_get_table( &current_id ) )
            {
            }
            else if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &current_id ) )
            {
            }
            else if ( DATA_TABLE_DIAGRAM == data_id_get_table( &current_id ) )
            {
            }
        }

        /* the following functions may help to implement this: */
        /* data_error_t data_database_writer_delete_diagramelement( data_database_writer_t *this_, int64_t obj_id, data_diagramelement_t *out_old_diagramelement ) */
        /* data_error_t data_database_writer_delete_classifier( data_database_writer_t *this_, int64_t obj_id, data_classifier_t *out_old_classifier ) */
        /* data_error_t data_database_writer_delete_diagram ( data_database_writer_t *this_, int64_t obj_id, data_diagram_t *out_old_diagram ) */
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
