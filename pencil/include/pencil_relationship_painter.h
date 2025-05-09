/* File: pencil_relationship_painter.h; Copyright and License: see below */

#ifndef PENCIL_RELATIONSHIP_PAINTER_H
#define PENCIL_RELATIONSHIP_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a relationship into a cairo drawing context
 */

#include "pencil_marker.h"
#include "pencil_size.h"
#include "layout/layout_relationship.h"
#include "draw/draw_relationship_label.h"
#include "draw/draw_stereotype_icon.h"
#include "geometry/geometry_connector.h"
#include "set/data_profile_part.h"
#include "set/data_visible_set.h"
#include "set/data_profile_part.h"
#include "set/data_small_set.h"
#include "entity/data_diagram.h"
#include "entity/data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the relationship painter
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_relationship_painter_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
    draw_relationship_label_t draw_relationship_label;  /*!< collection of draw label functions */
};

typedef struct pencil_relationship_painter_struct pencil_relationship_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_painter_init( pencil_relationship_painter_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_painter_destroy( pencil_relationship_painter_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_relationship pointer to the layout-information and data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_relationship_painter_draw ( pencil_relationship_painter_t *this_,
                                        const layout_relationship_t *layouted_relationship,
                                        bool mark_focused,
                                        bool mark_highlighted,
                                        bool mark_selected,
                                        const data_profile_part_t *profile,
                                        const pencil_size_t *pencil_size,
                                        PangoLayout *layout,
                                        cairo_t *cr
                                      );

#endif  /* PENCIL_RELATIONSHIP_PAINTER_H */


/*
Copyright 2017-2025 Andreas Warnke

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
