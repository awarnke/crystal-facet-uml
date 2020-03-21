/* File: io_exporter.h; Copyright and License: see below */

#ifndef IO_EXPORTER_H
#define IO_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports all diagrams and documents
 */

#include "io_file_format.h"
#include "io_diagram_image_exporter.h"
#include "io_format_writer.h"
#include "storage/data_database.h"
#include "pencil_diagram_maker.h"
#include "io_diagram_text_exporter.h"
#include "set/data_visible_set.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file exporter
 */
struct io_exporter_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    data_visible_set_t input_data;  /*!< caches the diagram data */
    io_diagram_image_exporter_t diagram_image_exporter;  /*!< exports single diagram images to one given file */
    io_diagram_text_exporter_t diagram_text_exporter;  /*!< own instance of a diagram_text_exporter */

    io_format_writer_t temp_format_writer;  /*!< possibly uninitialized memory for a format writer */
    char temp_filename_buf[512];  /*!< buffer space for temporary filename construction */
    utf8stringbuf_t temp_filename;  /*!< buffer space for temporary filename construction */
};

typedef struct io_exporter_struct io_exporter_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 */
void io_exporter_init( io_exporter_t *this_,
                       data_database_reader_t *db_reader
                     );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void io_exporter_destroy( io_exporter_t *this_ );

/*!
 *  \brief renders diagrams and exports these to picture (or text) files
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_folder path name to a folder where to store the images
 *  \param document_file_path path to the central/main document file
 *  \result 0 in case of success, -1 otherwise
 */
int io_exporter_export_files( io_exporter_t *this_,
                              io_file_format_t export_type,
                              const char* target_folder,
                              const char* document_file_path
                            );

/*!
 *  \brief extracts the base file name without extension from the given path
 *  \param this_ pointer to own object attributes
 *  \param path path name to a file from which to extract the base file name
 *  \param out_base_filename name of the file - without path and without filename-suffix
 *  \result 0 in case of success, -1 otherwise, e.g if filename is empty or too long
 */
int io_exporter_private_get_filename( io_exporter_t *this_,
                                      const char* path,
                                      utf8stringbuf_t out_base_filename
                                    );

/*!
 *  \brief renders diagrams and exports these to picture (or text) files
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to export; DATA_ID_VOID_ID to export all root diagrams
 *  \param max_recursion if greater than 0 and children exist, this function calls itself recursively
 *  \param export_type image file format
 *  \param target_folder path name to a folder where to store the images
 *  \result 0 in case of success, -1 otherwise
 */
int io_exporter_private_export_image_files( io_exporter_t *this_,
                                            int64_t diagram_id,
                                            uint32_t max_recursion,
                                            io_file_format_t export_type,
                                            const char* target_folder
                                          );

/*!
 *  \brief creates a document file
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_folder directory where to write the document to
 *  \param document_file_name name of the central/main document file (without filename-suffix)
 *  \result 0 in case of success, -1 otherwise
 */
int io_exporter_private_export_document_file( io_exporter_t *this_,
                                              io_file_format_t export_type,
                                              const char* target_folder,
                                              const char* document_file_name
                                            );

/*!
 *  \brief creates a document part
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to export; DATA_ID_VOID_ID to export all root diagrams
 *  \param max_recursion if greater than 0 and children exist, this function calls itself recursively
 *  \param format_writer writer to format the data and stream it out to a file
 *  \result 0 in case of success, -1 otherwise
 */
int io_exporter_private_export_document_part( io_exporter_t *this_,
                                              int64_t diagram_id,
                                              uint32_t max_recursion,
                                              io_format_writer_t *format_writer
                                            );

/*!
 *  \brief appends all characters that are valid within a filename to filename
 *  \param this_ pointer to own object attributes
 *  \param name name of the object, after wjich the file shall be named
 *  \param filename filename stringbuffer to which to write the valid characters
 */
void io_exporter_private_append_valid_chars_to_filename( io_exporter_t *this_,
                                                         const char *name,
                                                         utf8stringbuf_t filename
                                                       );

#endif  /* IO_EXPORTER_H */


/*
Copyright 2016-2020 Andreas Warnke

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
