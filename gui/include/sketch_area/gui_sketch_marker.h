/* File: gui_sketch_marker.h; Copyright and License: see below */

#ifndef GUI_SKETCH_MARKER_H
#define GUI_SKETCH_MARKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Marks objects by colors and/or lines/ornaments
 */

#include "gui_marked_set.h"
#include "util/shape/shape_int_rectangle.h"
#include "pencil_marker.h"
#include "util/geometry/geometry_rectangle.h"
#include "set/data_small_set.h"
#include "data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the marker
 */
struct gui_sketch_marker_struct {
    bool with_pencil_markers;
    pencil_marker_t pencil_marker;
};

typedef struct gui_sketch_marker_struct gui_sketch_marker_t;

/*!
 *  \brief initializes the marker
 *
 *  \param this_ pointer to own object attributes
 *  \param with_pencil_markers true if pencil markers shall also be drawn,
 *                             false if not, e.g. when pencil is called anyway.
 */
static inline void gui_sketch_marker_init( gui_sketch_marker_t *this_, bool with_pencil_markers );

/*!
 *  \brief destroys the marker
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_destroy( gui_sketch_marker_t *this_ );

/*!
 *  \brief draws markers around the destination area of the cairo drawing context
 *         and sets the drawing color of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param element_id id of the object to draw
 *  \param marked_set set of objects to be marked
 *  \param rect the rectangle which to mark. Drawings may be done outside.
 *  \param cr a cairo drawing context
 */
void gui_sketch_marker_prepare_draw ( const gui_sketch_marker_t *this_,
                                      data_id_t element_id,
                                      gui_marked_set_t *marked_set,
                                      shape_int_rectangle_t rect,
                                      cairo_t *cr
                                    );

#include "gui_sketch_marker.inl"

#endif  /* GUI_SKETCH_MARKER_H */


/*
Copyright 2018-2021 Andreas Warnke

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
