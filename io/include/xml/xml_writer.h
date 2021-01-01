/* File: xml_writer.h; Copyright and License: see below */

#ifndef XML_WRITER_H
#define XML_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief writes strings to an xml stream
 */

#include "data_diagram.h"
#include "data_classifier.h"
#include "data_table.h"
#include "util/string/utf8stringbuf.h"
#include "stream/universal_output_stream.h"
#include "stream/universal_escaping_output_stream.h"

/*!
 *  \brief constants for max string sizes
 */
enum xml_writer_max_enum {
    XML_WRITER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

extern const char XML_WRITER_START_TAG_START[2];
extern const char XML_WRITER_START_TAG_END[2];
extern const char XML_WRITER_END_TAG_START[3];
extern const char XML_WRITER_END_TAG_END[2];
/*!
 *  \brief \c XML \c empty \c tags are leaf tags that do not contain contents or other tags - only attributes
 */
extern const char XML_WRITER_EMPTY_TAG_START[2];
extern const char XML_WRITER_EMPTY_TAG_END[3];
extern const char XML_WRITER_ATTR_SEPARATOR[2];
extern const char XML_WRITER_ATTR_VALUE_START[3];
extern const char XML_WRITER_ATTR_VALUE_END[2];
extern const char XML_WRITER_COMMENT_START[5];
extern const char XML_WRITER_COMMENT_END[4];
extern const char XML_WRITER_NL[2];

/*!
 *  \brief attributes of the xml writer
 */
struct xml_writer_struct {
    universal_output_stream_t *output;  /*!< output stream where to write the generated document to */
    universal_escaping_output_stream_t esc_output;  /*!< escaping output stream filter that does the xml escaping */
    unsigned int indent_level;  /*!< current indentation level of written lines */

    const char *const ((*xml_encode_table)[][2]);  /*!< table for xml encode string replacements */
    const char *const ((*xml_comments_encode_table)[][2]);  /*!< table for xml coments encode string replacements */
    const char *const ((*xml_plain_table)[][2]);  /*!< table for xml plain output, just performing indentation */
};

typedef struct xml_writer_struct xml_writer_t;

/*!
 *  \brief initializes the xml writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated document to
 */
void xml_writer_init( xml_writer_t *this_,
                      universal_output_stream_t *output
                    );

/*!
 *  \brief destroys the xml writer
 *
 *  \param this_ pointer to own object attributes
 */
void xml_writer_destroy( xml_writer_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_plain ( xml_writer_t *this_, const char *text );

/*!
 *  \brief writes buffer (stringview) to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_plain_buf ( xml_writer_t *this_, const char *start, size_t length );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param id the identifier
 *  \result 0 in case of success, -1 otherwise
 */
int xml_writer_write_plain_id ( xml_writer_t *this_, data_id_t id );

/*!
 *  \brief prints an integer
 *
 *  \param this_ pointer to own object attributes
 *  \param number the integer to print
 *  \result 0 in case of success, -1 otherwise
 */
int xml_writer_write_int ( xml_writer_t *this_, int64_t number );

/*!
 *  \brief writes a string to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_xml_enc ( xml_writer_t *this_, const char *text );

/*!
 *  \brief writes a buffer (stringview) to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_xml_enc_buf ( xml_writer_t *this_, const char *start, size_t length );

/*!
 *  \brief writes a string to a file, xml encoded and double-minus gets space-separated
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write, encoded for xml comments
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_xml_comment ( xml_writer_t *this_, const char *text );

/*!
 *  \brief resets the indentation level to 0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void xml_writer_reset_indent ( xml_writer_t *this_ );

/*!
 *  \brief increases the indentation level
 *
 *  \param this_ pointer to own object attributes
 */
static inline void xml_writer_increase_indent ( xml_writer_t *this_ );

/*!
 *  \brief decreases the indentation level
 *
 *  \param this_ pointer to own object attributes
 */
static inline void xml_writer_decrease_indent ( xml_writer_t *this_ );

/*!
 *  \brief adapts the encoding tables according to the current indentation level
 *
 *  asserts that no more decreases are called than increases
 *
 *  \param this_ pointer to own object attributes
 */
void xml_writer_update_encoding_tables ( xml_writer_t *this_ );

#include "xml_writer.inl"

#endif  /* XML_WRITER_H */


/*
Copyright 2019-2021 Andreas Warnke

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
