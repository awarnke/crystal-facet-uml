/* File: universal_memory_output_stream.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8stringview.h"
#include <assert.h>

static inline void* universal_memory_output_stream_get_start( const universal_memory_output_stream_t *this_ )
{
    assert( (*this_).mem_buf_start != NULL );
    return (*this_).mem_buf_start;
}

static inline size_t universal_memory_output_stream_get_size( const universal_memory_output_stream_t *this_ )
{
    return (*this_).mem_buf_size;
}

static inline size_t universal_memory_output_stream_get_fill( const universal_memory_output_stream_t *this_ )
{
    assert( (*this_).mem_buf_filled <= (*this_).mem_buf_size );
    return (*this_).mem_buf_filled;
}

static inline u8_error_t universal_memory_output_stream_private_write_0term ( universal_memory_output_stream_t *this_, bool utf8_mode )
{
    U8_TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).mem_buf_size == 0 )
    {
        U8_LOG_ERROR( "buffer size is 0; buffer is not terminated by zero." );
        err = U8_ERROR_CONFIG_OUT_OF_RANGE;
    }
    else if ( (*this_).mem_buf_filled < (*this_).mem_buf_size )
    {
        /* add a terminating zero at position (*this_).mem_buf_filled, */
        /* but do not increase the mem_buf_filled because more bytes may be written ... */
        char *const term_char = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_filled]  );
        *term_char = '\0';
    }
    else
    {
        if ( utf8_mode )
        {
            /* use a utf8stringview_t to determine the end-position of the last full unicode codepoint: */
            utf8stringview_t view_on_buf;
            const utf8error_t cut = utf8stringview_init( &view_on_buf, (*this_).mem_buf_start, (*this_).mem_buf_size-1 );
            char *const last_char
                = &(  (*(  (char(*)[])utf8stringview_get_start(&view_on_buf)  ))[utf8stringview_get_length(&view_on_buf)]  );
            *last_char = '\0';
            utf8stringview_destroy( &view_on_buf );
            if ( cut == UTF8ERROR_SUCCESS )
            {
                U8_TRACE_INFO( "last byte overwritten by terminating zero" );
                /* prevent errors on consecutive calls to flush (e.g. at a reset to utf8stream_writer): */
                (*this_).mem_buf_filled = (*this_).mem_buf_size - 1;
            }
            else
            {
                U8_TRACE_INFO( "multiple last bytes dropped by terminating zero" );
                /* prevent errors on consecutive calls to flush (-1 is not accurate here but suits the purpose): */
                (*this_).mem_buf_filled = (*this_).mem_buf_size - 1;
            }
        }
        else
        {
            char *const last_char = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_size - 1]  );
            *last_char = '\0';
            U8_TRACE_INFO( "last byte overwritten by terminating zero" );
            /* prevent errors on consecutive calls to flush: */
            (*this_).mem_buf_filled = (*this_).mem_buf_size - 1;
        }
        err = U8_ERROR_AT_FILE_WRITE;
    }


    U8_TRACE_END_ERR(err);
    return err;
}


/*
Copyright 2024-2024 Andreas Warnke

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
