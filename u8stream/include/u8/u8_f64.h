/* File: u8_f64.h; Copyright and License: see below */

#ifndef U8_F64_H
#define U8_F64_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides some basic f64ematical constants and functions for type double
 */


/*!
 *  \brief determines the maximum of 2 double
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \return the maximum of a and b
 */
static inline double u8_f64_max2 ( const double a, const double b );

/*!
 *  \brief determines the maximum of 3 double
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \param c a number to compare
 *  \return the maximum of a and b and c
 */
static inline double u8_f64_max3 ( const double a, const double b, const double c );

/*!
 *  \brief determines the minimum of 2 double
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \return the minimum of a and b
 */
static inline double u8_f64_min2 ( const double a, const double b );

/*!
 *  \brief determines the minimum of 3 double
 *
 *  \param a a number to compare
 *  \param b a number to compare
 *  \param c a number to compare
 *  \return the minimum of a and b and c
 */
static inline double u8_f64_min3 ( const double a, const double b, const double c );

#include "u8/u8_f64.inl"

#endif  /* U8_F64_H */


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
