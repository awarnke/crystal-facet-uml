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

    const int err = universal_output_stream_write( (*this_).sink, start, length );

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
