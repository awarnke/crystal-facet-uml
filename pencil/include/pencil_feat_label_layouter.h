/* File: pencil_feat_label_layouter.h; Copyright and License: see below */

#ifndef PENCIL_FEAT_LABEL_LAYOUTER_H
#define PENCIL_FEAT_LABEL_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates the label positions of features in a diagram
 */

#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "draw/draw_label.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "set/data_visible_set.h"
#include "data_id.h"
#include "universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the feature-label layouter
 */
struct pencil_feat_label_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to an instance of layout data */
    pencil_size_t *pencil_size;  /*!< pointer to an instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */

    draw_label_t draw_label;  /*!< collection of draw label functions */
};

typedef struct pencil_feat_label_layouter_struct pencil_feat_label_layouter_t;

/*!
 *  \brief initializes the feature-label layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_feat_label_layouter_init( pencil_feat_label_layouter_t *this_,
                                      pencil_layout_data_t *layout_data,
                                      pencil_size_t *pencil_size
                                    );

/*!
 *  \brief destroys the feature-label layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feat_label_layouter_destroy( pencil_feat_label_layouter_t *this_ );

/*!
 *  \brief determines the rectangels of the feature-label
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feat_label_layouter_private_do_layout ( pencil_feat_label_layouter_t *this_ );

/*!
 *  \brief determine order by which to layout the feature-labels
 *
 *  Features that are not visible are ignored. Therefore out_sorted may contain fewer features than (*this_).layout_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted sorting order by which to layout feature-labels; must not be NULL, shall be initialized to empty.
 */
void pencil_feat_label_layouter_private_propose_processing_order ( pencil_feat_label_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief propose multiple solutions to layout one feature-label
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to layout feature-labels; must not be NULL.
 *  \param sort_index index of the current feature for which to propose solutions
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_feat_label_layouter_private_propose_solutions ( pencil_feat_label_layouter_t *this_,
                                                            const universal_array_index_sorter_t *sorted,
                                                            uint32_t sort_index,
                                                            uint32_t solutions_max,
                                                            geometry_rectangle_t out_solutions[],
                                                            uint32_t *out_solutions_count
                                                          );

/*!
 *  \brief selects one solution to layout a feature-label
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to layout feature-labels; must not be NULL.
 *  \param sort_index index (in sorted features) of the current feature for which to select a solution
 *  \param solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 *  \param solutions array of solutions
 *  \param out_index_of_best index (of solution) of the best solution; must not be NULL.
 */
void pencil_feat_label_layouter_private_select_solution ( pencil_feat_label_layouter_t *this_,
                                                          const universal_array_index_sorter_t *sorted,
                                                          uint32_t sort_index,
                                                          uint32_t solutions_count,
                                                          const geometry_rectangle_t solutions[],
                                                          uint32_t *out_index_of_best
                                                        );

#endif  /* PENCIL_FEAT_LABEL_LAYOUTER_H */


/*
Copyright 2019-2019 Andreas Warnke

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
