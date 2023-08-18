/* File: u8_i32.h; Copyright and License: see below */

#ifndef U8_I32_H
#define U8_I32_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides some basic mathematical constants and functions for type int_fast32
 */


/*!
 *  \brief determines the maximum of 2 32 bit signed integer
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \return the maximum of a and b
 */
static inline int_fast32_t u8_i32_max2 ( const int_fast32_t a, const int_fast32_t b );

/*!
 *  \brief determines the maximum of 3 32 bit signed integer
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \param c a number to compare
 *  \return the maximum of a and b and c
 */
static inline int_fast32_t u8_i32_max3 ( const int_fast32_t a, const int_fast32_t b, const int_fast32_t c );

/*!
 *  \brief determines the minimum of 2 32 bit signed integer
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \return the minimum of a and b
 */
static inline int_fast32_t u8_i32_min2 ( const int_fast32_t a, const int_fast32_t b );

/*!
 *  \brief determines the minimum of 3 32 bit signed integer
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \param c a number to compare
 *  \return the minimum of a and b and c
 */
static inline int_fast32_t u8_i32_min3 ( const int_fast32_t a, const int_fast32_t b, const int_fast32_t c );

#include "u8/u8_i32.inl"

#endif  /* U8_I32_H */


/*
Copyright 2023-2023 Andreas Warnke

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
