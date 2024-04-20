/* File: pencil_relationship_2d_layouter.h; Copyright and License: see below */

#ifndef PENCIL_RELATIONSHIP_2D_LAYOUTER_H
#define PENCIL_RELATIONSHIP_2D_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of relationships in a diagram
 */

#include "pencil_size.h"
#include "layout/layout_visible_set.h"
#include "pencil_relationship_painter.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_non_linear_scale.h"
#include "set/data_profile_part.h"
#include "set/data_small_set.h"
#include "set/data_visible_set.h"
#include "data_diagram.h"
#include "data_id.h"
#include "u8list/universal_array_index_sorter.h"
#include "u8/u8_error.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the relationship layouter
 *
 *  \note This class is stateless. Only the layout_data, pencil_size and profile objects are stateful.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_relationship_2d_layouter_struct {
    layout_visible_set_t *layout_data;  /*!< pointer to an instance of layout data */
    const data_profile_part_t *profile;  /*!< pointer to an external stereotype-image cache */
    universal_array_index_sorter_t sorted_relationships;  /*!< a sorted list of relationships, ordered by processing order, */
                                                          /*!< empty if layouting algorithm finished */
    uint32_t sorted_rel_index;  /*!< currently processed relationship of the sorted list of relationships */

    const pencil_size_t *pencil_size;  /*!< pointer to an instance of a pencil_size_t object, defining pen sizes, gap sizes, */
                                 /*!< font sizes and colors */
    pencil_relationship_painter_t relationship_painter;  /*!< own instance of a painter object to ask for display dimensions */
};

typedef struct pencil_relationship_2d_layouter_struct pencil_relationship_2d_layouter_t;

/*!
 *  \brief initializes the layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be layouted
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_relationship_2d_layouter_init( pencil_relationship_2d_layouter_t *this_,
                                           layout_visible_set_t *layout_data,
                                           const data_profile_part_t *profile,
                                           const pencil_size_t *pencil_size
                                         );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_2d_layouter_destroy( pencil_relationship_2d_layouter_t *this_ );

/*!
 *  \brief determines the shapes of the relationships
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_2d_layouter_private_do_layout ( pencil_relationship_2d_layouter_t *this_ );

/*!
 *  \brief determine order by which to shape relationships
 *
 *  Relationships that are not visible are ignored. Therefore this_.sorted_relationships may contain fewer relationships than (*this_).layout_data.
 *
 *  \param this_ pointer to own object attributes, &((this_).sorted_relationships) is initialized
 */
void pencil_relationship_2d_layouter_private_propose_processing_order ( pencil_relationship_2d_layouter_t *this_ );

/*!
 *  \brief propose multiple solutions to shape one relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 */
void pencil_relationship_2d_layouter_private_propose_solutions ( pencil_relationship_2d_layouter_t *this_,
                                                                 uint32_t solutions_max,
                                                                 geometry_connector_t out_solutions[],
                                                                 uint32_t *out_solutions_count
                                                               );

/*!
 *  \brief selects one solution to shape a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 *  \param solutions array of solutions
 *  \param out_index_of_best index (of solution) of the best solution; must not be NULL.
 */
void pencil_relationship_2d_layouter_private_select_solution ( pencil_relationship_2d_layouter_t *this_,
                                                               uint32_t solutions_count,
                                                               const geometry_connector_t solutions[],
                                                               uint32_t *out_index_of_best
                                                             );

/*!
 *  \brief layouts a connection from one rectangle to another in shape of - or I
 *
 *  \param this_ pointer to own object attributes
 *  \param source_rect pointer to the source rectangle
 *  \param dest_rect pointer to the destination rectangle
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 */
void pencil_relationship_2d_layouter_private_connect_rectangles_by_I ( pencil_relationship_2d_layouter_t *this_,
                                                                       const geometry_rectangle_t *source_rect,
                                                                       const geometry_rectangle_t *dest_rect,
                                                                       uint32_t solutions_max,
                                                                       geometry_connector_t out_solutions[],
                                                                       uint32_t *out_solutions_count
                                                                     );

/*!
 *  \brief layouts a connection from one rectangle to another in shape of Z or N
 *
 *  \param this_ pointer to own object attributes
 *  \param source_rect pointer to the source rectangle
 *  \param dest_rect pointer to the destination rectangle
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 */
void pencil_relationship_2d_layouter_private_connect_rectangles_by_ZN ( pencil_relationship_2d_layouter_t *this_,
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
 *  \param out_solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 */
void pencil_relationship_2d_layouter_private_connect_rectangles_by_UC ( pencil_relationship_2d_layouter_t *this_,
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
 *  \param out_solutions_count number of proposed solutions; 1 &lt;= out_solutions_count &lt; solutions_max
 */
void pencil_relationship_2d_layouter_private_connect_rectangles_by_L7 ( pencil_relationship_2d_layouter_t *this_,
                                                                        const geometry_rectangle_t *source_rect,
                                                                        const geometry_rectangle_t *dest_rect,
                                                                        uint32_t solutions_max,
                                                                        geometry_connector_t out_solutions[],
                                                                        uint32_t *out_solutions_count
                                                                      );

/*!
 *  \brief finds an empty, unused line in a rectangle
 *
 *  In case there are several options, the one closer to the center is chosen.
 *
 *  Touching a rectangle at the border of search_rect is allowed since this may be the source or destination
 *
 *  \param this_ pointer to own object attributes
 *  \param search_rect pointer to the rectangle within which to search
 *  \param horizontal_line true if space for a horizontal line is searched for, false for vertical line
 *  \param min_gap minimum distance to other objects on the diagram
 *  \param io_coordinate ordinate (y-coordinate) or abscissa (x-coordinate) value of the line
 *                       for which to search space. Input value is the default; in case U8_ERROR_NONE is returned
 *                       this value is updated to the position where space is available.
 *  \return U8_ERROR_NONE if a space was found, U8_ERROR_NOT_FOUND otherwise
 */
u8_error_t pencil_relationship_2d_layouter_private_find_space_for_line ( pencil_relationship_2d_layouter_t *this_,
                                                                         const geometry_rectangle_t *search_rect,
                                                                         bool horizontal_line,
                                                                         double min_gap,
                                                                         double *io_coordinate
                                                                       );

/*!
 *  \brief finds an empty, unused line in a rectangle
 *
 *  In case there are several options, the one closer to the initial value of io_ordinate is chosen.
 *
 *  Touching a rectangle at the border of search_rect is allowed since this may be the source or destination
 *
 *  \param this_ pointer to own object attributes
 *  \param search_rect pointer to the rectangle within which to search
 *  \param min_gap minimum distance to other objects on the diagram
 *  \param io_ordinate ordinate (y-coordinate) of the line for which to search space.
 *                     Input value is the default; in case U8_ERROR_NONE is returned
 *                     this value is updated to the position where where space is available.
 *  \return U8_ERROR_NONE if a space was found, U8_ERROR_NOT_FOUND otherwise
 */
static inline u8_error_t pencil_relationship_2d_layouter_private_find_space_for_h_line ( pencil_relationship_2d_layouter_t *this_,
                                                                                         const geometry_rectangle_t *search_rect,
                                                                                         double min_gap,
                                                                                         double *io_ordinate
                                                                                       );

/*!
 *  \brief finds an empty, unused line in a rectangle
 *
 *  In case there are several options, the one closer to the initial value of io_ordinate is chosen.
 *
 *  Touching a rectangle at the border of search_rect is allowed since this may be the source or destination
 *
 *  \param this_ pointer to own object attributes
 *  \param search_rect pointer to the rectangle within which to search
 *  \param min_gap minimum distance to other objects on the diagram
 *  \param io_abscissa abscissa (x-coordinate) of the line for which to search space.
 *                     Input value is the default; in case U8_ERROR_NONE is returned
 *                     this value is updated to the position where where space is available.
 *  \return U8_ERROR_NONE if a space was found, U8_ERROR_NOT_FOUND otherwise
 */
static inline u8_error_t pencil_relationship_2d_layouter_private_find_space_for_v_line ( pencil_relationship_2d_layouter_t *this_,
                                                                                         const geometry_rectangle_t *search_rect,
                                                                                         double min_gap,
                                                                                         double *io_abscissa
                                                                                       );

/*!
 *  \brief make all relationships visible.
 *
 *  Sets either normal visibility or grayed out status.
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_2d_layouter_private_make_all_visible ( pencil_relationship_2d_layouter_t *this_ );

/*!
 *  \brief layouts all relationships.
 *
 *  This function performs a standard layouting of relationships
 *  suitable for most diagram types.
 *  Exceptions are LIST, BOX, SEQUENCE, TIMING, COMMUNICATION
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_2d_layouter_layout_standard( pencil_relationship_2d_layouter_t *this_ );

/*!
 *  \brief hides all relationships.
 *
 *  Relationships are not visible in simple list and box diagrams.
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_2d_layouter_layout_void( pencil_relationship_2d_layouter_t *this_ );

/*!
 *  \brief defines the shapes of relationships in communication diagrams.
 *
 *  Shows only relationships that are associated to a lifeline because a communication diagram
 *  shows only an example sequence of messages, not all dependencies.
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_2d_layouter_layout_for_communication( pencil_relationship_2d_layouter_t *this_ );

#include "pencil_relationship_2d_layouter.inl"

#endif  /* PENCIL_RELATIONSHIP_2D_LAYOUTER_H */


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
