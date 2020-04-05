/* File: pencil_classifier_layouter.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_LAYOUTER_H
#define PENCIL_CLASSIFIER_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of classifiers and features in a diagram
 */

#include "pencil_classifier_composer.h"
#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "pencil_feature_layouter.h"
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
 *  \brief attributes of the classifier layouter
 */
struct pencil_classifier_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to external layout data */

    const pencil_size_t *pencil_size;  /*!< pointer to an external pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */
    const geometry_rectangle_t *diagram_draw_area;  /*!< pointer to an external drawing rectangle containing inner contents of the diagram */
    geometry_non_linear_scale_t *x_scale;  /*!< pointer to an external scale object for the x-axis */
    geometry_non_linear_scale_t *y_scale;  /*!< pointer to an external scale object for the y-axis */
    geometry_dimensions_t *default_classifier_size;  /*!< pointer to an external classifier default size rectangle */

    pencil_classifier_composer_t classifier_composer;  /*!< own instance of a composer object to ask for display dimensions */

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
                                      pencil_layout_data_t *layout_data,
                                      const pencil_size_t *pencil_size,
                                      geometry_dimensions_t *default_classifier_size,
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
void pencil_classifier_layouter_private_propose_move_processing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief propose multiple solutions to move one classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to move classifiers; must not be NULL.
 *  \param sort_index index of the current classifier for which to propose solutions
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solution_move_dx array of solutions: proposal to move in x direction
 *  \param out_solution_move_dy array of solutions: proposal to move in y direction
 *  \param out_solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 */
void pencil_classifier_layouter_private_propose_move_solutions ( pencil_classifier_layouter_t *this_,
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
 *  \param solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 *  \param solution_move_dx array of solutions: proposal to move in x direction
 *  \param solution_move_dy array of solutions: proposal to move in y direction
 *  \param out_index_of_best index (of solution) of the best solution; must not be NULL.
 */
void pencil_classifier_layouter_private_select_move_solution ( pencil_classifier_layouter_t *this_,
                                                               const universal_array_index_sorter_t *sorted,
                                                               uint32_t sort_index,
                                                               uint32_t solutions_count,
                                                               const double solution_move_dx[],
                                                               const double solution_move_dy[],
                                                               uint32_t *out_index_of_best
                                                             );

/*!
 *  \brief move and grow classifiers so that they have nice distances to neighbours and children/parents
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics if re-layouting titles
 */
void pencil_classifier_layouter_local_move_and_grow_for_gaps( pencil_classifier_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief determine order by which to grow classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted sorting order by which to grow classifiers; must not be NULL, shall be initialized to empty.
 */
void pencil_classifier_layouter_private_propose_grow_processing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief determines the gaps between a reference classifier and all other classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param ref_classifier_idx the index of the reference classifier in (*this_).layout_data
 *  \param out_max_outer_bounds the maximum rectangle that does not intersect other classifiers.
 *                              In case there are several solutions, a solution is preferred that grows in all directions.
 *  \param out_min_inner_space the minimum rectangle that encompasses all child classifiers.
 *                             Only embraced child classifiers are evaluated, no features.
 */
void pencil_classifier_layouter_private_get_gaps_to_classifiers( const pencil_classifier_layouter_t *this_,
                                                                 uint32_t ref_classifier_idx,
                                                                 geometry_rectangle_t* out_max_outer_bounds,
                                                                 geometry_rectangle_t* out_min_inner_space
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

/*!
 *  \brief sorts the classifiers by list order
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted_classifiers an array sorter object
 */
static inline void pencil_classifier_layouter_private_sort_classifiers_by_list_order( const pencil_classifier_layouter_t *this_,
                                                                                      universal_array_index_sorter_t *out_sorted_classifiers
                                                                                    );

#include "pencil_classifier_layouter.inl"

#endif  /* PENCIL_CLASSIFIER_LAYOUTER_H */


/*
Copyright 2017-2020 Andreas Warnke

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
