/* File: pencil_classifier_composer.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_COMPOSER_H
#define PENCIL_CLASSIFIER_COMPOSER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a classifier by composing it of draw_graphics, draw_classifier_icon and draw_classifier_label commands
 */

#include "pencil_marker.h"
#include "pencil_size.h"
#include "pencil_feature_painter.h"
#include "pencil_error.h"
#include "layout/layout_visible_set.h"
#include "layout/layout_visible_classifier.h"
#include "draw/draw_classifier_icon.h"
#include "draw/draw_classifier_label.h"
#include "draw/draw_classifier_contour.h"
#include "draw/draw_stereotype_icon.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "entity/data_diagram.h"
#include "set/data_small_set.h"
#include "set/data_profile_part.h"
#include "set/data_visible_set.h"
#include "entity/data_id.h"
#include "data_rules.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the classifier-internal layouter
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 *
 *  The outer encompassing rectangle of the classifier is called
 *  - envelope
 *  .
 *
 *  There are two general layout schemes, based on the classifier type.
 *  Layouts with a contour (an outer border) follow the scheme
 *  - label + optional icon
 *  - space
 *  .
 *  whereas layouts with an icon follow the scheme
 *  - icon
 *  - label
 *  - space
 *  .
 *
 *  The icon is
 *  - either an indicator of the classifier type
 *  - or a graphical representation of the stereotype.
 *  .
 *
 *  The label consists of
 *  - an optional textual representation of the stereotype and
 *  - the name.
 *  .
 *
 *  The space consists of compartments:
 *  - properties
 *  - operations
 *  - tagged values
 *  - contained classifiers
 *  .
 */
struct pencil_classifier_composer_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
    draw_classifier_icon_t draw_classifier_icon;  /*!< collection of draw symbol/icon functions */
    draw_classifier_label_t draw_classifier_label;  /*!< collection of draw label functions */
    draw_classifier_contour_t draw_classifier_contour;  /*!< collection of draw contour functions */
    draw_stereotype_icon_t draw_stereotype_icon;  /*!< collection of draw functions for custom stereotype images */
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
 *  \brief recalculates symbol_box and label_box based on new space_and_label
 *
 *  Precondition: the objects coordinates need to be valid already. This is just a partly recalculation.
 *
 *  \param this_ pointer to own object attributes
 *  \param space exact new space
 *  \param shows_contained_children true if the classifier has contained children
 *                                  (needed for uml package because the symbol differs)
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param io_classifier_layout classifier layout of which the box coordinates shall be modified. Must not be NULL.
 *  \return PENCIL_ERROR_NONE in case of success,
 *          PENCIL_ERROR_OUT_OF_BOUNDS in case of additionally needed width by label text
 */
pencil_error_t pencil_classifier_composer_expand_space ( pencil_classifier_composer_t *this_,
                                                         const geometry_rectangle_t *space,
                                                         bool shows_contained_children,
                                                         const data_profile_part_t *profile,
                                                         const pencil_size_t *pencil_size,
                                                         PangoLayout *font_layout,
                                                         layout_visible_classifier_t *io_classifier_layout
                                                       );

/*!
 *  \brief resizes the contour, inner drawing space for contained classifiers and features - and the label_box
 *
 *  This method returns values to embrace title and ornaments. classifiers symbol_box is expected to be fix.
 *
 *  \param this_ pointer to own object attributes
 *  \param envelope exact new outer envelope box; should be big enough to contain the classifier
 *  \param shows_contained_children true if the classifier has contained children
 *                                  (needed for uml package because the symbol differs)
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param io_classifier_layout input is symbol box, output is space and label_box. Must not be NULL.
 *  \return PENCIL_ERROR_NONE in case of success,
 *          PENCIL_ERROR_OUT_OF_BOUNDS in case of additionally needed width or height
 */
pencil_error_t pencil_classifier_composer_set_envelope_box ( pencil_classifier_composer_t *this_,
                                                             const geometry_rectangle_t *envelope,
                                                             bool shows_contained_children,
                                                             const data_profile_part_t *profile,
                                                             const pencil_size_t *pencil_size,
                                                             PangoLayout *font_layout,
                                                             layout_visible_classifier_t *io_classifier_layout
                                                           );

/*!
 *  \brief determines the dimensions of the stereotype and name of the classifier.
 *
 *  Internally, this method determines also the classifier type and icon-dimensions to calculate the label alignment.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier the visible_classifier consisting of diagramelement and classifier to draw
 *  \param shows_contained_children true if the classifier has contained children
 *                                  (needed for uml package because the symbol differs)
 *  \param has_stereotype_icon true if no stereotype label shall be printed because an icon is shown instead
 *  \param space_and_label proposed rectangle for the position,
 *                         width and height of: label and icon and features and contained classifiers
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_label_box position and dimensions of the label box (smallest box containing stereotype-as-text and name)
 *  \param out_icon_box position and dimensions of the icon box of the stereotype icon (in case of has_stereotype_icon)
 *  \param out_label_compartment position and dimensions of label compartment
 *                               (width as the inner_area if fitting, may contain an icon)
 *  \return PENCIL_ERROR_NONE in case of success,
 *          PENCIL_ERROR_OUT_OF_BOUNDS in case of additionally needed width or height
 */
pencil_error_t pencil_classifier_composer_private_get_label_box ( pencil_classifier_composer_t *this_,
                                                                  const data_visible_classifier_t *visible_classifier,
                                                                  bool shows_contained_children,
                                                                  bool has_stereotype_icon,
                                                                  const geometry_rectangle_t *space_and_label,
                                                                  const pencil_size_t *pencil_size,
                                                                  PangoLayout *font_layout,
                                                                  geometry_rectangle_t *out_label_box,
                                                                  geometry_rectangle_t *out_icon_box,
                                                                  geometry_rectangle_t *out_label_compartment
                                                                );

#endif  /* PENCIL_CLASSIFIER_COMPOSER_H */


/*
Copyright 2016-2025 Andreas Warnke

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
