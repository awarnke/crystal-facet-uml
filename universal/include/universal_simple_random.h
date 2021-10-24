/* File: universal_simple_random.h; Copyright and License: see below */

#ifndef UNIVERSAL_SIMPLE_RANDOM_H
#define UNIVERSAL_SIMPLE_RANDOM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief wrapper on the stdlib rand/random function; sets the initial seed.
 *
 *  These functions produce pseudo-random numbers that are not suitable
 *  for security/cryptographic use cases.
 */

#include <stdint.h>

/*!
 *  \brief attributes of a universal_simple_random_t
 *
 */
struct universal_simple_random_struct {
    int dummy;
};

typedef struct universal_simple_random_struct universal_simple_random_t;

/*!
 *  \brief initializes the universal_simple_random_t struct, based on the clock as seed value
 *
 *  Note: You may call this often, the underlying random generator is just initialized once.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_simple_random_init ( universal_simple_random_t *this_ );

/*!
 *  \brief destroys the universal_simple_random_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_simple_random_destroy ( universal_simple_random_t *this_ );

/*!
 *  \brief calculates a pseudo-random integer covering the full range of uint16_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint16_t universal_simple_random_get_uint16 ( universal_simple_random_t *this_ );

/*!
 *  \brief calculates a pseudo-random integer covering the full range of uint32_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t universal_simple_random_get_uint32 ( universal_simple_random_t *this_ );

#include "universal_simple_random.inl"

#endif  /* UNIVERSAL_SIMPLE_RANDOM_H */


/*
Copyright 2021-2021 Andreas Warnke

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
