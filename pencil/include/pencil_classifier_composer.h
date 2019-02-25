/* File: pencil_classifier_composer.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_COMPOSER_H
#define PENCIL_CLASSIFIER_COMPOSER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a classifier by composing it of draw_graphics, draw_symbol and draw_label commands
 */

#include "pencil_input_data.h"
#include "pencil_marker.h"
#include "pencil_size.h"
#include "pencil_feature_painter.h"
#include "layout/layout_visible_classifier.h"
#include "draw/draw_symbol.h"
#include "draw/draw_label.h"
#include "draw/draw_geometry.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_dimensions.h"
#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "data_diagram.h"
#include "util/id/data_small_set.h"
#include "data_id.h"
#include "data_rules.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the classifier painter
 */
struct pencil_classifier_composer_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
    data_rules_t data_rules;  /*!< own instance of data rules */
    draw_symbol_t draw_symbol;  /*!< collection of draw symbol/icon functions */
    draw_label_t draw_label;  /*!< collection of draw label functions */
    draw_geometry_t draw_geometry;  /*!< collection of draw geometry functions */
};

typedef struct pencil_classifier_composer_struct pencil_classifier_composer_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_composer_init( pencil_classifier_composer_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_composer_destroy( pencil_classifier_composer_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_classifier pointer to the layout-information and data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_classifier_composer_draw ( const pencil_classifier_composer_t *this_,
                                      layout_visible_classifier_t *layouted_classifier,
                                      data_id_t mark_focused,
                                      data_id_t mark_highlighted,
                                      const data_small_set_t *mark_selected,
                                      const pencil_size_t *pencil_size,
                                      PangoLayout *font_layout,
                                      cairo_t *cr
                                    );

/*!
 *  \brief determines the minumum classifier bounds
 *
 *  without contained features, without contained classifiers.
 *
 *  This method returns values assuming title and ornaments are included.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier pointer to the data to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param proposed_bounds proposed bounds calculated for aesthetics
 *  \param minimum_feature_space minimum space needed by the contained features
 *  \param io_classifier_layout output is bounds, space and label_box. Must not be NULL.
 */
void pencil_classifier_composer_set_all_bounds ( const pencil_classifier_composer_t *this_,
                                                const data_visible_classifier_t *visible_classifier,
                                                const pencil_size_t *pencil_size,
                                                PangoLayout *font_layout,
                                                const geometry_dimensions_t *proposed_bounds,
                                                const geometry_dimensions_t *minimum_feature_space,
                                                layout_visible_classifier_t *io_classifier_layout
                                              );

/*!
 *  \brief determines the inner drawing space for contained classifiers and features - and the label_box
 *
 *  This method returns values assuming title and ornaments are included. classifier_bounds is expected to be fix.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier pointer to the data to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param io_classifier_layout input is bounds, output is space and label_box. Must not be NULL.
 */
void pencil_classifier_composer_set_space_and_label ( const pencil_classifier_composer_t *this_,
                                                     const data_visible_classifier_t *visible_classifier,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     layout_visible_classifier_t *io_classifier_layout
                                                   );

#endif  /* PENCIL_CLASSIFIER_COMPOSER_H */


/*
Copyright 2016-2019 Andreas Warnke

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
