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
    geometry_rectangle_t bounds;  /*!< outer bounds of a classifier record */
    geometry_rectangle_t space;  /*!< inner space of a classifier record where features or contained classifiers are drawn */
    const data_visible_classifier_t *data;  /*!< pointer to the data object of the visible classifier */
};

typedef struct layout_visible_classifier_struct layout_visible_classifier_t;

/*!
 *  \brief initializes the layout_visible_classifier_t struct with a pointer to the corresponding data_visible_classifier_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier_data pointer to data object which is layouted
 */
static inline void layout_visible_classifier_init ( layout_visible_classifier_t *this_, data_visible_classifier_t *visible_classifier_data );

/*!
 *  \brief destroys the layout_visible_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_visible_classifier_destroy ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the bounding box of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_visible_classifier_get_bounds_ptr ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the inner space box of the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_visible_classifier_get_space_ptr ( layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the data_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a data_visible_classifier_t object.
 */
static inline const data_visible_classifier_t *layout_visible_classifier_get_data_ptr ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the const classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a classifier.
 */
static inline const data_classifier_t *layout_visible_classifier_get_classifier_ptr ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the const diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a diagramelement.
 */
static inline const data_diagramelement_t *layout_visible_classifier_get_diagramelement_ptr ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the row id of the classifier
 *
 *  This is a shortcut for data_classifier_get_id( data_visible_classifier_get_classifier_const ( (*this_).data ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the classifier
 */
static inline int64_t layout_visible_classifier_get_classifier_id ( const layout_visible_classifier_t *this_ );

/*!
 *  \brief gets the row id of the diagramelement
 *
 *  This is a shortcut for data_diagramelement_get_id( data_visible_classifier_get_diagramelement_const ( (*this_).data ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the diagramelement
 */
static inline int64_t layout_visible_classifier_get_diagramelement_id ( const layout_visible_classifier_t *this_ );

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
