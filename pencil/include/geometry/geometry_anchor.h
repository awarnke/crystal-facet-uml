/* File: geometry_anchor.h; Copyright and License: see below */

#ifndef GEOMETRY_ANCHOR_H
#define GEOMETRY_ANCHOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the coordinates of a reference point and an x/y alignment instruction
 *
 *  This structure allows to layout a geometric object
 *  by providing an anchor point and the information how the object
 *  shall be aligned relative to this anchor point
 */

#include "geometry/geometry_point.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include <stdbool.h>

/*!
 *  \brief attributes of an anchor
 */
struct geometry_anchor_struct {
    geometry_point_t reference_point;
    geometry_h_align_t x_align;  /*!< align the objects left or right to the reference point */
    geometry_v_align_t y_align;  /*!< align the objects top or bottmo to the reference point */
};

typedef struct geometry_anchor_struct geometry_anchor_t;

/*!
 *  \brief initializes the geometry_anchor_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the reference point
 *  \param y y coordinate of the reference point
 *  \param x_align horizontal alignment relative to the reference point
 *  \param y_align vertical alignment relative to the reference point
 */
static inline void geometry_anchor_init ( geometry_anchor_t *this_,
                                          double x,
                                          double y,
                                          geometry_h_align_t x_align,
                                          geometry_v_align_t y_align
                                        );

/*!
 *  \brief copies original to this uninitialized geometry_anchor_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_anchor_copy ( geometry_anchor_t *this_, const geometry_anchor_t *original );

/*!
 *  \brief replaces the already initialized geometry_anchor_t struct by other data
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes that shall be copied
 */
static inline void geometry_anchor_replace ( geometry_anchor_t *this_, const geometry_anchor_t *original );

/*!
 *  \brief destroys the geometry_anchor_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_anchor_destroy ( geometry_anchor_t *this_ );

/*!
 *  \brief gets the x coordinate of geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_anchor_get_x ( const geometry_anchor_t *this_ );

/*!
 *  \brief gets the y coordinate of geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline double geometry_anchor_get_y ( const geometry_anchor_t *this_ );

/*!
 *  \brief gets the reference point of geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the reference point
 */
static inline const geometry_point_t * geometry_anchor_get_point_const ( const geometry_anchor_t *this_ );

/*!
 *  \brief gets the horizontal alignment of geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline geometry_h_align_t geometry_anchor_get_x_align ( const geometry_anchor_t *this_ );

/*!
 *  \brief gets the vertical alignment coordinate of geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline geometry_v_align_t geometry_anchor_get_y_align ( const geometry_anchor_t *this_ );

/*!
 *  \brief aligns a rectangle to geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 *  \param unaligned geometry_rectangle to be aligned to this_ anchor
 *  \return geometry_rectangle aligned to anchor
 */
static inline geometry_rectangle_t geometry_anchor_align_rect ( const geometry_anchor_t *this_,
                                                                const geometry_rectangle_t *unaligned
                                                              );

/*!
 *  \brief aligns a dimension to coordinate of geometry_anchor_t
 *
 *  \param this_ pointer to own object attributes
 *  \param unaligned geometry_dimensions to be aligned to this_ anchor
 *  \return geometry_rectangle aligned to anchor
 */
static inline geometry_rectangle_t geometry_anchor_align_dim ( const geometry_anchor_t *this_,
                                                               const geometry_dimensions_t *unaligned
                                                             );

/*!
 *  \brief aligns a dimension to coordinate of geometry_anchor_t but bounded by fence
 *
 *  \param this_ pointer to own object attributes
 *  \param unaligned geometry_dimensions to be aligned to this_ anchor
 *  \param fence geometry_rectangle within which the result shall be located
 *  \return geometry_rectangle aligned to anchor
 */
static inline geometry_rectangle_t geometry_anchor_align_dim_bounded ( const geometry_anchor_t *this_,
                                                                       const geometry_dimensions_t *unaligned,
                                                                       const geometry_rectangle_t *fence
                                                                     );

/*!
 *  \brief prints the geometry_anchor_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_anchor_trace ( const geometry_anchor_t *this_ );

#include "geometry/geometry_anchor.inl"

#endif  /* GEOMETRY_ANCHOR_H */


/*
Copyright 2021-2025 Andreas Warnke

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
