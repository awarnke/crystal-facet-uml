/* File: layout_order.h; Copyright and License: see below */

#ifndef LAYOUT_ORDER_H
#define LAYOUT_ORDER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Represents an order value for an object, either a list order or an x-/y- order
 */

#include <stdint.h>

/*!
 *  \brief attributes of the layout_order
 */
struct layout_order_struct {
    int32_t first;
    int32_t second;
};

typedef struct layout_order_struct layout_order_t;

/*!
 *  \brief initializes the layout_order_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void layout_order_init ( layout_order_t *this_, int32_t first, int32_t second );

/*!
 *  \brief re-initializes the layout_order_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param first first value of the pair
 *  \param second second value of the pair
 */
static inline void layout_order_reinit ( layout_order_t *this_, int32_t first, int32_t second );

/*!
 *  \brief copies original to this uninitialized layout_order_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void layout_order_copy ( layout_order_t *this_, const layout_order_t *original );

/*!
 *  \brief replaces the already initialized layout_order_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void layout_order_replace ( layout_order_t *this_, const layout_order_t *original );

/*!
 *  \brief initializes the layout_order_t struct to a pair of 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_order_init_empty ( layout_order_t *this_ );

/*!
 *  \brief re-initializes the layout_order_t struct to a pair of 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_order_reinit_empty ( layout_order_t *this_ );

/*!
 *  \brief destroys the layout_order_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_order_destroy ( layout_order_t *this_ );

/*!
 *  \brief gets the first value of layout_order_t
 *
 *  \param this_ pointer to own object attributes
 *  \return first value of the pair
 */
static inline int32_t layout_order_get_first ( const layout_order_t *this_ );

/*!
 *  \brief gets the second value of layout_order_t
 *
 *  \param this_ pointer to own object attributes
 *  \return second value of the pair
 */
static inline int32_t layout_order_get_second ( const layout_order_t *this_ );

/*!
 *  \brief prints the layout_order_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_order_trace ( const layout_order_t *this_ );

#include "layout_order.inl"

#endif  /* LAYOUT_ORDER_H */


/*
Copyright 2018-2018 Andreas Warnke

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
