/* File: json_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "utf8stringbuf/utf8error.h"
#include "utf8stringbuf/utf8codepoint.h"
#include "utf8stringbuf/utf8codepointiterator.h"

static inline u8_error_t json_writer_write_plain ( json_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    u8_error_t write_err;

    const size_t text_len = utf8string_get_length(text);
    write_err = universal_output_stream_write ( (*this_).output, text, text_len );

    return ( write_err );
}

static inline u8_error_t json_writer_write_plain_view ( json_writer_t *this_, utf8stringview_t string_view )
{
    u8_error_t write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    write_err = universal_output_stream_write( (*this_).output, start, length );

    return ( write_err );
}

static inline u8_error_t json_writer_write_string_enc ( json_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    u8_error_t write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_string_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), text, text_len );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline u8_error_t json_writer_write_string_view_enc ( json_writer_t *this_, utf8stringview_t string_view )
{
    u8_error_t write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_string_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline u8_error_t json_writer_write_stringlist_enc ( json_writer_t *this_, utf8string_t text )
{
    assert ( UTF8STRING_NULL != text );
    u8_error_t write_err;

    const size_t text_len = utf8string_get_length(text);
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_stringlist_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), text, text_len );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline u8_error_t json_writer_write_stringlist_view_enc ( json_writer_t *this_, utf8stringview_t string_view )
{
    u8_error_t write_err;

    const size_t length = utf8stringview_get_length( string_view );
    const char *const start = utf8stringview_get_start( string_view );
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_stringlist_encode_table );
    write_err = universal_escaping_output_stream_write( &((*this_).esc_output), start, length );
    universal_escaping_output_stream_flush( &((*this_).esc_output) );

    return write_err;
}

static inline u8_error_t json_writer_write_member_int ( json_writer_t *this_,
                                                 unsigned int indent,
                                                 utf8string_t enc_name,
                                                 int64_t number_value,
                                                 bool next_follows )
{
    assert( 7 == JSON_WRITER_MAX_INDENT );
    assert( indent <= JSON_WRITER_MAX_INDENT );
    u8_error_t write_err;

    write_err = json_writer_write_plain( this_, &(JSON_CONSTANTS_INDENT_QUOTE[2*(JSON_WRITER_MAX_INDENT-indent)]) );
    write_err |= json_writer_write_plain( this_, enc_name );
    write_err |= json_writer_write_plain( this_,
                                          JSON_CONSTANTS_QUOTE
                                          JSON_CONSTANTS_DEF
                                        );
    write_err |= json_writer_write_int( this_, number_value );
    write_err |= json_writer_write_plain( this_,
                                          next_follows
                                          ? JSON_CONSTANTS_NEXT_NL
                                          : JSON_CONSTANTS_NL
                                        );

    return write_err;
}

static inline u8_error_t json_writer_write_member_string ( json_writer_t *this_,
                                                    unsigned int indent,
                                                    utf8string_t enc_name,
                                                    utf8string_t unenc_value,
                                                    bool next_follows )
{
    assert( 7 == JSON_WRITER_MAX_INDENT );
    assert( indent <= JSON_WRITER_MAX_INDENT );
    u8_error_t write_err;

    write_err = json_writer_write_plain( this_, &(JSON_CONSTANTS_INDENT_QUOTE[2*(JSON_WRITER_MAX_INDENT-indent)]) );
    write_err |= json_writer_write_plain( this_, enc_name );
    write_err |= json_writer_write_plain( this_,
                                          JSON_CONSTANTS_QUOTE
                                          JSON_CONSTANTS_DEF
                                          JSON_CONSTANTS_QUOTE
                                        );
    write_err |= json_writer_write_string_enc( this_, unenc_value );
    write_err |= json_writer_write_plain( this_,
                                          next_follows
                                          ? JSON_CONSTANTS_QUOTE JSON_CONSTANTS_NEXT_NL
                                          : JSON_CONSTANTS_QUOTE JSON_CONSTANTS_NL
                                        );

    return write_err;
}

static inline u8_error_t json_writer_write_member_string_array ( json_writer_t *this_,
                                                          unsigned int indent,
                                                          utf8string_t enc_name,
                                                          utf8string_t unenc_value,
                                                          bool next_follows )
{
    assert( 7 == JSON_WRITER_MAX_INDENT );
    assert( indent <= JSON_WRITER_MAX_INDENT );
    u8_error_t write_err;

    write_err = json_writer_write_plain( this_, &(JSON_CONSTANTS_INDENT_QUOTE[2*(JSON_WRITER_MAX_INDENT-indent)]) );
    write_err |= json_writer_write_plain( this_, enc_name );
    write_err |= json_writer_write_plain( this_,
                                          JSON_CONSTANTS_QUOTE
                                          JSON_CONSTANTS_DEF
                                          JSON_CONSTANTS_BEGIN_ARRAY
                                          JSON_CONSTANTS_NL
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_TAB
                                          JSON_CONSTANTS_QUOTE
                                        );
    write_err |= json_writer_write_stringlist_enc( this_, unenc_value );
    write_err |= json_writer_write_plain( this_, JSON_CONSTANTS_QUOTE JSON_CONSTANTS_NL );
    write_err = json_writer_write_plain( this_, &(JSON_CONSTANTS_INDENT[2*(JSON_WRITER_MAX_INDENT-indent)]) );
    write_err |= json_writer_write_plain( this_,
                                          next_follows
                                          ? JSON_CONSTANTS_END_ARRAY JSON_CONSTANTS_NEXT_NL
                                          : JSON_CONSTANTS_END_ARRAY JSON_CONSTANTS_NL
                                        );

    return write_err;
}


/*
Copyright 2021-2024 Andreas Warnke

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
