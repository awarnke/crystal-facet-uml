/* File: gui_sketch_nav_tree.c; Copyright and License: see below */

#include "gui_sketch_nav_tree.h"
#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

static const int STANDARD_FONT_SIZE = 12;
static const char *STANDARD_FONT_FAMILY = "Sans";

void gui_sketch_nav_tree_init( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();

    (*this_).ancestors_count = 0;
    (*this_).siblings_count = 0;
    (*this_).children_count = 0;
    (*this_).siblings_self_index = -1;
    (*this_).new_child_button_index = -1;
    (*this_).new_sibling_button_index = -1;

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    /* init the pango font renering engine stuff: */
    (*this_).standard_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).standard_font_description, STANDARD_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).standard_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).standard_font_description, PANGO_WEIGHT_MEDIUM );
    pango_font_description_set_stretch ( (*this_).standard_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).standard_font_description, STANDARD_FONT_SIZE * PANGO_SCALE );

    gui_sketch_marker_init( &((*this_).sketch_marker) );

    TRACE_END();
}

void gui_sketch_nav_tree_destroy( gui_sketch_nav_tree_t *this_ )
{
    TRACE_BEGIN();

    gui_sketch_marker_destroy( &((*this_).sketch_marker) );

    /* destroy the pango font renering engine stuff: */
    pango_font_description_free ( (*this_).standard_font_description );
    (*this_).standard_font_description = NULL;

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
            id_to_load = data_diagram_get_parent_id( &((*this_).ancestor_diagrams[anc_index-1]) );
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

    /* get siblings */
    if ( db_err == DATA_ERROR_NONE )
    {
        int64_t parent_id;
        parent_id = data_diagram_get_parent_id( &((*this_).ancestor_diagrams[0]) );
        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  parent_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS,
                                                                  &( (*this_).sibling_diagrams ),
                                                                  &( (*this_).siblings_count )
                                                                );

        /* search self in list of siblings */
        {
            assert( (*this_).siblings_count <= GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS );

            for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
            {
                if ( diagram_id == data_diagram_get_id( &((*this_).sibling_diagrams[sib_index]) ) )
                {
                    (*this_).siblings_self_index = sib_index;
                }
            }
        }
    }

    /* get children */
    if ( db_err == DATA_ERROR_NONE )
    {

        db_err = data_database_reader_get_diagrams_by_parent_id ( db_reader,
                                                                  diagram_id,
                                                                  GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN,
                                                                  &( (*this_).child_diagrams ),
                                                                  &( (*this_).children_count )
                                                                );
    }

    /* calculate the "new" button positions */
    if ( (*this_).ancestors_count == 0 )
    {
        /* cannot create a child without a parent */
        (*this_).new_child_button_index = -1;
        /* show only a new sibling button */
        (*this_).new_sibling_button_index = 0;
    }
    else
    {
        (*this_).new_child_button_index = (*this_).ancestors_count - 1 + (*this_).siblings_self_index + 1 + (*this_).children_count + 1;
        if ( (*this_).ancestors_count == 1 )
        {
            /* cannot create a sibling to the root */
            (*this_).new_sibling_button_index = -1;
        }
        else
        {
            (*this_).new_sibling_button_index = (*this_).ancestors_count - 1 + (*this_).siblings_count + (*this_).children_count + 1 + 1;
        }
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
    (*this_).siblings_self_index = -1;
    (*this_).new_child_button_index = -1;
    (*this_).new_sibling_button_index = -1;

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

        /* draw the background */
        {
            cairo_set_source_rgba( cr, 0.8, 0.8, 0.8, 1.0 );
            cairo_rectangle ( cr, left, top, width, height );
            cairo_fill (cr);
        }

        /* draw the elements */
        {
            PangoLayout *layout;
            layout = pango_cairo_create_layout (cr);
            pango_layout_set_font_description ( layout, (*this_).standard_font_description );

            /* EXAMPLE: precalculate text dimensions to vertically center the text */
            /*
            pango_layout_set_text ( layout, "PANGO Demo", -1 );
            int text2_width;
            int text2_height;
            pango_layout_get_pixel_size (layout, &text2_width, &text2_height);
            double y_adjust = ( height - text2_height ) / 2.0;
            */

            shape_int_rectangle_t destination_rect;
            if ( (*this_).ancestors_count >= 2 )
            {
                /* there is at least one ancestor (and self) */
                for ( int anc_index = (*this_).ancestors_count-1; anc_index > 0 /* 0 equals self */; anc_index -- )
                {
                    destination_rect = gui_sketch_nav_tree_private_get_ancestor_bounds( this_, anc_index );

                    gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                    DATA_TABLE_DIAGRAM,
                                                    data_diagram_get_id( &((*this_).ancestor_diagrams[anc_index]) ),
                                                    marker,
                                                    destination_rect,
                                                    cr
                                                  );

                    cairo_move_to ( cr, shape_int_rectangle_get_left( &destination_rect ), shape_int_rectangle_get_top( &destination_rect ) );
                    pango_layout_set_text ( layout, data_diagram_get_name_ptr( &((*this_).ancestor_diagrams[anc_index]) ), -1 );
                    pango_cairo_show_layout ( cr, layout );
                }
            }

            for ( int sib_index = 0; sib_index < (*this_).siblings_count; sib_index ++ )
            {
                destination_rect = gui_sketch_nav_tree_private_get_sibling_bounds( this_, sib_index );

                gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                DATA_TABLE_DIAGRAM,
                                                data_diagram_get_id( &((*this_).sibling_diagrams[sib_index]) ),
                                                marker,
                                                destination_rect,
                                                cr
                                              );

                cairo_move_to ( cr, shape_int_rectangle_get_left( &destination_rect ), shape_int_rectangle_get_top( &destination_rect ) );
                pango_layout_set_text ( layout, data_diagram_get_name_ptr( &((*this_).sibling_diagrams[sib_index]) ), -1 );
                pango_cairo_show_layout ( cr, layout );
            }
            if ( (*this_).ancestors_count != 1 )  /* create sibling if not root diagram */
            {
                destination_rect = gui_sketch_nav_tree_private_get_sibling_bounds( this_, (*this_).siblings_count );
                cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
                cairo_move_to ( cr, shape_int_rectangle_get_left( &destination_rect ), shape_int_rectangle_get_top( &destination_rect ) );
                pango_layout_set_text ( layout, "[+]", -1 );
                pango_cairo_show_layout ( cr, layout );
            }

            for ( int chi_index = 0; chi_index < (*this_).children_count; chi_index ++ )
            {
                destination_rect = gui_sketch_nav_tree_private_get_child_bounds( this_, chi_index );

                gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                DATA_TABLE_DIAGRAM,
                                                data_diagram_get_id( &((*this_).child_diagrams[chi_index]) ),
                                                marker,
                                                destination_rect,
                                                cr
                                              );

                cairo_move_to ( cr, shape_int_rectangle_get_left( &destination_rect ), shape_int_rectangle_get_top( &destination_rect ) );
                pango_layout_set_text ( layout, data_diagram_get_name_ptr( &((*this_).child_diagrams[chi_index]) ), -1 );
                pango_cairo_show_layout ( cr, layout );
            }
            if ( (*this_).ancestors_count != 0 )  /* create children if parent exists */
            {
                destination_rect = gui_sketch_nav_tree_private_get_child_bounds( this_, (*this_).children_count );
                cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
                cairo_move_to ( cr, shape_int_rectangle_get_left( &destination_rect ), shape_int_rectangle_get_top( &destination_rect ) );
                pango_layout_set_text ( layout, "(+)", -1 );
                pango_cairo_show_layout ( cr, layout );
            }
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

