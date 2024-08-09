/* File: layout_visible_set.h; Copyright and License: see below */

#ifndef LAYOUT_VISIBLE_SET_H
#define LAYOUT_VISIBLE_SET_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the layout data for input data.
 */

#include "geometry/geometry_connector.h"
#include "geometry/geometry_rectangle.h"
#include "layout/layout_diagram.h"
#include "layout/layout_feature.h"
#include "layout/layout_relationship.h"
#include "layout/layout_visible_classifier.h"
#include "filter/pencil_visibility.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "data_rules.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of layout_visible_set_t
 *
 *  While the data_visible_set_t contains features and relationships just once,
 *  layout_visible_set_t contains these once per instance of every visible_classifier_t.
 */
enum layout_visible_set_max_enum {
    LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS = DATA_VISIBLE_SET_MAX_CLASSIFIERS,  /*!< maximum number of classifiers to be shown in one single diagram */
    LAYOUT_VISIBLE_SET_MAX_FEATURES = DATA_VISIBLE_SET_MAX_FEATURES + 32,  /*!< maximum number of features to be shown in one single diagram */
    LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS = DATA_VISIBLE_SET_MAX_RELATIONSHIPS * 2,  /*!< maximum number of relationships to be shown in one single diagram */
};

/*!
 *  \brief attributes of the layout_visible_set_t
 *
 *  layout_visible_set_t contains a consistent subset of the database model:
 *  If a feature is contained, its classifier is contained.
 *  If a relationship is contained, the classifiers of both ends are contained.
 *
 *  In case the data exceeds the limits of the member arrays, some data cannot be shown.
 *  There is no guarantee for completeness.
 */
struct layout_visible_set_struct {
    /* diagram layout*/
    layout_diagram_t diagram_layout;  /*!< layout data of the diagram */
    bool diagram_valid;  /*!< true if diagram_layout is initialized */

    /* classifier layout*/
    layout_visible_classifier_t visible_classifier_layout[LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS];  /*!< layout data of visible classifiers */
    uint32_t visible_classifier_count;  /*!< number of all layouted visible classifier records */

    /* feature layout */
    layout_feature_t feature_layout[LAYOUT_VISIBLE_SET_MAX_FEATURES];  /*!< layout data of features */
    uint32_t feature_count;  /*!< number of all layouted feature records */

    /* relationship layout */
    layout_relationship_t relationship_layout[LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS];  /*!< layout data of relationships */
    uint32_t relationship_count;  /*!< number of all layouted relationship records */

    /* input data */
    const data_visible_set_t *input_data;  /*!< the input data which is base for the layout data */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
};

typedef struct layout_visible_set_struct layout_visible_set_t;

/*!
 *  \brief initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 */
void layout_visible_set_init( layout_visible_set_t *this_, const data_visible_set_t *input_data );

/*!
 *  \brief re-initializes the painter input data layout
 *
 *  reinit shall be called when the input_data set has changed
 *  to ensure that the layout data reflects the input data
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 */
static inline void layout_visible_set_reinit( layout_visible_set_t *this_, const data_visible_set_t *input_data );

/*!
 *  \brief destroys the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void layout_visible_set_destroy( layout_visible_set_t *this_ );

/*!
 *  \brief re-initializes the painter input data layout
 *
 *  resync shall be called when any element in the input_data set has changed
 *  (but not the set itself)
 *  to ensure that the layout data reflects the input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_visible_set_resync( layout_visible_set_t *this_ );

/*!
 *  \brief checks if this set contains valid layout data for a diagram
 *
 *  This function checks if the data was initialized - it does not perform a consistency check.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this object contains valid layout data
 */
static inline bool layout_visible_set_is_valid( const layout_visible_set_t *this_ );

/* ================================ diagram ================================ */

/*!
 *  \brief gets the layouted diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to layout_diagram_t.
 */
static inline layout_diagram_t *layout_visible_set_get_diagram_ptr ( layout_visible_set_t *this_ );

/*!
 *  \brief gets the layouted diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to layout_diagram_t.
 */
static inline const layout_diagram_t *layout_visible_set_get_diagram_const ( const layout_visible_set_t *this_ );

/* ================================ classifiers ================================ */

/*!
 *  \brief gets the number of visible classifiers within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t layout_visible_set_get_visible_classifier_count ( const layout_visible_set_t *this_ );

/*!
 *  \brief gets the layouted visible_classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted classifier to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS.
 *  \return pointer to layout_visible_classifier_t.
 */
static inline layout_visible_classifier_t *layout_visible_set_get_visible_classifier_ptr ( layout_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets the layouted visible_classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted classifier to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS.
 *  \return pointer to layout_visible_classifier_t.
 */
static inline const layout_visible_classifier_t *layout_visible_set_get_visible_classifier_const ( const layout_visible_set_t *this_, uint32_t index );

/* ================================ features ================================ */

/*!
 *  \brief gets the number of features within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t layout_visible_set_get_feature_count ( const layout_visible_set_t *this_ );

/*!
 *  \brief gets the layouted feature
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted feature to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_FEATURES.
 *  \return pointer to layout_feature_t.
 */
static inline layout_feature_t *layout_visible_set_get_feature_ptr ( layout_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets the layouted feature
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted feature to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_FEATURES.
 *  \return pointer to layout_feature_t.
 */
static inline const layout_feature_t *layout_visible_set_get_feature_const ( const layout_visible_set_t *this_, uint32_t index );

/* ================================ relationships ================================ */

/*!
 *  \brief gets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS.
 *  \return true if visible.
 */
static inline pencil_visibility_t layout_visible_set_get_relationship_visibility ( const layout_visible_set_t *this_, uint32_t index );

/*!
 *  \brief sets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to set; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS.
 *  \param visible true if visible.
 */
static inline void layout_visible_set_set_relationship_visibility ( layout_visible_set_t *this_, uint32_t index, pencil_visibility_t visible );

/*!
 *  \brief gets the number of relationships within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t layout_visible_set_get_relationship_count ( const layout_visible_set_t *this_ );

/*!
 *  \brief gets the layouted relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted relationship to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS.
 *  \return pointer to layout_relationship_t.
 */
static inline layout_relationship_t *layout_visible_set_get_relationship_ptr ( layout_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets the layouted relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the layouted relationship to retrieve; 0 &lt;= index &lt; LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS.
 *  \return pointer to layout_relationship_t.
 */
static inline const layout_relationship_t *layout_visible_set_get_relationship_const ( const layout_visible_set_t *this_, uint32_t index );

/*!
 *  \brief determines if ancestor is an ancestor of descendant
 *
 *  \param this_ pointer to own object attributes
 *  \param ancestor the ancestor classifier
 *  \param descendant the descendant classifier
 *  \return true if there is a DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT relationship from ancestor to descendant within the data_visible_set object
 */
static inline bool layout_visible_set_is_ancestor ( const layout_visible_set_t *this_,
                                                    const layout_visible_classifier_t *ancestor,
                                                    const layout_visible_classifier_t *descendant
                                                  );

/*!
 *  \brief counts the number of ancestors of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier the classifier of which to count ancestors
 *  \return number of ancestors of classifier which are listed in this_.
 */
static inline uint32_t layout_visible_set_count_ancestors ( const layout_visible_set_t *this_,
                                                            const layout_visible_classifier_t *classifier
                                                          );

/*!
 *  \brief counts the number of descendants of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier the classifier of which to count decendants
 *  \return number of descendants of classifier which are listed in this_.
 */
static inline uint32_t layout_visible_set_count_descendants ( const layout_visible_set_t *this_,
                                                              const layout_visible_classifier_t *classifier
                                                            );

/* ================================ misc ================================ */

/*!
 *  \brief checks if the layout_visible_set is initialized and consistent
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this_ contains valid data for sketching a diagram
 */
bool layout_visible_set_is_consistent ( const layout_visible_set_t *this_ );

/*!
 *  \brief gets statistics on layout_visible_set.
 *
 *  This encompasses number of objects, number of overlaps (warnings), number of objects outside diagram (errors).
 *
 *  \param this_ pointer to own object attributes
 *  \param io_layout_stat pointer to already initialized statistics object where layouting statistics are added
 */
void layout_visible_set_get_statistics ( const layout_visible_set_t *this_, data_stat_t *io_layout_stat );

/*!
 *  \brief analyzes layout_visible_set: gets statistics and calls back a function in case of overlaps
 *
 *  \param this_ pointer to own object attributes
 *  \param io_layout_stat pointer to already initialized statistics object where layouting statistics are added
 *  \param overlap_callback pointer to a function that is called in case of overlaps
 *  \param data pointer that is passed to overlap_callback
 */
void layout_visible_set_analyze ( const layout_visible_set_t *this_,
                                  data_stat_t *io_layout_stat,
                                  void (*overlap_callback)(void *data, const geometry_rectangle_t *a, const geometry_rectangle_t *b),
                                  void *data
                                );

/*!
 *  \brief analyzes layout_visible_set: gets statistics and calls back a function in case of overlaps
 *
 *  \param data pointer that is passed to overlap_callback
 *  \param rect_a pointer to a rectangle that overlaps with rect_b
 *  \param rect_b pointer to a rectangle that overlaps with rect_a
 */
void layout_visible_set_private_analyze_nothing_callback ( void *data,
                                                           const geometry_rectangle_t *rect_a,
                                                           const geometry_rectangle_t *rect_b
                                                         );

/*!
 *  \brief initializes the diagram_layout member
 *
 *  \param this_ pointer to own object attributes, input_data shall be already initialized
 */
void layout_visible_set_private_init_diagram( layout_visible_set_t *this_ );

/*!
 *  \brief initializes the visible_classifier_layout array
 *
 *  \param this_ pointer to own object attributes, input_data shall be already initialized
 */
void layout_visible_set_private_init_classifiers( layout_visible_set_t *this_ );

/*!
 *  \brief initializes the feature_layout array
 *
 *  \param this_ pointer to own object attributes, input_data and visible_classifier_layout shall be already initialized
 */
void layout_visible_set_private_init_features( layout_visible_set_t *this_ );

/*!
 *  \brief initializes the relationship_layout array
 *
 *  \param this_ pointer to own object attributes, input_data and visible_classifier_layout and feature_layout shall be already initialized
 */
void layout_visible_set_private_init_relationships( layout_visible_set_t *this_ );

/*!
 *  \brief initializes all layout relationships for one data relationship
 *
 *  \param this_ pointer to own object attributes, input_data and visible_classifier_layout and feature_layout shall be already initialized
 *  \param relationship_data pointer into the data set structure (*this_).input_data, pointing to the data relationship struct
 *  \param io_dropped_relationships io parameter, counter that is increased for every non-instantiated layout relationship
 *  \return number of created layout relationships
 */
uint32_t layout_visible_set_private_init_relationship( layout_visible_set_t *this_,
                                                       const data_relationship_t *relationship_data,
                                                       uint32_t *io_dropped_relationships
                                                     );

#include "layout_visible_set.inl"

#endif  /* LAYOUT_VISIBLE_SET_H */


/*
Copyright 2017-2024 Andreas Warnke

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
