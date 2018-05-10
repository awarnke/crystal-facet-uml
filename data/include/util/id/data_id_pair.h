/* File: data_id_pair.h; Copyright and License: see below */

#ifndef DATA_ID_PAIR_H
#define DATA_ID_PAIR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of visible object id and secondary object id - or feature id and parent classifier id
 *
 *  The following type combinations are allowed:
 *  DATA_TABLE_VOID (primary) and DATA_TABLE_VOID (secondary);
 *  DATA_TABLE_DIAGRAMELEMENT (primary) and DATA_TABLE_CLASSIFIER (secondary);
 *  DATA_TABLE_FEATURE (primary) and DATA_TABLE_CLASSIFIER (secondary);
 *  DATA_TABLE_RELATIONSHIP (primary) and DATA_TABLE_VOID (secondary);
 *  DATA_TABLE_DIAGRAM (primary) and DATA_TABLE_VOID (secondary);
 *
 *  The primary_id is never of type DATA_TABLE_CLASSIFIER.
 */

#include "data_id.h"

/*!
 *  \brief the pair of visible object id and secondary object id, e.g. diagramelement and classifier of feature and classifier
 */
struct data_id_pair_struct {
    data_id_t primary_id;  /*!< the primary object id */
    data_id_t secondary_id;  /*!< the classifier object id */
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
 *  \param primary_id primary object id
 *  \param secondary_id secondary object id
 */
static inline void data_id_pair_init ( data_id_pair_t *this_, data_id_t primary_id, data_id_t secondary_id );

/*!
 *  \brief re-initializes the data_id_pair_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 *  \param secondary_id secondary object id
 */
static inline void data_id_pair_reinit ( data_id_pair_t *this_, data_id_t primary_id, data_id_t secondary_id );

/*!
 *  \brief initializes the data_id_pair_t struct, the secondary_id is initialized to VOID.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 */
static inline void data_id_pair_init_solo ( data_id_pair_t *this_, data_id_t primary_id );

/*!
 *  \brief re-initializes the data_id_pair_t struct, the secondary_id is re-initialized to VOID.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 */
static inline void data_id_pair_reinit_solo ( data_id_pair_t *this_, data_id_t primary_id );

/*!
 *  \brief initializes the data_id_pair_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_table primary object id (talbe)
 *  \param primary_row_id primary object id (row_id)
 *  \param secondary_table secondary object id (table)
 *  \param secondary_row_id secondary object id (row_id)
 */
static inline void data_id_pair_init_by_table_and_id ( data_id_pair_t *this_,
                                                       data_table_t primary_table,
                                                       int64_t primary_row_id,
                                                       data_table_t secondary_table,
                                                       int64_t secondary_row_id
                                                     );

/*!
 *  \brief re-initializes the data_id_pair_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_table primary object id (talbe)
 *  \param primary_row_id primary object id (row_id)
 *  \param secondary_table secondary object id (table)
 *  \param secondary_row_id secondary object id (row_id)
 */
static inline void data_id_pair_reinit_by_table_and_id ( data_id_pair_t *this_,
                                                         data_table_t primary_table,
                                                         int64_t primary_row_id,
                                                         data_table_t secondary_table,
                                                         int64_t secondary_row_id
                                                       );

/*!
 *  \brief destroys the data_id_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_pair_destroy ( data_id_pair_t *this_ );

/*!
 *  \brief gets the primary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the primary object
 */
static inline data_id_t data_id_pair_get_primary_id ( const data_id_pair_t *this_ );

/*!
 *  \brief gets a pointer on the primary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the primary object
 */
static inline data_id_t *data_id_pair_get_primary_id_ptr ( data_id_pair_t *this_ );

/*!
 *  \brief gets the secondary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the secondary object
 */
static inline data_id_t data_id_pair_get_secondary_id ( const data_id_pair_t *this_ );

/*!
 *  \brief gets a pointer on the secondary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the secondary object
 */
static inline data_id_t *data_id_pair_get_secondary_id_ptr ( data_id_pair_t *this_ );

/*!
 *  \brief checks if primary id and secondary id are valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if both members are valid
 */
static inline bool data_id_pair_is_valid ( const data_id_pair_t *this_ );

/*!
 *  \brief traces primary_id and secondary_id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_pair_trace ( const data_id_pair_t *this_ );

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
