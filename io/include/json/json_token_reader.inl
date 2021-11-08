/* File: json_token_reader.inl; Copyright and License: see below */

#include "json/json_constants.h"
#include "trace.h"
#include <assert.h>

static inline data_error_t json_token_reader_get_member_name ( json_token_reader_t *this_, utf8stringbuf_t out_name )
{
    return json_token_reader_get_string_value( this_, out_name );
}

static inline void json_token_reader_private_skip_whitespace ( json_token_reader_t *this_ )
{
    bool ws_end_reached = false;
    char current = ' ';
    while ( ( ! ws_end_reached ) && ( current != '\0' ) )
    {
        current = json_token_reader_private_peek_next( this_ );
        if ( ( JSON_CONSTANTS_CHAR_NL != current )
            && ( JSON_CONSTANTS_CHAR_CR != current )
            && ( JSON_CONSTANTS_CHAR_TAB != current )
            && ( JSON_CONSTANTS_CHAR_SPACE != current ))
        {
            ws_end_reached = true;
        }
        else
        {
            json_token_reader_private_read_next( this_ );
        }
    }
}

static inline bool json_token_reader_private_is_value_end ( json_token_reader_t *this_ )
{
    char next = json_token_reader_private_peek_next( this_ );

    const bool result = (( next == '\0' )
        || ( next == JSON_CONSTANTS_CHAR_NL )
        || ( next == JSON_CONSTANTS_CHAR_CR )
        || ( next == JSON_CONSTANTS_CHAR_TAB )
        || ( next == JSON_CONSTANTS_CHAR_SPACE )
        || ( next == JSON_CONSTANTS_CHAR_BEGIN_OBJECT )
        || ( next == JSON_CONSTANTS_CHAR_END_OBJECT )
        || ( next == JSON_CONSTANTS_CHAR_BEGIN_ARRAY )
        || ( next == JSON_CONSTANTS_CHAR_END_ARRAY )
        || ( next == JSON_CONSTANTS_CHAR_NAME_SEPARATOR )
        || ( next == JSON_CONSTANTS_CHAR_VALUE_SEPARATOR ));

    return result;
}

static inline data_error_t json_token_reader_private_read_string ( json_token_reader_t *this_, universal_output_stream_t *out_stream )
{
    assert( out_stream != NULL );
    data_error_t result = DATA_ERROR_NONE;
    bool str_end_reached = false;
    char esc_incomplete = false;
    char out_buffer[64];
    size_t ouf_buf_len = 0;
    while ( ! str_end_reached )
    {
        char current = json_token_reader_private_peek_next( this_ );
        if ( '\0' == current )
        {
            str_end_reached = true;
            result = DATA_ERROR_LEXICAL_STRUCTURE;
        }
        else if (( JSON_CONSTANTS_CHAR_END_STRING == current )&&( ! esc_incomplete ))
        {
            str_end_reached = true;
        }
        else
        {
            out_buffer[ouf_buf_len] = json_token_reader_private_read_next( this_ );
            ouf_buf_len ++;
        }
        if (( JSON_CONSTANTS_CHAR_ESC == current ) && ( ! esc_incomplete ))
        {
            esc_incomplete = true;
        }
        else
        {
            esc_incomplete = false;
            if ( ouf_buf_len >= (sizeof(out_buffer)-sizeof(char)) )
            {
                const int err
                    = universal_output_stream_write( out_stream, &out_buffer, ouf_buf_len);
                ouf_buf_len = 0;
                if ( err != 0 )
                {
                    TRACE_INFO( "could not write all data to output stream in json_token_reader_private_read_string." );
                    result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
                }
            }
        }
    }
    {
        const int err2
            = universal_output_stream_write( out_stream, &out_buffer, ouf_buf_len);
        ouf_buf_len = 0;
        if ( err2 != 0 )
        {
            TRACE_INFO( "could not write all data to output stream in json_token_reader_private_read_string" );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
    }

    return result;
}

static inline data_error_t json_token_reader_private_parse_integer ( json_token_reader_t *this_, int64_t *out_int )
{
    assert( out_int != NULL );
    int64_t result;
    data_error_t err = DATA_ERROR_NONE;

    char first = json_token_reader_private_peek_next( this_ );
    if ( '0' == first )
    {
        /* only zero may begin with digit zero */
        result = 0;
        json_token_reader_private_read_next( this_ );
    }
    else
    {
        bool minus = false;
        bool has_digits = false;
        bool minus_zero_error = false;
        result = 0;

        /* check for minus */
        if ( '-' == first )
        {
            minus = true;
            json_token_reader_private_read_next( this_ );
            if ( '0' == json_token_reader_private_peek_next( this_ ) )
            {
                /* number starts with -0 which is invalid */
                minus_zero_error = true;
            }
        }

        bool int_end_reached = false;
        while ( ! int_end_reached )
        {
            char current = json_token_reader_private_peek_next( this_ );
            if (( '0' <= current )&&( current <= '9'))
            {
                has_digits = true;
                json_token_reader_private_read_next( this_ );
                result = (result*10)+((int) (current-'0'));
            }
            else
            {
                int_end_reached = true;
            }
        }

        if ( minus )
        {
            result = -result;
        }

        if ( ( ! has_digits ) || minus_zero_error )
        {
            result = 0;
            err = DATA_ERROR_LEXICAL_STRUCTURE;
        }
    }

    *out_int = result;
    return err;
}

static inline data_error_t json_token_reader_private_skip_number ( json_token_reader_t *this_ )
{
    data_error_t result = DATA_ERROR_LEXICAL_STRUCTURE;

    bool num_end_reached = false;
    while ( ! num_end_reached )
    {
        char current = json_token_reader_private_peek_next( this_ );
        if ( (( '0' <= current )&&( current <= '9'))
            || ( 'e' == current )
            || ( 'E' == current )
            || ( '.' == current )
            || ( '+' == current )
            || ( '-' == current ) )
        {
            /* could be part of a valid number */
            json_token_reader_private_read_next( this_ );
            result = DATA_ERROR_NONE;
        }
        else
        {
            num_end_reached = true;
        }
    }

    return result;
}

static inline void json_token_reader_private_fill_input_buffer( json_token_reader_t *this_ )
{
    /* fill back of input buffer */
    if ( (*this_).ring_length < JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE )
    {
        const uint_fast32_t first_free
            = ((*this_).ring_start + (*this_).ring_length) % JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE;
        if ( first_free >= (*this_).ring_start )
        {
            const uint_fast32_t free_at_end = JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE - first_free;
            void *const start = &(((*this_).ring_input_buffer)[first_free]);
            size_t read;
            const int err
                = universal_input_stream_read( (*this_).in_stream, start, free_at_end, &read );
            if ( err != 0 )
            {
                TRACE_INFO( "no more input bytes at json_token_reader_private_fill_input_buffer" );
            }
            (*this_).ring_length += read;
        }
    }

    /* fill front of input buffer */
    if ( (*this_).ring_length < JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE )
    {
        const uint_fast32_t first_free
            = ((*this_).ring_start + (*this_).ring_length) % JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE;
        if ( first_free < (*this_).ring_start )
        {
            const uint_fast32_t free_at_end = (*this_).ring_start - first_free;
            void *const start = &(((*this_).ring_input_buffer)[first_free]);
            size_t read;
            const int err
                = universal_input_stream_read( (*this_).in_stream, start, free_at_end, &read );
            if ( err != 0 )
            {
                TRACE_INFO( "no more input bytes at json_token_reader_private_fill_input_buffer." );
            }
            (*this_).ring_length += read;
        }
    }
}

static inline char json_token_reader_private_peek_next( json_token_reader_t *this_ )
{
    if ( (*this_).ring_length == 0 )
    {
        json_token_reader_private_fill_input_buffer( this_ );
    }

    char result;
    if ( (*this_).ring_length == 0 )
    {
        result = '\0';
    }
    else
    {
        result = ((*this_).ring_input_buffer)[(*this_).ring_start];
    }

    return result;
}

static inline char json_token_reader_private_read_next( json_token_reader_t *this_ )
{
    if ( (*this_).ring_length == 0 )
    {
        json_token_reader_private_fill_input_buffer( this_ );
    }

    char result;
    if ( (*this_).ring_length == 0 )
    {
        result = '\0';
    }
    else
    {
        result = ((*this_).ring_input_buffer)[(*this_).ring_start];
        (*this_).ring_start = ((*this_).ring_start + 1) % JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE;
        (*this_).ring_length --;
    }

    return result;
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
