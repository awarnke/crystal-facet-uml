/* File: layout_relationship.h; Copyright and License: see below */

#ifndef LAYOUT_RELATIONSHIP_H
#define LAYOUT_RELATIONSHIP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines visibility, shape and location of a relationship
 */

#include "layout/layout_visible_classifier.h"
#include "layout/layout_feature.h"
#include "option/pencil_visibility.h"
#include "util/geometry/geometry_connector.h"
#include "data_relationship.h"

/*!
 *  \brief visibility, shape and location of a relationship
 */
struct layout_relationship_struct {
    pencil_visibility_t visible;  /*!< defines if the relationship is visible */
    geometry_connector_t shape;  /*!< shape of relationship records */
    const data_relationship_t *data;  /*!< pointer to the data object of the relationship */
    layout_visible_classifier_t *from_classifier;  /*!< pointer to the layout of the source visible classifier */
    layout_visible_classifier_t *to_classifier;  /*!< pointer to the layout of the destination visible classifier */
    layout_feature_t *from_feature;  /*!< pointer to the layout of the source feature, NULL if none */
    layout_feature_t *to_feature;  /*!< pointer to the layout of the destination feature, NULL if none */
};

typedef struct layout_relationship_struct layout_relationship_t;

/*!
 *  \brief initializes the layout_relationship_t struct with a pointer to data_relationship_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_data pointer to data object which is layouted
 *  \param from_classifier pointer to layout of the source classifier object
 *  \param to_classifier pointer to layout of the destination classifier object
 *  \param from_feature pointer to layout of the source feature object or NULL if not originating from a feature
 *  \param to_feature pointer to layout of the destination feature object or NULL if not directed to a feature
 */
static inline void layout_relationship_init ( layout_relationship_t *this_,
                                              const data_relationship_t *relationship_data,
                                              layout_visible_classifier_t *from_classifier,
                                              layout_visible_classifier_t *to_classifier,
                                              layout_feature_t *from_feature,
                                              layout_feature_t *to_feature
                                            );

/*!
 *  \brief destroys the layout_relationship_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_relationship_destroy ( layout_relationship_t *this_ );

/*!
 *  \brief gets the visibility of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \return visibility of the relationship layout.
 */
static inline pencil_visibility_t layout_relationship_get_visibility ( const layout_relationship_t *this_ );

/*!
 *  \brief sets the visibility of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param visible visibility of the relationship layout.
 */
static inline void layout_relationship_set_visibility ( layout_relationship_t *this_, pencil_visibility_t visible );

/*!
 *  \brief gets the shape of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_connector_t *layout_relationship_get_shape_ptr ( layout_relationship_t *this_ );

/*!
 *  \brief gets the data_relationship_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a data_relationship_t object.
 */
static inline const data_relationship_t *layout_relationship_get_data_ptr ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the source layout_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the source layout_visible_classifier_t object.
 */
static inline layout_visible_classifier_t *layout_relationship_get_from_classifier_ptr ( layout_relationship_t *this_ );

/*!
 *  \brief gets the destination layout_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the destination layout_visible_classifier_t object.
 */
static inline layout_visible_classifier_t *layout_relationship_get_to_classifier_ptr ( layout_relationship_t *this_ );

/*!
 *  \brief gets the source layout_feature_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the source layout_feature_t object or NULL if the relationship does not originate from a feature
 */
static inline layout_feature_t *layout_relationship_get_from_feature_ptr ( layout_relationship_t *this_ );

/*!
 *  \brief gets the destination layout_feature_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the destination layout_feature_t object, or NULL if the relationship does not point to a feature
 */
static inline layout_feature_t *layout_relationship_get_to_feature_ptr ( layout_relationship_t *this_ );

#include "layout_relationship.inl"

#endif  /* LAYOUT_RELATIONSHIP_H */


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
