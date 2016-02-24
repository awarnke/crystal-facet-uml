/* File: ctrl_diagram_controller.c; Copyright and License: see below */

#include "ctrl_diagram_controller.h"
#include "trace.h"
#include "log.h"

void ctrl_diagram_controller_init ( ctrl_diagram_controller_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    
    (*this_).database = database;
    data_database_writer_init( &((*this_).db_writer), database );
    data_database_reader_init( &((*this_).db_reader), database );
    
    TRACE_END();
}

void ctrl_diagram_controller_destroy ( ctrl_diagram_controller_t *this_ )
{
    TRACE_BEGIN();
    
    data_database_writer_destroy( &((*this_).db_writer) );
    data_database_reader_destroy( &((*this_).db_reader) );
    
    TRACE_END();
}

int32_t ctrl_diagram_controller_create_diagram ( ctrl_diagram_controller_t *this_, int32_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name )
{
    TRACE_BEGIN();
    data_diagram_t to_be_created;
    int32_t result;
    
    data_diagram_init_new( &to_be_created, parent_diagram_id, diagram_type, diagram_name );
    
    TRACE_INFO_INT( "creating diagram to parent", parent_diagram_id );
    result = data_database_writer_create_diagram( &((*this_).db_writer) ,&to_be_created );
    
    data_diagram_destroy( &to_be_created );
    
    TRACE_END();
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
