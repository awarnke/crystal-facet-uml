/* File: io_diagram_image_exporter.h; Copyright and License: see below */

#ifndef IO_DIAGRAM_IMAGE_EXPORTER_H
#define IO_DIAGRAM_IMAGE_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports a single diagram
 */

#include "io_file_format.h"
#include "pencil_diagram_maker.h"
#include "set/data_visible_set.h"
#include "util/geometry/geometry_rectangle.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the diagram image file exporter
 */
struct io_diagram_image_exporter_struct {
    geometry_rectangle_t bounds;  /*!< bounding box of the exported images */
    pencil_diagram_maker_t painter;  /*!< own instance of a diagram painter */
    data_visible_set_t *input_data;  /*!< pointer to external cache of diagram data */
};

typedef struct io_diagram_image_exporter_struct io_diagram_image_exporter_t;

/*!
 *  \brief initializes the diagram image file exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to cache of diagram data
 */
void io_diagram_image_exporter_init( io_diagram_image_exporter_t *this_,
                                     data_visible_set_t *input_data
                                   );

/*!
 *  \brief destroys the diagram image file exporter
 *
 *  \param this_ pointer to own object attributes
 */
void io_diagram_image_exporter_destroy( io_diagram_image_exporter_t *this_ );

/*!
 *  \brief creates one cairo surface to render a diagram into a file
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param target_filename path name of the file to store the cairo surface
 *  \result 0 in case of success, -1 otherwise
 */
int io_diagram_image_exporter_render_surface_to_file( io_diagram_image_exporter_t *this_,
                                                      io_file_format_t export_type,
                                                      const char* target_filename
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
