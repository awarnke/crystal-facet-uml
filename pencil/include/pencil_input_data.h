/* File: pencil_input_data.h; Copyright and License: see below */

#ifndef PENCIL_INPUT_DATA_H
#define PENCIL_INPUT_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads all input data for the pencil_diagram_painter from the database
 */

#include "data_diagram.h"
#include "data_visible_classifier.h"
#include "storage/data_database_reader.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of pencil_input_data_t
 */
enum pencil_input_data_max_enum {
    GUI_SKETCH_AREA_CONST_MAX_CLASSIFIERS = 256,  /*!< maximum number of classifiers to be shown in one single diagram */
};

/*!
 *  \brief attributes of the pencil_input_data_t
 */
struct pencil_input_data_struct {
    data_diagram_t diagram;  /*!< the diagram record */
    uint32_t visible_classifier_count;  /*!< number of all contained visible classifier records */
    data_visible_classifier_t visible_classifiers[GUI_SKETCH_AREA_CONST_MAX_CLASSIFIERS];  /*!< all contained visible_classifier records */
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
 *  \param diagram_id id of the diagram for which the data shall be loaded
 *  \param db_reader database reader to use when acquiring data
 */
void pencil_input_data_load( pencil_input_data_t *this_, int64_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief gets the address of the diagram within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to current diagram cache
 */
static inline data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ );

/*!
 *  \brief gets the number of visible classifiers within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_input_data_get_visible_classifier_count ( pencil_input_data_t *this_ );

/*!
 *  \brief gets the address of the diagram within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the classifier to retrieve; 0 <= index < pencil_input_data_get_classifier_count.
 *  \return NULL if index >= pencil_input_data_get_classifier_count; pointer to data_visible_classifier_t otherwise.
 */
static inline data_visible_classifier_t *pencil_input_data_get_visible_classifier_ptr ( pencil_input_data_t *this_, uint32_t index );

/*!
 *  \brief checks if the diagram and diagram-contents data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this_ contains valid data for sketching a diagram
 */
static inline bool pencil_input_data_is_valid ( pencil_input_data_t *this_ );

/*!
 *  \brief invalidates the diagram data
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_invalidate ( pencil_input_data_t *this_ );

/*!
 *  \brief destroys all contained visible classifiers
 *
 *  classifier_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_private_destroy_visible_classifiers( pencil_input_data_t *this_ );

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
