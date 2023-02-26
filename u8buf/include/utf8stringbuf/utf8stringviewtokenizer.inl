/* File: utf8stringviewtokenizer.inl; Copyright and License: see below */

#ifdef __cplusplus
extern "C" {
#endif

static inline void utf8stringviewtokenizer_init ( utf8stringviewtokenizer_t *this_,
                                                  utf8stringview_t input_text,
                                                  utf8stringviewtokenmode_t mode )
{
    (*this_).remaining_input_text = input_text;
    (*this_).mode = mode;
    utf8stringviewtokenizer_private_skip_space( this_ );
}

static inline void utf8stringviewtokenizer_destroy ( utf8stringviewtokenizer_t *this_ )
{
}

static inline bool utf8stringviewtokenizer_has_next ( const utf8stringviewtokenizer_t *this_ )
{
    return ( utf8stringview_get_length( (*this_).remaining_input_text ) != 0 );
}

static inline bool utf8stringviewtokenizer_private_is_space( char ascii )
{
    const unsigned char u_asc = (unsigned char) ascii;
    return ( u_asc < 0x21 );
}

static inline bool utf8stringviewtokenizer_private_is_standalone( char ascii )
{
    const unsigned char u_asc = (unsigned char) ascii;
    return (( u_asc > 0x20 )&&( u_asc < 0x30 )) || (( u_asc > 0x39 )&&( u_asc < 0x41 )) || (( u_asc > 0x5a )&&( u_asc < 0x61 )) || (( u_asc > 0x7a )&&( u_asc <= 0x7f ));
}

static inline utf8stringview_t utf8stringviewtokenizer_next ( utf8stringviewtokenizer_t *this_ )
{
    utf8stringview_t result;
    const char *const tok_start = utf8stringview_get_start( (*this_).remaining_input_text );
    const size_t len = utf8stringview_get_length( (*this_).remaining_input_text );
    if ( len > 0 )
    {
        const char tok_first = *tok_start;
        bool end_found = false;
        size_t tok_len = 1;
        for ( size_t probe_idx = 0; ( probe_idx < len )&&( ! end_found ); probe_idx ++ )
        {
            end_found
                = utf8stringviewtokenizer_private_is_space( tok_start[probe_idx] )
                || utf8stringviewtokenizer_private_is_standalone( tok_start[probe_idx] );
            tok_len = (probe_idx==0) ? 1 : probe_idx;
        }
        if ( end_found )
        {
            result = UTF8STRINGVIEW(tok_start,tok_len);
            (*this_).remaining_input_text = UTF8STRINGVIEW(tok_start+tok_len,len-tok_len);
            utf8stringviewtokenizer_private_skip_space( this_ );
        }
        else
        {
            result = (*this_).remaining_input_text;
            (*this_).remaining_input_text = UTF8STRINGVIEW_NULL;
        }
    }
    else
    {
        result = UTF8STRINGVIEW_NULL;
    }
    return result;
}

static inline void utf8stringviewtokenizer_private_skip_space ( utf8stringviewtokenizer_t *this_ )
{
    const  char *start = utf8stringview_get_start( (*this_).remaining_input_text );
    size_t len = utf8stringview_get_length( (*this_).remaining_input_text );
    while ( ( len > 0 ) && ( ((unsigned char)(*start)) < 0x21 ) ) /* 0x0 - 0x19 are control chars like line break and tab, 0x20 is space */
    {
        len --;
        start ++;
    }
    (*this_).remaining_input_text = UTF8STRINGVIEW(start,len);
}

#ifdef __cplusplus
}
#endif


/*
Copyright 2023-2023 Andreas Warnke

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
