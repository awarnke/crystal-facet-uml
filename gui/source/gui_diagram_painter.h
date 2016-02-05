/* File: gui_diagram_painter.h; Copyright and License: see below */

#ifndef GUI_DIAGRAM_PAINTER_H
#define GUI_DIAGRAM_PAINTER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_database.h"
#include "trace.h"
#include <cairo.h>
#include <stdint.h>

/*!
 * \brief draws the chosen diagram contents into the clip area of the cairo drawing context
 */
void gui_diagram_painter_draw ( data_database_t *db, int32_t diagram_id, cairo_t *cr );

#endif  /* GUI_DIAGRAM_PAINTER_H */


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
