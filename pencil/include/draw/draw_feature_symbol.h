/* File: draw_feature_symbol.h; Copyright and License: see below */

#ifndef DRAW_FEATURE_SYMBOL_H
#define DRAW_FEATURE_SYMBOL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws simple geometric objects like a lifeline with activity bar
 */

#include "pencil_size.h"
#include "geometry/geometry_rectangle.h"
#include "entity/data_diagram_type.h"
#include "entity/data_diagram.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the draw geometry functions
 *
 *  \note This class is nearly stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 *        Only all segments of the same lifeline are expected to be drawn in one backwards-directed sequence.
 */
struct draw_feature_symbol_struct {
    uint32_t last_depth;  /*!< This is the last depth of the execution order of the current lifeline */
    double last_location;   /*!< This is the last y coordinate of an execution order or the last x coordinate of a timeline */
};

typedef struct draw_feature_symbol_struct draw_feature_symbol_t;

/*!
 *  \brief initializes the draw_feature_symbol_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_feature_symbol_init( draw_feature_symbol_t *this_ );

/*!
 *  \brief resets the draw_feature_symbol_t to start drawing a different lifeline
 *
 *  \param this_ pointer to own object attributes
 */
void draw_feature_symbol_reinit( draw_feature_symbol_t *this_ );

/*!
 *  \brief destroys the draw_feature_symbol_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_feature_symbol_destroy( draw_feature_symbol_t *this_ );

/*!
 *  \brief draws a section of a lifeline
 *
 *  The lifeline may possibly show one or more execution specifications (activity bar).
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the lifeline
 *  \param to_y upper end coordinate of the section to be drawn
 *  \param depth depth of execution specifications on the section
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_feature_symbol_draw_execution_spec ( draw_feature_symbol_t *this_,
                                               const geometry_rectangle_t *bounds,
                                               double to_y,
                                               uint32_t depth,
                                               const pencil_size_t *pencil_size,
                                               cairo_t *cr
                                             );

/*!
 *  \brief draws a section of a lifeline
 *
 *  The lifeline may possibly show a timeline.
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the lifeline
 *  \param to_x left end coordinate of the section to be drawn
 *  \param active true if a timeline shall be shown
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_feature_symbol_draw_timeline ( draw_feature_symbol_t *this_,
                                         const geometry_rectangle_t *bounds,
                                         double to_x,
                                         bool active,
                                         const pencil_size_t *pencil_size,
                                         cairo_t *cr
                                       );

/*!
 *  \brief draws a rectangular lifeline
 *
 *  The lifeline may be used in interaction overview diagrams or in communication diagrams.
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding rectangle of the lifeline
 *  \param highlighted true if a lifeline shall be shown in green
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param cr a cairo drawing context
 */
void draw_feature_symbol_draw_life_rectangle ( const draw_feature_symbol_t *this_,
                                               const geometry_rectangle_t *bounds,
                                               bool highlighted,
                                               const pencil_size_t *pencil_size,
                                               cairo_t *cr
                                             );

#endif  /* DRAW_FEATURE_SYMBOL_H */


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
