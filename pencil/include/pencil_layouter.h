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
#include "layout/layout_visible_set.h"
#include "pencil_diagram_painter.h"
#include "pencil_feature_painter.h"
#include "pencil_feature_layouter.h"
#include "pencil_feat_label_layouter.h"
#include "pencil_relationship_2d_layouter.h"
#include "pencil_relationship_1d_layouter.h"
#include "pencil_rel_label_layouter.h"
#include "pencil_classifier_2d_layouter.h"
#include "pencil_classifier_1d_layouter.h"
#include "pencil_error.h"
#include "layout/layout_order.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "geometry/geometry_grid.h"
#include "geometry/geometry_non_linear_scale.h"
#include "set/data_full_id.h"
#include "set/data_profile_part.h"
#include "set/data_small_set.h"
#include "set/data_stat.h"
#include "set/data_visible_set.h"
#include "data_diagram.h"
#include "data_id.h"
#include "data_rules.h"
#include "u8list/universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the layouter
 */
struct pencil_layouter_struct {
    layout_visible_set_t layout_data;  /* own instance of layout data */
    const data_profile_part_t *profile;  /*!< pointer to an external stereotype-image cache */

    pencil_size_t pencil_size;  /*!< own instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */
    geometry_non_linear_scale_t x_scale;  /*!< own instance of a scale object for the x-axis */
    geometry_non_linear_scale_t y_scale;  /*!< own instance of a scale object for the y-axis */
    geometry_dimensions_t default_classifier_size;  /*!< own instance of a classifier default size */
    data_rules_t rules;  /*!< own instance of modelling rules */

    pencil_diagram_painter_t diagram_painter;  /*!< own instance of a painter object to ask for display dimensions */

    pencil_classifier_2d_layouter_t pencil_classifier_2d_layouter;  /*!< own instance of a helper object to layout classifiers */
    pencil_classifier_1d_layouter_t pencil_classifier_1d_layouter;  /*!< own instance of a helper object to layout classifiers */
    pencil_feature_layouter_t feature_layouter;  /*!< own instance of a helper object to layout features */
    pencil_feat_label_layouter_t feature_label_layouter;  /*!< own instance of a helper object to layout feature labels */
    pencil_relationship_2d_layouter_t pencil_relationship_2d_layouter;  /*!< own instance of a helper object to layout relationships */
    pencil_relationship_1d_layouter_t pencil_relationship_1d_layouter;  /*!< own instance of a helper object to layout relationships */
    pencil_rel_label_layouter_t relationship_label_layouter;  /*!< own instance of a helper object to layout relationship labels */
};

typedef struct pencil_layouter_struct pencil_layouter_t;

/*!
 *  \brief initializes the layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be layouted
 */
void pencil_layouter_init( pencil_layouter_t *this_,
                           const data_visible_set_t *input_data,
                           const data_profile_part_t *profile
                         );

/*!
 *  \brief re-initializes the layouter to layout new/other input_data
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the data to be layouted
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be layouted
 */
void pencil_layouter_reinit( pencil_layouter_t *this_,
                             const data_visible_set_t *input_data,
                             const data_profile_part_t *profile
                           );

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
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_layouter_define_grid ( pencil_layouter_t *this_,
                                   geometry_rectangle_t diagram_bounds,
                                   PangoLayout *font_layout
                                 );

/*!
 *  \brief layouts the chosen diagram contents into the diagram_bounds area
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_layouter_layout_elements ( pencil_layouter_t *this_, PangoLayout *font_layout );

/* !
 *  \brief returns the layout_visible_set_t object
 *
 *  \param this_ pointer to own object attributes
 */
static inline layout_visible_set_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ );

/*!
 *  \brief returns the layout_visible_set_t object
 *
 *  \param this_ pointer to own object attributes
 */
static inline const layout_visible_set_t *pencil_layouter_get_layout_data_const ( const pencil_layouter_t *this_ );

/*!
 *  \brief returns the pencil size object
 *
 *  \param this_ pointer to own object attributes
 */
static inline const pencil_size_t *pencil_layouter_get_pencil_size_const ( pencil_layouter_t *this_ );

/*!
 *  \brief gets the layout order for a classifier at a given position
 *
 *  \param this_ pointer to own object attributes
 *
 *  \param c_type classifier type for which to determine the order
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next grid line when to snap to a grid position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram
 */
pencil_error_t pencil_layouter_get_classifier_order_at_pos ( const pencil_layouter_t *this_,
                                                             data_classifier_type_t c_type,
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
 *  \param x x-position
 *  \param y y-position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram,
 *          PENCIL_ERROR_UNKNOWN_OBJECT if the object is not in the diagram or has unsupported type
 */
pencil_error_t pencil_layouter_get_relationship_order_at_pos ( const pencil_layouter_t *this_,
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
Copyright 2017-2024 Andreas Warnke

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
