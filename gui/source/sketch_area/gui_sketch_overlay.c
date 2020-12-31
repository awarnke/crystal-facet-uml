/* File: gui_sketch_overlay.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_overlay.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <assert.h>

void gui_sketch_overlay_init( gui_sketch_overlay_t *this_ )
{
    TRACE_BEGIN();

    (*this_).overlay_std_red = 0.2;
    (*this_).overlay_std_green = 1.0;
    (*this_).overlay_std_blue = 0.6;
    (*this_).overlay_std_alpha = 0.8;

    TRACE_END();
}

void gui_sketch_overlay_destroy( gui_sketch_overlay_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void gui_sketch_overlay_draw( gui_sketch_overlay_t *this_,
                              gui_tool_t selected_tool,
                              const gui_sketch_drag_state_t *drag_state,
                              const gui_sketch_card_t *card_under_mouse,
                              const gui_sketch_nav_tree_t *nav_tree,
                              gui_marked_set_t *marked_objects,
                              cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );
    assert( NULL != nav_tree );

    switch ( selected_tool )
    {
        case GUI_TOOLBOX_NAVIGATE:
        {
            gui_sketch_overlay_private_draw_nav_mode( this_, drag_state, nav_tree, cr );
        }
        break;

        case GUI_TOOLBOX_EDIT:
        {
            gui_sketch_overlay_private_draw_edit_mode( this_, drag_state, card_under_mouse, cr );
        }
        break;

        case GUI_TOOLBOX_CREATE:
        {
            const data_id_t highlighted = gui_marked_set_get_highlighted( marked_objects );
            gui_sketch_overlay_private_draw_create_mode( this_, drag_state, card_under_mouse, data_id_get_table(&highlighted), cr );
        }
        break;

        case GUI_TOOLBOX_SEARCH:
        {
        }
        break;

        default:
        {
            TSLOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    TRACE_END();
}

void gui_sketch_overlay_private_draw_nav_mode( gui_sketch_overlay_t *this_,
                                               const gui_sketch_drag_state_t *drag_state,
                                               const gui_sketch_nav_tree_t *nav_tree,
                                               cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );
    assert( NULL != nav_tree );

    if ( gui_sketch_drag_state_is_dragging ( drag_state ) )
    {
        const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
        const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );
        data_id_t out_parent_id;
        int32_t out_list_order;
        shape_int_rectangle_t out_gap_line;
        gui_error_t gap_err;
        gap_err = gui_sketch_nav_tree_get_gap_info_at_pos ( nav_tree,
                                                            to_x,
                                                            to_y,
                                                            &out_parent_id,
                                                            &out_list_order,
                                                            &out_gap_line
                                                          );
        if ( gap_err == GUI_ERROR_NONE )
        {
            cairo_set_source_rgba( cr,
                                   (*this_).overlay_std_red,
                                   (*this_).overlay_std_green,
                                   (*this_).overlay_std_blue,
                                   (*this_).overlay_std_alpha
                                 );
            cairo_rectangle ( cr,
                              shape_int_rectangle_get_left(&out_gap_line),
                              shape_int_rectangle_get_top(&out_gap_line),
                              shape_int_rectangle_get_width(&out_gap_line),
                              shape_int_rectangle_get_height(&out_gap_line)
                            );
            cairo_fill (cr);
        }
        else
        {
            TRACE_INFO("dragging diagram outside nav_tree");
        }
    }
    TRACE_END();
}

void gui_sketch_overlay_private_draw_edit_mode( gui_sketch_overlay_t *this_,
                                                const gui_sketch_drag_state_t *drag_state,
                                                const gui_sketch_card_t *card_under_mouse,
                                                cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );

    if ( gui_sketch_drag_state_is_dragging ( drag_state ) )
    {
        if ( NULL != card_under_mouse )
        {
            universal_bool_list_t is_snapped;
            const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
            const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );
            is_snapped = gui_sketch_card_is_pos_on_grid ( card_under_mouse, to_x, to_y );

            cairo_set_source_rgba( cr,
                                   (*this_).overlay_std_red,
                                   (*this_).overlay_std_green,
                                   (*this_).overlay_std_blue,
                                   (*this_).overlay_std_alpha
                                 );
            if ( universal_bool_list_get_first ( &is_snapped ) )
            {
                cairo_move_to ( cr, to_x, to_y-60 );
                cairo_line_to ( cr, to_x, to_y-20 );
                cairo_move_to ( cr, to_x, to_y+20 );
                cairo_line_to ( cr, to_x, to_y+60 );
                cairo_stroke (cr);
            }
            if ( universal_bool_list_get_second ( &is_snapped ) )
            {
                cairo_move_to ( cr, to_x-60, to_y );
                cairo_line_to ( cr, to_x-20, to_y );
                cairo_move_to ( cr, to_x+20, to_y );
                cairo_line_to ( cr, to_x+60, to_y );
                cairo_stroke (cr);
            }

            /* draw lines */
            const data_diagram_t *diag = gui_sketch_card_get_diagram_const ( card_under_mouse );
            const data_diagram_type_t diag_type = data_diagram_get_diagram_type( diag );
            if (( diag_type != DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
                && ( diag_type != DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
                && ( diag_type != DATA_DIAGRAM_TYPE_LIST ))
            {
                shape_int_rectangle_t bounds;
                uint32_t x_count;
                uint32_t y_count;
                gui_sketch_card_get_grid_area( card_under_mouse, &bounds, &x_count, &y_count );
                for ( uint32_t x_idx = 0; x_idx < x_count; x_idx ++ )
                {
                    for ( uint32_t y_idx = 0; y_idx < y_count; y_idx ++ )
                    {
                        const int32_t x = shape_int_rectangle_get_left(&bounds)
                                          + (( shape_int_rectangle_get_width(&bounds) * x_idx )/( x_count-1 ));
                        const int32_t y = shape_int_rectangle_get_top(&bounds)
                                          + (( shape_int_rectangle_get_height(&bounds) * y_idx )/( y_count-1 ));
                        static const int32_t HALF_LINE = 4;
                        cairo_move_to ( cr, x-HALF_LINE, y );
                        cairo_line_to ( cr, x+HALF_LINE, y );
                        cairo_move_to ( cr, x, y-HALF_LINE );
                        cairo_line_to ( cr, x, y+HALF_LINE );
                        cairo_stroke (cr);
                    }
                }
            }
            else
            {
                /* no x/y grid for seq, timing and list diagrams*/
            }
        }
    }

    TRACE_END();
}

void gui_sketch_overlay_private_draw_create_mode( gui_sketch_overlay_t *this_,
                                                  const gui_sketch_drag_state_t *drag_state,
                                                  const gui_sketch_card_t *card_under_mouse,
                                                  data_table_t highlighted_object_table,
                                                  cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );

    if ( gui_sketch_drag_state_is_dragging ( drag_state ) )
    {
        /* draw a line */
        const int32_t from_x = gui_sketch_drag_state_get_from_x ( drag_state );
        const int32_t from_y = gui_sketch_drag_state_get_from_y ( drag_state );
        const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
        const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );
        cairo_set_source_rgba( cr,
                                (*this_).overlay_std_red,
                                (*this_).overlay_std_green,
                                (*this_).overlay_std_blue,
                                (*this_).overlay_std_alpha
        );
        cairo_move_to ( cr, from_x, from_y );
        cairo_line_to ( cr, to_x, to_y );
        cairo_stroke (cr);

        /* draw the arrow tip */
        int clock_direction;  /* the gdk coordinate system if bottom up - and out clock is also mirrored! */
        const int32_t x_dist = to_x - from_x;
        const int32_t y_dist = to_y - from_y;
        if ( x_dist == 0 )
        {
            clock_direction = ( y_dist < 0 ) ? 6 : 0;
        }
        else
        {
            int32_t y_x_deci_ratio = ( y_dist * 10 ) / x_dist;
            if ( y_x_deci_ratio < 3 /* = 0.26 = tan(15 deg) */ )
            {
                if ( y_x_deci_ratio < -10 /* = -1.00 = tan(-45 deg) */ )
                {
                    if ( y_x_deci_ratio < -37 /* = -3.73 = tan(-75 deg) */ )
                    {
                        clock_direction = 6;
                    }
                    else
                    {
                        clock_direction = 5;
                    }
                }
                else
                {
                    if ( y_x_deci_ratio < -2 /* = -0.26 = tan(-15 deg) */ )
                    {
                        clock_direction = 4;
                    }
                    else
                    {
                        clock_direction = 3;
                    }
                }
            }
            else
            {
                if ( y_x_deci_ratio < 10 /* = 1.00 = tan(45 deg) */ )
                {
                    clock_direction = 2;
                }
                else
                {
                    if ( y_x_deci_ratio < 38 /* = 3.73 = tan(75 deg) */ )
                    {
                        clock_direction = 1;
                    }
                    else
                    {
                        clock_direction = 0;
                    }
                }
            }
            if ( x_dist < 0 )
            {
                clock_direction = (clock_direction+6)%12;
            }
        }
        const static int32_t DX[12][2]={{-6,+6},{-9,  0},{-11,-4},{-9,-9},{-4,-11},{  0,-9},{+6,-6},{+9,  0},{+11,+4},{+9,+9},{+4,+11},{  0,+9}};
        const static int32_t DY[12][2]={{-9,-9},{-4,-11},{  0,-9},{+6,-6},{+9,  0},{+11,+4},{+9,+9},{+4,+11},{  0,+9},{+6,-6},{-9,  0},{-11,-4}};
        cairo_move_to ( cr, to_x + DX[clock_direction][0], to_y + DY[clock_direction][0] );
        cairo_line_to ( cr, to_x, to_y );
        cairo_line_to ( cr, to_x + DX[clock_direction][1], to_y + DY[clock_direction][1] );
        cairo_stroke (cr);
    }
    else /* ! gui_sketch_drag_state_is_dragging() */
    {
        if ( NULL != card_under_mouse )
        {
            const bool draw_arrow 
                = (( highlighted_object_table == DATA_TABLE_CLASSIFIER )
                ||( highlighted_object_table == DATA_TABLE_FEATURE )
                ||( highlighted_object_table == DATA_TABLE_DIAGRAMELEMENT ));
            
            /* get coordinates */
            const int32_t cur_x = gui_sketch_drag_state_get_to_x ( drag_state );
            const int32_t cur_y = gui_sketch_drag_state_get_to_y ( drag_state );
            cairo_set_source_rgba( cr,
                                    (*this_).overlay_std_red,
                                    (*this_).overlay_std_green,
                                    (*this_).overlay_std_blue,
                                    (*this_).overlay_std_alpha
            );
            static const int32_t ICON_UNIT = 2;
            static const int32_t DIST = 12;

            /* star */
            const int32_t star_center_x = cur_x+(draw_arrow?-2:1)*ICON_UNIT;
            const int32_t star_center_y = cur_y-DIST-6*ICON_UNIT;
            cairo_move_to ( cr, star_center_x, star_center_y-2*ICON_UNIT );
            cairo_line_to ( cr, star_center_x, star_center_y+2*ICON_UNIT );
            cairo_move_to ( cr, star_center_x-2*ICON_UNIT, star_center_y-1*ICON_UNIT );
            cairo_line_to ( cr, star_center_x+2*ICON_UNIT, star_center_y+1*ICON_UNIT );
            cairo_move_to ( cr, star_center_x-2*ICON_UNIT, star_center_y+1*ICON_UNIT );
            cairo_line_to ( cr, star_center_x+2*ICON_UNIT, star_center_y-1*ICON_UNIT );

            if ( draw_arrow )
            {
                /* small box */
                cairo_move_to ( cr, cur_x-4*ICON_UNIT, cur_y-DIST-6*ICON_UNIT );
                cairo_line_to ( cr, cur_x-8*ICON_UNIT, cur_y-DIST-6*ICON_UNIT );
                cairo_line_to ( cr, cur_x-8*ICON_UNIT, cur_y-DIST-0*ICON_UNIT );
                cairo_line_to ( cr, cur_x-2*ICON_UNIT, cur_y-DIST-0*ICON_UNIT );
                cairo_line_to ( cr, cur_x-2*ICON_UNIT, cur_y-DIST-2*ICON_UNIT );

                /* arrow */
                cairo_move_to ( cr, cur_x+2*ICON_UNIT, cur_y-DIST-0*ICON_UNIT );
                cairo_line_to ( cr, cur_x+8*ICON_UNIT, cur_y-DIST-6*ICON_UNIT );
                cairo_line_to ( cr, cur_x+5*ICON_UNIT, cur_y-DIST-5*ICON_UNIT );
                cairo_move_to ( cr, cur_x+8*ICON_UNIT, cur_y-DIST-6*ICON_UNIT );
                cairo_line_to ( cr, cur_x+7*ICON_UNIT, cur_y-DIST-3*ICON_UNIT );
            }
            else
            {
                /* big box */
                cairo_move_to ( cr, cur_x-1*ICON_UNIT, cur_y-DIST-6*ICON_UNIT );
                cairo_line_to ( cr, cur_x-8*ICON_UNIT, cur_y-DIST-6*ICON_UNIT );
                cairo_line_to ( cr, cur_x-8*ICON_UNIT, cur_y-DIST+3*ICON_UNIT );
                cairo_line_to ( cr, cur_x+1*ICON_UNIT, cur_y-DIST+3*ICON_UNIT );
                cairo_line_to ( cr, cur_x+1*ICON_UNIT, cur_y-DIST-2*ICON_UNIT );
            }

            cairo_stroke (cr);
        }
    }

    TRACE_END();
}


/*
Copyright 2017-2020 Andreas Warnke

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
