/* File: draw_classifier_label.h; Copyright and License: see below */

#ifndef DRAW_CLASSIFIER_LABEL_H
#define DRAW_CLASSIFIER_LABEL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws titles, stereotypes, underscores; performs linebreaks
 *
 *  This class does not show stereotype images, only their textual representation.
 */

#include "draw/draw_line_breaker.h"
#include "pencil_size.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "data_rules.h"
#include "set/data_visible_classifier.h"
#include "utf8stream/utf8stream_writemem.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw label functions
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct draw_classifier_label_struct {
    char text_buffer[ ( DATA_CLASSIFIER_MAX_NAME_SIZE + 1 ) * 4 ];  /*!< +1 for colon, x4 because any character may be followed by a 3-byte space */
    utf8stream_writemem_t text_builder;  /*!< a pair of utf8stream_writer_t and universal_memory_output_stream_t to build an output text */
    draw_line_breaker_t linebr;  /*!< An object to insert zero-width spaces (a 3 byte utf8 sequence) wherever a linebreak is allowed */
};

typedef struct draw_classifier_label_struct draw_classifier_label_t;

/*!
 *  \brief initializes the draw_classifier_label_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_classifier_label_init( draw_classifier_label_t *this_ );

/*!
 *  \brief destroys the draw_classifier_label_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_classifier_label_destroy( draw_classifier_label_t *this_ );

/*!
 *  \brief determines the dimensions of the stereotype-as-text and name of the classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier the visible_classifier consisting of diagramelement and classifier to draw
 *  \param with_stereotype false if no stereotype label shall be printed because an icon is shown instead
 *  \param proposed_bounds proposed bounds for the text width and height
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_label_dim width and height of the type and name is returned. NULL is not permitted.
 */
void draw_classifier_label_get_stereotype_and_name_dimensions( draw_classifier_label_t *this_,
                                                               const data_visible_classifier_t *visible_classifier,
                                                               bool with_stereotype,
                                                               const geometry_dimensions_t *proposed_bounds,
                                                               const pencil_size_t *pencil_size,
                                                               PangoLayout *font_layout,
                                                               geometry_dimensions_t *out_label_dim
                                                             );

/*!
 *  \brief renders stereotype-as-text and name of the classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier the visible_classifier consisting of diagramelement and classifier to draw
 *  \param with_stereotype false if no stereotype label shall be printed because an icon is shown instead
 *  \param color color to draw text
 *  \param label_box the rectangle where to draw to
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param cr the cairo drawing context.
 */
void draw_classifier_label_draw_stereotype_and_name( draw_classifier_label_t *this_,
                                                     const data_visible_classifier_t *visible_classifier,
                                                     bool with_stereotype,
                                                     const GdkRGBA *color,
                                                     const geometry_rectangle_t *label_box,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     cairo_t *cr
                                                   );

#endif  /* DRAW_CLASSIFIER_LABEL_H */


/*
Copyright 2019-2025 Andreas Warnke

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
