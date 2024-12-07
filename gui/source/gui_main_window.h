 /* File: gui_main_window.h; Copyright and License: see below */

#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a main window and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "sketch/gui_sketch_area.h"
#include "gui_toolbox.h"
#include "gui_resources.h"
#include "gui_type_resource_list.h"
#include "attributes/gui_attributes_editor.h"
#include "gui_file_use_db_dialog.h"
#include "gui_file_export_dialog.h"
#include "gui_search_request.h"
#include "gui_search_runner.h"
#include "gui_button.h"
#include "attributes/gui_attribute_type_of_classifier.h"
#include "attributes/gui_attribute_type_of_diagram.h"
#include "attributes/gui_attribute_type_of_feature.h"
#include "attributes/gui_attribute_type_of_relationship.h"
#include "io_data_file.h"
#include "storage/data_change_message.h"
#include "ctrl_controller.h"
#include "entity/data_diagram_type.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship_type.h"
#include "observer/observer.h"
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
#ifndef NDEBUG
    ctrl_controller_t *controller;  /*!< pointer to external controller, which can test the database in debug mode */
#endif  /* not NDEBUG */

    GtkWidget *window;
    GtkWidget *main_stack_column;
    GtkWidget *two_panes;
    GtkWidget *sketch_stack_column;
    GtkWidget *sketcharea;

    GtkWidget *tool_row;

    gui_button_t file_new;
    gui_button_t file_open;
    gui_button_t file_save;
    gui_button_t file_export;

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

    gui_button_t edit_undo;
    gui_button_t edit_redo;
    gui_button_t edit_cut;
    gui_button_t edit_copy;
    gui_button_t edit_paste;
    gui_button_t edit_delete;
    gui_button_t edit_instantiate;
    gui_button_t edit_highlight;
    gui_button_t edit_reset;
    gui_button_t help_about;

    GtkShortcutController *keyboard_shortcut_ctrl;

    GtkWidget *attr_edit_column;
    GtkWidget *attr_section_icon;
    GtkWidget *id_row;
    GtkWidget *id_search_btn_icon;
    GtkWidget *id_search_btn;
    GtkWidget *id_label;
    GtkWidget *name_label;
    GtkWidget *name_entry;
    GtkWidget *description_label;
    GtkWidget *description_text_view;
    GtkWidget *description_scroll_win;
    GtkWidget *type_label;
    GtkDropDown *type_dropdown;
    GtkGrid   *type_diag_grid;
    GtkImage  *( type_diag_img[ GUI_TYPE_RESOURCE_LIST_DIAGRAMS ] );
    GtkButton *( type_diag_btn[ GUI_TYPE_RESOURCE_LIST_DIAGRAMS ] );
    gui_attribute_type_of_diagram_t type_diag_data[ GUI_TYPE_RESOURCE_LIST_DIAGRAMS ];
    GtkGrid   *type_clas_grid;
    GtkImage  *( type_clas_img[ GUI_TYPE_RESOURCE_LIST_CLASSIFIERS ] );
    GtkButton *( type_clas_btn[ GUI_TYPE_RESOURCE_LIST_CLASSIFIERS ] );
    gui_attribute_type_of_classifier_t type_clas_data[ GUI_TYPE_RESOURCE_LIST_CLASSIFIERS ];
    GtkGrid   *type_feat_grid;
    GtkImage  *( type_feat_img[ GUI_TYPE_RESOURCE_LIST_INV_FEATURES ] );
    GtkButton *( type_feat_btn[ GUI_TYPE_RESOURCE_LIST_INV_FEATURES ] );
    gui_attribute_type_of_feature_t type_feat_data[ GUI_TYPE_RESOURCE_LIST_INV_FEATURES ];
    GtkGrid   *type_rel_grid;
    GtkImage  *( type_rel_img[ GUI_TYPE_RESOURCE_LIST_RELATIONS ] );
    GtkButton *( type_rel_btn[ GUI_TYPE_RESOURCE_LIST_RELATIONS ] );
    gui_attribute_type_of_relationship_t type_rel_data[ GUI_TYPE_RESOURCE_LIST_RELATIONS ];
    GtkWidget *stereotype_label;
    GtkWidget *stereotype_entry;

    GtkWidget *options_layout;  /*!< the grid layout container for the export options, created by file_export_dialog */

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
                           GtkApplication *gtk_app,
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
gboolean gui_main_window_delete_event_callback( GtkWindow *widget, gpointer data );

/*!
 *  \brief callback that informs that the new_db button was pressed
 */
void gui_main_window_new_db_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the open_db button was pressed
 */
void gui_main_window_open_db_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the save button was pressed
 */
void gui_main_window_save_btn_callback( GtkButton *button, gpointer user_data );

/*!
 *  \brief callback that informs that the save shortcut was activated
 */
gboolean gui_main_window_save_shortcut_callback( GtkWidget* widget, GVariant* args, gpointer user_data );

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
Copyright 2016-2024 Andreas Warnke

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
