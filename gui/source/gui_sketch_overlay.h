/* File: gui_sketch_overlay.h; Copyright and License: see below */

#ifndef GUI_SKETCH_OVERLAY_H
#define GUI_SKETCH_OVERLAY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws graphical elements on top of diagrams
 */

#include "gui_sketch_drag_state.h"
#include "gui_sketch_tools.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the sketch overlay
 */
struct gui_sketch_overlay_struct {
    void* dummy;  /*!< no attributes */
};

typedef struct gui_sketch_overlay_struct gui_sketch_overlay_t;

/*!
 *  \brief initializes the sketch overlay
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_overlay_init( gui_sketch_overlay_t *this_ );

/*!
 *  \brief destroys the sketch overlay
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_overlay_destroy( gui_sketch_overlay_t *this_ );

/*!
 *  \brief draws overlay graphics to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param selected_tool currently selected tool/edit-mode
 *  \param drag_state current dragging status
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_draw( gui_sketch_overlay_t *this_,
                              gui_sketch_tools_tool_t selected_tool,
                              gui_sketch_drag_state_t *drag_state,
                              cairo_t *cr
                            );

#endif  /* GUI_SKETCH_OVERLAY_H */


/*
Copyright 2017-2017 Andreas Warnke

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
