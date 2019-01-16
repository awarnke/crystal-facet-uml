/* File: layout_feature.h; Copyright and License: see below */

#ifndef LAYOUT_FEATURE_H
#define LAYOUT_FEATURE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines visibility, shape and location of a feature
 */

#include "layout/layout_visible_classifier.h"
#include "layout/layout_direction.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_feature.h"

/*!
 *  \brief visibility, shape and location of a feature
 */
struct layout_feature_struct {
    geometry_rectangle_t bounds;  /*!< bounds of a feature */
    layout_direction_t icon_direction;  /*!< icon direction, a hint for drawing the icon of a feature. */
    layout_direction_t label_direction;  /*!< label direction, a hint for drawing the name of a feature. */
    const data_feature_t *data;  /*!< pointer to the data object of the feature */
    layout_visible_classifier_t *classifier;  /*!< pointer to the layout of the parent visible classifier */
};

typedef struct layout_feature_struct layout_feature_t;

/*!
 *  \brief initializes the layout_feature_t struct with a pointer to feature_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_data pointer to data object which is layouted
 *  \param classifier pointer to layout of the parent classifier object
 */
static inline void layout_feature_init ( layout_feature_t *this_,
                                         const data_feature_t *feature_data,
                                         layout_visible_classifier_t *classifier
                                       );

/*!
 *  \brief destroys the layout_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_feature_destroy ( layout_feature_t *this_ );

/*!
 *  \brief gets the bounding box of the feature
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_feature_get_bounds_ptr ( layout_feature_t *this_ );

/*!
 *  \brief sets the bounding box of the feature
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_bounds coordinates of new bounding box
 */
static inline void layout_feature_set_bounds ( layout_feature_t *this_, const geometry_rectangle_t *feature_bounds );

/*!
 *  \brief gets the icon direction of the layout feature
 *
 *  \param this_ pointer to own object attributes
 *  \return icon direction of the feature
 */
static inline layout_direction_t layout_feature_get_icon_direction ( const layout_feature_t *this_ );

/*!
 *  \brief sets the icon direction of the layout feature
 *
 *  \param this_ pointer to own object attributes
 *  \param direction new icon direction to set
 */
static inline void layout_feature_set_icon_direction ( layout_feature_t *this_, layout_direction_t direction );

/*!
 *  \brief gets the label direction of the layout feature
 *
 *  \param this_ pointer to own object attributes
 *  \return label direction of the feature
 */
static inline layout_direction_t layout_feature_get_label_direction ( const layout_feature_t *this_ );

/*!
 *  \brief sets the label direction of the layout feature
 *
 *  \param this_ pointer to own object attributes
 *  \param direction new label direction to set
 */
static inline void layout_feature_set_label_direction ( layout_feature_t *this_, layout_direction_t direction );

/*!
 *  \brief gets the data_feature_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the corresponding data_feature_t object
 */
static inline const data_feature_t *layout_feature_get_data_ptr ( const layout_feature_t *this_ );

/*!
 *  \brief gets the layout_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the parent layout_visible_classifier_t object
 */
static inline layout_visible_classifier_t *layout_feature_get_classifier_ptr ( layout_feature_t *this_ );

/*!
 *  \brief gets the row id of the feature
 *
 *  This is a shortcut for data_feature_get_id( layout_feature_get_data_ptr ( X ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the feature
 */
static inline int64_t layout_feature_get_feature_id ( const layout_feature_t *this_ );

#include "layout_feature.inl"

#endif  /* LAYOUT_FEATURE_H */


/*
Copyright 2018-2019 Andreas Warnke

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
