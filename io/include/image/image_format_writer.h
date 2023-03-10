/* File: image_format_writer.h; Copyright and License: see below */

#ifndef IMAGE_FORMAT_WRITER_H
#define IMAGE_FORMAT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports a single diagram to a single file
 */

#include "io_file_format.h"
#include "pencil_diagram_maker.h"
#include "set/data_visible_set.h"
#include "set/data_profile_part.h"
#include "set/data_stat.h"
#include "geometry/geometry_rectangle.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the diagram image file exporter
 */
struct image_format_writer_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_visible_set_t *input_data;  /*!< pointer to an external buffer for private use as data cache */
    data_profile_part_t *profile;  /*!< the stereotypes referenced from the current diagram */
    geometry_rectangle_t bounds;  /*!< bounding box of the exported images */
    pencil_diagram_maker_t painter;  /*!< own instance of a diagram painter */
};

typedef struct image_format_writer_struct image_format_writer_t;

/*!
 *  \brief initializes the diagram image file exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param input_data pointer to an external buffer for private use as data cache
 *  \param profile the stereotypes referenced from the current diagram
 */
void image_format_writer_init( image_format_writer_t *this_,
                               data_database_reader_t *db_reader,
                               data_visible_set_t *input_data,
                               data_profile_part_t *profile
                             );

/*!
 *  \brief destroys the diagram image file exporter
 *
 *  \param this_ pointer to own object attributes
 */
void image_format_writer_destroy( image_format_writer_t *this_ );

/*!
 *  \brief creates one cairo surface to render a diagram into a file
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram which to process for export
 *  \param export_type image file format
 *  \param target_filename path name of the file to store the cairo surface
 *  \param io_render_stat pointer to already initialized statistics object where rendering statistics are added
 *  \return 0 in case of success, -1 otherwise
 */
int image_format_writer_render_diagram_to_file( image_format_writer_t *this_,
                                                data_id_t diagram_id,
                                                io_file_format_t export_type,
                                                const char* target_filename,
                                                data_stat_t *io_render_stat
                                              );

/*!
 *  \brief creates one cairo surface to render a diagram into a file
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_filename path name of the file to store the cairo surface
 *  \param io_render_stat pointer to already initialized statistics object where rendering statistics are added
 *  \return 0 in case of success, -1 otherwise
 */
int image_format_writer_private_render_surface_to_file( image_format_writer_t *this_,
                                                        io_file_format_t export_type,
                                                        const char* target_filename,
                                                        data_stat_t *io_render_stat
                                                      );

#endif  /* IMAGE_FORMAT_WRITER_H */


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
