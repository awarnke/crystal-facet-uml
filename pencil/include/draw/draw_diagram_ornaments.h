/* File: draw_diagram_ornaments.h; Copyright and License: see below */

#ifndef DRAW_DIAGRAM_ORNAMENTS_H
#define DRAW_DIAGRAM_ORNAMENTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws simple geometric objects like a scale for x axis
 */

#include "pencil_size.h"
#include "geometry/geometry_rectangle.h"
#include "entity/data_diagram_type.h"
#include "entity/data_diagram.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw geometry functions
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct draw_diagram_ornaments_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_diagram_ornaments_struct draw_diagram_ornaments_t;

/*!
 *  \brief initializes the draw_diagram_ornaments_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_diagram_ornaments_init( draw_diagram_ornaments_t *this_ );

/*!
 *  \brief destroys the draw_diagram_ornaments_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_diagram_ornaments_destroy( draw_diagram_ornaments_t *this_ );

/*!
 *  \brief draws a rounded-corner-rectangle shape into the outer bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the x axis scale
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_diagram_ornaments_draw_scale ( const draw_diagram_ornaments_t *this_,
                                         const geometry_rectangle_t *bounds,
                                         const pencil_size_t *pencil_size,
                                         cairo_t *cr
                                       );

#endif  /* DRAW_DIAGRAM_ORNAMENTS_H */


/*
Copyright 2026-2026 Andreas Warnke

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
