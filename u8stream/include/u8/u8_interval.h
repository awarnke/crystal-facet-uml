/* File: u8_interval.h; Copyright and License: see below */

#ifndef U8_INTERVAL_H
#define U8_INTERVAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides a representation of an interval of double values
 */

/*!
 *  \brief attributes of the interval
 */
struct u8_interval_struct {
    double low;
    double high;
};

typedef struct u8_interval_struct u8_interval_t;

/*!
 *  \brief initializes a new interval covering void.
 *
 *  \return a new interval
 */
static inline u8_interval_t u8_interval_new_void();

/*!
 *  \brief initializes a new interval by unordered boundry values
 *
 *  \param a one of the boundaries
 *  \param b one of the boundaries
 *  \return the interval
 */
static inline u8_interval_t u8_interval_new_unordered ( double a, double b );

/*!
 *  \brief initializes a new interval by lesser and greater bounds
 *
 *  \param low the lesser boundary
 *  \param high the greater boundary
 *  \return the interval
 */
static inline u8_interval_t u8_interval_new ( double low, double high );

/*!
 *  \brief initializes a new interval by the intersect of a and b
 *
 *  \param a an interval
 *  \param b another interval
 *  \return the interval
 */
static inline u8_interval_t u8_interval_new_intersect ( const u8_interval_t *a, const u8_interval_t *b );

/*!
 *  \brief checks if the interval is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if both boundaries are not NaN
 */
static inline bool u8_interval_is_valid ( const u8_interval_t *this_ );

/*!
 *  \brief gets the lesser boundary
 *
 *  \param this_ pointer to own object attributes
 *  \return the lesser boundary
 */
static inline double u8_interval_get_low ( const u8_interval_t *this_ );

/*!
 *  \brief gets the greater boundary
 *
 *  \param this_ pointer to own object attributes
 *  \return the greater boundary
 */
static inline double u8_interval_get_high ( const u8_interval_t *this_ );

/*!
 *  \brief gets the width
 *
 *  \param this_ pointer to own object attributes
 *  \return the width of the interval
 */
static inline double u8_interval_get_width ( const u8_interval_t *this_ );

#include "u8/u8_interval.inl"

#endif  /* U8_INTERVAL_H */


/*
Copyright 2025-2025 Andreas Warnke

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
