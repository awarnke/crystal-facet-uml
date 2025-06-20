/* File: utf8stringlines.inl; Copyright and License: see below */

#ifdef __cplusplus
extern "C" {
#endif

static inline void utf8stringlines_init ( utf8stringlines_t *this_, const utf8stringview_t *lines_list, uint32_t line_length )
{
    assert( lines_list != NULL );
    (*this_).remaining = *lines_list;
    (*this_).line_length = line_length;
    (*this_).next_is_end = false;
    (*this_).has_next = true;
    utf8stringlines_private_step_to_next( this_ );
}

static inline void utf8stringlines_destroy ( utf8stringlines_t *this_ )
{
}

static inline bool utf8stringlines_has_next ( const utf8stringlines_t *this_ )
{
    return (*this_).has_next;
}

static inline utf8stringview_t utf8stringlines_next ( utf8stringlines_t *this_ )
{
    utf8stringview_t result = (*this_).next;
    utf8stringlines_private_step_to_next( this_ );
    return result;
}

static inline void utf8stringlines_private_step_to_next ( utf8stringlines_t *this_ )
{
    if ( (*this_).next_is_end )
    {
        (*this_).has_next = false;
        (*this_).next = UTF8STRINGVIEW_EMPTY;
    }
    else
    {
        /* search good line end */
        uint_fast32_t line_end_pos = 0;
        uint_fast32_t a_good_pos = 0;
        uint_fast32_t codepoints = 0;
        const char *start = utf8stringview_get_start( &((*this_).remaining) );
        const size_t len = utf8stringview_get_length( &((*this_).remaining) );
        for ( uint_fast32_t probe_idx = 0; ( probe_idx < len )&&( line_end_pos == 0 ); probe_idx ++ )
        {
            char probe = start[probe_idx];

            /* analyze the current character */
            if (( 0xc0 & probe ) == 0x80 )
            {
                /* This is not a first byte of an utf8-character byte sequence; check for asian line break possibility */
                if ( probe_idx >= 2 )
                {
                    if ( utf8stringlines_private_is_ideographic_comma( this_, start[probe_idx-2], start[probe_idx-1], probe ) )
                    {
                        a_good_pos = probe_idx + 1;
                    }
                }
            }
            else
            {
                codepoints ++;
                if ( utf8stringlines_private_is_space( this_, probe ) )
                {
                    /* this can only happen for 1-byte code points */
                    a_good_pos = probe_idx + 1;
                }
            }

            /* evaluate if this is a good cutting point */
            if ( probe == '\n' )
            {
                line_end_pos = probe_idx + 1;
            }
            else if ( codepoints >= (*this_).line_length )
            {
                /* we are beyond the limit */
                /* take the best we have till now */
                line_end_pos = a_good_pos;
            }
        }

        /* cut stringview at good line end position */
        if ( line_end_pos != 0 )
        {
            utf8stringview_t before = UTF8STRINGVIEW_EMPTY;
            utf8stringview_t after = UTF8STRINGVIEW_EMPTY;

            utf8error_t err1 = utf8stringview_init_region( &before, start, 0 /*start_idx*/, line_end_pos /*length*/ );
            assert( err1 == UTF8ERROR_SUCCESS );
            (void) err1;  /* ok to ignore an error - should not happen */
            utf8error_t err2 = utf8stringview_init_region( &after, start, line_end_pos /*start_idx*/, len-line_end_pos /*length*/ );
            assert( err2 == UTF8ERROR_SUCCESS );
            (void) err2;  /* ok to ignore an error - should not happen */

            (*this_).next_is_end = ( 0 == utf8stringview_get_length( &after ));
            (*this_).next = before;
            (*this_).remaining = after;
        }
        else
        {
            /* no suitable line end found */
            (*this_).next_is_end = true;
            (*this_).next = (*this_).remaining;
            (*this_).remaining = UTF8STRINGVIEW_EMPTY;
        }
    }
}

static inline bool utf8stringlines_private_is_space( utf8stringlines_t *this_, char ascii )
{
    const unsigned char u_asc = (unsigned char) ascii;
    /* 0x0 - 0x19 are control chars like line break and tab, 0x20 is space, 0x7f is a control character */
    return ( u_asc <= 0x20 )||( u_asc == 0x7f );
}

static inline bool utf8stringlines_private_is_ideographic_comma( utf8stringlines_t *this_,
                                                                 char utf8_first,
                                                                 char utf8_second,
                                                                 char utf8_third )
{
    /* note: a full coverage of unicode is more complicated, */
    /* see https://stackoverflow.com/questions/9506869/are-there-character-collections-for-all-international-full-stop-punctuations */
    /* this function only covers a small set of use cases: */
    /* U+03000 IDEOGRAPHIC SPACE (maybe not needed?) */
    const bool is_ideo_space = ( utf8_first == 0xe3 )&&( utf8_second == 0x80 )&&( utf8_third == 0x80 );
    /* U+03002 IDEOGRAPHIC FULL STOP/COMMA (both seem frequently used) */
    const bool is_ideo_comma = ( utf8_first == 0xe3 )&&( utf8_second == 0x80 )&&( utf8_third == 0x81 );
    const bool is_ideo_fullstop = ( utf8_first == 0xe3 )&&( utf8_second == 0x80 )&&( utf8_third == 0x82 );
    /* U+0FF0E FULLWIDTH FULL STOP/COMMA (both seem frequently used) */
    const bool is_full_comma = ( utf8_first == 0xef )&&( utf8_second == 0xbc )&&( utf8_third == 0x8c );
    const bool is_full_fullstop = ( utf8_first == 0xef )&&( utf8_second == 0xbc )&&( utf8_third == 0x8e );
    /* U+0FF61 HALFWIDTH IDEOGRAPHIC FULL STOP/COMMA (maybe not needed?)*/
    const bool is_half_comma = ( utf8_first == 0xef )&&( utf8_second == 0xbd )&&( utf8_third == 0xa4 );
    const bool is_half_fullstop = ( utf8_first == 0xef )&&( utf8_second == 0xbd )&&( utf8_third == 0xa1 );
    return is_ideo_space || is_ideo_comma || is_ideo_fullstop || is_full_comma || is_full_fullstop || is_half_comma || is_half_fullstop;
}

#ifdef __cplusplus
}
#endif


/*
Copyright 2025-2025 Andreas Warnke

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
