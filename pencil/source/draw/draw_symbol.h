/* File: draw_symbol.h; Copyright and License: see below */

#ifndef DRAW_SYMBOL_H
#define DRAW_SYMBOL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a feature into a cairo drawing context
 */

#include "pencil_size.h"
#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "util/geometry/geometry_rectangle.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw symbol functions
 */
struct draw_symbol_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_symbol_struct draw_symbol_t;

/*!
 *  \brief initializes the draw_symbol_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_symbol_init( draw_symbol_t *this_ );

/*!
 *  \brief destroys the draw_symbol_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_symbol_destroy( draw_symbol_t *this_ );

/*!
 *  \brief draws the component icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param cr a cairo drawing context
 *  \param out_width width of the drawn icon to be returned. NULL is not allowed.
 */
void draw_symbol_draw_component_icon ( const draw_symbol_t *this_,
                                       double x,
                                       double y,
                                       geometry_h_align_t h_align,
                                       geometry_v_align_t v_align,
                                       double height,
                                       cairo_t *cr,
                                       double *out_width
                                     );

/*!
 *  \brief draws the artifact icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param cr a cairo drawing context
 *  \param out_width width of the drawn icon to be returned. NULL is not allowed.
 */
void draw_symbol_draw_artifact_icon ( const draw_symbol_t *this_,
                                      double x,
                                      double y,
                                      geometry_h_align_t h_align,
                                      geometry_v_align_t v_align,
                                      double height,
                                      cairo_t *cr,
                                      double *out_width
                                    );

/*!
 *  \brief draws the actor icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param cr a cairo drawing context
 *  \param out_width width of the drawn icon to be returned. NULL is not allowed.
 */
void draw_symbol_draw_actor_icon ( const draw_symbol_t *this_,
                                   double x,
                                   double y,
                                   geometry_h_align_t h_align,
                                   geometry_v_align_t v_align,
                                   double height,
                                   cairo_t *cr,
                                   double *out_width
                                 );

#endif  /* DRAW_SYMBOL_H */


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
