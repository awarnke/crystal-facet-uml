/* File: utf8stringviewtokenizer.inl; Copyright and License: see below */

#ifdef __cplusplus
extern "C" {
#endif

static inline void utf8stringviewtokenizer_init ( utf8stringviewtokenizer_t *this_,
                                                  const utf8stringview_t *input_text,
                                                  utf8stringviewtokenmode_t mode )
{
    assert( input_text != NULL );
    (*this_).remaining_input_text = *input_text;
    (*this_).mode = mode;
    (*this_).last_token_line = 0;
    (*this_).current_line = 1;
    utf8stringviewtokenizer_private_skip_space( this_ );
}

static inline void utf8stringviewtokenizer_destroy ( utf8stringviewtokenizer_t *this_ )
{
}

static inline bool utf8stringviewtokenizer_has_next ( const utf8stringviewtokenizer_t *this_ )
{
    return ( utf8stringview_get_length( &((*this_).remaining_input_text) ) != 0 );
}

static inline utf8stringviewtokenmode_t utf8stringviewtokenizer_get_mode ( utf8stringviewtokenizer_t *this_ )
{
    return (*this_).mode;
}

static inline void utf8stringviewtokenizer_set_mode ( utf8stringviewtokenizer_t *this_, utf8stringviewtokenmode_t mode )
{
    (*this_).mode = mode;
}

static inline utf8stringview_t utf8stringviewtokenizer_next ( utf8stringviewtokenizer_t *this_ )
{
    utf8stringview_t result;
    (*this_).last_token_line = (*this_).current_line;
    const char *const tok_start = utf8stringview_get_start( &((*this_).remaining_input_text) );
    const size_t len = utf8stringview_get_length( &((*this_).remaining_input_text) );
    if ( len > 0 )
    {
        size_t tok_len = 0;
        bool end_found = false;
        /* check for numbers */
        if ( (*this_).mode != UTF8STRINGVIEWTOKENMODE_TEXT )
        {
            const size_t num_len = utf8stringviewtokenizer_private_get_number_len( this_ );
            if ( num_len != 0 )
            {
                tok_len = num_len;
                end_found = true;
            }
        }
        /* check for special characters / standalone-tokens */
        if ( ! end_found )
        {
            const bool is_stanalone
                = ( (*this_).mode == UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY ) || utf8stringviewtokenizer_private_is_standalone( this_, tok_start[0] );
            if ( is_stanalone )
            {
                tok_len = 1;
                end_found = true;
            }
        }
        /* check for alphanumerical tokens, end when other token found */
        for ( size_t probe_idx = 1; ( probe_idx < len )&&( ! end_found ); probe_idx ++ )
        {
            end_found
                = utf8stringviewtokenizer_private_is_space( this_, tok_start[probe_idx] )
                || utf8stringviewtokenizer_private_is_standalone( this_, tok_start[probe_idx] );
            tok_len = probe_idx;
        }
        /* determine result */
        if ( end_found )
        {
            result = UTF8STRINGVIEW(tok_start,tok_len);
            (*this_).remaining_input_text = UTF8STRINGVIEW(tok_start+tok_len,len-tok_len);
            utf8stringviewtokenizer_private_skip_space( this_ );
        }
        else
        {
            result = (*this_).remaining_input_text;
            (*this_).remaining_input_text = UTF8STRINGVIEW_EMPTY;
        }
    }
    else
    {
        result = UTF8STRINGVIEW_EMPTY;
    }
    return result;
}

static inline uint32_t utf8stringviewtokenizer_get_line ( const utf8stringviewtokenizer_t *this_ )
{
    return (*this_).last_token_line;
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

enum utf8stringviewtokenizer_private_number_passed_enum {
    UTF8STRINGVIEWTOKENIZER_INIT = 0,  /*!< nothing passed yet */
    UTF8STRINGVIEWTOKENIZER_MANT_SIGN = 1,  /*!< sign of mantissa passed */
    UTF8STRINGVIEWTOKENIZER_MANT_INT = 2,  /*!< some integer portion of mantissa passed */
    UTF8STRINGVIEWTOKENIZER_END_INT = 3,  /*!< whatever valid integer number might have been processed, we are beyond now */
    UTF8STRINGVIEWTOKENIZER_MANT_POINT = 4,  /*!< decimal point of mantissa passed */
    UTF8STRINGVIEWTOKENIZER_MANT_FRACT = 5,  /*!< some fraction of mantissa passed */
    UTF8STRINGVIEWTOKENIZER_BASE = 6,  /*!< base e identifier passed */
    UTF8STRINGVIEWTOKENIZER_EXP_SIGN = 7,  /*!< sign of exponent passed */
    UTF8STRINGVIEWTOKENIZER_EXP_INT = 8,  /*!< some integer portion of exponent passed */
    UTF8STRINGVIEWTOKENIZER_INFINITY = 9,  /*!< name of Infinity being processed */
    UTF8STRINGVIEWTOKENIZER_NAN = 10,  /*!< name of NaN being processed */
    UTF8STRINGVIEWTOKENIZER_END_FLOAT = 11,  /*!< whatever valid float number might have been processed, we are beyond now */
};

static inline size_t utf8stringviewtokenizer_private_get_number_len( utf8stringviewtokenizer_t *this_ )
{
    const char *start = utf8stringview_get_start( &((*this_).remaining_input_text) );
    const size_t len = utf8stringview_get_length( &((*this_).remaining_input_text) );
    enum utf8stringviewtokenizer_private_number_passed_enum state = UTF8STRINGVIEWTOKENIZER_INIT;
    const bool float_mode = (( (*this_).mode == UTF8STRINGVIEWTOKENMODE_FLOAT )||( (*this_).mode == UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY ));
    const enum utf8stringviewtokenizer_private_number_passed_enum end_state
        = float_mode ? UTF8STRINGVIEWTOKENIZER_END_FLOAT : UTF8STRINGVIEWTOKENIZER_END_INT;
    size_t valid_len = 0;

    for ( size_t probe_idx = 0; ( probe_idx < len )&&( state < end_state ); probe_idx ++ )
    {
        char probe = start[probe_idx];
        /* printf("state:%i\n",state); */
        switch ( state )
        {
            case UTF8STRINGVIEWTOKENIZER_INIT:
            {
                if (( probe == '+' )||( probe == '-' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_MANT_SIGN;
                }
                else if (( probe >= '0' )&&( probe <= '9' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_MANT_INT;
                    valid_len = probe_idx+1;
                }
                else if (( probe == 'i' )||( probe == 'I' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_INFINITY;
                }
                else if (( probe == 'n' )||( probe == 'N' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_NAN;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_MANT_SIGN:
            {
                if (( probe >= '0' )&&( probe <= '9' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_MANT_INT;
                    valid_len = probe_idx+1;
                }
                else if (( probe == 'i' )||( probe == 'I' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_INFINITY;
                }
                else
                {
                    /* a mantissa has to have at least i digit in the integer portion */
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_MANT_INT:
            {
                if (( probe >= '0' )&&( probe <= '9' ))
                {
                    /* stay in state = UTF8STRINGVIEWTOKENIZER_MANT_INT; */
                    valid_len = probe_idx+1;
                }
                else if ( probe == '.' )
                {
                    state = UTF8STRINGVIEWTOKENIZER_MANT_POINT;
                    if ( float_mode )  /* do not consider the point as valid in integer mode */
                    {
                        valid_len = probe_idx+1;
                    }
                }
                else if (( probe == 'e' )||( probe == 'E' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_BASE;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_END_INT:
            {
                /* finished, the for loop is ended */
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_MANT_POINT:
            {
                if (( probe >= '0' )&&( probe <= '9' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_MANT_FRACT;
                    valid_len = probe_idx+1;
                }
                else if (( probe == 'e' )||( probe == 'E' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_BASE;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_MANT_FRACT:
            {
                if (( probe >= '0' )&&( probe <= '9' ))
                {
                    /* stay in state = UTF8STRINGVIEWTOKENIZER_MANT_FRACT; */
                    valid_len = probe_idx+1;
                }
                else if (( probe == 'e' )||( probe == 'E' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_BASE;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_BASE:
            {
                if (( probe == '+' )||( probe == '-' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_EXP_SIGN;
                }
                else if (( probe >= '0' )&&( probe <= '9' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_EXP_INT;
                    valid_len = probe_idx+1;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_EXP_SIGN:
            {
                if (( probe >= '0' )&&( probe <= '9' ))
                {
                    state = UTF8STRINGVIEWTOKENIZER_EXP_INT;
                    valid_len = probe_idx+1;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_EXP_INT:
            {
                if (( probe >= '0' )&&( probe <= '9' ))
                {
                    /* stay in state = UTF8STRINGVIEWTOKENIZER_EXP_INT; */
                    valid_len = probe_idx+1;
                }
                else
                {
                    state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
                }
            }
            break;

            case UTF8STRINGVIEWTOKENIZER_INFINITY:
            {
                /* the first character has already been processed */
                const size_t infinity_len = strlen("nfinity");
                if ( (probe_idx + infinity_len) <= len )
                {
                    const bool is_infinity
                        = ( 0 == memcmp( start+probe_idx, "nfinity", infinity_len ) )
                        ||( 0 == memcmp( start+probe_idx, "NFINITY", infinity_len ) );
                    if (is_infinity)
                    {
                        valid_len = probe_idx+infinity_len;
                    }
                }
                state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
            }

            case UTF8STRINGVIEWTOKENIZER_NAN:
            {
                /* the first character has already been processed */
                const size_t nan_len = strlen("aN");
                if ( (probe_idx + nan_len) <= len )
                {
                    const bool is_nan
                        = ( 0 == memcmp( start+probe_idx, "an", nan_len ) )
                        ||( 0 == memcmp( start+probe_idx, "aN", nan_len ) )
                        ||( 0 == memcmp( start+probe_idx, "AN", nan_len ) );
                    if (is_nan)
                    {
                        valid_len = probe_idx+nan_len;
                    }
                }
                state = UTF8STRINGVIEWTOKENIZER_END_FLOAT;
            }

            case UTF8STRINGVIEWTOKENIZER_END_FLOAT:
            {
                /* finished, the for loop is ended */
            }
            break;
        }
    }
    return valid_len;
}

static inline void utf8stringviewtokenizer_private_skip_space ( utf8stringviewtokenizer_t *this_ )
{
    const char *start = utf8stringview_get_start( &((*this_).remaining_input_text) );
    size_t len = utf8stringview_get_length( &((*this_).remaining_input_text) );
    while ( ( len > 0 ) && ( utf8stringviewtokenizer_private_is_space( this_, *start ) ) )
    {
        if ( *start == '\n' )
        {
            (*this_).current_line ++;
        }
        len --;
        start ++;
    }
    (*this_).remaining_input_text = UTF8STRINGVIEW(start,len);
}

#ifdef __cplusplus
}
#endif


/*
Copyright 2023-2024 Andreas Warnke

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
