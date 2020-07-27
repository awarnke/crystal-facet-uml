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
#include "util/geometry/geometry_rectangle.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the diagram image file exporter
 */
struct image_format_writer_struct {
    geometry_rectangle_t bounds;  /*!< bounding box of the exported images */
    pencil_diagram_maker_t painter;  /*!< own instance of a diagram painter */
    //data_visible_set_t *input_data;  /*!< pointer to an external buffer for private use as data cache */
};

typedef struct image_format_writer_struct image_format_writer_t;

/*!
 *  \brief initializes the diagram image file exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to an external buffer for private use as data cache
 */
void image_format_writer_init( image_format_writer_t *this_,
                               data_visible_set_t *input_data
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
 *  \param export_type image file format
 *  \param target_filename path name of the file to store the cairo surface
 *  \result 0 in case of success, -1 otherwise
 */
int image_format_writer_render_surface_to_file( image_format_writer_t *this_,
                                                io_file_format_t export_type,
                                                const char* target_filename
                                              );

#endif  /* IMAGE_FORMAT_WRITER_H */


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
