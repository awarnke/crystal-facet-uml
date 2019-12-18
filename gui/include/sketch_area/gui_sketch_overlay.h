/* File: gui_sketch_overlay.h; Copyright and License: see below */

#ifndef GUI_SKETCH_OVERLAY_H
#define GUI_SKETCH_OVERLAY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws graphical elements on top of diagrams
 */

#include "sketch_area/gui_sketch_card.h"
#include "sketch_area/gui_sketch_nav_tree.h"
#include "sketch_area/gui_sketch_drag_state.h"
#include "gui_tool.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the sketch overlay
 */
struct gui_sketch_overlay_struct {
    double overlay_std_red;  /*!< standard overlay color */
    double overlay_std_green;  /*!< standard overlay color */
    double overlay_std_blue;  /*!< standard overlay color */
    double overlay_std_alpha;  /*!< standard overlay opaqueness */
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
 *  \param card_under_mouse the sketch card under the current mouse position or NULL if no card there
 *  \param nav_tree the navigation tree, not NULL
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_draw( gui_sketch_overlay_t *this_,
                              gui_tool_t selected_tool,
                              const gui_sketch_drag_state_t *drag_state,
                              const gui_sketch_card_t *card_under_mouse,
                              const gui_sketch_nav_tree_t *nav_tree,
                              cairo_t *cr
                            );

/*!
 *  \brief draws overlay graphics for GUI_TOOLBOX_NAVIGATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param drag_state current dragging status
 *  \param nav_tree the navigation tree, not NULL
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_nav_mode( gui_sketch_overlay_t *this_,
                                               const gui_sketch_drag_state_t *drag_state,
                                               const gui_sketch_nav_tree_t *nav_tree,
                                               cairo_t *cr
                                             );

/*!
 *  \brief draws overlay graphics for GUI_TOOLBOX_EDIT mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param drag_state current dragging status
 *  \param card_under_mouse the sketch card under the current mouse position or NULL if no card there
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_edit_mode( gui_sketch_overlay_t *this_,
                                                const gui_sketch_drag_state_t *drag_state,
                                                const gui_sketch_card_t *card_under_mouse,
                                                cairo_t *cr
                                              );

/*!
 *  \brief draws overlay graphics for GUI_TOOLBOX_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param drag_state current dragging status
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_create_mode( gui_sketch_overlay_t *this_,
                                                  const gui_sketch_drag_state_t *drag_state,
                                                  cairo_t *cr
                                                );

#endif  /* GUI_SKETCH_OVERLAY_H */


/*
Copyright 2017-2019 Andreas Warnke

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
