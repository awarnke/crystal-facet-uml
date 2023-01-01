/* File: universal_escaping_output_stream.c; Copyright and License: see below */

#include "u8stream/universal_escaping_output_stream.h"
#include "u8stream/universal_output_stream_if.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_escaping_output_stream_private_if
    = {
        .write = (u8_error_t (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_escaping_output_stream_write,
        .flush = (u8_error_t (*)(universal_output_stream_impl_t*)) &universal_escaping_output_stream_flush
    };

void universal_escaping_output_stream_init ( universal_escaping_output_stream_t *this_,
                                             const char *const ((*patterns_and_replacements)[][2]),
                                             universal_output_stream_t *sink )
{
    U8_TRACE_BEGIN();
    assert( patterns_and_replacements != NULL );
    assert( sink != NULL );

    (*this_).patterns_and_replacements = patterns_and_replacements;
    (*this_).sink = sink;
    universal_output_stream_private_init( &((*this_).output_stream), &universal_escaping_output_stream_private_if, this_ );

    U8_TRACE_END();
}

u8_error_t universal_escaping_output_stream_destroy( universal_escaping_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    (*this_).patterns_and_replacements = NULL;
    (*this_).sink = NULL;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    U8_TRACE_END_ERR(err);
    return err;
}

void universal_escaping_output_stream_change_rules( universal_escaping_output_stream_t *this_,
                                                    const char *const ((*patterns_and_replacements)[][2]) )
{
    U8_TRACE_BEGIN();
    assert( patterns_and_replacements != NULL );

    (*this_).patterns_and_replacements = patterns_and_replacements;

    U8_TRACE_END();
}

u8_error_t universal_escaping_output_stream_write ( universal_escaping_output_stream_t *this_, const void *start, size_t length )
{
    /*U8_TRACE_BEGIN();*/
    assert( start != NULL );
    assert( (*this_).patterns_and_replacements != NULL );
    assert( (*this_).sink != NULL );
    u8_error_t err = U8_ERROR_NONE;
    const char (*char_buf)[] = (void*)start;

    /* count and analyze input patterns */
    unsigned int pattern_count = 0;
    char head_common_bits = '\x00';  /* optimization, improves xml export by 5% */
    char head_common_pattern = '\x00';  /* optimization, improves xml export by 5% */
    for ( unsigned int pattern_idx = 0; (*((*this_).patterns_and_replacements))[pattern_idx][0] != NULL; pattern_idx++ )
    {
        pattern_count++;
        if ( pattern_idx == 0 )
        {
            head_common_bits = '\xff';
            head_common_pattern = *((*((*this_).patterns_and_replacements))[pattern_idx][0]);
        }
        else
        {
            const char unequal_bits = head_common_pattern ^ *((*((*this_).patterns_and_replacements))[pattern_idx][0]);
            head_common_bits = head_common_bits & ( ~ unequal_bits );
        }
    };
    head_common_pattern &= head_common_bits;

    /* search and replace patterns */
    size_t bytes_already_written = 0;
    for ( size_t index = 0; index < length; index ++ )
    {
        /* check if a pattern matches */
        int matching_pattern_idx = -1;
        const char chr_at_idx = (*char_buf)[index];
        if (( chr_at_idx & head_common_bits ) == head_common_pattern )
        {
            for ( unsigned int pattern_idx = 0; ( pattern_idx < pattern_count )&&( matching_pattern_idx == -1 ); pattern_idx++ )
            {
                const char * pattern = (*((*this_).patterns_and_replacements))[pattern_idx][0];
                const unsigned int pattern_len = strlen( pattern );
                if (( index + pattern_len <= length )&&( pattern_len > 0 ))
                {
                    if ( 0 == memcmp( &((*char_buf)[index]), pattern, pattern_len ) )
                    {
                        matching_pattern_idx = pattern_idx;
                        /*fprintf(stderr,"found pattern %d at pos %zd\n",matching_pattern_idx,index);*/
                    }
                }
            }
        }

        /* replace pattern */
        if ( matching_pattern_idx != -1 )
        {
            /* write previously processed bytes */
            err |= universal_output_stream_write( (*this_).sink, &((*char_buf)[bytes_already_written]), index-bytes_already_written );
            bytes_already_written = index;
            /* write pattern */
            const char * pattern = (*((*this_).patterns_and_replacements))[matching_pattern_idx][0];
            const unsigned int pattern_len = strlen( pattern );
            const char * replacement = (*((*this_).patterns_and_replacements))[matching_pattern_idx][1];
            unsigned int replace_len = 0;
            if ( replacement != NULL )
            {
                replace_len = strlen(replacement);
            }
            err |= universal_output_stream_write( (*this_).sink, replacement, replace_len );
            bytes_already_written += pattern_len;
            /* forward index */
            index = index + pattern_len - 1;
        }

        if ( (index+1)==length ) /* is last? */
        {
            err |= universal_output_stream_write( (*this_).sink, &((*char_buf)[bytes_already_written]), length-bytes_already_written );
            bytes_already_written = length;
        }
    }

    /*U8_TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_escaping_output_stream_flush( universal_escaping_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).sink != NULL );

    const u8_error_t err = universal_output_stream_flush( (*this_).sink );

    U8_TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_escaping_output_stream_get_output_stream( universal_escaping_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();

    universal_output_stream_t* result = &((*this_).output_stream);

    U8_TRACE_END();
    return result;
}


/*
Copyright 2020-2023 Andreas Warnke

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
