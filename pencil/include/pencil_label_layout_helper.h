/* File: pencil_label_layout_helper.h; Copyright and License: see below */

#ifndef PENCIL_LABEL_LAYOUT_HELPER_H
#define PENCIL_LABEL_LAYOUT_HELPER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Evaluates proposed label positions in a diagram
 */

#include "pencil_size.h"
#include "layout/layout_visible_set.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_point.h"
/* #include "geometry/geometry_anchor.h" */

/*!
 *  \brief attributes of the label layout helper
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_label_layout_helper_struct {
    const pencil_size_t *pencil_size;  /*!< the pencil size proposes preferred distances */
};

typedef struct pencil_label_layout_helper_struct pencil_label_layout_helper_t;

/*!
 *  \brief initializes the label layout helper
 *
 *  \param this_ pointer to own object attributes
 *  \param pencil_size proposal for preferred distances
 */
void pencil_label_layout_helper_init( pencil_label_layout_helper_t *this_, const pencil_size_t *pencil_size );

/*!
 *  \brief destroys the label layout helper
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_label_layout_helper_destroy( pencil_label_layout_helper_t *this_ );

/*!
 *  \brief selects one solution to layout a label rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data data that is already (partly) layouted - by which to select a solution
 *  \param target_point point where the object is located to which the label belongs
 *  \param solutions_count number of proposed solutions; at least 1 shall be provided
 *  \param solutions array of solutions
 *  \param out_index_of_best index of the best solution; must not be NULL.
 */
void pencil_label_layout_helper_select_solution ( pencil_label_layout_helper_t *this_,
                                                  layout_visible_set_t *layout_data,
                                                  geometry_point_t target_point,
                                                  uint32_t solutions_count,
                                                  const geometry_rectangle_t solutions[],
                                                  uint32_t *out_index_of_best
                                                );

#endif  /* PENCIL_LABEL_LAYOUT_HELPER_H */


/*
Copyright 2019-2025 Andreas Warnke

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
