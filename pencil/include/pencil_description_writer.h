/* File: pencil_description_writer.h; Copyright and License: see below */

#ifndef PENCIL_DESCRIPTION_WRITER_H
#define PENCIL_DESCRIPTION_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders diagram and classifier descriptions to an output stream
 */

#include "pencil_input_data.h"
#include <stdio.h>

/*!
 *  \brief attributes of the description writer
 */
struct pencil_description_writer_struct {
    pencil_input_data_t *input_data;  /*!< pointer to an external data cache */
};

typedef struct pencil_description_writer_struct pencil_description_writer_t;

/*!
 *  \brief initializes the pencil_description_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be rendered
 */
void pencil_description_writer_init( pencil_description_writer_t *this_, pencil_input_data_t *input_data );

/*!
 *  \brief destroys the pencil_description_writer_t
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_description_writer_destroy( pencil_description_writer_t *this_ );

/*!
 *  \brief prints names and descriptions of the diagram and contained classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int pencil_description_writer_draw ( pencil_description_writer_t *this_, FILE *out );

#endif  /* PENCIL_DESCRIPTION_WRITER_H */


/*
Copyright 2016-2017 Andreas Warnke

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
