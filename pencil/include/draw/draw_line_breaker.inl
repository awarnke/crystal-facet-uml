/* File: draw_line_breaker.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8codepointiterator.h"
#include <assert.h>

static inline void draw_line_breaker_init( draw_line_breaker_t *this_, bool more )
{
    (*this_).more = more;
}

static inline void draw_line_breaker_destroy( draw_line_breaker_t *this_ )
{
}

u8_error_t draw_line_breaker_append( const draw_line_breaker_t *this_,
                                     const utf8stringview_t *in_text,
                                     utf8stringbuf_t out_text )
{
    u8_error_t err = U8_ERROR_NONE;
    utf8codepointiterator_t iter;
    utf8codepointiterator_init( &iter, in_text );

    const utf8codepoint_t space0width = utf8codepoint( 0x200B );
    uint32_t last = ' ';
    for(; utf8codepointiterator_has_next( &iter ); )
    {
        utf8codepoint_t codepnt = utf8codepointiterator_next( &iter );
        uint32_t current = utf8codepoint_get_char ( codepnt );
        const bool no_space = ( last != ' ' )&&( current != ' ' );
        if ( no_space )
        {
            const bool last_AZ = ( last >= 'A' )&&( last <= 'Z' );
            const bool last_az = ( last >= 'a' )&&( last <= 'z' );
            const bool last_09 = ( last >= '0' )&&( last <= '9' );
            const bool last_other = ( last > '\x7f' );
            const bool cur_AZ = ( current >= 'A' )&&( current <= 'Z' );
            const bool cur_az = ( current >= 'a' )&&( current <= 'z' );
            const bool cur_09 = ( current >= '0' )&&( current <= '9' );
            const bool cur_other = ( current > '\x7f' );
            const bool same_token
                = ( last_AZ && ( cur_AZ || cur_az || cur_09 ) )
                || ( last_az && ( cur_az || cur_09 ) )
                || ( last_09 && cur_09 )
                || ( last_other && cur_other );
            if ( ! same_token )
            {
                out_text = utf8stringbuf_get_end( out_text );
                err |= utf8stringbuf_append_char( out_text, utf8codepoint_get_char( space0width ) );
            }
        }
        err |= utf8stringbuf_append_char( out_text, utf8codepoint_get_char( codepnt ) );
        last = current;
    }

    utf8codepointiterator_destroy( &iter );
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
