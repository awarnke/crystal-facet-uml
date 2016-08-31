/* File: data_json_deserializer.h; Copyright and License: see below */

#ifndef DATA_JSON_DESERIALIZER_H
#define DATA_JSON_DESERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Deserializes data objects from json format.
 *
 *  This data_json_deserializer verifies the JSON file structure and allows the caller to validate the data structure.
 *
 *  In contrast to a DOM parser, this data_json_deserializer parses the input sequentially and provides parsed contents step by step.
 *  In contrast to a SAX parser, this data_json_deserializer does not require callbacks that are called whenever the next token is parsed.
 *  This data_json_deserializer allows the caller to specify the expectation what the next token is and fails if this expectation is wrong.
 */

#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for deserializing data objects
 */
struct data_json_deserializer_struct {
    int dummy;
};

typedef struct data_json_deserializer_struct data_json_deserializer_t;

/*!
 *  \brief initializes the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_deserializer_init ( data_json_deserializer_t *this_ );

/*!
 *  \brief destroys the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_deserializer_destroy ( data_json_deserializer_t *this_ );

data_error_t data_json_deserializer_is_object_begin ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *object_begin );

data_error_t data_json_deserializer_expect_object_begin ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_deserializer_expect_attribute_key ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, const char *key );

data_error_t data_json_deserializer_get_attribute_key ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, utf8stringbuf_t out );

data_error_t data_json_deserializer_is_object_end ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *object_end );

data_error_t data_json_deserializer_expect_object_end ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_deserializer_is_array_begin ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *array_begin );

data_error_t data_json_deserializer_expect_array_begin ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_deserializer_is_array_end ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *array_end );

data_error_t data_json_deserializer_expect_array_end ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

data_error_t data_json_deserializer_get_value_type ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, int *value_type );

data_error_t data_json_deserializer_get_string_value ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, utf8stringbuf_t out );

data_error_t data_json_deserializer_get_int_value ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, int64_t *out );

data_error_t data_json_deserializer_is_eof ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos, bool *eof );

data_error_t data_json_deserializer_expect_eof ( data_json_deserializer_t *this_, utf8stringbuf_t in_data, uint32_t *io_read_pos );

#endif  /* DATA_JSON_DESERIALIZER_H */


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
