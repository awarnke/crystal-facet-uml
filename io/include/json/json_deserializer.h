/* File: json_deserializer.h; Copyright and License: see below */

#ifndef JSON_DESERIALIZER_H
#define JSON_DESERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Deserializes data objects from json format.
 *
 *  This object is a json parser, using the json_token_reader as lexer.
 */

#include "json/json_token_reader.h"
#include "data_error.h"
#include "data_table.h"
#include "data_classifier.h"
#include "data_diagram.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "stream/universal_input_stream.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for deserializing data objects
 *
 *  The attribute values keep track on the structure of the input tokens
 *  to check if these appear in the right order.
 */
struct json_deserializer_struct {
    json_token_reader_t tokenizer;  /*!< own token_reader instance to consecutively fetch tokens from the json input file */
    bool after_first_array_entry;  /*!< true if the first array entry has already been parsed */
    utf8stringbuf_t temp_string;  /*!< a local buffer to buffer streamed strings before assigning them to data objects */
    char temp_string_buffer[DATA_DIAGRAM_MAX_DESCRIPTION_SIZE];
};

typedef struct json_deserializer_struct json_deserializer_t;

/*!
 *  \brief initializes the json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data json text to be parsed
 */
void json_deserializer_init ( json_deserializer_t *this_, universal_input_stream_t *in_data );

/*!
 *  \brief re-initializes the json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data json text to be parsed
 */
void json_deserializer_reinit ( json_deserializer_t *this_, universal_input_stream_t *in_data );

/*!
 *  \brief destroys the json_deserializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_deserializer_destroy ( json_deserializer_t *this_ );

/*!
 *  \brief checks that the header of the json data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_expect_header ( json_deserializer_t *this_ );

/*!
 *  \brief checks that the footer/ending of the json data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_expect_footer ( json_deserializer_t *this_ );

/*!
 *  \brief checks that the beginning of the json data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_expect_begin_data ( json_deserializer_t *this_ );

/*!
 *  \brief checks if the data array is at end.
 *
 *  If yes, the end is read.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] true if the data array is at its end.
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_check_end_data ( json_deserializer_t *this_, bool* out_end );

/*!
 *  \brief determines the type of object which is contained in this object
 *
 *  \param this_ pointer to own object attributes
 *  \param out_type pointer to storage location for the result. Must not be NULL.
 *                  DATA_TABLE_CLASSIFIER if the next object is a classifier,
 *                  DATA_TABLE_DIAGRAM if the next object is a diagram,
 *                  DATA_TABLE_RELATIONSHIP if the next object is a relationship,
 *                  DATA_TABLE_VOID if there is no next object.
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_expect_begin_type_of_element ( json_deserializer_t *this_, data_table_t *out_type );

/*!
 *  \brief checks that the ending of the json type-object is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_expect_end_type_of_element ( json_deserializer_t *this_ );

/*!
 *  \brief parses the next object as classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param out_object pointer to storage location for the result. Must not be NULL.
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_feature array of features read from the database (in case of success), must not be NULL
 *  \param out_feature_count number of feature records stored in out_feature
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_get_next_classifier ( json_deserializer_t *this_,
                                                     data_classifier_t *out_object,
                                                     uint32_t max_out_array_size,
                                                     data_feature_t (*out_feature)[],
                                                     uint32_t *out_feature_count
                                                   );

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
data_error_t json_deserializer_get_next_diagram ( json_deserializer_t *this_, data_diagram_t *out_object );

/*!
 *  \brief parses the next object as relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param out_object pointer to storage location for the result. Must not be NULL.
 *  \param out_from_classifier_name stringbuffer to copy the from classifier name into;
 *                                  size should be DATA_CLASSIFIER_MAX_NAME_SIZE.
 *  \param out_from_feature_key stringbuffer to copy the from feature key into;
 *                              empty string is written if no feature available;
 *                              size should be DATA_FEATURE_MAX_KEY_SIZE.
 *  \param out_to_classifier_name stringbuffer to copy the to classifier name into;
 *                                size should be DATA_CLASSIFIER_MAX_NAME_SIZE.
 *  \param out_to_feature_key stringbuffer to copy the to feature key into;
 *                            empty string is written if no feature available;
 *                            size should be DATA_FEATURE_MAX_KEY_SIZE.
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_get_next_relationship ( json_deserializer_t *this_,
                                                       data_relationship_t *out_object,
                                                       utf8stringbuf_t out_from_classifier_name,
                                                       utf8stringbuf_t out_from_feature_key,
                                                       utf8stringbuf_t out_to_classifier_name,
                                                       utf8stringbuf_t out_to_feature_key
                                                     );

/*!
 *  \brief skips the next object, e.g. a relationship if this is of no interest
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected (e.g. array member),
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_skip_next_object ( json_deserializer_t *this_ );

/*!
 *  \brief skips the next string, e.g. a key or string-value of no interest
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected (e.g. array member),
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_skip_next_string ( json_deserializer_t *this_ );

/*!
 *  \brief gets the line number at the current read position
 *
 *  May be used to determine the position where a parse error occurred.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_read_pos pointer to storage location for the result. Must not be NULL.
 */
void json_deserializer_get_read_line ( json_deserializer_t *this_, uint32_t *out_read_pos );

/*!
 *  \brief parses the next feature array
 *
 *  \param this_ pointer to own object attributes
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_feature array of features read from the database (in case of success), must not be NULL
 *  \param out_feature_count number of feature records stored in out_feature
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_private_get_next_feature_array ( json_deserializer_t *this_,
                                                                uint32_t max_out_array_size,
                                                                data_feature_t (*out_feature)[],
                                                                uint32_t *out_feature_count
                                                              );

/*!
 *  \brief parses the next object as feature
 *
 *  This function may only be called from within parsing a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_object pointer to storage location for the result. Must not be NULL.
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_private_get_next_feature ( json_deserializer_t *this_, data_feature_t *out_object );

/*!
 *  \brief parses the next diagramelement array
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          DATA_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          DATA_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          DATA_ERROR_NONE if structure of the input is valid.
 */
data_error_t json_deserializer_private_skip_next_diagramelement_array ( json_deserializer_t *this_ );

#endif  /* JSON_DESERIALIZER_H */


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
