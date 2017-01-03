/* File: data_json_deserializer.h; Copyright and License: see below */

#ifndef DATA_JSON_DESERIALIZER_H
#define DATA_JSON_DESERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Deserializes data objects from json format.
 *
 *  This object is a json parser, using the data_json_tokenizer as lexer.
 */

#include "serial/data_json_tokenizer.h"
#include "data_error.h"
#include "data_table.h"
#include "data_classifier.h"
#include "data_diagram.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for deserializing data objects
 *
 *  The attribute values keep track on the structure of the input tokens
 *  to check if these appear in the right order.
 */
struct data_json_deserializer_struct {
    data_json_tokenizer_t tokenizer;  /*!< own tokenizer instance to consecutively fetch tokens from the json input file */
    const char *in_data;  /*!< string to be parsed */
    uint32_t read_pos;  /*!< current read position in string to be parsed */
    bool after_first_array_entry;  /*!< true if the first array entry has already been parsed */
};

typedef struct data_json_deserializer_struct data_json_deserializer_t;

/*!
 *  \brief initializes the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data json text to be parsed
 */
void data_json_deserializer_init ( data_json_deserializer_t *this_, const char *in_data );

/*!
 *  \brief re-initializes the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data json text to be parsed
 */
void data_json_deserializer_reinit ( data_json_deserializer_t *this_, const char *in_data );

/*!
 *  \brief destroys the data_json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_deserializer_destroy ( data_json_deserializer_t *this_ );

/*!
 *  \brief checks that the header of the json set is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t data_json_deserializer_expect_begin_set ( data_json_deserializer_t *this_ );

/*!
 *  \brief checks that the ending of the json set is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t data_json_deserializer_expect_end_set ( data_json_deserializer_t *this_ );

/*!
 *  \brief determines the type of object which will be parsed next
 *
 *  \param this_ pointer to own object attributes
 *  \param out_type pointer to storage location for the result. Must not be NULL.
 *                  DATA_TABLE_CLASSIFIER if the next object is a classifier,
 *                  DATA_TABLE_DIAGRAM if the next object is a diagram,
 *                  DATA_TABLE_VOID if there is no next object.
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t data_json_deserializer_get_type_of_next_element ( data_json_deserializer_t *this_, data_table_t *out_type );

/*!
 *  \brief parses the next object as classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param out_object pointer to storage location for the result. Must not be NULL.
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t data_json_deserializer_get_next_classifier ( data_json_deserializer_t *this_, data_classifier_t *out_object );

/*!
 *  \brief parses the next object as diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param out_object pointer to storage location for the result. Must not be NULL.
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t data_json_deserializer_get_next_diagram ( data_json_deserializer_t *this_, data_diagram_t *out_object );

/*!
 *  \brief gets the current read position
 *
 *  May be used to determine the position where a parse error occurred.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_read_pos pointer to storage location for the result. Must not be NULL.
 */
void data_json_deserializer_get_read_pos ( data_json_deserializer_t *this_, uint32_t *out_read_pos );

#endif  /* DATA_JSON_DESERIALIZER_H */


/*
Copyright 2016-2017 Andreas Warnke

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
