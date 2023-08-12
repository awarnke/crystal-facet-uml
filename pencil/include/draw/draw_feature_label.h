/* File: draw_feature_label.h; Copyright and License: see below */

#ifndef DRAW_FEATURE_LABEL_H
#define DRAW_FEATURE_LABEL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws keys and values of features
 */

#include "pencil_size.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "data_feature.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw label functions
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct draw_feature_label_struct {
    int dummy;
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
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_text_width width of the text is returned. NULL is not allowed.
 *  \param out_text_height height of the text is returned. NULL is not allowed.
 */
void draw_feature_label_get_key_and_value_dimensions ( const draw_feature_label_t *this_,
                                                       const data_feature_t *feature,
                                                       /* const data_profile_part_t *profile, */
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       double *out_text_width,
                                                       double *out_text_height
                                                     );

/*!
 *  \brief renders key and value of the feature
 *
 *  \param this_ pointer to own object attributes
 *  \param feature the feature data to draw
 *  \param label_box the rectangle where to draw to
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param cr the cairo drawing context.
 */
void draw_feature_label_draw_key_and_value ( const draw_feature_label_t *this_,
                                             const data_feature_t *feature,
                                             /* const data_profile_part_t *profile, */
                                             const geometry_rectangle_t *label_box,
                                             const pencil_size_t *pencil_size,
                                             PangoLayout *font_layout,
                                             cairo_t *cr
                                           );

#include "draw_feature_label.inl"

#endif  /* DRAW_FEATURE_LABEL_H */


/*
Copyright 2019-2023 Andreas Warnke

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
