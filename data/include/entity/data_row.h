/* File: data_row.h; Copyright and License: see below */

#ifndef DATA_ROW_H
#define DATA_ROW_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Type for wrapping the database-internal identifier-type
 */

#include <stdint.h>

/*!
 *  \brief constant to define a void data_row_t
 */
enum data_row_enum {
    DATA_ROW_VOID = -1,  /*!< invalid id */
};

/*!
 *  \brief wraps the database-internal identifier-type
 */
typedef int64_t data_row_t;

#endif  /* DATA_ROW_H */


/*
Copyright 2020-2025 Andreas Warnke

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
