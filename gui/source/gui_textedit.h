/* File: gui_textedit.h; Copyright and License: see below */

#ifndef GUI_TEXTEDIT_H
#define GUI_TEXTEDIT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides data to editing widgets and reacts on user events
 */

#include "data_database_reader.h"
#include "ctrl_controller.h"
#include "data_id.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the gui_textedit_t
 */
struct gui_textedit_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */

    data_diagram_t private_diagram_cache;  /*!< own instance of a diagram chace */

    GtkListStore *diagram_types;
    GtkListStore *classifier_types;
    GtkListStore *relationship_types;
};

typedef struct gui_textedit_struct gui_textedit_t;

/*!
 *  \brief initializes the gui_textedit_t struct
 */
void gui_textedit_init ( gui_textedit_t *this_, ctrl_controller_t *controller, data_database_reader_t *db_reader );

/*!
 *  \brief destroys the gui_textedit_t struct
 */
void gui_textedit_destroy ( gui_textedit_t *this_ );

/*!
 *  \brief gets the diagram_types attribute
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
 *  \param id identifier of the object to be loaded
 *  \param force_reload if false, the cache is only updated if the wrong object was cached before.
 */
void gui_textedit_private_load_object ( gui_textedit_t *this_, data_id_t id, bool force_reload );

#include "gui_textedit.inl"

#endif  /* GUI_TEXTEDIT_H */


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
