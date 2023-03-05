/* File: data_profile_part.h; Copyright and License: see below */

#ifndef DATA_PROFILE_PART_H
#define DATA_PROFILE_PART_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Loads the stereotypes needed by the pencil_diagram_maker to render a data_visible_set_t from the database
 */

#include "data_diagram.h"
#include "set/data_visible_classifier.h"
#include "storage/data_database_reader.h"
#include "data_relationship.h"
#include "data_feature.h"
#include "data_id.h"
#include "data_row_id.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of data_profile_part_t
 */
enum data_profile_part_max_enum {
    DATA_PROFILE_PART_MAX_STEREOTYPES = 24,  /*!< maximum number of stereotypes to be used in one single diagram */
};

/*!
 *  \brief attributes of the data_profile_part_t
 */
struct data_profile_part_struct {
    uint32_t visible_classifier_count;  /*!< number of all contained visible classifier records */
    data_visible_classifier_t visible_classifiers[DATA_PROFILE_PART_MAX_STEREOTYPES];  /*!< all contained visible_classifier records */
};

typedef struct data_profile_part_struct data_profile_part_t;

/*!
 *  \brief initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_profile_part_init( data_profile_part_t *this_ );

/*!
 *  \brief re-initializes the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_profile_part_reinit( data_profile_part_t *this_ );

/*!
 *  \brief destroys the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
void data_profile_part_destroy( data_profile_part_t *this_ );

/*!
 *  \brief fetches the painter input data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram for which the data shall be loaded
 *  \param db_reader database reader to use when acquiring data
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_profile_part_load( data_profile_part_t *this_, data_row_id_t diagram_id, data_database_reader_t *db_reader );

/* ================================ classifiers ================================ */

/*!
 *  \brief gets the number of visible classifiers within the painter input data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t data_profile_part_get_visible_classifier_count ( const data_profile_part_t *this_ );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the visible classifier to retrieve; 0 &lt;= index &lt; data_profile_part_get_visible_classifier_count().
 *  \return NULL if index &gt;= data_profile_part_get_visible_classifier_count(); pointer to data_visible_classifier_t otherwise.
 */
static inline const data_visible_classifier_t *data_profile_part_get_visible_classifier_const ( const data_profile_part_t *this_, uint32_t index );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the visible classifier to retrieve; 0 &lt;= index &lt; data_profile_part_get_visible_classifier_count().
 *  \return NULL if index &gt;= data_profile_part_get_visible_classifier_count(); pointer to data_visible_classifier_t otherwise.
 */
static inline data_visible_classifier_t *data_profile_part_get_visible_classifier_ptr ( data_profile_part_t *this_, uint32_t index );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement for which to retrieve the visible classifier
 *  \return NULL if id not existant; pointer to data_visible_classifier_t otherwise.
 */
static inline const data_visible_classifier_t *data_profile_part_get_visible_classifier_by_id_const ( const data_profile_part_t *this_, data_row_id_t diagramelement_id );

/*!
 *  \brief gets a visible classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement for which to retrieve the visible classifier
 *  \return NULL if id not existant; pointer to data_visible_classifier_t otherwise.
 */
static inline data_visible_classifier_t *data_profile_part_get_visible_classifier_by_id_ptr ( data_profile_part_t *this_, data_row_id_t diagramelement_id );

/*!
 *  \brief gets the classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return NULL if row_id not in cache; pointer to data_classifier_t otherwise.
 */
static inline const data_classifier_t *data_profile_part_get_classifier_by_id_const ( const data_profile_part_t *this_, data_row_id_t row_id );

/*!
 *  \brief gets the classifier within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return NULL if row_id not in cache; pointer to data_classifier_t otherwise.
 */
static inline data_classifier_t *data_profile_part_get_classifier_by_id_ptr ( data_profile_part_t *this_, data_row_id_t row_id );

/*!
 *  \brief gets the classifier index within the painter input data
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id of the classifier to retrieve
 *  \return -1 if row_id not in cache; index of data_classifier_t otherwise.
 */
static inline int32_t data_profile_part_get_classifier_index ( const data_profile_part_t *this_, data_row_id_t row_id );

/*!
 *  \brief gets the classifier index within the painter input data from a pointer
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to the classifier of which to retrieve the index. must be a valid pointer to (*this_).visible_classifiers.
 *  \return index of data_classifier_t.
 */
static inline uint32_t data_profile_part_get_classifier_index_from_pointer ( const data_profile_part_t *this_,
                                                                            const data_visible_classifier_t *classifier_ptr
                                                                          );

/*!
 *  \brief appends a visible_classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param new_classifier pointer to visible_classifier record that shall be copied
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_ARRAY_BUFFER_EXCEEDED if array is full
 */
static inline u8_error_t data_profile_part_append_classifier( data_profile_part_t *this_, const data_visible_classifier_t *new_classifier );

#include "data_profile_part.inl"

#endif  /* DATA_PROFILE_PART_H */


/*
Copyright 2023-2023 Andreas Warnke

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
