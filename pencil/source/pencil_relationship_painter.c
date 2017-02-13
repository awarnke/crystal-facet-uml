/* File: pencil_relationship_painter.c; Copyright and License: see below */

#include "pencil_relationship_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_relationship_painter_init( pencil_relationship_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_relationship_painter_destroy( pencil_relationship_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_relationship_painter_draw ( pencil_relationship_painter_t *this_,
                                        data_relationship_t *the_relationship,
                                        bool mark_focused,
                                        bool mark_highlighted,
                                        bool mark_selected,
                                        pencil_size_t *pencil_size,
                                        geometry_connector_t *connector_shape,
                                        PangoLayout *layout,
                                        cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_relationship );
    assert( NULL != connector_shape );
    assert( NULL != layout );
    assert( NULL != cr );

    if ( data_relationship_is_valid( the_relationship ) )
    {
        TRACE_INFO_INT("drawing relationship id", data_relationship_get_id( the_relationship ) );

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        /* draw connector */
        GdkRGBA foreground_color;
        if ( mark_highlighted )
        {
            foreground_color = pencil_size_get_highlight_color( pencil_size );
        }
        else
        {
            foreground_color = pencil_size_get_standard_color( pencil_size );
        }
        cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        double p1x = geometry_connector_get_source_end_x ( connector_shape );
        double p1y = geometry_connector_get_source_end_y ( connector_shape );
        double p2x = geometry_connector_get_main_line_source_x ( connector_shape );
        double p2y = geometry_connector_get_main_line_source_y ( connector_shape );
        double p3x = geometry_connector_get_main_line_destination_x ( connector_shape );
        double p3y = geometry_connector_get_main_line_destination_y ( connector_shape );
        double p4x = geometry_connector_destination_end_x ( connector_shape );
        double p4y = geometry_connector_get_destination_end_y ( connector_shape );
        double center_x = (p2x+p3x)/2.0;
        double center_y = (p2y+p3y)/2.0;

        /* simple cornered line */
        /*
        cairo_move_to ( cr, p1x, p1y );
        cairo_line_to ( cr, p2x, p2y );
        cairo_line_to ( cr, p3x, p3y );
        cairo_line_to ( cr, p4x, p4y );
        cairo_stroke (cr);
        */

        /* alternative with fix radius */
        {
            double radius = 2.0 * pencil_size_get_arrow_stroke_length( pencil_size );
            double dx;
            double dy;
            cairo_move_to ( cr, p1x, p1y );
            dx = p2x - p1x;
            dy = p2y - p1y;
            if ( dx > +radius ) { dx = +radius; }
            if ( dx < -radius ) { dx = -radius; }
            if ( dy > +radius ) { dy = +radius; }
            if ( dy < -radius ) { dy = -radius; }
            cairo_line_to ( cr, p2x - dx, p2y - dy );
            dx = center_x - p2x;
            dy = center_y - p2y;
            if ( dx > +radius ) { dx = +radius; }
            if ( dx < -radius ) { dx = -radius; }
            if ( dy > +radius ) { dy = +radius; }
            if ( dy < -radius ) { dy = -radius; }
            cairo_curve_to ( cr, p2x, p2y, p2x, p2y, p2x + dx, p2y + dy );
            dx = p3x - center_x;
            dy = p3y - center_y;
            if ( dx > +radius ) { dx = +radius; }
            if ( dx < -radius ) { dx = -radius; }
            if ( dy > +radius ) { dy = +radius; }
            if ( dy < -radius ) { dy = -radius; }
            cairo_line_to ( cr, p3x - dx, p3y - dy );
            dx = p4x - p3x;
            dy = p4y - p3y;
            if ( dx > +radius ) { dx = +radius; }
            if ( dx < -radius ) { dx = -radius; }
            if ( dy > +radius ) { dy = +radius; }
            if ( dy < -radius ) { dy = -radius; }
            cairo_curve_to ( cr, p3x, p3y, p3x, p3y, p3x + dx, p3y + dy );
            cairo_line_to ( cr, p4x, p4y );
            cairo_stroke (cr);
        }

        /* draw arrow */
        int clock_direction; /* wall-clock direction assuming ascending y direction to top */
        if ( fabs( p3x-p4x ) > 0.5 )
        {
            clock_direction = ( p3x < p4x ) ? 3 : 9;
        }
        else
        {
            if ( fabs( p3y-p4y ) > 0.5 )
            {
                clock_direction = ( p3y < p4y ) ? 0 : 6;
            }
            else
            {
                if ( fabs( p2x-p4x ) > 0.5 )
                {
                    clock_direction = ( p2x < p4x ) ? 3 : 9;
                }
                else
                {
                    if ( fabs( p2y-p4y ) > 0.5 )
                    {
                        clock_direction = ( p2y < p4y ) ? 0 : 6;
                    }
                    else
                    {
                    }
                }
            }
        }

        double half_stroke_length = 0.5 * pencil_size_get_arrow_stroke_length( pencil_size );
        double part_stroke_length = pencil_size_get_arrow_stroke_087_length( pencil_size );
        switch ( clock_direction )
        {
            case 0:  /* direction: 12 o clock */
            {
                cairo_move_to ( cr, p4x - half_stroke_length, p4y - part_stroke_length );
                cairo_line_to ( cr, p4x, p4y );
                cairo_line_to ( cr, p4x + half_stroke_length, p4y - part_stroke_length );
                cairo_stroke (cr);
            }
            break;

            case 3:
            {
                cairo_move_to ( cr, p4x - part_stroke_length, p4y - half_stroke_length );
                cairo_line_to ( cr, p4x, p4y );
                cairo_line_to ( cr, p4x - part_stroke_length, p4y + half_stroke_length );
                cairo_stroke (cr);
            }
            break;

            case 6:
            {
                cairo_move_to ( cr, p4x + half_stroke_length, p4y + part_stroke_length );
                cairo_line_to ( cr, p4x, p4y );
                cairo_line_to ( cr, p4x - half_stroke_length, p4y + part_stroke_length );
                cairo_stroke (cr);
            }
            break;

            case 9:
            {
                cairo_move_to ( cr, p4x + part_stroke_length, p4y + half_stroke_length );
                cairo_line_to ( cr, p4x, p4y );
                cairo_line_to ( cr, p4x + part_stroke_length, p4y - half_stroke_length );
                cairo_stroke (cr);
            }
            break;

            default:
            {
                TSLOG_ERROR("illegal case");
            }
            break;
        }

        /* draw name text */
        {
            int text2_width;
            int text2_height;
            pango_layout_set_font_description (layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (layout, data_relationship_get_name_ptr( the_relationship ), -1);
            pango_layout_get_pixel_size (layout, &text2_width, &text2_height);

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, center_x - 0.5*text2_width, center_y - text2_height );
            pango_cairo_show_layout (cr, layout);
        }

        /* draw markers */
        if ( mark_selected )
        {
            geometry_rectangle_t bounds;
            bounds = geometry_connector_get_bounding_rectangle( connector_shape );
            pencil_marker_mark_selected_rectangle( &((*this_).marker), bounds, cr );
            geometry_rectangle_destroy( &bounds );
        }

        if ( mark_focused )
        {
            geometry_rectangle_t bounds;
            bounds = geometry_connector_get_bounding_rectangle( connector_shape );
            pencil_marker_mark_focused_rectangle( &((*this_).marker), bounds, cr );
            geometry_rectangle_destroy( &bounds );
        }
    }
    else
    {
        TSLOG_ERROR("invalid relationship in array!");
    }

    TRACE_END();
}


/*
Copyright 2017-2017 Andreas Warnke

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
