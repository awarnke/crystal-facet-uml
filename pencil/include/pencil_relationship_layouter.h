/* File: pencil_relationship_layouter.h; Copyright and License: see below */

#ifndef PENCIL_RELATIONSHIP_LAYOUTER_H
#define PENCIL_RELATIONSHIP_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions relationships in a diagram
 */

#include "pencil_input_data.h"
#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "pencil_relationship_painter.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "util/id/data_small_set.h"
#include "data_id.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include "universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the relationship layouter
 */
struct pencil_relationship_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to an instance of layout data */
    pencil_size_t *pencil_size;  /*!< pointer to an instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */

    pencil_relationship_painter_t relationship_painter;  /*!< own instance of a painter object to ask for display dimensions */
};

typedef struct pencil_relationship_layouter_struct pencil_relationship_layouter_t;

/*!
 *  \brief initializes the layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_relationship_layouter_init( pencil_relationship_layouter_t *this_,
                                        pencil_layout_data_t *layout_data,
                                        pencil_size_t *pencil_size
                                      );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_layouter_destroy( pencil_relationship_layouter_t *this_ );

/*!
 *  \brief determines the shapes of the relationships
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_layouter_do_layout ( pencil_relationship_layouter_t *this_ );

/*!
 *  \brief determine order by which to shape relationships
 *
 *  Relationships that are not visible are ignored. Therefore out_sorted may contain fewer relationships than (*this_).layout_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted sorting order by which to shape relationships; must not be NULL, shall be initialized to empty.
 */
void pencil_relationship_layouter_private_propose_processing_order ( pencil_relationship_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief propose multiple solutions to shape one relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to shape relationships; must not be NULL.
 *  \param sort_index index of the current relationship for which to propose solutions
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_relationship_layouter_private_propose_solutions ( pencil_relationship_layouter_t *this_,
                                                              const universal_array_index_sorter_t *sorted,
                                                              uint32_t sort_index,
                                                              uint32_t solutions_max,
                                                              geometry_connector_t out_solutions[],
                                                              uint32_t *out_solutions_count
                                                            );

/*!
 *  \brief selects one solution to shape a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param sorted sorting order by which to shape relationships; must not be NULL.
 *  \param sort_index index (in sorted relationships) of the current relationship for which to select a solution
 *  \param solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 *  \param solutions array of solutions
 *  \param out_index_of_best index (of solution) of the best solution; must not be NULL.
 */
void pencil_relationship_layouter_private_select_solution ( pencil_relationship_layouter_t *this_,
                                                            const universal_array_index_sorter_t *sorted,
                                                            uint32_t sort_index,
                                                            uint32_t solutions_count,
                                                            const geometry_connector_t solutions[],
                                                            uint32_t *out_index_of_best
                                                          );

/*!
 *  \brief layouts a connection from one rectangle to another in shape of Z or N
 *
 *  \param this_ pointer to own object attributes
 *  \param source_rect pointer to the source rectangle
 *  \param dest_rect pointer to the destination rectangle
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_relationship_layouter_private_connect_rectangles_by_ZN ( pencil_relationship_layouter_t *this_,
                                                                     const geometry_rectangle_t *source_rect,
                                                                     const geometry_rectangle_t *dest_rect,
                                                                     uint32_t solutions_max,
                                                                     geometry_connector_t out_solutions[],
                                                                     uint32_t *out_solutions_count
                                                                   );

/*!
 *  \brief layouts a connection from one rectangle to another in shape of U or C
 *
 *  \param this_ pointer to own object attributes
 *  \param source_rect pointer to the source rectangle
 *  \param dest_rect pointer to the destination rectangle
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_relationship_layouter_private_connect_rectangles_by_UC ( pencil_relationship_layouter_t *this_,
                                                                     const geometry_rectangle_t *source_rect,
                                                                     const geometry_rectangle_t *dest_rect,
                                                                     uint32_t solutions_max,
                                                                     geometry_connector_t out_solutions[],
                                                                     uint32_t *out_solutions_count
                                                                   );

/*!
 *  \brief layouts a connection from one rectangle to another in shape of L or 7
 *
 *  \param this_ pointer to own object attributes
 *  \param source_rect pointer to the source rectangle
 *  \param dest_rect pointer to the destination rectangle
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_relationship_layouter_private_connect_rectangles_by_L7 ( pencil_relationship_layouter_t *this_,
                                                                     const geometry_rectangle_t *source_rect,
                                                                     const geometry_rectangle_t *dest_rect,
                                                                     uint32_t solutions_max,
                                                                     geometry_connector_t out_solutions[],
                                                                     uint32_t *out_solutions_count
                                                                   );

/*!
 *  \brief finds an empty, unused line in a rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param search_rect pointer to the rectangle within which to search
 *  \param horizontal_line true if space for a horizontal line is searched for, false for vertical line
 *  \param min_gap minimum distance to other objects on the diagram
 *  \param out_success pointer to locatoin where to store if the function was successful or not
 *  \param out_coordinate ordinate (y-coordinate) or abscissa (x-coordinate) value where space is available.
 */
void pencil_relationship_layouter_private_find_space_for_line ( pencil_relationship_layouter_t *this_,
                                                                const geometry_rectangle_t *search_rect,
                                                                bool horizontal_line,
                                                                double min_gap,
                                                                bool *out_success,
                                                                double *out_coordinate
                                                              );

#endif  /* PENCIL_RELATIONSHIP_LAYOUTER_H */


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
