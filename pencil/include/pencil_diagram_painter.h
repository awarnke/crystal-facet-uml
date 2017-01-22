/* File: pencil_diagram_painter.h; Copyright and License: see below */

#ifndef PENCIL_DIAGRAM_PAINTER_H
#define PENCIL_DIAGRAM_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a diagram and its contents into a cairo drawing context
 */

#include "pencil_input_data.h"
#include "pencil_layouter.h"
#include "pencil_marker.h"
#include "pencil_classifier_painter.h"
#include "pencil_size.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include "data_id.h"
#include "universal_int32_pair.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the diagram painter
 */
struct pencil_diagram_painter_struct {
    pencil_input_data_t *input_data;  /*!< pointer to an external data cache */

    pencil_layouter_t layouter;  /* own instance of a layouter */

    pencil_classifier_painter_t classifier_painter;  /*!< own instance of a classifier painter */
    pencil_marker_t marker;  /*!< own instance of a marker */
};

typedef struct pencil_diagram_painter_struct pencil_diagram_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 */
static inline void pencil_diagram_painter_init( pencil_diagram_painter_t *this_, pencil_input_data_t *input_data );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ );

/*!
 *  \brief layouts the grid / defines coordinates
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 *  \param diagram_bounds the diagram_bounds rectangle where to draw the diagram
 */
static inline void pencil_diagram_painter_layout_grid ( pencil_diagram_painter_t *this_,
                                                        pencil_input_data_t *input_data,
                                                        geometry_rectangle_t diagram_bounds
                                                      );

/*!
 *  \brief layouts the chosen diagram contents into the diagram_bounds area
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_diagram_layout_elements ( pencil_diagram_painter_t *this_ );

/*!
 *  \brief draws the chosen diagram contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param cr a cairo drawing context
 */
void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   data_id_t mark_focused,
                                   data_id_t mark_highlighted,
                                   data_small_set_t *mark_selected,
                                   cairo_t *cr
                                 );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_diagram_painter_private_draw_classifiers ( pencil_diagram_painter_t *this_,
                                                       data_id_t mark_focused,
                                                       data_id_t mark_highlighted,
                                                       data_small_set_t *mark_selected,
                                                       PangoLayout *layout,
                                                       cairo_t *cr
                                                     );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param dereference true if the real, dereferenced object shall be returned (e.g. data_classifier_t or data_diagram_t),
 *                     false if the visible object shall be returned (e.g. data_diagramelement_t or data_diagram_t)
 *  \return an object id. The id is invalid if there is no object at the given location.
 */
static inline data_id_t pencil_diagram_painter_get_object_id_at_pos ( pencil_diagram_painter_t *this_,
                                                                      double x,
                                                                      double y,
                                                                      bool dereference
                                                                    );

/*!
 *  \brief gets the order values at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return the x- and y- order values as first and second element of the pair
 */
static inline universal_int32_pair_t pencil_diagram_painter_get_order_at_pos ( pencil_diagram_painter_t *this_,
                                                                               double x,
                                                                               double y
                                                                             );

#include "pencil_diagram_painter.inl"

#endif  /* PENCIL_DIAGRAM_PAINTER_H */


/*
Copyright 2016-2017 Andreas Warnke

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
