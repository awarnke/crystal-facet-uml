/* File: json_writer.h; Copyright and License: see below */

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief writes strings to an json utf8 stream
 */

#include "data_diagram.h"
#include "data_classifier.h"
#include "data_table.h"
#include "util/string/utf8string.h"
#include "util/string/utf8stringview.h"
#include "stream/universal_output_stream.h"
#include "stream/universal_escaping_output_stream.h"

/*!
 *  \brief constants for max string sizes
 */
enum json_writer_max_enum {
    XML_WRITER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the xml writer
 */
struct json_writer_struct {
    universal_output_stream_t *output;  /*!< output stream where to write the generated document to */
    universal_escaping_output_stream_t esc_output;  /*!< escaping output stream filter that does the xml escaping */
    unsigned int indent_level;  /*!< current indentation level of written lines */

    const char *const ((*json_string_encode_table)[][2]);  /*!< table for json encode string replacements */
    const char *const ((*json_stringlist_encode_table)[][2]);  /*!< table for json encode stringlist replacements */
};

typedef struct json_writer_struct json_writer_t;

/*!
 *  \brief initializes the xml writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated document to
 */
void json_writer_init( json_writer_t *this_,
                       universal_output_stream_t *output
                     );

/*!
 *  \brief destroys the xml writer
 *
 *  \param this_ pointer to own object attributes
 */
void json_writer_destroy( json_writer_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return 0 in case of success, -1 otherwise
 */
static inline int json_writer_write_plain ( json_writer_t *this_, utf8string_t text );

/*!
 *  \brief writes stringview to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return 0 in case of success, -1 otherwise
 */
static inline int json_writer_write_plain_view ( json_writer_t *this_, utf8stringview_t string_view );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param id the identifier
 *  \return 0 in case of success, -1 otherwise
 */
int json_writer_write_plain_id ( json_writer_t *this_, data_id_t id );

/*!
 *  \brief prints an integer
 *
 *  \param this_ pointer to own object attributes
 *  \param number the integer to print
 *  \return 0 in case of success, -1 otherwise
 */
int json_writer_write_int ( json_writer_t *this_, int64_t number );

/*!
 *  \brief writes a string to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return 0 in case of success, -1 otherwise
 */
static inline int json_writer_write_string_enc ( json_writer_t *this_, utf8string_t text );

/*!
 *  \brief writes a stringview to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return 0 in case of success, -1 otherwise
 */
static inline int json_writer_write_string_view_enc ( json_writer_t *this_, utf8stringview_t string_view );

/*!
 *  \brief writes a string to a file, xml encoded and double-minus gets space-separated
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write, encoded for xml comments
 *  \return 0 in case of success, -1 otherwise
 */
static inline int json_writer_write_stringlist_enc ( json_writer_t *this_, utf8string_t text );

/*!
 *  \brief writes a stringview to a file, xml encoded and double-minus gets space-separated
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return 0 in case of success, -1 otherwise
 */
static inline int json_writer_write_stringlist_view_enc ( json_writer_t *this_, utf8stringview_t string_view );

#include "json_writer.inl"

#endif  /* JSON_WRITER_H */


/*
Copyright 2021-2021 Andreas Warnke

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
