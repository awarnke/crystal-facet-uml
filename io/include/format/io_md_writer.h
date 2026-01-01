/* File: io_md_writer.h; Copyright and License: see below */

#ifndef IO_MD_WRITER_H
#define IO_MD_WRITER_H

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
#include "entity/data_diagram.h"
#include "u8/u8_error.h"

/*!
 *  \brief constants for max string sizes
 */
enum io_md_writer_max_enum {
    IO_MD_WRITER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the xml writer
 */
struct io_md_writer_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    io_xml_writer_t *sink;  /*!< output sink */

    const char * tag_linebreak;  /*!< tag for linebreak */
    const char * tag_xref_start;  /*!< tag for xref_start */
    const char * tag_xref_middle;  /*!< tag for xref_middle */
    const char * tag_xref_end;  /*!< tag for xref_end */

    data_diagram_t temp_diagram;  /*!< memory for temporary diagram storage */
};

typedef struct io_md_writer_struct io_md_writer_t;

/*!
 *  \brief initializes the markdown writer
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param tag_linebreak tag for linebreak
 *  \param tag_xref_start tag for xref_start
 *  \param tag_xref_middle tag for xref_middle
 *  \param tag_xref_end tag for xref_end
 *  \param sink stream sink where to write the output to
 */
void io_md_writer_init( io_md_writer_t *this_,
                        data_database_reader_t *db_reader,
                        const char * tag_linebreak,
                        const char * tag_xref_start,
                        const char * tag_xref_middle,
                        const char * tag_xref_end,
                        io_xml_writer_t *sink
                      );

/*!
 *  \brief destroys the markdown writer
 *
 *  \param this_ pointer to own object attributes
 */
void io_md_writer_destroy( io_md_writer_t *this_ );

/*!
 *  \brief parses the string, enriches it by links and paragraph-breaks and writes it to the sink
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to parse
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_md_writer_transform ( io_md_writer_t *this_, const char *text );

#endif  /* IO_MD_WRITER_H */


/*
Copyright 2019-2026 Andreas Warnke

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
