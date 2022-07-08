/* File: io_data_file.c; Copyright and License: see below */

#include "io_data_file.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <assert.h>


void io_data_file_init ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    data_database_init( &((*this_).database) );

    (*this_).data_file_name
        = utf8stringbuf_init( sizeof((*this_).private_data_file_name_buffer), (*this_).private_data_file_name_buffer );
    utf8stringbuf_clear( (*this_).data_file_name );

    (*this_).auto_writeback_to_json = false;

    TRACE_END();
}


u8_error_t io_data_file_close ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    const u8_error_t result = data_database_close( &((*this_).database) );

    TRACE_END_ERR( result );
    return result;
}

void io_data_file_destroy ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    data_database_destroy( &((*this_).database) );

    TRACE_END();
}

u8_error_t io_data_file_flush_caches ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    const u8_error_t result = data_database_flush_caches( &((*this_).database) );

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2022-2022 Andreas Warnke

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
