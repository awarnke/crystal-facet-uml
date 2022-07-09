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
#include "gui_attributes_editor.h"
#include "gui_file_use_db_dialog.h"
#include "gui_file_export_dialog.h"
#include "gui_search_request.h"
#include "gui_search_runner.h"
#include "io_data_file.h"
#include "storage/data_change_message.h"
#include "ctrl_controller.h"
#include "util/observer/observer.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the main window
 */
struct gui_main_window_struct {
    gui_sketch_area_t sketcharea_data;  /*!< own instance of gui_sketch_area_t */
    gui_toolbox_t tools_data;  /*!< own instance of gui_toolbox_t */
    gui_marked_set_t marker_data;  /*!< own instance of a marker */
    data_change_notifier_t *data_notifier;  /*!< pointer to external data change notifier */
    gui_attributes_editor_t attributes_editor;  /*!<  own instance of gui_attributes_editor_t */
    observer_t *window_close_observer;  /*!< pointer to external observer_t listener object */
    observer_t *window_open_observer;  /*!< pointer to external observer_t listener object */
    gui_file_use_db_dialog_t file_use_db_dialog;  /*!<  own instance of gui_file_use_db_dialog_t */
    gui_file_export_dialog_t file_export_dialog;  /*!<  own instance of gui_file_export_dialog_t */
    io_data_file_t *data_file;  /*!< pointer to external data_file */

    GtkWidget *window;
    GtkWidget *main_stack_column;
    GtkWidget *two_panes;
    GtkWidget *sketch_stack_column;
    GtkWidget *sketcharea;

    GtkWidget *tool_row;

    GtkWidget *file_new_db_icon;
    GtkButton *file_new_db;
    GtkWidget *file_use_db_icon;
    GtkButton *file_use_db;
    /*
    GtkWidget *file_save_as_icon;
    GtkButton *file_save_as;
    */
    GtkWidget *file_export_icon;
    GtkButton *file_export;

    GtkWidget *tool_sect_1_icon;

    GtkWidget *view_new_window_icon;
    GtkButton *view_new_window;
    GtkWidget *view_navigate_icon;
    GtkWidget *view_navigate;
    GtkWidget *view_edit_icon;
    GtkWidget *view_edit;
    GtkWidget *view_create_icon;
    GtkWidget *view_create;
    GtkWidget *view_search_icon;
    GtkWidget *view_search;

    GtkWidget *tool_sect_2_icon;

    GtkWidget *edit_undo_icon;
    GtkButton *edit_undo;
    GtkWidget *edit_redo_icon;
    GtkButton *edit_redo;
    GtkWidget *edit_cut_icon;
    GtkButton *edit_cut;
    GtkWidget *edit_copy_icon;
    GtkButton *edit_copy;
    GtkWidget *edit_paste_icon;
    GtkButton *edit_paste;
    GtkWidget *edit_delete_icon;
    GtkButton *edit_delete;
    GtkWidget *edit_instantiate_icon;
    GtkButton *edit_instantiate;
    GtkWidget *edit_highlight_icon;
    GtkButton *edit_highlight;
    GtkWidget *edit_reset_icon;
    GtkButton *edit_reset;
    GtkWidget *tool_about_icon;
    GtkButton *tool_about;

#if ( GTK_MAJOR_VERSION >= 4 )
    GtkShortcutController *keyboard_shortcut_ctrl;
#else
    GtkAccelGroup *keyboard_shortcut_group;
#endif

    GtkWidget *attr_edit_column;
    GtkWidget *attr_section_icon;
    GtkWidget *id_label;
    GtkWidget *name_label;
    GtkWidget *name_entry;
    GtkWidget *description_label;
    GtkWidget *description_text_view;
    GtkWidget *description_scroll_win;
    GtkWidget *type_label;
    GtkWidget *type_combo_box;
    GtkWidget *type_icon_grid;
    GtkWidget *stereotype_label;
    GtkWidget *stereotype_entry;
    GtkWidget *file_save_icon;
    GtkWidget *file_save_button;

    GtkWidget *search_row;
    GtkWidget *search_label;
    GtkWidget *search_entry;
    GtkWidget *search_btn_icon;
    GtkWidget *search_button;
    gui_search_request_t search_request;  /*!<  own instance of gui_search_request_t */
    gui_search_runner_t search_runner;  /*!<  own instance of gui_search_runner_t */

    GtkWidget *message_row;
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
 *  \param data_file pointer to a data_file object
 *  \param db_reader pointer to a database reader object
 *  \param res pointer to a resource provider
 *  \param gtk_app the gtk application object
 *  \param window_close_observer observer for a button press on the close window button
 *  \param window_open_observer observer for a button press on the open window button
 */
void gui_main_window_init( gui_main_window_t *this_,
                           ctrl_controller_t *controller,
                           io_data_file_t *data_file,
                           data_database_reader_t *db_reader,
                           gui_resources_t *res,
#if ( GTK_MAJOR_VERSION >= 4 )
                           GtkApplication *gtk_app,
#endif
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
 *  \brief initializes the widgets that belong to the toolbox group at the top of the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param res pointer to a resource provider
 */
void gui_main_window_private_init_toolbox( gui_main_window_t *this_, gui_resources_t *res );

/*!
 *  \brief initializes the widgets that belong to the attributes editor group at the right of the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param res pointer to a resource provider
 */
void gui_main_window_private_init_attributes_editor( gui_main_window_t *this_, gui_resources_t *res );

/*!
 *  \brief initializes the widgets that belong to the simple message to user group at the bottom of the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param res pointer to a resource provider
 */
void gui_main_window_private_init_simple_message_to_user( gui_main_window_t *this_, gui_resources_t *res );

/*!
 *  \brief initializes the widgets that belong to the sketch area group at the center of the main window,
 *         including the search bar
 *
 *  \param this_ pointer to own object attributes
 *  \param res pointer to a resource provider
 */
void gui_main_window_private_init_search_and_sketch_area( gui_main_window_t *this_, gui_resources_t *res );

/*!
 *  \brief callback that is executed when the main window is destroyed
 */
void gui_main_window_destroy_event_callback( GtkWidget *widget, gpointer data );

/*!
 *  \brief callback that is executed when the main window is requested to be deleted
 *
 *  \return true if the window shall not be deleted.
 */
#if ( GTK_MAJOR_VERSION >= 4 )
gboolean gui_main_window_delete_event_callback( GtkWindow *widget, gpointer data );
#else
gboolean gui_main_window_delete_event_callback( GtkWidget *widget, GdkEvent *event, gpointer data );
#endif

/*!
 *  \brief callback that informs that the new_db button was pressed
 */
void gui_main_window_new_db_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the open_db button was pressed
 */
void gui_main_window_open_db_btn_callback( GtkWidget* button, gpointer data );

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
