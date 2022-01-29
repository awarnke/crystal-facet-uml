/* File: json_type_name_map.h; Copyright and License: see below */

#ifndef JSON_TYPE_NAME_MAP_H
#define JSON_TYPE_NAME_MAP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief maps type ids to type names (for the model)
 *         and flag bitsets to tag name lists (for the diagrams)
 */

#include "data_diagram.h"
#include "data_classifier.h"
#include "data_table.h"
#include "util/string/utf8string.h"
#include "util/string/utf8stringview.h"

/*!
 *  \brief attributes of the json_type_name_map
 */
struct json_type_name_map_struct {
    int dummy;  /*!< a dummy value */
};

typedef struct json_type_name_map_struct json_type_name_map_t;

/*!
 *  \brief initializes the json_type_name_map
 *
 *  \param this_ pointer to own object attributes
 */
static inline void json_type_name_map_init( json_type_name_map_t *this_ );

/*!
 *  \brief destroys the json_type_name_map
 *
 *  \param this_ pointer to own object attributes
 */
static inline void json_type_name_map_destroy( json_type_name_map_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return 0 in case of success, -1 otherwise
 */
static inline const char * json_type_name_map_get_classifier_type ( json_type_name_map_t *this_, data_classifier_type_t c_type );

#include "json_type_name_map.inl"

#endif  /* JSON_TYPE_NAME_MAP_H */


/*
Copyright 2022-2022 Andreas Warnke

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
