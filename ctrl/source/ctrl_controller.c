/* File: ctrl_controller.c; Copyright and License: see below */

#include "ctrl_controller.h"
#include "trace.h"
#include "log.h"

void ctrl_controller_init ( ctrl_controller_t *this_, data_database_t *database )
{
    TRACE_BEGIN();

    /* init member attributes */
    (*this_).database = database;
    ctrl_classifier_controller_init ( &((*this_).classifiers), database );
    ctrl_diagram_controller_init ( &((*this_).diagrams), database );

    TRACE_END();
}

void ctrl_controller_destroy ( ctrl_controller_t *this_ )
{
    TRACE_BEGIN();

    /* destroy member attributes */
    ctrl_diagram_controller_destroy ( &((*this_).diagrams) );
    ctrl_classifier_controller_destroy ( &((*this_).classifiers) );

    TRACE_END();
}

ctrl_error_t ctrl_controller_switch_database ( ctrl_controller_t *this_, const char* db_file_path )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    /*
    data_result = data_database_close( (*this_).database );
    data_result |= data_database_open( (*this_).database, db_file_path );
    result = (ctrl_error_t) data_result;
    */
    LOG_ERROR("switching the database does not work as sketched in above excluded code! prepared statements are still open.");

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
