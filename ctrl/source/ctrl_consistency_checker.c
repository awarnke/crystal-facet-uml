/* File: ctrl_consistency_checker.c; Copyright and License: see below */

#include "ctrl_consistency_checker.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_, data_database_reader_t *db_reader, data_database_writer_t *db_writer )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != db_writer );

    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    TRACE_END();
}

void ctrl_consistency_checker_destroy ( ctrl_consistency_checker_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
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
