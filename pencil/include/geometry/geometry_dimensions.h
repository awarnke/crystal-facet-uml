/* File: geometry_dimensions.h; Copyright and License: see below */

#ifndef GEOMETRY_DIMENSIONS_H
#define GEOMETRY_DIMENSIONS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores width and height of an object
 */

#include <stdbool.h>

/*!
 *  \brief attributes of dimensions
 */
struct geometry_dimensions_struct {
    double width;
    double height;
};

typedef struct geometry_dimensions_struct geometry_dimensions_t;

/*!
 *  \brief initializes the geometry_dimensions_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param width width of the object
 *  \param height height of the object
 */
static inline void geometry_dimensions_init ( geometry_dimensions_t *this_, double width, double height );

/*!
 *  \brief re-initializes the geometry_dimensions_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param width width of the object
 *  \param height height of the object
 */
static inline void geometry_dimensions_reinit ( geometry_dimensions_t *this_, double width, double height );

/*!
 *  \brief copies original to this uninitialized geometry_dimensions_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_dimensions_copy ( geometry_dimensions_t *this_, const geometry_dimensions_t *original );

/*!
 *  \brief replaces the already initialized geometry_dimensions_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_dimensions_replace ( geometry_dimensions_t *this_, const geometry_dimensions_t *original );

/*!
 *  \brief initializes the geometry_dimensions_t struct to an empty rect
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_dimensions_init_empty ( geometry_dimensions_t *this_ );

/*!
 *  \brief re-initializes the geometry_dimensions_t struct to an empty rect
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_dimensions_reinit_empty ( geometry_dimensions_t *this_ );

/*!
 *  \brief destroys the geometry_dimensions_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_dimensions_destroy ( geometry_dimensions_t *this_ );

/*!
 *  \brief gets the width of geometry_dimensions_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_dimensions_get_width ( const geometry_dimensions_t *this_ );

/*!
 *  \brief gets the height of geometry_dimensions_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_dimensions_get_height ( const geometry_dimensions_t *this_ );

/*!
 *  \brief gets the area-value of geometry_dimensions_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_dimensions_get_area ( const geometry_dimensions_t *this_ );

/*!
 *  \brief determines if the given dimensions have a valid (positive) size
 *
 *  \param this_ pointer to own object attributes
 *  \return false if the dimensions have a positive size.
 */
static inline bool geometry_dimensions_is_empty ( const geometry_dimensions_t *this_ );

/*!
 *  \brief determines if the given dimensions can contain the other
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to others object attributes
 *  \return false if the dimensions can not contain the other. (touching is still containing)
 */
static inline bool geometry_dimensions_can_contain ( const geometry_dimensions_t *this_, const geometry_dimensions_t *that );

/*!
 *  \brief expands or shrinks the geometry_dimensions_t
 *
 *  \param this_ pointer to own object attributes
 *  \param delta_width value by which to increase the width. In case of a negative value, the dimensions shrink (but not below 0).
 *  \param delta_height value by which to increase the height. In case of a negative value, the dimensions shrink (but not below 0).
 */
static inline void geometry_dimensions_expand ( geometry_dimensions_t *this_, double delta_width, double delta_height );

/*!
 *  \brief prints the geometry_dimensions_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_dimensions_trace ( const geometry_dimensions_t *this_ );

#include "geometry/geometry_dimensions.inl"

#endif  /* GEOMETRY_DIMENSIONS_H */


/*
Copyright 2019-2025 Andreas Warnke

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
