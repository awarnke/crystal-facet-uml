/* File: gui_main_window.h; Copyright and License: see below */

#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a main window and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "sketch_area/gui_sketch_area.h"
#include "gui_toolbox.h"
#include "gui_resources.h"
#include "gui_textedit.h"
#include "gui_file_use_db_dialog.h"
#include "gui_file_export_dialog.h"
#include "gui_search_request.h"
#include "storage/data_database.h"
#include "storage/data_change_message.h"
#include "ctrl_controller.h"
#include "util/observer/observer.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the main window
 */
struct gui_main_window_struct {
    GtkWidget *window;
    GtkWidget *sketcharea;
    gui_sketch_area_t sketcharea_data;  /*!< own instance of gui_sketch_area_t */
    gui_toolbox_t tools_data;  /*!< own instance of gui_toolbox_t */
    gui_marked_set_t marker_data;  /*!< own instance of a marker */
    GtkWidget *layout;
    data_change_notifier_t *data_notifier;  /*!< pointer to external data change notifier */
    gui_textedit_t text_editor;  /*!<  own instance of gui_text_edit_t */
    observer_t *window_close_observer;  /*!< pointer to external observer_t listener object */
    observer_t *window_open_observer;  /*!< pointer to external observer_t listener object */
    gui_file_use_db_dialog_t file_use_db_dialog;  /*!<  own instance of gui_file_use_db_dialog_t */
    gui_file_export_dialog_t file_export_dialog;  /*!<  own instance of gui_file_export_dialog_t */
    data_database_t *database;  /*!< pointer to external database */

    GtkWidget *toolbar;
    GtkWidget *file_use_db_icon;
    GtkToolItem *file_use_db;
    GtkWidget *file_export_icon;
    GtkToolItem *file_export;
    GtkWidget *file_new_window_icon;
    GtkToolItem *file_new_window;
    GtkWidget *tool_navigate_icon;
    GtkToolItem *tool_navigate;
    GtkWidget *tool_edit_icon;
    GtkToolItem *tool_edit;
    GtkWidget *tool_create_icon;
    GtkToolItem *tool_create;
    GtkWidget *tool_search_icon;
    GtkToolItem *tool_search;
    GtkWidget *edit_cut_icon;
    GtkToolItem *edit_cut;
    GtkWidget *edit_copy_icon;
    GtkToolItem *edit_copy;
    GtkWidget *edit_paste_icon;
    GtkToolItem *edit_paste;
    GtkWidget *edit_delete_icon;
    GtkToolItem *edit_delete;
    GtkWidget *edit_instantiate_icon;
    GtkToolItem *edit_instantiate;
    GtkWidget *edit_highlight_icon;
    GtkToolItem *edit_highlight;
    GtkWidget *edit_reset_icon;
    GtkToolItem *edit_reset;
    GtkWidget *edit_undo_icon;
    GtkToolItem *edit_undo;
    GtkWidget *edit_redo_icon;
    GtkToolItem *edit_redo;
    GtkWidget *tool_about_icon;
    GtkToolItem *tool_about;

    GtkAccelGroup *keyboard_shortcut_group;

    GtkWidget *id_label;
    GtkWidget *name_label;
    GtkWidget *name_entry;
    GtkWidget *description_label;
    GtkWidget *description_text_view;
    GtkWidget *type_label;
    GtkWidget *type_combo_box;
    GtkWidget *stereotype_label;
    GtkWidget *stereotype_entry;
    GtkWidget *edit_commit_icon;
    GtkWidget *edit_commit_button;

    GtkWidget *search_label;
    GtkWidget *search_entry;
    GtkWidget *search_btn_icon;
    GtkWidget *search_button;
    gui_search_request_t search_request;  /*!<  own instance of gui_search_request_t */

    GtkWidget *message_text_label;
    GtkWidget *message_icon_image;
    gui_simple_message_to_user_t message_to_user;  /*!<  own instance of gui_simple_message_to_user_t */
};

typedef struct gui_main_window_struct gui_main_window_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param database pointer to a database object
 *  \param db_reader pointer to a database reader object
 *  \param res pointer to a resource provider
 *  \param window_close_observer observer for a button press on the close window button
 *  \param window_open_observer observer for a button press on the open window button
 */
void gui_main_window_init( gui_main_window_t *this_,
                           ctrl_controller_t *controller,
                           data_database_t *database,
                           data_database_reader_t *db_reader,
                           gui_resources_t *res,
                           observer_t *window_close_observer,
                           observer_t *window_open_observer
);

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_main_window_destroy( gui_main_window_t *this_ );

/*!
 *  \brief callback that is executed when the main window is destroyed
 */
void gui_main_window_destroy_event_callback( GtkWidget *widget, gpointer data );

/*!
 *  \brief callback that is executed when the main window is requested to be deleted
 *
 *  \return true if the window shall not be deleted.
 */
gboolean gui_main_window_delete_event_callback( GtkWidget *widget, GdkEvent *event, gpointer data );

/*!
 *  \brief callback that informs that the use_db button was pressed
 */
void gui_main_window_use_db_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the export button was pressed
 */
void gui_main_window_export_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the new window button was pressed
 */
void gui_main_window_new_window_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the about button was pressed
 */
void gui_main_window_about_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the data of an object or the whole database changed
 */
void gui_main_window_data_changed_callback( GtkWidget *window, data_change_message_t *msg, gpointer user_data );

#endif  /* GUI_MAIN_WINDOW_H */


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
