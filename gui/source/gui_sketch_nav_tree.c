/* File: gui_sketch_nav_tree.c; Copyright and License: see below */

#include "gui_sketch_nav_tree.h"
#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

void gui_sketch_nav_tree_init( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();

    (*this_).ancestors_count = 0;
    (*this_).siblings_count = 0;
    (*this_).children_count = 0;

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    TRACE_END();
}

void gui_sketch_nav_tree_destroy( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();

    gui_sketch_nav_tree_invalidate_data( this_ );

    shape_int_rectangle_destroy(&((*this_).bounds));

    TRACE_END();
}

void gui_sketch_nav_tree_load_data( gui_sketch_nav_tree_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    gui_sketch_nav_tree_invalidate_data( this_ );

    data_error_t db_err = DATA_ERROR_NONE;

    /* get ancestors */
    bool finished = false;
    for ( unsigned int anc_index = 0; ( anc_index < GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS ) && ( db_err == DATA_ERROR_NONE ) && ( ! finished ); anc_index ++ )
    {
        int64_t id_to_load;
        if ( anc_index == 0 )
        {
            id_to_load = diagram_id;
        }
        else
        {
            id_to_load = data_diagram_get_id( &((*this_).ancestor_diagrams[anc_index-1]) );
        }

        if ( id_to_load != DATA_ID_VOID_ID )
        {
            db_err = data_database_reader_get_diagram_by_id ( db_reader, id_to_load, &((*this_).ancestor_diagrams[anc_index]) );
            if ( db_err == DATA_ERROR_NONE )
            {
                (*this_).ancestors_count = anc_index+1;
            }
        }
        else
        {
            finished = true;
        }
    }

    if ( (*this_).ancestors_count == 0 )
    {
        db_err = DATA_ERROR_INVALID_REQUEST;
        TSLOG_ANOMALY_INT( "gui_sketch_nav_tree_load_data cannot load diagram", diagram_id );
    }

    if ( db_err == DATA_ERROR_NONE )
    {
        int64_t parent_id;
        parent_id = data_diagram_get_id( &((*this_).ancestor_diagrams[0]) );
        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  parent_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS,
                                                                  &( (*this_).sibling_diagrams ),
                                                                  &( (*this_).siblings_count )
                                                                );
    }

    if ( db_err == DATA_ERROR_NONE )
    {

        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  diagram_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN,
                                                                  &( (*this_).child_diagrams ),
                                                                  &( (*this_).children_count )
                                                                );
    }


    TRACE_END();
}

void gui_sketch_nav_tree_invalidate_data( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    for ( int anc_index = 0; anc_index < (*this_).ancestors_count; anc_index ++ )
    {
        data_diagram_destroy( &((*this_).ancestor_diagrams[anc_index]) );
    }
    (*this_).ancestors_count = 0;

    for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
    {
        data_diagram_destroy( &((*this_).sibling_diagrams[sib_index]) );
    }
    (*this_).siblings_count = 0;

    for ( int chi_index = 0; chi_index < (*this_).children_count; chi_index ++ )
    {
        data_diagram_destroy( &((*this_).child_diagrams[chi_index]) );
    }
    (*this_).children_count = 0;

    TRACE_END();
}

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;
static const double TITLE_FONT_SIZE = 12.0;

void gui_sketch_nav_tree_draw ( gui_sketch_nav_tree_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != marker );
    assert( NULL != cr );
    assert( (*this_).ancestors_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS );
    assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );
    assert( (*this_).children_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN );

    if ( (*this_).visible )
    {
        int32_t left;
        int32_t top;
        uint32_t width;
        uint32_t height;

        left = shape_int_rectangle_get_left( &((*this_).bounds) );
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        width = shape_int_rectangle_get_width( &((*this_).bounds) );
        height = shape_int_rectangle_get_height( &((*this_).bounds) );

        cairo_set_source_rgba( cr, 0.8, 0.8, 0.8, 1.0 );
        cairo_rectangle ( cr, left+10, top+10, width-20, height-20 );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
        cairo_set_font_size ( cr, TITLE_FONT_SIZE );
        cairo_move_to ( cr, 10, 10+14 );
        cairo_show_text ( cr, "gui_sketch_nav_tree_t" );
        cairo_move_to ( cr, 10, 10+2*14 );
        cairo_show_text ( cr, "gui_sketch_nav_tree_t" );

        uint32_t cursor_y = 10+2*14;
        for ( int anc_index = 0; anc_index < (*this_).ancestors_count; anc_index ++ )
        {
            cairo_move_to ( cr, 10, cursor_y );
            cairo_show_text ( cr, data_diagram_get_name_ptr( &((*this_).ancestor_diagrams[anc_index]) ) );
            cursor_y += 14;
        }

        for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
        {
            cairo_move_to ( cr, 10, cursor_y );
            cairo_show_text ( cr, data_diagram_get_name_ptr( &((*this_).sibling_diagrams[sib_index]) ) );
            cursor_y += 14;
        }

        for ( int chi_index = 0; chi_index < (*this_).children_count; chi_index ++ )
        {
            cairo_move_to ( cr, 10, cursor_y );
            cairo_show_text ( cr, data_diagram_get_name_ptr( &((*this_).child_diagrams[chi_index]) ) );
            cursor_y += 14;
        }


    }

    TRACE_END();
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

