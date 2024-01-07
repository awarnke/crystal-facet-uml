/* File: io_xml_writer.h; Copyright and License: see below */

#ifndef IO_XML_WRITER_H
#define IO_XML_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief writes strings to an xml stream
 */

#include "data_diagram.h"
#include "data_classifier.h"
#include "data_table.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include "u8stream/universal_output_stream.h"
#include "u8stream/universal_escaping_output_stream.h"
#include "u8/u8_error.h"

/*!
 *  \brief constants for max string sizes
 */
enum io_xml_writer_max_enum {
    IO_XML_WRITER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

extern const char IO_XML_WRITER_START_TAG_START[2];
extern const char IO_XML_WRITER_START_TAG_END[2];
extern const char IO_XML_WRITER_END_TAG_START[3];
extern const char IO_XML_WRITER_END_TAG_END[2];
/*!
 *  \brief \c XML \c empty \c tags are leaf tags that do not contain contents or other tags - only attributes
 */
extern const char IO_XML_WRITER_EMPTY_TAG_START[2];
extern const char IO_XML_WRITER_EMPTY_TAG_END[3];
extern const char IO_XML_WRITER_ATTR_SEPARATOR[2];
extern const char IO_XML_WRITER_ATTR_VALUE_START[3];
extern const char IO_XML_WRITER_ATTR_VALUE_END[2];
extern const char IO_XML_WRITER_COMMENT_START[5];
extern const char IO_XML_WRITER_COMMENT_END[4];
extern const char IO_XML_WRITER_NL[2];

/*!
 *  \brief attributes of the xml writer
 */
struct io_xml_writer_struct {
    universal_output_stream_t *output;  /*!< output stream where to write the generated document to */
    universal_escaping_output_stream_t esc_output;  /*!< escaping output stream filter that does the xml escaping */
    unsigned int indent_level;  /*!< current indentation level of written lines */

    const char *const ((*xml_encode_table)[][2]);  /*!< table for xml encode string replacements */
    const char *const ((*xml_comments_encode_table)[][2]);  /*!< table for xml comments encode string replacements */
    const char *const ((*xml_plain_table)[][2]);  /*!< table for xml plain output, just performing indentation */
};

typedef struct io_xml_writer_struct io_xml_writer_t;

/*!
 *  \brief initializes the xml writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated document to
 */
void io_xml_writer_init( io_xml_writer_t *this_,
                         universal_output_stream_t *output
                       );

/*!
 *  \brief destroys the xml writer
 *
 *  \param this_ pointer to own object attributes
 */
void io_xml_writer_destroy( io_xml_writer_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_xml_writer_write_plain ( io_xml_writer_t *this_, utf8string_t text );

/*!
 *  \brief writes stringview to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_xml_writer_write_plain_view ( io_xml_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param id the identifier
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_xml_writer_write_plain_id ( io_xml_writer_t *this_, data_id_t id );

/*!
 *  \brief prints an integer
 *
 *  \param this_ pointer to own object attributes
 *  \param number the integer to print
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_xml_writer_write_int ( io_xml_writer_t *this_, int64_t number );

/*!
 *  \brief writes a string to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_xml_writer_write_xml_enc ( io_xml_writer_t *this_, utf8string_t text );

/*!
 *  \brief writes a stringview to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_xml_writer_write_xml_enc_view ( io_xml_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief writes a string to a file, xml encoded and double-minus gets space-separated
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write, encoded for xml comments
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_xml_writer_write_xml_comment ( io_xml_writer_t *this_, utf8string_t text );

/*!
 *  \brief writes a stringview to a file, xml encoded and double-minus gets space-separated
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view stringview to write, not 0-terminated
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_xml_writer_write_xml_comment_view ( io_xml_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief checks if the stringview contains valid characters to form an xml-tag-name.
 *
 *  Checks for characters according to Extensible Markup Language (XML) 1.1, chapter 2.3, Names and Tokens.
 *
 *  Excludes the colon ":" because this is the namespace separator.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view string_view to check
 *  \return true if string_view contains characters of which a valid xml-tag-name can be constructed
 */
static inline bool io_xml_writer_contains_xml_tag_name_characters ( io_xml_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief writes a valid xml-tag-name.
 *
 *  Writes valid characters according to Extensible Markup Language (XML) 1.1, chapter 2.3, Names and Tokens
 *  to the writer. Invalid ones are filtered. E.g. ", Design Decsisions" is transformed to "DesignDecisions"
 *
 *  Excludes the colon ":" because this is the namespace separator.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_view string_view to 1) filter and 2) write
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_FOUND if none in string_view
 */
static inline u8_error_t io_xml_writer_write_xml_tag_name_characters ( io_xml_writer_t *this_, const utf8stringview_t *string_view );

/*!
 *  \brief resets the indentation level to 0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_xml_writer_reset_indent ( io_xml_writer_t *this_ );

/*!
 *  \brief increases the indentation level
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_xml_writer_increase_indent ( io_xml_writer_t *this_ );

/*!
 *  \brief decreases the indentation level
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_xml_writer_decrease_indent ( io_xml_writer_t *this_ );

/*!
 *  \brief adapts the encoding tables according to the current indentation level
 *
 *  asserts that no more decreases are called than increases
 *
 *  \param this_ pointer to own object attributes
 */
void io_xml_writer_private_update_encoding_tables ( io_xml_writer_t *this_ );

/*!
 *  \brief checks if the codepoint is a valid character in an xml name
 *
 *  Checks for characters according to Extensible Markup Language (XML) 1.1, chapter 2.3, Names and Tokens
 *
 *  Excludes the colon ":" because this is the namespace separator.
 *
 *  \param this_ pointer to own object attributes
 *  \param codepoint codepoint to check
 *  \param start true if this codepoint shall be the start of an xml name
 *  \return true if codepoint is valid in an xml-tag-name
 */
static inline bool io_xml_writer_private_is_xml_tag_name_character ( io_xml_writer_t *this_, uint32_t codepoint, bool start );

#include "io_xml_writer.inl"

#endif  /* IO_XML_WRITER_H */


/*
Copyright 2019-2024 Andreas Warnke

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
