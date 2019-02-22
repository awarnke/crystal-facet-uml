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
 *  \brief calculates the bounding rectangle for component icon
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \return bounding box rectangle of the icon
 */
geometry_rectangle_t draw_symbol_get_component_bounds ( const draw_symbol_t *this_,
                                                        double x,
                                                        double y,
                                                        geometry_h_align_t h_align,
                                                        geometry_v_align_t v_align,
                                                        double height
                                                      );

/*!
 *  \brief draws the component icon into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the icon
 *  \param cr a cairo drawing context
 */
void draw_symbol_draw_component ( const draw_symbol_t *this_,
                                  geometry_rectangle_t bounds,
                                  cairo_t *cr
                                );

/*!
 *  \brief calculates the bounding rectangle for the artifact icon
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \return bounding box rectangle of the icon
 */
geometry_rectangle_t draw_symbol_get_artifact_bounds ( const draw_symbol_t *this_,
                                                       double x,
                                                       double y,
                                                       geometry_h_align_t h_align,
                                                       geometry_v_align_t v_align,
                                                       double height
                                                     );

/*!
 *  \brief draws the artifact icon into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the icon
 *  \param cr a cairo drawing context
 */
void draw_symbol_draw_artifact ( const draw_symbol_t *this_,
                                  geometry_rectangle_t bounds,
                                  cairo_t *cr
                                );

/*!
 *  \brief calculates the bounding rectangle for the actor icon
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \return bounding box rectangle of the icon
 */
geometry_rectangle_t draw_symbol_get_actor_bounds ( const draw_symbol_t *this_,
                                                    double x,
                                                    double y,
                                                    geometry_h_align_t h_align,
                                                    geometry_v_align_t v_align,
                                                    double height
                                                  );

/*!
 *  \brief draws the actor icon into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the icon
 *  \param cr a cairo drawing context
 */
void draw_symbol_draw_actor ( const draw_symbol_t *this_,
                              geometry_rectangle_t bounds,
                              cairo_t *cr
                            );

/*!
 *  \brief calculates the bounding rectangle for the circle icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \return bounding box rectangle of the icon
 */
geometry_rectangle_t draw_symbol_get_circle_bounds ( const draw_symbol_t *this_,
                                                     double x,
                                                     double y,
                                                     geometry_h_align_t h_align,
                                                     geometry_v_align_t v_align,
                                                     double height
                                                   );

/*!
 *  \brief draws the circle icon into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the icon
 *  \param pencil_size a set of size-values used for drawing elements in this diagram
 *  \param stroke true if the outer circle line shall be drawn
 *  \param fill true if the inner circle area shall be filled
 *  \param shallow_history true if the H character shall be drawn
 *  \param deep_history true if the H* character pir shall be drawn
 *  \param cr a cairo drawing context
 */
void draw_symbol_draw_circle ( const draw_symbol_t *this_,
                               geometry_rectangle_t bounds,
                               const pencil_size_t *pencil_size,
                               bool stroke,
                               bool fill,
                               bool shallow_history,
                               bool deep_history,
                               cairo_t *cr
                             );

/*!
 *  \brief calculates the bounding rectangle for the time icon
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \return bounding box rectangle of the icon
 */
geometry_rectangle_t draw_symbol_get_time_bounds ( const draw_symbol_t *this_,
                                                   double x,
                                                   double y,
                                                   geometry_h_align_t h_align,
                                                   geometry_v_align_t v_align,
                                                   double height
                                                 );

/*!
 *  \brief draws the time icon into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the icon
 *  \param cr a cairo drawing context
 */
void draw_symbol_draw_time ( const draw_symbol_t *this_,
                             geometry_rectangle_t bounds,
                             cairo_t *cr
                           );

/*!
 *  \brief calculates the bounding rectangle for the sync icon
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param pencil_size a set of size-values used for drawing elements in this diagram
 *  \return bounding box rectangle of the icon
 */
geometry_rectangle_t draw_symbol_get_sync_bounds ( const draw_symbol_t *this_,
                                                   double x,
                                                   double y,
                                                   geometry_h_align_t h_align,
                                                   geometry_v_align_t v_align,
                                                   double height,
                                                   const pencil_size_t *pencil_size
                                                 );

/*!
 *  \brief draws the sync icon into the bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the icon
 *  \param cr a cairo drawing context
 */
void draw_symbol_draw_sync ( const draw_symbol_t *this_,
                             geometry_rectangle_t bounds,
                             cairo_t *cr
                           );

#include "draw_symbol.inl"

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
