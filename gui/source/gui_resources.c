/* File: gui_resources.c; Copyright and License: see below */

#include "gui_resources.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdbool.h>

/* ugly code: including a c file */
#include "resources/tool_create_diagram.c"

void gui_resources_init ( gui_resources_t *this_ )
{
    TRACE_BEGIN();

    (*this_).tool_create_diagram = gdk_pixbuf_new_from_data(
        &(gimp_image.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        false,
        8,
        gimp_image.width,
        gimp_image.height,
        gimp_image.width * gimp_image.bytes_per_pixel,
        NULL,
        NULL
    );

    TRACE_END();
}

/*!
 *  \brief destroys the gui_resources_t
 */
void gui_resources_destroy ( gui_resources_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}


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
