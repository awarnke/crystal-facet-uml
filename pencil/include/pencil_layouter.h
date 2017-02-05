/* File: pencil_layouter.h; Copyright and License: see below */

#ifndef PENCIL_LAYOUTER_H
#define PENCIL_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of classifiers, features and relationships in a diagram
 */

#include "pencil_input_data.h"
#include "pencil_marker.h"
#include "pencil_classifier_painter.h"
#include "pencil_size.h"
#include "pencil_input_data_layout.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include "data_id.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the layouter
 */
struct pencil_layouter_struct {
    pencil_input_data_t *input_data;  /*!< pointer to an external data cache */
    pencil_input_data_layout_t layout_data;  /* own instance of layout data */

    geometry_rectangle_t diagram_bounds;  /*!< own instance of a boundary rectangle defining the diagram border */
    pencil_size_t pencil_size;  /*!< own instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */
    geometry_rectangle_t diagram_draw_area;  /*!< own instance of a drawing rectangle containing inner contents of the diagram */
    geometry_non_linear_scale_t x_scale;  /*!< own instance of a scale object for the x-axis */
    geometry_non_linear_scale_t y_scale;  /*!< own instance of a scale object for the y-axis */
    geometry_rectangle_t default_classifier_size;  /*!< own instance of a classifier default size rectangle */
};

typedef struct pencil_layouter_struct pencil_layouter_t;

/*!
 *  \brief initializes the layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 */
void pencil_layouter_init( pencil_layouter_t *this_, pencil_input_data_t *input_data );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_layouter_destroy( pencil_layouter_t *this_ );

/*!
 *  \brief layouts the grid
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 *  \param diagram_bounds the diagram_bounds rectangle where to draw the diagram
 */
void pencil_layouter_layout_grid ( pencil_layouter_t *this_,
                                   pencil_input_data_t *input_data,
                                   geometry_rectangle_t diagram_bounds
);

/*!
 *  \brief layouts the chosen diagram contents into the diagram_bounds area
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_layouter_layout_elements ( pencil_layouter_t *this_ );

/*!
 *  \brief returns the pencil_input_data_layout_t object
 *
 *  \param this_ pointer to own object attributes
 */
static inline pencil_input_data_layout_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief returns the pencil size object
 *
 *  \param this_ pointer to own object attributes
 */
static inline pencil_size_t *pencil_layouter_get_pencil_size_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief returns the diagram bounds object
 *
 *  \param this_ pointer to own object attributes
 */
static inline geometry_rectangle_t *pencil_layouter_get_diagram_bounds_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param dereference true if the real, dereferenced object shall be returned (e.g. data_classifier_t or data_diagram_t),
 *                     false if the visible object shall be returned (e.g. data_diagramelement_t or data_diagram_t)
 *  \return an object id. The id is invalid if there is no object at the given location.
 */
data_id_t pencil_layouter_get_object_id_at_pos ( pencil_layouter_t *this_,
                                                 double x,
                                                 double y,
                                                 bool dereference
                                               );

/*!
 *  \brief gets the classifier-id of the classifier at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param dereference true if the real, dereferenced object shall be returned (e.g. data_classifier_t or data_diagram_t),
 *                     false if the visible object shall be returned (e.g. data_diagramelement_t or data_diagram_t)
 *  \return an object id. The id is invalid if there is no object at the given location.
 */
data_id_t pencil_layouter_private_get_classifier_id_at_pos ( pencil_layouter_t *this_,
                                                             double x,
                                                             double y,
                                                             bool dereference
                                                           );

/*!
 *  \brief gets the order values at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return the x- and y- order values as first and second element of the pair
 */
static inline universal_int32_pair_t pencil_layouter_get_order_at_pos ( pencil_layouter_t *this_,
                                                                        double x,
                                                                        double y
                                                                      );

/*!
 *  \brief determines if the given position is on a grid line
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return a pair of bool values indicating if x- and y- position values are on grid lines
 */
static inline universal_bool_list_t pencil_layouter_is_pos_on_grid ( pencil_layouter_t *this_,
                                                                     double x,
                                                                     double y
                                                                   );

/*!
 *  \brief layouts a connection from one rectangle to another
 *
 *  \param this_ pointer to own object attributes
 *  \param source_rect pointer to the source rectangle
 *  \param dest_rect pointer to the destination rectangle
 *  \param out_connection pointer to an already initialized geometry_connector_t which is overwritten
 */
void pencil_layouter_private_connect_rectangles ( pencil_layouter_t *this_,
                                                  const geometry_rectangle_t *source_rect,
                                                  const geometry_rectangle_t *dest_rect,
                                                  geometry_connector_t *out_connection
                                                );

#include "pencil_layouter.inl"

#endif  /* PENCIL_LAYOUTER_H */


/*
Copyright 2017-2017 Andreas Warnke

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