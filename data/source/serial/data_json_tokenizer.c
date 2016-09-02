/* File: data_json_tokenizer.c; Copyright and License: see below */

#include "serial/data_json_tokenizer.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

void data_json_tokenizer_init ( data_json_tokenizer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).container_stack_size = 0;
    (*this_).root_object_count = 0;

    TRACE_END();
}

void data_json_tokenizer_reinit ( data_json_tokenizer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).container_stack_size = 0;
    (*this_).root_object_count = 0;

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
    data_json_value_type_t value_type = DATA_JSON_VALUE_TYPE_UNDEF;

    //result_err = data_json_tokenizer_get_value_type ( this_, in_data, io_read_pos, &value_type );
    if ( DATA_ERROR_PARSER_STRUCTURE == result_err )
    {
        /* there is no value-start as next token */
        value_type = DATA_JSON_VALUE_TYPE_UNDEF;
        result_err = DATA_ERROR_NONE;
    }
    else if ( DATA_ERROR_NONE == result_err )
    {
        /* move the read pointer forward: */
        //result_err = data_json_tokenizer_expect_begin_object ( this_, in_data, io_read_pos );
    }
    *begin_object = ( DATA_JSON_VALUE_TYPE_OBJECT == value_type );

    TRACE_END_ERR( result_err );
}

/*!
 *  \brief checks that the next token is a "begin-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a "begin-object".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no begin-object token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_begin_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief checks that the next token is a "member-name" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is the expected member name..
 *  \param name name of the expected "member-name". This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no or the wrong member-name token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_member_name ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, const char *name );

/*!
 *  \brief returns the next "member-name" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a "member-name".
 *  \param out_name return value: name of the "member-name"
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_name does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no member-name token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_member_name ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_name );

/*!
 *  \brief checks if the next token is an "end-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "end-object".
 *  \param out_name return value: true if the next token is an "end-object" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_end_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_object );

/*!
 *  \brief checks that the next token is an "end-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "end-object".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no end-object token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_end_object ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief checks if the next token is a "begin-array" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a "begin-array".
 *  \param begin_array return value: true if the next token is a "begin-array" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_begin_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *begin_array );

/*!
 *  \brief checks that the next token is a "begin-array" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a "begin-array".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no begin-array token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_begin_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief checks if the next token is an "end-array" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "end-array".
 *  \param end_array return value: true if the next token is an "end-array" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_end_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_array );

/*!
 *  \brief checks that the next token is an "end-array" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "end-array".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no end-array token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_end_array ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief determines the type of the next value without modifying the read-pointer
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param in_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "end-array".
 *  \param value_type return value: type of the next value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no value-token (e.g. we are at the end of an array),
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_value_type ( data_json_tokenizer_t *this_, const char *in_data, const uint32_t *in_read_pos, data_json_value_type_t *value_type );

/*!
 *  \brief determines the next value of type string
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param in_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a string.
 *  \param out_value return value: string-contents of the value-token
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_value does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no string-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_string_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_value );

/*!
 *  \brief determines the next value of type integer (subtype of number)
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param in_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an integer.
 *  \param out_int return value: integer-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no integer-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_int_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, int64_t *out_int );

/*!
 *  \brief determines the next value of type number
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param in_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a number.
 *  \param out_number return value: number-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no number-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_number_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, double *out_number );

/*!
 *  \brief determines the next value of type boolean
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param in_read_pos pointer to current read position. The read position will be moved(changed) if the next token is boolean.
 *  \param out_bool return value: boolean-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no boolean-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_get_boolean_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *out_bool );

/*!
 *  \brief checks that the next token is a "null" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is "null".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no null token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_null_value ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief checks if the next token is EOF
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is EOF.
 *  \param end_array return value: true if the next token is EOF. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_is_eof ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *eof );

/*!
 *  \brief checks that the next token is EOF
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is EOF.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no EOF,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t data_json_tokenizer_expect_eof ( data_json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

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
