/* File: io_data_file.inl; Copyright and License: see below */

#include "tslog/tslog.h"
#include <assert.h>

static inline u8_error_t io_data_file_open ( io_data_file_t *this_, const char* db_file_path )
{
    assert( db_file_path != NULL );

    const u8_error_t err = data_database_open( &((*this_).database), db_file_path );

    return err;
}

static inline u8_error_t io_data_file_open_read_only ( io_data_file_t *this_, const char* db_file_path )
{
    assert( db_file_path != NULL );

    const u8_error_t err = data_database_open_read_only( &((*this_).database), db_file_path );

    return err;
}

static inline data_database_t *io_data_file_get_database_ptr ( io_data_file_t *this_ )
{
    return &((*this_).database);
}

static inline const char *io_data_file_get_filename_ptr ( io_data_file_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).data_file_name );
}

static inline bool io_data_file_is_open( io_data_file_t *this_ )
{
    return data_database_is_open( &((*this_).database) );
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
