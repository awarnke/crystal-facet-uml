/* File: pos_nav_tree_gap.h; Copyright and License: see below */

#ifndef POS_NAV_TREE_GAP_H
#define POS_NAV_TREE_GAP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines parent and list order of gap between diagrams in a nav_tree
 */

#include "shape/shape_int_rectangle.h"
#include "entity/data_id.h"

/*!
 *  \brief parent and list order of gap between diagrams
 */
struct pos_nav_tree_gap_struct {
    shape_int_rectangle_t gap_box;  /*!< The rectangle where the gap is indicated */
    data_id_t parent_id;  /*!< The parent diagram id */
    int32_t list_order;  /*!< The list_order at the gap position */
};

typedef struct pos_nav_tree_gap_struct pos_nav_tree_gap_t;

/*!
 *  \brief initializes the pos_nav_tree_gap_t struct with a pointer to diagram_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id The parent diagram id
 *  \param list_order The list_order at the gap position
 */
static inline void pos_nav_tree_gap_init ( pos_nav_tree_gap_t *this_,
                                           data_id_t parent_id,
                                           int32_t list_order
                                         );

/*!
 *  \brief destroys the pos_nav_tree_gap_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pos_nav_tree_gap_destroy ( pos_nav_tree_gap_t *this_ );

/*!
 *  \brief gets the gap_box of the gap
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_nav_tree_gap_get_gap_box_const ( const pos_nav_tree_gap_t *this_ );

/*!
 *  \brief sets the gap_box of the gap
 *
 *  \param this_ pointer to own object attributes
 *  \param gap_box coordinates of gap_box
 */
static inline void pos_nav_tree_gap_set_gap_box ( pos_nav_tree_gap_t *this_, const shape_int_rectangle_t *gap_box );

/*!
 *  \brief sets the gap_box of the gap
 *
 *  \param this_ pointer to own object attributes
 *  \param left left coordinate of gap_box
 *  \param top top coordinate of gap_box
 *  \param width width of gap_box
 *  \param height height of gap_box
 */
static inline void pos_nav_tree_gap_set_gap_box_coords ( pos_nav_tree_gap_t *this_,
                                                         int32_t left,
                                                         int32_t top,
                                                         uint32_t width,
                                                         uint32_t height
                                                       );

/*!
 *  \brief gets the gaps parent_id
 *
 *  \param this_ pointer to own object attributes
 *  \return parent_id at gap pos
 */
static inline data_id_t pos_nav_tree_gap_get_parent_id ( const pos_nav_tree_gap_t *this_ );

/*!
 *  \brief gets the gaps list_order
 *
 *  \param this_ pointer to own object attributes
 *  \return list_order at gap position
 */
static inline int32_t pos_nav_tree_gap_get_list_order ( const pos_nav_tree_gap_t *this_ );

#include "pos_nav_tree_gap.inl"

#endif  /* POS_NAV_TREE_GAP_H */


/*
Copyright 2021-2024 Andreas Warnke

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
