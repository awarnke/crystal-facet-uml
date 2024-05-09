/* File: gui_sketch_overlay.h; Copyright and License: see below */

#ifndef GUI_SKETCH_OVERLAY_H
#define GUI_SKETCH_OVERLAY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws graphical elements on top of sketch cards
 */

#include "sketch/gui_sketch_card.h"
#include "sketch/gui_sketch_nav_tree.h"
#include "sketch/gui_sketch_drag_state.h"
#include "sketch/gui_sketch_style.h"
#include "gui_tool.h"
#include "layout/layout_subelement_kind.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the sketch overlay
 */
struct gui_sketch_overlay_struct {
    gui_sketch_style_t sketch_style;  /*!< helper class to perform drawing */
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
 *  \param marked_objects the focused and highighted objects
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_draw( gui_sketch_overlay_t *this_,
                              gui_tool_t selected_tool,
                              const gui_sketch_drag_state_t *drag_state,
                              const gui_sketch_card_t *card_under_mouse,
                              gui_marked_set_t *marked_objects,
                              cairo_t *cr
                            );

/*!
 *  \brief draws overlay graphics for GUI_TOOL_EDIT mode to the cairo context
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
 *  \brief draws overlay graphics for GUI_TOOL_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param drag_state current dragging status
 *  \param card_under_mouse the sketch card under the current mouse position or NULL if no card there
 *  \param highlighted_table the elements's type (table) the mouse is hovering on
 *  \param highlighted_kind the kind of the highlighted element part
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_create_mode( gui_sketch_overlay_t *this_,
                                                  const gui_sketch_drag_state_t *drag_state,
                                                  const gui_sketch_card_t *card_under_mouse,
                                                  data_table_t highlighted_table,
                                                  layout_subelement_kind_t highlighted_kind,
                                                  cairo_t *cr
                                                );

/*!
 *  \brief draws an overlay arrow for GUI_TOOL_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param from_x source x coordinate of the mouse pointer
 *  \param from_y source y coordinate of the mouse pointer
 *  \param to_x destination x coordinate of the mouse pointer
 *  \param to_y destination y coordinate of the mouse pointer
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_arrow( gui_sketch_overlay_t *this_,
                                            int32_t from_x,
                                            int32_t from_y,
                                            int32_t to_x,
                                            int32_t to_y,
                                            cairo_t *cr
                                          );

/*!
 *  \brief draws an overlay icon for GUI_TOOL_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the mouse pointer
 *  \param y y coordinate of the mouse pointer
 *  \param with_arrow_option true if arrow and feature box shall be shown, false for classifier-box
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_create_icon( gui_sketch_overlay_t *this_,
                                                  int32_t x,
                                                  int32_t y,
                                                  bool with_arrow_option,
                                                  cairo_t *cr
                                                );

/*!
 *  \brief draws overlay graphics to visualize the grid to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param card_under_mouse the sketch card under the current mouse position, not NULL
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_grid( gui_sketch_overlay_t *this_,
                                           const gui_sketch_card_t *card_under_mouse,
                                           cairo_t *cr
                                         );

/*!
 *  \brief draws overlay graphics to visualize the snapped-to-grid to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param card_under_mouse the sketch card under the current mouse position, not NULL
 *  \param snapped flags indicating if the current cursor position is snapped
 *  \param x actual pointer position
 *  \param y actual pointer position
 *  \param cr cairo drawing context
 */
void gui_sketch_overlay_private_draw_snap_indicator( gui_sketch_overlay_t *this_,
                                                     const gui_sketch_card_t *card_under_mouse,
                                                     gui_sketch_snap_state_t snapped,
                                                     int32_t x,
                                                     int32_t y,
                                                     cairo_t *cr
                                                   );


#endif  /* GUI_SKETCH_OVERLAY_H */


/*
Copyright 2017-2024 Andreas Warnke

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
