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
#include "pencil_diagram_painter.h"
#include "pencil_feature_painter.h"
#include "pencil_relationship_layouter.h"
#include "pencil_classifier_layouter.h"
#include "pencil_error.h"
#include "pencil_visible_object_id.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include "data_id.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include "universal_array_index_sorter.h"
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

    pencil_diagram_painter_t diagram_painter;  /*!< own instance of a painter object to ask for display dimensions */

    pencil_relationship_layouter_t pencil_relationship_layouter;  /*!< own instance of a helper object to layout relationships */
    pencil_classifier_layouter_t pencil_classifier_layouter;  /*!< own instance of a helper object to layout classifiers and features */
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
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_layouter_layout_elements ( pencil_layouter_t *this_, PangoLayout *font_layout );

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
 *  \return pointer to rectangle of diagram bounds
 */
static inline geometry_rectangle_t *pencil_layouter_get_diagram_bounds_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief returns the newly calculated feature bounds object
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id pointer to own object attributes
 *  \param c_index index of the classifier in the classifier array at input_data; 0 <= c_index < pencil_input_data_get_visible_classifier_count ( (*this_).input_data )
 *  \param f_index index of the feature in the feature array at input_data; 0 <= f_index < pencil_input_data_get_feature_count ( (*this_).input_data )
 *  \param line_index line number of the feature at the selected classifier. first line has index 0.
 *  \return rectangle of feature bounds
 */
static inline geometry_rectangle_t pencil_layouter_get_feature_bounds ( pencil_layouter_t *this_,
                                                                        int64_t classifier_id,
                                                                        uint32_t c_index,
                                                                        uint32_t f_index,
                                                                        uint32_t line_index
                                                                      );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next connector line when to select the connector
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram.
 */
pencil_error_t pencil_layouter_get_object_id_at_pos ( pencil_layouter_t *this_,
                                                      double x,
                                                      double y,
                                                      double snap_distance,
                                                      pencil_visible_object_id_t* out_selected_id,
                                                      pencil_visible_object_id_t* out_surrounding_id
                                                    );

/*!
 *  \brief gets the classifier-id of the classifier at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram.
 */
pencil_error_t pencil_layouter_private_get_classifier_id_at_pos ( pencil_layouter_t *this_,
                                                                  double x,
                                                                  double y,
                                                                  pencil_visible_object_id_t* out_selected_id,
                                                                  pencil_visible_object_id_t* out_surrounding_id
                                                                );

/*!
 *  \brief gets the order values at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next grid line when to snap to a grid position
 *  \param out_order_x x-order value at given x-position
 *  \param out_order_y y-order value at given y-position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram.
 */
static inline pencil_error_t pencil_layouter_get_order_at_pos ( pencil_layouter_t *this_,
                                                                double x,
                                                                double y,
                                                                double snap_distance,
                                                                int32_t *out_order_x,
                                                                int32_t *out_order_y
                                                              );

/*!
 *  \brief determines if the given position is on a grid line
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next grid line when to evaluate to "yes, on grid"
 *  \param out_x_on_grid flag indicating if the given x position is on a grid line
 *  \param out_y_on_grid flag indicating if the given y position is on a grid line
 */
static inline void pencil_layouter_is_pos_on_grid ( pencil_layouter_t *this_,
                                                    double x,
                                                    double y,
                                                    double snap_distance,
                                                    bool *out_x_on_grid,
                                                    bool *out_y_on_grid
                                                  );

/*!
 *  \brief proposes a default classifier bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_layouter_private_propose_default_classifier_size ( pencil_layouter_t *this_ );


#include "pencil_layouter.inl"

#endif  /* PENCIL_LAYOUTER_H */


/*
Copyright 2017-2018 Andreas Warnke

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
