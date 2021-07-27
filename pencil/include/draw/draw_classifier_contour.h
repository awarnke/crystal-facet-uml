/* File: draw_classifier_contour.h; Copyright and License: see below */

#ifndef DRAW_CLASSIFIER_CONTOUR_H
#define DRAW_CLASSIFIER_CONTOUR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws simple geometric objects like 2d boxes, 3d boxes, ovals, rounded-corner-boxes
 */

#include "pencil_size.h"
#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_classifier_type.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw geometry functions
 */
struct draw_classifier_contour_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_classifier_contour_struct draw_classifier_contour_t;

/*!
 *  \brief initializes the draw_classifier_contour_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_classifier_contour_init( draw_classifier_contour_t *this_ );

/*!
 *  \brief destroys the draw_classifier_contour_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_classifier_contour_destroy( draw_classifier_contour_t *this_ );

/*!
 *  \brief determines the dimensions of the inner area (label and space) of the classifier-shape.
 *
 *  based on the outer_bounds, the inner area is calculated.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the classifier to layout
 *  \param outer_bounds outer bounding rectangle for the classifier
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \result inner area. empty rectangle if outer_bounds too small.
 */
geometry_rectangle_t draw_classifier_contour_calc_inner_area ( const draw_classifier_contour_t *this_,
                                                               data_classifier_type_t classifier_type,
                                                               const geometry_rectangle_t *outer_bounds,
                                                               const pencil_size_t *pencil_size
                                                             );

/*!
 *  \brief determines the outer bounding dimensions of the classifier-shape.
 *
 *  based on the inner_area (label and space), the outer bounds is calculated.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the classifier to layout
 *  \param inner_area inner_area (label and space) that is needed rom the classifier
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \result outer bounds.
 */
geometry_rectangle_t draw_classifier_contour_calc_outer_bounds ( const draw_classifier_contour_t *this_,
                                                                 data_classifier_type_t classifier_type,
                                                                 const geometry_rectangle_t *inner_area,
                                                                 const pencil_size_t *pencil_size
                                                               );

/*!
 *  \brief draws a rectangle shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the rectangle has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
static inline void draw_classifier_contour_draw_rect ( const draw_classifier_contour_t *this_,
                                                       const geometry_rectangle_t *outer_bounds,
                                                       const pencil_size_t *pencil_size,
                                                       cairo_t *cr
                                                     );

/*!
 *  \brief draws a rounded-corner-rectangle shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the rectangle has a distance of gap to the outer bounds
 *  \param dashed_line true if the line shall be dashed
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_rounded_rect ( const draw_classifier_contour_t *this_,
                                                 const geometry_rectangle_t *outer_bounds,
                                                 bool dashed_line,
                                                 const pencil_size_t *pencil_size,
                                                 cairo_t *cr
                                               );

/*!
 *  \brief draws an ellipse shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the ellipse has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_ellipse ( const draw_classifier_contour_t *this_,
                                            const geometry_rectangle_t *outer_bounds,
                                            const pencil_size_t *pencil_size,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws a rhombus shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the rhombus has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_rhombus ( const draw_classifier_contour_t *this_,
                                            const geometry_rectangle_t *outer_bounds,
                                            const pencil_size_t *pencil_size,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws an 3d_box shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the 3d_box has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_3d_box ( const draw_classifier_contour_t *this_,
                                           const geometry_rectangle_t *outer_bounds,
                                           const pencil_size_t *pencil_size,
                                           cairo_t *cr
                                         );

/*!
 *  \brief draws an accept_event shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the accept_event has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_accept_event ( const draw_classifier_contour_t *this_,
                                                 const geometry_rectangle_t *outer_bounds,
                                                 const pencil_size_t *pencil_size,
                                                 cairo_t *cr
                                               );

/*!
 *  \brief draws a send_signal shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the send_signal has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_send_signal ( const draw_classifier_contour_t *this_,
                                                const geometry_rectangle_t *outer_bounds,
                                                const pencil_size_t *pencil_size,
                                                cairo_t *cr
                                              );

/*!
 *  \brief draws a package shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the package has a distance of gap to the outer bounds
 *  \param label_box rectangle of the label text
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_package ( const draw_classifier_contour_t *this_,
                                            const geometry_rectangle_t *outer_bounds,
                                            const geometry_rectangle_t *label_box,
                                            const pencil_size_t *pencil_size,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws a diagram shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the diagram has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_diagram_ref ( const draw_classifier_contour_t *this_,
                                                const geometry_rectangle_t *outer_bounds,
                                                const pencil_size_t *pencil_size,
                                                cairo_t *cr
                                              );

/*!
 *  \brief draws a comment shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the comment has a distance of gap to the outer bounds
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_classifier_contour_draw_comment ( const draw_classifier_contour_t *this_,
                                            const geometry_rectangle_t *outer_bounds,
                                            const pencil_size_t *pencil_size,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws a horizontal line to e.g. split compartments of feature-groups
 *
 *  \param this_ pointer to own object attributes
 *  \param outer_bounds outer bounding rectangle of the shape, the line has a distance of gap to the outer bounds
 *  \param y_coordinate coordinate where to draw the line
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
static inline void draw_classifier_contour_draw_compartment_line ( const draw_classifier_contour_t *this_,
                                                                   const geometry_rectangle_t *outer_bounds,
                                                                   double y_coordinate,
                                                                   const pencil_size_t *pencil_size,
                                                                   cairo_t *cr
                                                                 );

#include "draw_classifier_contour.inl"

#endif  /* DRAW_CLASSIFIER_CONTOUR_H */


/*
Copyright 2019-2021 Andreas Warnke

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
