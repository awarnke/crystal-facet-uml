/* File: data_json_serializer.h; Copyright and License: see below */

#ifndef DATA_JSON_SERIALIZER_H
#define DATA_JSON_SERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Serializes data objects to json format.
 */

#include "data_error.h"

/*!
 *  \brief all data attributes needed for serializing data objects
 */
struct data_json_serializer_struct {
    int32_t dummy;
};

typedef struct data_json_serializer_struct data_json_serializer_t;

/*!
 *  \brief initializes the data_json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_serializer_init ( data_json_serializer_t *this_ );

/*!
 *  \brief destroys the data_json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_serializer_destroy ( data_json_serializer_t *this_ );

#endif  /* DATA_JSON_SERIALIZER_H */


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
