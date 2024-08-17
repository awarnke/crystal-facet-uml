/* File: pencil_marker.h; Copyright and License: see below */

#ifndef PENCIL_MARKER_H
#define PENCIL_MARKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Marks objects by colors and/or lines/ornaments
 */

#include "geometry/geometry_rectangle.h"
#include "entity/data_diagram.h"
#include "set/data_small_set.h"
#include "entity/data_id.h"
#include "set/data_visible_set.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the marker
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_marker_struct {
    void *dummy;
};

typedef struct pencil_marker_struct pencil_marker_t;

/*!
 *  \brief initializes the marker
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_marker_init( pencil_marker_t *this_ );

/*!
 *  \brief destroys the marker
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_marker_destroy( pencil_marker_t *this_ );

/*!
 *  \brief draws markers around the destination area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param rect the rectangle which to mark as focused. Drawings may be done outside.
 *  \param cr a cairo drawing context
 */
void pencil_marker_mark_focused_rectangle ( const pencil_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr );

/*!
 *  \brief draws markers around the destination area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param rect the rectangle which to mark as selected. Drawings may be done outside.
 *  \param cr a cairo drawing context
 */
void pencil_marker_mark_selected_rectangle ( const pencil_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr );

#endif  /* PENCIL_MARKER_H */


/*
Copyright 2016-2024 Andreas Warnke

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
