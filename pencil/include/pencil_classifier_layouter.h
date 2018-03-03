/* File: pencil_classifier_layouter.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_LAYOUTER_H
#define PENCIL_CLASSIFIER_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of classifiers and features in a diagram
 */

#include "pencil_input_data.h"
#include "pencil_classifier_painter.h"
#include "pencil_size.h"
#include "pencil_input_data_layout.h"
#include "pencil_feature_layouter.h"
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
 *  \brief attributes of the classifier layouter
 */
struct pencil_classifier_layouter_struct {
    pencil_input_data_layout_t *layout_data;  /* pointer to external layout data */

    const pencil_size_t *pencil_size;  /*!< pointer to an external pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */
    const geometry_rectangle_t *diagram_draw_area;  /*!< pointer to an external drawing rectangle containing inner contents of the diagram */
    geometry_non_linear_scale_t *x_scale;  /*!< pointer to an external scale object for the x-axis */
    geometry_non_linear_scale_t *y_scale;  /*!< pointer to an external scale object for the y-axis */
    geometry_rectangle_t *default_classifier_size;  /*!< pointer to an external classifier default size rectangle */

    pencil_classifier_painter_t classifier_painter;  /*!< own instance of a painter object to ask for display dimensions */

    pencil_feature_layouter_t *feature_layouter;  /*!< pointer or an external helper to layout features */
};

typedef struct pencil_classifier_layouter_struct pencil_classifier_layouter_t;

/*!
 *  \brief initializes the classifier layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 *  \param default_classifier_size pointer to the default size of a classifier
 *  \param x_scale pointer to the scale object for the x-axis
 *  \param y_scale pointer to the scale object for the y-axis
 *  \param feature_layouter pointer to a feature layout helper
 */
void pencil_classifier_layouter_init( pencil_classifier_layouter_t *this_,
                                      pencil_input_data_layout_t *layout_data,
                                      const pencil_size_t *pencil_size,
                                      geometry_rectangle_t *default_classifier_size,
                                      geometry_non_linear_scale_t *x_scale,
                                      geometry_non_linear_scale_t *y_scale,
                                      pencil_feature_layouter_t *feature_layouter
                                    );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_layouter_destroy( pencil_classifier_layouter_t *this_ );

/*!
 *  \brief estimates classifier bounds for each classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_layouter_estimate_bounds ( pencil_classifier_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief resize classifiers so that they embrace their children
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_layouter_embrace_children( pencil_classifier_layouter_t *this_ );

/*!
 *  \brief determine order by which to embrace classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted sorting order of relationships by which to adapt classifiers; must not be NULL, shall be initialized to empty.
 */
void pencil_classifier_layouter_private_propose_embracing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief try to resize classifiers so that they embrace their children
 *
 *  \param this_ pointer to own object attributes
 *  \param the_relationship the relationship to process: the parent tries to embrace the child
 */
void pencil_classifier_layouter_private_try_embrace_child( pencil_classifier_layouter_t *this_, layout_relationship_t *the_relationship );

/*!
 *  \brief hides containment relationships if parents embrace their children
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_layouter_hide_relations_of_embraced_children( pencil_classifier_layouter_t *this_ );

/*!
 *  \brief move classifiers to avoid overlaps
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_layouter_move_to_avoid_overlaps ( pencil_classifier_layouter_t *this_ );

/*!
 *  \brief determine order by which to move classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted sorting order by which to move classifiers; must not be NULL, shall be initialized to empty.
 */
void pencil_classifier_layouter_private_propose_processing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief propose multiple solutions to move one classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to move classifiers; must not be NULL.
 *  \param sort_index index of the current classifier for which to propose solutions
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solution_move_dx array of solutions: proposal to move in x direction
 *  \param out_solution_move_dy array of solutions: proposal to move in y direction
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_classifier_layouter_private_propose_solutions ( pencil_classifier_layouter_t *this_,
                                                                    const universal_array_index_sorter_t *sorted,
                                                                    uint32_t sort_index,
                                                                    uint32_t solutions_max,
                                                                    double out_solution_move_dx[],
                                                                    double out_solution_move_dy[],
                                                                    uint32_t *out_solutions_count
                                                                  );

/*!
 *  \brief selects one solution to move a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to move classifiers; must not be NULL.
 *  \param sort_index index (in sorted classifiers) of the current classifier for which to select a solution
 *  \param solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 *  \param solution_move_dx array of solutions: proposal to move in x direction
 *  \param solution_move_dy array of solutions: proposal to move in y direction
 *  \param out_index_of_best index (of solution) of the best solution; must not be NULL.
 */
void pencil_classifier_layouter_private_select_solution ( pencil_classifier_layouter_t *this_,
                                                                  const universal_array_index_sorter_t *sorted,
                                                                  uint32_t sort_index,
                                                                  uint32_t solutions_count,
                                                                  const double solution_move_dx[],
                                                                  const double solution_move_dy[],
                                                                  uint32_t *out_index_of_best
                                                                );

/*!
 *  \brief defines classifier bounds for list diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_layouter_layout_for_list( pencil_classifier_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief defines classifier bounds for sequence diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_layouter_layout_for_sequence( pencil_classifier_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief defines classifier bounds for timing diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_layouter_layout_for_timing( pencil_classifier_layouter_t *this_, PangoLayout *font_layout );


#endif  /* PENCIL_CLASSIFIER_LAYOUTER_H */


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
