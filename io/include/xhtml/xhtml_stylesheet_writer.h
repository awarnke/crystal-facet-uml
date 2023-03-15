/* File: xhtml_stylesheet_writer.h; Copyright and License: see below */

#ifndef XHTML_STYLESHEET_WRITER_H
#define XHTML_STYLESHEET_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Writes a css stylesheet to one file output stream.
 */

#include "u8stream/universal_output_stream.h"

/*!
 *  \brief attributes of the format writer
 */
struct xhtml_stylesheet_writer_struct {
    universal_output_stream_t *output;  /*!< reference to a universal_output_stream_t */
};

typedef struct xhtml_stylesheet_writer_struct xhtml_stylesheet_writer_t;

/*!
 *  \brief initializes the format writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated output to
 */
void xhtml_stylesheet_writer_init( xhtml_stylesheet_writer_t *this_,
                                   universal_output_stream_t *output
                                 );

/*!
 *  \brief destroys the format writer
 *
 *  \param this_ pointer to own object attributes
 */
void xhtml_stylesheet_writer_destroy( xhtml_stylesheet_writer_t *this_ );

/*!
 *  \brief writes a css stylesheet file
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t xhtml_stylesheet_writer_write_stylesheet( xhtml_stylesheet_writer_t *this_ );

#endif  /* XHTML_STYLESHEET_WRITER_H */


/*
Copyright 2019-2023 Andreas Warnke

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
