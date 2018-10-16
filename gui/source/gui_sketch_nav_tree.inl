/* File: gui_sketch_nav_tree.inl; Copyright and License: see below */

#include "tslog.h"

static inline shape_int_rectangle_t gui_sketch_nav_tree_get_bounds( gui_sketch_nav_tree_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_nav_tree_set_bounds( gui_sketch_nav_tree_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_nav_tree_is_visible( gui_sketch_nav_tree_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_nav_tree_set_visible( gui_sketch_nav_tree_t *this_, bool visible )
{
    (*this_).visible = visible;
}

static inline data_diagram_t *gui_sketch_nav_tree_get_diagram_ptr ( gui_sketch_nav_tree_t *this_ )
{
    return &((*this_).ancestor_diagrams[0]);
}

static const int GUI_SKETCH_NAV_TREE_LINE_HEIGHT = 16;
static const int GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT = 4;
static const int GUI_SKETCH_NAV_TREE_CHILD_INDENT = 12;

static inline void gui_sketch_nav_tree_get_object_id_at_pos ( gui_sketch_nav_tree_t *this_,
                                                              int32_t x,
                                                              int32_t y,
                                                              data_id_t* out_selected_id )
{
    assert ( NULL != out_selected_id );

    int32_t top;
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    
    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        
        /* is this the ancester region ? */
        if ( (*this_).ancestors_count > 1 )
        {
            if ( y - top < ( GUI_SKETCH_NAV_TREE_LINE_HEIGHT * ( (*this_).ancestors_count-1 ) ) )
            {
                uint32_t ancester_idx = (*this_).ancestors_count-1-((y-top)/GUI_SKETCH_NAV_TREE_LINE_HEIGHT);
                data_id_reinit( out_selected_id, 
                                DATA_TABLE_DIAGRAM, 
                                data_diagram_get_id( &((*this_).ancestor_diagrams[ancester_idx]) ) 
                              );
            }
        }
        else
        {
            data_id_reinit_void( out_selected_id );
        }
    }
    else
    {
        data_id_reinit_void( out_selected_id );
    }
}

static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_ancestor_bounds ( gui_sketch_nav_tree_t *this_,
                                                                                      uint32_t ancestor_index
                                                                                    )
{
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert ( ancestor_index < (*this_).ancestors_count );

    shape_int_rectangle_t result;

    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    uint32_t descendant_count;
    descendant_count = ( (*this_).ancestors_count - ancestor_index - 1 );
    uint32_t y_offset;
    y_offset = descendant_count * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    uint32_t x_offset;
    x_offset = descendant_count * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT;

    shape_int_rectangle_init( &result, left + x_offset, top + y_offset, width - x_offset, GUI_SKETCH_NAV_TREE_LINE_HEIGHT );

    return result;
}

static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_sibling_bounds ( gui_sketch_nav_tree_t *this_,
                                                                                     uint32_t sibling_index
                                                                                   )
{
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );

    shape_int_rectangle_t result;

    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    uint32_t y_offset;
    if (( (*this_).siblings_self_index < 0 )||( (*this_).ancestors_count == 0 ))
    {
        /* error case */
        y_offset = sibling_index * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    }
    else if ( sibling_index > (*this_).siblings_self_index )
    {
        y_offset = ( (*this_).ancestors_count - 1 + (*this_).children_count + 1 + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    }
    else
    {
        y_offset = ( (*this_).ancestors_count - 1 + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    }
    uint32_t x_offset;
    x_offset = (*this_).ancestors_count * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT;

    shape_int_rectangle_init( &result, left + x_offset, top + y_offset, width - x_offset, GUI_SKETCH_NAV_TREE_LINE_HEIGHT );

    return result;
}

static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_child_bounds ( gui_sketch_nav_tree_t *this_,
                                                                                   uint32_t child_index
                                                                                 )
{
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    shape_int_rectangle_t result;

    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    uint32_t y_offset;
    if (( (*this_).siblings_self_index < 0 )||( (*this_).ancestors_count == 0 ))
    {
        /* error case */
        /* if self is not a sibling, simply add children to the end */
        y_offset = ( (*this_).ancestors_count + (*this_).siblings_count + child_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    }
    else
    {
        y_offset = ( (*this_).ancestors_count - 1 + (*this_).siblings_self_index + 1 + child_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    }
    uint32_t x_offset;
    x_offset = (*this_).ancestors_count * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT + GUI_SKETCH_NAV_TREE_CHILD_INDENT;

    shape_int_rectangle_init( &result, left + x_offset, top + y_offset, width - x_offset, GUI_SKETCH_NAV_TREE_LINE_HEIGHT );

    return result;
}

static inline layout_order_t gui_sketch_nav_tree_get_order_at_pos ( gui_sketch_nav_tree_t *this_, data_id_t obj_id, int32_t x, int32_t y )
{
    layout_order_t result;
    layout_order_init_empty( &result );
    TSLOG_ERROR("Not yet implemented: gui_sketch_nav_tree_get_order_at_pos");
    return result;
}

static inline void gui_sketch_nav_tree_move_object_to_order ( gui_sketch_nav_tree_t *this_, data_id_t obj_id, layout_order_t *order )
{
    layout_order_t result;
    layout_order_init_empty( &result );
    TSLOG_ERROR("Not yet implemented: gui_sketch_nav_tree_move_object_to_order");
    return result;
}


/*
Copyright 2018-2018 Andreas Warnke

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
