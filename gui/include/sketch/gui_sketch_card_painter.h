/* File: gui_sketch_card_painter.h; Copyright and License: see below */

#ifndef GUI_SKETCH_CARD_PAINTER_H
#define GUI_SKETCH_CARD_PAINTER_H

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
struct gui_sketch_card_painter_struct {
    gui_sketch_style_t sketch_style;  /*!< helper class to perform drawing */
};

typedef struct gui_sketch_card_painter_struct gui_sketch_card_painter_t;

/*!
 *  \brief initializes the sketch overlay
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_painter_init( gui_sketch_card_painter_t *this_ );

/*!
 *  \brief destroys the sketch overlay
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_painter_destroy( gui_sketch_card_painter_t *this_ );

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
void gui_sketch_card_painter_draw_overlay( gui_sketch_card_painter_t *this_,
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
void gui_sketch_card_painter_private_draw_edit_mode( gui_sketch_card_painter_t *this_,
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
void gui_sketch_card_painter_private_draw_create_mode( gui_sketch_card_painter_t *this_,
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
void gui_sketch_card_painter_private_draw_arrow( gui_sketch_card_painter_t *this_,
                                                 int32_t from_x,
                                                 int32_t from_y,
                                                 int32_t to_x,
                                                 int32_t to_y,
                                                 cairo_t *cr
                                               );

/*!
 *  \brief draws an classifier icon as overlay for GUI_TOOL_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the mouse pointer
 *  \param y y coordinate of the mouse pointer
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_private_draw_new_classifier( gui_sketch_card_painter_t *this_,
                                                          int32_t x,
                                                          int32_t y,
                                                          cairo_t *cr
                                                        );

/*!
 *  \brief draws an feature icon as overlay for GUI_TOOL_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the mouse pointer
 *  \param y y coordinate of the mouse pointer
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_private_draw_new_feature( gui_sketch_card_painter_t *this_,
                                                       int32_t x,
                                                       int32_t y,
                                                       cairo_t *cr
                                                     );

/*!
 *  \brief draws an relationship icon as overlay for GUI_TOOL_CREATE mode to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param x x coordinate of the mouse pointer
 *  \param y y coordinate of the mouse pointer
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_private_draw_new_relationship( gui_sketch_card_painter_t *this_,
                                                            int32_t x,
                                                            int32_t y,
                                                            cairo_t *cr
                                                          );

/*!
 *  \brief draws overlay graphics to visualize the grid to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param card_under_mouse the sketch card under the current mouse position, not NULL
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_private_draw_grid( gui_sketch_card_painter_t *this_,
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
void gui_sketch_card_painter_private_draw_snap_indicator( gui_sketch_card_painter_t *this_,
                                                          const gui_sketch_card_t *card_under_mouse,
                                                          gui_sketch_snap_state_t snapped,
                                                          int32_t x,
                                                          int32_t y,
                                                          cairo_t *cr
                                                        );

/*!
 *  \brief draws overlay graphics to visualize the order where the mouse points at
 *
 *  \param this_ pointer to own object attributes
 *  \param card_under_mouse the sketch card under the current mouse position, not NULL
 *  \param x actual pointer position
 *  \param y actual pointer position
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_private_visualize_order( gui_sketch_card_painter_t *this_,
                                                      const gui_sketch_card_t *card_under_mouse,
                                                      int32_t x,
                                                      int32_t y,
                                                      cairo_t *cr
                                                    );

/*!
 *  \brief draws the highlighted box of an element-part
 *
 *  \param this_ pointer to own object attributes
 *  \param subelement currently highlighted element-part
 *  \param layouted_set set of layouted elements
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_private_draw_element_space ( const gui_sketch_card_painter_t *this_,
                                                          const layout_subelement_id_t *subelement,
                                                          const layout_visible_set_t *layouted_set,
                                                          cairo_t *cr
                                                        );

/*!
 * \brief draws a rectangle in bold/highlighted color
 *
 *  \param this_ pointer to own object attributes
 *  \param rect rectangle to draw
 *  \param cr cairo drawing context
 */
static inline void gui_sketch_card_painter_private_draw_rect ( const gui_sketch_card_painter_t *this_,
                                                               const geometry_rectangle_t *rect,
                                                               cairo_t *cr
                                                             );

/*!
 * \brief draws a rectangle in bold/highlighted color, leaves out the empty space rect
 *
 *  \param this_ pointer to own object attributes
 *  \param border rectangle to draw
 *  \param space rectangle to leave out from draw
 *  \param cr cairo drawing context
 */
static inline void gui_sketch_card_painter_private_draw_border ( const gui_sketch_card_painter_t *this_,
                                                                 const geometry_rectangle_t *border,
                                                                 const geometry_rectangle_t *space,
                                                                 cairo_t *cr
                                                               );

/*!
 *  \brief draws the background of a single diagram card
 *
 *  If gui_sketch_card_is_visible(card) is false, this method does nothing.
 *
 *  \param this_ pointer to own object attributes
 *  \param selected_tool currently selected tool/edit-mode
 *  \param drag_state current dragging status
 *  \param card the sketch card of which to draw the background (paper)
 *  \param cr cairo drawing context
 */
void gui_sketch_card_painter_draw_paper ( gui_sketch_card_painter_t *this_,
                                          gui_tool_t selected_tool,
                                          const gui_sketch_drag_state_t *drag_state,
                                          const gui_sketch_card_t *card,
                                          cairo_t *cr
                                        );

#include "gui_sketch_card_painter.inl"

#endif  /* GUI_SKETCH_CARD_PAINTER_H */


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
