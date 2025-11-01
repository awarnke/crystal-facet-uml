/* File: data_database_head.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline u8_error_t data_database_head_delete_value_by_key ( data_database_head_t *this_,
                                                                  const char *key,
                                                                  data_head_t *out_old_head )
{
    u8_error_t result;
    data_head_t head;
    result = data_database_head_read_value_by_key( this_, key, &head );
    if ( result == U8_ERROR_NONE )
    {
        if ( out_old_head != NULL )
        {
            data_head_replace( out_old_head, &head );
        }
        result |= data_database_head_delete_value( this_, data_head_get_row_id( &head ), NULL );
    }
    return result;
}

static inline u8_error_t data_database_head_update_value_by_key ( data_database_head_t *this_,
                                                                  const char *key,
                                                                  const char* new_head_value,
                                                                  data_head_t *out_old_head )
{
    u8_error_t result;
    data_head_t head;
    result = data_database_head_read_value_by_key( this_, key, &head );
    if ( result == U8_ERROR_NONE )
    {
        if ( out_old_head != NULL )
        {
            data_head_replace( out_old_head, &head );
        }
        result |= data_database_head_update_value( this_, data_head_get_row_id( &head ), new_head_value, NULL );
    }
    return result;
}


/*
Copyright 2025-2025 Andreas Warnke

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
