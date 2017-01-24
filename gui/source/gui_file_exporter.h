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
#include "pencil_diagram_maker.h"
#include "pencil_description_writer.h"
#include "pencil_input_data.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief constants of gui_file_exporter_t
 */
enum gui_file_exporter_const_enum {
    GUI_FILE_EXPORTER_CONST_EXPORT_TXT = 0x70a0,  /*!< response code to export txt files */
    GUI_FILE_EXPORTER_CONST_EXPORT_PNG = 0x70a1,  /*!< response code to export png files */
    GUI_FILE_EXPORTER_CONST_EXPORT_SVG = 0x70a2,  /*!< response code to export svg files */
};

/*!
 *  \brief attributes of the file exporter
 */
struct gui_file_exporter_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */
    geometry_rectangle_t bounds;  /*!< bounding box of the exported images */
    pencil_input_data_t painter_input_data;  /*!< caches the diagram data */
    pencil_diagram_maker_t painter;  /*!< own instance of a diagram painter */
    pencil_description_writer_t description_writer;  /*!< own instance of a description writer */

    char temp_filename_buf[512];  /*!< buffer space for temporary filename construction */
    utf8stringbuf_t temp_filename;  /*!< buffer space for temporary filename construction */
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
 *  \brief renders diagrams and exports these to picture (or text) files
 *  \param this_ pointer to own object attributes
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

/*!
 *  \brief appends all characters that are valid within a filename to filename
 *  \param this_ pointer to own object attributes
 *  \param name name of the object, after wjich the file shall be named
 *  \param filename filename stringbuffer to which to write the valid characters
 */
void gui_file_exporter_private_append_valid_chars_to_filename( gui_file_exporter_t *this_, const char *name, utf8stringbuf_t filename );

#endif  /* GUI_FILE_EXPORTER_H */


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
