/* File: gui_sketch_area.h; Copyright and License: see below */

#ifndef GUI_SKETCH_AREA_H
#define GUI_SKETCH_AREA_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Loads data to be drawn, layouts sketch cards, reacts on user input
 */

#include "sketch/gui_sketch_card.h"
#include "sketch/gui_sketch_nav_tree.h"
#include "sketch/gui_sketch_result_list.h"
#include "sketch/gui_sketch_drag_state.h"
#include "sketch/gui_sketch_object_creator.h"
#include "sketch/gui_sketch_overlay.h"
#include "sketch/gui_sketch_background.h"
#include "sketch/gui_sketch_request.h"
#include "gui_toolbox.h"
#include "gui_marked_set.h"
#include "gui_resources.h"
#include "gui_simple_message_to_user.h"
#include "shape/shape_int_rectangle.h"
#include "storage/data_database_reader.h"
#include "storage/data_change_message.h"
#include "data_diagram.h"
#include "data_table.h"
#include "set/data_visible_set.h"
#include "set/data_full_id.h"
#include "set/data_small_set.h"
#include "set/data_search_result.h"
#include "set/data_search_result_list.h"
#include "ctrl_controller.h"
#include "pencil_diagram_maker.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants of gui_sketch_area_t
 */
enum gui_sketch_area_const_enum {
    GUI_SKETCH_AREA_CONST_MAX_CARDS = 22,  /*!< maximum number of diagrams to be shown in one single window */
    GUI_SKETCH_AREA_CONST_PARENT_CARD = 1,  /*!< index of the card showing the parent diagram */
    GUI_SKETCH_AREA_CONST_FOCUSED_CARD = 0,  /*!< index of the card showing the currently focused diagram */
    GUI_SKETCH_AREA_CONST_FIRST_CHILD_CARD = 2,  /*!< index of the card showing the first child diagram */
};

/*!
 *  \brief attributes of the sketch area widget
 */
struct gui_sketch_area_struct {
    /* external references */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    gui_resources_t *resources;  /*!< pointer to external resources */
    gui_toolbox_t *toolbox;  /*!< pointer to external tool box */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */
    gui_marked_set_t *marker;  /*!< pointer to external marker */
    GtkWidget *drawing_area;  /*!< pointer to the gtk drawing area, used as origin for selected-object-changed signals */

    /* the quest */
    gui_sketch_request_t request;  /*!< own instance of the requested tool-mode and diagram-ids */

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
    data_feature_t private_temp_fake_feature;
};

typedef struct gui_sketch_area_struct gui_sketch_area_t;

/*!
 *  \brief initializes the gui_sketch_area_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param drawing_area pointer to the gtk drawing area, used as origin for selected-object-changed signals. Ownership remains at caller.
 *  \param marker pointer to an object which references all focused, highlichted and selected ojects
 *  \param toolbox pointer to an object which represents the tool buttons
 *  \param message_to_user pointer to an object that can show a message to the user
 *  \param resources pointer to a resource provider
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 */
void gui_sketch_area_init ( gui_sketch_area_t *this_,
                            GtkWidget *drawing_area,
                            gui_marked_set_t *marker,
                            gui_toolbox_t *toolbox,
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
 *  \brief shows the list of search results
 *
 *  \param this_ pointer to own object attributes
 *  \param result_list list of search results including their diagram ids to be displayed
 */
void gui_sketch_area_show_result_list ( gui_sketch_area_t *this_, const data_search_result_list_t *result_list );

/*!
 *  \brief loads the cards, nav_tree and result_list data to be shown
 *
 *  \param this_ pointer to own object attributes
 *  \param main_diagram_id id of the main diagram to be shown or DATA_ROW_ID_VOID for root diagram
 */
void gui_sketch_area_show_diagram ( gui_sketch_area_t *this_, data_id_t main_diagram_id );

/*!
 *  \brief loads the cards and result_list data to be shown in search mode
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_area_private_load_cards_data ( gui_sketch_area_t *this_ );

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
 *  \note {DOES NOT WORK IN SEARCH MODE}
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagram_t *gui_sketch_area_private_get_focused_diagram_ptr ( gui_sketch_area_t *this_ );

/*!
 *  \brief gets the currently focused diagram id
 *
 *  \note {DOES NOT WORK IN SEARCH MODE}
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_row_id_t gui_sketch_area_private_get_focused_diagram_id ( gui_sketch_area_t *this_ );

/*!
 *  \brief callback that redraws the sketch_area widget
 */
#if ( GTK_MAJOR_VERSION >= 4 )
void gui_sketch_area_draw_callback( GtkDrawingArea *widget, cairo_t *cr, int width, int height, gpointer data );
#else
gboolean gui_sketch_area_draw_old_callback( GtkWidget *widget, cairo_t *cr, gpointer data );
#endif

/*!
 *  \brief redraws the sketch_area widget
 *
 *  \param this_ pointer to own object attributes
 *  \param width width of sketch widget
 *  \param height height of sketch widget
 *  \param cr cairo drawing context to draw the sketch widget
 */
void gui_sketch_area_draw( gui_sketch_area_t *this_, int width, int height, cairo_t *cr );

/*!
 *  \brief callback that informs that the mouse cursor left the sketch_area widget
 */
#if ( GTK_MAJOR_VERSION >= 4 )
void gui_sketch_area_leave_notify_callback( GtkEventControllerMotion* self, gpointer user_data );
#else
gboolean gui_sketch_area_leave_notify_old_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data );
#endif

/*!
 *  \brief callback that informs that the mouse cursor entered or moved on the sketch_area widget
 */
#if ( GTK_MAJOR_VERSION >= 4 )
void gui_sketch_area_motion_notify_callback( GtkEventControllerMotion* self,
                                             gdouble in_x,
                                             gdouble in_y,
                                             gpointer user_data
                                           );
#else
gboolean gui_sketch_area_mouse_motion_old_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data );
#endif

/*!
 *  \brief the mouse cursor entered or moved on the sketch_area widget
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position of mouse pointer
 *  \param y y-position of mouse pointer
 */
void gui_sketch_area_motion_notify( gui_sketch_area_t *this_, int x, int y );

/*!
 *  \brief callback that informs that the mouse button was pressed on the sketch_area widget
 */
#if ( GTK_MAJOR_VERSION >= 4 )
void gui_sketch_area_button_press_callback( GtkGestureClick* self,
                                            gint n_press,
                                            gdouble x,
                                            gdouble y,
                                            gpointer user_data
                                          );
#else
gboolean gui_sketch_area_button_press_old_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );
#endif

/*!
 *  \brief the primary mouse button was pressed on the sketch_area widget
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position of mouse pointer
 *  \param y y-position of mouse pointer
 */
void gui_sketch_area_button_press( gui_sketch_area_t *this_, int x, int y );

/*!
 *  \brief callback that informs that the mouse button was released on the sketch_area widget
 */
#if ( GTK_MAJOR_VERSION >= 4 )
void gui_sketch_area_button_release_callback( GtkGestureClick* self,
                                              gint n_press,
                                              gdouble x,
                                              gdouble y,
                                              gpointer user_data
                                            );
#else
gboolean gui_sketch_area_button_release_old_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );
#endif

/*!
 *  \brief the primary mouse button was released on the sketch_area widget
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position of mouse pointer
 *  \param y y-position of mouse pointer
 */
void gui_sketch_area_button_release( gui_sketch_area_t *this_, int x, int y );

/*!
 *  \brief callback that informs that the keyboard was pressed
 */
#if ( GTK_MAJOR_VERSION >= 4 )
gboolean gui_sketch_area_key_press_callback( GtkEventControllerKey* self,
                                             guint keyval,
                                             guint keycode,
                                             GdkModifierType state,
                                             gpointer user_data
                                           );
#else
gboolean gui_sketch_area_key_press_old_callback( GtkWidget* widget, GdkEventKey* evt, gpointer data );
#endif

/*!
 *  \brief the keyboard was pressed
 *
 *  \param this_ pointer to own object attributes
 *  \param ctrl_state true if the ctrl button is pressed
 *  \param keyval the key value of the pressed key
 *  \return true if the keypress was handled by this sketch area
 */
bool gui_sketch_area_key_press( gui_sketch_area_t *this_, bool ctrl_state, guint keyval );

/*!
 *  \brief callback that informs that the displayed data changed
 */
void gui_sketch_area_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer data );

/*!
 *  \brief callback that informs that the chosen tool changed
 */
void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_tool_t tool, gpointer data );

/*!
 *  \brief gets the diagram-id of the diagram and the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param[out] out_diagram_id a diagram id. The id is invalid if there is no diagram at the given location.
 *  \param[out] out_object_id an object id. This may refer to a diagram. The id is invalid if there is no diagram at the given location.
 */
static inline void gui_sketch_area_private_get_diagram_and_object_id_at_pos ( gui_sketch_area_t *this_,
                                                                              int32_t x,
                                                                              int32_t y,
                                                                              data_id_t* out_diagram_id,
                                                                              data_id_t* out_object_id
                                                                            );

/*!
 *  \brief gets the gui_sketch_card_t at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return a sketch card pointer. The return value is NULL if there is no card at the given location!
 */
static inline gui_sketch_card_t *gui_sketch_area_private_get_card_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  Nav-Tree and Result-List objects are ignored; this function is intended for use in EDIT and CREATE modes.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param[out] out_object_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param[out] out_diagram_id the diagram id at given location. The id is invalid if there is no diagram.
 */
static inline void gui_sketch_area_private_get_object_id_at_pos ( gui_sketch_area_t *this_,
                                                                  int32_t x,
                                                                  int32_t y,
                                                                  pencil_type_filter_t filter,
                                                                  data_full_id_t* out_object_id,
                                                                  data_id_t* out_diagram_id
                                                                );

/*!
 *  \brief gets the object-id of the surrounding object and a selected-part-info at a given position
 *
 *  Nav-Tree and Result-List objects are ignored; this function is intended for use in EDIT and CREATE modes.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param[out] out_object_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param[out] out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object embracing the given location.
 *  \param[out] out_diagram_id the diagram id at given location. The id is invalid if there is no diagram.
 */
static inline void gui_sketch_area_private_get_object_ids_at_pos ( gui_sketch_area_t *this_,
                                                                   int32_t x,
                                                                   int32_t y,
                                                                   pencil_type_filter_t filter,
                                                                   data_full_id_t* out_object_id,
                                                                   data_full_id_t* out_surrounding_id,
                                                                   data_id_t* out_diagram_id
                                                                 );

#include "gui_sketch_area.inl"

#endif  /* GUI_SKETCH_AREA_H */


/*
Copyright 2016-2022 Andreas Warnke

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
