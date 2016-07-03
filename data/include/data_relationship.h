/* File: data_relationship.h; Copyright and License: see below */

#ifndef DATA_RELATIONSHIP_H
#define DATA_RELATIONSHIP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a relationship between two classifiers
 */

#include "data_relationship_type.h"
#include "util/string/utf8stringbuf.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_relationship_max_enum {
    DATA_RELATIONSHIP_MAX_NAME_SIZE = 48,
    DATA_RELATIONSHIP_MAX_NAME_LENGTH = 47,
    DATA_RELATIONSHIP_MAX_DESCRIPTION_SIZE = 1024,
    DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH = 1023,
};

/*!
 *  \brief all data attributes needed for the relationship functions
 *
 *  \see http://www.uml-diagrams.org/uml-core.html#relationship
 */
struct data_relationship_struct {
    int64_t id;
    data_relationship_type_t main_type;
    int64_t from_classifier_id;
    int64_t to_classifier_id;
    utf8stringbuf_t name;
    char private_name_buffer[DATA_RELATIONSHIP_MAX_NAME_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_RELATIONSHIP_MAX_DESCRIPTION_SIZE];
    int32_t list_order;
};

typedef struct data_relationship_struct data_relationship_t;

/*!
 *  \brief initializes the data_relationship_t struct with id, from_classifier_id and to_classifier_id DATA_ID_VOID_ID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_init_empty ( data_relationship_t *this_ );

/*!
 *  \brief re-initializes the data_relationship_t struct with id, from_classifier_id and to_classifier_id DATA_ID_VOID_ID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_reinit_empty ( data_relationship_t *this_ );

/*!
 *  \brief destroys the data_relationship_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_relationship_destroy ( data_relationship_t *this_ );

#include "data_relationship.inl"

#endif  /* DATA_RELATIONSHIP_H */


/*
Copyright 2016-2016 Andreas Warnke

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
