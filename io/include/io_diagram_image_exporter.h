/* File: io_diagram_image_exporter.h; Copyright and License: see below */

#ifndef IO_DIAGRAM_IMAGE_EXPORTER_H
#define IO_DIAGRAM_IMAGE_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports all diagrams
 */

#include "io_file_format.h"
#include "storage/data_database.h"
#include "pencil_diagram_maker.h"
#include "pencil_description_writer.h"
#include "pencil_input_data.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file exporter
 */
struct io_diagram_image_exporter_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    geometry_rectangle_t bounds;  /*!< bounding box of the exported images */
    pencil_input_data_t painter_input_data;  /*!< caches the diagram data */
    pencil_diagram_maker_t painter;  /*!< own instance of a diagram painter */
    pencil_description_writer_t description_writer;  /*!< own instance of a description writer */

    char temp_filename_buf[512];  /*!< buffer space for temporary filename construction */
    utf8stringbuf_t temp_filename;  /*!< buffer space for temporary filename construction */
};

typedef struct io_diagram_image_exporter_struct io_diagram_image_exporter_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 */
void io_diagram_image_exporter_init( io_diagram_image_exporter_t *this_,
                                     data_database_reader_t *db_reader
                                   );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void io_diagram_image_exporter_destroy( io_diagram_image_exporter_t *this_ );

/*!
 *  \brief renders diagrams and exports these to picture (or text) files
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_folder path name to a folder where to store the images
 *  \result 0 in case of success, -1 otherwise
 */
int io_diagram_image_exporter_export_files( io_diagram_image_exporter_t *this_,
                                            io_file_format_t export_type,
                                            const char* target_folder
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
int io_diagram_image_exporter_private_export_image_files( io_diagram_image_exporter_t *this_,
                                                          int64_t diagram_id,
                                                          uint32_t max_recursion,
                                                          io_file_format_t export_type,
                                                          const char* target_folder
                                                        );

/*!
 *  \brief creates one cairo surface to render a diagram into a file
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_filename path name of the file to store the cairo surface
 *  \result 0 in case of success, -1 otherwise
 */
int io_diagram_image_exporter_private_render_surface_to_file( io_diagram_image_exporter_t *this_,
                                                              io_file_format_t export_type,
                                                              const char* target_filename
                                                            );

/*!
 *  \brief appends all characters that are valid within a filename to filename
 *  \param this_ pointer to own object attributes
 *  \param name name of the object, after wjich the file shall be named
 *  \param filename filename stringbuffer to which to write the valid characters
 */
void io_diagram_image_exporter_private_append_valid_chars_to_filename( io_diagram_image_exporter_t *this_,
                                                                       const char *name,
                                                                       utf8stringbuf_t filename
                                                                     );

#endif  /* IO_DIAGRAM_IMAGE_EXPORTER_H */


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
