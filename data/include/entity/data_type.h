/* File: data_type.h; Copyright and License: see below */

#ifndef DATA_TYPE_H
#define DATA_TYPE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Type of a data object consisting of table and table-specific type
 */

#include "entity/data_table.h"
#include "entity/data_relationship_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_classifier_type.h"
#include "entity/data_diagram_type.h"
#include <stdbool.h>

/*!
 *  \brief attributes of the data type
 */
struct data_type_struct {
    data_table_t context;  /*!<  element type for which this type resource applies */
    int specific_type;  /*!<  type id for which this type resource applies */
};

typedef struct data_type_struct data_type_t;

/*!
 *  \brief initializes the data_type_t struct to void
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_type_init_void ( data_type_t *this_ );

/*!
 *  \brief initializes the data_type_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 */
static inline void data_type_init_diagram ( data_type_t *this_, data_diagram_type_t type );

/*!
 *  \brief initializes the data_type_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 */
static inline void data_type_init_classifier ( data_type_t *this_, data_classifier_type_t type );

/*!
 *  \brief initializes the data_type_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 */
static inline void data_type_init_feature ( data_type_t *this_, data_feature_type_t type );

/*!
 *  \brief initializes the data_type_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param type type id
 */
static inline void data_type_init_relationship ( data_type_t *this_, data_relationship_type_t type );

/*!
 *  \brief destroys the data_type_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_type_destroy ( data_type_t *this_ );

/*!
 *  \brief gets the context
 *
 *  \param this_ pointer to own object attributes
 *  \return context of this data_type_t
 */
static inline data_table_t data_type_get_context ( const data_type_t *this_ );

/*!
 *  \brief gets the type as integer
 *
 *  \param this_ pointer to own object attributes
 *  \return type of this data_type_t
 */
static inline int data_type_get_type_as_int ( const data_type_t *this_ );

/*!
 *  \brief gets the diagram type
 *
 *  \param this_ pointer to own object attributes
 *  \return diagram type of this data_type_t, void if this_ is not a diagram
 */
static inline const data_diagram_type_t data_type_get_diagram_type ( const data_type_t *this_ );

/*!
 *  \brief gets the classifier type
 *
 *  \param this_ pointer to own object attributes
 *  \return classifier type of this data_type_t, void if this_ is not a classifier
 */
static inline data_classifier_type_t data_type_get_classifier_type ( const data_type_t *this_ );

/*!
 *  \brief gets the feature type
 *
 *  \param this_ pointer to own object attributes
 *  \return feature type of this data_type_t, void if this_ is not a feature
 */
static inline data_feature_type_t data_type_get_feature_type ( const data_type_t *this_ );

/*!
 *  \brief gets the relationship type
 *
 *  \param this_ pointer to own object attributes
 *  \return relationship type of this data_type_t, void if this_ is not a relationship
 */
static inline data_relationship_type_t data_type_get_relationship_type ( const data_type_t *this_ );

/*!
 *  \brief checks if two data_type_t are equal
 *
 *  Two data types are equal if data_table_t is not DATA_TABLE_VOID and equal
 *  and specific_type is not DATA_&lt;anytype&gt;_TYPE_VOID and equal
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if both data_type_t are equal
 */
static inline bool data_type_equals ( const data_type_t *this_, const data_type_t *that );

#include "entity/data_type.inl"

#endif  /* DATA_TYPE_H */


/*
Copyright 2023-2026 Andreas Warnke

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
