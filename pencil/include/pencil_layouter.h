/* File: pencil_layouter.h; Copyright and License: see below */

#ifndef PENCIL_LAYOUTER_H
#define PENCIL_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of classifiers, features and relationships in a diagram
 */

#include "pencil_marker.h"
#include "pencil_classifier_composer.h"
#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "pencil_diagram_painter.h"
#include "pencil_feature_painter.h"
#include "pencil_feature_layouter.h"
#include "pencil_feat_label_layouter.h"
#include "pencil_relationship_layouter.h"
#include "pencil_rel_label_layouter.h"
#include "pencil_classifier_layouter.h"
#include "pencil_error.h"
#include "option/pencil_type_filter.h"
#include "layout/layout_order.h"
#include "set/data_id_pair.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_dimensions.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "data_id.h"
#include "set/data_visible_set.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include "universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the layouter
 */
struct pencil_layouter_struct {
    pencil_layout_data_t layout_data;  /* own instance of layout data */

    pencil_size_t pencil_size;  /*!< own instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */
    geometry_non_linear_scale_t x_scale;  /*!< own instance of a scale object for the x-axis */
    geometry_non_linear_scale_t y_scale;  /*!< own instance of a scale object for the y-axis */
    geometry_dimensions_t default_classifier_size;  /*!< own instance of a classifier default size */

    pencil_diagram_painter_t diagram_painter;  /*!< own instance of a painter object to ask for display dimensions */

    pencil_classifier_layouter_t pencil_classifier_layouter;  /*!< own instance of a helper object to layout classifiers */
    pencil_feature_layouter_t feature_layouter;  /*!< own instance of a helper object to layout features */
    pencil_feat_label_layouter_t feature_label_layouter;  /*!< own instance of a helper object to layout feature labels */
    pencil_relationship_layouter_t pencil_relationship_layouter;  /*!< own instance of a helper object to layout relationships */
    pencil_rel_label_layouter_t relationship_label_layouter;  /*!< own instance of a helper object to layout relationship labels */
};

typedef struct pencil_layouter_struct pencil_layouter_t;

/*!
 *  \brief initializes the layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 */
void pencil_layouter_init( pencil_layouter_t *this_, const data_visible_set_t *input_data );

/*!
 *  \brief re-initializes the layouter to layout new/other input_data
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the data to be layouted
 */
void pencil_layouter_reinit( pencil_layouter_t *this_, const data_visible_set_t *input_data );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_layouter_destroy( pencil_layouter_t *this_ );

/*!
 *  \brief resets previous layout data, synchronizes the internal layout data with the *input_data
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_layouter_prepare ( pencil_layouter_t *this_ );

/*!
 *  \brief defines the grid coordinates
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_bounds the diagram_bounds rectangle where to draw the diagram
 */
void pencil_layouter_define_grid ( pencil_layouter_t *this_, geometry_rectangle_t diagram_bounds );

/*!
 *  \brief layouts the chosen diagram contents into the diagram_bounds area
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_layouter_layout_elements ( pencil_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief returns the pencil_layout_data_t object
 *
 *  \param this_ pointer to own object attributes
 */
static inline pencil_layout_data_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief returns the pencil size object
 *
 *  \param this_ pointer to own object attributes
 */
static inline pencil_size_t *pencil_layouter_get_pencil_size_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param snap_distance maximum distance to the next connector line when to select the connector
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram.
 */
pencil_error_t pencil_layouter_get_object_id_at_pos ( const pencil_layouter_t *this_,
                                                      double x,
                                                      double y,
                                                      double snap_distance,
                                                      pencil_type_filter_t filter,
                                                      data_id_pair_t* out_selected_id,
                                                      data_id_pair_t* out_surrounding_id
                                                    );

/*!
 *  \brief gets the classifier-id of the classifier at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if no classifier is at the given position x, y.
 */
pencil_error_t pencil_layouter_private_get_classifier_id_at_pos ( const pencil_layouter_t *this_,
                                                                  double x,
                                                                  double y,
                                                                  data_id_pair_t* out_selected_id,
                                                                  data_id_pair_t* out_surrounding_id
                                                                );

/*!
 *  \brief gets the feature-id of the feature at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if no feature is at the given position x, y.
 */
pencil_error_t pencil_layouter_private_get_feature_id_at_pos ( const pencil_layouter_t *this_,
                                                               double x,
                                                               double y,
                                                               pencil_type_filter_t filter,
                                                               data_id_pair_t* out_selected_id,
                                                               data_id_pair_t* out_surrounding_id
                                                             );

/*!
 *  \brief gets the relationship-id of the relationship at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next connector line when to select the connector
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if no relationship is at the given position x, y.
 */
pencil_error_t pencil_layouter_private_get_relationship_id_at_pos ( const pencil_layouter_t *this_,
                                                                    double x,
                                                                    double y,
                                                                    double snap_distance,
                                                                    data_id_pair_t* out_selected_id
                                                                  );

/*!
 *  \brief gets the layout order for a classifier at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next grid line when to snap to a grid position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram
 */
pencil_error_t pencil_layouter_get_classifier_order_at_pos ( const pencil_layouter_t *this_,
                                                             double x,
                                                             double y,
                                                             double snap_distance,
                                                             layout_order_t* out_layout_order
                                                           );

/*!
 *  \brief gets the layout order for a feature at a given position (relative to a classifier)
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr pointer to the feature for which to determine the order at x/y
 *  \param x x-position
 *  \param y y-position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram,
 *          PENCIL_ERROR_UNKNOWN_OBJECT if the object is not in the diagram or has unsupported type
 */
pencil_error_t pencil_layouter_get_feature_order_at_pos ( const pencil_layouter_t *this_,
                                                          const data_feature_t *feature_ptr,
                                                          double x,
                                                          double y,
                                                          layout_order_t* out_layout_order
                                                        );

/*!
 *  \brief gets the layout order for a relationship at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id relationship id for which to determine the layout order
 *  \param x x-position
 *  \param y y-position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram,
 *          PENCIL_ERROR_UNKNOWN_OBJECT if the object is not in the diagram or has unsupported type
 */
pencil_error_t pencil_layouter_get_relationship_order_at_pos ( const pencil_layouter_t *this_,
                                                               int64_t relationship_id,
                                                               double x,
                                                               double y,
                                                               layout_order_t* out_layout_order
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
static inline void pencil_layouter_is_pos_on_grid ( const pencil_layouter_t *this_,
                                                    double x,
                                                    double y,
                                                    double snap_distance,
                                                    bool *out_x_on_grid,
                                                    bool *out_y_on_grid
                                                  );

/*!
 *  \brief determines the grid lines
 *
 *  \param this_ pointer to own object attributes
 *  \param out_x0 x-position of leftmost grid line
 *  \param out_y0 y-position of topmost grid line
 *  \param out_dx width between grid lines
 *  \param out_dy height between grid lines
 *  \param out_x_count number of x-position grid lines
 *  \param out_y_count number of y-position grid lines
 */
static inline void pencil_layouter_get_grid_lines ( const pencil_layouter_t *this_,
                                                    double *out_x0,
                                                    double *out_y0,
                                                    double *out_dx,
                                                    double *out_dy,
                                                    uint32_t *out_x_count,
                                                    uint32_t *out_y_count
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
Copyright 2017-2019 Andreas Warnke

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
