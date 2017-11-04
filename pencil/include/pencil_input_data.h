/* File: pencil_input_data.h; Copyright and License: see below */

#ifndef PENCIL_INPUT_DATA_H
#define PENCIL_INPUT_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads all input data for the pencil_diagram_maker from the database
 */

#include "data_diagram.h"
#include "data_visible_classifier.h"
#include "storage/data_database_reader.h"
#include "data_relationship.h"
#include "data_feature.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of pencil_input_data_t
 */
enum pencil_input_data_max_enum {
    PENCIL_INPUT_DATA_MAX_CLASSIFIERS = 128,  /*!< maximum number of classifiers to be shown in one single diagram */
    PENCIL_INPUT_DATA_MAX_FEATURES = 256,  /*!< maximum number of features to be shown in one single diagram */
    PENCIL_INPUT_DATA_MAX_RELATIONSHIPS = 256,  /*!< maximum number of relationships to be shown in one single diagram */
};

/*!
 *  \brief attributes of the pencil_input_data_t
 */
struct pencil_input_data_struct {
    data_diagram_t diagram;  /*!< the diagram record */
    uint32_t visible_classifier_count;  /*!< number of all contained visible classifier records */
    data_visible_classifier_t visible_classifiers[PENCIL_INPUT_DATA_MAX_CLASSIFIERS];  /*!< all contained visible_classifier records */
    uint32_t feature_count;  /*!< number of all contained feature records */
    data_feature_t features[PENCIL_INPUT_DATA_MAX_FEATURES];  /*!< all contained feature records */
    uint32_t relationship_count;  /*!< number of all contained relationship records */
    data_relationship_t relationships[PENCIL_INPUT_DATA_MAX_RELATIONSHIPS];  /*!< all contained relationship records */
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
 *  \brief gets the diagram within the painter input data
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
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the visible classifier to retrieve; 0 <= index < pencil_input_data_get_visible_classifier_count().
 *  \return NULL if index >= pencil_input_data_get_visible_classifier_count(); pointer to data_visible_classifier_t otherwise.
 */
static inline data_visible_classifier_t *pencil_input_data_get_visible_classifier_ptr ( pencil_input_data_t *this_, uint32_t index );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement fpr which to retrieve the visible classifier
 *  \return NULL if id not existant; pointer to data_visible_classifier_t otherwise.
 */
data_visible_classifier_t *pencil_input_data_get_visible_classifier_by_id_ptr ( pencil_input_data_t *this_, int64_t diagramelement_id );

/*!
 *  \brief gets the classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return NULL if row_id not in cache; pointer to data_classifier_t otherwise.
 */
static inline data_classifier_t *pencil_input_data_get_classifier_ptr ( pencil_input_data_t *this_, int64_t row_id );

/*!
 *  \brief gets the classifier index within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return -1 if row_id not in cache; index of data_classifier_t otherwise.
 */
static inline int32_t pencil_input_data_get_classifier_index ( pencil_input_data_t *this_, int64_t row_id );

/*!
 *  \brief gets the number of features within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_input_data_get_feature_count ( pencil_input_data_t *this_ );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the feature to retrieve; 0 <= index < pencil_input_data_get_feature_count().
 *  \return NULL if index >= pencil_input_data_get_feature_count(); pointer to data_feature_t otherwise.
 */
static inline data_feature_t *pencil_input_data_get_feature_ptr ( pencil_input_data_t *this_, uint32_t index );

/*!
 *  \brief gets the list of features within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t[] otherwise.
 */
static inline data_feature_t *pencil_input_data_get_feature_list_ptr ( pencil_input_data_t *this_ );

/*!
 *  \brief gets the number of relationships within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_input_data_get_relationship_count ( pencil_input_data_t *this_ );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship to retrieve; 0 <= index < pencil_input_data_get_relationship_count().
 *  \return NULL if index >= pencil_input_data_get_relationship_count(); pointer to data_relationship_t otherwise.
 */
static inline data_relationship_t *pencil_input_data_get_relationship_ptr ( pencil_input_data_t *this_, uint32_t index );

/*!
 *  \brief determines if parent is a direct parent of child
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_index index of the parent classifier; 0 <= parent_index < pencil_input_data_get_visible_classifier_count().
 *  \param child_index index of the child classifier; 0 <= child_index < pencil_input_data_get_visible_classifier_count().
 *  \return true if there is a DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT relationship from parent to child within this pencil_input_data object
 */
static inline bool pencil_input_data_is_parent_by_index ( pencil_input_data_t *this_, uint32_t parent_index, uint32_t child_index );

/*!
 *  \brief determines if parent is a direct parent of child
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent classifier
 *  \param child_id id of the child classifier
 *  \return true if there is a DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT relationship from parent to child within this pencil_input_data object
 */
bool pencil_input_data_is_parent ( pencil_input_data_t *this_, int64_t parent_id, int64_t child_id );

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

/*!
 *  \brief destroys all contained deatures
 *
 *  feature_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_private_destroy_features( pencil_input_data_t *this_ );

/*!
 *  \brief destroys all contained relationships
 *
 *  relationship_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_input_data_private_destroy_relationships( pencil_input_data_t *this_ );

#include "pencil_input_data.inl"

#endif  /* PENCIL_INPUT_DATA_H */


/*
Copyright 2016-2017 Andreas Warnke

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
