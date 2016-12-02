/* File: gui_file_exporter.h; Copyright and License: see below */

#ifndef GUI_FILE_EXPORTER_H
#define GUI_FILE_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports all diagrams
 */

#include "gui_simple_message_to_user.h"
#include "gui_file_export_format.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include <gtk/gtk.h>

/*!
 *  \brief constants of gui_file_exporter_t
 */
enum gui_file_manager_const_enum {
    GUI_FILEMANAGER_CONST_EXPORT_PNG = 0x70a1,  /*!< response code to export png files */
    GUI_FILEMANAGER_CONST_EXPORT_SVG = 0x70a2,  /*!< response code to export svg files */
};

/*!
 *  \brief attributes of the file exporter
 */
struct gui_file_exporter_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */
};

typedef struct gui_file_exporter_struct gui_file_exporter_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_file_exporter_init( gui_file_exporter_t *this_,
                             data_database_reader_t *db_reader,
                             gui_simple_message_to_user_t *message_to_user
                           );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_exporter_destroy( gui_file_exporter_t *this_ );

/*!
 *  \brief callback function of a GtkDialog
 */
void gui_file_exporter_export_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data );

/*!
 *  \brief renders diagrams and exports these to picture files
 *  \param diagram_id id of the diagram to export; DATA_ID_VOID_ID to export all root diagrams
 *  \param max_recursion if greater than 0 and children exist, this function calls itself recursively
 *  \param export_type image file format
 *  \param target_folder path name to a folder where to store the images
 *  \result 0 in case of success, -1 otherwise
 */
int gui_file_exporter_private_export_image_files( gui_file_exporter_t *this_,
                                                  int64_t diagram_id,
                                                  uint32_t max_recursion,
                                                  gui_file_export_format_t export_type,
                                                  const char* target_folder
                                                 );

#endif  /* GUI_FILE_EXPORTER_H */


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
