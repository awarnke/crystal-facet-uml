/* File: draw_feature_label.h; Copyright and License: see below */

#ifndef DRAW_FEATURE_LABEL_H
#define DRAW_FEATURE_LABEL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws keys and values of features
 */

#include "draw/draw_stereotype_image.h"
#include "pencil_size.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "data_feature.h"
#include "set/data_profile_part.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw label functions
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct draw_feature_label_struct {
    draw_stereotype_image_t image_renderer;  /*!< own instance of stereotype image renderer */
};

typedef struct draw_feature_label_struct draw_feature_label_t;

/*!
 *  \brief initializes the draw_feature_label_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_feature_label_init( draw_feature_label_t *this_ );

/*!
 *  \brief destroys the draw_feature_label_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void draw_feature_label_destroy( draw_feature_label_t *this_ );

/*!
 *  \brief determines the dimensions of the key and value of the feature.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature the feature data to draw
 *  \param profile profile-part that provides the stereotype definition of the element to be drawn
 *  \param proposed_bounds proposed bounds for the text width and height
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_label_dim width and height of the type and name is returned. NULL is not permitted.
 */
void draw_feature_label_get_key_and_value_dimensions ( const draw_feature_label_t *this_,
                                                       const data_feature_t *feature,
                                                       const data_profile_part_t *profile,
                                                       const geometry_dimensions_t *proposed_bounds,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       geometry_dimensions_t *out_label_dim
                                                     );

/*!
 *  \brief renders key and value of the feature
 *
 *  \param this_ pointer to own object attributes
 *  \param feature the feature data to draw
 *  \param profile profile-part that provides the stereotype definition of the element to be drawn
 *  \param color color to draw text and to use as default for stereotype-image strokes
 *  \param label_box the rectangle where to draw to
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param cr the cairo drawing context.
 */
void draw_feature_label_draw_key_and_value ( const draw_feature_label_t *this_,
                                             const data_feature_t *feature,
                                             const data_profile_part_t *profile,
                                             const GdkRGBA *color,
                                             const geometry_rectangle_t *label_box,
                                             const pencil_size_t *pencil_size,
                                             PangoLayout *font_layout,
                                             cairo_t *cr
                                           );

#include "draw_feature_label.inl"

#endif  /* DRAW_FEATURE_LABEL_H */


/*
Copyright 2019-2024 Andreas Warnke

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
