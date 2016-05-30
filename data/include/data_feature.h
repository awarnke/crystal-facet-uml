/* File: data_feature.h; Copyright and License: see below */

#ifndef DATA_FEATURE_H
#define DATA_FEATURE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a feature like a class-attribute or a class-method
 */

#include "data_feature_type.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_feature_max_enum {
    DATA_FEATURE_MAX_KEY_SIZE = 32,
    DATA_FEATURE_MAX_KEY_LENGTH = 31,
    DATA_FEATURE_MAX_VALUE_SIZE = 256,
    DATA_FEATURE_MAX_VALUE_LENGTH = 255,
    DATA_FEATURE_MAX_DESCRIPTION_SIZE = 1024,
    DATA_FEATURE_MAX_DESCRIPTION_LENGTH = 1023,
};

/*!
 *  \brief all data attributes needed for the feature (property or operation) functions
 *
 *  \see http://www.uml-diagrams.org/uml-core.html#feature
 */
struct data_feature_struct {
    int64_t id;
    data_feature_type_t main_type;
    int64_t classifier_id;
    utf8stringbuf_t key;
    char private_key_buffer[DATA_FEATURE_MAX_KEY_SIZE];
    utf8stringbuf_t value;
    char private_value_buffer[DATA_FEATURE_MAX_VALUE_SIZE];
    utf8stringbuf_t description;
    char private_description_buffer[DATA_FEATURE_MAX_DESCRIPTION_SIZE];
    int32_t list_order;
};

typedef struct data_feature_struct data_feature_t;

/*!
 *  \brief initializes the data_feature_t struct with id and classifier_id DATA_ID_VOID_ID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_init_empty ( data_feature_t *this_ );

/*!
 *  \brief re-initializes the data_feature_t struct with id and classifier_id DATA_ID_VOID_ID; all other values are zero.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_reinit_empty ( data_feature_t *this_ );

/*!
 *  \brief destroys the data_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_feature_destroy ( data_feature_t *this_ );

#include "data_feature.inl"

#endif  /* DATA_FEATURE_H */


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
