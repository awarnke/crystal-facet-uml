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
    geometry_rectangle_t label_box;  /*!< bounding box of the label of the relationship. */
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
 *  \brief checks if the relationship data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if (*this_).data points to valid data and is consistent with (*this_).from_classifier,
 *          (*this_).to_classifier, (*this_).from_feature, (*this_).to_feature
 */
static inline bool layout_relationship_is_valid ( const layout_relationship_t *this_ );

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
 *  \brief gets the shape of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_connector_t *layout_relationship_get_shape_const ( const layout_relationship_t *this_ );

/*!
 *  \brief sets the shape of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param shape new shape of this relationship
 */
static inline void layout_relationship_set_shape ( layout_relationship_t *this_, const geometry_connector_t *shape );

/*!
 *  \brief gets the middle location of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \return point of middle of connector.
 */
static inline geometry_point_t layout_relationship_get_middle ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the label bounds of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_relationship_get_label_box_ptr ( layout_relationship_t *this_ );

/*!
 *  \brief gets the label bounds of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_relationship_get_label_box_const ( const layout_relationship_t *this_ );

/*!
 *  \brief sets the label bounds of the relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param label_box coordinates of new label bounds
 */
static inline void layout_relationship_set_label_box ( layout_relationship_t *this_, const geometry_rectangle_t *label_box );

/*!
 *  \brief gets the data_relationship_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a data_relationship_t object.
 */
static inline const data_relationship_t *layout_relationship_get_data_const ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the source layout_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the source layout_visible_classifier_t object.
 */
static inline layout_visible_classifier_t *layout_relationship_get_from_classifier_ptr ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the destination layout_visible_classifier_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the destination layout_visible_classifier_t object.
 */
static inline layout_visible_classifier_t *layout_relationship_get_to_classifier_ptr ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the source layout_feature_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the source layout_feature_t object or NULL if the relationship does not originate from a feature
 */
static inline layout_feature_t *layout_relationship_get_from_feature_ptr ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the destination layout_feature_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the destination layout_feature_t object, or NULL if the relationship does not point to a feature
 */
static inline layout_feature_t *layout_relationship_get_to_feature_ptr ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the row id of the relationship
 *
 *  This is a shortcut for data_relationship_get_id( layout_relationship_get_data_const ( X ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the relationship
 */
static inline int64_t layout_relationship_get_relationship_id ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the symbol_box of the source object, the feature or the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to const geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_relationship_get_from_symbol_box_const ( const layout_relationship_t *this_ );

/*!
 *  \brief gets the symbol_box of the destination object, the feature or the classifier
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to const geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_relationship_get_to_symbol_box_const ( const layout_relationship_t *this_ );


#include "layout_relationship.inl"

#endif  /* LAYOUT_RELATIONSHIP_H */


/*
Copyright 2018-2020 Andreas Warnke

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
