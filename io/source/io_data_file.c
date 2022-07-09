/* File: io_data_file.c; Copyright and License: see below */

#include "io_data_file.h"
#include "u8stream/universal_file_input_stream.h"
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

void io_data_file_destroy ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    data_database_destroy( &((*this_).database) );

    TRACE_END();
}

u8_error_t io_data_file_open ( io_data_file_t *this_, const char* db_file_path )
{
    TRACE_BEGIN();
    assert( db_file_path != NULL );

    utf8stringbuf_copy_str( (*this_).data_file_name, db_file_path );

    const u8_error_t err = data_database_open( &((*this_).database), db_file_path );

    TRACE_END_ERR( err );
    return err;
}

u8_error_t io_data_file_open_read_only ( io_data_file_t *this_, const char* db_file_path )
{
    TRACE_BEGIN();
    assert( db_file_path != NULL );

    utf8stringbuf_copy_str( (*this_).data_file_name, db_file_path );

    const u8_error_t err = data_database_open_read_only( &((*this_).database), db_file_path );

    TRACE_END_ERR( err );
    return err;
}

u8_error_t io_data_file_close ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    const u8_error_t result = data_database_close( &((*this_).database) );

    TRACE_END_ERR( result );
    return result;
}

u8_error_t io_data_file_sync_to_disk ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    const u8_error_t result = data_database_flush_caches( &((*this_).database) );

    TRACE_END_ERR( result );
    return result;
}

u8_error_t io_data_file_trace_stats ( io_data_file_t *this_ )
{
    TRACE_BEGIN();

    TRACE_INFO_STR( "io_data_file_t:", utf8stringbuf_get_string( (*this_).data_file_name ) );

    const u8_error_t result = data_database_trace_stats( &((*this_).database) );

    TRACE_END_ERR( result );
    return result;
}

u8_error_t io_data_file_private_guess_db_type ( io_data_file_t *this_, const char *filename, bool *out_json )
{
    TRACE_BEGIN();
    assert( filename != NULL );
    assert( out_json != NULL );
    u8_error_t scan_head_error = U8_ERROR_NONE;

    /* open file */
    universal_file_input_stream_t in_file;
    universal_file_input_stream_init( &in_file );
    scan_head_error |= universal_file_input_stream_open( &in_file, filename );

    /* import from stream */
    if ( scan_head_error == U8_ERROR_NONE )
    {
        char file_prefix[16];
        size_t prefix_size;
        assert( sizeof(file_prefix) == sizeof(DATA_DATABASE_SQLITE3_MAGIC) );
        scan_head_error = universal_file_input_stream_read( &in_file, &file_prefix, sizeof(file_prefix), &prefix_size );
        if (( scan_head_error == U8_ERROR_NONE )&&( prefix_size == sizeof(file_prefix) )
            &&( 0 == memcmp( &file_prefix, &DATA_DATABASE_SQLITE3_MAGIC, sizeof(file_prefix) ) ))
        {
            TRACE_INFO_STR("File exists and starts with sqlite3 magic:", filename);
            *out_json = false;
        }
        else
        {
            TRACE_INFO_STR("File exists and is not of type sqlite3:", filename);
            *out_json = true;
        }
    }
    else
    {
        if( 1 == utf8string_ends_with_str( filename, ".json" ) )
        {
            TRACE_INFO_STR("File does not exist and is of type json:", filename);
            *out_json = true;
        }
        else
        {
            TRACE_INFO_STR("File does not exist and type defaults to sqlite3:", filename);
            *out_json = false;
        }
    }

    /* close file */
    scan_head_error |= universal_file_input_stream_close( &in_file );
    scan_head_error |= universal_file_input_stream_destroy( &in_file );

    TRACE_END_ERR( scan_head_error );
    return scan_head_error;
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
