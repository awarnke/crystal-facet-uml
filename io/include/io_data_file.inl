/* File: io_data_file.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline u8_error_t io_data_file_open_writeable ( io_data_file_t *this_,
                                                       const char* requested_file_path,
                                                       u8_error_info_t *out_err_info )
{
    return io_data_file_open( this_, requested_file_path, false, out_err_info );
}

static inline u8_error_t io_data_file_open_read_only ( io_data_file_t *this_,
                                                       const char* requested_file_path,
                                                       u8_error_info_t *out_err_info )
{
    return io_data_file_open( this_, requested_file_path, true, out_err_info );
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
                                                    const utf8stringview_t *path,
                                                    utf8stringview_t *out_parent,
                                                    utf8stringview_t *out_filename )
{
    assert( path != NULL );
    assert( out_parent != NULL );
    assert( out_filename != NULL );
    utf8stringview_t before_winpath_sep;
    utf8stringview_t after_winpath_sep;
    const utf8error_t err_w = utf8stringview_split_at_last_str( path, "\\", &before_winpath_sep, &after_winpath_sep );
    if (  err_w == UTF8ERROR_SUCCESS )
    {
        /* add the separator to before_winpath_sep */
        before_winpath_sep = UTF8STRINGVIEW( utf8stringview_get_start( &before_winpath_sep ),
                                             utf8stringview_get_length( &before_winpath_sep ) + sizeof(char)
                                           );
    }
    utf8stringview_t before_unixpath_sep;
    utf8stringview_t after_unixpath_sep;
    const utf8error_t err_u = utf8stringview_split_at_last_str( path, "/", &before_unixpath_sep, &after_unixpath_sep );
    if ( err_u == UTF8ERROR_SUCCESS )
    {
        /* add the separator to before_unixpath_sep */
        before_unixpath_sep = UTF8STRINGVIEW( utf8stringview_get_start( &before_unixpath_sep ),
                                              utf8stringview_get_length( &before_unixpath_sep ) + sizeof(char)
                                            );
    }
    if ( err_w == UTF8ERROR_SUCCESS )
    {
        if ( err_u == UTF8ERROR_SUCCESS )
        {
            /* There is a win and a unix separator, take the shorter filename */
            if ( utf8stringview_get_length( &after_winpath_sep ) < utf8stringview_get_length( &after_unixpath_sep ) )
            {
                *out_parent = before_winpath_sep;
                *out_filename = after_winpath_sep;
            }
            else
            {
                *out_parent = before_unixpath_sep;
                *out_filename = after_unixpath_sep;
            }
        }
        else
        {
            /* There is a win separator */
            *out_parent = before_winpath_sep;
            *out_filename = after_winpath_sep;
        }
    }
    else
    {
        if ( err_u == UTF8ERROR_SUCCESS )
        {
            /* There is a unix separator */
            *out_parent = before_unixpath_sep;
            *out_filename = after_unixpath_sep;
        }
        else
        {
            /* There is neither a win nor a unix separator */
            *out_parent = UTF8STRINGVIEW_EMPTY;
            *out_filename = *path;
        }
    }
}

static inline void io_data_file_private_split_extension( const io_data_file_t *this_,
                                                         const utf8stringview_t *filename,
                                                         utf8stringview_t *out_basename,
                                                         utf8stringview_t *out_extension )
{
    assert( filename != NULL );
    assert( out_basename != NULL );
    assert( out_extension != NULL );
    utf8stringview_t before_dot;
    utf8stringview_t after_dot;
    const utf8error_t err = utf8stringview_split_at_last_str( filename, ".", &before_dot, &after_dot );
    if ( ( err != UTF8ERROR_SUCCESS )
        || ( utf8stringview_get_length( &before_dot ) == 0 )
        || ( utf8stringview_get_length( &after_dot ) == 0 ) )
    {
        /* either no dot found or the filename begins with dot or the filename ends on dot */
        *out_basename = *filename;
        *out_extension = UTF8STRINGVIEW_EMPTY;
    }
    else
    {
        *out_basename = before_dot;
        *out_extension = after_dot;
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
