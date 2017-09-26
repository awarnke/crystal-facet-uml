/* File: gui_textedit.h; Copyright and License: see below */

#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides data to editing widgets and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "storage/data_database_reader.h"
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
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */

    data_id_t selected_object_id;  /*!< id of the object which is currently edited */
    data_diagram_t private_diagram_cache;  /*!< own instance of a diagram cache */
    data_classifier_t private_classifier_cache;  /*!< own instance of a classifier cache */
    data_feature_t private_feature_cache;  /*!< own instance of a feature cache */
    data_relationship_t private_relationship_cache;  /*!< own instance of a relationship cache */

    GtkListStore *diagram_types;
    GtkListStore *classifier_types;
    GtkListStore *relationship_types;
    GtkListStore *feature_types;

    GtkEntry *name_entry;  /*!< pointer to external text entry widget */
    GtkTextView *description_text_view;  /*!< pointer to external text entry widget */
    GtkEntry *stereotype_entry;  /*!< pointer to external text entry widget */
};

typedef struct gui_textedit_struct gui_textedit_t;

/*!
 *  \brief initializes the gui_textedit_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param name_entry  pointer to text entry widget
 *  \param description_text_view pointer to text entry widget
 *  \param stereotype_entry pointer to external text entry widget
 *  \param controller pointer to the controller object to use
 *  \param db_reader pointer to the database reader object to use
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_textedit_init ( gui_textedit_t *this_,
                         GtkEntry *name_entry,
                         GtkTextView *description_text_view,
                         GtkEntry *stereotype_entry,
                         ctrl_controller_t *controller,
                         data_database_reader_t *db_reader,
                         gui_simple_message_to_user_t *message_to_user
                       );

/*!
 *  \brief destroys the gui_textedit_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_textedit_destroy ( gui_textedit_t *this_ );

/*!
 *  \brief gets the diagram_types attribute
 *
 *  \param this_ pointer to own object attributes
 */
static inline GtkTreeModel *gui_textedit_get_diagram_types_ptr ( gui_textedit_t *this_ );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
gboolean gui_textedit_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
gboolean gui_textedit_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data );

/*!
 *  \brief callback that informs that the focus of a widget is lost
 */
gboolean gui_textedit_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data );

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
void gui_textedit_description_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that another object was selected
 */
void gui_textedit_type_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that the type in the combo box changed
 */
void gui_textedit_type_changed_callback ( GtkComboBox *widget, gpointer user_data );

/*!
 *  \brief callback that informs that the commit button was pressed
 */
void gui_textedit_commit_clicked_callback (GtkButton *button, gpointer user_data );

/*!
 *  \brief loads an object into cache.
 *
 *  \param this_ pointer to own object attributes
 *  \param id identifier of the object to be loaded
 *  \param force_reload if false, the cache is only updated if the wrong object was cached before.
 */
void gui_textedit_private_load_object ( gui_textedit_t *this_, data_id_t id, bool force_reload );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_name_data_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_stereotype_data_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_description_data_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief callback that informs that the data of an object changed
 */
void gui_textedit_type_data_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data );

/*!
 *  \brief commits changes to the objects name.
 *
 *  If the name is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 *  \param name_widget pointer to the gtk entry widget
 */
void gui_textedit_private_name_commit_changes ( gui_textedit_t *this_, GtkEntry *name_widget );

/*!
 *  \brief commits changes to the objects stereotype.
 *
 *  If the stereotype is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 *  \param stereotype_widget pointer to the gtk entry widget
 */
void gui_textedit_private_stereotype_commit_changes ( gui_textedit_t *this_, GtkEntry *stereotype_widget );

/*!
 *  \brief commits changes to the objects description.
 *
 *  If the description is not modified, nothing happens.
 *  \param this_ pointer to own object attributes
 *  \param description_widget pointer to the gtk text view widget
 */
void gui_textedit_private_description_commit_changes ( gui_textedit_t *this_, GtkTextView *description_widget );

#include "gui_textedit.inl"

#endif  /* GUI_TEXTEDIT_H */


/*
Copyright 2016-2017 Andreas Warnke

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
