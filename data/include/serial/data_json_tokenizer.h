/* File: data_json_tokenizer.h; Copyright and License: see below */

#ifndef DATA_JSON_TOKENIZER_H
#define DATA_JSON_TOKENIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Tokenizes a json input string.
 *
 *  This data_json_tokenizer verifies the JSON file structure (lexer) and allows the caller (parser) to validate the data structure.
 *
 *  In contrast to a DOM parser, this data_json_tokenizer parses the input sequentially and provides parsed contents step by step.
 *  In contrast to a SAX parser, this data_json_tokenizer does not require callbacks that are called whenever the next token is parsed.
 *  This data_json_tokenizer allows the caller to specify the expectation what the next token is and fails if this expectation is wrong.
 */

#include "serial/data_json_value_type.h"
#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for tokenizing data objects
 */
struct data_json_tokenizer_struct {
    int dummy;
};

typedef struct data_json_tokenizer_struct data_json_tokenizer_t;

/*!
 *  \brief initializes the data_json_tokenizer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_tokenizer_init ( data_json_tokenizer_t *this_ );

/*!
 *  \brief destroys the data_json_tokenizer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_tokenizer_destroy ( data_json_tokenizer_t *this_ );

/*!
 *  \brief checks if the next token is a "begin-object" json token
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data utf8 encoded string where to read from
 *  \param io_read_pos pointer to current read position. The read position will be moved(changed) if the next token is a "begin-object".
 *  \param begin_object pointer to own object attributes
 *  \return DATA_ERROR_NONE if the lexical structure of the input is valid, DATA_ERROR_LEXICAL_STRUCTURE otherwise.
 *
 */
data_error_t data_json_tokenizer_is_begin_object ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *begin_object );

data_error_t data_json_tokenizer_expect_begin_object ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_tokenizer_expect_member_name ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, const char *name );

data_error_t data_json_tokenizer_get_member_name ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, utf8stringbuf_t out_name );

data_error_t data_json_tokenizer_is_end_object ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *end_object );

data_error_t data_json_tokenizer_expect_end_object ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_tokenizer_is_begin_array ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *begin_array );

data_error_t data_json_tokenizer_expect_begin_array ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_tokenizer_is_end_array ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *end_array );

data_error_t data_json_tokenizer_expect_end_array ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_tokenizer_get_value_type ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, data_json_value_type_t *value_type );

data_error_t data_json_tokenizer_get_string_value ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, utf8stringbuf_t out_value );

data_error_t data_json_tokenizer_get_int_value ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, int64_t *out_int );

data_error_t data_json_tokenizer_get_number_value ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, double *out_number );

data_error_t data_json_tokenizer_get_boolean_value ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *out_bool );

data_error_t data_json_tokenizer_expect_null_value ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_tokenizer_is_eof ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *eof );

data_error_t data_json_tokenizer_expect_eof ( data_json_tokenizer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

#endif  /* DATA_JSON_TOKENIZER_H */


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
