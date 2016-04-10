/* File: ctrl_classifier_controller.c; Copyright and License: see below */

#include "ctrl_classifier_controller.h"
#include "trace.h"
#include "log.h"

void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_, data_database_t *database )
{
    TRACE_BEGIN();

    (*this_).database = database;
    data_database_writer_init( &((*this_).db_writer), database );
    data_database_reader_init( &((*this_).db_reader), database );

    TRACE_END();
}

void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ )
{
    TRACE_BEGIN();

    data_database_writer_destroy( &((*this_).db_writer) );
    data_database_reader_destroy( &((*this_).db_reader) );

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

    data_classifier_init_new( &to_be_created, classifier_type, classifier_name, "" );

    data_result = data_database_writer_create_classifier( &((*this_).db_writer), &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = (ctrl_error_t) data_result;

    data_classifier_destroy( &to_be_created );

    /* create a diagramelement row */
    if ( CTRL_ERROR_NONE == result )
    {
        data_diagramelement_t link_to_be_created;
        int64_t new_link_id;

        data_diagramelement_init_new( &link_to_be_created, diagram_id, new_id );

        data_result = data_database_writer_create_diagramelement( &((*this_).db_writer), &link_to_be_created, &new_link_id );
        if ( DATA_ERROR_NONE != data_result )
        {
            result = (ctrl_error_t) data_result;
        }

        data_diagramelement_destroy( &link_to_be_created );
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
