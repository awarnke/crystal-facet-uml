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
static const int GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT = 12;
static const int GUI_SKETCH_NAV_TREE_CHILD_INDENT = 12;

static inline void gui_sketch_nav_tree_get_button_at_pos ( gui_sketch_nav_tree_t *this_,
                                                           int32_t x,
                                                           int32_t y,
                                                           gui_sketch_action_t *out_action_id )
{
    assert ( NULL != out_action_id );

    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        /* determine index of line, top linie has index 0 */
        int32_t top;
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        uint32_t line_index;
        line_index = ( y - top ) / GUI_SKETCH_NAV_TREE_LINE_HEIGHT;

        if ( line_index == (*this_).line_idx_new_root )
        {
            *out_action_id = GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM;
        }
        else if ( line_index == (*this_).line_idx_new_child )
        {
            *out_action_id = GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM;
        }
        else if ( line_index == (*this_).line_idx_new_sibling )
        {
            *out_action_id = GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM;
        }
        else
        {
            *out_action_id = GUI_SKETCH_ACTION_NONE;
        }
    }
    else
    {
        /* position out of bounding box */
        *out_action_id = GUI_SKETCH_ACTION_NONE;
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
    /*uint32_t height;*/

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    /*height = shape_int_rectangle_get_height( &((*this_).bounds) );*/

    uint32_t descendant_count;
    descendant_count = ( (*this_).ancestors_count - ancestor_index - 1 );
    uint32_t y_offset;
    y_offset = ( (*this_).line_idx_ancestors_start + descendant_count ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
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
    /*uint32_t height;*/

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    /*height = shape_int_rectangle_get_height( &((*this_).bounds) );*/

    uint32_t y_offset;
    uint32_t x_offset;
    if (( (*this_).siblings_self_index < 0 )||( (*this_).ancestors_count == 0 ))
    {
        /* error case */
        y_offset = ( (*this_).line_idx_siblings_start + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = 0;
    }
    else if ( sibling_index > (*this_).siblings_self_index )
    {
        y_offset = ( (*this_).line_idx_siblings_next_after_self + ( sibling_index - (*this_).siblings_self_index - 1 ) ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT;
    }
    else
    {
        y_offset = ( (*this_).line_idx_siblings_start + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT;
    }

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
    /*uint32_t height;*/

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    /*height = shape_int_rectangle_get_height( &((*this_).bounds) );*/

    uint32_t y_offset;
    uint32_t x_offset;
    y_offset = ( (*this_).line_idx_children_start + child_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    if ( (*this_).ancestors_count == 0 )
    {
        /* the no-root case */
        x_offset = 0;
    }
    else
    {
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT + GUI_SKETCH_NAV_TREE_CHILD_INDENT;
    }

    shape_int_rectangle_init( &result, left + x_offset, top + y_offset, width - x_offset, GUI_SKETCH_NAV_TREE_LINE_HEIGHT );

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
