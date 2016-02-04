/* File: data_database.c; Copyright and License: see below */

#include "data_database.h"
#include "trace.h"
#include "log.h"

void data_database_init ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;
    
    (*this_).db_file_name = "crystal_facet_uml_default.db";
    
    sqlite_err = sqlite3_open( (*this_).db_file_name, &((*this_).db) );
    if ( sqlite_err ) 
    {
        LOG_ERROR_INT( "sqlite3_open() failed:", sqlite_err );
        (*this_).is_open = false;
    }
    else
    {
        (*this_).is_open = true;
    }
    
    TRACE_END();
}

void data_database_destroy ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;

    (*this_).db_file_name = "";
    (*this_).is_open = false;

    sqlite_err = sqlite3_close( (*this_).db );
    if ( sqlite_err ) 
    {
        LOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
    }

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
