/* File: u8dir_file.c; Copyright and License: see below */

#include "u8dir/u8dir_file.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <assert.h>

u8_error_t u8dir_file_remove( u8dir_file_t this_ )
{
    U8_TRACE_BEGIN();
    assert( this_ != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const int remove_err = remove( this_ );
    if ( 0 != remove_err )
    {
        /* This error may have happened on purpose or by an unexpected condition */
        if (errno == ENOENT)
        {
            U8_TRACE_INFO_STR( "tried to remove non-existing file:", this_ );
            U8_LOG_EVENT("remove() called on non-existing file.");
            err |= U8_ERROR_FILE_ALREADY_REMOVED;
        }
        else
        {
            U8_TRACE_INFO_STR( "error at removing file:", this_ );
            U8_LOG_EVENT("remove() failed to remove a file.");
            err |= U8_ERROR_AT_FILE_WRITE;
        }
    }
    else
    {
        U8_TRACE_INFO_STR( "removed file:", this_ );
        U8_LOG_EVENT("remove() removed a file.");
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t u8dir_file_get_size( u8dir_file_t this_, uint64_t* out_file_size )
{
    U8_TRACE_BEGIN();
    assert( this_ != NULL );
    assert( out_file_size != NULL );
    u8_error_t err = U8_ERROR_NONE;

    struct stat stat_buffer;
    const int stat_err = stat( this_, &stat_buffer );
    if ( stat_err == -1 )
    {
        err = U8_ERROR_AT_FILE_READ;
    }
    else
    {
        *out_file_size = stat_buffer.st_size;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t u8dir_file_get_modification_time( u8dir_file_t this_, uint64_t* out_mod_time )
{
    U8_TRACE_BEGIN();
    assert( this_ != NULL );
    assert( out_mod_time != NULL );
    u8_error_t err = U8_ERROR_NONE;

    struct stat stat_buffer;
    const int stat_err = stat( this_, &stat_buffer );
    if ( stat_err == -1 )
    {
        err = U8_ERROR_AT_FILE_READ;
    }
    else
    {
#ifdef _WIN32
        *out_mod_time = stat_buffer.st_mtime;
#else
        *out_mod_time = stat_buffer.st_mtim.tv_sec;
#endif
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t u8dir_file_get_creation_time( u8dir_file_t this_, uint64_t* out_create_time )
{
    U8_TRACE_BEGIN();
    assert( this_ != NULL );
    assert( out_create_time != NULL );
    u8_error_t err = U8_ERROR_NONE;

    struct stat stat_buffer;
    const int stat_err = stat( this_, &stat_buffer );
    if ( stat_err == -1 )
    {
        err = U8_ERROR_AT_FILE_READ;
    }
    else
    {
#ifdef _WIN32
        *out_create_time = stat_buffer.st_ctime;
#else
        *out_create_time = stat_buffer.st_ctim.tv_sec;
#endif
    }

    U8_TRACE_END_ERR(err);
    return err;
}


/*
Copyright 2022-2025 Andreas Warnke

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
