/* File: gui_sketch_nav_tree.inl; Copyright and License: see below */

#include "tslog.h"

static inline int64_t gui_sketch_nav_tree_get_root_diagram_id ( gui_sketch_nav_tree_t *this_ )
{
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    int64_t result;
    
    if ( (*this_).ancestors_count == 0 )
    {
        result = DATA_ID_VOID_ID;
    }
    else
    {
        result = data_diagram_get_id ( &((*this_).ancestor_diagrams[(*this_).ancestors_count-1]) );
    }
    
    return result;
}

static inline int32_t gui_sketch_nav_tree_get_siblings_highest_order ( gui_sketch_nav_tree_t *this_ )
{
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    int32_t result;
    
    if ( (*this_).siblings_count == 0 )
    {
        result = 0;
    }
    else
    {
        result = data_diagram_get_list_order ( &((*this_).sibling_diagrams[(*this_).siblings_count-1]) );
    }

    return result;
}

static inline int32_t gui_sketch_nav_tree_get_children_highest_order ( gui_sketch_nav_tree_t *this_ )
{
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );
    
    int32_t result;
    
    if ( (*this_).children_count == 0 )
    {
        result = 0;
    }
    else
    {
        result = data_diagram_get_list_order ( &((*this_).child_diagrams[(*this_).children_count-1]) );
    }

    return result;
}

static inline gui_error_t gui_sketch_nav_tree_is_descendant ( gui_sketch_nav_tree_t *this_,
                                                              int64_t probe_ancestor_id,
                                                              int64_t probe_descendant_id,
                                                              bool *out_is_descendant )
{
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );
    
    gui_error_t result;
    
    /* search the two ids in the diagram lists */
    int_fast32_t probe_anc_in_anc_idx = -1;
    int_fast32_t probe_anc_in_sib_idx = -1;
    int_fast32_t probe_anc_in_child_idx = -1;
    int_fast32_t probe_desc_in_anc_idx = -1;
    int_fast32_t probe_desc_in_sib_idx = -1;
    int_fast32_t probe_desc_in_child_idx = -1;
    
    for ( uint_fast32_t anc_idx = 0; anc_idx < (*this_).ancestors_count; anc_idx ++ )
    {
        if ( probe_ancestor_id == data_diagram_get_id ( &((*this_).ancestor_diagrams[anc_idx]) ) )
        {
            probe_anc_in_anc_idx = anc_idx;
        }
        if ( probe_descendant_id == data_diagram_get_id ( &((*this_).ancestor_diagrams[anc_idx]) ) )
        {
            probe_desc_in_anc_idx = anc_idx;
        }
    }
    for ( uint_fast32_t sib_idx = 0; sib_idx < (*this_).siblings_count; sib_idx ++ )
    {
        if ( probe_ancestor_id == data_diagram_get_id ( &((*this_).sibling_diagrams[sib_idx]) ) )
        {
            probe_anc_in_sib_idx = sib_idx;
        }
        if ( probe_descendant_id == data_diagram_get_id ( &((*this_).sibling_diagrams[sib_idx]) ) )
        {
            probe_desc_in_sib_idx = sib_idx;
        }
    }
    for ( uint_fast32_t child_idx = 0; child_idx < (*this_).children_count; child_idx ++ )
    {
        if ( probe_ancestor_id == data_diagram_get_id ( &((*this_).child_diagrams[child_idx]) ) )
        {
            probe_anc_in_child_idx = child_idx;
        }
        if ( probe_descendant_id == data_diagram_get_id ( &((*this_).child_diagrams[child_idx]) ) )
        {
            probe_desc_in_child_idx = child_idx;
        }
    }
    
    /* define the return value */
    if (( probe_anc_in_anc_idx == -1 )&&( probe_anc_in_sib_idx == -1 )&&( probe_anc_in_child_idx == -1 ))
    {
        /* probe_ancestor_id not found */
        result = GUI_ERROR_UNKNOWN_OBJECT;
        *out_is_descendant = false;  /* avoid "uninitialzed" warnings */
    }
    else if (( probe_desc_in_anc_idx == -1 )&&( probe_desc_in_sib_idx == -1 )&&( probe_desc_in_child_idx == -1 ))
    {
        /* probe_descendant_id not found */
        result = GUI_ERROR_UNKNOWN_OBJECT;
        *out_is_descendant = false;  /* avoid "uninitialzed" warnings */
    }
    else
    {
        result = GUI_ERROR_NONE;
        
        if ( probe_anc_in_anc_idx != -1 )
        {
            if ( probe_desc_in_anc_idx != -1 )
            {
                *out_is_descendant = ( probe_anc_in_anc_idx > probe_desc_in_anc_idx ); /* root is last in list */
            }
            else 
            {
                assert( ( probe_desc_in_sib_idx != -1 )||( probe_desc_in_child_idx != -1 ) );
                *out_is_descendant = true;
            }
        }
        else if ( probe_anc_in_sib_idx != -1 )
        {
            if ( probe_desc_in_child_idx != -1 ) 
            {
                *out_is_descendant = ( probe_anc_in_sib_idx == (*this_).siblings_self_index );
            }
            else
            {
                assert( ( probe_desc_in_anc_idx != -1 )||( probe_desc_in_sib_idx != -1 ) );
                *out_is_descendant = false;  /* probe_descendant_id is ancestor or sibling to probe_ancestor_id, not descendant */
            }
        }
        else
        {
            assert ( probe_anc_in_child_idx != -1 );
            *out_is_descendant = false;  /* probe_descendant_id is sibling or ancestor to probe_ancestor_id, not descendant */
        }
    }
    
    return result;
}

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
static const int GUI_SKETCH_NAV_TREE_INDENT = 12;

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
    x_offset = descendant_count * GUI_SKETCH_NAV_TREE_INDENT;

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
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_INDENT;
    }
    else
    {
        y_offset = ( (*this_).line_idx_siblings_start + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_INDENT;
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
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_INDENT + GUI_SKETCH_NAV_TREE_INDENT;
    }

    shape_int_rectangle_init( &result, left + x_offset, top + y_offset, width - x_offset, GUI_SKETCH_NAV_TREE_LINE_HEIGHT );

    return result;
}


/*
Copyright 2018-2019 Andreas Warnke

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
