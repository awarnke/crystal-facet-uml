/* File: draw_geometry.h; Copyright and License: see below */

#ifndef DRAW_GEOMETRY_H
#define DRAW_GEOMETRY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws simple geometric objects like 2d boxes, 3d boxes, ovals, rounded-corner-boxes
 */

#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "pencil_size.h"
#include "data_classifier_type.h"
#include "util/geometry/geometry_rectangle.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw geometry functions
 */
struct draw_geometry_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_geometry_struct draw_geometry_t;

/*!
 *  \brief initializes the draw_geometry_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_geometry_init( draw_geometry_t *this_ );

/*!
 *  \brief destroys the draw_geometry_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_geometry_destroy( draw_geometry_t *this_ );

/*!
 *  \brief determines the dimensions of the border of the classifier-shape.
 *
 *  E.g. a simple line border is 2.0 * pencil_size_get_standard_object_border()
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the classifier to draw
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param out_top_border height of the top border is returned. NULL is not allowed.
 *  \param out_left_border width of the left border is returned. NULL is not allowed.
 *  \param out_bottom_border height of the bottom border is returned. NULL is not allowed.
 *  \param out_right_border width of the right border is returned. NULL is not allowed.
 */
void draw_geometry_get_shape_border_dimensions( const draw_geometry_t *this_,
                                                data_classifier_type_t classifier_type,
                                                const pencil_size_t *pencil_size,
                                                double *out_top_border,
                                                double *out_left_border,
                                                double *out_bottom_border,
                                                double *out_right_border
                                              );

#endif  /* DRAW_GEOMETRY_H */


/*
Copyright 2019-2019 Andreas Warnke

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
