/* File: layout_relationship.h; Copyright and License: see below */

#ifndef LAYOUT_RELATIONSHIP_H
#define LAYOUT_RELATIONSHIP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines visibility, shape and location of a relationship
 */

#include "option/pencil_visibility.h"
#include "util/geometry/geometry_connector.h"
#include "data_relationship.h"

/*!
 *  \brief visibility, shape and location of a relationship
 */
struct layout_relationship_struct {
    pencil_visibility_t visible;  /*!< defines if the relationship is visible */
    geometry_connector_t shape;  /*!< shape of relationship records */
    data_relationship_t *relationship_data;  /*!< pointer to the data object of the relationship */
};

typedef struct layout_relationship_struct layout_relationship_t;

/*!
 *  \brief initializes the layout_relationship_t struct with a pointer to data_relationship_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_data pointer to data object which is layouted
 */
static inline void layout_relationship_init_empty ( layout_relationship_t *this_, data_relationship_t *relationship_data );

/*!
 *  \brief destroys the layout_relationship_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_relationship_destroy ( layout_relationship_t *this_ );

/*!
 *  \brief gets the data_relationship_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to a data_relationship_t object.
 */
static inline data_relationship_t *layout_relationship_get_data_ptr ( const layout_relationship_t *this_ );

#include "layout_relationship.inl"

#endif  /* LAYOUT_RELATIONSHIP_H */


/*
Copyright 2018-2018 Andreas Warnke

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
