/* File: gui_toolbox.h; Copyright and License: see below */

#ifndef GUI_TOOLBOX_H
#define GUI_TOOLBOX_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Informs the listener on the currently selected tool
 */

#include "gui_tool.h"
#include "gui_marked_set.h"
#include "gui_simple_message_to_user.h"
#include "gui_clipboard.h"
#include "set/data_stat.h"
#include "ctrl_controller.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the sketch tools object
 */
struct gui_toolbox_struct {
    gui_tool_t selected_tool;
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    gui_marked_set_t *marker;  /*!< pointer to external sketch marker */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */
    gui_clipboard_t clipboard;  /*!< own instance of a clipboard object */

    GtkWidget *toolbar;  /*!< pointer to external GObject which is used as origin of tool change signals */
    GtkToolItem *tool_navigate;  /*!< pointer to external GtkRadioToolButton */
    GtkToolItem *tool_edit;  /*!< pointer to external GtkRadioToolButton */
    GtkToolItem *tool_create;  /*!< pointer to external GtkRadioToolButton */
    GtkToolItem *tool_search;  /*!< pointer to external GtkRadioToolButton */
};

typedef struct gui_toolbox_struct gui_toolbox_t;

extern const char *GUI_TOOLBOX_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_toolbox_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param toolbar the GTK widget that contains the buttons. Ownership remains at caller.
 *  \param tool_navigate the GTK widget. Ownership remains at caller.
 *  \param tool_edit the GTK widget. Ownership remains at caller.
 *  \param tool_create the GTK widget. Ownership remains at caller.
 *  \param tool_search the GTK widget. Ownership remains at caller.
 *  \param gtk_clipboard pointer to the main/primary GtkClipboard
 *  \param marker pointer to the set of marked items
 *  \param message_to_user pointer to an object that can show a message to the user
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 */
void gui_toolbox_init ( gui_toolbox_t *this_,
                        GtkWidget *toolbar,
                        GtkToolItem *tool_navigate,
                        GtkToolItem *tool_edit,
                        GtkToolItem *tool_create,
                        GtkToolItem *tool_search,
                        GtkClipboard *gtk_clipboard,
                        gui_marked_set_t *marker,
                        gui_simple_message_to_user_t *message_to_user,
                        data_database_reader_t *db_reader,
                        ctrl_controller_t *controller
                      );

/*!
 *  \brief destroys the gui_toolbox_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_toolbox_destroy ( gui_toolbox_t *this_ );

/*!
 *  \brief gets the selected tool
 *
 *  \param this_ pointer to own object attributes
 */
static inline gui_tool_t gui_toolbox_get_selected_tool ( gui_toolbox_t *this_ );

/*!
 *  \brief sets the selected tool
 *
 *  \param this_ pointer to own object attributes
 *  \param tool tool to be selected
 */
static inline void gui_toolbox_set_selected_tool ( gui_toolbox_t *this_, gui_tool_t tool );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_navigate_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_edit_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_create_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_search_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_cut_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_copy_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_paste_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_delete_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_highlight_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_instantiate_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_reset_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_undo_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_toolbox_redo_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief notifies the listeners
 *
 *  \param this_ pointer to own object attributes
 */
void gui_toolbox_private_notify_listeners( gui_toolbox_t *this_ );

/*!
 *  \brief cuts the selected objects to the clipboard
 */
void gui_toolbox_cut( gui_toolbox_t *this_ );

/*!
 *  \brief copies the selected objects to the clipboard
 */
void gui_toolbox_copy( gui_toolbox_t *this_ );

/*!
 *  \brief pastes the objects from the clipboard
 */
void gui_toolbox_paste( gui_toolbox_t *this_ );

/*!
 *  \brief deletes the selected objects
 */
void gui_toolbox_delete( gui_toolbox_t *this_ );

/*!
 *  \brief deletes a set of objects
 *
 *  \param this_ pointer to own object attributes
 *  \param set_to_be_deleted set of element ids to be deleted
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_DELETED,
 *                 DATA_STAT_SERIES_IGNORED (e.g. lifelines) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if t.b.d.).
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t gui_toolbox_private_delete_set( gui_toolbox_t *this_,
                                             const data_small_set_t *set_to_be_deleted,
                                             data_stat_t *io_stat
                                           );

/*!
 *  \brief toggles display flags in a set of objects
 */
void gui_toolbox_private_toggle_display_flag_in_set( gui_toolbox_t *this_, const data_small_set_t *set_to_be_toggled, data_diagramelement_flag_t flag_bits_to_toggle );

#include "gui_toolbox.inl"

#endif  /* GUI_TOOLBOX_H */


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
