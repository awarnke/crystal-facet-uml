/* File: json_tokenizer.h; Copyright and License: see below */

#ifndef JSON_TOKENIZER_H
#define JSON_TOKENIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Tokenizes a json input string.
 *
 *  This json_tokenizer verifies the JSON file tokens (lexer) and allows the caller (parser) to validate the data structure.
 *
 *  In contrast to a DOM parser, this json_tokenizer parses the input sequentially and provides parsed contents step by step.
 *  In contrast to a SAX parser, this json_tokenizer does not require callbacks that are called whenever the next token is parsed.
 *  This json_tokenizer allows the caller to specify the expectation what the next token is and fails if this expectation is wrong.
 */

#include "json/json_value_type.h"
#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max input size
 */
enum json_tokenizer_max_enum {
    JSON_TOKENIZER_MAX_INPUT_SIZE = 16*1024*1024,  /*!< assuming a maxmum input string size allows to prevent endless-loops. 16MB should be big enough. */
};

/*!
 *  \brief all data attributes needed for tokenizing data objects
 */
struct json_tokenizer_struct {
    int dummy;
};

typedef struct json_tokenizer_struct json_tokenizer_t;

/*!
 *  \brief initializes the json_tokenizer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_tokenizer_init ( json_tokenizer_t *this_ );

/*!
 *  \brief re-initializes the json_tokenizer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_tokenizer_reinit ( json_tokenizer_t *this_ );

/*!
 *  \brief destroys the json_tokenizer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_tokenizer_destroy ( json_tokenizer_t *this_ );

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
data_error_t json_tokenizer_expect_begin_object ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

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
data_error_t json_tokenizer_get_member_name ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_name );

/*!
 *  \brief checks if the next token is an "end-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "end-object".
 *  \param end_object return value: true if the next token is an "end-object" token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_is_end_object ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_object );

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
data_error_t json_tokenizer_expect_name_separator ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

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
data_error_t json_tokenizer_expect_begin_array ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

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
data_error_t json_tokenizer_is_end_array ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *end_array );

/*!
 *  \brief checks that the next token is a "value-separator" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an "value-separator".
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no value-separator token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_expect_value_separator ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief determines the type of the next value without modifying the read-pointer
 *
 *  This function reads only as many charactes as necessary to determine the type.
 *  It does not verify if the type is valid.
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) only by whitespaces, not over the next value.
 *  \param value_type return value: type of the next value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no value-token (e.g. we are at the end of an array),
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_get_value_type ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, json_value_type_t *value_type );

/*!
 *  \brief determines the next value of type string
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a string.
 *  \param out_value return value: string-contents of the value-token
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if out_value does not provide enough space,
 *          DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no string-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_get_string_value ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, utf8stringbuf_t out_value );

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
data_error_t json_tokenizer_get_int_value ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, int64_t *out_int );

/*!
 *  \brief determines the next value of type number
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a number.
 *  \param out_number return value: number-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no number-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_get_number_value ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, double *out_number );

/*!
 *  \brief determines the next value of type boolean
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is boolean.
 *  \param out_bool return value: boolean-contents of the value-token. This parameter must not be NULL.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no boolean-value-token,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_get_boolean_value ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos, bool *out_bool );

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
data_error_t json_tokenizer_expect_null_value ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief checks that the next token is EOF
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is EOF, but not after the terminating zero.
 *  \return DATA_ERROR_NONE if the lexical+parser structure of the input is valid,
 *          DATA_ERROR_PARSER_STRUCTURE if there is no EOF,
 *          DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 */
data_error_t json_tokenizer_expect_eof ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief skips whitespaces
 *
 *  Whitespaces are skipped as defined in http://rfc7159.net/rfc7159#rfc.section.2 .
 *  If there is no whitespace as next chatacter, the read pointer is not modified.
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position.
 *                     The read position will be moved(changed) if there are whitespaces after the current read position.
 */
static inline void json_tokenizer_private_skip_whitespace ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief checks if the current token is ended
 *
 *  If the previous character before the current position
 *  or the next character after the current position
 *  is one of the following:
 *  {, }, [, ], :, comma, space, tab, newline, return,
 *  then the current token is ended.
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param in_read_pos pointer to current read position.
 *  \return false if in_data[*in_read_pos] and in_data[*in_read_pos+1] belong to the same token
 */
static inline bool json_tokenizer_private_is_token_end ( json_tokenizer_t *this_, const char *in_data, const uint32_t *in_read_pos );

/*!
 *  \brief deterines the string end of a string literal
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to (before:) the position where the string literal starts; (after:) the position of the terminating quote character
 */
static inline void json_tokenizer_private_find_string_end ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief parses the integer token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is an integer.
 *  \result parsed integer or 0 in case of error
 */
static inline int64_t json_tokenizer_private_parse_integer ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

/*!
 *  \brief skips the number token, numbers are not supported by this parser
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a number.
 */
static inline void json_tokenizer_private_skip_number ( json_tokenizer_t *this_, const char *in_data, uint32_t *io_read_pos );

#include "json/json_tokenizer.inl"

#endif  /* JSON_TOKENIZER_H */


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
