/* File: data_id_pair.h; Copyright and License: see below */

#ifndef DATA_ID_PAIR_H
#define DATA_ID_PAIR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of visible object id and model object id - or feature id and parent classifier id
 *
 *  e.g. data_diagramelement_t (visible) and data_classifier_t (model)
 *  or simply data_feature_t and data_feature_t if visible object and model object are identical
 *  or data_feature_t and data_classifier_t if both are of interest
 */

#include "data_id.h"

/*!
 *  \brief the pair of visible object id and model object id, e.g. diagramelement and classifier
 */
struct data_id_pair_struct {
    data_id_t visible_id;  /*!< the visible object id */
    data_id_t model_id;  /*!< the model object id */
};

typedef struct data_id_pair_struct data_id_pair_t;

/*!
 *  \brief initializes the data_id_pair_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_pair_init_void ( data_id_pair_t *this_ );

/*!
 *  \brief re-initializes the data_id_pair_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_pair_reinit_void ( data_id_pair_t *this_ );

/*!
 *  \brief initializes the data_id_pair_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_id visible object id
 *  \param model_id model object id
 */
static inline void data_id_pair_init ( data_id_pair_t *this_, data_id_t visible_id, data_id_t model_id );

/*!
 *  \brief re-initializes the data_id_pair_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_id visible object id
 *  \param model_id model object id
 */
static inline void data_id_pair_reinit ( data_id_pair_t *this_, data_id_t visible_id, data_id_t model_id );

/*!
 *  \brief initializes the data_id_pair_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_table visible object id (talbe)
 *  \param visible_row_id visible object id (row_id)
 *  \param model_table model object id (table)
 *  \param model_row_id model object id (row_id)
 */
static inline void data_id_pair_init_by_table_and_id ( data_id_pair_t *this_,
                                                                   data_table_t visible_table,
                                                                   int64_t visible_row_id,
                                                                   data_table_t model_table,
                                                                   int64_t model_row_id
                                                                 );

/*!
 *  \brief re-initializes the data_id_pair_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_table visible object id (talbe)
 *  \param visible_row_id visible object id (row_id)
 *  \param model_table model object id (table)
 *  \param model_row_id model object id (row_id)
 */
static inline void data_id_pair_reinit_by_table_and_id ( data_id_pair_t *this_,
                                                                     data_table_t visible_table,
                                                                     int64_t visible_row_id,
                                                                     data_table_t model_table,
                                                                     int64_t model_row_id
                                                                   );

/*!
 *  \brief destroys the data_id_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_pair_destroy ( data_id_pair_t *this_ );

/*!
 *  \brief gets the visible object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the visible object
 */
static inline data_id_t data_id_pair_get_visible_id ( data_id_pair_t *this_ );

/*!
 *  \brief gets a pointer on the visible object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the visible object
 */
static inline data_id_t *data_id_pair_get_visible_id_ptr ( data_id_pair_t *this_ );

/*!
 *  \brief gets the model object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the model object
 */
static inline data_id_t data_id_pair_get_model_id ( data_id_pair_t *this_ );

/*!
 *  \brief gets a pointer on the model object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the model object
 */
static inline data_id_t *data_id_pair_get_model_id_ptr ( data_id_pair_t *this_ );

/*!
 *  \brief checks if visible id and model id are valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if both members are valid
 */
static inline bool data_id_pair_is_valid ( data_id_pair_t *this_ );

/*!
 *  \brief traces visible_id and model_id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_pair_trace ( data_id_pair_t *this_ );

#include "util/id/data_id_pair.inl"

#endif  /* DATA_ID_PAIR_H */


/*
Copyright 2017-2018 Andreas Warnke

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
