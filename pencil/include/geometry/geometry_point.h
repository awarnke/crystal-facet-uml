/* File: geometry_point.h; Copyright and License: see below */

#ifndef GEOMETRY_POINT_H
#define GEOMETRY_POINT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the coordinates of a point
 */

#include "geometry/geometry_direction.h"
#include <stdbool.h>

/*!
 *  \brief attributes of a point
 */
struct geometry_point_struct {
    double x;
    double y;
};

typedef struct geometry_point_struct geometry_point_t;

/*!
 *  \brief initializes the geometry_point_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the object
 *  \param y y coordinate of the object
 */
static inline void geometry_point_init ( geometry_point_t *this_, double x, double y );

/*!
 *  \brief initializes the geometry_point_t struct
 *
 *  \param x x coordinate of the object
 *  \param y y coordinate of the object
 *  \return the initialized point
 */
static inline geometry_point_t geometry_point_new ( double x, double y );

/*!
 *  \brief re-initializes the geometry_point_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the object
 *  \param y y coordinate of the object
 */
static inline void geometry_point_reinit ( geometry_point_t *this_, double x, double y );

/*!
 *  \brief copies original to this uninitialized geometry_point_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_point_copy ( geometry_point_t *this_, const geometry_point_t *original );

/*!
 *  \brief replaces the already initialized geometry_point_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_point_replace ( geometry_point_t *this_, const geometry_point_t *original );

/*!
 *  \brief destroys the geometry_point_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_point_destroy ( geometry_point_t *this_ );

/*!
 *  \brief gets the x coordinate of geometry_point_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_point_get_x ( const geometry_point_t *this_ );

/*!
 *  \brief gets the y coordinate of geometry_point_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_point_get_y ( const geometry_point_t *this_ );

/*!
 *  \brief gets the chess distance between two geometry_point_t structs
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return chess distance between the two points
 */
static inline double geometry_point_calc_chess_distance ( const geometry_point_t *this_, const geometry_point_t *that );

/*!
 *  \brief determines if the two geometry_point_t refer to the same location
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if the two points are equal
 */
static inline bool geometry_point_equals ( const geometry_point_t *this_, const geometry_point_t *that );

/*!
 *  \brief shifts the geometry_point_t
 *
 *  \param this_ pointer to own object attributes
 *  \param delta_x value by which to increase the x.
 *  \param delta_y value by which to increase the y.
 */
static inline void geometry_point_shift ( geometry_point_t *this_, double delta_x, double delta_y );

/*!
 *  \brief prints the geometry_point_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_point_trace ( const geometry_point_t *this_ );

#include "geometry/geometry_point.inl"

#endif  /* GEOMETRY_POINT_H */


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
