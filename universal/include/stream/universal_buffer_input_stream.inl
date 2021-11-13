/* File: universal_buffer_input_stream.inl; Copyright and License: see below */

#include <assert.h>

static inline char universal_buffer_input_stream_peek_next( universal_buffer_input_stream_t *this_ )
{
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).mem_buf_fill >= (*this_).mem_buf_pos );
    assert( (*this_).mem_buf_size >= (*this_).mem_buf_fill );
    char result = '\0';

    const size_t buf_available1 = (*this_).mem_buf_fill - (*this_).mem_buf_pos;
    if ( buf_available1 == 0 )
    {
        /* try to fill buffer */
        (*this_).mem_buf_pos = 0;
        (*this_).mem_buf_fill = 0;
        const int err
            = universal_input_stream_read( (*this_).source, (*this_).mem_buf_start, (*this_).mem_buf_size, &((*this_).mem_buf_fill) );
        if (( 0 == err )&&( (*this_).mem_buf_fill > 0 ))
        {
            result = (*(  (char(*)[]) (*this_).mem_buf_start  ))[0];
        }
    }
    else
    {
        result = (*(  (char(*)[]) (*this_).mem_buf_start  ))[(*this_).mem_buf_pos];
    }

    return result;
}

static inline char universal_buffer_input_stream_read_next( universal_buffer_input_stream_t *this_ )
{
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).mem_buf_fill >= (*this_).mem_buf_pos );
    assert( (*this_).mem_buf_size >= (*this_).mem_buf_fill );
    char result = '\0';

    const size_t buf_available1 = (*this_).mem_buf_fill - (*this_).mem_buf_pos;
    if ( buf_available1 == 0 )
    {
        /* try to fill buffer */
        (*this_).mem_buf_pos = 0;
        (*this_).mem_buf_fill = 0;
        const int err
            = universal_input_stream_read( (*this_).source, (*this_).mem_buf_start, (*this_).mem_buf_size, &((*this_).mem_buf_fill) );
        if (( 0 == err )&&( (*this_).mem_buf_fill > 0 ))
        {
            result = (*(  (char(*)[]) (*this_).mem_buf_start  ))[0];
            (*this_).mem_buf_pos = 1;
        }
    }
    else
    {
        result = (*(  (char(*)[]) (*this_).mem_buf_start  ))[(*this_).mem_buf_pos];
        (*this_).mem_buf_pos ++;
    }

    return result;
}


/*
Copyright 2021-2021 Andreas Warnke

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
