/* File: gui_main_window.h; Copyright and License: see below */

#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a main window and reacts on user events
 */

#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "gui_sketch_area.h"
#include "gui_sketch_tools.h"
#include "gui_resources.h"
#include "gui_textedit.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the main window
 */
struct gui_main_window_struct {
    GtkWidget *window;
    GtkWidget *sketcharea;
    gui_sketch_area_t sketcharea_data;  /*!< own instance of gui_sketch_area_t */
    gui_sketch_tools_t sketchtools_data;  /*!< own instance of gui_sketch_tools_t */
    GtkWidget *layout;
    data_change_notifier_t *data_notifier;  /*!< pointer to external data change notifier */
    gui_textedit_t text_editor;  /*!<  own instance of gui_text_edit_t */
    data_database_t *database;  /*!< pointer to external database */
    data_database_reader_t db_reader;  /*!< own instance of a database reader */

    GtkWidget *toolbar;
    GtkToolItem *tool_use_db;
    GtkToolItem *tool_export;
    GtkToolItem *tool_new_window;
    GtkWidget *tool_navigate_icon;
    GtkToolItem *tool_navigate;
    GtkWidget *tool_edit_icon;
    GtkToolItem *tool_edit;
    GtkWidget *tool_new_obj_icon;
    GtkToolItem *tool_new_obj;
    GtkWidget *tool_new_view_icon;
    GtkToolItem *tool_new_view;
    GtkToolItem *tool_cut;
    GtkToolItem *tool_copy;
    GtkToolItem *tool_paste;
    GtkToolItem *tool_delete;
    GtkToolItem *tool_undo;
    GtkToolItem *tool_redo;
    GtkToolItem *tool_about;

    GtkWidget *name_entry;
    GtkWidget *description_text_view;
    GtkWidget *type_combo_box;
    GtkWidget *stereotype_entry;
    GtkWidget *edit_commit_icon;
    GtkWidget *edit_commit_button;
};

typedef struct gui_main_window_struct gui_main_window_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_main_window_init( gui_main_window_t *this_, ctrl_controller_t *controller, data_database_t *database, data_database_reader_t *db_reader, gui_resources_t *res );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_main_window_destroy( gui_main_window_t *this_ );

/*!
 *  \brief callback that is executed when the main window is destroyed
 */
void gui_main_window_destroy_event_callback(GtkWidget *widget, gpointer data );

/*!
 *  \brief callback that is executed when the main window is requeted to be deleted
 */
gboolean gui_main_window_delete_event_callback(GtkWidget *widget, GdkEvent *event, gpointer data );

#endif  /* GUI_MAIN_WINDOW_H */


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
