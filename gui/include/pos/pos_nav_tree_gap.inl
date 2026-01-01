/* File: pos_nav_tree_gap.inl; Copyright and License: see below */

#include <assert.h>

static inline void pos_nav_tree_gap_init ( pos_nav_tree_gap_t *this_,
                                           data_id_t parent_id,
                                           int32_t list_order )
{
    shape_int_rectangle_init_empty( &((*this_).gap_box) );
    (*this_).parent_id = parent_id;
    (*this_).list_order = list_order;
}

static inline void pos_nav_tree_gap_destroy ( pos_nav_tree_gap_t *this_ )
{
    shape_int_rectangle_destroy( &((*this_).gap_box) );
}

static inline const shape_int_rectangle_t *pos_nav_tree_gap_get_gap_box_const ( const pos_nav_tree_gap_t *this_ )
{
    return &((*this_).gap_box);
}

static inline void pos_nav_tree_gap_set_gap_box ( pos_nav_tree_gap_t *this_, const shape_int_rectangle_t *gap_box )
{
    shape_int_rectangle_replace( &((*this_).gap_box), gap_box );
}

static inline void pos_nav_tree_gap_set_gap_box_coords ( pos_nav_tree_gap_t *this_,
                                                         int32_t left,
                                                         int32_t top,
                                                         uint32_t width,
                                                         uint32_t height )
{
    shape_int_rectangle_reinit( &((*this_).gap_box), left, top, width, height );
}

static inline data_id_t pos_nav_tree_gap_get_parent_id ( const pos_nav_tree_gap_t *this_ )
{
    return (*this_).parent_id;
}

static inline int32_t pos_nav_tree_gap_get_list_order ( const pos_nav_tree_gap_t *this_ )
{
    return (*this_).list_order;
}


/*
Copyright 2021-2026 Andreas Warnke

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
