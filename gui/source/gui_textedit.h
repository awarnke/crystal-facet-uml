/* File: gui_textedit.h; Copyright and License: see below */

#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides data to editing widgets and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "storage/data_database_reader.h"
#include "storage/data_database.h"
#include "storage/data_change_message.h"
#include "data_classifier.h"
#include "data_diagram.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "ctrl_controller.h"
#include "data_id.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the gui_textedit_t
 */
struct gui_textedit_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    data_database_t *database;  /*!< pointer to external data_database_t */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */

    data_id_t selected_object_id;  /*!< id of the object which is currently edited */
    data_diagram_t private_diagram_cache;  /*!< own instance of a diagram cache */
    data_classifier_t private_classifier_cache;  /*!< own instance of a classifier cache */
    data_feature_t private_feature_cache;  /*!< own instance of a feature cache */
    data_relationship_t private_relationship_cache;  /*!< own instance of a relationship cache */

    GtkListStore *no_types;  /* a list representing only n/a */
    GtkListStore *diagram_types;
    GtkListStore *classifier_types;
    GtkListStore *relationship_types;
    GtkListStore *feature_types;
    GtkListStore *feature_lifeline_type;

    GtkEntry *name_entry;  /*!< pointer to external text entry widget */
    GtkEntry *stereotype_entry;  /*!< pointer to external text entry widget */
    GtkComboBox *type_combo_box;  /*!< pointer to external combo box widget */
    GtkTextView *description_text_view;  /*!< pointer to external text view widget */
    GtkButton *commit_button;  /*!< pointer to external button widget */
};

typedef struct gui_textedit_struct gui_textedit_t;

/*!
 *  \brief initializes the gui_textedit_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param name_entry  pointer to text entry widget
 *  \param stereotype_entry pointer to external text entry widget
 *  \param type_combo_box pointer to external combo box widget
 *  \param description_text_view pointer to text entry widget
 *  \param commit_button pointer to button widget
 *  \param controller pointer to the controller object to use
 *  \param db_reader pointer to the database reader object to use
 *  \param database pointer to the database object to use, used to flush the database if requested
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_textedit_init ( gui_textedit_t *this_,
                         GtkEntry *name_entry,
                         GtkEntry *stereotype_entry,
                         GtkComboBox *type_combo_box,
                         GtkTextView *description_text_view,
                         GtkButton *commit_button,
                         ctrl_controller_t *controller,
                         data_database_reader_t *db_reader,
                         data_database_t *database,
                         gui_simple_message_to_user_t *message_to_user
                       );

/*!
 *  \brief destroys the gui_textedit_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_destroy ( gui_textedit_t *this_ );

/*!
 *  \brief redraws the textedit widgets.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_update_widgets ( gui_textedit_t *this_ );

/*!
 *  \brief commits changed properties to the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_commit_changes ( gui_textedit_t *this_ );

/*!
 *  \brief prints the gui_textedit_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_trace ( const gui_textedit_t *this_ );

/* ================================ USER INPUT CALLBACKS ================================ */

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
gboolean gui_textedit_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
gboolean gui_textedit_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data );

/*!
 *  \brief callback that informs that the type in the combo box changed
 */
void gui_textedit_type_changed_callback ( GtkComboBox *widget, gpointer user_data );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
gboolean gui_textedit_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data );

/*!
 *  \brief callback that informs that the commit button was pressed
 */
void gui_textedit_commit_clicked_callback (GtkButton *button, gpointer user_data );

/* ================================ SELECTION or MODEL CHANGED CALLBACKS ================================ */

/*!
 *  \brief callback that informs that another object was selected
 */
void gui_textedit_name_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that another object was selected
 */
void gui_textedit_stereotype_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that another object was selected
 */
void gui_textedit_type_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that another object was selected
 */
void gui_textedit_description_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_name_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_stereotype_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_description_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_type_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data );

/* ================================ PRIVATE METHODS ================================ */

/*!
 *  \brief loads an object into cache (even if this object is already cached).
 *
 *  \param this_ pointer to own object attributes
 *  \param id identifier of the object to be loaded
 */
void gui_textedit_private_load_object ( gui_textedit_t *this_, data_id_t id );

/*!
 *  \brief commits changes to the objects name to the controller.
 *
 *  If the name is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_name_commit_changes ( gui_textedit_t *this_ );

/*!
 *  \brief commits changes to the objects stereotype to the controller.
 *
 *  If the stereotype is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_stereotype_commit_changes ( gui_textedit_t *this_ );

/*!
 *  \brief commits changes to the objects type to the controller.
 *
 *  If the type is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_type_commit_changes ( gui_textedit_t *this_ );

/*!
 *  \brief commits changes to the objects description to the controller.
 *
 *  If the description is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_description_commit_changes ( gui_textedit_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_textedit_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_name_update_view ( gui_textedit_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_textedit_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_stereotype_update_view ( gui_textedit_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_textedit_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_type_update_view ( gui_textedit_t *this_ );

/*!
 *  \brief redraws the widget using the cached data loaded by gui_textedit_private_load_object from the database.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_private_description_update_view ( gui_textedit_t *this_ );

#endif  /* GUI_TEXTEDIT_H */


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
