/* File: pencil_rel_label_layouter.h; Copyright and License: see below */

#ifndef PENCIL_REL_LABEL_LAYOUTER_H
#define PENCIL_REL_LABEL_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates the label positions of relationships in a diagram
 */

#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "pencil_label_layout_helper.h"
#include "draw/draw_relationship_label.h"
#include "geometry/geometry_rectangle.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "set/data_visible_set.h"
#include "data_id.h"
#include "u8list/universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the relationship-label layouter
 */
struct pencil_rel_label_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to an instance of layout data */
    pencil_size_t *pencil_size;  /*!< pointer to an instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */

    draw_relationship_label_t draw_relationship_label;  /*!< collection of draw label functions */
    pencil_label_layout_helper_t label_layout_helper;   /*!< collection of layout label functions */
};

typedef struct pencil_rel_label_layouter_struct pencil_rel_label_layouter_t;

/*!
 *  \brief initializes the relationship-label layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_rel_label_layouter_init( pencil_rel_label_layouter_t *this_,
                                     pencil_layout_data_t *layout_data,
                                     pencil_size_t *pencil_size
                                   );

/*!
 *  \brief destroys the relationship-label layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_rel_label_layouter_destroy( pencil_rel_label_layouter_t *this_ );

/*!
 *  \brief determines the rectangels of the relationship-labels
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout structure to layout fonts
 */
void pencil_rel_label_layouter_do_layout ( pencil_rel_label_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief determine order by which to layout relationship-labels
 *
 *  Relationships that are not visible are ignored. Therefore out_sorted may contain fewer relationships than (*this_).layout_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param out_sorted sorting order by which to layout relationship-labels; must not be NULL, shall be initialized to empty.
 */
void pencil_rel_label_layouter_private_propose_processing_order ( pencil_rel_label_layouter_t *this_, universal_array_index_sorter_t *out_sorted );

/*!
 *  \brief propose multiple solutions to layout one relationship-label
 *
 *  \param this_ pointer to own object attributes
 *  \param current_relation relationship for which to propose solutions
 *  \param font_layout structure to layout fonts
 *  \param solutions_max maximum number (array size) of solutions to propose
 *  \param out_solutions array of solutions
 *  \param out_solutions_count number of proposed solutions; 1 <= out_solutions_count < solutions_max
 */
void pencil_rel_label_layouter_private_propose_solutions ( pencil_rel_label_layouter_t *this_,
                                                           layout_relationship_t *current_relation,
                                                           PangoLayout *font_layout,
                                                           uint32_t solutions_max,
                                                           geometry_rectangle_t out_solutions[],
                                                           uint32_t *out_solutions_count
                                                         );

#endif  /* PENCIL_REL_LABEL_LAYOUTER_H */


/*
Copyright 2019-2022 Andreas Warnke

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
