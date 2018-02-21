/* File: layout_visible_classifier.h; Copyright and License: see below */

#ifndef LAYOUT_VISIBLE_CLASSIFIER_H
#define LAYOUT_VISIBLE_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines shape and location of a visible_classifier
 */

#include "util/geometry/geometry_rectangle.h"
#include "data_visible_classifier.h"

/*!
 *  \brief shape and location of a visible_classifier
 */
struct layout_visible_classifier_struct {
    geometry_rectangle_t classifier_bounds;  /*!< outer bounds of a classifier record */
    geometry_rectangle_t classifier_space;  /*!< inner space of a classifier record where features or contained classifiers are drawn */
    data_visible_classifier_t *visible_classifier_data;  /*!< pointer to the data object of the visible classifier */
};

typedef struct layout_visible_classifier_struct layout_visible_classifier_t;

/*!
 *  \brief initializes the layout_visible_classifier_t struct with a pointer to the corresponding data_visible_classifier_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier_data pointer to data object which is layouted
 */
static inline void layout_visible_classifier_init_empty ( layout_visible_classifier_t *this_, data_visible_classifier_t *visible_classifier_data );

/*!
 *  \brief destroys the layout_visible_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_visible_classifier_destroy ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the data_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a data_visible_classifier_t object.
 */
static inline data_visible_classifier_t *layout_visible_classifier_get_data_ptr ( layout_visible_classifier_t *this_ );

#include "layout_visible_classifier.inl"

#endif  /* LAYOUT_VISIBLE_CLASSIFIER_H */


/*
Copyright 2018-2018 Andreas Warnke

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
