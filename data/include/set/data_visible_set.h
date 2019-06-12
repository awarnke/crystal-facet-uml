/* File: data_visible_set.h; Copyright and License: see below */

#ifndef DATA_VISIBLE_SET_H
#define DATA_VISIBLE_SET_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads all input data for the pencil_diagram_maker from the database
 */

#include "data_diagram.h"
#include "set/data_visible_classifier.h"
#include "storage/data_database_reader.h"
#include "data_relationship.h"
#include "data_feature.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of data_visible_set_t
 */
enum data_visible_set_max_enum {
    DATA_VISIBLE_SET_MAX_CLASSIFIERS = 128,  /*!< maximum number of classifiers to be shown in one single diagram */
    DATA_VISIBLE_SET_MAX_FEATURES = 256,  /*!< maximum number of features linked to all visible classifiers, even invisible lifelines */
    DATA_VISIBLE_SET_MAX_RELATIONSHIPS = 1024,  /*!< maximum number of relationships linked to all visible classifiers, even some of other diagrams */
};

/*!
 *  \brief attributes of the data_visible_set_t
 */
struct data_visible_set_struct {
    data_diagram_t diagram;  /*!< the diagram record */
    uint32_t visible_classifier_count;  /*!< number of all contained visible classifier records */
    data_visible_classifier_t visible_classifiers[DATA_VISIBLE_SET_MAX_CLASSIFIERS];  /*!< all contained visible_classifier records */
    uint32_t feature_count;  /*!< number of all contained feature records */
    data_feature_t features[DATA_VISIBLE_SET_MAX_FEATURES];  /*!< all contained feature records */
    uint32_t relationship_count;  /*!< number of all contained relationship records */
    data_relationship_t relationships[DATA_VISIBLE_SET_MAX_RELATIONSHIPS];  /*!< all contained relationship records */

    bool containment_cache[DATA_VISIBLE_SET_MAX_CLASSIFIERS][DATA_VISIBLE_SET_MAX_CLASSIFIERS];  /*!< states if ancestor index classifier directly or indirectly contains child index classifier */
};

typedef struct data_visible_set_struct data_visible_set_t;

/*!
 *  \brief initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_visible_set_init( data_visible_set_t *this_ );

/*!
 *  \brief destroys the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_visible_set_destroy( data_visible_set_t *this_ );

/*!
 *  \brief fetches the painter input data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram for which the data shall be loaded
 *  \param db_reader database reader to use when acquiring data
 */
void data_visible_set_load( data_visible_set_t *this_, int64_t diagram_id, data_database_reader_t *db_reader );

/* ================================ diagram ================================ */

/*!
 *  \brief gets the diagram within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to current diagram cache
 */
static inline const data_diagram_t *data_visible_set_get_diagram_const ( const data_visible_set_t *this_ );

/*!
 *  \brief gets the diagram within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to current diagram cache
 */
static inline data_diagram_t *data_visible_set_get_diagram_ptr ( data_visible_set_t *this_ );

/* ================================ classifiers ================================ */

/*!
 *  \brief gets the number of visible classifiers within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t data_visible_set_get_visible_classifier_count ( const data_visible_set_t *this_ );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the visible classifier to retrieve; 0 <= index < data_visible_set_get_visible_classifier_count().
 *  \return NULL if index >= data_visible_set_get_visible_classifier_count(); pointer to data_visible_classifier_t otherwise.
 */
static inline const data_visible_classifier_t *data_visible_set_get_visible_classifier_const ( const data_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the visible classifier to retrieve; 0 <= index < data_visible_set_get_visible_classifier_count().
 *  \return NULL if index >= data_visible_set_get_visible_classifier_count(); pointer to data_visible_classifier_t otherwise.
 */
static inline data_visible_classifier_t *data_visible_set_get_visible_classifier_ptr ( data_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement for which to retrieve the visible classifier
 *  \return NULL if id not existant; pointer to data_visible_classifier_t otherwise.
 */
static inline data_visible_classifier_t *data_visible_set_get_visible_classifier_by_id_ptr ( data_visible_set_t *this_, int64_t diagramelement_id );

/*!
 *  \brief gets the classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return NULL if row_id not in cache; pointer to data_classifier_t otherwise.
 */
static inline data_classifier_t *data_visible_set_get_classifier_by_id_ptr ( data_visible_set_t *this_, int64_t row_id );

/*!
 *  \brief gets the classifier index within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return -1 if row_id not in cache; index of data_classifier_t otherwise.
 */
static inline int32_t data_visible_set_get_classifier_index ( data_visible_set_t *this_, int64_t row_id );

/*!
 *  \brief gets the classifier index within the painter input data from a pointer
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to the classifier of which to retrieve the index. must be a valid pointer to (*this_).visible_classifiers.
 *  \return index of data_classifier_t.
 */
static inline uint32_t data_visible_set_get_classifier_index_from_pointer ( const data_visible_set_t *this_,
                                                                             const data_visible_classifier_t *classifier_ptr
                                                                           );

/* ================================ features ================================ */

/*!
 *  \brief gets the number of features within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t data_visible_set_get_feature_count ( const data_visible_set_t *this_ );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the feature to retrieve; 0 <= index < data_visible_set_get_feature_count().
 *  \return NULL if index >= data_visible_set_get_feature_count(); pointer to data_feature_t otherwise.
 */
static inline const data_feature_t *data_visible_set_get_feature_const ( const data_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the feature to retrieve; 0 <= index < data_visible_set_get_feature_count().
 *  \return NULL if index >= data_visible_set_get_feature_count(); pointer to data_feature_t otherwise.
 */
static inline data_feature_t *data_visible_set_get_feature_ptr ( data_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the feature to retrieve
 *  \return NULL if row_id not in cache; pointer to data_feature_t otherwise.
 */
static inline data_feature_t *data_visible_set_get_feature_by_id_ptr ( data_visible_set_t *this_, int64_t row_id );

/*!
 *  \brief gets the list of features within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t[] otherwise.
 */
static inline data_feature_t *data_visible_set_get_feature_list_ptr ( data_visible_set_t *this_ );

/* ================================ relationships ================================ */

/*!
 *  \brief gets the number of relationships within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t data_visible_set_get_relationship_count ( const data_visible_set_t *this_ );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship to retrieve; 0 <= index < data_visible_set_get_relationship_count().
 *  \return NULL if index >= data_visible_set_get_relationship_count(); pointer to data_relationship_t otherwise.
 */
static inline const data_relationship_t *data_visible_set_get_relationship_const ( const data_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship to retrieve; 0 <= index < data_visible_set_get_relationship_count().
 *  \return NULL if index >= data_visible_set_get_relationship_count(); pointer to data_relationship_t otherwise.
 */
static inline data_relationship_t *data_visible_set_get_relationship_ptr ( data_visible_set_t *this_, uint32_t index );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the relationship to retrieve
 *  \return NULL if row_id not in cache; pointer to data_relationship_t otherwise.
 */
static inline data_relationship_t *data_visible_set_get_relationship_by_id_ptr ( data_visible_set_t *this_, int64_t row_id );

/*!
 *  \brief determines if ancestor is an ancestor of descendant
 *
 *  \param this_ pointer to own object attributes
 *  \param ancestor_index index of the ancestor classifier; 0 <= ancestor_index < data_visible_set_get_visible_classifier_count().
 *  \param descendant_index index of the descendant classifier; 0 <= descendant_index < data_visible_set_get_visible_classifier_count().
 *  \return true if there is a DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT relationship from ancestor to descendant within this data_visible_set object
 */
static inline bool data_visible_set_is_ancestor_by_index ( const data_visible_set_t *this_, uint32_t ancestor_index, uint32_t descendant_index );

/*!
 *  \brief counts the number of ancestors of a classifier denoted by index
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_index index of the classifier; 0 <= classifier_index < data_visible_set_get_visible_classifier_count().
 *  \return number of ancestors of classifier_index which are listed in this_.
 */
static inline uint32_t data_visible_set_count_ancestors_of_index ( const data_visible_set_t *this_, uint32_t classifier_index );

/*!
 *  \brief counts the number of descendants of a classifier denoted by index
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_index index of the classifier; 0 <= classifier_index < data_visible_set_get_visible_classifier_count().
 *  \return number of descendants of classifier_index which are listed in this_.
 */
static inline uint32_t data_visible_set_count_descendants_of_index ( const data_visible_set_t *this_, uint32_t classifier_index );

/* ================================ misc ================================ */

/*!
 *  \brief checks if the diagram and diagram-contents data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this_ contains valid data for sketching a diagram
 */
static inline bool data_visible_set_is_valid ( const data_visible_set_t *this_ );

/*!
 *  \brief invalidates the diagram data
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_visible_set_invalidate ( data_visible_set_t *this_ );

/*!
 *  \brief initializes or re-initializes the containment_cache
 *
 *  \param this_ pointer to own object attributes
 */
void data_visible_set_private_update_containment_cache ( data_visible_set_t *this_ );

/*!
 *  \brief destroys all contained visible classifiers
 *
 *  classifier_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_visible_set_private_destroy_visible_classifiers( data_visible_set_t *this_ );

/*!
 *  \brief destroys all contained deatures
 *
 *  feature_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_visible_set_private_destroy_features( data_visible_set_t *this_ );

/*!
 *  \brief destroys all contained relationships
 *
 *  relationship_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_visible_set_private_destroy_relationships( data_visible_set_t *this_ );

#include "data_visible_set.inl"

#endif  /* DATA_VISIBLE_SET_H */


/*
Copyright 2016-2019 Andreas Warnke

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
