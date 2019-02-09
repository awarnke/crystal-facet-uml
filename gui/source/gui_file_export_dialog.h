/* File: gui_file_export_dialog.h; Copyright and License: see below */

#ifndef GUI_FILE_EXPORT_DIALOG_H
#define GUI_FILE_EXPORT_DIALOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a file export dialog and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "gui_file_exporter.h"
#include "storage/data_database.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file export dialog
 */
struct gui_file_export_dialog_struct {
    gui_file_exporter_t file_exporter;  /*!<  own instance of gui_file_exporter_t */

    GtkWidget *export_file_chooser;  /*!< pointer to instance of a file chooser for export */
};

typedef struct gui_file_export_dialog_struct gui_file_export_dialog_t;

/*!
 *  \brief initializes the file export dialog
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_file_export_dialog_init( gui_file_export_dialog_t *this_,
                                  data_database_reader_t *db_reader,
                                  GtkWindow *parent_window,
                                  gui_simple_message_to_user_t *message_to_user
                                );

/*!
 *  \brief destroys the file export dialog
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_export_dialog_destroy( gui_file_export_dialog_t *this_ );

/*!
 *  \brief shows the file export dialog
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_export_dialog_show( gui_file_export_dialog_t *this_ );

#endif  /* GUI_FILE_EXPORT_DIALOG_H */


/*
Copyright 2019-2019 Andreas Warnke

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
