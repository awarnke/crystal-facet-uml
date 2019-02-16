/* File: pencil_classifier_painter.c; Copyright and License: see below */

#include "pencil_classifier_painter.h"
#include "trace.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    data_rules_init ( &((*this_).data_rules) );
    draw_symbol_init( &((*this_).draw_symbol) );
    draw_label_init( &((*this_).draw_label) );
    draw_geometry_init( &((*this_).draw_geometry) );

    TRACE_END();
}

void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    draw_symbol_destroy( &((*this_).draw_symbol) );
    draw_label_destroy( &((*this_).draw_label) );
    draw_geometry_destroy( &((*this_).draw_geometry) );
    data_rules_destroy ( &((*this_).data_rules) );
    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_painter_draw ( const pencil_classifier_painter_t *this_,
                                      layout_visible_classifier_t *layouted_classifier,
                                      data_id_t mark_focused,
                                      data_id_t mark_highlighted,
                                      const data_small_set_t *mark_selected,
                                      const pencil_size_t *pencil_size,
                                      PangoLayout *font_layout,
                                      cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_classifier );
    assert( NULL != font_layout );
    assert( NULL != cr );

    const data_visible_classifier_t *visible_classifier;
    visible_classifier = layout_visible_classifier_get_data_ptr( layouted_classifier );
    const geometry_rectangle_t *classifier_bounds;
    classifier_bounds = layout_visible_classifier_get_bounds_ptr( layouted_classifier );
    const data_classifier_t *classifier;
    const data_diagramelement_t *diagramelement;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );

    double left, top;
    double width, height;

    left = geometry_rectangle_get_left ( classifier_bounds );
    top = geometry_rectangle_get_top ( classifier_bounds );
    width = geometry_rectangle_get_width ( classifier_bounds );
    height = geometry_rectangle_get_height ( classifier_bounds );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* draw id */
    {
        /* prepare text */
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        data_id_t the_id;
        data_id_init( &the_id, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );
        data_id_to_utf8stringbuf( &the_id, id_str );

        /* determine text dimension */
        int text4_width;
        int text4_height;
        pango_layout_set_font_description (font_layout, pencil_size_get_footnote_font_description(pencil_size) );
        pango_layout_set_text (font_layout, utf8stringbuf_get_string( id_str ), -1);
        pango_layout_get_pixel_size (font_layout, &text4_width, &text4_height);

        /* position the text */
        int x_gap = 0;
        int y_gap = 0;
        switch ( classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_BLOCK:
            case DATA_CLASSIFIER_TYPE_FEATURE:
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:
            case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
            case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
            case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
            case DATA_CLASSIFIER_TYPE_UML_PART:
            case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
            case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
            case DATA_CLASSIFIER_TYPE_UML_CLASS:
            case DATA_CLASSIFIER_TYPE_UML_OBJECT:
            case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
            case DATA_CLASSIFIER_TYPE_UML_COMMENT:
            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:
            {
                /* there is a border line */
                x_gap = 2*gap;
                y_gap = 2*gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_NODE:
            {
                /* there is a 3D border line */
                x_gap = 4*gap;
                y_gap = 2*gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
            case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_UML_STATE:
            case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
            {
                /* there is a border line with a round corner */
                x_gap = 7*gap;
                y_gap = 2*gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTOR:
            case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
            case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
            {
                /* position the text right-bottom of the icon */
                x_gap = (width - pencil_size_get_classifier_symbol_height( pencil_size ))/2 - text4_width;
                y_gap = 0;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
            {
                /* there is a chance that the bottom-right corner is empty */
                x_gap = 0;
                y_gap = 0;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
            {
                /* the send signal has a 45 degree edge */
                x_gap = height/2;
                y_gap = 2*gap;
            }
            break;

            default:
            {
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_painter_draw()");
            }
            break;
        }

        /* draw text */
        GdkRGBA grey_color;
        grey_color = pencil_size_get_gray_out_color( pencil_size );
        cairo_set_source_rgba( cr, grey_color.red, grey_color.green, grey_color.blue, grey_color.alpha );
        cairo_move_to ( cr, left + width - text4_width - x_gap, top + height - text4_height - y_gap );
        pango_cairo_show_layout (cr, font_layout);
    }

    /* draw the classifier */
    {
        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );

        TRACE_INFO_INT("drawing classifier id", data_classifier_get_id( classifier ) );

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        /* set color */
        GdkRGBA foreground_color;
        {
            if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) ) )
            {
                foreground_color = pencil_size_get_highlight_color( pencil_size );
            }
            else if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GREY_OUT ))
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* prepare text to draw */
        int text1_top = top+2*gap;

        /* draw rectangle */
        int border_left = left + gap;
        int border_top = top + gap;
        int border_width = width - 2.0 * gap;
        int border_height = height - 2.0 * gap;
        switch ( classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_FEATURE:
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_UML_CLASS:
            case DATA_CLASSIFIER_TYPE_UML_OBJECT:
            case DATA_CLASSIFIER_TYPE_UML_PART:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* draw icon */
                double icon_height = pencil_size_get_title_font_size( pencil_size );
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_component_bounds( &((*this_).draw_symbol),
                                                                border_left + border_width - gap,  /* x */
                                                                border_top + gap,  /* y */
                                                                GEOMETRY_H_ALIGN_RIGHT,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                icon_height
                                                              );
                draw_symbol_draw_component ( &((*this_).draw_symbol),
                                             icon_bounds,
                                             cr
                                            );

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* draw icon */
                double icon_height = pencil_size_get_title_font_size( pencil_size );
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_artifact_bounds ( &((*this_).draw_symbol),
                                                                border_left + border_width - gap,  /* x */
                                                                border_top + gap,  /* y */
                                                                GEOMETRY_H_ALIGN_RIGHT,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                icon_height
                                                              );
                draw_symbol_draw_artifact ( &((*this_).draw_symbol),
                                            icon_bounds,
                                            cr
                                          );

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_UML_STATE:
            case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
            case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
            {
                double corner_radius = 6.0*gap;
                double bottom = border_top + border_height;
                double right = border_left + border_width;
                double ctrl_offset = corner_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                /* set dashes */
                if ( DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION == data_classifier_get_main_type ( classifier ) )
                {
                    double dashes[2];
                    dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
                    dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
                    cairo_set_dash ( cr, dashes, 2, 0.0 );
                }

                cairo_move_to ( cr, right - corner_radius, bottom );
                cairo_line_to ( cr, border_left + corner_radius, bottom );
                cairo_curve_to ( cr, border_left + ctrl_offset, bottom, border_left, bottom - ctrl_offset, border_left /* end point x */, bottom - corner_radius /* end point y */ );
                cairo_line_to ( cr, border_left, border_top + corner_radius );
                cairo_curve_to ( cr, border_left, border_top + ctrl_offset, border_left + ctrl_offset, border_top, border_left + corner_radius /* end point x */, border_top /* end point y */ );
                cairo_line_to ( cr, right - corner_radius, border_top );
                cairo_curve_to ( cr, right - ctrl_offset, border_top, right, border_top + ctrl_offset, right /* end point x */, border_top + corner_radius /* end point y */ );
                cairo_line_to ( cr, right, bottom - corner_radius );
                cairo_curve_to ( cr, right, bottom - ctrl_offset, right - ctrl_offset, bottom, right - corner_radius /* end point x */, bottom /* end point y */ );
                cairo_stroke (cr);

                /* reset dashes */
                cairo_set_dash ( cr, NULL, 0, 0.0 );

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
            {
                double bottom = border_top + border_height;
                double right = border_left + border_width;
                double half_width = 0.5 * border_width;
                double half_height = 0.5 * border_height;
                double center_x = border_left + half_width;
                double center_y = border_top + half_height;
                double ctrl_xoffset = half_width * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
                double ctrl_yoffset = half_height * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                cairo_move_to ( cr, center_x, bottom );
                cairo_curve_to ( cr, border_left + ctrl_xoffset, bottom, border_left, bottom - ctrl_yoffset, border_left /* end point x */, center_y /* end point y */ );
                cairo_curve_to ( cr, border_left, border_top + ctrl_yoffset, border_left + ctrl_xoffset, border_top, center_x /* end point x */, border_top /* end point y */ );
                cairo_curve_to ( cr, right - ctrl_xoffset, border_top, right, border_top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
                cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
                cairo_stroke (cr);

                /* adjust the text position */
                double some_offset = pencil_size_get_standard_font_size( pencil_size );
                text1_top = border_top + gap + some_offset;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_NODE:
            {
                double offset_3d = 2.0*gap;
                double bottom = border_top + border_height;
                double right = border_left + border_width;

                cairo_rectangle ( cr, border_left, border_top+offset_3d, border_width-offset_3d, border_height-offset_3d );
                cairo_stroke (cr);

                cairo_move_to ( cr, border_left, border_top+offset_3d );
                cairo_line_to ( cr, border_left+offset_3d, border_top );
                cairo_line_to ( cr, right, border_top );
                cairo_line_to ( cr, right, bottom-offset_3d );
                cairo_line_to ( cr, right-offset_3d, bottom );
                cairo_stroke (cr);
                cairo_move_to ( cr, right, border_top );
                cairo_line_to ( cr, right-offset_3d, border_top+offset_3d );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + offset_3d + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTOR:
            {
                double actor_height = pencil_size_get_classifier_symbol_height( pencil_size );
                double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_actor_bounds ( &((*this_).draw_symbol),
                                                             border_left + half_width,
                                                             border_top,
                                                             GEOMETRY_H_ALIGN_CENTER,
                                                             GEOMETRY_V_ALIGN_TOP,
                                                             actor_height
                                                           );
                draw_symbol_draw_actor ( &((*this_).draw_symbol),
                                         icon_bounds,
                                         cr
                                       );

                /* adjust the text position */
                text1_top = border_top + actor_height + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
            case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
            {
                //double circle_diameter = height;
                double circle_diameter = pencil_size_get_classifier_symbol_height( pencil_size );
                double circle_radius = 0.5 * circle_diameter;

                double circle_top = top;
                double circle_bottom = circle_top + circle_diameter;
                double half_width = 0.5 * width;
                double center_x = left + half_width;
                double center_y = top + circle_radius;
                double circle_left = center_x - circle_radius;
                double circle_right = center_x + circle_radius;

                if ( data_classifier_get_main_type ( classifier ) != DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE )
                {
                    double ctrl_offset = circle_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                    cairo_move_to ( cr, center_x, circle_bottom );
                    cairo_curve_to ( cr, circle_left + ctrl_offset, circle_bottom, circle_left, circle_bottom - ctrl_offset, circle_left /* end point x */, center_y /* end point y */ );
                    cairo_curve_to ( cr, circle_left, circle_top + ctrl_offset, circle_left + ctrl_offset, circle_top, center_x /* end point x */, circle_top /* end point y */ );
                    cairo_curve_to ( cr, circle_right - ctrl_offset, circle_top, circle_right, circle_top + ctrl_offset, circle_right /* end point x */, center_y /* end point y */ );
                    cairo_curve_to ( cr, circle_right, circle_bottom - ctrl_offset, circle_right - ctrl_offset, circle_bottom, center_x /* end point x */, circle_bottom /* end point y */ );
                    cairo_stroke (cr);
                }

                if ( ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE )
                    || ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE ) )
                {
                    double circle2_radius = circle_radius - gap;
                    double circle2_top = circle_top + gap;
                    double circle2_bottom = circle_bottom - gap;
                    double circle2_left = circle_left + gap;
                    double circle2_right = circle_right - gap;
                    double ctrl2_offset = circle2_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                    /* draw a smaller filled circle */
                    cairo_move_to ( cr, center_x, circle2_bottom );
                    cairo_curve_to ( cr, circle2_left + ctrl2_offset, circle2_bottom, circle2_left, circle2_bottom - ctrl2_offset, circle2_left /* end point x */, center_y /* end point y */ );
                    cairo_curve_to ( cr, circle2_left, circle2_top + ctrl2_offset, circle2_left + ctrl2_offset, circle2_top, center_x /* end point x */, circle2_top /* end point y */ );
                    cairo_curve_to ( cr, circle2_right - ctrl2_offset, circle2_top, circle2_right, circle2_top + ctrl2_offset, circle2_right /* end point x */, center_y /* end point y */ );
                    cairo_curve_to ( cr, circle2_right, circle2_bottom - ctrl2_offset, circle2_right - ctrl2_offset, circle2_bottom, center_x /* end point x */, circle2_bottom /* end point y */ );
                    cairo_fill (cr);
                }

                if ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY )
                {
                    double quarter_font = 0.15 * pencil_size_get_classifier_symbol_height( pencil_size );
                    cairo_move_to ( cr, center_x - quarter_font, center_y - 2.0 * quarter_font );
                    cairo_line_to ( cr, center_x - quarter_font, center_y + 2.0 * quarter_font );
                    cairo_move_to ( cr, center_x - quarter_font, center_y );
                    cairo_line_to ( cr, center_x + quarter_font, center_y );
                    cairo_move_to ( cr, center_x + quarter_font, center_y - 2.0 * quarter_font );
                    cairo_line_to ( cr, center_x + quarter_font, center_y + 2.0 * quarter_font );
                    cairo_stroke (cr);
                }
                else if ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY )
                {
                    double quarter_font = 0.15 * pencil_size_get_classifier_symbol_height( pencil_size );
                    cairo_move_to ( cr, center_x - 1.5 * quarter_font, center_y - 2.0 * quarter_font );
                    cairo_line_to ( cr, center_x - 1.5 * quarter_font, center_y + 2.0 * quarter_font );
                    cairo_move_to ( cr, center_x - 1.5 * quarter_font, center_y );
                    cairo_line_to ( cr, center_x + 0.3 * quarter_font, center_y );
                    cairo_move_to ( cr, center_x + 0.3 * quarter_font, center_y - 2.0 * quarter_font );
                    cairo_line_to ( cr, center_x + 0.3 * quarter_font, center_y + 2.0 * quarter_font );
                    cairo_stroke (cr);
                    cairo_move_to ( cr, center_x + 1.5 * quarter_font, center_y - 2.0 * quarter_font );
                    cairo_line_to ( cr, center_x + 1.5 * quarter_font, center_y );
                    cairo_move_to ( cr, center_x + 0.8 * quarter_font, center_y - 1.6 * quarter_font );
                    cairo_line_to ( cr, center_x + 2.2 * quarter_font, center_y - 0.4 * quarter_font );
                    cairo_move_to ( cr, center_x + 2.2 * quarter_font, center_y - 1.6 * quarter_font );
                    cairo_line_to ( cr, center_x + 0.8 * quarter_font, center_y - 0.4 * quarter_font );
                    cairo_stroke (cr);
                }

                /* adjust the text position */
                text1_top = circle_bottom + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
            {
                double border_right = border_left + border_width;
                double border_bottom = border_top + border_height;
                double y_center;
                y_center = geometry_rectangle_get_y_center ( classifier_bounds );
                double x_indent;
                x_indent = border_height / 2.0;

                cairo_move_to ( cr, border_right, border_bottom );
                cairo_line_to ( cr, border_right, border_top );
                cairo_line_to ( cr, border_left, border_top );
                cairo_line_to ( cr, border_left + x_indent, y_center );
                cairo_line_to ( cr, border_left, border_bottom );
                cairo_line_to ( cr, border_right, border_bottom );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
            {
                double border_right = border_left + border_width;
                double border_bottom = border_top + border_height;
                double y_center;
                y_center = geometry_rectangle_get_y_center ( classifier_bounds );
                double x_indent;
                x_indent = border_height / 2.0;

                cairo_move_to ( cr, border_right - x_indent, border_bottom );
                cairo_line_to ( cr, border_right, y_center );
                cairo_line_to ( cr, border_right - x_indent, border_top );
                cairo_line_to ( cr, border_left, border_top );
                cairo_line_to ( cr, border_left, border_bottom );
                cairo_line_to ( cr, border_right - x_indent, border_bottom );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
            {
                double border_right = border_left + border_width;
                double border_bottom = border_top + border_height;

                cairo_move_to ( cr, border_right, border_bottom );
                cairo_line_to ( cr, border_left, border_top );
                cairo_line_to ( cr, border_right, border_top );
                cairo_line_to ( cr, border_left, border_bottom );
                cairo_line_to ( cr, border_right, border_bottom );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_bottom + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
            {
                double x_center;
                x_center = geometry_rectangle_get_x_center ( classifier_bounds );
                double block_width;
                block_width = 3.0 * pencil_size_get_bold_line_width( pencil_size );

                cairo_rectangle ( cr, x_center - (0.5 * block_width), border_top, block_width, border_height );
                cairo_fill (cr);

                /* adjust the text position */
                text1_top = border_top + border_height + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
            {
                double right, bottom;
                right = geometry_rectangle_get_right ( classifier_bounds );
                bottom = geometry_rectangle_get_bottom ( classifier_bounds );
                double x_center, y_center;
                x_center = geometry_rectangle_get_x_center ( classifier_bounds );
                y_center = geometry_rectangle_get_y_center ( classifier_bounds );

                cairo_move_to ( cr, x_center, bottom );
                cairo_line_to ( cr, left, y_center );
                cairo_line_to ( cr, x_center, top );
                cairo_line_to ( cr, right, y_center );
                cairo_line_to ( cr, x_center, bottom );
                cairo_stroke (cr);

                /* adjust the text position */
                double some_offset = pencil_size_get_standard_font_size( pencil_size );
                text1_top = y_center - some_offset;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
            case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
            {
                double title_corner_height = pencil_size_get_standard_font_size( pencil_size );
                double title_corner_edge45 = 0.4 * title_corner_height;
                double title_corner_width = border_width/5.0;

                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_move_to ( cr, border_left, border_top+title_corner_height );
                cairo_line_to ( cr, border_left+title_corner_width - title_corner_edge45, border_top+title_corner_height );
                cairo_line_to ( cr, border_left+title_corner_width, border_top+title_corner_height-title_corner_edge45 );
                cairo_line_to ( cr, border_left+title_corner_width, border_top );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap + title_corner_height;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
            {
                double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

                cairo_rectangle ( cr, border_left, border_top, border_width/3.0, top_ornament_height );

                cairo_rectangle ( cr, border_left, border_top+top_ornament_height, border_width, border_height-top_ornament_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap + top_ornament_height;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMMENT:
            {
                double corner_edge = pencil_size_get_standard_font_size( pencil_size );
                double border_bottom = border_top + border_height;
                double border_right = border_left + border_width;

                cairo_move_to ( cr, border_right, border_top + corner_edge );
                cairo_line_to ( cr, border_right - corner_edge, border_top + corner_edge );
                cairo_line_to ( cr, border_right - corner_edge, border_top );
                cairo_line_to ( cr, border_left, border_top );
                cairo_line_to ( cr, border_left, border_bottom );
                cairo_line_to ( cr, border_right, border_bottom );
                cairo_line_to ( cr, border_right, border_top + corner_edge );
                cairo_line_to ( cr, border_right - corner_edge, border_top );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            default:
            {
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_painter_draw()");
            }
            break;
        }

        /* draw stereotype text */
        int text1_height = 0;
        {
            if ( 0 != utf8string_get_length( data_classifier_get_stereotype_ptr( classifier ) ) )
            {
                /* prepare text */
                char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                utf8stringbuf_copy_str( stereotype_buf, "<<" );
                utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
                utf8stringbuf_append_str( stereotype_buf, ">>" );

                int text1_width;
                pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
                pango_layout_set_text (font_layout, utf8stringbuf_get_string( stereotype_buf ), -1);
                pango_layout_get_pixel_size (font_layout, &text1_width, &text1_height);
                cairo_move_to ( cr, left + 0.5*( width - text1_width ), text1_top );
                pango_cairo_show_layout (cr, font_layout);
            }
        }

        /* draw name text */
        int text2_height = 0;
        {
            bool is_always_instance;
            bool is_anonymous_instance;
            is_always_instance = data_rules_is_always_instance( &((*this_).data_rules), classifier_type );
            is_anonymous_instance = ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ));

            /* prepare text */
            char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE + 1 ];
            utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
            if ( is_anonymous_instance && ( ! is_always_instance ) )
            {
                utf8stringbuf_copy_str( name_buf, ":" );
            }
            else
            {
                utf8stringbuf_clear( name_buf );
            }
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

            int text2_width;
            pango_layout_set_font_description (font_layout, pencil_size_get_title_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( name_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

            /* highlight */
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_EMPHASIS ))
            {
                GdkRGBA emph_color = pencil_size_get_emphasized_color( pencil_size );
                cairo_set_source_rgba( cr, emph_color.red, emph_color.green, emph_color.blue, emph_color.alpha );
                cairo_rectangle ( cr, left + 0.5*( width - text2_width ), text1_top+text1_height+f_line_gap, text2_width, text2_height );
                cairo_fill (cr);
            }

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, left + 0.5*( width - text2_width ), text1_top+text1_height+f_line_gap );
            pango_cairo_show_layout (cr, font_layout);

            /* underline instances */
            if ( is_always_instance || is_anonymous_instance )
            {
                cairo_move_to ( cr, left + 0.5*( width - text2_width ), text1_top+text1_height+f_line_gap+text2_height );
                cairo_line_to ( cr, left + 0.5*( width + text2_width ), text1_top+text1_height+f_line_gap+text2_height );
                cairo_stroke (cr);
            }
        }

        /* draw description text */
        if (( DATA_CLASSIFIER_TYPE_UML_COMMENT == classifier_type )
            || ( DATA_CLASSIFIER_TYPE_REQUIREMENT == classifier_type ))
        {
            int text3_width;
            int text3_height;
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (font_layout, data_classifier_get_description_ptr( classifier ), -1);
            pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, left + 0.5*( width - text3_width ), text1_top+text1_height+f_line_gap+text2_height+gap+gap );
            pango_cairo_show_layout (cr, font_layout);
        }
        else if (( DATA_CLASSIFIER_TYPE_UML_CLASS == classifier_type )
            || ( DATA_CLASSIFIER_TYPE_UML_OBJECT == classifier_type )
            || ( DATA_CLASSIFIER_TYPE_UML_INTERFACE == classifier_type ))
        {
            /* draw property and operation compartments */
            cairo_move_to ( cr, border_left, text1_top+text1_height+f_line_gap+text2_height+gap+gap );
            cairo_line_to ( cr, border_left + border_width, text1_top+text1_height+f_line_gap+text2_height+gap+gap );
            cairo_stroke (cr);

            cairo_move_to ( cr, border_left, text1_top+text1_height+f_line_gap+text2_height+gap+gap+gap );
            cairo_line_to ( cr, border_left + border_width, text1_top+text1_height+f_line_gap+text2_height+gap+gap+gap );
            cairo_stroke (cr);
        }

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ) )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *classifier_bounds, cr );
        }

        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ) )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *classifier_bounds, cr );
        }
    }

    TRACE_END();
}

void pencil_classifier_painter_get_minimum_bounds ( const pencil_classifier_painter_t *this_,
                                                    const data_visible_classifier_t *visible_classifier,
                                                    const pencil_size_t *pencil_size,
                                                    PangoLayout *font_layout,
                                                    geometry_rectangle_t *out_classifier_bounds,
                                                    geometry_rectangle_t *out_classifier_space )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != visible_classifier );
    assert( NULL != out_classifier_bounds );
    assert( NULL != out_classifier_space );
    assert( NULL != font_layout );

    /* border sizes of the classifier-shape */
    double top_border = 0.0;
    double left_border = 0.0;
    double bottom_border = 0.0;
    double right_border = 0.0;

    /* stereotype and name dimensions */
    double text_height;
    double text_width;

    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );

    TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

    /* determine border sizes of the classifier-shape */
    pencil_classifier_painter_private_get_shape_border_dimensions( this_,
                                                                   data_classifier_get_main_type ( classifier ),
                                                                   pencil_size,
                                                                   &top_border,
                                                                   &left_border,
                                                                   &bottom_border,
                                                                   &right_border
                                                                 );

    /* determine stereotype and name dimensions */
    pencil_classifier_painter_private_get_stereotype_and_name_dimensions( this_,
                                                                          visible_classifier,
                                                                          pencil_size,
                                                                          font_layout,
                                                                          &text_height,
                                                                          &text_width
                                                                        );

    /* minimum bounding box */
    double left = 0.0;
    double top = 0.0;
    double width = left_border + text_width + right_border;
    double height = top_border + text_height + bottom_border;

    geometry_rectangle_reinit( out_classifier_bounds, left, top, width, height );
    geometry_rectangle_reinit( out_classifier_space, left + left_border, top + top_border + text_height, text_width, 0.0 );
    TRACE_END();
}

void pencil_classifier_painter_get_drawing_space ( const pencil_classifier_painter_t *this_,
                                                   const data_visible_classifier_t *visible_classifier,
                                                   const pencil_size_t *pencil_size,
                                                   const geometry_rectangle_t *classifier_bounds,
                                                   PangoLayout *font_layout,
                                                   geometry_rectangle_t *out_classifier_space )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != visible_classifier );
    assert( NULL != classifier_bounds );
    assert( NULL != out_classifier_space );
    assert( NULL != font_layout );

    double space_left = 0.0;
    double space_top = 0.0;
    double space_width = 0.0;
    double space_height = 0.0;

    /* get the classifier */
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );

    /* determine border sizes of the classifier-shape */
    double top_border;
    double left_border;
    double bottom_border;
    double right_border;
    pencil_classifier_painter_private_get_shape_border_dimensions( this_,
                                                                   data_classifier_get_main_type ( classifier ),
                                                                   pencil_size,
                                                                   &top_border,
                                                                   &left_border,
                                                                   &bottom_border,
                                                                   &right_border
                                                                 );

    /* determine stereotype and name dimensions */
    double text_height;
    double text_width;
    pencil_classifier_painter_private_get_stereotype_and_name_dimensions( this_,
                                                                          visible_classifier,
                                                                          pencil_size,
                                                                          font_layout,
                                                                          &text_height,
                                                                          &text_width
                                                                        );

    /* calculate the result */
    space_left = geometry_rectangle_get_left( classifier_bounds ) + left_border;
    space_width = geometry_rectangle_get_width( classifier_bounds ) - left_border - right_border;
    space_top = geometry_rectangle_get_top( classifier_bounds ) + top_border + text_height;
    space_height = geometry_rectangle_get_height( classifier_bounds ) - top_border - bottom_border - text_height;

    geometry_rectangle_reinit( out_classifier_space, space_left, space_top, space_width, space_height );

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
