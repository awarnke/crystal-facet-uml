/* File: pencil_classifier_composer.c; Copyright and License: see below */

#include "pencil_classifier_composer.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_classifier_composer_init( pencil_classifier_composer_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    data_rules_init ( &((*this_).data_rules) );
    draw_classifier_icon_init( &((*this_).draw_classifier_icon) );
    draw_classifier_label_init( &((*this_).draw_classifier_label) );
    draw_classifier_contour_init( &((*this_).draw_classifier_contour) );
    draw_stereotype_image_init( &((*this_).draw_stereotype_image) );

    U8_TRACE_END();
}

void pencil_classifier_composer_destroy( pencil_classifier_composer_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_classifier_icon_destroy( &((*this_).draw_classifier_icon) );
    draw_classifier_label_destroy( &((*this_).draw_classifier_label) );
    draw_classifier_contour_destroy( &((*this_).draw_classifier_contour) );
    draw_stereotype_image_destroy( &((*this_).draw_stereotype_image) );
    data_rules_destroy ( &((*this_).data_rules) );
    pencil_marker_destroy( &((*this_).marker) );

    U8_TRACE_END();
}

void pencil_classifier_composer_draw ( const pencil_classifier_composer_t *this_,
                                       const layout_visible_classifier_t *layouted_classifier,
                                       data_id_t mark_focused,
                                       data_id_t mark_highlighted,
                                       const data_small_set_t *mark_selected,
                                       const pencil_layout_data_t *layout_data,
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
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_diagramelement_t *const diagramelement
        = data_visible_classifier_get_diagramelement_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier );
    const bool has_stereotype = data_classifier_has_stereotype( classifier );
    const bool has_stereotype_image
        = draw_stereotype_image_exists( &((*this_).draw_stereotype_image), classifier_stereotype, profile );
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

        /* determine border sizes of the classifier-shape */
        const geometry_rectangle_t space_and_label
            = has_contour
            ? draw_classifier_contour_calc_inner_area( &((*this_).draw_classifier_contour),
                                                       classifier_type,
                                                       classifier_symbol_box,
                                                       pencil_size
                                                     )
            : (*classifier_symbol_box);

        /* draw icon */
        cairo_set_line_width( cr, std_line_width );
        /* cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha ); */
        const geometry_rectangle_t stereotype_box
            = has_contour
            ? draw_stereotype_image_get_bounds( &((*this_).draw_stereotype_image),
                                                geometry_rectangle_get_right( &space_and_label ),  /* x */
                                                geometry_rectangle_get_top( &space_and_label ),  /* y */
                                                GEOMETRY_H_ALIGN_RIGHT,
                                                GEOMETRY_V_ALIGN_TOP,
                                                pencil_size
                                              )
            : (*classifier_symbol_box);
        u8_error_info_t err_info;
        const u8_error_t stereotype_err
            = draw_stereotype_image_draw( &((*this_).draw_stereotype_image),
                                          data_classifier_get_stereotype_const( classifier ),
                                          profile,
                                          &foreground_color,
                                          &err_info,
                                          &stereotype_box,
                                          cr
                                        );
        if ( u8_error_info_is_error( &err_info ) )
        {
            U8_LOG_WARNING_INT( "stereotype image: unxpected token in svg path in line", u8_error_info_get_line( &err_info ) );
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
                                                        classifier_label_box,
                                                        pencil_size,
                                                        font_layout,
                                                        cr
                                                      );

        /* draw rectangle */
        geometry_rectangle_t classifier_icon_box;
        {
            geometry_rectangle_copy( &classifier_icon_box, classifier_symbol_box );
            geometry_rectangle_enlarge( &classifier_icon_box, -2.0*gap, -2.0*gap );
            geometry_rectangle_shift( &classifier_icon_box, gap, gap );
        }
        const double icon_left = geometry_rectangle_get_left ( &classifier_icon_box );
        const double icon_top = geometry_rectangle_get_top ( &classifier_icon_box );
        const double icon_width = geometry_rectangle_get_width ( &classifier_icon_box );
        const double icon_height = geometry_rectangle_get_height ( &classifier_icon_box );
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
                pencil_classifier_composer_private_draw_feature_compartments( this_,
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
                                                                 icon_left + icon_width - gap,  /* x */
                                                                 icon_top + gap,  /* y */
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
                                                                icon_left + icon_width - gap,  /* x */
                                                                icon_top + gap,  /* y */
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
                pencil_classifier_composer_private_draw_feature_compartments ( this_,
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
                pencil_classifier_composer_private_draw_feature_compartments ( this_,
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
                    const double actor_height = icon_height;
                    const double half_width = 0.5 * icon_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_actor_bounds( &((*this_).draw_classifier_icon),
                                                                 icon_left + half_width,
                                                                 icon_top,
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
                    const double circle_diameter = icon_height;
                    const double half_width = 0.5 * icon_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_circle_bounds( &((*this_).draw_classifier_icon),
                                                                  icon_left + half_width,
                                                                  icon_top,
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
                    const double time_icon_height = icon_height;
                    const double half_width = 0.5 * icon_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_time_bounds( &((*this_).draw_classifier_icon),
                                                                icon_left + half_width,
                                                                icon_top,
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
                    const double box_icon_height = icon_height;
                    const double half_width = 0.5 * icon_width;
                    const geometry_rectangle_t icon_bounds
                        = draw_classifier_icon_get_sync_bounds( &((*this_).draw_classifier_icon),
                                                                icon_left + half_width,
                                                                icon_top,
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
                draw_classifier_contour_draw_package( &((*this_).draw_classifier_contour),
                                                      classifier_symbol_box,
                                                      classifier_label_box,
                                                      pencil_size,
                                                      cr
                                                    );
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
                U8_LOG_ERROR("unknown data_classifier_type_t in pencil_classifier_composer_draw()");
            }
            break;
        }

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
        /* draw the rectangles */
        {
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const( layouted_classifier );
            const geometry_rectangle_t *const classifier_label_box
                = layout_visible_classifier_get_label_box_const( layouted_classifier );

            cairo_set_source_rgba( cr, 1.0, 0.5, 0.6, 0.5 );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_symbol_box ),
                              geometry_rectangle_get_top ( classifier_symbol_box ),
                              geometry_rectangle_get_width ( classifier_symbol_box ),
                              geometry_rectangle_get_height ( classifier_symbol_box )
                            );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_space ),
                              geometry_rectangle_get_top ( classifier_space ),
                              geometry_rectangle_get_width ( classifier_space ),
                              geometry_rectangle_get_height ( classifier_space )
                            );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_label_box ),
                              geometry_rectangle_get_top ( classifier_label_box ),
                              geometry_rectangle_get_width ( classifier_label_box ),
                              geometry_rectangle_get_height ( classifier_label_box )
                            );
            cairo_stroke (cr);
        }
#endif

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

int pencil_classifier_composer_expand_space ( const pencil_classifier_composer_t *this_,
                                              const geometry_rectangle_t *space,
                                              bool shows_contained_children,
                                              const data_profile_part_t *profile,
                                              const pencil_size_t *pencil_size,
                                              PangoLayout *font_layout,
                                              layout_visible_classifier_t *io_classifier_layout )
{
    U8_TRACE_BEGIN();
    assert( NULL != space );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != io_classifier_layout );

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier );
    const bool has_stereotype = data_classifier_has_stereotype( classifier );
    const bool has_stereotype_image
        = draw_stereotype_image_exists( &((*this_).draw_stereotype_image), classifier_stereotype, profile );

    U8_TRACE_INFO_INT("expanding bounds of classifier id:", data_classifier_get_row_id( classifier ) );
    U8_TRACE_INFO_INT_INT("expanding bounds of classifier type, children:", classifier_type, shows_contained_children?1:0 );

    /* determine icon space */
    const geometry_dimensions_t icon_dim
        = has_stereotype_image
        ? draw_stereotype_image_get_dimensions( &((*this_).draw_stereotype_image),
                                                pencil_size
                                              )
        : draw_classifier_icon_get_icon_dimensions( &((*this_).draw_classifier_icon),
                                                    classifier_type,
                                                    pencil_size
                                                  );

    /* determine border sizes of the main line (label and optionally icon) */
    geometry_rectangle_t label_rect;
    geometry_rectangle_init_empty( &label_rect );
    geometry_rectangle_t label_compartment;
    geometry_rectangle_init_empty( &label_compartment );
    const geometry_rectangle_t *space_and_label_fake = space;  /* fake space_and_label to be identical to requested space */
    const int area_too_small
        = pencil_classifier_composer_private_get_label_box( this_,
                                                            visible_classifier,
                                                            shows_contained_children,
                                                            space_and_label_fake,
                                                            &icon_dim,
                                                            pencil_size,
                                                            font_layout,
                                                            &label_rect,
                                                            &label_compartment
                                                          );
    if ( area_too_small != 0 )
    {
        U8_TRACE_INFO("new width is defined by label-and-icon, not by requested inner space" );
    }
    const double label_top = geometry_rectangle_get_top( space ) - geometry_rectangle_get_height( &label_compartment );
    geometry_rectangle_set_top( &label_rect, label_top );
    geometry_rectangle_set_top( &label_compartment, label_top );

    /* sizes of geometric objects are determined, */
    /* now position the geometric objects */
    {
        const bool has_contour
            = draw_classifier_contour_has_contour( &((*this_).draw_classifier_contour), classifier_type );

        if ( has_contour )
        {
            U8_TRACE_INFO("calculating symbol box as envelope around label and space..." );

            /* calculate symbol bounds */
            geometry_rectangle_t inner_area;
            if ( area_too_small )
            {
                geometry_rectangle_init( &inner_area,
                                         geometry_rectangle_get_left( &label_compartment ),
                                         geometry_rectangle_get_top( &label_compartment ),
                                         geometry_rectangle_get_width( &label_compartment ),
                                         geometry_rectangle_get_height( &label_compartment )
                                         + geometry_rectangle_get_height( space )
                                       );
            }
            else
            {
                geometry_rectangle_copy( &inner_area, space );
                geometry_rectangle_shift( &inner_area, 0.0, -geometry_rectangle_get_height( &label_compartment ) );
                geometry_rectangle_enlarge( &inner_area, 0.0, geometry_rectangle_get_height( &label_compartment ) );
            }
            const geometry_rectangle_t envelope
                = draw_classifier_contour_calc_outer_bounds( &((*this_).draw_classifier_contour),
                                                             classifier_type,
                                                             &inner_area,
                                                             pencil_size
                                                           );
            layout_visible_classifier_set_symbol_box( io_classifier_layout, &envelope );
            geometry_rectangle_destroy( &inner_area );
        }
        else
        {
            U8_TRACE_INFO("calculating symbol box for fixed-sized icon..." );

            const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
            const double symbol_width = symbol_height;

            /* calculate symbol bounds */
            const geometry_h_align_t H_CENTER = GEOMETRY_H_ALIGN_CENTER;
            const double symbol_left
                = geometry_h_align_get_left( &H_CENTER,
                                             symbol_width,
                                             geometry_rectangle_get_left( space ),
                                             geometry_rectangle_get_width( space )
                                           );
            const double symbol_top = geometry_rectangle_get_top( &label_compartment ) - symbol_height;
            geometry_rectangle_t classifier_symbol_box;
            geometry_rectangle_init( &classifier_symbol_box, symbol_left, symbol_top, symbol_width, symbol_height );
            layout_visible_classifier_set_symbol_box( io_classifier_layout, &classifier_symbol_box );
            geometry_rectangle_destroy( &classifier_symbol_box );
        }

        /* calculate label_box */
        layout_visible_classifier_set_label_box( io_classifier_layout, &label_rect );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        /* get the symbol and label boxes and inner space rectangles to modify */
        geometry_rectangle_t classifier_space;
        geometry_rectangle_init( &classifier_space,
                                 geometry_rectangle_get_left( &label_compartment ),
                                 geometry_rectangle_get_top( space ),
                                 geometry_rectangle_get_width( &label_compartment ),
                                 geometry_rectangle_get_height( space )
                               );
        layout_visible_classifier_set_space( io_classifier_layout, &classifier_space );
        geometry_rectangle_destroy( &classifier_space );
    }

    U8_TRACE_INFO("==== symbol_box ====" );
    geometry_rectangle_trace( layout_visible_classifier_get_symbol_box_const( io_classifier_layout ) );
    U8_TRACE_INFO("==== label_box  ====" );
    geometry_rectangle_trace( &label_rect );
    U8_TRACE_INFO("==== space     =====" );
    geometry_rectangle_trace( layout_visible_classifier_get_space_const( io_classifier_layout ) );

    geometry_rectangle_destroy( &label_rect );
    geometry_rectangle_destroy( &label_compartment );

    U8_TRACE_END_ERR(area_too_small);
    return area_too_small;
}

int pencil_classifier_composer_set_envelope_box( const pencil_classifier_composer_t *this_,
                                                 const geometry_rectangle_t *envelope,
                                                 bool shows_contained_children,
                                                 const data_profile_part_t *profile,
                                                 const pencil_size_t *pencil_size,
                                                 PangoLayout *font_layout,
                                                 layout_visible_classifier_t *io_classifier_layout )
{
    U8_TRACE_BEGIN();
    assert( NULL != envelope );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != io_classifier_layout );

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier );
    const bool has_stereotype = data_classifier_has_stereotype( classifier );
    const bool has_stereotype_image
        = draw_stereotype_image_exists( &((*this_).draw_stereotype_image), classifier_stereotype, profile );

    U8_TRACE_INFO_INT("calculating bounds of classifier id, type:", data_classifier_get_row_id( classifier ) );
    U8_TRACE_INFO_INT_INT("calculating bounds of classifier type, children:", classifier_type, shows_contained_children?1:0 );

    /* determine icon space */
    const geometry_dimensions_t icon_dim
        = has_stereotype_image
        ? draw_stereotype_image_get_dimensions( &((*this_).draw_stereotype_image),
                                                pencil_size
                                              )
        : draw_classifier_icon_get_icon_dimensions( &((*this_).draw_classifier_icon),
                                                    classifier_type,
                                                    pencil_size
                                                  );

    /* determine border sizes of the classifier-shape */
    const geometry_rectangle_t space_and_label
        = draw_classifier_contour_calc_inner_area( &((*this_).draw_classifier_contour),
                                                   classifier_type,
                                                   envelope,
                                                   pencil_size
                                                 );

    /* determine border sizes of the main line (label and optionally icon) */
    geometry_rectangle_t label_rect;
    geometry_rectangle_init_empty( &label_rect );
    geometry_rectangle_t label_compartment;
    geometry_rectangle_init_empty( &label_compartment );
    const int area_too_small
        = pencil_classifier_composer_private_get_label_box( this_,
                                                            visible_classifier,
                                                            shows_contained_children,
                                                            &space_and_label,
                                                            &icon_dim,
                                                            pencil_size,
                                                            font_layout,
                                                            &label_rect,
                                                            &label_compartment
                                                          );

    /* if label fits into space_and_label */
    if ( 0 == area_too_small )
    {
        const bool has_contour
            = draw_classifier_contour_has_contour( &((*this_).draw_classifier_contour), classifier_type );

        /* get the symbol and label boxes and inner space rectangles to modify */
        geometry_rectangle_t classifier_space;
        geometry_rectangle_copy( &classifier_space, &space_and_label );

        if ( has_contour )
        {
            U8_TRACE_INFO("calculating symbol box as envelope around label and space..." );

            /* calculate symbol bounds */
            layout_visible_classifier_set_symbol_box( io_classifier_layout, envelope );

            /* calculate space */
            geometry_rectangle_shift( &classifier_space, 0.0, geometry_rectangle_get_height( &label_compartment ) );
            geometry_rectangle_enlarge( &classifier_space, 0.0, -geometry_rectangle_get_height( &label_compartment ) );
        }
        else
        {
            U8_TRACE_INFO("calculating symbol box for fixed-sized icon..." );

            const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
            const double symbol_width = symbol_height;

            /* calculate symbol bounds */
            const geometry_h_align_t H_CENTER = GEOMETRY_H_ALIGN_CENTER;
            const double symbol_left
                = geometry_h_align_get_left( &H_CENTER,
                                             symbol_width,
                                             geometry_rectangle_get_left( &space_and_label ),
                                             geometry_rectangle_get_width( &space_and_label )
                                           );
            const double symbol_top = geometry_rectangle_get_top( &label_compartment ) - symbol_height;
            geometry_rectangle_t classifier_symbol_box;
            geometry_rectangle_init( &classifier_symbol_box, symbol_left, symbol_top, symbol_width, symbol_height );
            layout_visible_classifier_set_symbol_box( io_classifier_layout, &classifier_symbol_box );
            geometry_rectangle_destroy( &classifier_symbol_box );

            /* calculate space */
            const double label_and_symbol_height = geometry_rectangle_get_height( &label_compartment ) + symbol_height;
            geometry_rectangle_shift( &classifier_space, 0.0, label_and_symbol_height );
            geometry_rectangle_enlarge( &classifier_space, 0.0, -label_and_symbol_height );
        }
        layout_visible_classifier_set_space( io_classifier_layout, &classifier_space );


        /* calculate label_box */
        layout_visible_classifier_set_label_box( io_classifier_layout, &label_rect );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        U8_TRACE_INFO("==== symbol_box ====" );
        geometry_rectangle_trace( layout_visible_classifier_get_symbol_box_const( io_classifier_layout ) );
        U8_TRACE_INFO("==== label_box  ====" );
        geometry_rectangle_trace( &label_rect );
        U8_TRACE_INFO("==== space     =====" );
        geometry_rectangle_trace( &classifier_space );

        geometry_rectangle_destroy( &classifier_space );
    }
    else
    {
        geometry_rectangle_t space_guess;  /* guess the inner space based on current text height */
        geometry_rectangle_copy( &space_guess, &space_and_label );
        geometry_rectangle_shift( &space_guess, 0.0, geometry_rectangle_get_height( &label_compartment ) );
        geometry_rectangle_enlarge( &space_guess, 0.0, -geometry_rectangle_get_height( &label_compartment ) );
        U8_TRACE_INFO("==== space_guess====" );
        geometry_rectangle_trace( &space_guess );
        pencil_classifier_composer_expand_space( this_,
                                                 &space_guess,
                                                 shows_contained_children,
                                                 profile,
                                                 pencil_size,
                                                 font_layout,
                                                 io_classifier_layout
                                               );
        geometry_rectangle_destroy( &space_guess );

        /* shift/center to requested position after resizing beyond requested size */
        const geometry_rectangle_t current_envelope
            = layout_visible_classifier_get_envelope_box( io_classifier_layout );
        const double shift_to_right = geometry_rectangle_get_center_x( envelope ) - geometry_rectangle_get_center_x( &current_envelope );
        const double shift_to_bottom = geometry_rectangle_get_center_y( envelope ) - geometry_rectangle_get_center_y( &current_envelope );
        layout_visible_classifier_shift( io_classifier_layout, shift_to_right, shift_to_bottom );
    }

    geometry_rectangle_destroy( &label_rect );
    geometry_rectangle_destroy( &label_compartment );

    U8_TRACE_END_ERR( area_too_small );
    return area_too_small;
}

static inline double PENCIL_MAX_OF_2 ( double a, double b ) { return (a<b) ? b : a; }

int pencil_classifier_composer_private_get_label_box ( const pencil_classifier_composer_t *this_,
                                                       const data_visible_classifier_t *visible_classifier,
                                                       bool shows_contained_children,
                                                       const geometry_rectangle_t *space_and_label,
                                                       const geometry_dimensions_t *icon_dim,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       geometry_rectangle_t *out_label_box,
                                                       geometry_rectangle_t *out_label_compartment )
{
    U8_TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != space_and_label );
    assert( NULL != icon_dim );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_label_box );
    assert( NULL != out_label_compartment );

    int result = 0;

    /* get classifier type */
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );

    /* get standard gap size */
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* determine stereotype and name dimensions */
    double text_width;
    double text_height;
    const bool has_contour
        = draw_classifier_contour_has_contour( &((*this_).draw_classifier_contour), classifier_type );
    const double icon_gap
        = ( ! has_contour ) ? 0.0 : ( geometry_dimensions_get_width( icon_dim ) < 0.000001 ) ? 0.0 : gap;
    {
        const double proposed_label_width
            = has_contour
            ? geometry_rectangle_get_width( space_and_label ) - geometry_dimensions_get_width( icon_dim ) - icon_gap
            : geometry_rectangle_get_width( space_and_label );
        geometry_dimensions_t proposed_label_dim;
        geometry_dimensions_init( &proposed_label_dim, proposed_label_width, geometry_rectangle_get_height( space_and_label ) );
        draw_classifier_label_get_stereotype_and_name_dimensions( &((*this_).draw_classifier_label),
                                                                  visible_classifier,
                                                                  &proposed_label_dim,
                                                                  pencil_size,
                                                                  font_layout,
                                                                  &text_width,
                                                                  &text_height
                                                                );
        geometry_dimensions_destroy( &proposed_label_dim );

        if ( text_width > (proposed_label_width + 0.0001) )
        {
            U8_TRACE_INFO_INT_INT("label does not fit to provided width", (int)text_width, (int)proposed_label_width );
            result = 1;
        }
        const double proposed_label_height = geometry_rectangle_get_height( space_and_label );
        if ( text_height > (proposed_label_height + 0.0001) )
        {
            U8_TRACE_INFO_INT_INT("label does not fit to provided height", (int)text_height, (int)proposed_label_height );
            result = 1;
        }
    }

    if ( has_contour )
    {
        double top_border = geometry_rectangle_get_top( space_and_label );

        /* calculate label_compartment */
        {
            const double min_required_width = text_width + icon_gap + geometry_dimensions_get_width( icon_dim );
            const double comp_width = PENCIL_MAX_OF_2( min_required_width, geometry_rectangle_get_width( space_and_label ) );
            const geometry_h_align_t compartment_h_align = GEOMETRY_H_ALIGN_CENTER;
            const double comp_left = geometry_h_align_get_left( &compartment_h_align,
                                                                comp_width,
                                                                geometry_rectangle_get_left( space_and_label ),
                                                                geometry_rectangle_get_width( space_and_label )
                                                              );
            geometry_rectangle_reinit( out_label_compartment, comp_left, top_border, comp_width, text_height );
        }

        /* calculate label_box */
        const bool is_package_with_contents = (classifier_type == DATA_CLASSIFIER_TYPE_PACKAGE) && shows_contained_children;
        if ( is_package_with_contents )
        {
            const geometry_rectangle_t envelope
                = draw_classifier_contour_calc_outer_bounds( &((*this_).draw_classifier_contour),
                                                             classifier_type,
                                                             out_label_compartment,
                                                             pencil_size
                                                           );
            geometry_rectangle_reinit( out_label_box,
                                       geometry_rectangle_get_left( &envelope ) + 2.0 * gap,
                                       geometry_rectangle_get_top( &envelope ) + 2.0 * gap,
                                       text_width,
                                       text_height
                                     );
        }
        else
        {
            const geometry_h_align_t text_h_align = GEOMETRY_H_ALIGN_CENTER;
            const double text_left = geometry_h_align_get_left( &text_h_align,
                                                                text_width,
                                                                geometry_rectangle_get_left( space_and_label ),
                                                                geometry_rectangle_get_width( space_and_label ) - icon_gap - geometry_dimensions_get_width( icon_dim )
                                                              );
            geometry_rectangle_reinit( out_label_box,
                                       text_left,
                                       top_border,
                                       text_width,
                                       text_height
                                     );
        }
    }
    else
    {
        /*const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );*/

        /* calculate text position */
        const geometry_h_align_t text_h_align = GEOMETRY_H_ALIGN_CENTER;
        const double text_left = geometry_h_align_get_left( &text_h_align,
                                                            text_width,
                                                            geometry_rectangle_get_left( space_and_label ),
                                                            geometry_rectangle_get_width( space_and_label )
                                                          );
        const double text_top = geometry_rectangle_get_top( space_and_label ) /*+ symbol_height*/;

        /* calculate label_compartment */
        const double comp_width = PENCIL_MAX_OF_2( text_width, geometry_rectangle_get_width( space_and_label ) );
        const geometry_h_align_t compartment_h_align = GEOMETRY_H_ALIGN_CENTER;
        const double comp_left = geometry_h_align_get_left( &compartment_h_align,
                                                            comp_width,
                                                            geometry_rectangle_get_left( space_and_label ),
                                                            geometry_rectangle_get_width( space_and_label )
                                                          );
        geometry_rectangle_reinit( out_label_compartment, comp_left, text_top, comp_width, text_height );

        /* calculate label_box */
        geometry_rectangle_reinit( out_label_box, text_left, text_top, text_width, text_height );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

void pencil_classifier_composer_private_draw_feature_compartments ( const pencil_classifier_composer_t *this_,
                                                                    const layout_visible_classifier_t *layouted_classifier,
                                                                    const pencil_layout_data_t *layout_data,
                                                                    const pencil_size_t *pencil_size,
                                                                    cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != layouted_classifier );
    assert( NULL != layout_data );
    assert( NULL != pencil_size );
    assert( NULL != cr );

    /* determine number of properties and operations */
    uint32_t count_properties = 0;
    uint32_t count_operations = 0;
    uint32_t count_tagged_values = 0;
    {
        /* define names for input data */
        const data_row_id_t diagele_id = layout_visible_classifier_get_diagramelement_id ( layouted_classifier );

        const uint32_t num_features = pencil_layout_data_get_feature_count ( layout_data );
        for ( uint32_t f_probe_idx = 0; f_probe_idx < num_features; f_probe_idx ++ )
        {
            const layout_feature_t *const f_probe_layout = pencil_layout_data_get_feature_const ( layout_data, f_probe_idx );
            assert ( NULL != f_probe_layout );
            const layout_visible_classifier_t *const probe_vis_classfy = layout_feature_get_classifier_const ( f_probe_layout );
            assert ( NULL != probe_vis_classfy );

            /* check if this f_probe_layout has the same diagram element id as the_feature */
            if ( diagele_id == layout_visible_classifier_get_diagramelement_id ( probe_vis_classfy ) )
            {
                /* this is a feature of the current layouted_classifier */
                /* define names for input data */
                const data_feature_t *const f_probe_data = layout_feature_get_data_const ( f_probe_layout );
                assert ( NULL != f_probe_data );
                const data_feature_type_t f_probe_type = data_feature_get_main_type ( f_probe_data );

                if ( DATA_FEATURE_TYPE_PROPERTY == f_probe_type )
                {
                    count_properties ++;
                }
                else if ( DATA_FEATURE_TYPE_OPERATION == f_probe_type )
                {
                    count_operations ++;
                }
                else if ( DATA_FEATURE_TYPE_TAGGED_VALUE == f_probe_type )
                {
                    count_tagged_values ++;
                }
            }
        }
    }

    /* draw compartments if there are features */
    if (( count_properties != 0 )||( count_operations != 0 )||( count_tagged_values != 0 ))
    {
        /* define names for input data */
        const geometry_rectangle_t *const classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
        const geometry_rectangle_t *const classifier_space
            = layout_visible_classifier_get_space_const( layouted_classifier );
        const double feature_height = pencil_size_get_standard_font_size( pencil_size )
            + pencil_size_get_font_line_gap( pencil_size );
        const double gap = pencil_size_get_standard_object_border( pencil_size );

        const double y_coordinate_1 = geometry_rectangle_get_top( classifier_space );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_1,
                                                        pencil_size,
                                                        cr
                                                      );
        const double y_coordinate_2 = geometry_rectangle_get_top( classifier_space )
            + ( count_properties * feature_height ) + ( 2.0 * gap );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_2,
                                                        pencil_size,
                                                        cr
                                                      );
        const double y_coordinate_3 = geometry_rectangle_get_top( classifier_space )
            + ( (count_properties+count_operations) * feature_height ) + ( 4.0 * gap );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_3,
                                                        pencil_size,
                                                        cr
                                                      );
    }

    U8_TRACE_END();
}

/*
Copyright 2016-2023 Andreas Warnke
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
