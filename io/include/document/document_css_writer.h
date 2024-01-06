/* File: document_css_writer.h; Copyright and License: see below */

#ifndef DOCUMENT_CSS_WRITER
#define DOCUMENT_CSS_WRITER

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes a css stylesheet to one file output stream.
 */

#include "u8stream/universal_output_stream.h"

/*!
 *  \brief attributes of the format writer
 */
struct document_css_writer_struct {
    universal_output_stream_t *output;  /*!< reference to a universal_output_stream_t */
};

typedef struct document_css_writer_struct document_css_writer_t;

/*!
 *  \brief initializes the format writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated output to
 */
void document_css_writer_init( document_css_writer_t *this_,
                               universal_output_stream_t *output
                             );

/*!
 *  \brief destroys the format writer
 *
 *  \param this_ pointer to own object attributes
 */
void document_css_writer_destroy( document_css_writer_t *this_ );

/*!
 *  \brief writes a css stylesheet file
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t document_css_writer_write_stylesheet( document_css_writer_t *this_ );

#endif  /* DOCUMENT_CSS_WRITER */


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
