/* File: geometry_rectangle.h; Copyright and License: see below */

#ifndef GEOMETRY_RECTANGLE_H
#define GEOMETRY_RECTANGLE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores coordinates of a rectangle
 */

#include <stdbool.h>

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
 *  \param left left coordinate of the rectangle
 *  \param top top coordinate of the rectangle
 *  \param width width of the rectangle
 *  \param height height of the rectangle
 */
static inline void geometry_rectangle_init ( geometry_rectangle_t *this_, double left, double top, double width, double height );

/*!
 *  \brief re-initializes the geometry_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param left left coordinate of the rectangle
 *  \param top top coordinate of the rectangle
 *  \param width width of the rectangle
 *  \param height height of the rectangle
 */
static inline void geometry_rectangle_reinit ( geometry_rectangle_t *this_, double left, double top, double width, double height );

/*!
 *  \brief copies original to this uninitialized geometry_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_rectangle_copy ( geometry_rectangle_t *this_, const geometry_rectangle_t *original );

/*!
 *  \brief replaces the already initialized geometry_rectangle_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_rectangle_replace ( geometry_rectangle_t *this_, const geometry_rectangle_t *original );

/*!
 *  \brief initializes the geometry_rectangle_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_init_empty ( geometry_rectangle_t *this_ );

/*!
 *  \brief re-initializes the geometry_rectangle_t struct to an empty rect at position 0,0
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_reinit_empty ( geometry_rectangle_t *this_ );

/*!
 *  \brief initializes the geometry_rectangle_t struct by the intersect of a and b
 *
 *  \param this_ pointer to own object attributes
 *  \param rect_a rectangle a. Must not be NULL
 *  \param rect_b rectangle b. Must not be NULL
 *  \return 0 in case of success, -1 if the rectangles a and b do not overlap (touching is intersecting)
 */
static inline int geometry_rectangle_init_by_intersect ( geometry_rectangle_t *this_,
                                                         const geometry_rectangle_t *rect_a,
                                                         const geometry_rectangle_t *rect_b
                                                       );

/*!
 *  \brief initializes the geometry_rectangle_t struct by providing two corner coordinates
 *
 *  \param this_ pointer to own object attributes
 *  \param x1 x coordinate of a corner
 *  \param y1 y coordinate of a corner
 *  \param x2 x coordinate of a corner
 *  \param y2 y coordinate of a corner
 */
static inline void geometry_rectangle_init_by_corners ( geometry_rectangle_t *this_, double x1, double y1, double x2, double y2 );

/*!
 *  \brief destroys the geometry_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_destroy ( geometry_rectangle_t *this_ );

/*!
 *  \brief gets the left coordinate of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_left ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the top coordinate of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_top ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the right coordinate of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_right ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the bottom coordinate of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_bottom ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the width of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_width ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the height of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_height ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the x-center of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_x_center ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the y-center of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_y_center ( const geometry_rectangle_t *this_ );

/*!
 *  \brief gets the area-value of geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_rectangle_get_area ( const geometry_rectangle_t *this_ );

/*!
 *  \brief determines if a given coordinate is within geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate
 *  \param y y coordinate
 *  \return true if the location is within the rectangle.
 */
static inline bool geometry_rectangle_contains ( const geometry_rectangle_t *this_, double x, double y );

/*!
 *  \brief determines if the given rectangle has a valid (positive) size
 *
 *  \param this_ pointer to own object attributes
 *  \return false if the rectangle has a positive size.
 */
static inline bool geometry_rectangle_is_empty ( const geometry_rectangle_t *this_ );

/*!
 *  \brief determines if the given rectangle intersects the other
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to others object attributes
 *  \return false if the rectangle does not intersect the other. (touching is intersecting)
 */
static inline bool geometry_rectangle_is_intersecting ( const geometry_rectangle_t *this_, const geometry_rectangle_t *that );

/*!
 *  \brief determines if the given rectangle contains the other
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to others object attributes
 *  \return false if the rectangle does not contain the other. (touching is containing)
 */
static inline bool geometry_rectangle_is_containing ( const geometry_rectangle_t *this_, const geometry_rectangle_t *that );

/*!
 *  \brief determines the chess-distance/manhattan-distance of a given coordinate to this geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate
 *  \param y y coordinate
 *  \return chess-distance from x/y to the rectangle; 0.0 if x/y is contained.
 */
static inline double geometry_rectangle_calc_chess_distance ( const geometry_rectangle_t *this_, double x, double y );

/*!
 *  \brief moves the geometry_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \param delta_x delta-x difference by which to shift the rectangle
 *  \param delta_y delta-y difference by which to shift the rectangle
 */
static inline void geometry_rectangle_shift ( geometry_rectangle_t *this_, double delta_x, double delta_y );

/*!
 *  \brief expands or shrinks the geometry_rectangle_t
 *
 *  The top left corner remains fix.
 *
 *  \param this_ pointer to own object attributes
 *  \param delta_width value by which to increase the width. In case of a negative value, the rectangle shrinks (but not below 0).
 *  \param delta_height value by which to increase the height. In case of a negative value, the rectangle shrinks (but not below 0).
 */
static inline void geometry_rectangle_expand ( geometry_rectangle_t *this_, double delta_width, double delta_height );

/*!
 *  \brief prints the geometry_rectangle_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_rectangle_trace ( const geometry_rectangle_t *this_ );

#include "util/geometry/geometry_rectangle.inl"

#endif  /* GEOMETRY_RECTANGLE_H */


/*
Copyright 2016-2019 Andreas Warnke

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
