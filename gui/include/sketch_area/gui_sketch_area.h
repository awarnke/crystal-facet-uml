/* File: gui_sketch_area.h; Copyright and License: see below */

#ifndef GUI_SKETCH_AREA_H
#define GUI_SKETCH_AREA_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Loads data to be drawn, layouts sketch cards, reacts on user input
 */

#include "sketch_area/gui_sketch_card.h"
#include "sketch_area/gui_sketch_nav_tree.h"
#include "sketch_area/gui_sketch_result_list.h"
#include "sketch_area/gui_sketch_drag_state.h"
#include "sketch_area/gui_sketch_object_creator.h"
#include "sketch_area/gui_sketch_overlay.h"
#include "sketch_area/gui_sketch_background.h"
#include "gui_toolbox.h"
#include "gui_marked_set.h"
#include "gui_resources.h"
#include "gui_simple_message_to_user.h"
#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database_reader.h"
#include "storage/data_change_message.h"
#include "data_diagram.h"
#include "data_table.h"
#include "set/data_visible_set.h"
#include "set/data_id_pair.h"
#include "ctrl_controller.h"
#include "pencil_diagram_maker.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants of gui_sketch_area_t
 */
enum gui_sketch_area_const_enum {
    GUI_SKETCH_AREA_CONST_MAX_CARDS = 32,  /*!< maximum number of diagrams to be shown in one single window */
    GUI_SKETCH_AREA_CONST_PARENT_CARD = 1,  /*!< index of the card showing the parent diagram */
    GUI_SKETCH_AREA_CONST_FOCUSED_CARD = 0,  /*!< index of the card showing the currently focused diagram */
    GUI_SKETCH_AREA_CONST_FIRST_CHILD_CARD = 2,  /*!< index of the card showing the furst child diagram */
    GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS = 30,  /*!< maximum size of temporary diagram buffer */
};

/*!
 *  \brief attributes of the sketch area widget
 */
struct gui_sketch_area_struct {
    /* external references */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    gui_resources_t *resources;  /*!< pointer to external resources */
    gui_toolbox_t *tools;  /*!< pointer to external tools */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */
    gui_marked_set_t *marker;  /*!< pointer to external marker */
    GtkWidget *drawing_area;  /*!< pointer to the gtk drawing area, used as origin for selected-object-changed signals */

    /* helper objects */
    gui_sketch_drag_state_t drag_state;  /*!< own instance of the drag state */
    gui_sketch_object_creator_t object_creator;  /*!< own instance of an object creator */

    /* sub widgets, things that can draw provided input-data */
    gui_sketch_overlay_t overlay;  /*!< own instance of sketch overlay */
    gui_sketch_background_t background;  /*!< own instance of sketch background */
    gui_sketch_card_t cards[GUI_SKETCH_AREA_CONST_MAX_CARDS];  /*!< own instance of card objects that draw diagrams */
    uint32_t card_num;
    gui_sketch_nav_tree_t nav_tree;  /*!< own instance of a navigation tree sub-widget */
    gui_sketch_result_list_t result_list;  /*!< own instance of a search result list sub-widget */

    /* internal data structures */
    data_diagram_t private_temp_diagram_buf[GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS];
    data_feature_t private_temp_fake_feature;
};

typedef struct gui_sketch_area_struct gui_sketch_area_t;

extern const char *GUI_SKETCH_AREA_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_sketch_area_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param drawing_area pointer to the gtk drawing area, used as origin for selected-object-changed signals. Ownership remains at caller.
 *  \param marker pointer to an object which references all focused, highlichted and selected ojects
 *  \param tools pointer to an object which represents the tool buttons
 *  \param message_to_user pointer to an object that can show a message to the user
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 *  \param resources pointer to a resource provider
 */
void gui_sketch_area_init ( gui_sketch_area_t *this_,
                            GtkWidget *drawing_area,
                            gui_marked_set_t *marker,
                            gui_toolbox_t *tools,
                            gui_simple_message_to_user_t *message_to_user,
                            gui_resources_t *resources,
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
 *  \brief loads the cards, nav_tree and result_list data to be shown
 *
 *  \param this_ pointer to own object attributes
 *  \param main_diagram_id id of the main diagram to be shown or DATA_ID_VOID_ID for root diagram
 */
void gui_sketch_area_private_load_data ( gui_sketch_area_t *this_, int64_t main_diagram_id );

/*!
 *  \brief re-loads the cards, nav_tree and result_list data to be shown
 *
 *  If the currently visible diagram is not available anymore, this
 *  function moves the current focus to another diagram.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_private_refocus_and_reload_data ( gui_sketch_area_t *this_ );

/*!
 *  \brief layouts the cards, nav_tree and result_list in the sketch area widget
 *
 *  \param this_ pointer to own object attributes
 *  \param area_bounds bounding box within which the cards are layouted
 *  \param cr cairo drawing context, needed to determine the font metrics in the given drawing context
 */
void gui_sketch_area_private_layout_subwidgets ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr );

/*!
 *  \brief layouts a list of cards into a bounding rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param first index of the first card (in cards array) to be layouted
 *  \param length number of cards to be layouted
 *  \param bounds bounding box within which the cards are layouted
 *  \param cr cairo drawing context, needed to determine the font metrics in the given drawing context
 */
void gui_sketch_area_private_layout_card_list ( gui_sketch_area_t *this_,
                                                uint32_t first,
                                                uint32_t length,
                                                shape_int_rectangle_t bounds,
                                                cairo_t *cr
                                              );

/*!
 *  \brief draws all cards, nav_tree and result_list
 *
 *  \param this_ pointer to own object attributes
 *  \param area_bounds bounding box within which the cards are drawn
 *  \param cr cairo drawing context
 */
void gui_sketch_area_private_draw_subwidgets ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr );

/*!
 *  \brief gets the currently focused diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagram_t *gui_sketch_area_get_focused_diagram_ptr ( gui_sketch_area_t *this_ );

/*!
 *  \brief gets the currently focused diagram id
 *
 *  \param this_ pointer to own object attributes
 */
static inline int64_t gui_sketch_area_get_focused_diagram_id ( gui_sketch_area_t *this_ );

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
void gui_sketch_area_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer data );

/*!
 *  \brief callback that informs that the chosen tool changed
 */
void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_tool_t tool, gpointer data );

/*!
 *  \brief notifies all listeners.
 *
 *  Sends the currently focused object id.
 *
 *  \param this_ pointer to own object attributes
 *  \param modified_real_object_id id of the real object that was modified (the classifier, not the diagramelement)
 */
void gui_sketch_area_private_notify_listeners( gui_sketch_area_t *this_, data_id_t modified_real_object_id );

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
 *  \brief gets the gui_sketch_card_t at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return a sketch card pointer. The return value is NULL if there is no card at the given location!
 */
static inline gui_sketch_card_t *gui_sketch_area_get_card_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 */
static inline void gui_sketch_area_private_get_object_id_at_pos ( gui_sketch_area_t *this_,
                                                                  int32_t x,
                                                                  int32_t y,
                                                                  pencil_type_filter_t filter,
                                                                  data_id_pair_t* out_selected_id
                                                                );

/*!
 *  \brief gets the object-id of the surrounding object and a selected-part-info at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object embracing the given location.
 */
static inline void gui_sketch_area_private_get_object_ids_at_pos ( gui_sketch_area_t *this_,
                                                                   int32_t x,
                                                                   int32_t y,
                                                                   pencil_type_filter_t filter,
                                                                   data_id_pair_t* out_selected_id,
                                                                   data_id_pair_t* out_surrounding_id
                                                                 );

#include "gui_sketch_area.inl"

#endif  /* GUI_SKETCH_AREA_H */


/*
Copyright 2016-2019 Andreas Warnke

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
