/* File: json_writer.h; Copyright and License: see below */

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief writes strings to an json utf8 stream
 */

#include "json/json_constants.h"
#include "entity/data_diagram.h"
#include "entity/data_classifier.h"
#include "entity/data_table.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include "u8stream/universal_output_stream.h"
#include "u8stream/universal_escaping_output_stream.h"
#include "u8/u8_error.h"

/*!
 *  \brief constants for max string sizes
 */
enum json_writer_max_enum {
    JSON_WRITER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
    JSON_WRITER_MAX_INDENT = 7,
};

/*!
 *  \brief indentation
 */
extern const char JSON_CONSTANTS_INDENT[(2*JSON_WRITER_MAX_INDENT)+sizeof("")];

/*!
 *  \brief indentation, followed by quote
 */
extern const char JSON_CONSTANTS_INDENT_QUOTE[(2*JSON_WRITER_MAX_INDENT)+sizeof(JSON_CONSTANTS_QUOTE)];

/*!
 *  \brief attributes of the json writer
 *
 *  Lifecycle: A json writer shall perform a single export operation only.
 *  It may be initialized before one export operation and be destroyed afterwards.
 */
struct json_writer_struct {
    universal_output_stream_t *output;  /*!< output stream where to write the generated document to */
    universal_escaping_output_stream_t esc_output;  /*!< escaping output stream filter that does the json escaping */

    const char *const ((*json_string_encode_table)[][2]);  /*!< table for json encode string replacements */
    const char *const ((*json_stringlist_encode_table)[][2]);  /*!< table for json encode stringlist replacements */
};

typedef struct json_writer_struct json_writer_t;

/*!
 *  \brief initializes the json writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated document to
 */
void json_writer_init( json_writer_t *this_,
                       universal_output_stream_t *output
                     );

/*!
 *  \brief destroys the json writer
 *
 *  \param this_ pointer to own object attributes
 */
void json_writer_destroy( json_writer_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_plain ( json_writer_t *this_, utf8string_t *text );

/*!
 *  \brief writes stringview to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_plain_view ( json_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param id the identifier
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_writer_write_plain_id ( json_writer_t *this_, data_id_t id );

/*!
 *  \brief prints an integer
 *
 *  \param this_ pointer to own object attributes
 *  \param number the integer to print
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t json_writer_write_int ( json_writer_t *this_, int64_t number );

/*!
 *  \brief writes a string to a file, json-string encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_string_enc ( json_writer_t *this_, utf8string_t *text );

/*!
 *  \brief writes a stringview to a file, json-string encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_string_view_enc ( json_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief writes a string to a file, encoded as list of json-strings, one string per line
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write, encoded for json comments
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_stringlist_enc ( json_writer_t *this_, utf8string_t *text );

/*!
 *  \brief writes a stringview to a file, encoded as list of json-strings, one string per line
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_stringlist_view_enc ( json_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief writes a member name and a value integer
 *
 *  \param this_ pointer to own object attributes
 *  \param indent indentation level: number of tabs, 0 <= indent <= JSON_WRITER_MAX_INDENT
 *  \param enc_name name of the object member, json encoded string
 *  \param number_value the integer to print
 *  \param next_follows true if another member follows (a comma will be printed)
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_member_int ( json_writer_t *this_,
                                                        unsigned int indent,
                                                        utf8string_t *enc_name,
                                                        int64_t number_value,
                                                        bool next_follows
                                                      );

/*!
 *  \brief writes a member name and an string, the value string is being json encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param indent indentation level: number of tabs, 0 <= indent <= JSON_WRITER_MAX_INDENT
 *  \param enc_name name of the object member, json encoded string
 *  \param unenc_value string to write, being json encoded
 *  \param next_follows true if another member follows (a comma will be printed)
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_member_string ( json_writer_t *this_,
                                                           unsigned int indent,
                                                           utf8string_t *enc_name,
                                                           utf8string_t *unenc_value,
                                                           bool next_follows
                                                         );

/*!
 *  \brief writes a member name and an string,
 *         the value string is split at newlines and being json array encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param indent indentation level: number of tabs, 0 <= indent <= JSON_WRITER_MAX_INDENT
 *  \param enc_name name of the object member, json encoded string
 *  \param unenc_value string to write, being json encoded
 *  \param next_follows true if another member follows (a comma will be printed)
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t json_writer_write_member_string_array ( json_writer_t *this_,
                                                                 unsigned int indent,
                                                                 utf8string_t *enc_name,
                                                                 utf8string_t *unenc_value,
                                                                 bool next_follows
                                                               );

#include "json_writer.inl"

#endif  /* JSON_WRITER_H */


/*
Copyright 2021-2024 Andreas Warnke

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
