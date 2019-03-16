/* File: pencil_layout_data.h; Copyright and License: see below */

#ifndef PENCIL_LAYOUT_DATA_H
#define PENCIL_LAYOUT_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the layout data for input data.
 */

#include "util/geometry/geometry_connector.h"
#include "util/geometry/geometry_rectangle.h"
#include "layout/layout_diagram.h"
#include "layout/layout_feature.h"
#include "layout/layout_relationship.h"
#include "layout/layout_visible_classifier.h"
#include "set/data_visible_set.h"
#include "option/pencil_visibility.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef NDEBUG
/* make the layout visible for debugging: */
#define PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
#endif

/*!
 *  \brief constants for maximum values of pencil_layout_data_t
 */
enum pencil_layout_data_max_enum {
    PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS = PENCIL_INPUT_DATA_MAX_CLASSIFIERS,  /*!< maximum number of classifiers to be shown in one single diagram */
    PENCIL_LAYOUT_DATA_MAX_FEATURES = PENCIL_INPUT_DATA_MAX_FEATURES,  /*!< maximum number of features to be shown in one single diagram */
    PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS = PENCIL_INPUT_DATA_MAX_RELATIONSHIPS,  /*!< maximum number of relationships to be shown in one single diagram */
};

/*!
 *  \brief attributes of the pencil_layout_data_t
 */
struct pencil_layout_data_struct {
    /* diagram layout*/
    layout_diagram_t diagram_layout;  /*!< layout data of the diagram */
    bool diagram_valid;  /*!< true if diagram_layout is initialized */

    /* classifier layout*/
    layout_visible_classifier_t visible_classifier_layout[PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS];  /*!< layout data of visible classifiers */
    uint32_t visible_classifier_count;  /*!< number of all layouted visible classifier records */

    /* feature layout */
    layout_feature_t feature_layout[PENCIL_LAYOUT_DATA_MAX_FEATURES];  /*!< layout data of features */
    uint32_t feature_count;  /*!< number of all layouted feature records */

    /* relationship layout */
    layout_relationship_t relationship_layout[PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS];  /*!< layout data of relationships */
    uint32_t relationship_count;  /*!< number of all layouted relationship records */

    /* input data */
    const data_visible_set_t *input_data;  /*!< the input data which is base for the layout data */
};

typedef struct pencil_layout_data_struct pencil_layout_data_t;

/*!
 *  \brief initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_layout_data_init( pencil_layout_data_t *this_ );

/*!
 *  \brief re-initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 */
void pencil_layout_data_reinit( pencil_layout_data_t *this_, data_visible_set_t *input_data );

/*!
 *  \brief destroys the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_layout_data_destroy( pencil_layout_data_t *this_ );

/* ================================ diagram ================================ */

/*!
 *  \brief gets the layouted diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to layout_diagram_t.
 */
static inline layout_diagram_t *pencil_layout_data_get_diagram_ptr ( pencil_layout_data_t *this_ );

/* ================================ classifiers ================================ */

/*!
 *  \brief gets the number of visible classifiers within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_layout_data_get_classifier_count ( pencil_layout_data_t *this_ );

/*!
 *  \brief gets the layouted visible_classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted classifier to retrieve; 0 <= index < PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS.
 *  \return pointer to layout_visible_classifier_t.
 */
static inline layout_visible_classifier_t *pencil_layout_data_get_classifier_ptr ( pencil_layout_data_t *this_, uint32_t index );

/* ================================ features ================================ */

/*!
 *  \brief gets the number of features within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_layout_data_get_feature_count ( pencil_layout_data_t *this_ );

/*!
 *  \brief gets the layouted feature
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted feature to retrieve; 0 <= index < PENCIL_LAYOUT_DATA_MAX_FEATURES.
 *  \return pointer to layout_feature_t.
 */
static inline layout_feature_t *pencil_layout_data_get_feature_ptr ( pencil_layout_data_t *this_, uint32_t index );

/* ================================ relationships ================================ */

/*!
 *  \brief gets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to retrieve; 0 <= index < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS.
 *  \return true if visible.
 */
static inline pencil_visibility_t pencil_layout_data_get_relationship_visibility ( pencil_layout_data_t *this_, uint32_t index );

/*!
 *  \brief sets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to set; 0 <= index < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS.
 *  \param visible true if visible.
 */
static inline void pencil_layout_data_set_relationship_visibility ( pencil_layout_data_t *this_, uint32_t index, pencil_visibility_t visible );

/*!
 *  \brief gets the number of relationships within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_layout_data_get_relationship_count ( pencil_layout_data_t *this_ );

/*!
 *  \brief gets the layouted relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted relationship to retrieve; 0 <= index < PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS.
 *  \return pointer to layout_relationship_t.
 */
static inline layout_relationship_t *pencil_layout_data_get_relationship_ptr ( pencil_layout_data_t *this_, uint32_t index );

/*!
 *  \brief determines if ancestor is an ancestor of descendant
 *
 *  \param this_ pointer to own object attributes
 *  \param ancestor the ancestor classifier
 *  \param descendant the descendant classifier
 *  \return true if there is a DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT relationship from ancestor to descendant within the data_visible_set object
 */
static inline bool pencil_layout_data_is_ancestor ( pencil_layout_data_t *this_, layout_visible_classifier_t *ancestor, layout_visible_classifier_t *descendant );

/*!
 *  \brief counts the number of ancestors of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier the classifier of which to count ancestors
 *  \return number of ancestors of classifier which are listed in this_.
 */
static inline uint32_t pencil_layout_data_count_ancestors ( pencil_layout_data_t *this_, layout_visible_classifier_t *classifier );

/*!
 *  \brief counts the number of descendants of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier the classifier of which to count decendants
 *  \return number of descendants of classifier which are listed in this_.
 */
static inline uint32_t pencil_layout_data_count_descendants ( pencil_layout_data_t *this_, layout_visible_classifier_t *classifier );

#include "pencil_layout_data.inl"

#endif  /* PENCIL_LAYOUT_DATA_H */


/*
Copyright 2017-2019 Andreas Warnke

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
