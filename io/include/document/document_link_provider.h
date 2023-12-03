/* File: document_link_provider.h; Copyright and License: see below */

#ifndef DOCUMENT_LINK_PROVIDER_H
#define DOCUMENT_LINK_PROVIDER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief parses some md-like and link features of the input string to an xml writer.
 *
 *  This class is a filter in the sense of the pipes-and-filters architecture pattern:
 *  It modifies and enriches the piped-through data.
 */

#include "xml/xml_writer.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "u8/u8_error.h"

/*!
 *  \brief constants for max string sizes
 */
enum document_link_provider_max_enum {
    DOCUMENT_LINK_PROVIDER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the xml writer
 */
struct document_link_provider_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    xml_writer_t *sink;  /*!< output sink */

    const char * tag_xref_start;  /*!< tag for xref_start */
    const char * tag_xref_middle;  /*!< tag for xref_middle */
    const char * tag_xref_end;  /*!< tag for xref_end */
    const char * tag_xref_next;  /*!< tag for continuing with another xref */
    const char * tag_xref_more;  /*!< tag for indicating that there are more, skipped, xrefs */

    data_diagram_t temp_diagram;  /*!< memory for temporary diagram storage */
};

typedef struct document_link_provider_struct document_link_provider_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param tag_xref_start tag for xref_start
 *  \param tag_xref_middle tag for xref_middle
 *  \param tag_xref_end tag for xref_end
 *  \param tag_xref_next tag for continuing with another xref
 *  \param tag_xref_more tag for indicating that there are more, skipped, xrefs
 *  \param sink stream sink where to write the output to
 */
void document_link_provider_init( document_link_provider_t *this_,
                                  data_database_reader_t *db_reader,
                                  const char * tag_xref_start,
                                  const char * tag_xref_middle,
                                  const char * tag_xref_end,
                                  const char * tag_xref_next,
                                  const char * tag_xref_more,
                                  xml_writer_t *sink
                                );

/*!
 *  \brief destroys the document exporter
 *
 *  \param this_ pointer to own object attributes
 */
void document_link_provider_destroy( document_link_provider_t *this_ );

/*!
 *  \brief parses the string, enriches it by links and paragraph-breaks and writes it to the sink
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to parse
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_link_provider_write_occurrences( document_link_provider_t *this_, const char *text );

#endif  /* DOCUMENT_LINK_PROVIDER_H */


/*
Copyright 2023-2023 Andreas Warnke

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
