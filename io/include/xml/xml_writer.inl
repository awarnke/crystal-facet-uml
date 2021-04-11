/* File: xml_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "util/string/utf8error.h"

static inline int xml_writer_write_plain ( xml_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    int write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_plain_table );
    write_err = universal_escaping_output_stream_write ( &((*this_).esc_output), text, text_len );

    return ( write_err );
}

static inline int xml_writer_write_plain_view ( xml_writer_t *this_, utf8stringview_t string_view )
{
    int write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_plain_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );

    return ( write_err );
}

static inline int xml_writer_write_xml_enc ( xml_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    int write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), text, text_len );

    return write_err;
}

static inline int xml_writer_write_xml_enc_view ( xml_writer_t *this_, utf8stringview_t string_view )
{
    int write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );

    return write_err;
}

static inline int xml_writer_write_xml_comment ( xml_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    int write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_comments_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), text, text_len );

    return write_err;
}

static inline int xml_writer_write_xml_comment_view ( xml_writer_t *this_, utf8stringview_t string_view )
{
    int write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_comments_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );

    return write_err;
}

static inline void xml_writer_reset_indent ( xml_writer_t *this_ )
{
    (*this_).indent_level = 0;
    xml_writer_update_encoding_tables( this_ );
}

static inline void xml_writer_increase_indent ( xml_writer_t *this_ )
{
    (*this_).indent_level++;
    xml_writer_update_encoding_tables( this_ );
}

static inline void xml_writer_decrease_indent ( xml_writer_t *this_ )
{
    if ( (*this_).indent_level > 0 )
    {
        (*this_).indent_level--;
        xml_writer_update_encoding_tables( this_ );
    }
    else
    {
        assert(false);
    }
}


/*
Copyright 2019-2021 Andreas Warnke

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
