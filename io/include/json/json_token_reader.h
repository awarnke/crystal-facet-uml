/* File: json_token_reader.h; Copyright and License: see below */

#ifndef JSON_TOKEN_READER_H
#define JSON_TOKEN_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief This class tokenizes one json input stream into a series of tokens,
 *
 *  This json_token_reader verifies the JSON file tokens (lexer) and allows the caller (parser) to validate the data structure.
 *
 *  In contrast to a DOM parser, this json_token_reader parses the input sequentially and provides parsed contents step by step.
 *  In contrast to a SAX parser, this json_token_reader does not require callbacks that are called whenever the next token is parsed.
 *  This json_token_reader allows the caller to specify the expectation what the next token is and fails if this expectation is wrong.
 */

#include "json/json_value_type.h"
#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include "stream/universal_input_stream.h"
#include "stream/universal_output_stream.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max buffer input size
 */
enum json_token_reader_max_enum {
    JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE = 256, /*!< maximum input buffer, at least 1 key string-value pair must fit in */
};

/*!
 *  \brief all data attributes needed for tokenizing data objects
 */
struct json_token_reader_struct {
    universal_input_stream_t *in_stream;  /*!< json input stream to be parsed */

    char ring_input_buffer[JSON_DESERIALIZER_MAX_RING_INPUT_BUFFER_SIZE];  /*!< look-ahead input buffer to check for next bytes */
    uint_fast32_t ring_start;
    uint_fast32_t ring_length;
};

typedef struct json_token_reader_struct json_token_reader_t;

/*!
 *  \brief initializes the json_token_reader_t struct
 *
 *  \param in_stream json stream to be parsed
 *  \param this_ pointer to own object attributes
 */
void json_token_reader_init ( json_token_reader_t *this_, universal_input_stream_t *in_stream );

/*!
 *  \brief re-initializes the json_token_reader_t struct
 *
 *  \param in_stream json stream to be parsed
 *  \param this_ pointer to own object attributes
 */
void json_token_reader_reinit ( json_token_reader_t *this_, universal_input_stream_t *in_stream );

/*!
 *  \brief destroys the json_token_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_token_reader_destroy ( json_token_reader_t *this_ );

/*!
 *  \brief checks that the next token is a "begin-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no begin-object token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_expect_begin_object ( json_token_reader_t *this_ );

/*!
 *  \brief returns the next "member-name" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param out_name return value: name of the "member-name"
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_name does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no member-name token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
static inline data_error_t json_token_reader_get_member_name ( json_token_reader_t *this_, utf8stringbuf_t out_name );

/*!
 *  \brief checks if the next token is an "end-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param end_object return value: true if the next token is an "end-object" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_is_end_object ( json_token_reader_t *this_, bool *end_object );

/*!
 *  \brief checks that the next token is a "name-separator" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a "name-separator".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no end-object token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_expect_name_separator ( json_token_reader_t *this_ );

/*!
 *  \brief checks that the next token is a "begin-array" json token
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no begin-array token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_expect_begin_array ( json_token_reader_t *this_ );

/*!
 *  \brief checks if the next token is an "end-array" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param end_array return value: true if the next token is an "end-array" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_is_end_array ( json_token_reader_t *this_, bool *end_array );

/*!
 *  \brief checks that the next token is a "value-separator" json token
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no value-separator token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_expect_value_separator ( json_token_reader_t *this_ );

/*!
 *  \brief determines the type of the next value without modifying the read-pointer
 *
 *  This function reads only as many charactes as necessary to determine the type.
 *  It does not verify if the type is valid.
 *
 *  \param this_ pointer to own object attributes
 *  \param value_type return value: type of the next value-token.
 *                    JSON_VALUE_TYPE_INTEGER will not be returned, JSON_VALUE_TYPE_NUMBER instead.
 *                    This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no value-token (e.g. we are at the end of an array),
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_get_value_type ( json_token_reader_t *this_, json_value_type_t *value_type );

/*!
 *  \brief determines the next value of type string
 *
 *  \param this_ pointer to own object attributes
 *  \param out_value return value: string-contents of the value-token
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_value does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no string-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_get_string_value ( json_token_reader_t *this_, utf8stringbuf_t out_value );

/*!
 *  \brief determines the next value of type integer (subtype of number)
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an integer.
 *  \param out_int return value: integer-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no integer-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_get_int_value ( json_token_reader_t *this_, int64_t *out_int );

/*!
 *  \brief determines the next value of type number
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_number return value: number-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no number-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_get_number_value ( json_token_reader_t *this_, double *out_number );

/*!
 *  \brief determines the next value of type boolean
 *
 *  \param this_ pointer to own object attributes
 *  \param out_bool return value: boolean-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no boolean-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_get_boolean_value ( json_token_reader_t *this_, bool *out_bool );

/*!
 *  \brief checks that the next token is a "null" json token
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no null token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_expect_null_value ( json_token_reader_t *this_ );

/*!
 *  \brief checks that the next token is EOF
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no EOF,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_token_reader_expect_eof ( json_token_reader_t *this_ );

/*!
 *  \brief skips whitespaces
 *
 *  Whitespaces are skipped as defined in http://rfc7159.net/rfc7159#rfc.section.2 .
 *  If there is no whitespace as next chatacter, the read pointer is not modified.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void json_token_reader_private_skip_whitespace ( json_token_reader_t *this_ );

/*!
 *  \brief checks if the current value-token is ended
 *
 *  If the next character after the current position
 *  is one of the following:
 *  {, }, [, ], :, comma, space, tab, newline, return,
 *  then the current value-token is ended.
 *
 *  This function is not suitable to check if a single-character token is ended,
 *  it is intended to check if numbers, strings, true, null, false are finished.
 *
 *  \param this_ pointer to own object attributes
 *  \return false if input stream read position points to a stand-alone token
 */
static inline bool json_token_reader_private_is_value_end ( json_token_reader_t *this_ );

/*!
 *  \brief reads a string literal to a utf8stringbuf
 *
 *  \param this_ pointer to own object attributes
 *  \param out_stream string-contents of the value-token, unmodified including escape sequences
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_stream does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
static inline data_error_t json_token_reader_private_read_string ( json_token_reader_t *this_, universal_output_stream_t *out_stream );

/*!
 *  \brief parses the integer token
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_int parsed integer or 0 in case of error
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
static inline data_error_t json_token_reader_private_parse_integer ( json_token_reader_t *this_, int64_t *out_int );

/*!
 *  \brief skips the number token, numbers are not supported by this parser
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
static inline data_error_t json_token_reader_private_skip_number ( json_token_reader_t *this_ );

/*!
 *  \brief reads more bytes from the input stream into the own ring input buffer
 *
 *  \param this_ pointer to own object attributes
 */
static inline void json_token_reader_private_fill_input_buffer ( json_token_reader_t *this_ );

/*!
 *  \brief fetches 1 byte from the ring input buffer without moving the read position,
 *         calls json_token_reader_private_fill_input_buffer if more bytes needed.
 *
 *  \param this_ pointer to own object attributes
 *  \return next byte or '\0' if end of stream
 */
static inline char json_token_reader_private_peek_next ( json_token_reader_t *this_ );

/*!
 *  \brief reads 1 byte from the ring input buffer,
 *         calls json_token_reader_private_fill_input_buffer if more bytes needed.
 *
 *  \param this_ pointer to own object attributes
 *  \return next byte or '\0' if end of stream
 */
static inline char json_token_reader_private_read_next ( json_token_reader_t *this_ );

#include "json/json_token_reader.inl"

#endif  /* JSON_TOKEN_READER_H */


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
