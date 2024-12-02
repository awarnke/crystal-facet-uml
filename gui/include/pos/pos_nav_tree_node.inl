/* File: pos_nav_tree_node.inl; Copyright and License: see below */

#include <assert.h>

static inline void pos_nav_tree_node_init ( pos_nav_tree_node_t *this_,
                                            pos_nav_tree_node_type_t node_type,
                                            const data_diagram_t *diagram_data )
{
    shape_int_rectangle_init_empty( &((*this_).icon_box) );
    shape_int_rectangle_init_empty( &((*this_).label_box) );
    (*this_).node_type = node_type;
    (*this_).data = diagram_data;
}

static inline void pos_nav_tree_node_destroy ( pos_nav_tree_node_t *this_ )
{
    shape_int_rectangle_destroy( &((*this_).icon_box) );
    shape_int_rectangle_destroy( &((*this_).label_box) );
    (*this_).data = NULL;
}

static inline bool pos_nav_tree_node_is_valid ( const pos_nav_tree_node_t *this_ )
{
    bool result;
    if ( (*this_).data == NULL )
    {
        assert(false);
        result = false;  /* cannot happen on initialized objects */
    }
    else
    {
        result = data_diagram_is_valid( (*this_).data );
    }
    return result;
}

static inline const shape_int_rectangle_t *pos_nav_tree_node_get_icon_box_const ( const pos_nav_tree_node_t *this_ )
{
    return &((*this_).icon_box);
}

static inline void pos_nav_tree_node_set_icon_box ( pos_nav_tree_node_t *this_, const shape_int_rectangle_t *icon_box )
{
    shape_int_rectangle_replace( &((*this_).icon_box), icon_box );
}

static inline const shape_int_rectangle_t *pos_nav_tree_node_get_label_box_const ( const pos_nav_tree_node_t *this_ )
{
    return &((*this_).label_box);
}

static inline void pos_nav_tree_node_set_label_box ( pos_nav_tree_node_t *this_, const shape_int_rectangle_t *label_box )
{
    shape_int_rectangle_replace( &((*this_).label_box), label_box );
}

static inline pos_nav_tree_node_type_t pos_nav_tree_node_get_type ( const pos_nav_tree_node_t *this_ )
{
    return (*this_).node_type;
}

static inline const data_diagram_t *pos_nav_tree_node_get_data_const ( const pos_nav_tree_node_t *this_ )
{
    return (*this_).data;
}

static inline data_row_t pos_nav_tree_node_get_diagram_id ( const pos_nav_tree_node_t *this_ )
{
    return data_diagram_get_row_id( (*this_).data );
}


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
