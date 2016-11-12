/* File: data_database_writer.inl; Copyright and License: see below */

#include "tslog.h"

static inline data_error_t data_database_writer_private_lock ( data_database_writer_t *this_ )
{
    data_error_t result = DATA_ERROR_NONE;
    int perr;

    perr = pthread_mutex_lock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        TSLOG_ERROR_INT( "pthread_mutex_lock() failed:", perr );
        result = DATA_ERROR_AT_MUTEX;
    }
}

static inline data_error_t data_database_writer_private_unlock ( data_database_writer_t *this_ )
{
    data_error_t result = DATA_ERROR_NONE;
    int perr;

    perr = pthread_mutex_unlock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        TSLOG_ERROR_INT( "pthread_mutex_unlock() failed:", perr );
        result = DATA_ERROR_AT_MUTEX;
    }
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
