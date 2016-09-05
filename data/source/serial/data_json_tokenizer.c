/* File: data_json_tokenizer.c; Copyright and License: see below */

#include "serial/data_json_tokenizer.h"
#include "trace.h"
#include "log.h"
#include <string.h>
#include <assert.h>

void data_json_tokenizer_init ( data_json_tokenizer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void data_json_tokenizer_reinit ( data_json_tokenizer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void data_json_tokenizer_destroy ( data_json_tokenizer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

data_error_t data_json_tokenizer_is_begin_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *begin_object )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != begin_object );
    data_error_t result_err = DATA_ERROR_NONE;
    data_error_t test_err;
    data_json_value_type_t value_type = DATA_JSON_VALUE_TYPE_UNDEF;

    test_err = data_json_tokenizer_get_value_type ( this_, in_data, io_read_pos, &value_type );
    if ( DATA_ERROR_PARSER_STRUCTURE == test_err )
    {
        /* there is no value-start as next token */
        *begin_object = false;
    }
    else if ( DATA_ERROR_NONE == test_err )
    {
        if ( DATA_JSON_VALUE_TYPE_OBJECT == value_type )
        {
            /* move the read pointer forward: */
            result_err = data_json_tokenizer_expect_begin_object ( this_, in_data, io_read_pos );
            *begin_object = true;
        }
        else
        {
            *begin_object = false;
        }
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_begin_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    char begin_object = in_data[*io_read_pos];
    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_OBJECT == begin_object )
    {
        /* expected token found */
        (*io_read_pos) ++;
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param name name of the expected "member-name". This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no or the wrong member-name token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_member_name ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, const char *name )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != name );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }
    
    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param out_name return value: name of the "member-name"
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_name does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no member-name token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_member_name ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_name )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param out_name return value: true if the next token is an "end-object" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_end_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_object )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != end_object );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_end_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    char end_object = in_data[*io_read_pos];
    if ( DATA_JSON_CONSTANTS_CHAR_END_OBJECT == end_object )
    {
        /* expected token found */
        (*io_read_pos) ++;
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param begin_array return value: true if the next token is a "begin-array" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_begin_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *begin_array )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != begin_array );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_begin_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    char begin_array = in_data[*io_read_pos];
    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_ARRAY == begin_array )
    {
        /* expected token found */
        (*io_read_pos) ++;
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param end_array return value: true if the next token is an "end-array" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_end_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_array )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != end_array );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );


    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no end-array token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_end_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    char end_array = in_data[*io_read_pos];
    if ( DATA_JSON_CONSTANTS_CHAR_END_ARRAY == end_array )
    {
        /* expected token found */
        (*io_read_pos) ++;
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_get_value_type ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, data_json_value_type_t *value_type )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != value_type );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    /* determine next token */
    char current = in_data[*io_read_pos];
    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_OBJECT == current )
    {
        *value_type = DATA_JSON_VALUE_TYPE_OBJECT;
    }
    else if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_ARRAY == current )
    {
        *value_type = DATA_JSON_VALUE_TYPE_ARRAY;
    }
    else if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_STRING == current )
    {
        *value_type = DATA_JSON_VALUE_TYPE_STRING;
    }
    else if ( 0 == strncmp( &(in_data[*io_read_pos]), DATA_JSON_CONSTANTS_FALSE, sizeof(DATA_JSON_CONSTANTS_FALSE)-1 ) )
    {
        *value_type = DATA_JSON_VALUE_TYPE_BOOLEAN;
    }
    else if ( 0 == strncmp( &(in_data[*io_read_pos]), DATA_JSON_CONSTANTS_TRUE, sizeof(DATA_JSON_CONSTANTS_TRUE)-1 ) )
    {
        *value_type = DATA_JSON_VALUE_TYPE_BOOLEAN;
    }
    else if ( 0 == strncmp( &(in_data[*io_read_pos]), DATA_JSON_CONSTANTS_NULL, sizeof(DATA_JSON_CONSTANTS_NULL)-1 ) )
    {
        *value_type = DATA_JSON_VALUE_TYPE_NULL;
    }
    else if ( ( '-' == current ) || (( '0' <= current ) && ( current <= '9')) )
    {
        /* assume integer */
        *value_type = DATA_JSON_VALUE_TYPE_INTEGER;

        /* check if there is an 'e', 'E' or '.' in the number */
        bool finished = false;
        uint32_t num_pos;
        for ( uint32_t num_pos = *io_read_pos; ( ! finished ) && ( num_pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); num_pos ++ )
        {
            char probe = in_data[num_pos];
            if ( ( '.' == probe ) || ( 'e' == probe ) || ( 'E' == probe ) )
            {
                *value_type = DATA_JSON_VALUE_TYPE_NUMBER;
                finished = true;
            }
            else if (( probe < '0' )||( '9' < probe ))
            {
                finished = true;
            }
        }
    }
    else
    {
        *value_type = DATA_JSON_VALUE_TYPE_UNDEF;
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param out_value return value: string-contents of the value-token
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_value does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no string-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_string_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_value )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param out_int return value: integer-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no integer-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_int_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, int64_t *out_int )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != out_int );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param out_number return value: number-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no number-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_number_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, double *out_number )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != out_number );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    /* rfc
     *
     * number = [ minus ] int [ frac ] [ exp ]
     *
     * decimal-point = %x2E       ; .
     *
     * digit1-9 = %x31-39         ; 1-9
     *
     * e = %x65 / %x45            ; e E
     *
     * exp = e [ minus / plus ] 1*DIGIT
     *
     * frac = decimal-point 1*DIGIT
     *
     * int = zero / ( digit1-9 *DIGIT )
     *
     * minus = %x2D               ; -
     *
     * plus = %x2B                ; +
     *
     * zero = %x30                ; 0
     */

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param out_bool return value: boolean-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no boolean-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_boolean_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *out_bool )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != out_bool );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no null token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_null_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, io_read_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

/*  \param end_array return value: true if the next token is EOF. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_eof ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *eof )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != eof );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    bool finished = false;
    uint32_t pos;
    for ( pos = *io_read_pos; ( ! finished ) && ( pos < DATA_JSON_TOKENIZER_MAX_INPUT_SIZE ); pos ++ )
    {
    }
    *io_read_pos = pos;

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_eof ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    char eof = in_data[*io_read_pos];
    if ( '\0' == eof )
    {
        /* expected token found */
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}


/*
Copyright 2016-2016 Andreas Warnke

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
