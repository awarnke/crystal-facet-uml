/* File: geometry_offset.h; Copyright and License: see below */

#ifndef GEOMETRY_OFFSET_H
#define GEOMETRY_OFFSET_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores an offset in a 2-dimensional space
 */

#include <stdbool.h>

/*!
 *  \brief attributes of an offset
 */
struct geometry_offset_struct {
    double dx;
    double dy;
};

typedef struct geometry_offset_struct geometry_offset_t;

/*!
 *  \brief initializes the geometry_offset_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param dx x offset of the object
 *  \param dy y offset of the object
 */
static inline void geometry_offset_init ( geometry_offset_t *this_, double dx, double dy );

/*!
 *  \brief initializes the geometry_offset_t struct
 *
 *  \param dx x offset of the object
 *  \param dy y offset of the object
 *  \return the initialized offset
 */
static inline geometry_offset_t geometry_offset_new ( double dx, double dy );

/*!
 *  \brief re-initializes the geometry_offset_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param dx x offset of the object
 *  \param dy y offset of the object
 */
static inline void geometry_offset_reinit ( geometry_offset_t *this_, double dx, double dy );

/*!
 *  \brief copies original to this uninitialized geometry_offset_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_offset_copy ( geometry_offset_t *this_, const geometry_offset_t *original );

/*!
 *  \brief replaces the already initialized geometry_offset_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_offset_replace ( geometry_offset_t *this_, const geometry_offset_t *original );

/*!
 *  \brief destroys the geometry_offset_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_offset_destroy ( geometry_offset_t *this_ );

/*!
 *  \brief gets the x offset of geometry_offset_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_offset_get_dx ( const geometry_offset_t *this_ );

/*!
 *  \brief gets the y offset of geometry_offset_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_offset_get_dy ( const geometry_offset_t *this_ );

/*!
 *  \brief determines if the two geometry_offset_t refer to the same location
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if the two points are equal
 */
static inline bool geometry_offset_equals ( const geometry_offset_t *this_, const geometry_offset_t *that );

/*!
 *  \brief prints the geometry_offset_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_offset_trace ( const geometry_offset_t *this_ );

#include "geometry/geometry_offset.inl"

#endif  /* GEOMETRY_OFFSET_H */


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
