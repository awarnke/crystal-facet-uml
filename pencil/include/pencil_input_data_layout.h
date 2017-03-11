/* File: pencil_input_data_layout.h; Copyright and License: see below */

#ifndef PENCIL_INPUT_DATA_LAYOUT_H
#define PENCIL_INPUT_DATA_LAYOUT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the layout data for input data.
 */

#include "util/geometry/geometry_connector.h"
#include "util/geometry/geometry_rectangle.h"
#include "pencil_input_data.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of pencil_input_data_layout_t
 */
enum pencil_input_data_layout_max_enum {
    PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS = PENCIL_INPUT_DATA_MAX_CLASSIFIERS,  /*!< maximum number of classifiers to be shown in one single diagram */
    PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS = PENCIL_INPUT_DATA_MAX_RELATIONSHIPS,  /*!< maximum number of relationships to be shown in one single diagram */
};

/*!
 *  \brief attributes of the pencil_input_data_layout_t
 */
struct pencil_input_data_layout_struct {
    /* classifier layout*/
    geometry_rectangle_t classifier_bounds[PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS];  /*!< outer bounds of classifier records */
    geometry_rectangle_t classifier_space[PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS];  /*!< inner space of classifier records where features or contained classifiers are drawn */

    /* relationship layout */
    bool relationship_visible[PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS];  /*!< defines if the relationship is visible */
    geometry_connector_t relationship_shape[PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS];  /*!< shape of relationship records */
};

typedef struct pencil_input_data_layout_struct pencil_input_data_layout_t;

/*!
 *  \brief initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_layout_init( pencil_input_data_layout_t *this_ );

/*!
 *  \brief destroys the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_layout_destroy( pencil_input_data_layout_t *this_ );

/*!
 *  \brief gets the outer geometry of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the classifier bounds to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the inner space of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the classifier space to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_space_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the geometry of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship shape to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_connector_t *pencil_input_data_layout_get_relationship_shape_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS.
 *  \return true if visible.
 */
static inline bool pencil_input_data_layout_is_relationship_visible ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief sets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS.
 *  \param visible true if visible.
 */
static inline void pencil_input_data_layout_set_relationship_visible ( pencil_input_data_layout_t *this_, uint32_t index, bool visible );

#include "pencil_input_data_layout.inl"

#endif  /* PENCIL_INPUT_DATA_LAYOUT_H */


/*
Copyright 2017-2017 Andreas Warnke

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
