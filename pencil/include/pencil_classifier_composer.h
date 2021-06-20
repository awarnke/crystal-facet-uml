/* File: pencil_classifier_composer.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_COMPOSER_H
#define PENCIL_CLASSIFIER_COMPOSER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a classifier by composing it of draw_graphics, draw_classifier_symbol and draw_classifier_label commands
 */

#include "pencil_marker.h"
#include "pencil_size.h"
#include "pencil_feature_painter.h"
#include "pencil_layout_data.h"
#include "layout/layout_visible_classifier.h"
#include "draw/draw_classifier_symbol.h"
#include "draw/draw_classifier_label.h"
#include "draw/draw_classifier_contour.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_dimensions.h"
#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "data_id.h"
#include "data_rules.h"
#include "set/data_visible_set.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the classifier painter
 */
struct pencil_classifier_composer_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
    data_rules_t data_rules;  /*!< own instance of data rules */
    draw_classifier_symbol_t draw_classifier_symbol;  /*!< collection of draw symbol/icon functions */
    draw_classifier_label_t draw_classifier_label;  /*!< collection of draw label functions */
    draw_classifier_contour_t draw_classifier_contour;  /*!< collection of draw contour functions */
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
 *  \param layout_data pointer to the diagrams layout-information needed to calculate the feature compartments
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_classifier_composer_draw ( const pencil_classifier_composer_t *this_,
                                       const layout_visible_classifier_t *layouted_classifier,
                                       data_id_t mark_focused,
                                       data_id_t mark_highlighted,
                                       const data_small_set_t *mark_selected,
                                       const pencil_layout_data_t *layout_data,
                                       const pencil_size_t *pencil_size,
                                       PangoLayout *font_layout,
                                       cairo_t *cr
                                     );

/*!
 *  \brief determines the minumum classifier bounds
 *
 *  without contained features, without contained classifiers.
 *
 *  This method returns values to embrace title and ornaments.
 *
 *  \param this_ pointer to own object attributes
 *  \param proposed_bounds proposed bounds calculated for aesthetics
 *  \param minimum_feature_space minimum space needed by the contained features
 *  \param shows_contained_children true if the classifier has contained children (needed for uml package because the symbol differs)
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param io_classifier_layout output is symbol box, space and label_box. Must not be NULL.
 */
void pencil_classifier_composer_set_all_bounds ( const pencil_classifier_composer_t *this_,
                                                 const geometry_dimensions_t *proposed_bounds,
                                                 const geometry_dimensions_t *minimum_feature_space,
                                                 bool shows_contained_children,
                                                 const pencil_size_t *pencil_size,
                                                 PangoLayout *font_layout,
                                                 layout_visible_classifier_t *io_classifier_layout
                                               );

/*!
 *  \brief recalculates symbol_box and label_box based on new inner_space
 *
 *  Precondition: the objects coordinates need to be valid already. This is just a partly recalculation.
 *
 *  \param this_ pointer to own object attributes
 *  \param inner_space exact new inner space
 *  \param shows_contained_children true if the classifier has contained children (needed for uml package because the symbol differs)
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param io_classifier_layout classifier layout of which the box coordinates shall be modified. Must not be NULL.
 */
void pencil_classifier_composer_expand_inner_space ( const pencil_classifier_composer_t *this_,
                                                     const geometry_rectangle_t *inner_space,
                                                     bool shows_contained_children,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     layout_visible_classifier_t *io_classifier_layout
                                                   );

/*!
 *  \brief determines the inner drawing space for contained classifiers and features - and the label_box
 *
 *  This method returns values to embrace title and ornaments. classifiers symbol_box is expected to be fix.
 *
 *  \param this_ pointer to own object attributes
 *  \param shows_contained_children true if the classifier has contained children (needed for uml package because the symbol differs)
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param io_classifier_layout input is symbol box, output is space and label_box. Must not be NULL.
 */
void pencil_classifier_composer_calc_space_and_label ( const pencil_classifier_composer_t *this_,
                                                       bool shows_contained_children,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       layout_visible_classifier_t *io_classifier_layout
                                                     );

/*!
 *  \brief draws feature compartments of the given classifier into the classifier_space area
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_classifier pointer to the classifiers layout-information and data to be drawn
 *  \param layout_data pointer to the diagrams layout-information needed to calculate the feature compartments
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void pencil_classifier_composer_private_draw_feature_compartments ( const pencil_classifier_composer_t *this_,
                                                                    const layout_visible_classifier_t *layouted_classifier,
                                                                    const pencil_layout_data_t *layout_data,
                                                                    const pencil_size_t *pencil_size,
                                                                    cairo_t *cr
                                                                  );

#endif  /* PENCIL_CLASSIFIER_COMPOSER_H */


/*
Copyright 2016-2021 Andreas Warnke

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
