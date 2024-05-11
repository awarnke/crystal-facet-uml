/* File: gui_sketch_overlay.c; Copyright and License: see below */

#include "sketch/gui_sketch_overlay.h"
#include "sketch/gui_sketch_snap_state.h"
#include "u8/u8_trace.h"
#include <gtk/gtk.h>
#include <assert.h>

void gui_sketch_overlay_init( gui_sketch_overlay_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_sketch_style_init( &((*this_).sketch_style) );

    U8_TRACE_END();
}

void gui_sketch_overlay_destroy( gui_sketch_overlay_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_sketch_style_destroy( &((*this_).sketch_style) );

    U8_TRACE_END();
}

void gui_sketch_overlay_draw( gui_sketch_overlay_t *this_,
                              gui_tool_t selected_tool,
                              const gui_sketch_drag_state_t *drag_state,
                              const gui_sketch_card_t *card_under_mouse,
                              gui_marked_set_t *marked_objects,
                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );

    switch ( selected_tool )
    {
        case GUI_TOOL_NAVIGATE:
        {
        }
        break;

        case GUI_TOOL_EDIT:
        {
            gui_sketch_overlay_private_draw_edit_mode( this_, drag_state, card_under_mouse, cr );
        }
        break;

        case GUI_TOOL_CREATE:
        {
            const data_id_t highlighted = gui_marked_set_get_highlighted( marked_objects );
            const layout_subelement_kind_t highlighted_kind = gui_marked_set_get_highlighted_kind( marked_objects );
            gui_sketch_overlay_private_draw_create_mode( this_,
                                                         drag_state,
                                                         card_under_mouse,
                                                         data_id_get_table(&highlighted),
                                                         highlighted_kind,
                                                         cr
                                                       );
        }
        break;

        case GUI_TOOL_SEARCH:
        {
        }
        break;

        default:
        {
            U8_LOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    U8_TRACE_END();
}


void gui_sketch_overlay_private_draw_edit_mode( gui_sketch_overlay_t *this_,
                                                const gui_sketch_drag_state_t *drag_state,
                                                const gui_sketch_card_t *card_under_mouse,
                                                cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );

    if ( gui_sketch_drag_state_is_dragging ( drag_state ) )
    {
        if ( NULL != card_under_mouse )
        {
            const data_diagram_t *diag = gui_sketch_card_get_diagram_const ( card_under_mouse );
            const data_diagram_type_t diag_type = data_diagram_get_diagram_type( diag );
            const bool draw_2d_diagram
                = (( diag_type != DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
                && ( diag_type != DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
                && ( diag_type != DATA_DIAGRAM_TYPE_LIST ));

            if ( draw_2d_diagram )
            {
                /* draw grid line crossings */
                gui_sketch_overlay_private_draw_grid( this_, card_under_mouse, cr );

                /* draw snap-to-grid indicator */
                const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
                const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );
                const gui_sketch_snap_state_t snapped = gui_sketch_card_is_pos_on_grid ( card_under_mouse, to_x, to_y );
                gui_sketch_overlay_private_draw_snap_indicator( this_, card_under_mouse, snapped, to_x, to_y, cr );
            }
        }
    }

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_create_mode( gui_sketch_overlay_t *this_,
                                                  const gui_sketch_drag_state_t *drag_state,
                                                  const gui_sketch_card_t *card_under_mouse,
                                                  data_table_t highlighted_table,
                                                  layout_subelement_kind_t highlighted_kind,
                                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != cr );

    if ( gui_sketch_drag_state_is_dragging ( drag_state ) )
    {
        /* draw a line */
        const int32_t from_x = gui_sketch_drag_state_get_from_x ( drag_state );
        const int32_t from_y = gui_sketch_drag_state_get_from_y ( drag_state );
        const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
        const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );
        gui_sketch_overlay_private_draw_arrow( this_, from_x, from_y, to_x, to_y, cr );
    }
    else /* ! gui_sketch_drag_state_is_dragging() */
    {
        if ( NULL != card_under_mouse )
        {
            const bool draw_new_classifier
                = (( highlighted_table == DATA_TABLE_CLASSIFIER )&&( highlighted_kind == LAYOUT_SUBELEMENT_KIND_SPACE ))
                || (( highlighted_table == DATA_TABLE_DIAGRAMELEMENT )&&( highlighted_kind == LAYOUT_SUBELEMENT_KIND_SPACE ))
                || (( highlighted_table == DATA_TABLE_DIAGRAM )&&( highlighted_kind == LAYOUT_SUBELEMENT_KIND_SPACE ));
            const bool draw_new_feature
                = (( highlighted_table == DATA_TABLE_CLASSIFIER )&&( highlighted_kind == LAYOUT_SUBELEMENT_KIND_OUTLINE ))
                || (( highlighted_table == DATA_TABLE_DIAGRAMELEMENT )&&( highlighted_kind == LAYOUT_SUBELEMENT_KIND_OUTLINE ));
            const bool draw_new_relationship
                = (( highlighted_table == DATA_TABLE_CLASSIFIER )&&( highlighted_kind != LAYOUT_SUBELEMENT_KIND_SPACE ))
                || (( highlighted_table == DATA_TABLE_DIAGRAMELEMENT )&&( highlighted_kind != LAYOUT_SUBELEMENT_KIND_SPACE ))
                || ( highlighted_table == DATA_TABLE_FEATURE );

            const data_diagram_t *diag = gui_sketch_card_get_diagram_const ( card_under_mouse );
            const data_diagram_type_t diag_type = data_diagram_get_diagram_type( diag );
            const bool draw_2d_diagram
                = (( diag_type != DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
                && ( diag_type != DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
                && ( diag_type != DATA_DIAGRAM_TYPE_LIST ));

            /* get coordinates */
            const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
            const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );

            /* draw grid */
            if (( draw_new_classifier )&&( draw_2d_diagram ))
            {
                /* draw grid line crossings */
                gui_sketch_overlay_private_draw_grid( this_, card_under_mouse, cr );

                /* draw snap-to-grid indicator */
                const gui_sketch_snap_state_t snapped = gui_sketch_card_is_pos_on_grid( card_under_mouse, to_x, to_y );
                gui_sketch_overlay_private_draw_snap_indicator( this_, card_under_mouse, snapped, to_x, to_y, cr );
            }

            /* draw boxes and arrow icons */
            if ( draw_new_classifier )
            {
                gui_sketch_overlay_private_draw_new_classifier( this_, to_x, to_y, cr );
            }
            if ( draw_new_feature )
            {
                gui_sketch_overlay_private_draw_new_feature( this_, to_x, to_y, cr );
            }
            if ( draw_new_relationship )
            {
                gui_sketch_overlay_private_draw_new_relationship( this_, to_x, to_y, cr );
            }
        }
    }

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_arrow( gui_sketch_overlay_t *this_,
                                            int32_t from_x,
                                            int32_t from_y,
                                            int32_t to_x,
                                            int32_t to_y,
                                            cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );
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

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_new_classifier( gui_sketch_overlay_t *this_,
                                                     int32_t x,
                                                     int32_t y,
                                                     cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );
    static const int32_t ICON_UNIT = 2;
    static const int32_t DIST = 12;

    /* star */
    const int32_t star_center_x = x+(1)*ICON_UNIT;
    const int32_t star_center_y = y-DIST-6*ICON_UNIT;
    cairo_move_to ( cr, star_center_x, star_center_y-2*ICON_UNIT );
    cairo_line_to ( cr, star_center_x, star_center_y+2*ICON_UNIT );
    cairo_move_to ( cr, star_center_x-2*ICON_UNIT, star_center_y-1*ICON_UNIT );
    cairo_line_to ( cr, star_center_x+2*ICON_UNIT, star_center_y+1*ICON_UNIT );
    cairo_move_to ( cr, star_center_x-2*ICON_UNIT, star_center_y+1*ICON_UNIT );
    cairo_line_to ( cr, star_center_x+2*ICON_UNIT, star_center_y-1*ICON_UNIT );

    /* big box */
    cairo_move_to ( cr, x-1*ICON_UNIT, y-DIST-6*ICON_UNIT );
    cairo_line_to ( cr, x-8*ICON_UNIT, y-DIST-6*ICON_UNIT );
    cairo_line_to ( cr, x-8*ICON_UNIT, y-DIST+3*ICON_UNIT );
    cairo_line_to ( cr, x+1*ICON_UNIT, y-DIST+3*ICON_UNIT );
    cairo_line_to ( cr, x+1*ICON_UNIT, y-DIST-2*ICON_UNIT );

    cairo_stroke (cr);

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_new_feature( gui_sketch_overlay_t *this_,
                                                  int32_t x,
                                                  int32_t y,
                                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );
    static const int32_t ICON_UNIT = 2;
    static const int32_t DIST = 12;

    /* star */
    const int32_t star_center_x = x+(-2)*ICON_UNIT;
    const int32_t star_center_y = y-DIST-6*ICON_UNIT;
    cairo_move_to ( cr, star_center_x, star_center_y-2*ICON_UNIT );
    cairo_line_to ( cr, star_center_x, star_center_y+2*ICON_UNIT );
    cairo_move_to ( cr, star_center_x-2*ICON_UNIT, star_center_y-1*ICON_UNIT );
    cairo_line_to ( cr, star_center_x+2*ICON_UNIT, star_center_y+1*ICON_UNIT );
    cairo_move_to ( cr, star_center_x-2*ICON_UNIT, star_center_y+1*ICON_UNIT );
    cairo_line_to ( cr, star_center_x+2*ICON_UNIT, star_center_y-1*ICON_UNIT );

    /* small box */
    cairo_move_to ( cr, x-4*ICON_UNIT, y-DIST-6*ICON_UNIT );
    cairo_line_to ( cr, x-8*ICON_UNIT, y-DIST-6*ICON_UNIT );
    cairo_line_to ( cr, x-8*ICON_UNIT, y-DIST-2*ICON_UNIT );
    cairo_line_to ( cr, x-2*ICON_UNIT, y-DIST-2*ICON_UNIT );

    cairo_stroke (cr);

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_new_relationship( gui_sketch_overlay_t *this_,
                                                       int32_t x,
                                                       int32_t y,
                                                       cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );
    static const int32_t ICON_UNIT = 2;
    static const int32_t DIST = 12;

    /* arrow */
    cairo_move_to ( cr, x+2*ICON_UNIT, y-DIST-0*ICON_UNIT );
    cairo_line_to ( cr, x+8*ICON_UNIT, y-DIST-6*ICON_UNIT );
    cairo_line_to ( cr, x+5*ICON_UNIT, y-DIST-5*ICON_UNIT );
    cairo_move_to ( cr, x+8*ICON_UNIT, y-DIST-6*ICON_UNIT );
    cairo_line_to ( cr, x+7*ICON_UNIT, y-DIST-3*ICON_UNIT );

    cairo_stroke (cr);

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_grid( gui_sketch_overlay_t *this_,
                                           const gui_sketch_card_t *card_under_mouse,
                                           cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != card_under_mouse );
    assert( NULL != cr );

    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );

    /* draw grid */
    shape_int_rectangle_t bounds;
    uint32_t x_count;
    uint32_t y_count;
    gui_sketch_card_get_grid_area( card_under_mouse, &bounds, &x_count, &y_count );
    assert( x_count >= 2 );
    assert( y_count >= 2 );
    const int32_t left = shape_int_rectangle_get_left(&bounds);
    const int32_t top = shape_int_rectangle_get_top(&bounds);
    const int32_t width = shape_int_rectangle_get_width(&bounds);
    const int32_t height = shape_int_rectangle_get_height(&bounds);
    for ( uint32_t x_idx = 1; (x_idx+1) < x_count; x_idx ++ )
    {
        for ( uint32_t y_idx = 1; (y_idx+1) < y_count; y_idx ++ )
        {
            const int32_t x = left + (( width * x_idx )/( x_count-1 ));
            const int32_t y = top + (( height * y_idx )/( y_count-1 ));
            static const int32_t HALF_LINE = 16;
            cairo_rectangle ( cr, x, y-HALF_LINE, 1, 2*HALF_LINE );
            cairo_fill (cr);
            cairo_rectangle ( cr, x-HALF_LINE, y, 2*HALF_LINE, 1 );
            cairo_fill (cr);
        }
    }

    U8_TRACE_END();
}

void gui_sketch_overlay_private_draw_snap_indicator( gui_sketch_overlay_t *this_,
                                                     const gui_sketch_card_t *card_under_mouse,
                                                     gui_sketch_snap_state_t snapped,
                                                     int32_t x,
                                                     int32_t y,
                                                     cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != card_under_mouse );
    assert( NULL != cr );

    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );

    /* draw marker that position snapped to grid */
    const shape_int_rectangle_t bounds = gui_sketch_card_get_bounds( card_under_mouse );
    const int32_t left = shape_int_rectangle_get_left(&bounds);
    const int32_t top = shape_int_rectangle_get_top(&bounds);
    const int32_t width = shape_int_rectangle_get_width(&bounds);
    const int32_t height = shape_int_rectangle_get_height(&bounds);
    if ( ( snapped & GUI_SKETCH_SNAP_STATE_X ) == GUI_SKETCH_SNAP_STATE_X )
    {
        cairo_rectangle ( cr, x-2, top, 1, height );
        cairo_fill (cr);
        cairo_rectangle ( cr, x+2, top, 1, height );
        cairo_fill (cr);
    }
    if ( ( snapped & GUI_SKETCH_SNAP_STATE_Y ) == GUI_SKETCH_SNAP_STATE_Y )
    {
        cairo_rectangle ( cr, left, y-2, width, 1 );
        cairo_fill (cr);
        cairo_rectangle ( cr, left, y+2, width, 1 );
        cairo_fill (cr);
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2024 Andreas Warnke

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
