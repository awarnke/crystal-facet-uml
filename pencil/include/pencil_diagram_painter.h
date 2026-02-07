/* File: pencil_diagram_painter.h; Copyright and License: see below */

#ifndef PENCIL_DIAGRAM_PAINTER_H
#define PENCIL_DIAGRAM_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a diagram into a cairo drawing context
 */

#include "pencil_marker.h"
#include "pencil_size.h"
#include "layout/layout_diagram.h"
#include "geometry/geometry_rectangle.h"
#include "draw/draw_diagram_label.h"
#include "set/data_profile_part.h"
#include "set/data_small_set.h"
#include "set/data_visible_set.h"
#include "entity/data_diagram.h"
#include "entity/data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the diagram painter
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_diagram_painter_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
    draw_diagram_label_t draw_diagram_label;  /*!< collection of draw label functions */
};

typedef struct pencil_diagram_painter_struct pencil_diagram_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_diagram pointer to the the layout-information and data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   const layout_diagram_t *layouted_diagram,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   const data_profile_part_t *profile,
                                   const pencil_size_t *pencil_size,
                                   PangoLayout *font_layout,
                                   cairo_t *cr
                                 );

/*!
 *  \brief determines the label box and inner drawing space for contained classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param the_diagram pointer to the data to be drawn
 *  \param diagram_bounds the destination rectangle where to draw the diagram
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout structure to layout fonts
 *  \param io_layout_diagram bounds, draw_area and label_box are updated. The pointer must not be NULL.
 */
void pencil_diagram_painter_do_layout ( pencil_diagram_painter_t *this_,
                                        const data_diagram_t *the_diagram,
                                        const geometry_rectangle_t *diagram_bounds,
                                        const data_profile_part_t *profile,
                                        const pencil_size_t *pencil_size,
                                        PangoLayout *font_layout,
                                        layout_diagram_t *io_layout_diagram
                                      );

#endif  /* PENCIL_DIAGRAM_PAINTER_H */


/*
Copyright 2017-2026 Andreas Warnke

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
