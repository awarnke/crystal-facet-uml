/* File: io_data_file.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline u8_error_t io_data_file_open_writeable ( io_data_file_t *this_,
                                                       const char* db_file_path,
                                                       u8_error_info_t *out_err_info )
{
    return io_data_file_open( this_, db_file_path, false, out_err_info );
}

static inline u8_error_t io_data_file_open_read_only ( io_data_file_t *this_,
                                                       const char* db_file_path,
                                                       u8_error_info_t *out_err_info )
{
    return io_data_file_open( this_, db_file_path, true, out_err_info );
}

static inline data_database_t *io_data_file_get_database_ptr ( io_data_file_t *this_ )
{
    return &((*this_).database);
}

static inline ctrl_controller_t *io_data_file_get_controller_ptr ( io_data_file_t *this_ )
{
    return &((*this_).controller);
}

static inline const char *io_data_file_get_filename_const ( const io_data_file_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).data_file_name );
}

static inline bool io_data_file_is_open( io_data_file_t *this_ )
{
    return data_database_is_open( &((*this_).database) );
}

static inline void io_data_file_private_split_path( const io_data_file_t *this_,
                                                    utf8string_t *path,
                                                    utf8stringview_t *out_parent,
                                                    utf8stringview_t *out_basename,
                                                    utf8stringview_t *out_extension )
{
    assert( path != NULL );
    assert( out_parent != NULL );
    assert( out_basename != NULL );
    assert( out_extension != NULL );

#if 0
        utf8stringview_t before = UTF8STRINGVIEW_EMPTY;
        utf8stringview_t after = UTF8STRINGVIEW_EMPTY;
        const utf8error_t has_next
            = utf8stringview_split_at_first_str( &((*this_).remaining), (*this_).separator, &before, &after );
        if ( has_next != UTF8ERROR_SUCCESS )
#endif

    const int last_winpath_sep = utf8string_find_last_str( path, "\\" );
    const int last_path_sep = utf8string_find_last_str( path, "/" );
    const int last_sep = u8_i32_max2( last_winpath_sep, last_path_sep );
    const int sep_length = utf8string_get_length( "/" );
    const int last_dot = utf8string_find_last_str( path, "." );
    const int dot_length = utf8string_get_length( "." );
    const int len = utf8string_get_length( path );
    int start_basename;
    utf8error_t err = UTF8ERROR_SUCCESS;
    if ( last_sep == -1 )
    {
        *out_parent = UTF8STRINGVIEW_EMPTY;
        start_basename = 0;
    }
    else
    {
        err|= utf8stringview_init_region( out_parent, path, 0, last_sep + sep_length );
        start_basename = last_sep + sep_length;
    }
    if (( start_basename != last_dot )&&( len != ( last_dot + dot_length ) ))
    {
        err|= utf8stringview_init_region( out_basename, path, start_basename, ( last_dot - start_basename ) );
        err|= utf8stringview_init_region( out_extension, path, last_dot+dot_length, ( len - last_dot - dot_length ) );
    }
    else
    {
        err|= utf8stringview_init_region( out_basename, path, start_basename, ( len - start_basename ) );
        *out_extension = UTF8STRINGVIEW_EMPTY;
    }
    if ( err != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR("Unexpected internal error in formula for io_data_file_private_split_path()");
        assert( false );
    }
}


/*
Copyright 2022-2024 Andreas Warnke

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
