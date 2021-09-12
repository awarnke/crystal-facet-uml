/* File: universal_random.h; Copyright and License: see below */

#ifndef UNIVERSAL_RANDOM_H
#define UNIVERSAL_RANDOM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief wrapper on the stdlib random functions and perforns the initial seed
 */

#include <stdint.h>

/*!
 *  \brief attributes of a universal_random_t
 *
 */
struct universal_random_struct {
    int dummy;
};

typedef struct universal_random_struct universal_random_t;

/*!
 *  \brief initializes the universal_random_t struct
 *
 *  Note: You may call this often, the underlying random generator is just initialized once.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_random_init ( universal_random_t *this_ );

/*!
 *  \brief destroys the universal_random_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_random_destroy ( universal_random_t *this_ );

/*!
 *  \brief calculates a pseudo-random integer
 *
 *  \param this_ pointer to own object attributes
 */
static inline int universal_random_get_int ( universal_random_t *this_ );

#include "universal_random.inl"

#endif  /* UNIVERSAL_RANDOM_H */


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
