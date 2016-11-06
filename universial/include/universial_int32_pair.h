/* File: universial_int32_pair.h; Copyright and License: see below */

#ifndef UNIVERSIAL_INT32_PAIR_H
#define UNIVERSIAL_INT32_PAIR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores a pair of int32_t
 */

#include <stdint.h>

/*!
 *  \brief attributes of an int pair
 */
struct universial_int32_pair_struct {
    int32_t first;
    int32_t second;
};

typedef struct universial_int32_pair_struct universial_int32_pair_t;

/*!
 *  \brief initializes the universial_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void universial_int32_pair_init ( universial_int32_pair_t *this_, int32_t first, int32_t second );

/*!
 *  \brief re-initializes the universial_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void universial_int32_pair_reinit ( universial_int32_pair_t *this_, int32_t first, int32_t second );

/*!
 *  \brief copies original to this uninitialized universial_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void universial_int32_pair_copy ( universial_int32_pair_t *this_, const universial_int32_pair_t *original );

/*!
 *  \brief replaces the already initialized universial_int32_pair_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void universial_int32_pair_replace ( universial_int32_pair_t *this_, const universial_int32_pair_t *original );

/*!
 *  \brief initializes the universial_int32_pair_t struct to a pair of 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universial_int32_pair_init_empty ( universial_int32_pair_t *this_ );

/*!
 *  \brief re-initializes the universial_int32_pair_t struct to a pair of 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universial_int32_pair_reinit_empty ( universial_int32_pair_t *this_ );

/*!
 *  \brief destroys the universial_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universial_int32_pair_destroy ( universial_int32_pair_t *this_ );

/*!
 *  \brief gets the first value of universial_int32_pair_t
 *
 *  \param this_ pointer to own object attributes
 *  \return first value of the pair
 */
static inline int32_t universial_int32_pair_get_first ( universial_int32_pair_t *this_ );

/*!
 *  \brief gets the second value of universial_int32_pair_t
 *
 *  \param this_ pointer to own object attributes
 *  \return second value of the pair
 */
static inline int32_t universial_int32_pair_get_second ( universial_int32_pair_t *this_ );

/*!
 *  \brief prints the universial_int32_pair_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universial_int32_pair_trace ( universial_int32_pair_t *this_ );

#include "universial_int32_pair.inl"

#endif  /* UNIVERSIAL_INT32_PAIR_H */


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
