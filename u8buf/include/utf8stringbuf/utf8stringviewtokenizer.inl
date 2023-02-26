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

static inline bool utf8stringviewtokenizer_private_is_space( utf8stringviewtokenizer_t *this_, char ascii )
{
    const unsigned char u_asc = (unsigned char) ascii;
    /* 0x0 - 0x19 are control chars like line break and tab, 0x20 is space, 0x7f is a control character */
    return ( u_asc <= 0x20 )||( u_asc == 0x7f );
}

static inline bool utf8stringviewtokenizer_private_is_standalone( utf8stringviewtokenizer_t *this_, char ascii )
{
    const unsigned char u_asc = (unsigned char) ascii;
    return (( u_asc >= 0x21 )&&( u_asc <= 0x2f )) || (( u_asc >= 0x3a )&&( u_asc <= 0x40 )) || (( u_asc >= 0x5b )&&( u_asc <= 0x5e ))
        || (( u_asc == 0x60 )) || (( u_asc >= 0x7b )&&( u_asc <= 0x7e ));
}

static inline size_t utf8stringviewtokenizer_private_get_number_len( utf8stringviewtokenizer_t *this_ )
{
    const unsigned char u_asc = (unsigned char) *(utf8stringview_get_start( (*this_).remaining_input_text ));
    return (size_t) 13;
}

static inline utf8stringview_t utf8stringviewtokenizer_next ( utf8stringviewtokenizer_t *this_ )
{
    utf8stringview_t result;
    const char *const tok_start = utf8stringview_get_start( (*this_).remaining_input_text );
    const size_t len = utf8stringview_get_length( (*this_).remaining_input_text );
    if ( len > 0 )
    {
        bool end_found = utf8stringviewtokenizer_private_is_standalone( this_, tok_start[0] );
        size_t tok_len = 1;
        for ( size_t probe_idx = 1; ( probe_idx < len )&&( ! end_found ); probe_idx ++ )
        {
            end_found
                = utf8stringviewtokenizer_private_is_space( this_, tok_start[probe_idx] )
                || utf8stringviewtokenizer_private_is_standalone( this_, tok_start[probe_idx] );
            tok_len = probe_idx;
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
    const char *start = utf8stringview_get_start( (*this_).remaining_input_text );
    size_t len = utf8stringview_get_length( (*this_).remaining_input_text );
    while ( ( len > 0 ) && ( utf8stringviewtokenizer_private_is_space( this_, *start ) ) )
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
