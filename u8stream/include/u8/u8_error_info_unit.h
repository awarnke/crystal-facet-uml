/* File: u8_error_info_unit.h; Copyright and License: see below */

#ifndef U8_ERROR_INFO_UNIT_H
#define U8_ERROR_INFO_UNIT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines unit and thereby semantics of data in u8_error_info_t
 */

/*!
 *  \brief enumeration of units
 *
 *  This is a simple enumeration, no semantics on single bits in integers.
 */
enum u8_error_info_unit_enum {
    U8_ERROR_INFO_UNIT_VOID = 0,  /*!< there is no information in the data fields of u8_error_info_t */
    U8_ERROR_INFO_UNIT_LINE = 1,  /*!< u8_error_info_t describes a position, unit is line number, first line is 1 */
    U8_ERROR_INFO_UNIT_NAME = 2,  /*!< u8_error_info_t describes an object, unit is its name/key */
    U8_ERROR_INFO_UNIT_LINE_NAME = 3,  /*!< u8_error_info_t describes a position and an object, units are line and name */
    U8_ERROR_INFO_UNIT_MAX = 4,  /*!< maximum number of units, can be used to iterate over all units */
};

typedef enum u8_error_info_unit_enum u8_error_info_unit_t;

#endif  /* U8_ERROR_INFO_UNIT_H */


/*
Copyright 2022-2025 Andreas Warnke

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
