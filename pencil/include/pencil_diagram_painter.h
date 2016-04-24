/* File: pencil_diagram_painter.h; Copyright and License: see below */

#ifndef PENCIL_DIAGRAM_PAINTER_H
#define PENCIL_DIAGRAM_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a diagram and its contents into a cairo drawing context
 */

#include "pencil_input_data.h"
#include "pencil_classifier_painter.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include "data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the diagram painter
 */
struct pencil_diagram_painter_struct {
    pencil_classifier_painter_t classifier_painter;  /*!< own instance of a classifier painter */
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
 *  \brief draws the chosen diagram contents into the destination area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param cr a cairo drawing context
 *  \param destination the destination rectangle where to draw the diagram
 */
void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   pencil_input_data_t *input_data,
                                   data_id_t *mark_focused,
                                   data_id_t *mark_highlighted,
                                   data_small_set_t *mark_selected,
                                   cairo_t *cr,
                                   geometry_rectangle_t destination
                                 );

#endif  /* PENCIL_DIAGRAM_PAINTER_H */


/*
Copyright 2016-2016 Andreas Warnke

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
