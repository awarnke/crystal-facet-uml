/* File: data_enum_iterator.inl; Copyright and License: see below */

#include <assert.h>

static inline void data_enum_iterator_init ( data_enum_iterator_t *this_,
                                             const data_enum_def_t* list,
                                             unsigned int size,
                                             unsigned int next )
{
    (*this_).list = list;
    (*this_).size = size;
    (*this_).next = next;
}

static inline void data_enum_iterator_destroy ( data_enum_iterator_t *this_ )
{
    (*this_).next = 0;
    (*this_).size = 0;
}

static inline bool data_enum_iterator_has_next ( const data_enum_iterator_t *this_ )
{
    return ((*this_).next < (*this_).size);
}

static inline u8_error_t data_enum_iterator_next ( data_enum_iterator_t *this_, data_enum_def_t *out_data_enum )
{
    assert( out_data_enum != NULL );
    u8_error_t result = U8_ERROR_NONE;

    if ((*this_).next < (*this_).size)
    {
        *out_data_enum = (*this_).list[(*this_).next];
        (*this_).next ++;
    }
    else
    {
        result = U8_ERROR_INVALID_REQUEST;
        *out_data_enum = DATA_ENUM_DEF_VOID;
    };

    return result;
}

static inline void data_enum_iterator_trace ( const data_enum_iterator_t *this_ )
{
    U8_TRACE_INFO_INT_INT( "data_enum_iterator_t", (*this_).next, (*this_).size );
}


/*
Copyright 2026-2026 Andreas Warnke

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
