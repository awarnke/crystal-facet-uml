/* File: data_database_reader.c; Copyright and License: see below */

#include "data_database_reader.h"
#include "trace.h"
#include "log.h"
#include <sqlite3.h>

/*!
 * \brief callback to trace database results
 */
static int data_database_reader_private_trace_sql_result( void *my_data, int num, char** a, char** b );

void data_database_reader_init ( data_database_reader_t *this_, data_database_t *database )
{
    TRACE_BEGIN();

    (*this_).database = database;

    TRACE_END();
}

void data_database_reader_destroy ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

static int data_database_writer_private_trace_sql_result( void *my_data, int num, char** a, char** b )
{
    TRACE_BEGIN();
    int result;

    TRACE_INFO_INT( "num:", num );

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
