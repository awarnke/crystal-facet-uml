/* File: gui_file_db_manager.h; Copyright and License: see below */

#ifndef GUI_FILE_DB_MANAGER_H
#define GUI_FILE_DB_MANAGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Switches the currently used database
 */

#include "gui_simple_message_to_user.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file manager
 */
struct gui_file_db_manager_struct {
    ctrl_controller_t *controller;  /*!< pointer to external ctrl_controller_t */
    data_database_t *database;  /*!< pointer to external data_database_t */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */
};

typedef struct gui_file_db_manager_struct gui_file_db_manager_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to the controller object to use
 *  \param database pointer to the database object to use
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_file_db_manager_init( gui_file_db_manager_t *this_,
                               ctrl_controller_t *controller,
                               data_database_t *database,
                               gui_simple_message_to_user_t *message_to_user
                             );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_db_manager_destroy( gui_file_db_manager_t *this_ );

/*!
 *  \brief callback function of a GtkDialog
 */
void gui_file_db_manager_use_db_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data );

#endif  /* GUI_FILE_DB_MANAGER_H */


/*
Copyright 2016-2021 Andreas Warnke

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
