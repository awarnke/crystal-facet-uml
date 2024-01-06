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

#include "format/io_xml_writer.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "u8/u8_error.h"

/*!
 *  \brief constants for max string sizes
 */
enum document_link_provider_max_enum {
    DOCUMENT_LINK_PROVIDER_MAX_LINKS = 10,  /*!< in case of more than 10 occurrences, using a search is more */
                                            /*!< efficient than going through a list of links as provided by this */
                                            /*!< document_link_provider_t */
};

/*!
 *  \brief attributes of the xml writer
 */
struct document_link_provider_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */

    data_diagram_t temp_diagram[ DOCUMENT_LINK_PROVIDER_MAX_LINKS ];  /*!< memory for temporary diagram storage */
};

typedef struct document_link_provider_struct document_link_provider_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 */
void document_link_provider_init( document_link_provider_t *this_,
                                  data_database_reader_t *db_reader
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
 *  \param classifier_id id of the classifier
 *  \param[out] out_diagram pointer to array of diagrams read from the database (in case of success or exceeded buffer)
 *  \param[out] out_diagram_count number of diagram records stored in out_diagram
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if the database is not open,
 *          U8_ERROR_ARRAY_BUFFER_EXCEEDED if some results are dropped.
 */
u8_error_t document_link_provider_get_occurrences( document_link_provider_t *this_,
                                                   data_id_t classifier_id,
                                                   data_diagram_t (**out_diagram)[],
                                                   uint32_t *out_diagram_count
                                                 );

#endif  /* DOCUMENT_LINK_PROVIDER_H */


/*
Copyright 2023-2024 Andreas Warnke

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
