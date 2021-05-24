/* File: pos_nav_tree_node.h; Copyright and License: see below */

#ifndef POS_NAV_TREE_NODE_H
#define POS_NAV_TREE_NODE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines location of a diagram in a nav_tree
 */

#include "util/shape/shape_int_rectangle.h"
#include "pos/pos_nav_tree_node_type.h"
#include "data_diagram.h"

/*!
 *  \brief location of a diagram
 */
struct pos_nav_tree_node_struct {
    shape_int_rectangle_t icon_box;  /*!< The rectangle where the icon of the diagram is drawn */
    shape_int_rectangle_t label_box;  /*!< bounding box of the label of the diagram. */
    pos_nav_tree_node_type_t node_type;  /*!< type of the nav tree node. */
    const data_diagram_t *data;  /*!< pointer to the data object of the diagram, NULL if no diagram attached */
};

typedef struct pos_nav_tree_node_struct pos_nav_tree_node_t;

/*!
 *  \brief initializes the pos_nav_tree_node_t struct with a pointer to diagram_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param node_type type of the nav tree node
 *  \param diagram_data pointer to data object which is layouted, NULL if no diagram attached
 */
static inline void pos_nav_tree_node_init ( pos_nav_tree_node_t *this_,
                                            pos_nav_tree_node_type_t node_type,
                                            const data_diagram_t *diagram_data );

/*!
 *  \brief destroys the pos_nav_tree_node_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pos_nav_tree_node_destroy ( pos_nav_tree_node_t *this_ );

/*!
 *  \brief checks if the diagram data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if (*this_).data points to valid data
 */
static inline bool pos_nav_tree_node_is_valid ( const pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the icon_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline shape_int_rectangle_t *pos_nav_tree_node_get_icon_box_ptr ( pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the icon_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_nav_tree_node_get_icon_box_const ( const pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the icon box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param icon_box coordinates of new icon_box
 */
static inline void pos_nav_tree_node_set_icon_box ( pos_nav_tree_node_t *this_, const shape_int_rectangle_t *icon_box );

/*!
 *  \brief gets the label_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline shape_int_rectangle_t *pos_nav_tree_node_get_label_box_ptr ( pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the label_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_nav_tree_node_get_label_box_const ( const pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the label_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_bounds coordinates of new label_box
 */
static inline void pos_nav_tree_node_set_label_box ( pos_nav_tree_node_t *this_, const shape_int_rectangle_t *label_box );

/*!
 *  \brief gets the node type
 *
 *  \param this_ pointer to own object attributes
 *  \return type of node
 */
static inline pos_nav_tree_node_type_t pos_nav_tree_node_get_type ( const pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the data_diagram_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the corresponding data_diagram_t object, NULL if no diagram attached
 */
static inline const data_diagram_t *pos_nav_tree_node_get_data_const ( const pos_nav_tree_node_t *this_ );

/*!
 *  \brief gets the row id of the diagram
 *
 *  This is a shortcut for data_diagram_get_row_id( pos_nav_tree_node_get_data_const ( X ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the diagram
 */
static inline data_row_id_t pos_nav_tree_node_get_diagram_id ( const pos_nav_tree_node_t *this_ );

#include "pos_nav_tree_node.inl"

#endif  /* POS_NAV_TREE_NODE_H */


/*
Copyright 2021-2021 Andreas Warnke

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
