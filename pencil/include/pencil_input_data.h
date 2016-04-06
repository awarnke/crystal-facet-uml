/* File: pencil_input_data.h; Copyright and License: see below */

#ifndef PENCIL_INPUT_DATA_H
#define PENCIL_INPUT_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads all input data for the pencil_diagram_painter from the database
 */

#include "data_diagram.h"
#include "storage/data_database_reader.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the pencil_input_data_t
 */
struct pencil_input_data_struct {
    data_diagram_t diagram;  /*!< main diagram record */
};

typedef struct pencil_input_data_struct pencil_input_data_t;

/*!
 *  \brief initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_input_data_init( pencil_input_data_t *this_ );

/*!
 *  \brief destroys the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_input_data_destroy( pencil_input_data_t *this_ );

/*!
 *  \brief fetches the painter input data from the database
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_input_data_load( pencil_input_data_t *this_, int64_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief gets the address of the diagram within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ );

/*!
 *  \brief checks if the diagram data is valid
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool pencil_input_data_is_valid ( pencil_input_data_t *this_ );

/*!
 *  \brief invalidates the diagram data
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_invalidate ( pencil_input_data_t *this_ );

#include "pencil_input_data.inl"

#endif  /* PENCIL_INPUT_DATA_H */


/*
Copyright 2016-2016 Andreas Warnke

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
