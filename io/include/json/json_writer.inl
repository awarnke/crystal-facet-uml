/* File: json_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "util/string/utf8error.h"
#include "util/string/utf8codepoint.h"
#include "util/string/utf8codepointiterator.h"

static inline int json_writer_write_plain ( json_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    int write_err;

    const size_t text_len = utf8string_get_length(text);
    write_err = universal_output_stream_write ( (*this_).output, text, text_len );

    return ( write_err );
}

static inline int json_writer_write_plain_view ( json_writer_t *this_, utf8stringview_t string_view )
{
    int write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    write_err = universal_output_stream_write( (*this_).output, start, length );

    return ( write_err );
}

static inline int json_writer_write_string_enc ( json_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    int write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_string_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), text, text_len );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline int json_writer_write_string_view_enc ( json_writer_t *this_, utf8stringview_t string_view )
{
    int write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_string_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline int json_writer_write_stringlist_enc ( json_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    int write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_stringlist_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), text, text_len );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline int json_writer_write_stringlist_view_enc ( json_writer_t *this_, utf8stringview_t string_view )
{
    int write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_stringlist_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
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
