/* File: draw_classifier_contour.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void draw_classifier_contour_init( draw_classifier_contour_t *this_ )
{

}

static inline void draw_classifier_contour_destroy( draw_classifier_contour_t *this_ )
{

}

static inline void draw_classifier_contour_draw_rect ( const draw_classifier_contour_t *this_,
                                                       const geometry_rectangle_t *outer_bounds,
                                                       const pencil_size_t *pencil_size,
                                                       cairo_t *cr )
{
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double left = geometry_rectangle_get_left ( outer_bounds );
    const double top = geometry_rectangle_get_top ( outer_bounds );
    const double width = geometry_rectangle_get_width ( outer_bounds );
    const double height = geometry_rectangle_get_height ( outer_bounds );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    cairo_rectangle ( cr, left+gap, top+gap, width-gap-gap, height-gap-gap );
    cairo_stroke (cr);
}

static inline void draw_classifier_contour_draw_compartment_line ( const draw_classifier_contour_t *this_,
                                                                   const geometry_rectangle_t *outer_bounds,
                                                                   double y_coordinate,
                                                                   const pencil_size_t *pencil_size,
                                                                   cairo_t *cr )
{
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double left = geometry_rectangle_get_left ( outer_bounds );
    const double width = geometry_rectangle_get_width ( outer_bounds );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    cairo_move_to ( cr, left + gap, y_coordinate );
    cairo_line_to ( cr, left + width - gap, y_coordinate );
    cairo_stroke (cr);
}


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
