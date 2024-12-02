/* File: data_full_id.h; Copyright and License: see below */

#ifndef DATA_FULL_ID_H
#define DATA_FULL_ID_H

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
 *  DATA_TABLE_CLASSIFIER (primary) and DATA_TABLE_VOID (secondary);
 *
 *  Note that some combinations may refer to multiple layout artifacts:
 *  E.g. a feature or classifier can occur several times within one diagram,
 *  a relationship also may occur multiple times if the source or destination exists multiple times.
 */

#include "entity/data_id.h"
#include "entity/data_row.h"

/*!
 *  \brief the pair of visible object id and secondary object id, e.g. diagramelement and classifier or feature and classifier
 */
struct data_full_id_struct {
    data_id_t primary_id;  /*!< the primary object id */
    data_id_t secondary_id;  /*!< the classifier parent object id (if applicable) */
};

typedef struct data_full_id_struct data_full_id_t;

#define DATA_FULL_ID_VOID ((data_full_id_t){.primary_id=DATA_ID_VOID,.secondary_id=DATA_ID_VOID})
#define DATA_FULL_ID_SOLO(prim_tab,prim_row) ((data_full_id_t)\
{.primary_id=DATA_ID(prim_tab,prim_row),.secondary_id=DATA_ID_VOID})
#define DATA_FULL_ID(prim_tab,prim_row,classifier_row) ((data_full_id_t)\
{.primary_id=DATA_ID(prim_tab,prim_row),.secondary_id=DATA_ID(DATA_TABLE_CLASSIFIER,classifier_row)})

/*!
 *  \brief initializes the data_full_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_full_id_init_void ( data_full_id_t *this_ );

/*!
 *  \brief re-initializes the data_full_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_full_id_reinit_void ( data_full_id_t *this_ );

/*!
 *  \brief initializes the data_full_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 *  \param secondary_id secondary object id
 */
static inline void data_full_id_init ( data_full_id_t *this_, const data_id_t *primary_id, const data_id_t *secondary_id );

/*!
 *  \brief re-initializes the data_full_id_t struct.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 *  \param secondary_id secondary object id
 */
static inline void data_full_id_reinit ( data_full_id_t *this_, const data_id_t *primary_id, const data_id_t *secondary_id );

/*!
 *  \brief initializes the data_full_id_t struct, the secondary_id is initialized to VOID.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 */
static inline void data_full_id_init_solo ( data_full_id_t *this_, const data_id_t *primary_id );

/*!
 *  \brief re-initializes the data_full_id_t struct, the secondary_id is re-initialized to VOID.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_id primary object id
 */
static inline void data_full_id_reinit_solo ( data_full_id_t *this_, const data_id_t *primary_id );

/*!
 *  \brief initializes the data_full_id_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_table primary object id (talbe)
 *  \param primary_row_id primary object id (row_id)
 *  \param secondary_table secondary object id (table)
 *  \param secondary_row_id secondary object id (row_id)
 */
static inline void data_full_id_init_by_table_and_id ( data_full_id_t *this_,
                                                       data_table_t primary_table,
                                                       data_row_t primary_row_id,
                                                       data_table_t secondary_table,
                                                       data_row_t secondary_row_id
                                                     );

/*!
 *  \brief re-initializes the data_full_id_t struct by table and row ids.
 *
 *  \param this_ pointer to own object attributes
 *  \param primary_table primary object id (talbe)
 *  \param primary_row_id primary object id (row_id)
 *  \param secondary_table secondary object id (table)
 *  \param secondary_row_id secondary object id (row_id)
 */
static inline void data_full_id_reinit_by_table_and_id ( data_full_id_t *this_,
                                                         data_table_t primary_table,
                                                         data_row_t primary_row_id,
                                                         data_table_t secondary_table,
                                                         data_row_t secondary_row_id
                                                       );

/*!
 *  \brief copies a data_full_id_t struct to this uninitialized data_full_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes
 */
static inline void data_full_id_copy ( data_full_id_t *this_, const data_full_id_t *that );

/*!
 *  \brief copies a data_full_id_t struct over this already initialized data_full_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes
 */
static inline void data_full_id_replace ( data_full_id_t *this_, const data_full_id_t *that );

/*!
 *  \brief destroys the data_full_id_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_full_id_destroy ( data_full_id_t *this_ );

/*!
 *  \brief gets the primary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the primary object
 */
static inline data_id_t data_full_id_get_primary_id ( const data_full_id_t *this_ );

/*!
 *  \brief gets a pointer on the primary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the primary object
 */
static inline const data_id_t *data_full_id_get_primary_id_const ( const data_full_id_t *this_ );

/*!
 *  \brief gets the secondary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the secondary object
 */
static inline data_id_t data_full_id_get_secondary_id ( const data_full_id_t *this_ );

/*!
 *  \brief gets a pointer on the secondary object id
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the secondary object
 */
static inline const data_id_t *data_full_id_get_secondary_id_const ( const data_full_id_t *this_ );

/*!
 *  \brief checks if two data_full_id_t objects are equal
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if both primary_id are equal.
 */
static inline bool data_full_id_equals ( const data_full_id_t *this_, const data_full_id_t *that );

/*!
 *  \brief checks if the primary_id is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if primary_id is valid
 */
static inline bool data_full_id_is_valid ( const data_full_id_t *this_ );

/*!
 *  \brief traces primary_id and secondary_id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_full_id_trace ( const data_full_id_t *this_ );

#include "set/data_full_id.inl"

#endif  /* DATA_FULL_ID_H */


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
