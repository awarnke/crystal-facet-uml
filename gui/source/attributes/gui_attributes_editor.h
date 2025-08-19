/* File: gui_attributes_editor.h; Copyright and License: see below */

#ifndef GUI_ATTRIBUTES_EDITOR_H
#define GUI_ATTRIBUTES_EDITOR_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides data to editing widgets and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "gui_resources.h"
#include "gui_type_resource.h"
#include "gui_type_resource_list.h"
#include "storage/data_database_reader.h"
#include "storage/data_database.h"
#include "storage/data_change_message.h"
#include "entity/data_classifier.h"
#include "entity/data_diagram.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "ctrl_controller.h"
#include "entity/data_id.h"
#include "gui_gtk.h"

/*!
 *  \brief enumeration on tools
 */
enum gui_attributes_editor_sync_dir_enum {
    GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI = 0,  /*!< default sync direction: data changes update gui widgets */
    GUI_ATTRIBUTES_EDITOR_SYNC_DIR_GUI_TO_DB,  /*!< gui widgets must not be updated till user changes are written back to db */
};

typedef enum gui_attributes_editor_sync_dir_enum gui_attributes_editor_sync_dir_t;

/*!
 *  \brief attributes of the gui_attributes_editor_t
 */
struct gui_attributes_editor_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    data_database_t *database;  /*!< pointer to external data_database_t */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */
    gui_attributes_editor_sync_dir_t sync_dir;  /*!< flag that indicates if changes to database shall overwrite user input */

    data_id_t selected_object_id;  /*!< id of the object which is currently edited */
    data_diagram_t private_diagram_cache;  /*!< own instance of a diagram cache */
    data_classifier_t private_classifier_cache;  /*!< own instance of a classifier cache */
    data_feature_t private_feature_cache;  /*!< own instance of a feature cache */
    data_relationship_t private_relationship_cache;  /*!< own instance of a relationship cache */
    data_id_t latest_created_id;  /*!< id of the latest created object, allows to check if the selected object is new */
    data_id_t second_latest_id;  /*!< id of the second latest created object, needed if a classifier and a containment relation are created together. */


    GtkLabel *id_label;  /*!< pointer to external id label widget */
    GtkEntry *name_entry;  /*!< pointer to external text entry widget */
    GtkEntry *stereotype_entry;  /*!< pointer to external text entry widget */
    gui_type_resource_list_t type_lists;  /*!< own instance of type lists */
    GtkDropDown *type_dropdown;  /*!< pointer to external drop down widget */
    GuiTypeResource *temp_search_me;  /*!< own instance of a pointer to an object stating a search request */
    data_id_t previous_selected_object_id;  /*!< id of the object which was previously edited */
    GtkWidget *type_diag_grid;  /*!< pointer to external type icon grid widget of diagram types */
    GtkWidget *type_clas_grid;  /*!< pointer to external type icon grid widget of classifier types */
    GtkWidget *type_feat_grid;  /*!< pointer to external type icon grid widget of feature types */
    GtkWidget *type_rel_grid;  /*!< pointer to external type icon grid widget of relationship types */
    GtkTextView *description_text_view;  /*!< pointer to external text view widget */
    GtkButton *commit_button;  /*!< pointer to external button widget */
};

typedef struct gui_attributes_editor_struct gui_attributes_editor_t;

/*!
 *  \brief initializes the gui_attributes_editor_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id_label pointer to id-label widget
 *  \param name_entry pointer to text entry widget
 *  \param stereotype_entry pointer to external text entry widget
 *  \param type_dropdown pointer to external drop down widget
 *  \param type_diag_grid pointer to external type-icon grid widget of diagram types
 *  \param type_clas_grid pointer to external type-icon grid widget of classifier types
 *  \param type_feat_grid pointer to external type-icon grid widget of feature types
 *  \param type_rel_grid pointer to external type-icon grid widget of relationship types
 *  \param description_text_view pointer to text entry widget
 *  \param commit_button pointer to button widget
 *  \param resources pointer to a resource provider
 *  \param controller pointer to the controller object to use
 *  \param db_reader pointer to the database reader object to use
 *  \param database pointer to the database object to use, used to flush the database if requested
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_attributes_editor_init ( gui_attributes_editor_t *this_,
                                  GtkLabel *id_label,
                                  GtkEntry *name_entry,
                                  GtkEntry *stereotype_entry,
                                  GtkDropDown *type_dropdown,
                                  GtkWidget *type_diag_grid,
                                  GtkWidget *type_clas_grid,
                                  GtkWidget *type_feat_grid,
                                  GtkWidget *type_rel_grid,
                                  GtkTextView *description_text_view,
                                  GtkButton *commit_button,
                                  gui_resources_t *resources,
                                  ctrl_controller_t *controller,
                                  data_database_reader_t *db_reader,
                                  data_database_t *database,
                                  gui_simple_message_to_user_t *message_to_user
                                );

/*!
 *  \brief destroys the gui_attributes_editor_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_destroy ( gui_attributes_editor_t *this_ );

/*!
 *  \brief redraws the textedit widgets.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_update_widgets ( gui_attributes_editor_t *this_ );

/*!
 *  \brief commits changed properties to the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_commit_changes ( gui_attributes_editor_t *this_ );

/*!
 *  \brief prints the gui_attributes_editor_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_trace ( const gui_attributes_editor_t *this_ );

/* ================================ USER INPUT CALLBACKS ================================ */

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
void gui_attributes_editor_name_focus_left_callback( GtkEventControllerFocus* self, gpointer user_data );

/*!
 *  \brief callback that informs that enter was pressed
 */
void gui_attributes_editor_name_enter_callback ( GtkEntry *widget, gpointer user_data );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
void gui_attributes_editor_stereotype_focus_left_callback( GtkEventControllerFocus* self, gpointer user_data );

/*!
 *  \brief callback that informs that enter was pressed
 */
void gui_attributes_editor_stereotype_enter_callback ( GtkEntry *widget, gpointer user_data );

/*!
 *  \brief callback that informs that the type in the combo box changed
 */
void gui_attributes_editor_type_changed_callback ( GObject* self, GParamSpec* pspec, gpointer user_data );

/*!
 *  \brief callback that informs that a type button was pressed
 */
void gui_attributes_editor_type_of_diagram_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that a type button was pressed
 */
void gui_attributes_editor_type_of_classifier_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that a type button was pressed
 */
void gui_attributes_editor_type_of_feature_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that a type button was pressed
 */
void gui_attributes_editor_type_of_relationship_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
void gui_attributes_editor_description_focus_left_callback( GtkEventControllerFocus* self, gpointer user_data );

/* ================================ SELECTION or MODEL CHANGED CALLBACKS ================================ */

/*!
 *  \brief callback that informs that another object was focused
 */
void gui_attributes_editor_focused_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_attributes_editor_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data );

/* ================================ PRIVATE METHODS ================================ */

/*!
 *  \brief loads an object into cache (even if this object is already cached).
 *
 *  \param this_ pointer to own object attributes
 *  \param id identifier of the object to be loaded
 */
void gui_attributes_editor_private_load_object ( gui_attributes_editor_t *this_, data_id_t id );

/*!
 *  \brief commits changes to the objects name to the controller.
 *
 *  If the name is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_name_commit_changes ( gui_attributes_editor_t *this_ );

/*!
 *  \brief commits changes to the objects stereotype to the controller.
 *
 *  If the stereotype is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_stereotype_commit_changes ( gui_attributes_editor_t *this_ );

/*!
 *  \brief commits changes to the objects type to the controller.
 *
 *  If the type is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 *  \param obj_type obj_type new object type to be set. This has to be read out from the type selection widgets by the caller
 */
void gui_attributes_editor_private_type_commit_changes ( gui_attributes_editor_t *this_, int obj_type );

/*!
 *  \brief commits changes to the objects description to the controller.
 *
 *  If the description is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_description_commit_changes ( gui_attributes_editor_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_attributes_editor_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_id_update_view ( gui_attributes_editor_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_attributes_editor_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_name_update_view ( gui_attributes_editor_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_attributes_editor_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_stereotype_update_view ( gui_attributes_editor_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_attributes_editor_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_type_update_view ( gui_attributes_editor_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_attributes_editor_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_attributes_editor_private_description_update_view ( gui_attributes_editor_t *this_ );

#endif  /* GUI_ATTRIBUTES_EDITOR_H */


/*
Copyright 2016-2025 Andreas Warnke

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
