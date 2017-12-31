/* File: gui_sketch_area.h; Copyright and License: see below */

#ifndef GUI_SKETCH_AREA_H
#define GUI_SKETCH_AREA_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Loads data to be drawn, layouts sketch cards, reacts on user input
 */

#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_table.h"
#include "ctrl_controller.h"
#include "pencil_diagram_maker.h"
#include "pencil_input_data.h"
#include "pencil_visible_object_id.h"
#include "gui_sketch_card.h"
#include "gui_sketch_tools.h"
#include "gui_sketch_marker.h"
#include "gui_sketch_drag_state.h"
#include "gui_sketch_object_creator.h"
#include "gui_sketch_overlay.h"
#include "gui_sketch_background.h"
#include "gui_resources.h"
#include "gui_simple_message_to_user.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants of gui_sketch_area_t
 */
enum gui_sketch_area_const_enum {
    GUI_SKETCH_AREA_CONST_MAX_CARDS = 32,  /*!< maximum number of diagrams to be shown in one single window */
    GUI_SKETCH_AREA_CONST_PARENT_CARD = 1,  /*!< index of the card showing the parent diagram */
    GUI_SKETCH_AREA_CONST_SELECTED_CARD = 0,  /*!< index of the card showing the currently selected diagram */
    GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS = 30,  /*!< maximum size of temporary diagram buffer */
    GUI_SKETCH_AREA_CONST_MAX_LISTENERS = 4,  /*!< maximum number of listeners */
};

/*!
 *  \brief attributes of the sketch area widget
 */
struct gui_sketch_area_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    gui_resources_t *res;  /*!< pointer to external resources */
    gui_sketch_drag_state_t drag_state;  /*!< own instance of the drag state */
    gui_sketch_marker_t *marker;  /*!< pointer to external sketch marker */
    gui_sketch_overlay_t overlay;  /*!< own instance of sketch overlay */
    gui_sketch_background_t background;  /*!< own instance of sketch background */
    gui_sketch_tools_t *tools;  /*!< pointer to external sketch tools */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */

    gui_sketch_card_t cards[GUI_SKETCH_AREA_CONST_MAX_CARDS];  /*!< own instance of card objects that draw diagrams */
    int32_t card_num;
    data_diagram_t private_temp_diagram_buf[GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS];

    GObject *(listener[GUI_SKETCH_AREA_CONST_MAX_LISTENERS]);  /*!< array of pointers to listeners on selecting objects */
    gui_sketch_object_creator_t object_creator;  /*!< own instance of an object creator */
};

typedef struct gui_sketch_area_struct gui_sketch_area_t;

extern const char *GUI_SKETCH_AREA_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_sketch_area_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param marker pointer to an object which references all focused, highlichted and selected ojects
 *  \param tools pointer to an object which represents the tool buttons
 *  \param message_to_user pointer to an object that can show a message to the user
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 *  \param res pointer to a resource provider
 */
void gui_sketch_area_init ( gui_sketch_area_t *this_,
                            gui_sketch_marker_t *marker,
                            gui_sketch_tools_t *tools,
                            gui_simple_message_to_user_t *message_to_user,
                            gui_resources_t *res,
                            ctrl_controller_t *controller,
                            data_database_reader_t *db_reader
                          );

/*!
 *  \brief destroys the gui_sketch_area_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_destroy ( gui_sketch_area_t *this_ );

/*!
 *  \brief loads the cards to be shown
 *
 *  \param this_ pointer to own object attributes
 *  \param main_diagram_id id of the main diagram to be shown or DATA_ID_VOID_ID for root diagram
 */
void gui_sketch_area_private_load_cards ( gui_sketch_area_t *this_, int64_t main_diagram_id );

/*!
 *  \brief re-loads the cards to be shown
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_private_reload_cards ( gui_sketch_area_t *this_ );

/*!
 *  \brief layouts the cards in the sketch area widget
 *
 *  \param this_ pointer to own object attributes
 *  \param area_bounds bounding box within which the cards are layouted
 *  \param cr cairo drawing context, needed the determine the font metrics in the given drawing context
 */
void gui_sketch_area_private_layout_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr );

/*!
 *  \brief draws all diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param area_bounds bounding box within which the cards are drawn
 *  \param cr cairo drawing context
 */
void gui_sketch_area_private_draw_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr );

/*!
 *  \brief gets the currently selected diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagram_t *gui_sketch_area_get_selected_diagram_ptr ( gui_sketch_area_t *this_ );

/*!
 *  \brief gets the currently selected diagram id
 *
 *  \param this_ pointer to own object attributes
 */
static inline int64_t gui_sketch_area_get_selected_diagram_id ( gui_sketch_area_t *this_ );

/*!
 *  \brief callback that redraws the sketch_area widget
 */
gboolean gui_sketch_area_draw_callback ( GtkWidget *widget, cairo_t *cr, gpointer data );

/*!
 *  \brief callback that informs that the mouse cursor left the sketch_area widget
 */
gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data );

/*!
 *  \brief callback that informs that the mouse cursor moved on the sketch_area widget
 */
gboolean gui_sketch_area_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data );

/*!
 *  \brief callback that informs that the mouse button was pressed on the sketch_area widget
 */
gboolean gui_sketch_area_button_press_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );

/*!
 *  \brief callback that informs that the mouse button was released on the sketch_area widget
 */
gboolean gui_sketch_area_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );

/*!
 *  \brief callback that informs that the keyboard was pressed
 */
gboolean gui_sketch_area_key_press_callback( GtkWidget* widget, GdkEventKey* evt, gpointer data );

/*!
 *  \brief callback that informs that the displayed data changed
 */
void gui_sketch_area_data_changed_callback( GtkWidget *widget, data_id_t *object_id, gpointer data );

/*!
 *  \brief callback that informs that the chosen tool changed
 */
void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_sketch_tools_tool_t tool, gpointer data );

/*!
 *  \brief sets a listener in the listener array
 *
 *  \param this_ pointer to own object attributes
 *  \param index index in the array where to add the listener; 0 <= index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS
 *  \param listener listener object which is called when the focus changes
 */
static inline void gui_sketch_area_set_listener ( gui_sketch_area_t *this_, unsigned int index, GObject *listener );

/*!
 *  \brief removes a listener from the listener array
 *
 *  \param this_ pointer to own object attributes
 *  \param index index in the array where to remove the listener; 0 <= index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS
 */
static inline void gui_sketch_area_remove_listener ( gui_sketch_area_t *this_, unsigned int index );

/*!
 *  \brief notifies all listeners.
 *
 *  Sends the currently focused object id.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_private_notify_listener( gui_sketch_area_t *this_ );

/*!
 *  \brief gets the diagram-id of the diagram at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return a diagram id. The id is invalid if there is no diagram at the given location.
 */
static inline data_id_t gui_sketch_area_get_diagram_id_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 */
static inline void gui_sketch_area_get_object_id_at_pos ( gui_sketch_area_t *this_,
                                                          int32_t x,
                                                          int32_t y,
                                                          pencil_visible_object_id_t* out_selected_id,
                                                          pencil_visible_object_id_t* out_surrounding_id
                                                        );

/*!
 *  \brief gets the gui_sketch_card_t at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return a sketch card pointer. The return value is NULL if there is no card at the given location!
 */
static inline gui_sketch_card_t *gui_sketch_area_get_card_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y );

#include "gui_sketch_area.inl"

#endif  /* GUI_SKETCH_AREA_H */


/*
Copyright 2016-2018 Andreas Warnke

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
