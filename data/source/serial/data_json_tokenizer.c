/* File: data_json_tokenizer.c; Copyright and License: see below */

#include "serial/data_json_tokenizer.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <assert.h>

static const char * const DATA_JSON_TOKENIZER_PRIVATE_DECODE_JSON_STRINGS[] = {
    "\\t", "\x09",  /* tab */
    "\\n", "\x0a",  /* newline */
    "\\r", "\x0d",  /* return */
    "\\b", "\x08",  /* backspace */
    "\\f", "\x0c",  /* form feed */
    "\\\"", "\"",  /* double quote */
    "\\\\", "\\",  /* backslash*/
    "\\/", "/",  /* slash */
    NULL,  /* for JSON, see rfc7159 */
};

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

data_error_t data_json_tokenizer_expect_begin_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_OBJECT == in_data[*io_read_pos] )
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

data_error_t data_json_tokenizer_get_member_name ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_name )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;
    utf8error_t str_err = UTF8ERROR_SUCCESS;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    /* expect string begin */
    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_STRING == in_data[*io_read_pos] )
    {
        /* expected token found */
        uint32_t start = (*io_read_pos)+1;
        uint32_t end = start;

        data_json_tokenizer_private_find_string_end ( this_, in_data, &end );

        /* expect string end */
        if ( DATA_JSON_CONSTANTS_CHAR_END_STRING == in_data[end] )
        {
            uint32_t token_end = end+1;
            if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &token_end ) )
            {
                result_err = DATA_ERROR_LEXICAL_STRUCTURE;
            }
            else
            {
                /* we are happy */
                (*io_read_pos) = token_end;

                /* copy and unescape the string */
                str_err |= utf8stringbuf_copy_region_from_str( out_name, in_data, start, end-start );
                str_err |= utf8stringbuf_replace_all( out_name,
                                                      DATA_JSON_TOKENIZER_PRIVATE_DECODE_JSON_STRINGS );
                if ( UTF8ERROR_SUCCESS != str_err )
                {
                    result_err = DATA_ERROR_STRING_BUFFER_EXCEEDED;
                }
                else
                {
                    TRACE_INFO_STR( "member name:", utf8stringbuf_get_string( out_name ) );
                }
            }
        }
        else
        {
            /* expected end token missing */
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
    }
    else
    {
        /* expected start token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_is_end_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_object )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != end_object );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    if ( DATA_JSON_CONSTANTS_CHAR_END_OBJECT == in_data[*io_read_pos] )
    {
        /* object-end token found */
        (*io_read_pos) ++;
        (*end_object) = true;
        TRACE_INFO( "end object: true" );
    }
    else
    {
        /* expected token missing */
        (*end_object) = false;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_name_separator ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    if ( DATA_JSON_CONSTANTS_CHAR_NAME_SEPARATOR == in_data[*io_read_pos] )
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

data_error_t data_json_tokenizer_expect_begin_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_ARRAY == in_data[*io_read_pos] )
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

data_error_t data_json_tokenizer_is_end_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_array )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != end_array );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    if ( DATA_JSON_CONSTANTS_CHAR_END_ARRAY == in_data[*io_read_pos] )
    {
        /* array-end token found */
        (*io_read_pos) ++;
        (*end_array) = true;
        TRACE_INFO( "end array: true" );
    }
    else
    {
        /* expected token missing */
        (*end_array) = false;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_value_separator ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    if ( DATA_JSON_CONSTANTS_CHAR_VALUE_SEPARATOR == in_data[*io_read_pos] )
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

data_error_t data_json_tokenizer_get_string_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_value )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;
    utf8error_t str_err;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    /* expect string begin */
    if ( DATA_JSON_CONSTANTS_CHAR_BEGIN_STRING == in_data[*io_read_pos] )
    {
        /* expected token found */
        uint32_t start = (*io_read_pos)+1;
        uint32_t end = start;

        data_json_tokenizer_private_find_string_end ( this_, in_data, &end );

        /* expect string end */
        if ( DATA_JSON_CONSTANTS_CHAR_END_STRING == in_data[end] )
        {
            uint32_t token_end = end+1;
            if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &token_end ) )
            {
                result_err = DATA_ERROR_LEXICAL_STRUCTURE;
            }
            else
            {
                /* we are happy */
                (*io_read_pos) = token_end;

                /* copy and unescape the string */
                str_err |= utf8stringbuf_copy_region_from_str( out_value, in_data, start, end-start );
                str_err |= utf8stringbuf_replace_all( out_value,
                                                      DATA_JSON_TOKENIZER_PRIVATE_DECODE_JSON_STRINGS );
                if ( UTF8ERROR_SUCCESS != str_err )
                {
                    result_err = DATA_ERROR_STRING_BUFFER_EXCEEDED;
                }
            }
        }
        else
        {
            /* expected end token missing */
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
    }
    else
    {
        /* expected start token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_get_int_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, int64_t *out_int )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != out_int );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    uint32_t new_pos = *io_read_pos;
    int64_t parsed_int;
    parsed_int = data_json_tokenizer_private_parse_integer( this_, in_data, &new_pos );

    if ( new_pos <= *io_read_pos )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }
    else if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &new_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }
    else
    {
        (*out_int) = parsed_int;
        (*io_read_pos) = new_pos;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_get_number_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, double *out_number )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != out_number );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    uint32_t new_pos = *io_read_pos;
    data_json_tokenizer_private_skip_number( this_, in_data, &new_pos );

    if ( new_pos <= *io_read_pos )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }
    else if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &new_pos ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }
    else
    {
        (*out_number) = 0.0;
        (*io_read_pos) = new_pos;
        result_err = DATA_ERROR_NOT_YET_IMPLEMENTED;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_get_boolean_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *out_bool )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    assert( NULL != out_bool );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    uint32_t new_pos = *io_read_pos;
    if ( 0 == strncmp( &(in_data[*io_read_pos]), DATA_JSON_CONSTANTS_FALSE, sizeof(DATA_JSON_CONSTANTS_FALSE)-1 ) )
    {
        new_pos += sizeof(DATA_JSON_CONSTANTS_FALSE)-1;
        if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &new_pos ) )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
        else
        {
            (*out_bool) = false;
            (*io_read_pos) = new_pos;
        }
    }
    else if ( 0 == strncmp( &(in_data[*io_read_pos]), DATA_JSON_CONSTANTS_TRUE, sizeof(DATA_JSON_CONSTANTS_TRUE)-1 ) )
    {
        new_pos += sizeof(DATA_JSON_CONSTANTS_TRUE)-1;
        if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &new_pos ) )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
        else
        {
            (*out_bool) = true;
            (*io_read_pos) = new_pos;
        }
    }
    else
    {
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t data_json_tokenizer_expect_null_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != in_data );
    assert( NULL != io_read_pos );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    data_json_tokenizer_private_skip_whitespace( this_, in_data, io_read_pos );

    uint32_t new_pos = *io_read_pos;
    if ( 0 == strncmp( &(in_data[*io_read_pos]), DATA_JSON_CONSTANTS_NULL, sizeof(DATA_JSON_CONSTANTS_NULL)-1 ) )
    {
        new_pos += sizeof(DATA_JSON_CONSTANTS_NULL)-1;
        if ( ! data_json_tokenizer_private_is_token_end( this_, in_data, &new_pos ) )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
        else
        {
            (*io_read_pos) = new_pos;
        }
    }
    else
    {
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

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
