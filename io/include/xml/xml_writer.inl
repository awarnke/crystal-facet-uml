/* File: xml_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "util/string/utf8error.h"
#include "util/string/utf8codepoint.h"
#include "util/string/utf8codepointiterator.h"

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

static inline bool xml_writer_contains_xml_tag_name_characters ( xml_writer_t *this_, utf8stringview_t string_view )
{
    bool result = false;
    
    utf8codepointiterator_t it;
    utf8codepointiterator_init( &it, string_view );
    while( utf8codepointiterator_has_next( &it ) && ( ! result ) )
    {
        utf8codepoint_t next = utf8codepointiterator_next( &it );
        if ( xml_writer_private_is_xml_tag_name_character( this_, utf8codepoint_get_char( next ), true ) )
        {
            result = true;
        }   
    }
    utf8codepointiterator_destroy( &it );
    
    return result;
}

static inline int xml_writer_write_xml_tag_name_characters ( xml_writer_t *this_, utf8stringview_t string_view )
{
    int result = -1;
    bool is_start = true;
    
    universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_plain_table );

    utf8codepointiterator_t it;
    utf8codepointiterator_init( &it, string_view );
    while( utf8codepointiterator_has_next( &it ) )
    {
        utf8codepoint_t next = utf8codepointiterator_next( &it );
        if ( xml_writer_private_is_xml_tag_name_character( this_, utf8codepoint_get_char( next ), is_start ) )
        {
            if ( is_start )
            {
                is_start = false;
                result = 0;
            }
            
            const utf8codepointseq_t text = utf8codepoint_get_utf8( next );
            const unsigned int text_len = utf8codepoint_get_length( next );
            result |= universal_escaping_output_stream_write ( &((*this_).esc_output), &(text.seq), text_len );
        }   
    }
    utf8codepointiterator_destroy( &it );
    
    return result;
}

static inline void xml_writer_reset_indent ( xml_writer_t *this_ )
{
    (*this_).indent_level = 0;
    xml_writer_private_update_encoding_tables( this_ );
}

static inline void xml_writer_increase_indent ( xml_writer_t *this_ )
{
    (*this_).indent_level++;
    xml_writer_private_update_encoding_tables( this_ );
}

static inline void xml_writer_decrease_indent ( xml_writer_t *this_ )
{
    if ( (*this_).indent_level > 0 )
    {
        (*this_).indent_level--;
        xml_writer_private_update_encoding_tables( this_ );
    }
    else
    {
        assert(false);
    }
}

static inline bool xml_writer_private_is_xml_tag_name_character ( xml_writer_t *this_, uint32_t codepoint, bool start )
{
    bool result = false;
    
    result
        = (( codepoint >= 'A' )&&( codepoint <= 'Z' ))  /* [A-Z] */
        || ( codepoint == '_' )  /* _ */
        || (( codepoint >= 'a' )&&( codepoint <= 'z' ))  /* [a-z] */
        || (( codepoint >= 0xC0 )&&( codepoint <= 0xD6 )) /* [#xC0-#xD6] */
        || (( codepoint >= 0xD8 )&&( codepoint <= 0xF6 )) /* [#xD8-#xF6] */
        || (( codepoint >= 0xF8 )&&( codepoint <= 0x2FF )) /* [#xF8-#x2FF] */
        || (( codepoint >= 0x370 )&&( codepoint <= 0x37D )) /* [#x370-#x37D] */
        || (( codepoint >= 0x37F )&&( codepoint <= 0x1FFF )) /* [#x37F-#x1FFF] */
        || (( codepoint >= 0x200C )&&( codepoint <= 0x200D )) /* [#x200C-#x200D] */
        || (( codepoint >= 0x2070 )&&( codepoint <= 0x218F )) /* [#x2070-#x218F] */
        || (( codepoint >= 0x2C00 )&&( codepoint <= 0x2FEF )) /* [#x2C00-#x2FEF] */
        || (( codepoint >= 0x3001 )&&( codepoint <= 0xD7FF )) /* [#x3001-#xD7FF] */
        || (( codepoint >= 0xF900 )&&( codepoint <= 0xFDCF )) /* [#xF900-#xFDCF] */
        || (( codepoint >= 0xFDF0 )&&( codepoint <= 0xFFFD )) /* [#xFDF0-#xFFFD] */
        || (( codepoint >= 0x10000 )&&( codepoint <= 0xEFFFF )); /* [#x10000-#xEFFFF] */
    
    if (( ! result )&&( ! start ))
    {
        /* after name start, more characters are allowed: */
        result 
            = ( codepoint == '-' )  /* - */
            || ( codepoint == '.' )  /* . */
            || (( codepoint >= '0' )&&( codepoint <= '9' ))  /* [0-9] */
            || ( codepoint == 0xB7 ) /* xB7 */
            || (( codepoint >= 0x0300 )&&( codepoint <= 0x036F )) /* [#x0300-#x036F] */
            || (( codepoint >= 0x203F )&&( codepoint <= 0x2040 )); /* [#x203F-#x2040] */
    }
    
    return result;
}


/*
Copyright 2019-2022 Andreas Warnke

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
