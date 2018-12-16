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

        if ( (*this_).ancestors_count == 0 )
        {
            if ( line_index == 0 )
            {
                *out_action_id = GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM;
            }
            else
            {
                *out_action_id = GUI_SKETCH_ACTION_NONE;
            }
        }
        else
        {
            if ( line_index == (*this_).new_child_button_index )
            {
                *out_action_id = GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM;
            }
            else if ( line_index == (*this_).new_sibling_button_index )
            {
                *out_action_id = GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM;
            }
            else
            {
                *out_action_id = GUI_SKETCH_ACTION_NONE;
            }
        }
    }
    else
    {
        /* position out of bounding box */
        *out_action_id = GUI_SKETCH_ACTION_NONE;
    }
}

static inline gui_error_t gui_sketch_nav_tree_get_gap_info_at_pos ( gui_sketch_nav_tree_t *this_,
                                                                    int32_t x,
                                                                    int32_t y,
                                                                    data_id_t *out_parent_id,
                                                                    int32_t *out_list_order,
                                                                    shape_int_rectangle_t *out_gap_line )
{
    assert ( NULL != out_parent_id );
    assert ( NULL != out_list_order );
    assert ( NULL != out_gap_line );
    
    gui_error_t ret_error = GUI_ERROR_NONE;

    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        /* determine index of line, top linie has index 0 */
        int32_t top;
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        uint32_t gap_index;
        gap_index = ( y+(GUI_SKETCH_NAV_TREE_LINE_HEIGHT/2) - top ) / GUI_SKETCH_NAV_TREE_LINE_HEIGHT;

        shape_int_rectangle_init( out_gap_line, 
                                  shape_int_rectangle_get_left( &((*this_).bounds) ),
                                  gap_index*GUI_SKETCH_NAV_TREE_LINE_HEIGHT-1,
                                  shape_int_rectangle_get_width( &((*this_).bounds) ),
                                  2
                                );
#if 0
        /* is this the ancester region ? */
        uint32_t real_ancestors = ( (*this_).ancestors_count > 0 ) ? ( (*this_).ancestors_count - 1 ) : 0;
        if ( line_index < real_ancestors )
        {
            uint32_t ancester_idx = real_ancestors - line_index;
            data_id_reinit( out_selected_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_id( &((*this_).ancestor_diagrams[ancester_idx]) )
            );
        }
        else
        {
            uint32_t siblings_line = line_index - real_ancestors;
            if ( siblings_line <= (*this_).siblings_self_index )
            {
                data_id_reinit( out_selected_id,
                                DATA_TABLE_DIAGRAM,
                                data_diagram_get_id( &((*this_).sibling_diagrams[siblings_line]) )
                );
            }
            else
            {
                uint32_t child_line = siblings_line - (*this_).siblings_self_index - 1;
                if ( child_line < (*this_).children_count )
                {
                    data_id_reinit( out_selected_id,
                                    DATA_TABLE_DIAGRAM,
                                    data_diagram_get_id( &((*this_).child_diagrams[child_line]) )
                    );
                }
                else
                {
                    siblings_line -= (*this_).children_count;
                    if ( siblings_line == (*this_).siblings_self_index + 1 )
                    {
                        /* reserved for new child sign */
                        data_id_reinit_void( out_selected_id );
                    }
                    else if ( siblings_line <= (*this_).siblings_count )
                    {
                        data_id_reinit( out_selected_id,
                                        DATA_TABLE_DIAGRAM,
                                        data_diagram_get_id( &((*this_).sibling_diagrams[siblings_line-1]) )
                        );
                    }
                    else
                    {
                        data_id_reinit_void( out_selected_id );
                    }
                }
            }
        }
#endif // 0
    }
    else
    {
        ret_error = GUI_ERROR_OUT_OF_BOUNDS;
    }
    return ret_error;
}

static inline void gui_sketch_nav_tree_get_object_id_at_pos ( gui_sketch_nav_tree_t *this_,
                                                              int32_t x,
                                                              int32_t y,
                                                              data_id_t *out_selected_id )
{
    assert ( NULL != out_selected_id );

    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        /* determine index of line, top linie has index 0 */
        int32_t top;
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        uint32_t line_index;
        line_index = ( y - top ) / GUI_SKETCH_NAV_TREE_LINE_HEIGHT;

        /* is this the ancester region ? */
        uint32_t real_ancestors = ( (*this_).ancestors_count > 0 ) ? ( (*this_).ancestors_count - 1 ) : 0;
        if ( line_index < real_ancestors )
        {
            uint32_t ancester_idx = real_ancestors - line_index;
            data_id_reinit( out_selected_id,
                            DATA_TABLE_DIAGRAM,
                            data_diagram_get_id( &((*this_).ancestor_diagrams[ancester_idx]) )
                          );
        }
        else
        {
            uint32_t siblings_line = line_index - real_ancestors;
            if ( siblings_line <= (*this_).siblings_self_index )
            {
                data_id_reinit( out_selected_id,
                                DATA_TABLE_DIAGRAM,
                                data_diagram_get_id( &((*this_).sibling_diagrams[siblings_line]) )
                              );
            }
            else
            {
                uint32_t child_line = siblings_line - (*this_).siblings_self_index - 1;
                if ( child_line < (*this_).children_count )
                {
                    data_id_reinit( out_selected_id,
                                    DATA_TABLE_DIAGRAM,
                                    data_diagram_get_id( &((*this_).child_diagrams[child_line]) )
                                  );
                }
                else
                {
                    siblings_line -= (*this_).children_count;
                    if ( siblings_line == (*this_).siblings_self_index + 1 )
                    {
                        /* reserved for new child sign */
                        data_id_reinit_void( out_selected_id );
                    }
                    else if ( siblings_line <= (*this_).siblings_count )
                    {
                        data_id_reinit( out_selected_id,
                                        DATA_TABLE_DIAGRAM,
                                        data_diagram_get_id( &((*this_).sibling_diagrams[siblings_line-1]) )
                                      );
                    }
                    else
                    {
                        data_id_reinit_void( out_selected_id );
                    }
                }
            }
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
    /*uint32_t height;*/

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    /*height = shape_int_rectangle_get_height( &((*this_).bounds) );*/

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
        y_offset = sibling_index * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = 0;
    }
    else if ( sibling_index > (*this_).siblings_self_index )
    {
        y_offset = ( (*this_).ancestors_count - 1 + (*this_).children_count + 1 + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = ( (*this_).ancestors_count - 1 ) * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT;
    }
    else
    {
        y_offset = ( (*this_).ancestors_count - 1 + sibling_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
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
    if (( (*this_).siblings_self_index < 0 )||( (*this_).ancestors_count == 0 ))
    {
        /* error case */
        /* if self is not a sibling, simply add children to the end */
        y_offset = ( (*this_).ancestors_count + (*this_).siblings_count + child_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = 0;
    }
    else
    {
        y_offset = ( (*this_).ancestors_count - 1 + (*this_).siblings_self_index + 1 + child_index ) * GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
        x_offset = ( (*this_).ancestors_count - 1 )  * GUI_SKETCH_NAV_TREE_ANCESTOR_INDENT + GUI_SKETCH_NAV_TREE_CHILD_INDENT;
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
