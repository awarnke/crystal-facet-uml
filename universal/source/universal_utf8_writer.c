/* File: universal_utf8_writer.c; Copyright and License: see below */

#include "universal_utf8_writer.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>

void universal_utf8_writer_init ( universal_utf8_writer_t *this_, universal_output_stream_t* out_stream )
{
    TRACE_BEGIN();
    assert( out_stream != NULL );

    (*this_).output_stream = out_stream;

    TRACE_END();
}

void universal_utf8_writer_destroy ( universal_utf8_writer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).output_stream = NULL;

    TRACE_END();
}

int universal_utf8_writer_write_str ( universal_utf8_writer_t *this_, const void *utf8_string )
{
    /*TRACE_BEGIN();*/
    assert( (*this_).output_stream != NULL );
    assert( utf8_string != NULL );
    int err = 0;

    size_t length = strlen(utf8_string);
    err = universal_output_stream_write( (*this_).output_stream, utf8_string, length );

    /*TRACE_END_ERR(err);*/
    return err;
}

int universal_utf8_writer_flush ( universal_utf8_writer_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).output_stream != NULL );
    int err = 0;

    err = universal_output_stream_flush( (*this_).output_stream );

    TRACE_END_ERR(err);
    return err;
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
