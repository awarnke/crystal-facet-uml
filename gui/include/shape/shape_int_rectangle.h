/* File: shape_int_rectangle.h; Copyright and License: see below */

#ifndef SHAPE_INT_RECTANGLE_H
#define SHAPE_INT_RECTANGLE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores integer coordinates of a rectangle
 */

#include "shape/shape_h_align.h"
#include "shape/shape_v_align.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of a rectangle
 */
struct shape_int_rectangle_struct {
    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;
};

typedef struct shape_int_rectangle_struct shape_int_rectangle_t;

/*!
 *  \brief initializes the shape_int_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param left left coordinate of the rectangle
 *  \param top top coordinate of the rectangle
 *  \param width width of the rectangle in pixels
 *  \param height height of the rectangle in pixels
 */
static inline void shape_int_rectangle_init ( shape_int_rectangle_t *this_, int32_t left, int32_t top, uint32_t width, uint32_t height );

/*!
 *  \brief initializes the shape_int_rectangle_t struct to zeros
 *
 *  \param this_ pointer to own object attributes
 */
static inline void shape_int_rectangle_init_empty ( shape_int_rectangle_t *this_ );

/*!
 *  \brief copies a data_id_t struct to this uninitialized data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes
 */
static inline void shape_int_rectangle_copy ( shape_int_rectangle_t *this_, const shape_int_rectangle_t *that );

/*!
 *  \brief overwrites the shape_int_rectangle_t struct by that
 *
 *  \param this_ pointer to own object attributes
 *  \param that shape_int_rectangle_t top copy
 */
static inline void shape_int_rectangle_replace ( shape_int_rectangle_t *this_, const shape_int_rectangle_t *that );

/*!
 *  \brief re-initializes the shape_int_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param left left coordinate of the rectangle
 *  \param top top coordinate of the rectangle
 *  \param width width of the rectangle in pixels
 *  \param height height of the rectangle in pixels
 */
static inline void shape_int_rectangle_reinit ( shape_int_rectangle_t *this_, int32_t left, int32_t top, uint32_t width, uint32_t height );

/*!
 *  \brief initializes the shape_int_rectangle_t struct by the minimum bounding box of a and b (similar to a convex hull)
 *
 *  It is valid if parameters this_ and/or rect_a and/or rect_b are identical (same pointer).
 *
 *  \param this_ pointer to own object attributes
 *  \param rect_a rectangle a. Must not be NULL
 *  \param rect_b rectangle b. Must not be NULL
 */
static inline void shape_int_rectangle_init_by_bounds ( shape_int_rectangle_t *this_,
                                                        const shape_int_rectangle_t *rect_a,
                                                        const shape_int_rectangle_t *rect_b
                                                      );

/*!
 *  \brief destroys the shape_int_rectangle_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void shape_int_rectangle_destroy ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the left coordinate of shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline int32_t shape_int_rectangle_get_left ( const shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the top coordinate of shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline int32_t shape_int_rectangle_get_top ( const shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the right coordinates of shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \return left + width. We assume a coordinate system where integers denote pixel borders. 0 [pixel] 1 [pixel] 2 [pixel] 3 ...
 */
static inline int32_t shape_int_rectangle_get_right ( const shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the bottom coordinate of shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \return top + height. We assume a coordinate system where integers denote pixel borders. 0 [pixel] 1 [pixel] 2 [pixel] 3 ...
 */
static inline int32_t shape_int_rectangle_get_bottom ( const shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the width of shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t shape_int_rectangle_get_width ( const shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the height of shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t shape_int_rectangle_get_height ( const shape_int_rectangle_t *this_ );

/*!
 *  \brief determines if a given coordinate is within shape_int_rectangle_t
 *
 *  \param this_ pointer to own object attributes
 *  \param x a coordinate on pixel borders. 0 [pixel] 1 [pixel] 2 [pixel] 3 ...
 *  \param y a coordinate on pixel borders. 0 [pixel] 1 [pixel] 2 [pixel] 3 ...
 *  \return true if the pixel to the bottom-right of the denoted coordinate is within the rectangle.
 */
static inline bool shape_int_rectangle_contains ( const shape_int_rectangle_t *this_, int32_t x, int32_t y );

/*!
 *  \brief shrinks the rectangle by border
 *
 *  \param this_ pointer to own object attributes
 *  \param border number of pixels to be shrinked. use negative values to expand.
 */
static inline void shape_int_rectangle_shrink_by_border ( shape_int_rectangle_t *this_, int32_t border );

/*!
 *  \brief shrinks the rectangle to be of specified width to height ratio
 *
 *  \param this_ pointer to own object attributes
 *  \param ratio_width example width to specify the ratio. This is not the new width - it is just a factor to determine the new ratio.
 *  \param ratio_height example height to specify the ratio. This is not the new height - it is just a factor to determine the new height.
 *  \param h_align this parameter controls where in the original rectangle, the newly shrinked rectangle is located horizontal.
 *  \param v_align this parameter controls where in the original rectangle, the newly shrinked rectangle is located vertical.
 */
static inline void shape_int_rectangle_shrink_to_ratio ( shape_int_rectangle_t *this_,
                                                         uint32_t ratio_width,
                                                         uint32_t ratio_height,
                                                         shape_h_align_t h_align,
                                                         shape_v_align_t v_align
);

/*!
 *  \brief prints the shape_int_rectangle_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void shape_int_rectangle_trace ( const shape_int_rectangle_t *this_ );

#include "shape/shape_int_rectangle.inl"

#endif  /* SHAPE_INT_RECTANGLE_H */


/*
Copyright 2016-2026 Andreas Warnke

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
