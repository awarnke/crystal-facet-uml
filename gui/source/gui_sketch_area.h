/* File: gui_sketch_area.h; Copyright and License: see below */

#ifndef GUI_SKETCH_AREA_H
#define GUI_SKETCH_AREA_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Loads data to be drawn, layouts sketch cards, reacts on user input
 */

#include "util/shape/shape_int_rectangle.h"
#include "data_database_reader.h"
#include "data_diagram.h"
#include "data_table.h"
#include "ctrl_controller.h"
#include "pencil_diagram_painter.h"
#include "pencil_input_data.h"
#include "gui_sketch_card.h"
#include "gui_sketch_tools.h"
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
    gui_sketch_card_t cards[GUI_SKETCH_AREA_CONST_MAX_CARDS];  /*!< own instance of card objects that draw diagrams */
    int32_t card_num;
    bool mark_active;
    int32_t mark_start_x;
    int32_t mark_start_y;
    int32_t mark_end_x;
    int32_t mark_end_y;
    gui_sketch_tools_t *tools;  /*!< pointer to external sketch tools */
    data_diagram_t private_temp_diagram_buf[GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS];
    GObject *(listener[GUI_SKETCH_AREA_CONST_MAX_LISTENERS]);  /*!< array of pointers to listeners on selecting objects */
};

typedef struct gui_sketch_area_struct gui_sketch_area_t;

extern const char *GUI_SKETCH_AREA_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_sketch_area_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_init ( gui_sketch_area_t *this_, gui_sketch_tools_t *tools, ctrl_controller_t *controller, data_database_reader_t *db_reader );

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
 *  \param main_diagram_id id of the main diagram to be shown or DATA_DIAGRAM_ID_VOID_ID for root diagram
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
 */
void gui_sketch_area_private_layout_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds );

/*!
 *  \brief draws all diagrams
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_private_draw_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr );

/*!
 *  \brief queues the currently marked widget area for redrawing
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_area_private_queue_draw_mark_area( GtkWidget* widget, gui_sketch_area_t *this_ );

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
 *  \brief callback that informs that the displayed data changed
 */
void gui_sketch_area_data_changed_callback( GtkWidget *widget, void *unused, gpointer data );

/*!
 *  \brief callback that informs that the chosen tool changed
 */
void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_sketch_tools_tool_t tool, gpointer data );

/*!
 *  \brief sets a listener in the listener array
 *
 *  \param this_ pointer to own object attributes
 *  \param index index in the array where to add the listener; 0 <= index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS
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
 *  \brief notifies all listeners
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_private_notify_listener( gui_sketch_area_t *this_, data_table_t table, int64_t id );

#include "gui_sketch_area.inl"

#endif  /* GUI_SKETCH_AREA_H */


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
