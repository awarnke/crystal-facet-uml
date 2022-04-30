/* File: universal_int.h; Copyright and License: see below */

#ifndef UNIVERSAL_INT_H
#define UNIVERSAL_INT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief provides basic integer functions like min and max
 */

#include <stdint.h>

/*!
 *  \brief determines the minimum of two signed 32 bit integers
 *
 *  \param this_ first signed integer
 *  \param that second signed integer
 *  \return minimum of two signed integer
 */
static inline int32_t universal_int_min_i32 ( int32_t this_, int32_t that );

/*!
 *  \brief determines the maximum of two signed 32 bit integers
 *
 *  \param this_ first signed integer
 *  \param that second signed integer
 *  \return maximum of two signed integer
 */
static inline int32_t universal_int_max_i32 ( int32_t this_, int32_t that );

#include "u8list/universal_int.inl"

#endif  /* UNIVERSAL_INT_H */


/*
Copyright 2021-2022 Andreas Warnke

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
