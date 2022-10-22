/* File: json_element_reader.h; Copyright and License: see below */

#ifndef JSON_ELEMENT_READER_H
#define JSON_ELEMENT_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Deserializes data objects from json format.
 *
 *  This object is a json parser, using the json_token_reader as lexer.
 */

#include "json/json_token_reader.h"
#include "u8/u8_error.h"
#include "data_table.h"
#include "data_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_diagram.h"
#include "data_diagramelement.h"
#include "u8stream/universal_input_stream.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for deserializing data objects
 *
 *  The attribute values keep track on the structure of the input tokens
 *  to check if these appear in the right order.
 *
 *  Lifecycle: A json deserializer shall perform a single import operation only.
 *  It may be initialized before one import operation and be destroyed afterwards.
 */
struct json_element_reader_struct {
    json_token_reader_t tokenizer;  /*!< own token_reader instance to consecutively fetch tokens from the json input file */
    bool top_array_after_first_entry;  /*!< true if the first entry of the top-level array has already been parsed */
    bool sub_array_after_first_entry;  /*!< true if the first entry of the 2nd-level array has already been parsed */
    utf8stringbuf_t temp_string;  /*!< a local buffer to buffer streamed strings before assigning them to data objects */
    char temp_string_buffer[DATA_DIAGRAM_MAX_DESCRIPTION_SIZE];
};

typedef struct json_element_reader_struct json_element_reader_t;

/*!
 *  \brief initializes the json_element_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data json text to be parsed
 */
void json_element_reader_init ( json_element_reader_t *this_,
                                universal_input_stream_t *in_data
                              );

/*!
 *  \brief re-initializes the json_element_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param in_data json text to be parsed
 */
void json_element_reader_reinit ( json_element_reader_t *this_,
                                  universal_input_stream_t *in_data
                                );

/*!
 *  \brief destroys the json_element_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void json_element_reader_destroy ( json_element_reader_t *this_ );

/*!
 *  \brief checks that the header of the json data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_expect_header ( json_element_reader_t *this_ );

/*!
 *  \brief checks that the footer/ending of the json data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_expect_footer ( json_element_reader_t *this_ );

/*!
 *  \brief checks that the beginning of the json top-level array is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_expect_begin_top_array ( json_element_reader_t *this_ );

/*!
 *  \brief checks if the top-level array is at its end.
 *
 *  If yes, the end is read and consumed.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_end true if the data array is at its end.
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_check_end_top_array ( json_element_reader_t *this_, bool* out_end );

/*!
 *  \brief checks that the beginning of the json 2nd-level array is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_expect_begin_sub_array ( json_element_reader_t *this_ );

/*!
 *  \brief checks if the 2nd-level array is at its end.
 *
 *  If yes, the end is read and consumed.
 *  If no, a possible object separater is read and consumed.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_end true if the data array is at its end.
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_check_end_sub_array ( json_element_reader_t *this_, bool* out_end );

/*!
 *  \brief determines the type of object which is contained in this object
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_type pointer to storage location for the result. Must not be NULL.
 *                       DATA_TABLE_CLASSIFIER if the next object is a classifier,
 *                       DATA_TABLE_DIAGRAM if the next object is a diagram,
 *                       DATA_TABLE_RELATIONSHIP if the next object is a relationship,
 *                       DATA_TABLE_VOID if there is no next object.
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_expect_begin_type_of_element ( json_element_reader_t *this_, data_table_t *out_type );

/*!
 *  \brief checks that the ending of the json type-object is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_expect_end_type_of_element ( json_element_reader_t *this_ );

/*!
 *  \brief parses the next object as classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_object pointer to storage location for the result. Must not be NULL.
 *  \param[out] out_has_features_array pointer to a flag that says if subelements array needs to be parsed.
 *                                     Must not be NULL.
 *                                     If true, json_element_reader_end_unfinished_object shall be called afterwards.
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_get_next_classifier ( json_element_reader_t *this_,
                                                     data_classifier_t *out_object,
                                                     bool* out_has_features_array
                                                   );

/*!
 *  \brief parses the end token of the current object
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_end_unfinished_object ( json_element_reader_t *this_ );

/*!
 *  \brief parses the next object as diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_object pointer to storage location for the result. Must not be NULL.
 *  \param[out] out_parent_uuid stringbuffer to copy the parent into;
 *                         If there is no parent, the stringbuffer shall contain the empty string;
 *                         size should be DATA_UUID_STRING_SIZE.
 *  \param[out] out_has_diagramelements_array pointer to a flag that says if subelements array needs to be parsed.
 *                                            Must not be NULL.
 *                                            If true, json_element_reader_end_unfinished_object shall be called afterwards.
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_get_next_diagram ( json_element_reader_t *this_,
                                                  data_diagram_t *out_object,
                                                  utf8stringbuf_t out_parent_uuid,
                                                  bool* out_has_diagramelements_array
                                                );

/*!
 *  \brief parses the next object as relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_object pointer to storage location for the result. Must not be NULL.
 *  \param[out] out_from_node_uuid stringbuffer to copy the from node into;
 *                            This node may refer to a lifeline, another feature or a classifier;
 *                            size should be DATA_UUID_STRING_SIZE.
 *  \param[out] out_to_node_uuid stringbuffer to copy the to node into;
 *                          This node may refer to a lifeline, another feature or a classifier;
 *                          size should be DATA_UUID_STRING_SIZE.
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_get_next_relationship ( json_element_reader_t *this_,
                                                       data_relationship_t *out_object,
                                                       utf8stringbuf_t out_from_node_uuid,
                                                       utf8stringbuf_t out_to_node_uuid
                                                     );

/*!
 *  \brief skips the next object, e.g. a relationship if this is of no interest
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected (e.g. array member),
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_skip_next_object ( json_element_reader_t *this_ );

/*!
 *  \brief skips the next string, e.g. a key or string-value of no interest
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected (e.g. array member),
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_skip_next_string ( json_element_reader_t *this_ );

/*!
 *  \brief gets the line number at the current read position
 *
 *  May be used to determine the position where a parse error occurred.
 *
 *  \param this_ pointer to own object attributes
 *  \return the line number of the current reading position
 */
uint32_t json_element_reader_get_read_line ( json_element_reader_t *this_ );

/*!
 *  \brief parses the next object as feature
 *
 *  This function may only be called from within parsing a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_object pointer to storage location for the result. Must not be NULL.
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_get_next_feature ( json_element_reader_t *this_, data_feature_t *out_object );

/*!
 *  \brief parses the next object as diagramelement
 *
 *  This function may only be called from within parsing a diagram.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_object pointer to storage location for the result. Must not be NULL.
 *  \param[out] out_node_uuid stringbuffer to copy the node uuid into;
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_object,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_get_next_diagramelement ( json_element_reader_t *this_,
                                                         data_diagramelement_t *out_object,
                                                         utf8stringbuf_t out_node_uuid
                                                       );

/*!
 *  \brief parses an array of strings and merges the entries
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_joined_string stringbuffer to copy joined string elements to;
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if strings do not fit into the out_joined_string,
 *          U8_ERROR_PARSER_STRUCTURE if JSON format is valid but JSON content is unexpected,
 *          U8_ERROR_LEXICAL_STRUCTURE if JSON format is invalid,
 *          U8_ERROR_NONE if structure of the input is valid.
 */
u8_error_t json_element_reader_private_read_string_array ( json_element_reader_t *this_,
                                                           utf8stringbuf_t out_joined_string
                                                         );

#endif  /* JSON_ELEMENT_READER_H */


/*
Copyright 2016-2022 Andreas Warnke

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
