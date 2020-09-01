/* File: universal_escaping_output_stream.c; Copyright and License: see below */

#include "stream/universal_escaping_output_stream.h"
#include "stream/universal_output_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_escaping_output_stream_private_if
    = {
        /* open:   */ (int (*)(void*, const char*)) &universal_escaping_output_stream_open,
        /* write:  */ (int (*)(void*, const void*, size_t)) &universal_escaping_output_stream_write,
        /* flush:  */ (int (*)(void*)) &universal_escaping_output_stream_flush,
        /* close:  */ (int (*)(void*)) &universal_escaping_output_stream_close
    };

void universal_escaping_output_stream_init ( universal_escaping_output_stream_t *this_,
                                             const char *const ((*patterns_and_replacements)[][2]),
                                             universal_output_stream_t *sink )
{
    TRACE_BEGIN();
    assert( patterns_and_replacements != NULL );
    assert( sink != NULL );

    (*this_).patterns_and_replacements = patterns_and_replacements;
    (*this_).sink = sink;
    universal_output_stream_init( &((*this_).output_stream), &universal_escaping_output_stream_private_if, this_ );

    TRACE_END();
}

int universal_escaping_output_stream_destroy( universal_escaping_output_stream_t *this_ )
{
    TRACE_BEGIN();
    int err = 0;

    (*this_).patterns_and_replacements = NULL;
    (*this_).sink = NULL;
    universal_output_stream_destroy( &((*this_).output_stream) );

    TRACE_END_ERR(err);
    return err;
}

void universal_escaping_output_stream_change_rules( universal_escaping_output_stream_t *this_,
                                                    const char *const ((*patterns_and_replacements)[][2]) )
{
    TRACE_BEGIN();
    assert( patterns_and_replacements != NULL );

    (*this_).patterns_and_replacements = patterns_and_replacements;

    TRACE_END();
}

int universal_escaping_output_stream_open ( universal_escaping_output_stream_t *this_, const char *path )
{
    TRACE_BEGIN();
    assert( (*this_).sink != NULL );

    const int err = universal_output_stream_open( (*this_).sink, path );

    TRACE_END_ERR(err);
    return err;
}

int universal_escaping_output_stream_write ( universal_escaping_output_stream_t *this_, const void *start, size_t length )
{
    TRACE_BEGIN();
    assert( start != NULL );
    assert( (*this_).patterns_and_replacements != NULL );
    assert( (*this_).sink != NULL );
    int err = 0;
    const char (*char_buf)[] = (void*)start;
    
    /* count input patterns */
    unsigned int pattern_count = 0;
    for ( unsigned int pattern_idx = 0; (*((*this_).patterns_and_replacements))[pattern_idx][0] != NULL; pattern_idx++ )
    {
        pattern_count++;
    };

    /* search and replace patterns */
    size_t bytes_already_written = 0;
    for ( size_t index = 0; index < length; index ++ )
    {
        /* check if a pattern matches */
        int matching_pattern_idx = -1;
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

    TRACE_END_ERR(err);
    return err;
}

int universal_escaping_output_stream_flush( universal_escaping_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).sink != NULL );

    const int err = universal_output_stream_flush( (*this_).sink );

    TRACE_END_ERR(err);
    return err;
}

int universal_escaping_output_stream_close( universal_escaping_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).sink != NULL );

    const int err = universal_output_stream_close( (*this_).sink );

    TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_escaping_output_stream_get_output_stream( universal_escaping_output_stream_t *this_ )
{
    TRACE_BEGIN();

    universal_output_stream_t* result = &((*this_).output_stream);

    TRACE_END();
    return result;
}


/*
Copyright 2020-2020 Andreas Warnke

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