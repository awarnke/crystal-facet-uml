/* File: draw_stereotype_image.h; Copyright and License: see below */

#ifndef DRAW_STEREOTYPE_IMAGE_H
#define DRAW_STEREOTYPE_IMAGE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a classifier into a cairo drawing context
 */

#include "pencil_size.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "data_classifier_type.h"
#include <cairo.h>
#include <stdint.h>

/*! \brief aspect ratio of the artifact icon */
extern const double DRAW_STEREOTYPE_IMAGE_ARTIFACT_ICON_WIDTH_TO_HEIGHT;

/*!
 *  \brief attributes of the draw symbol functions
 */
struct draw_stereotype_image_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_stereotype_image_struct draw_stereotype_image_t;

/*!
 *  \brief initializes the draw_stereotype_image_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_stereotype_image_init( draw_stereotype_image_t *this_ );

/*!
 *  \brief destroys the draw_stereotype_image_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_stereotype_image_destroy( draw_stereotype_image_t *this_ );

/*!
 *  \brief returns true if the classifier is drawn as fixed-size symbol
 *
 *  The following classifier types are fixed-sized symbols: (use-case:) actor, (activity:) start, end, fork, join, timeout, history
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the classifier to draw
 *  \return id of the classifier
 */
static inline bool draw_stereotype_image_is_fix_sized_symbol ( const draw_stereotype_image_t *this_, data_classifier_type_t classifier_type );

/*!
 *  \brief determines the dimensions of the type icon.
 *
 *  Type icons are e.g. artifact-icon or component-icon.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the classifier to draw
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \return of the icon.
 */
static inline geometry_dimensions_t draw_stereotype_image_get_icon_dimensions ( const draw_stereotype_image_t *this_,
                                                                               data_classifier_type_t classifier_type,
                                                                               const pencil_size_t *pencil_size
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
static inline geometry_rectangle_t draw_stereotype_image_get_artifact_bounds ( const draw_stereotype_image_t *this_,
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
void draw_stereotype_image_draw_artifact ( const draw_stereotype_image_t *this_,
                                          geometry_rectangle_t bounds,
                                          cairo_t *cr
                                        );

#include "draw_stereotype_image.inl"

#endif  /* DRAW_STEREOTYPE_IMAGE_H */


/*
Copyright 2023-2023 Andreas Warnke

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
