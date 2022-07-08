/* File: gui_file_use_db_dialog.h; Copyright and License: see below */

#ifndef GUI_FILE_USE_DB_DIALOG_H
#define GUI_FILE_USE_DB_DIALOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a use database file dialog and reacts on user events
 *
 *  This file dialog can either create a new data file or open an existing data file.
 */

#include "gui_simple_message_to_user.h"
#include "gui_file_db_manager.h"
#include "io_data_file.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the use database file dialog
 */
struct gui_file_use_db_dialog_struct {
    gui_file_db_manager_t file_manager;  /*!<  own instance of gui_file_db_manager_t */

    GtkWidget *use_db_file_chooser;  /*!< pointer to instance of a file chooser for use_db */
};

typedef struct gui_file_use_db_dialog_struct gui_file_use_db_dialog_t;

/*!
 *  \brief initializes the gui_file_use_db_dialog
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param data_file pointer to a data_file object
 *  \param parent_window pointer to the gtk parent window, to which this modal dialog belongs
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_file_use_db_dialog_init( gui_file_use_db_dialog_t *this_,
                                  ctrl_controller_t *controller,
                                  io_data_file_t *data_file,
                                  GtkWindow *parent_window,
                                  gui_simple_message_to_user_t *message_to_user
                                );

/*!
 *  \brief destroys the gui_file_use_db_dialog
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_use_db_dialog_destroy( gui_file_use_db_dialog_t *this_ );

/*!
 *  \brief shows the gui_file_use_db_dialog
 *
 *  \param this_ pointer to own object attributes
 *  \param open_existing true if only existing files shall be selectable
 */
void gui_file_use_db_dialog_show( gui_file_use_db_dialog_t *this_, bool open_existing );

/*!
 *  \brief callback function of a GtkDialog
 */
void gui_file_use_db_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data );

#endif  /* GUI_FILE_USE_DB_DIALOG_H */


/*
Copyright 2019-2022 Andreas Warnke

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
