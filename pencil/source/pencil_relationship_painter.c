/* File: pencil_relationship_painter.c; Copyright and License: see below */

#include "pencil_relationship_painter.h"
#include "pencil_layout_data.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_relationship_painter_init( pencil_relationship_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    draw_relationship_label_init( &((*this_).draw_relationship_label) );

    U8_TRACE_END();
}

void pencil_relationship_painter_destroy( pencil_relationship_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_relationship_label_destroy( &((*this_).draw_relationship_label) );
    pencil_marker_destroy( &((*this_).marker) );

    U8_TRACE_END();
}

void pencil_relationship_painter_draw ( pencil_relationship_painter_t *this_,
                                        const layout_relationship_t *layouted_relationship,
                                        bool mark_focused,
                                        bool mark_highlighted,
                                        bool mark_selected,
                                        const pencil_size_t *pencil_size,
                                        PangoLayout *layout,
                                        cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_relationship );
    assert( NULL != layout );
    assert( NULL != cr );

    const data_relationship_t *const the_relationship
        = layout_relationship_get_data_const( layouted_relationship );
    const geometry_connector_t *const connector_shape
        = layout_relationship_get_shape_const( layouted_relationship );

    if ( data_relationship_is_valid( the_relationship ) )
    {
        U8_TRACE_INFO_INT("drawing relationship id", data_relationship_get_row_id( the_relationship ) );

        /* set the right line width */
        const double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        /* set the right drawing color */
        {
            GdkRGBA foreground_color;
            if ( mark_highlighted )
            {
                foreground_color = pencil_size_get_highlight_color( pencil_size );
            }
            else if (( PENCIL_VISIBILITY_GRAY_OUT == layout_relationship_get_visibility( layouted_relationship ) )
                || ( PENCIL_VISIBILITY_IMPLICIT == layout_relationship_get_visibility( layouted_relationship ) ))
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }

            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* get points */
        double p1x = geometry_connector_get_source_end_x ( connector_shape );
        double p1y = geometry_connector_get_source_end_y ( connector_shape );
        double p2x = geometry_connector_get_main_line_source_x ( connector_shape );
        double p2y = geometry_connector_get_main_line_source_y ( connector_shape );
        double p3x = geometry_connector_get_main_line_destination_x ( connector_shape );
        double p3y = geometry_connector_get_main_line_destination_y ( connector_shape );
        double p4x = geometry_connector_get_destination_end_x ( connector_shape );
        double p4y = geometry_connector_get_destination_end_y ( connector_shape );
        double center_x = (p2x+p3x)/2.0;
        double center_y = (p2y+p3y)/2.0;

        /* determine arrow direction */
        int arrow_clock_direction; /* wall-clock direction assuming ascending y direction to top */
        if ( fabs( p3x-p4x ) > 0.5 )
        {
            arrow_clock_direction = ( p3x < p4x ) ? 3 : 9;
        }
        else
        {
            if ( fabs( p3y-p4y ) > 0.5 )
            {
                arrow_clock_direction = ( p3y < p4y ) ? 0 : 6;
            }
            else
            {
                /* maybe the first segment is too short, take into account two segments */
                if ( fabs( p2x-p4x ) > 0.5 )
                {
                    arrow_clock_direction = ( p2x < p4x ) ? 3 : 9;
                }
                else
                {
                    if ( fabs( p2y-p4y ) > 0.5 )
                    {
                        arrow_clock_direction = ( p2y < p4y ) ? 0 : 6;
                    }
                    else
                    {
                        /* maybe the first two segments are too short, take into account all segments */
                        if ( fabs( p1x-p4x ) > 0.5 )
                        {
                            arrow_clock_direction = ( p1x < p4x ) ? 3 : 9;
                        }
                        else
                        {
                            if ( fabs( p1y-p4y ) > 0.5 )
                            {
                                arrow_clock_direction = ( p1y < p4y ) ? 0 : 6;
                            }
                            else
                            {
                                /* connector has no length */
                                arrow_clock_direction = 0;
                            }
                        }
                    }
                }
            }
        }

        /* determine feathers direction */
        int feathers_clock_direction; /* wall-clock direction assuming ascending y direction to top */
        if ( fabs( p2x-p1x ) > 0.5 )
        {
            feathers_clock_direction = ( p2x < p1x ) ? 9 : 3;
        }
        else
        {
            if ( fabs( p2y-p1y ) > 0.5 )
            {
                feathers_clock_direction = ( p2y < p1y ) ? 0 : 6;
            }
            else
            {
                /* maybe the first segment is too short, take into account two segments */
                if ( fabs( p3x-p1x ) > 0.5 )
                {
                    feathers_clock_direction = ( p3x < p1x ) ? 9 : 3;
                }
                else
                {
                    if ( fabs( p3y-p1y ) > 0.5 )
                    {
                        feathers_clock_direction = ( p3y < p1y ) ? 0 : 6;
                    }
                    else
                    {
                        /* maybe the first two segments are too short, take into account all segments */
                        if ( fabs( p4x-p1x ) > 0.5 )
                        {
                            feathers_clock_direction = ( p4x < p1x ) ? 9 : 3;
                        }
                        else
                        {
                            if ( fabs( p4y-p1y ) > 0.5 )
                            {
                                feathers_clock_direction = ( p4y < p1y ) ? 0 : 6;
                            }
                            else
                            {
                                /* connector has no length */
                                feathers_clock_direction = 0;
                            }
                        }
                    }
                }
            }
        }

        /* draw arrow */
        switch ( data_relationship_get_main_type( the_relationship ) )
        {
            case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
            case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
            case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
            case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
            case DATA_RELATIONSHIP_TYPE_UML_EXTEND:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
            case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
            case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
            case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_REFINE:
            case DATA_RELATIONSHIP_TYPE_UML_TRACE:
            {
                const double half_stroke_length = 0.5 * pencil_size_get_arrow_stroke_length( pencil_size );
                const double part_stroke_length = pencil_size_get_arrow_stroke_087_length( pencil_size );
                bool close_path;
                if (( data_relationship_get_main_type( the_relationship ) == DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION )
                    || ( data_relationship_get_main_type( the_relationship ) == DATA_RELATIONSHIP_TYPE_UML_REALIZATION ))
                {
                    close_path = true;
                }
                else
                {
                    close_path = false;
                }
                switch ( arrow_clock_direction )
                {
                    case 0:  /* direction: 12 o clock */
                    {
                        cairo_move_to ( cr, p4x - half_stroke_length, p4y - part_stroke_length );
                        cairo_line_to ( cr, p4x, p4y );
                        cairo_line_to ( cr, p4x + half_stroke_length, p4y - part_stroke_length );
                        if ( close_path )
                        {
                            cairo_line_to ( cr, p4x - half_stroke_length, p4y - part_stroke_length );
                            p4y = p4y - part_stroke_length;
                            /* if p3 == p4, also move p3 */
                            if ( p3y > p4y )
                            {
                                p3y = p4y;
                            }
                        }
                    }
                    break;

                    case 3:
                    {
                        cairo_move_to ( cr, p4x - part_stroke_length, p4y - half_stroke_length );
                        cairo_line_to ( cr, p4x, p4y );
                        cairo_line_to ( cr, p4x - part_stroke_length, p4y + half_stroke_length );
                        if ( close_path )
                        {
                            cairo_line_to ( cr, p4x - part_stroke_length, p4y - half_stroke_length );
                            p4x = p4x - part_stroke_length;
                            /* if p3 == p4, also move p3 */
                            if ( p3x > p4x )
                            {
                                p3x = p4x;
                            }
                        }
                    }
                    break;

                    case 6:
                    {
                        cairo_move_to ( cr, p4x + half_stroke_length, p4y + part_stroke_length );
                        cairo_line_to ( cr, p4x, p4y );
                        cairo_line_to ( cr, p4x - half_stroke_length, p4y + part_stroke_length );
                        if ( close_path )
                        {
                            cairo_line_to ( cr, p4x + half_stroke_length, p4y + part_stroke_length );
                            p4y = p4y + part_stroke_length;
                            /* if p3 == p4, also move p3 */
                            if ( p3y < p4y )
                            {
                                p3y = p4y;
                            }
                        }
                    }
                    break;

                    case 9:
                    {
                        cairo_move_to ( cr, p4x + part_stroke_length, p4y + half_stroke_length );
                        cairo_line_to ( cr, p4x, p4y );
                        cairo_line_to ( cr, p4x + part_stroke_length, p4y - half_stroke_length );
                        if ( close_path )
                        {
                            cairo_line_to ( cr, p4x + part_stroke_length, p4y + half_stroke_length );
                            p4x = p4x + part_stroke_length;
                            /* if p3 == p4, also move p3 */
                            if ( p3x < p4x )
                            {
                                p3x = p4x;
                            }
                        }
                    }
                    break;

                    default:
                    {
                        U8_LOG_ERROR("illegal case");
                    }
                    break;
                }
                if (( data_relationship_get_main_type( the_relationship ) == DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL )
                    || ( data_relationship_get_main_type( the_relationship ) == DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL ))
                {
                    cairo_fill (cr);
                }
                else
                {
                    cairo_stroke (cr);
                }
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
            case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
            case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
            case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
            {
                /* no arrow tip */
            }
            break;

            default:
            {
                U8_LOG_ERROR("unknown data_relationship_type_t in pencil_relationship_painter_draw()");
                /* no arrow tip */
            }
            break;
        }

        /* draw feathers */
        switch ( data_relationship_get_main_type( the_relationship ) )
        {
            case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
            case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
            {
                /* rhomboid */
                const double half_stroke_length = 0.5 * pencil_size_get_arrow_stroke_length( pencil_size );
                const double part_stroke_length = pencil_size_get_arrow_stroke_087_length( pencil_size );
                switch ( feathers_clock_direction )
                {
                    case 0:  /* direction: 12 o clock */
                    {
                        cairo_move_to ( cr, p1x, p1y );
                        cairo_line_to ( cr, p1x - half_stroke_length, p1y - part_stroke_length );
                        cairo_line_to ( cr, p1x, p1y - 2.0 * part_stroke_length );
                        cairo_line_to ( cr, p1x + half_stroke_length, p1y - part_stroke_length );
                        cairo_line_to ( cr, p1x, p1y );
                        p1y = p1y - 2.0 * part_stroke_length;
                        /* if p2 == p1, also move p2 */
                        if ( p2y > p1y )
                        {
                            p2y = p1y;
                        }
                    }
                    break;

                    case 3:
                    {
                        cairo_move_to ( cr, p1x, p1y );
                        cairo_line_to ( cr, p1x + part_stroke_length, p1y - half_stroke_length );
                        cairo_line_to ( cr, p1x + 2.0 * part_stroke_length, p1y );
                        cairo_line_to ( cr, p1x + part_stroke_length, p1y + half_stroke_length );
                        cairo_line_to ( cr, p1x, p1y );
                        p1x = p1x + 2.0 * part_stroke_length;
                        /* if p2 == p1, also move p2 */
                        if ( p2x < p1x )
                        {
                            p2x = p1x;
                        }
                    }
                    break;

                    case 6:
                    {
                        cairo_move_to ( cr, p1x, p1y );
                        cairo_line_to ( cr, p1x + half_stroke_length, p1y + part_stroke_length );
                        cairo_line_to ( cr, p1x, p1y + 2.0 * part_stroke_length );
                        cairo_line_to ( cr, p1x - half_stroke_length, p1y + part_stroke_length );
                        cairo_line_to ( cr, p1x, p1y );
                        p1y = p1y + 2.0 * part_stroke_length;
                        /* if p2 == p1, also move p2 */
                        if ( p2y < p1y )
                        {
                            p2y = p1y;
                        }
                    }
                    break;

                    case 9:
                    {
                        cairo_move_to ( cr, p1x, p1y );
                        cairo_line_to ( cr, p1x - part_stroke_length, p1y + half_stroke_length );
                        cairo_line_to ( cr, p1x - 2.0 * part_stroke_length, p1y );
                        cairo_line_to ( cr, p1x - part_stroke_length, p1y - half_stroke_length );
                        cairo_line_to ( cr, p1x, p1y );
                        p1x = p1x - 2.0 * part_stroke_length;
                        /* if p2 == p1, also move p2 */
                        if ( p2x > p1x )
                        {
                            p2x = p1x;
                        }
                    }
                    break;

                    default:
                    {
                        U8_LOG_ERROR("illegal case");
                    }
                    break;
                }
                if ( data_relationship_get_main_type( the_relationship ) == DATA_RELATIONSHIP_TYPE_UML_COMPOSITION )
                {
                    cairo_fill (cr);
                }
                else
                {
                    cairo_stroke (cr);
                }
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
            {
                /* circle with plus */
                /* const double stroke_length = pencil_size_get_arrow_stroke_length( pencil_size ); */
                const double half_stroke_length = 0.5 * pencil_size_get_arrow_stroke_length( pencil_size );
                switch ( feathers_clock_direction )
                {
                    case 0:  /* direction: 12 o clock */
                    {
                        cairo_new_sub_path (cr);  /* to be called before arc */
                        cairo_arc ( cr, p1x, p1y - half_stroke_length, half_stroke_length, 0.0, 2.0*M_PI );
                        cairo_move_to ( cr, p1x - half_stroke_length, p1y - half_stroke_length );
                        cairo_line_to ( cr, p1x + half_stroke_length, p1y - half_stroke_length );
                        cairo_stroke (cr);
                    }
                    break;

                    case 3:
                    {
                        cairo_new_sub_path (cr);
                        cairo_arc ( cr, p1x + half_stroke_length, p1y, half_stroke_length, 0.0, 2.0*M_PI );
                        cairo_move_to ( cr, p1x + half_stroke_length, p1y - half_stroke_length );
                        cairo_line_to ( cr, p1x + half_stroke_length, p1y + half_stroke_length );
                        cairo_stroke (cr);
                    }
                    break;

                    case 6:
                    {
                        cairo_new_sub_path (cr);
                        cairo_arc ( cr, p1x, p1y + half_stroke_length, half_stroke_length, 0.0, 2.0*M_PI );
                        cairo_move_to ( cr, p1x - half_stroke_length, p1y + half_stroke_length );
                        cairo_line_to ( cr, p1x + half_stroke_length, p1y + half_stroke_length );
                        cairo_stroke (cr);
                    }
                    break;

                    case 9:
                    {
                        cairo_new_sub_path (cr);
                        cairo_arc ( cr, p1x - half_stroke_length, p1y, half_stroke_length, 0.0, 2.0*M_PI );
                        cairo_move_to ( cr, p1x - half_stroke_length, p1y - half_stroke_length );
                        cairo_line_to ( cr, p1x - half_stroke_length, p1y + half_stroke_length );
                        cairo_stroke (cr);
                    }
                    break;

                    default:
                    {
                        U8_LOG_ERROR("illegal case");
                    }
                    break;
                }
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
            case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
            case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
            case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
            case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
            case DATA_RELATIONSHIP_TYPE_UML_EXTEND:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
            case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
            case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
            case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_REFINE:
            case DATA_RELATIONSHIP_TYPE_UML_TRACE:
            {
                /* no rhomboid or other feathers */
            }
            break;

            default:
            {
                U8_LOG_ERROR("unknown data_relationship_type_t in pencil_relationship_painter_draw()");
                /* no rhomboid or other feathers */
            }
            break;
        }

        /* set the right line type: dashed/normal */
        switch ( data_relationship_get_main_type( the_relationship ) )
        {
            case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
            case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
            case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
            case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
            case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
            case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
            case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
            case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
            {
                /* no dashes */
                cairo_set_dash ( cr, NULL, 0, 0.0 );
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_EXTEND:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
            case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:  /* t.b.d. */
            case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
            case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
            case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
            case DATA_RELATIONSHIP_TYPE_UML_REFINE:
            case DATA_RELATIONSHIP_TYPE_UML_TRACE:
            {
                double dashes[1];
                dashes[0] = pencil_size_get_line_dash_length( pencil_size );
                cairo_set_dash ( cr, dashes, 1, 0.0 );
            }
            break;

            default:
            {
                U8_LOG_ERROR("unknown data_relationship_type_t in pencil_relationship_painter_draw()");
                double error_dashes[1];
                error_dashes[0] = 0.2*pencil_size_get_line_dash_length( pencil_size );
                cairo_set_dash ( cr, error_dashes, 1, 0.0 );
            }
            break;
        }

        /* draw connector line */
        {
            const double radius = 2.0 * pencil_size_get_arrow_stroke_length( pencil_size );
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

        /* reset dashes */
        cairo_set_dash ( cr, NULL, 0, 0.0 );

        /* draw the label */
        draw_relationship_label_draw_type_and_name ( &((*this_).draw_relationship_label),
                                                     the_relationship,
                                                     layout_relationship_get_label_box_const( layouted_relationship ),
                                                     pencil_size,
                                                     layout,
                                                     cr
                                                   );

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
        /* draw the rectangles */
        {
            const geometry_rectangle_t bounds
                = geometry_connector_get_bounding_rectangle( connector_shape );
            const geometry_rectangle_t *const relation_label_box
                = layout_relationship_get_label_box_const( layouted_relationship );

            cairo_set_source_rgba( cr, 0.5, 1.0, 0.6, 0.5 );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( &bounds ),
                              geometry_rectangle_get_top ( &bounds ),
                              geometry_rectangle_get_width ( &bounds ),
                              geometry_rectangle_get_height ( &bounds )
                            );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( relation_label_box ),
                              geometry_rectangle_get_top ( relation_label_box ),
                              geometry_rectangle_get_width ( relation_label_box ),
                              geometry_rectangle_get_height ( relation_label_box )
                            );
            cairo_stroke (cr);
        }
#endif

        /* draw markers */
        if ( mark_selected )
        {
            const geometry_rectangle_t bounds
                = geometry_connector_get_bounding_rectangle( connector_shape );
            pencil_marker_mark_selected_rectangle( &((*this_).marker), bounds, cr );
        }

        if ( mark_focused )
        {
            const geometry_rectangle_t bounds
                = geometry_connector_get_bounding_rectangle( connector_shape );
            pencil_marker_mark_focused_rectangle( &((*this_).marker), bounds, cr );
        }
    }
    else
    {
        U8_LOG_ERROR("invalid relationship in array!");
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2022 Andreas Warnke

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
