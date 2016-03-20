/* File: shape_int_rectangle.h; Copyright and License: see below */

#ifndef SHAPE_INT_RECTANGLE_H
#define SHAPE_INT_RECTANGLE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores integer coordinates of a rectangle
 */

#include "util/shape/shape_alignment.h"
#include <stdint.h>

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
 */
static inline void shape_int_rectangle_init ( shape_int_rectangle_t *this_, int32_t left, int32_t top, uint32_t width, uint32_t height );

/*!
 *  \brief destroys the shape_int_rectangle_t struct
 */
static inline void shape_int_rectangle_destroy ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the shape_int_rectangle_t
 */
static inline int32_t shape_int_rectangle_get_left ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the shape_int_rectangle_t
 */
static inline int32_t shape_int_rectangle_get_top ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the right coordinates of shape_int_rectangle_t
 *  \return left + width. We assume a coordinate system where integers denote pixel borders. 0 [pixel] 1 [pixel] 2 [pixel] 3 ...
 */
static inline int32_t shape_int_rectangle_get_right ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the bottom coordinate of shape_int_rectangle_t
 *  \return top + height. We assume a coordinate system where integers denote pixel borders. 0 [pixel] 1 [pixel] 2 [pixel] 3 ...
 */
static inline int32_t shape_int_rectangle_get_bottom ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the shape_int_rectangle_t
 */
static inline uint32_t shape_int_rectangle_get_width ( shape_int_rectangle_t *this_ );

/*!
 *  \brief gets the shape_int_rectangle_t
 */
static inline uint32_t shape_int_rectangle_get_height ( shape_int_rectangle_t *this_ );

/*!
 *  \brief shrinks the rectangle by border
 *  \param border number of pixels to be shrinked. use negative values to expand.
 */
static inline void shape_int_rectangle_shrink_by_border ( shape_int_rectangle_t *this_, int32_t border );

/*!
 *  \brief shrinks the rectangle to be of specified width to height ratio
 *  \param ratio_width example width to specify the ratio
 *  \param ratio_height example height to specify the ratio
 */
static inline void shape_int_rectangle_shrink_to_ratio ( shape_int_rectangle_t *this_, uint32_t ratio_width, uint32_t ratio_height, shape_alignment_t align );

/*!
 *  \brief prints the shape_int_rectangle_t struct to the trace output
 */
static inline void shape_int_rectangle_trace ( shape_int_rectangle_t *this_ );

#include "util/shape/shape_int_rectangle.inl"

#endif  /* SHAPE_INT_RECTANGLE_H */


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
