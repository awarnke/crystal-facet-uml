/* File: pencil_feature_painter.h; Copyright and License: see below */

#ifndef PENCIL_FEATURE_PAINTER_H
#define PENCIL_FEATURE_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a feature into a cairo drawing context
 */

#include "pencil_input_data.h"
#include "pencil_marker.h"
#include "pencil_size.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include "data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the feature painter
 */
struct pencil_feature_painter_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
};

typedef struct pencil_feature_painter_struct pencil_feature_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_painter_init( pencil_feature_painter_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_painter_destroy( pencil_feature_painter_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier pointer to the data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param feature_bounds the destination rectangle where to draw the classifier
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_feature_painter_draw ( pencil_feature_painter_t *this_,
                                   data_feature_t *the_feature,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   pencil_size_t *pencil_size,
                                   geometry_rectangle_t *feature_bounds,
                                   PangoLayout *layout,
                                   cairo_t *cr
                                 );

#endif  /* PENCIL_FEATURE_PAINTER_H */


/*
Copyright 2016-2017 Andreas Warnke

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
