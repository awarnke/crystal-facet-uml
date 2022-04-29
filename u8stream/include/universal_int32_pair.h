/* File: universal_int32_pair.h; Copyright and License: see below */

#ifndef UNIVERSAL_INT32_PAIR_H
#define UNIVERSAL_INT32_PAIR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores a pair of int32_t
 */

#include <stdint.h>

/*!
 *  \brief attributes of an int pair
 */
struct universal_int32_pair_struct {
    int32_t first;
    int32_t second;
};

typedef struct universal_int32_pair_struct universal_int32_pair_t;

/*!
 *  \brief initializes the universal_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void universal_int32_pair_init ( universal_int32_pair_t *this_, int32_t first, int32_t second );

/*!
 *  \brief re-initializes the universal_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void universal_int32_pair_reinit ( universal_int32_pair_t *this_, int32_t first, int32_t second );

/*!
 *  \brief copies original to this uninitialized universal_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void universal_int32_pair_copy ( universal_int32_pair_t *this_, const universal_int32_pair_t *original );

/*!
 *  \brief replaces the already initialized universal_int32_pair_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void universal_int32_pair_replace ( universal_int32_pair_t *this_, const universal_int32_pair_t *original );

/*!
 *  \brief initializes the universal_int32_pair_t struct to a pair of 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_int32_pair_init_empty ( universal_int32_pair_t *this_ );

/*!
 *  \brief re-initializes the universal_int32_pair_t struct to a pair of 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_int32_pair_reinit_empty ( universal_int32_pair_t *this_ );

/*!
 *  \brief destroys the universal_int32_pair_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_int32_pair_destroy ( universal_int32_pair_t *this_ );

/*!
 *  \brief gets the first value of universal_int32_pair_t
 *
 *  \param this_ pointer to own object attributes
 *  \return first value of the pair
 */
static inline int32_t universal_int32_pair_get_first ( const universal_int32_pair_t *this_ );

/*!
 *  \brief gets the second value of universal_int32_pair_t
 *
 *  \param this_ pointer to own object attributes
 *  \return second value of the pair
 */
static inline int32_t universal_int32_pair_get_second ( const universal_int32_pair_t *this_ );

/*!
 *  \brief prints the universal_int32_pair_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_int32_pair_trace ( const universal_int32_pair_t *this_ );

#include "universal_int32_pair.inl"

#endif  /* UNIVERSAL_INT32_PAIR_H */


/*
Copyright 2016-2022 Andreas Warnke

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
