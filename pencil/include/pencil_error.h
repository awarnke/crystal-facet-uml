/* File: pencil_error.h; Copyright and License: see below */

#ifndef PENCIL_ERROR_H
#define PENCIL_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies errors in the pencil module
 */

/*!
 *  \brief error constants which explain errors which occurred in the pencil module
 */
enum pencil_error_enum {
    PENCIL_ERROR_NONE = 0,  /*!< 0: success */
    PENCIL_ERROR_OUT_OF_BOUNDS = 1,  /*!< 1: input parameters are out of bounds */
    PENCIL_ERROR_UNKNOWN_OBJECT = 2,  /*!< 2: referenced object is not in current diagram */
};

typedef enum pencil_error_enum pencil_error_t;

#endif  /* PENCIL_ERROR_H */


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
