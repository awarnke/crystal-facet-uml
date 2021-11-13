/* File: json_token_reader.c; Copyright and License: see below */

#include "json/json_token_reader.h"
#include "stream/universal_escaping_output_stream.h"
#include "stream/universal_memory_output_stream.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <assert.h>

static const char *const JSON_TOKENIZER_PRIVATE_DECODE_JSON_STRINGS[][2] = {
    { "\\t", "\x09" },  /* tab */
    { "\\n", "\x0a" },  /* newline */
    { "\\r", "\x0d" },  /* return */
    { "\\b", "\x08" },  /* backspace */
    { "\\f", "\x0c" },  /* form feed */
    { "\\\"", "\"" },  /* double quote */
    { "\\\\", "\\" },  /* backslash*/
    { "\\/", "/" },  /* slash */
    { NULL, NULL }  /* for JSON, see rfc7159 */
};

void json_token_reader_init ( json_token_reader_t *this_, universal_input_stream_t *in_stream )
{
    TRACE_BEGIN();
    assert( NULL != in_stream );

    (*this_).in_stream = in_stream;
    (*this_).ring_start = 0;
    (*this_).ring_length = 0;

    TRACE_END();
}

void json_token_reader_reinit ( json_token_reader_t *this_, universal_input_stream_t *in_stream )
{
    TRACE_BEGIN();
    assert( NULL != in_stream );

    (*this_).in_stream = in_stream;
    (*this_).ring_start = 0;
    (*this_).ring_length = 0;

    TRACE_END();
}

void json_token_reader_destroy ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).in_stream );

    (*this_).in_stream = NULL;

    TRACE_END();
}

data_error_t json_token_reader_expect_begin_object ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    if ( JSON_CONSTANTS_CHAR_BEGIN_OBJECT == json_token_reader_private_peek_next( this_ ) )
    {
        /* expected token found */
        json_token_reader_private_read_next( this_ );
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_is_end_object ( json_token_reader_t *this_, bool *end_object )
{
    TRACE_BEGIN();
    assert( NULL != end_object );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    if ( JSON_CONSTANTS_CHAR_END_OBJECT == json_token_reader_private_peek_next( this_ ) )
    {
        /* object-end token found */
        json_token_reader_private_read_next( this_ );
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

data_error_t json_token_reader_expect_name_separator ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    if ( JSON_CONSTANTS_CHAR_NAME_SEPARATOR == json_token_reader_private_peek_next( this_ ) )
    {
        /* expected token found */
        json_token_reader_private_read_next( this_ );
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_expect_begin_array ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    if ( JSON_CONSTANTS_CHAR_BEGIN_ARRAY == json_token_reader_private_peek_next( this_ ) )
    {
        /* expected token found */
        json_token_reader_private_read_next( this_ );
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_is_end_array ( json_token_reader_t *this_, bool *end_array )
{
    TRACE_BEGIN();
    assert( NULL != end_array );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    if ( JSON_CONSTANTS_CHAR_END_ARRAY == json_token_reader_private_peek_next( this_ ))
    {
        /* array-end token found */
        json_token_reader_private_read_next( this_ );
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

data_error_t json_token_reader_expect_value_separator ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    if ( JSON_CONSTANTS_CHAR_VALUE_SEPARATOR == json_token_reader_private_peek_next( this_ ) )
    {
        /* expected token found */
        json_token_reader_private_read_next( this_ );
    }
    else
    {
        /* expected token missing */
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_get_value_type ( json_token_reader_t *this_, json_value_type_t *value_type )
{
    TRACE_BEGIN();
    assert( NULL != value_type );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    /* determine next token */
    char current = json_token_reader_private_peek_next( this_ );
    if ( JSON_CONSTANTS_CHAR_BEGIN_OBJECT == current )
    {
        *value_type = JSON_VALUE_TYPE_OBJECT;
    }
    else if ( JSON_CONSTANTS_CHAR_BEGIN_ARRAY == current )
    {
        *value_type = JSON_VALUE_TYPE_ARRAY;
    }
    else if ( JSON_CONSTANTS_CHAR_BEGIN_STRING == current )
    {
        *value_type = JSON_VALUE_TYPE_STRING;
    }
    else if ( JSON_CONSTANTS_CHAR_BEGIN_FALSE == current )
    {
        *value_type = JSON_VALUE_TYPE_BOOLEAN;
    }
    else if ( JSON_CONSTANTS_CHAR_BEGIN_TRUE == current )
    {
        *value_type = JSON_VALUE_TYPE_BOOLEAN;
    }
    else if ( JSON_CONSTANTS_CHAR_BEGIN_NULL == current )
    {
        *value_type = JSON_VALUE_TYPE_NULL;
    }
    else if ( ( '-' == current ) || (( '0' <= current ) && ( current <= '9')) )
    {
        /* return number even if integer subtype */
        *value_type = JSON_VALUE_TYPE_NUMBER;
    }
    else
    {
        *value_type = JSON_VALUE_TYPE_UNDEF;
        result_err = DATA_ERROR_PARSER_STRUCTURE;  /* this could also be a lexical error (invalid next token) instead of unexpected next token */
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_get_string_value ( json_token_reader_t *this_, utf8stringbuf_t out_value )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    /* expect string begin */
    if ( JSON_CONSTANTS_CHAR_BEGIN_STRING == json_token_reader_private_peek_next( this_ ) )
    {
        /* expected token found */
        json_token_reader_private_read_next( this_ );

        universal_memory_output_stream_t mem_out;
        universal_memory_output_stream_init( &mem_out,
                                             utf8stringbuf_get_string(out_value),
                                             utf8stringbuf_get_size(out_value)
                                           );

        universal_escaping_output_stream_t esc_out;
        universal_escaping_output_stream_init( &esc_out,
                                               &JSON_TOKENIZER_PRIVATE_DECODE_JSON_STRINGS,
                                               universal_memory_output_stream_get_output_stream( &mem_out )
                                             );

        result_err = json_token_reader_private_read_string( this_,
                                                            universal_escaping_output_stream_get_output_stream( &esc_out )
                                                          );

        universal_escaping_output_stream_flush( &esc_out );
        universal_escaping_output_stream_destroy( &esc_out );
        const int out_err
            = universal_memory_output_stream_write_0term( &mem_out );
        if ( 0 != out_err )
        {
            result_err = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        universal_memory_output_stream_destroy( &mem_out );


        /* expect string end */
        const bool in_err
            = ( JSON_CONSTANTS_CHAR_END_STRING != json_token_reader_private_peek_next( this_ ) );
        if ( in_err )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
        else
        {
            json_token_reader_private_read_next( this_ );
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

data_error_t json_token_reader_get_int_value ( json_token_reader_t *this_, int64_t *out_int )
{
    TRACE_BEGIN();
    assert( NULL != out_int );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    int64_t parsed_int;
    result_err = json_token_reader_private_parse_integer( this_, &parsed_int );

    if ( ! json_token_reader_private_is_value_end( this_ ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_get_number_value ( json_token_reader_t *this_, double *out_number )
{
    TRACE_BEGIN();
    assert( NULL != out_number );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    result_err = json_token_reader_private_skip_number( this_ );  /* not yet implemented */

    if ( ! json_token_reader_private_is_value_end( this_ ) )
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }
    else
    {
        (*out_number) = 0.0;
        result_err = DATA_ERROR_NOT_YET_IMPLEMENTED;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_get_boolean_value ( json_token_reader_t *this_, bool *out_bool )
{
    TRACE_BEGIN();
    assert( NULL != out_bool );
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    char current = json_token_reader_private_peek_next( this_ );
    if ( JSON_CONSTANTS_CHAR_BEGIN_FALSE == current )
    {
        (*out_bool) = false;
        for ( int f_pos = 0; f_pos < sizeof(JSON_CONSTANTS_FALSE); f_pos ++ )
        {
            if ( JSON_CONSTANTS_FALSE[f_pos] != json_token_reader_private_read_next( this_ ) )
            {
                result_err = DATA_ERROR_LEXICAL_STRUCTURE;
            }
        }
        if ( ! json_token_reader_private_is_value_end( this_ ) )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
    }
    else if ( JSON_CONSTANTS_CHAR_BEGIN_TRUE == current )
    {
        (*out_bool) = true;
        for ( int t_pos = 0; t_pos < sizeof(JSON_CONSTANTS_TRUE); t_pos ++ )
        {
            if ( JSON_CONSTANTS_TRUE[t_pos] != json_token_reader_private_read_next( this_ ) )
            {
                result_err = DATA_ERROR_LEXICAL_STRUCTURE;
            }
        }
        if ( ! json_token_reader_private_is_value_end( this_ ) )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
    }
    else
    {
        result_err = DATA_ERROR_LEXICAL_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_expect_null_value ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    char current = json_token_reader_private_peek_next( this_ );
    if ( JSON_CONSTANTS_CHAR_BEGIN_NULL == current )
    {
        for ( int n_pos = 0; n_pos < sizeof(JSON_CONSTANTS_NULL); n_pos ++ )
        {
            if ( JSON_CONSTANTS_NULL[n_pos] != json_token_reader_private_read_next( this_ ) )
            {
                result_err = DATA_ERROR_LEXICAL_STRUCTURE;
            }
        }
        if ( ! json_token_reader_private_is_value_end( this_ ) )
        {
            result_err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
    }
    else
    {
        result_err = DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result_err );
    return result_err;
}

data_error_t json_token_reader_expect_eof ( json_token_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result_err = DATA_ERROR_NONE;

    /* skip whitespace */
    json_token_reader_private_skip_whitespace( this_ );

    char eof = json_token_reader_private_peek_next( this_ );
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
Copyright 2016-2021 Andreas Warnke

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
