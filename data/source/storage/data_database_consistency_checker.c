/* File: data_database_consistency_checker.c; Copyright and License: see below */

#include "storage/data_database_consistency_checker.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

data_error_t data_database_consistency_checker_init ( data_database_consistency_checker_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = database;

    TRACE_END_ERR(result);
    return result;
}


data_error_t data_database_consistency_checker_destroy ( data_database_consistency_checker_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_consistency_checker_find_unreferenced_diagrams ( data_database_consistency_checker_t *this_, data_small_set_t *out_set )
{
    TRACE_BEGIN();
    assert( NULL != out_set );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

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
