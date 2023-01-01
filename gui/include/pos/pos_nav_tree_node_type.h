/* File: pos_nav_tree_node_type.h; Copyright and License: see below */

#ifndef POS_NAV_TREE_NODE_TYPE_H
#define POS_NAV_TREE_NODE_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines types of a nav tree node
 */

/*!
 *  \brief enumeration of all nav tree node types
 */
enum pos_nav_tree_node_type_enum {
    POS_NAV_TREE_NODE_TYPE_VOID = 0,  /*!< unininitialized data field */
    POS_NAV_TREE_NODE_TYPE_ANCESTOR = 1,  /*!< breadcrumb symbol */
    POS_NAV_TREE_NODE_TYPE_OPEN = 2,
    POS_NAV_TREE_NODE_TYPE_CLOSED = 3,
    POS_NAV_TREE_NODE_TYPE_NEW_ROOT = 4,  /*!< button to create a new root diagram */
    POS_NAV_TREE_NODE_TYPE_NEW_SIBLING = 5,  /*!< button to create a new sibling diagram */
    POS_NAV_TREE_NODE_TYPE_NEW_CHILD = 6,  /*!< button to create a new child diagram */
};

typedef enum pos_nav_tree_node_type_enum pos_nav_tree_node_type_t;

/*!
 *  \brief gets the icon for the pos_nav_tree_node_type_t
 *
 *  \param this_ node type
 *  \param highlight true if the mouse/pointer device id over the icon
 *  \param resources the resource object
 *  \return icon representing the pos_nav_tree_node_type_t,
 *          gui_resources_get_type_undef in case of POS_NAV_TREE_NODE_TYPE_VOID
 */
static inline const GdkPixbuf * pos_nav_tree_node_type_get_icon ( pos_nav_tree_node_type_t this_,
                                                                  bool highlight,
                                                                  gui_resources_t *resources
                                                                );

#include "pos_nav_tree_node_type.inl"

#endif  /* POS_NAV_TREE_NODE_TYPE_H */


/*
Copyright 2021-2023 Andreas Warnke

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
