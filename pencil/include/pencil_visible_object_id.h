/* File: pencil_visible_object_id.h; Copyright and License: see below */

#ifndef PENCIL_VISIBLE_OBJECT_ID_H
#define PENCIL_VISIBLE_OBJECT_ID_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of visible object id and model object id
 *
 *  e.g. data_diagramelement_t and data_classifier_t
 *  or simply data_feature_t and data_feature_t if visible object and model object are identical
 */

#include "data_id.h"

/*!
 *  \brief the pair of visible object id and model object id, e.g. diagramelement and classifier
 */
struct pencil_visible_object_id_struct {
    data_id_t visible_id;  /*!< the visible object id */
    data_id_t model_id;  /*!< the model object id */
};

typedef struct pencil_visible_object_id_struct pencil_visible_object_id_t;

/*!
 *  \brief initializes the pencil_visible_object_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_visible_object_id_init_void ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief re-initializes the pencil_visible_object_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_visible_object_id_reinit_void ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief initializes the pencil_visible_object_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_id visible object id
 *  \param object_id model object id
 */
static inline void pencil_visible_object_id_init ( pencil_visible_object_id_t *this_, data_id_t visible_id, data_id_t model_id );

/*!
 *  \brief re-initializes the pencil_visible_object_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_id visible object id
 *  \param object_id model object id
 */
static inline void pencil_visible_object_id_reinit ( pencil_visible_object_id_t *this_, data_id_t visible_id, data_id_t model_id );

/*!
 *  \brief initializes the pencil_visible_object_id_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_table visible object id (talbe)
 *  \param visible_row_id visible object id (row_id)
 *  \param model_table model object id (table)
 *  \param model_row_id model object id (row_id)
 */
static inline void pencil_visible_object_id_init_by_table_and_id ( pencil_visible_object_id_t *this_,
                                                                   data_table_t visible_table,
                                                                   int64_t visible_row_id,
                                                                   data_table_t model_table,
                                                                   int64_t model_row_id
                                                                 );

/*!
 *  \brief re-initializes the pencil_visible_object_id_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_table visible object id (talbe)
 *  \param visible_row_id visible object id (row_id)
 *  \param model_table model object id (table)
 *  \param model_row_id model object id (row_id)
 */
static inline void pencil_visible_object_id_reinit_by_table_and_id ( pencil_visible_object_id_t *this_,
                                                                     data_table_t visible_table,
                                                                     int64_t visible_row_id,
                                                                     data_table_t model_table,
                                                                     int64_t model_row_id
                                                                   );

/*!
 *  \brief destroys the pencil_visible_object_id_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_visible_object_id_destroy ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief gets the visible object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the visible object
 */
static inline data_id_t pencil_visible_object_id_get_visible_id ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief gets a pointer on the visible object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the visible object
 */
static inline data_id_t *pencil_visible_object_id_get_visible_id_ptr ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief gets the model object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the model object
 */
static inline data_id_t pencil_visible_object_id_get_model_id ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief gets a pointer on the model object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the model object
 */
static inline data_id_t *pencil_visible_object_id_get_model_id_ptr ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief checks if visible id and model id are valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if both members are valid
 */
static inline bool pencil_visible_object_id_is_valid ( pencil_visible_object_id_t *this_ );

/*!
 *  \brief traces visible_id and model_id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_visible_object_id_trace ( pencil_visible_object_id_t *this_ );

#include "pencil_visible_object_id.inl"

#endif  /* PENCIL_VISIBLE_OBJECT_ID_H */


/*
Copyright 2017-2017 Andreas Warnke

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
