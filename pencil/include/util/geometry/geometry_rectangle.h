/* File: geometry_rectangle.h; Copyright and License: see below */

#ifndef GEOMETRY_RECTANGLE_H
#define GEOMETRY_RECTANGLE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores coordinates of a rectangle
 */

/*!
 *  \brief attributes of a rectangle
 */
struct geometry_rectangle_struct {
    double left;
    double top;
    double width;
    double height;
};

typedef struct geometry_rectangle_struct geometry_rectangle_t;

/*!
 *  \brief initializes the geometry_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_init ( geometry_rectangle_t *this_, double left, double top, double width, double height );

/*!
 *  \brief initializes the geometry_rectangle_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_init_emtpty ( geometry_rectangle_t *this_ );

/*!
 *  \brief destroys the geometry_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_destroy ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_left ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_top ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_right ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_bottom ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_width ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_height ( geometry_rectangle_t *this_ );

/*!
 *  \brief determines if a given coordinate is within geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate
 *  \param y y coordinate
 *  \return true if the location is within the rectangle.
 */
static inline bool geometry_rectangle_contains ( geometry_rectangle_t *this_, double x, double y );

/*!
 *  \brief determines if the given rectangle has a valid (positive) size
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the rectangle has a positive size.
 */
static inline bool geometry_rectangle_is_empty ( geometry_rectangle_t *this_ );

/*!
 *  \brief prints the geometry_rectangle_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_trace ( geometry_rectangle_t *this_ );

#include "util/geometry/geometry_rectangle.inl"

#endif  /* GEOMETRY_RECTANGLE_H */


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
