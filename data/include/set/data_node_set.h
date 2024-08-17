/* File: data_node_set.h; Copyright and License: see below */

#ifndef DATA_NODE_SET_H
#define DATA_NODE_SET_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads the direct model environment data of one model-node (classifier) from the database
 */

#include "entity/data_diagram.h"
#include "set/data_visible_classifier.h"
#include "storage/data_database_reader.h"
#include "entity/data_relationship.h"
#include "entity/data_feature.h"
#include "entity/data_id.h"
#include "entity/data_row_id.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of data_node_set_t
 */
enum data_node_set_max_enum {
    DATA_NODE_SET_MAX_FEATURES = 64,  /*!< maximum number of features linked to all visible classifiers, even invisible lifelines */
    DATA_NODE_SET_MAX_RELATIONSHIPS = 256,  /*!< maximum number of relationships linked to all visible classifiers, even these of other diagrams */
};

/*!
 *  \brief attributes of the data_node_set_t
 */
struct data_node_set_struct {
    data_classifier_t classifier;  /*!< the classifier */
    uint32_t feature_count;  /*!< number of all associated feature records */
    data_feature_t features[DATA_NODE_SET_MAX_FEATURES];  /*!< all associated feature records */
    uint32_t relationship_count;  /*!< number of all associated relationship records */
    data_relationship_t relationships[DATA_NODE_SET_MAX_RELATIONSHIPS];  /*!< all associated relationship records */
};

typedef struct data_node_set_struct data_node_set_t;

/*!
 *  \brief initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_node_set_init( data_node_set_t *this_ );

/*!
 *  \brief destroys the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_node_set_destroy( data_node_set_t *this_ );

/*!
 *  \brief fetches the painter input data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier for which the data shall be loaded
 *  \param db_reader database reader to use when acquiring data
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_node_set_load( data_node_set_t *this_, data_row_id_t classifier_id, data_database_reader_t *db_reader );

/* ================================ classifier ================================ */

/*!
 *  \brief gets the classifier within the node set
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to classifier cache
 */
static inline const data_classifier_t *data_node_set_get_classifier_const ( const data_node_set_t *this_ );

/*!
 *  \brief gets the classifier within the node set
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to classifier cache
 */
static inline data_classifier_t *data_node_set_get_classifier_ptr ( data_node_set_t *this_ );


/* ================================ features ================================ */

/*!
 *  \brief gets the number of features within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t data_node_set_get_feature_count ( const data_node_set_t *this_ );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the feature to retrieve; 0 <= index < data_node_set_get_feature_count().
 *  \return NULL if index >= data_node_set_get_feature_count(); pointer to data_feature_t otherwise.
 */
static inline const data_feature_t *data_node_set_get_feature_const ( const data_node_set_t *this_, uint32_t index );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the feature to retrieve; 0 <= index < data_node_set_get_feature_count().
 *  \return NULL if index >= data_node_set_get_feature_count(); pointer to data_feature_t otherwise.
 */
static inline data_feature_t *data_node_set_get_feature_ptr ( data_node_set_t *this_, uint32_t index );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the feature to retrieve
 *  \return NULL if row_id not in cache; pointer to data_feature_t otherwise.
 */
static inline const data_feature_t *data_node_set_get_feature_by_id_const ( const data_node_set_t *this_, data_row_id_t row_id );

/*!
 *  \brief gets a feature within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the feature to retrieve
 *  \return NULL if row_id not in cache; pointer to data_feature_t otherwise.
 */
static inline data_feature_t *data_node_set_get_feature_by_id_ptr ( data_node_set_t *this_, data_row_id_t row_id );

/*!
 *  \brief gets the list of features within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t[] otherwise.
 */
static inline data_feature_t *data_node_set_get_feature_list_ptr ( data_node_set_t *this_ );

/* ================================ relationships ================================ */

/*!
 *  \brief gets the number of relationships within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t data_node_set_get_relationship_count ( const data_node_set_t *this_ );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship to retrieve; 0 <= index < data_node_set_get_relationship_count().
 *  \return NULL if index >= data_node_set_get_relationship_count(); pointer to data_relationship_t otherwise.
 */
static inline const data_relationship_t *data_node_set_get_relationship_const ( const data_node_set_t *this_, uint32_t index );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship to retrieve; 0 <= index < data_node_set_get_relationship_count().
 *  \return NULL if index >= data_node_set_get_relationship_count(); pointer to data_relationship_t otherwise.
 */
static inline data_relationship_t *data_node_set_get_relationship_ptr ( data_node_set_t *this_, uint32_t index );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the relationship to retrieve
 *  \return NULL if row_id not in cache; pointer to data_relationship_t otherwise.
 */
static inline const data_relationship_t *data_node_set_get_relationship_by_id_const ( const data_node_set_t *this_, data_row_id_t row_id );

/*!
 *  \brief gets a relationship within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the relationship to retrieve
 *  \return NULL if row_id not in cache; pointer to data_relationship_t otherwise.
 */
static inline data_relationship_t *data_node_set_get_relationship_by_id_ptr ( data_node_set_t *this_, data_row_id_t row_id );

/* ================================ misc ================================ */

/*!
 *  \brief checks if the diagram and diagram-contents data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this_ contains valid data for sketching a diagram
 */
static inline bool data_node_set_is_valid ( const data_node_set_t *this_ );

/*!
 *  \brief invalidates the diagram data
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_node_set_invalidate ( data_node_set_t *this_ );

/*!
 *  \brief destroys all contained deatures
 *
 *  feature_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_node_set_private_destroy_features( data_node_set_t *this_ );

/*!
 *  \brief destroys all contained relationships
 *
 *  relationship_count is set to zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_node_set_private_destroy_relationships( data_node_set_t *this_ );

#include "data_node_set.inl"

#endif  /* DATA_NODE_SET_H */


/*
Copyright 2020-2024 Andreas Warnke

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
