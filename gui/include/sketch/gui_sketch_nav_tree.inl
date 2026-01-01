/* File: gui_sketch_nav_tree.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"

static inline data_row_t gui_sketch_nav_tree_get_root_diagram_id ( const gui_sketch_nav_tree_t *this_ )
{
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    data_row_t result;

    if ( (*this_).ancestors_count == 0 )
    {
        result = DATA_ROW_VOID;
    }
    else
    {
        result = data_diagram_get_row_id ( &((*this_).ancestor_diagrams[(*this_).ancestors_count-1]) );
    }

    return result;
}

static inline int32_t gui_sketch_nav_tree_get_siblings_highest_order ( const gui_sketch_nav_tree_t *this_ )
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

static inline int32_t gui_sketch_nav_tree_get_children_highest_order ( const gui_sketch_nav_tree_t *this_ )
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

static inline gui_error_t gui_sketch_nav_tree_is_descendant ( const gui_sketch_nav_tree_t *this_,
                                                              data_row_t probe_ancestor_id,
                                                              data_row_t probe_descendant_id,
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
        if ( probe_ancestor_id == data_diagram_get_row_id ( &((*this_).ancestor_diagrams[anc_idx]) ) )
        {
            probe_anc_in_anc_idx = anc_idx;
        }
        if ( probe_descendant_id == data_diagram_get_row_id ( &((*this_).ancestor_diagrams[anc_idx]) ) )
        {
            probe_desc_in_anc_idx = anc_idx;
        }
    }
    for ( uint_fast32_t sib_idx = 0; sib_idx < (*this_).siblings_count; sib_idx ++ )
    {
        if ( probe_ancestor_id == data_diagram_get_row_id ( &((*this_).sibling_diagrams[sib_idx]) ) )
        {
            probe_anc_in_sib_idx = sib_idx;
        }
        if ( probe_descendant_id == data_diagram_get_row_id ( &((*this_).sibling_diagrams[sib_idx]) ) )
        {
            probe_desc_in_sib_idx = sib_idx;
        }
    }
    for ( uint_fast32_t child_idx = 0; child_idx < (*this_).children_count; child_idx ++ )
    {
        if ( probe_ancestor_id == data_diagram_get_row_id ( &((*this_).child_diagrams[child_idx]) ) )
        {
            probe_anc_in_child_idx = child_idx;
        }
        if ( probe_descendant_id == data_diagram_get_row_id ( &((*this_).child_diagrams[child_idx]) ) )
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

static inline shape_int_rectangle_t gui_sketch_nav_tree_get_bounds( const gui_sketch_nav_tree_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_nav_tree_set_bounds( gui_sketch_nav_tree_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_nav_tree_is_visible( const gui_sketch_nav_tree_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_nav_tree_set_visible( gui_sketch_nav_tree_t *this_, bool visible )
{
    (*this_).visible = visible;
}

static inline const data_diagram_t *gui_sketch_nav_tree_get_diagram_ptr ( const gui_sketch_nav_tree_t *this_ )
{
    return &((*this_).ancestor_diagrams[0]);
}

static inline void gui_sketch_nav_tree_get_button_at_pos ( const gui_sketch_nav_tree_t *this_,
                                                           int32_t x,
                                                           int32_t y,
                                                           gui_sketch_action_t *out_action_id )
{
    assert ( NULL != out_action_id );

    /* default in case no object found */
    {
        *out_action_id = GUI_SKETCH_ACTION_NONE;
    }

    /* search object */
    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        const unsigned int count = (*this_).node_count;
        assert( count <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
        for ( unsigned int idx = 0; idx < count; idx ++ )
        {
            const pos_nav_tree_node_t *const node = &((*this_).node_pos[idx]);
            const shape_int_rectangle_t *icon_box = pos_nav_tree_node_get_icon_box_const( node );
            const shape_int_rectangle_t *label_box = pos_nav_tree_node_get_label_box_const( node );

            if ( shape_int_rectangle_contains( icon_box, x, y ) || shape_int_rectangle_contains( label_box, x, y ) )
            {
                const pos_nav_tree_node_type_t node_type = pos_nav_tree_node_get_type( node );
                switch( node_type )
                {
                    case POS_NAV_TREE_NODE_TYPE_NEW_ROOT:
                    {
                        *out_action_id = GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM;
                    }
                    break;

                    case POS_NAV_TREE_NODE_TYPE_NEW_SIBLING:
                    {
                        *out_action_id = GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM;
                    }
                    break;

                    case POS_NAV_TREE_NODE_TYPE_NEW_CHILD:
                    {
                        *out_action_id = GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM;
                    }
                    break;

                    default:
                    {
                    }
                }
            }
        }
    }
}

static inline const pos_nav_tree_node_t *gui_sketch_nav_tree_get_node_pos_const ( const gui_sketch_nav_tree_t *this_,
                                                                                  uint32_t index )
{
    assert( index < (*this_).node_count );
    return &((*this_).node_pos[index]);
}

static inline uint32_t gui_sketch_nav_tree_get_node_count ( const gui_sketch_nav_tree_t *this_ )
{
    assert( (*this_).node_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
    return (*this_).node_count;
}

static inline const pos_nav_tree_gap_t *gui_sketch_nav_tree_get_gap_pos_const ( const gui_sketch_nav_tree_t *this_,
                                                                                uint32_t index )
{
    assert( index < (*this_).gap_count );
    return &((*this_).gap_pos[index]);
}

static inline uint32_t gui_sketch_nav_tree_get_gap_count ( const gui_sketch_nav_tree_t *this_ )
{
    assert( (*this_).gap_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_GAPS );
    return (*this_).gap_count;
}

static inline u8_error_t gui_sketch_nav_tree_get_node_envelope ( gui_sketch_nav_tree_t *this_,
                                                                 const data_id_t* diagram_id,
                                                                 shape_int_rectangle_t* out_node_envelope_box )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diagram_id );
    assert ( NULL != out_node_envelope_box );
    u8_error_t err = U8_ERROR_NOT_FOUND;

    /* search node */
    const unsigned int count = (*this_).node_count;
    assert( count <= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES );
    for ( unsigned int idx = 0; ( idx < count )&&( err != U8_ERROR_NONE ); idx ++ )
    {
        const pos_nav_tree_node_t *const node = &((*this_).node_pos[idx]);
        const data_id_t node_id = pos_nav_tree_node_get_diagram_id( node );
        /* U8_TRACE_INFO_INT( "node_id", data_id_get_row_id( &node_id ) ); */
        if ( data_id_equals( &node_id, diagram_id ) )
        {
            const shape_int_rectangle_t *icon_box = pos_nav_tree_node_get_icon_box_const( node );
            const shape_int_rectangle_t *label_box = pos_nav_tree_node_get_label_box_const( node );
            shape_int_rectangle_init_by_bounds( out_node_envelope_box, icon_box, label_box );
            err = U8_ERROR_NONE;
        }
    }

    U8_TRACE_END_ERR( err );
    return err;
}


/*
Copyright 2018-2026 Andreas Warnke

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
