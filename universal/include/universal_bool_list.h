/* File: universal_bool_list.h; Copyright and License: see below */

#ifndef UNIVERSAL_BOOL_LIST_H
#define UNIVERSAL_BOOL_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores a list of up to 32 booleans
 */

#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of a bool list
 */
struct universal_bool_list_struct {
    uint32_t list;
};

typedef struct universal_bool_list_struct universal_bool_list_t;

/*!
 *  \brief initializes the universal_bool_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void universal_bool_list_init_pair ( universal_bool_list_t *this_, bool first, bool second );

/*!
 *  \brief re-initializes the universal_bool_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void universal_bool_list_reinit_pair ( universal_bool_list_t *this_, bool first, bool second );

/*!
 *  \brief copies original to this uninitialized universal_bool_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void universal_bool_list_copy ( universal_bool_list_t *this_, const universal_bool_list_t *original );

/*!
 *  \brief replaces the already initialized universal_bool_list_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void universal_bool_list_replace ( universal_bool_list_t *this_, const universal_bool_list_t *original );

/*!
 *  \brief initializes the universal_bool_list_t struct to empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_bool_list_init_empty ( universal_bool_list_t *this_ );

/*!
 *  \brief re-initializes the universal_bool_list_t struct empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_bool_list_reinit_empty ( universal_bool_list_t *this_ );

/* Note: Till now, only constructors for pairs are provided. Extend as needed. */

/*!
 *  \brief destroys the universal_bool_list_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_bool_list_destroy ( universal_bool_list_t *this_ );

/*!
 *  \brief gets the first value of universal_bool_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \return first value of the pair
 */
static inline bool universal_bool_list_get_first ( const universal_bool_list_t *this_ );

/*!
 *  \brief gets the second value of universal_bool_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \return second value of the pair
 */
static inline bool universal_bool_list_get_second ( const universal_bool_list_t *this_ );

/*!
 *  \brief prints the universal_bool_list_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_bool_list_trace ( const universal_bool_list_t *this_ );

#include "universal_bool_list.inl"

#endif  /* UNIVERSAL_BOOL_LIST_H */


/*
Copyright 2017-2018 Andreas Warnke

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
