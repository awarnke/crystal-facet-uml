/* File: io_data_file.inl; Copyright and License: see below */

#include "tslog/tslog.h"
#include <assert.h>

static inline u8_error_t io_data_file_open_writeable ( io_data_file_t *this_, const char* db_file_path )
{
    return io_data_file_open( this_, db_file_path, false );
}

static inline u8_error_t io_data_file_open_read_only ( io_data_file_t *this_, const char* db_file_path )
{
    return io_data_file_open( this_, db_file_path, true );
}

static inline data_database_t *io_data_file_get_database_ptr ( io_data_file_t *this_ )
{
    return &((*this_).database);
}

static inline ctrl_controller_t *io_data_file_get_controller_ptr ( io_data_file_t *this_ )
{
    return &((*this_).controller);
}

static inline const char *io_data_file_get_filename_ptr ( io_data_file_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).data_file_name );
}

static inline bool io_data_file_is_open( io_data_file_t *this_ )
{
    return data_database_is_open( &((*this_).database) );
}

static inline utf8error_t io_data_file_private_replace_file_extension ( const io_data_file_t *this_,
                                                                        utf8stringbuf_t file_path,
                                                                        const char* extension
                                                                      )
{
    assert( extension != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;
    const int dot_pos = utf8stringbuf_find_last_str( file_path, "." );
    if ( dot_pos != -1 )
    {
        /* The current name has an extension; remove it: */
        result = utf8stringbuf_delete_to_end( file_path, dot_pos );
    }
    result |= utf8stringbuf_append_str( file_path, extension );
    return result;
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
