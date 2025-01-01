/* File: gui_file_action.h; Copyright and License: see below */

#ifndef GUI_FILE_ACTION_H
#define GUI_FILE_ACTION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Switches the currently used data_file
 */

#include "gui_simple_message_to_user.h"
#include "wrap/gui_button.h"
#include "io_data_file.h"
#include "ctrl_controller.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file manager
 */
struct gui_file_action_struct {
    ctrl_controller_t *controller;  /*!< pointer to external ctrl_controller_t */
    io_data_file_t *data_file;  /*!< pointer to external io_data_file_t */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */
};

typedef struct gui_file_action_struct gui_file_action_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to the controller object to use
 *  \param data_file pointer to the data_file object to use
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_file_action_init( gui_file_action_t *this_,
                           ctrl_controller_t *controller,
                           io_data_file_t *data_file,
                           gui_simple_message_to_user_t *message_to_user
                         );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_action_destroy( gui_file_action_t *this_ );

/*!
 *  \brief switches the data_file to the new file
 *
 *  \param this_ pointer to own object attributes
 *  \param filename filename of the file to open, must not be NULL
 */
u8_error_t gui_file_action_use_db( gui_file_action_t *this_, const char *filename );

/*!
 *  \brief saves the data_file
 *
 *  Errors are reported directly to the user via (*this_).message_to_user.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE if save was successful
 */
u8_error_t gui_file_action_save( gui_file_action_t *this_ );

#endif  /* GUI_FILE_ACTION_H */


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
