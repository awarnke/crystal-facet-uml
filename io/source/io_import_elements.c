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
                        ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != controller );


    TRACE_END();
}

void io_import_elements_destroy ( io_import_elements_t *this_ )
{
    TRACE_BEGIN();


    TRACE_END();
}

int io_import_elements_import_file( io_import_elements_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != NULL );
    assert( NULL != NULL );
    assert( NULL != NULL );
    int parse_error = 0;



    TRACE_END_ERR( parse_error );
    return parse_error;
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
