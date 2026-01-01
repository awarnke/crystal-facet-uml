/* File: universal_escaping_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_ESCAPING_OUTPUT_STREAM_H
#define UNIVERSAL_ESCAPING_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_output_stream_if that replaces a set of predefined
 *         character sequences by other character sequences and forwards the output
 *         to an other output stream.
 *
 *  This class can be used to e.g escape a couple of characters by escape sequences
 *  or to indent lines after every newline character.
 *  This class does not search for sequences that are split between multiple write calls.
 *
 *  Examples for patterns_and_replacements:
 *  \code
 *      const char *const SQL_ENCODE[][2] = {
 *          { "\0", "\\0" },  //  within strings, null cannot be represented.
 *          { "\x09", "\\t" },
 *          { "\x0a", "\\n" },
 *          { "\x0d", "\\r" },
 *          { "\x0e", "\\b" },
 *          { "\x1a", "\\z" },
 *          { "\"", "\\\"" },
 *          { "'", "\\'" },
 *          { "\\", "\\\\" },
 *          { "%", "\\%" },  //  % replacement only needed in searches by LIKE operator
 *          { "_", "\\_" },  //  _ replacement only needed in searches by LIKE operator
 *          { NULL, NULL }
 *      };
 *      utf8stringbuf_replace_all( mySqlBuf, &SQL_ENCODE );
 *      const char *const XML_ENCODE[][2] = {
 *          { "<", "&lt;" },
 *          { ">", "&gt;" },
 *          { "&", "&amp;" },
 *          { "\"", "&quot;" },  //  " replacement only needed in attribute values
 *          { "'", "&apos;" },  //  ' replacement only needed in attribute values
 *          { NULL, NULL }
 *      };
 *      utf8stringbuf_replace_all( myXmlBuf, &XML_ENCODE );
 *  \endcode
 *
 */

#include "u8stream/universal_output_stream.h"

/*!
 *  \brief attributes of the universal_escaping_output_stream
 */
struct universal_escaping_output_stream_struct {
    universal_output_stream_t output_stream;  /*!< instance of implemented interface \c universal_output_stream_t */
    const char *const ((*patterns_and_replacements)[][2]);  /*!< array of 0-terminated pattern and replacement strings, NULL terminated. */
    universal_output_stream_t *sink;  /*!< pointer to data stream sink */
};

typedef struct universal_escaping_output_stream_struct universal_escaping_output_stream_t;

/*!
 *  \brief initializes the universal_escaping_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param patterns_and_replacements pointer to 2-dim array, NULL-terminated,
 *         each arrray-entry is a pointer to a 0-terminated string.
 *  \param sink pointer to data stream sink
 */
void universal_escaping_output_stream_init( universal_escaping_output_stream_t *this_,
                                            const char *const ((*patterns_and_replacements)[][2]),
                                            universal_output_stream_t *sink
                                          );

/*!
 *  \brief destroys the universal_escaping_output_stream_t but not the underlying sink
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_escaping_output_stream_destroy( universal_escaping_output_stream_t *this_ );

/*!
 *  \brief changes the patterns_and_replacements attribute
 *
 *  \param this_ pointer to own object attributes
 *  \param patterns_and_replacements pointer to 2-dim array, NULL-terminated,
 *         each arrray-entry is a pointer to a 0-terminated string.
 */
void universal_escaping_output_stream_change_rules( universal_escaping_output_stream_t *this_,
                                                    const char *const ((*patterns_and_replacements)[][2])
                                                  );

/*!
 *  \brief writes a buffer (e.g. a stringview) to the data sink, replacing patterns by replacements
 *
 *  Note that patterns can not cross two write operations. Writing "Hel" and "lo" will not match on "ll".
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t universal_escaping_output_stream_write ( universal_escaping_output_stream_t *this_, const void *start, size_t length );

/*!
 *  \brief flushes buffers including the ones of the underlying sink
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_escaping_output_stream_flush( universal_escaping_output_stream_t *this_ );

/*!
 *  \brief gets the output stream interface of this universal_escaping_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
universal_output_stream_t* universal_escaping_output_stream_get_output_stream( universal_escaping_output_stream_t *this_ );

#endif  /* UNIVERSAL_ESCAPING_OUTPUT_STREAM_H */


/*
Copyright 2020-2026 Andreas Warnke

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
