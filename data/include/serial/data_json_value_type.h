/* File: data_json_value_type.h; Copyright and License: see below */

#ifndef DATA_JSON_VALUE_TYPE_H
#define DATA_JSON_VALUE_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an enumeration of JSON value types
 */

/*!
 *  \brief type constants, see rfc7159#rfc.section.3
 */
enum data_json_value_type_enum {
    DATA_JSON_VALUE_TYPE_UNDEF = 0,  /*!< undefined type */
    DATA_JSON_VALUE_TYPE_OBJECT,  /*!< object type */
    DATA_JSON_VALUE_TYPE_ARRAY,  /*!< array type */
    DATA_JSON_VALUE_TYPE_NUMBER,  /*!< number type */
    DATA_JSON_VALUE_TYPE_INTEGER,  /*!< integer subtype of number: numbers without exponent and without fraction */
    DATA_JSON_VALUE_TYPE_STRING,  /*!< string type */
    DATA_JSON_VALUE_TYPE_BOOLEAN,  /*!< false or true literal of type boolean */
    DATA_JSON_VALUE_TYPE_NULL,  /*!< null literal */
};

typedef enum data_json_value_type_enum data_json_value_type_t;

#endif  /* DATA_JSON_VALUE_TYPE_H */


/*
Copyright 2016-2018 Andreas Warnke

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
