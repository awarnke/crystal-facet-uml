/* File: pencil_classifier_painter.c; Copyright and License: see below */

#include "pencil_classifier_painter.h"
#include "u8/u8_trace.h"
#include "u8/u8_f64.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    draw_classifier_icon_init( &((*this_).draw_classifier_icon) );
    draw_classifier_label_init( &((*this_).draw_classifier_label) );
    draw_classifier_contour_init( &((*this_).draw_classifier_contour) );
    draw_stereotype_icon_init( &((*this_).draw_stereotype_icon) );

    U8_TRACE_END();
}

void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_classifier_icon_destroy( &((*this_).draw_classifier_icon) );
    draw_classifier_label_destroy( &((*this_).draw_classifier_label) );
    draw_classifier_contour_destroy( &((*this_).draw_classifier_contour) );
    draw_stereotype_icon_destroy( &((*this_).draw_stereotype_icon) );
    pencil_marker_destroy( &((*this_).marker) );

    U8_TRACE_END();
}

void pencil_classifier_painter_draw( pencil_classifier_painter_t *this_,
                                     const layout_visible_classifier_t *layouted_classifier,
                                     data_id_t mark_focused,
                                     data_id_t mark_highlighted,
                                     const data_small_set_t *mark_selected,
                                     const layout_visible_set_t *layout_data,
                                     const data_profile_part_t *profile,
                                     const pencil_size_t *pencil_size,
                                     PangoLayout *font_layout,
                                     cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != layouted_classifier );
    assert( NULL != mark_selected );
    assert( NULL != layout_data );
    assert( NULL != profile );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( layouted_classifier );
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
    const geometry_rectangle_t *const classifier_label_box
        = layout_visible_classifier_get_label_box_const( layouted_classifier );
    const geometry_rectangle_t *const classifier_icon_box
        = layout_visible_classifier_get_icon_box_const( layouted_classifier );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_diagramelement_t *const diagramelement
        = data_visible_classifier_get_diagramelement_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier );
    const bool has_stereotype_image
        = draw_stereotype_icon_exists( &((*this_).draw_stereotype_icon), classifier_stereotype, profile );
    const data_diagramelement_flag_t display_flags
        = data_diagramelement_get_display_flags( diagramelement );

    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double std_line_width = pencil_size_get_standard_line_width( pencil_size );
    /* set the right drawing color */
    GdkRGBA foreground_color;
    {
        if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_row_id( diagramelement ) ) )
        {
            foreground_color = pencil_size_get_highlight_color( pencil_size );
        }
        else if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT ))
        {
            foreground_color = pencil_size_get_gray_out_color( pencil_size );
        }
        else
        {
            foreground_color = pencil_size_get_standard_color( pencil_size );
        }
    }

    U8_TRACE_INFO_INT("drawing classifier id", data_classifier_get_row_id( classifier ) );

    /* draw the stereotype image */
    bool icon_override = false;  /* in case of a stereotype image, the icon shall not be drawn. */
    if ( has_stereotype_image )
    {
        /* check if the image is a small icon within a contour or if it is the full symbol */
        const bool has_contour = geometry_rectangle_is_containing( classifier_symbol_box, classifier_label_box );

        cairo_set_line_width( cr, std_line_width );

        u8_error_info_t err_info;
        const u8_error_t stereotype_err
            = draw_stereotype_icon_draw( &((*this_).draw_stereotype_icon),
                                         data_classifier_get_stereotype_const( classifier ),
                                         profile,
                                         &foreground_color,
                                         &err_info,
                                         has_contour ? classifier_icon_box : classifier_symbol_box,
                                         cr
                                       );
        if ( u8_error_info_is_error( &err_info ) )
        {
            U8_LOG_WARNING_INT( "stereotype image: unxpected token in svg path in line",
                                u8_error_info_get_line( &err_info )
                              );
        }
        icon_override = ( stereotype_err == U8_ERROR_NONE );
    }

    /* draw the classifier */
    {
        /* set line width */
        cairo_set_line_width( cr, std_line_width );
        /* set color */
        cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );

        /* highlight */
        if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_EMPHASIS ))
        {
            const GdkRGBA emph_color = pencil_size_get_emphasized_bgcolor( pencil_size );
            cairo_set_source_rgba( cr, emph_color.red, emph_color.green, emph_color.blue, emph_color.alpha );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left( classifier_label_box ),
                              geometry_rectangle_get_top( classifier_label_box ),
                              geometry_rectangle_get_width( classifier_label_box ),
                              geometry_rectangle_get_height( classifier_label_box )
                            );
            cairo_fill (cr);
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* draw label */
        draw_classifier_label_draw_stereotype_and_name( &((*this_).draw_classifier_label),
                                                        visible_classifier,
                                                        ( ! has_stereotype_image ),
                                                        &foreground_color,
                                                        classifier_label_box,
                                                        pencil_size,
                                                        font_layout,
                                                        cr
                                                      );

        /* draw rectangle */
        geometry_rectangle_t classifier_symbol_border;
        {
            geometry_rectangle_copy( &classifier_symbol_border, classifier_symbol_box );
            geometry_rectangle_enlarge( &classifier_symbol_border, -2.0*gap, -2.0*gap );
            geometry_rectangle_shift( &classifier_symbol_border, gap, gap );
        }
        const double symbol_left = geometry_rectangle_get_left ( &classifier_symbol_border );
        const double symbol_top = geometry_rectangle_get_top ( &classifier_symbol_border );
        const double symbol_width = geometry_rectangle_get_width ( &classifier_symbol_border );
        const double symbol_height = geometry_rectangle_get_height ( &classifier_symbol_border );
        switch ( classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_PART:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_CLASS:
            case DATA_CLASSIFIER_TYPE_OBJECT:
            case DATA_CLASSIFIER_TYPE_INTERFACE:
            case DATA_CLASSIFIER_TYPE_STEREOTYPE:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
                pencil_classifier_painter_private_draw_feature_compartments( this_,
                                                                             layouted_classifier,
                                                                             layout_data,
                                                                             pencil_size,
                                                                             cr
                                                                           );
            }
            break;

            case DATA_CLASSIFIER_TYPE_COMPONENT:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );

                /* draw icon */
                const double component_icon_height = pencil_size_get_title_font_size( pencil_size );
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_component_bounds( &((*this_).draw_classifier_icon),
                                                                 symbol_left + symbol_width - gap,  /* x */
                                                                 symbol_top + gap,  /* y */
                                                                 GEOMETRY_H_ALIGN_RIGHT,
                                                                 GEOMETRY_V_ALIGN_TOP,
                                                                 component_icon_height
                                                               );
                if ( ! icon_override )
                {
                    draw_classifier_icon_draw_component ( &((*this_).draw_classifier_icon), icon_bounds, cr );
                }
            }
            break;

            case DATA_CLASSIFIER_TYPE_ARTIFACT:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );

                /* draw icon */
                const double artifact_icon_height = pencil_size_get_title_font_size( pencil_size );
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_artifact_bounds( &((*this_).draw_classifier_icon),
                                                                symbol_left + symbol_width - gap,  /* x */
                                                                symbol_top + gap,  /* y */
                                                                GEOMETRY_H_ALIGN_RIGHT,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                artifact_icon_height
                                                              );
                if ( ! icon_override )
                {
                    draw_classifier_icon_draw_artifact ( &((*this_).draw_classifier_icon), icon_bounds, cr );
                }
            }
            break;

            case DATA_CLASSIFIER_TYPE_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_STATE:
            case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
            {
                draw_classifier_contour_draw_rounded_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, false, pencil_size, cr );
                pencil_classifier_painter_private_draw_feature_compartments( this_,
                                                                             layouted_classifier,
                                                                             layout_data,
                                                                             pencil_size,
                                                                             cr
                                                                           );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
            {
                draw_classifier_contour_draw_rounded_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, true, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_USE_CASE:
            {
                draw_classifier_contour_draw_ellipse ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
                pencil_classifier_painter_private_draw_feature_compartments( this_,
                                                                             layouted_classifier,
                                                                             layout_data,
                                                                             pencil_size,
                                                                             cr
                                                                           );
            }
            break;

            case DATA_CLASSIFIER_TYPE_NODE:
            {
                draw_classifier_contour_draw_3d_box ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_ACTOR:
            {
                if ( ! icon_override )
                {
                    const double actor_height = symbol_height;
                    const double half_width = 0.5 * symbol_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_actor_bounds( &((*this_).draw_classifier_icon),
                                                                 symbol_left + half_width,
                                                                 symbol_top,
                                                                 GEOMETRY_H_ALIGN_CENTER,
                                                                 GEOMETRY_V_ALIGN_TOP,
                                                                 actor_height
                                                               );
                    draw_classifier_icon_draw_actor ( &((*this_).draw_classifier_icon), icon_bounds, cr );
                }
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
            case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
            {
                if ( ! icon_override )
                {
                    const double circle_diameter = symbol_height;
                    const double half_width = 0.5 * symbol_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_circle_bounds( &((*this_).draw_classifier_icon),
                                                                  symbol_left + half_width,
                                                                  symbol_top,
                                                                  GEOMETRY_H_ALIGN_CENTER,
                                                                  GEOMETRY_V_ALIGN_TOP,
                                                                  circle_diameter
                                                                );
                    const bool stroke = ( classifier_type != DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE );
                    const bool fill = ( ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE )
                                    || ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE ) );
                    const bool shallow_history = ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY );
                    const bool deep_history = ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY );
                    draw_classifier_icon_draw_circle( &((*this_).draw_classifier_icon),
                                                      icon_bounds,
                                                      pencil_size,
                                                      stroke,
                                                      fill,
                                                      shallow_history,
                                                      deep_history,
                                                      cr
                                                    );
                }
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
            {
                draw_classifier_contour_draw_accept_event ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
            {
                draw_classifier_contour_draw_send_signal ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
            {
                if ( ! icon_override )
                {
                    const double time_icon_height = symbol_height;
                    const double half_width = 0.5 * symbol_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_time_bounds( &((*this_).draw_classifier_icon),
                                                                symbol_left + half_width,
                                                                symbol_top,
                                                                GEOMETRY_H_ALIGN_CENTER,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                time_icon_height
                                                              );
                    draw_classifier_icon_draw_time ( &((*this_).draw_classifier_icon), icon_bounds, cr );
                }
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
            {
                if ( ! icon_override )
                {
                    const double box_icon_height = symbol_height;
                    const double half_width = 0.5 * symbol_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_sync_bounds( &((*this_).draw_classifier_icon),
                                                                symbol_left + half_width,
                                                                symbol_top,
                                                                GEOMETRY_H_ALIGN_CENTER,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                box_icon_height,
                                                                pencil_size
                                                              );
                    draw_classifier_icon_draw_sync ( &((*this_).draw_classifier_icon), icon_bounds, cr );
                }
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
            {
                draw_classifier_contour_draw_rhombus ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:
            {
                draw_classifier_contour_draw_diagram_ref ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_PACKAGE:
            {
                geometry_rectangle_t label_and_icon_box;
                geometry_rectangle_init_by_bounds( &label_and_icon_box, classifier_label_box, classifier_icon_box );
                draw_classifier_contour_draw_package( &((*this_).draw_classifier_contour),
                                                      classifier_symbol_box,
                                                      &label_and_icon_box,
                                                      pencil_size,
                                                      cr
                                                    );
                geometry_rectangle_destroy( &label_and_icon_box );
            }
            break;

            case DATA_CLASSIFIER_TYPE_COMMENT:
            {
                draw_classifier_contour_draw_comment ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_IMAGE:
            {
                if ( ! icon_override )
                {
                    draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
                }
            }
            break;

            default:
            {
                /* this case can happen if a model file of a new cfu version is opened with an older version of cfu */
                U8_LOG_ANOMALY("unknown data_classifier_type_t in pencil_classifier_painter_draw()");
                /* type is unknown, so one cannot decide if to draw rounded corners. */
                draw_classifier_contour_draw_cornerless( &((*this_).draw_classifier_contour),
                                                         classifier_symbol_box,
                                                         pencil_size,
                                                         cr
                                                       );
            }
            break;
        }

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_row_id(diagramelement) ) )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *classifier_symbol_box, cr );
        }

        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_row_id(diagramelement) )
            ||  data_id_equals_id( &mark_focused, DATA_TABLE_CLASSIFIER, data_classifier_get_row_id(classifier) ) )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *classifier_symbol_box, cr );
        }
    }

    U8_TRACE_END();
}

void pencil_classifier_painter_private_draw_feature_compartments( const pencil_classifier_painter_t *this_,
                                                                  const layout_visible_classifier_t *layouted_classifier,
                                                                  const layout_visible_set_t *layout_data,
                                                                  const pencil_size_t *pencil_size,
                                                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != layouted_classifier );
    assert( NULL != layout_data );
    assert( NULL != pencil_size );
    assert( NULL != cr );

    /* define names for input data */
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
    const geometry_rectangle_t *const classifier_space
        = layout_visible_classifier_get_space_const( layouted_classifier );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const data_row_t diagele_id = layout_visible_classifier_get_diagramelement_id ( layouted_classifier );

    /* determine number of properties and operations */
    double compartment1_y = geometry_rectangle_get_top( classifier_space ) + gap;
    double compartment2_y = geometry_rectangle_get_top( classifier_space ) + 3.0 * gap;
    double compartment3_y = geometry_rectangle_get_top( classifier_space ) + 5.0 * gap;
    uint_fast32_t count_compartment_entries = 0;

    const uint32_t num_features = layout_visible_set_get_feature_count ( layout_data );
    for ( uint32_t f_probe_idx = 0; f_probe_idx < num_features; f_probe_idx ++ )
    {
        const layout_feature_t *const f_probe_layout = layout_visible_set_get_feature_const ( layout_data, f_probe_idx );
        assert ( NULL != f_probe_layout );
        const layout_visible_classifier_t *const probe_vis_classfy = layout_feature_get_classifier_const ( f_probe_layout );
        assert ( NULL != probe_vis_classfy );

        /* check if this f_probe_layout has the same diagram element id as the_feature */
        if ( diagele_id == layout_visible_classifier_get_diagramelement_id( probe_vis_classfy ) )
        {
            /* this is a feature of the current layouted_classifier */
            const data_feature_t *const f_probe_data = layout_feature_get_data_const( f_probe_layout );
            assert ( NULL != f_probe_data );
            const data_feature_type_t f_probe_type = data_feature_get_main_type( f_probe_data );
            const double f_probe_bottom
                = geometry_rectangle_get_bottom( layout_feature_get_symbol_box_const( f_probe_layout ) );
            if ( data_feature_type_inside_compartment( f_probe_type ) )
            {
                count_compartment_entries ++;
            }
            if ( DATA_FEATURE_TYPE_PROPERTY == f_probe_type )
            {
                compartment2_y = u8_f64_max2( compartment2_y, f_probe_bottom + gap );
                compartment3_y = u8_f64_max2( compartment3_y, f_probe_bottom + 3.0 * gap );
            }
            else if ( DATA_FEATURE_TYPE_OPERATION == f_probe_type )
            {
                compartment3_y = u8_f64_max2( compartment3_y, f_probe_bottom + gap );
            }
        }
    }

    /* draw compartments if there are features */
    if ( count_compartment_entries != 0 )
    {
        const data_visible_classifier_t *const visible_classifier
            = layout_visible_classifier_get_data_const( layouted_classifier );
        const data_classifier_t *const classifier
            = data_visible_classifier_get_classifier_const( visible_classifier );
        const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );

        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_type,
                                                        classifier_symbol_box,
                                                        compartment1_y,
                                                        pencil_size,
                                                        cr
                                                      );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_type,
                                                        classifier_symbol_box,
                                                        compartment2_y,
                                                        pencil_size,
                                                        cr
                                                      );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_type,
                                                        classifier_symbol_box,
                                                        compartment3_y,
                                                        pencil_size,
                                                        cr
                                                      );
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2025 Andreas Warnke
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
