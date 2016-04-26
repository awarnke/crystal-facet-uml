/* File: gui_sketch_marker.h; Copyright and License: see below */

#ifndef GUI_SKETCH_MARKER_H
#define GUI_SKETCH_MARKER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief References objects that shall be marked (displayed as selected/focused/highlighted)
 */

#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch marker
 */
struct gui_sketch_marker_struct {
    data_id_t mark_focused;  /*!<  references the one focused object */
    data_id_t mark_highlighted;  /*!<  references the one highlighted/mouse over object */
    data_small_set_t mark_selected;  /*!<  references all selected objects */
};

typedef struct gui_sketch_marker_struct gui_sketch_marker_t;

/*!
 *  \brief initializes the gui_sketch_marker_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_init ( gui_sketch_marker_t *this_ );

/*!
 *  \brief destroys the gui_sketch_marker_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_destroy ( gui_sketch_marker_t *this_ );

#include "gui_sketch_marker.inl"

#endif  /* GUI_SKETCH_MARKER_H */


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
