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

u8_error_t draw_line_breaker_process( const draw_line_breaker_t *this_,
                                     const utf8stringview_t *in_text,
                                     utf8stringbuf_t *out_text )
{
    u8_error_t err = U8_ERROR_NONE;
    utf8codepointiterator_t iter;
    utf8codepointiterator_init( &iter, in_text );

    utf8stringbuf_clear( *out_text );

    const utf8codepoint_t space0width = utf8codepoint( 0x200B );
    utf8codepoint_t last = utf8codepoint( ' ' );
    unsigned int wordlen = 0;
    for(; utf8codepointiterator_has_next( &iter ); )
    {
        utf8codepoint_t codepnt = utf8codepointiterator_next( &iter );
        const bool no_space
            = ( utf8codepoint_get_char( last ) != ' ' )
            &&( utf8codepoint_get_char ( codepnt ) != ' ' );
        const bool insert_zws = no_space && ( wordlen >= 5 ); /* zws: zero width space */
        if ( insert_zws )
        {
            err |= utf8stringbuf_append_char( *out_text, utf8codepoint_get_char( space0width ) );
            wordlen = 0;
        }
        else if ( ! no_space )
        {
            wordlen = 0;
        }
        err |= utf8stringbuf_append_char( *out_text, utf8codepoint_get_char( codepnt ) );
        wordlen ++;
        last = codepnt;
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
