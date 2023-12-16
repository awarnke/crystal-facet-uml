/* File: io_exporter.h; Copyright and License: see below */

#ifndef IO_EXPORTER_H
#define IO_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports all diagrams and documents, defines filenames, traverses the diagram tree
 */

#include "io_file_format.h"
#include "image/image_format_writer.h"
#include "io_export_model_traversal.h"
#include "io_export_diagram_traversal.h"
#include "io_export_flat_traversal.h"
#include "document/document_element_writer.h"
#include "xmi/xmi_element_writer.h"
#include "json/json_element_writer.h"
#include "storage/data_database.h"
#include "pencil_diagram_maker.h"
#include "set/data_visible_set.h"
#include "set/data_profile_part.h"
#include "set/data_stat.h"
#include "geometry/geometry_rectangle.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_error.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file exporter
 *
 *  Lifecycle: An exporter may perform multiple export operations.
 *  It may be initialized at program start and live till program exit.
 */
struct io_exporter_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */

    /* temporary member attributes, only valid during exporting */
    data_visible_set_t temp_input_data;  /*!< buffer to cache the diagram data */
    data_profile_part_t temp_profile;  /*!< caches the stereotypes referenced from the current diagram */

    image_format_writer_t temp_image_format_exporter;  /*!< exports single diagram images to one given file */
    io_export_model_traversal_t temp_model_traversal;  /*!< own instance of a model_traversal for text export */
    io_export_diagram_traversal_t temp_diagram_traversal;  /*!< own instance of a diagram_traversal for text export */
    io_export_flat_traversal_t temp_flat_traversal;  /*!< own instance of a flat_nodes_traversal for json export */
    document_element_writer_t temp_format_writer;  /*!< memory for a temporary format writer */
    xmi_element_writer_t temp_xmi_writer;  /*!< memory for a temporary xmi writer */
    json_element_writer_t temp_json_writer;  /*!< memory for a temporary json writer */

    char temp_filename_buf[512];  /*!< buffer space for temporary filename construction */
    utf8stringbuf_t temp_filename;  /*!< buffer space for temporary filename construction */
    data_diagram_t temp_diagram;  /*!< buffer space for temporary diagram data */
};

typedef struct io_exporter_struct io_exporter_t;

/*!
 *  \brief initializes the exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 */
void io_exporter_init( io_exporter_t *this_,
                       data_database_reader_t *db_reader
                     );

/*!
 *  \brief destroys the exporter
 *
 *  \param this_ pointer to own object attributes
 */
void io_exporter_destroy( io_exporter_t *this_ );

/*!
 *  \brief renders diagrams and exports these to picture (or text) files
 *  \param this_ pointer to own object attributes
 *  \param export_type bitset of file formats to export
 *  \param target_folder path name to a folder where to store the images
 *  \param document_file_path path to the central/main document file
 *  \param io_export_stat pointer to already initialized statistics object where export statistics are collected
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_export_files( io_exporter_t *this_,
                                     io_file_format_t export_type,
                                     const char *target_folder,
                                     const char *document_file_path,
                                     data_stat_t *io_export_stat
                                   );

/*!
 *  \brief extracts the base file name without extension from the given path
 *  \param this_ pointer to own object attributes
 *  \param path path name to a file from which to extract the base file name
 *  \param out_base_filename name of the file - without path and without filename-suffix
 *  \return U8_ERROR_NONE in case of success, e.g if filename is empty or too long
 */
u8_error_t io_exporter_private_get_filename( io_exporter_t *this_,
                                             const char* path,
                                             utf8stringbuf_t out_base_filename
                                           );

/*!
 *  \brief renders diagrams and exports these to picture (or text) files, does recursion for child diagrams
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to export; DATA_ROW_ID_VOID to export all root diagrams
 *  \param max_recursion if greater than 0 and children exist, this function calls itself recursively
 *  \param export_type image file format
 *  \param target_folder path name to a folder where to store the images
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_private_export_image_files( io_exporter_t *this_,
                                                   data_id_t diagram_id,
                                                   uint32_t max_recursion,
                                                   io_file_format_t export_type,
                                                   const char *target_folder,
                                                   data_stat_t *io_export_stat
                                                 );

/*!
 *  \brief renders one diagram and exports it to a picture (or text) file
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to export; DATA_ROW_ID_VOID to export all root diagrams
 *  \param export_type image file format
 *  \param file_path path name to the export file
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_export_image_file( io_exporter_t *this_,
                                          data_id_t diagram_id,
                                          io_file_format_t export_type,
                                          const char *file_path,
                                          data_stat_t *io_export_stat
                                        );

/*!
 *  \brief creates a document file
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_folder directory where to write the document to
 *  \param document_file_name name of the central/main document file (without filename-suffix)
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_private_export_document_file( io_exporter_t *this_,
                                                     io_file_format_t export_type,
                                                     const char *target_folder,
                                                     const char *document_file_name,
                                                     data_stat_t *io_export_stat
                                                   );

/*!
 *  \brief creates a document file
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param document_title title of the document to export
 *  \param file_path path name to the export file
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_export_document_file( io_exporter_t *this_,
                                             io_file_format_t export_type,
                                             const char *document_title,
                                             const char *file_path,
                                             data_stat_t *io_export_stat
                                           );

/*!
 *  \brief creates a document part
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to export; DATA_ROW_ID_VOID to export all root diagrams
 *  \param max_recursion if greater than 0 and children exist, this function calls itself recursively
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_private_export_document_part( io_exporter_t *this_,
                                                     data_id_t diagram_id,
                                                     uint32_t max_recursion,
                                                     data_stat_t *io_export_stat
                                                   );

/*!
 *  \brief creates a table of contents
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to export; DATA_ROW_ID_VOID to export all root diagrams
 *  \param max_recursion if greater than 0 and children exist, this function calls itself recursively
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_private_export_table_of_contents( io_exporter_t *this_,
                                                         data_id_t diagram_id,
                                                         uint32_t max_recursion,
                                                         document_element_writer_t *format_writer
                                                       );

/*!
 *  \brief calculates a base filename (without type extension) for a givien diagram id
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagam
 *  \param filename filename stringbuffer to which to write the filename
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_exporter_private_get_filename_for_diagram( io_exporter_t *this_,
                                                         data_id_t diagram_id,
                                                         utf8stringbuf_t filename
                                                       );

/*!
 *  \brief appends all characters that are valid within a filename to filename
 *  \param this_ pointer to own object attributes
 *  \param name name of the object, after which the file shall be named
 *  \param filename filename stringbuffer to which to write the valid characters
 */
void io_exporter_private_append_valid_chars_to_filename( io_exporter_t *this_,
                                                         const char *name,
                                                         utf8stringbuf_t filename
                                                       );

#endif  /* IO_EXPORTER_H */


/*
Copyright 2016-2023 Andreas Warnke

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
