/* File: pencil_classifier_painter.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_PAINTER_H
#define PENCIL_CLASSIFIER_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a classifier by composing it of draw_graphics, draw_classifier_icon and draw_classifier_label commands
 */

#include "pencil_marker.h"
#include "pencil_size.h"
#include "pencil_feature_painter.h"
#include "layout/layout_visible_set.h"
#include "layout/layout_visible_classifier.h"
#include "draw/draw_classifier_icon.h"
#include "draw/draw_classifier_label.h"
#include "draw/draw_classifier_contour.h"
#include "draw/draw_stereotype_image.h"
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
 *  \brief attributes of the classifier painter
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_classifier_painter_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
    draw_classifier_icon_t draw_classifier_icon;  /*!< collection of draw symbol/icon functions */
    draw_classifier_label_t draw_classifier_label;  /*!< collection of draw label functions */
    draw_classifier_contour_t draw_classifier_contour;  /*!< collection of draw contour functions */
    draw_stereotype_image_t draw_stereotype_image;  /*!< collection of draw functions for custom stereotype images */
};

typedef struct pencil_classifier_painter_struct pencil_classifier_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_classifier pointer to the layout-information and data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param layout_data pointer to the diagrams layout-information needed to calculate the feature compartments
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_classifier_painter_draw ( pencil_classifier_painter_t *this_,
                                      const layout_visible_classifier_t *layouted_classifier,
                                      data_id_t mark_focused,
                                      data_id_t mark_highlighted,
                                      const data_small_set_t *mark_selected,
                                      const layout_visible_set_t *layout_data,
                                      const data_profile_part_t *profile,
                                      const pencil_size_t *pencil_size,
                                      PangoLayout *font_layout,
                                      cairo_t *cr
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
void pencil_classifier_painter_private_draw_feature_compartments ( const pencil_classifier_painter_t *this_,
                                                                   const layout_visible_classifier_t *layouted_classifier,
                                                                   const layout_visible_set_t *layout_data,
                                                                   const pencil_size_t *pencil_size,
                                                                   cairo_t *cr
                                                                 );

#endif  /* PENCIL_CLASSIFIER_PAINTER_H */


/*
Copyright 2016-2024 Andreas Warnke

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
