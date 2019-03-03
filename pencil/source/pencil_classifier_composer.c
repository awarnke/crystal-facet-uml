/* File: pencil_classifier_composer.c; Copyright and License: see below */

#include "pencil_classifier_composer.h"
#include "trace.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void pencil_classifier_composer_init( pencil_classifier_composer_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    data_rules_init ( &((*this_).data_rules) );
    draw_symbol_init( &((*this_).draw_symbol) );
    draw_label_init( &((*this_).draw_label) );
    draw_contour_init( &((*this_).draw_contour) );

    TRACE_END();
}

void pencil_classifier_composer_destroy( pencil_classifier_composer_t *this_ )
{
    TRACE_BEGIN();

    draw_symbol_destroy( &((*this_).draw_symbol) );
    draw_label_destroy( &((*this_).draw_label) );
    draw_contour_destroy( &((*this_).draw_contour) );
    data_rules_destroy ( &((*this_).data_rules) );
    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_composer_draw ( const pencil_classifier_composer_t *this_,
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

    /* draw id */
    {
        /* prepare text */
        data_id_t the_id;
        data_id_init( &the_id, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );

        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE+5];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_copy_str( id_str, "{id=" );
        data_id_to_utf8stringbuf( &the_id, id_str );
        utf8stringbuf_append_str( id_str, "}" );

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
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_composer_draw()");
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

        /* draw rectangle */
        const int border_left = left + gap;
        const int border_top = top + gap;
        const int border_width = width - 2.0 * gap;
        //const int border_height = height - 2.0 * gap;
        switch ( classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_FEATURE:
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_UML_PART:
            {
                draw_contour_draw_rect ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_CLASS:
            case DATA_CLASSIFIER_TYPE_UML_OBJECT:
            case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
            {
                draw_contour_draw_rect ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );

                const geometry_rectangle_t *box = layout_visible_classifier_get_label_box_ptr( layouted_classifier );
                double text_bottom = geometry_rectangle_get_bottom(box);

                /* draw property and operation compartments */
                cairo_move_to ( cr, border_left, text_bottom+gap );
                cairo_line_to ( cr, border_left + border_width, text_bottom+gap );
                cairo_stroke (cr);

                cairo_move_to ( cr, border_left, text_bottom+gap+gap );
                cairo_line_to ( cr, border_left + border_width, text_bottom+gap+gap );
                cairo_stroke (cr);
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
            {
                draw_contour_draw_rect ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );

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
                draw_symbol_draw_component ( &((*this_).draw_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
            {
                draw_contour_draw_rect ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );

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
                draw_symbol_draw_artifact ( &((*this_).draw_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_UML_STATE:
            case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
            {
                draw_contour_draw_rounded_rect ( &((*this_).draw_contour), classifier_bounds, false, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
            {
                draw_contour_draw_rounded_rect ( &((*this_).draw_contour), classifier_bounds, true, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
            {
                draw_contour_draw_ellipse ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_NODE:
            {
                draw_contour_draw_3d_box ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTOR:
            {
                const double actor_height = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_actor_bounds ( &((*this_).draw_symbol),
                                                             border_left + half_width,
                                                             border_top,
                                                             GEOMETRY_H_ALIGN_CENTER,
                                                             GEOMETRY_V_ALIGN_TOP,
                                                             actor_height
                                                           );
                draw_symbol_draw_actor ( &((*this_).draw_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
            case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
            {
                const double circle_diameter = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_circle_bounds ( &((*this_).draw_symbol),
                                                              border_left + half_width,
                                                              border_top,
                                                              GEOMETRY_H_ALIGN_CENTER,
                                                              GEOMETRY_V_ALIGN_TOP,
                                                              circle_diameter
                                                            );
                const bool stroke = ( data_classifier_get_main_type ( classifier ) != DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE );
                const bool fill = ( ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE )
                                  || ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE ) );
                const bool shallow_history = ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY );
                const bool deep_history = ( data_classifier_get_main_type ( classifier ) == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY );
                draw_symbol_draw_circle ( &((*this_).draw_symbol),
                                          icon_bounds,
                                          pencil_size,
                                          stroke,
                                          fill,
                                          shallow_history,
                                          deep_history,
                                          cr
                                        );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
            {
                draw_contour_draw_accept_event ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
            {
                draw_contour_draw_send_signal ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
            {
                const double icon_height = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_time_bounds ( &((*this_).draw_symbol),
                                                            border_left + half_width,
                                                            border_top,
                                                            GEOMETRY_H_ALIGN_CENTER,
                                                            GEOMETRY_V_ALIGN_TOP,
                                                            icon_height
                                                          );
                draw_symbol_draw_time ( &((*this_).draw_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
            {
                const double icon_height = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_symbol_get_sync_bounds ( &((*this_).draw_symbol),
                                                            border_left + half_width,
                                                            border_top,
                                                            GEOMETRY_H_ALIGN_CENTER,
                                                            GEOMETRY_V_ALIGN_TOP,
                                                            icon_height,
                                                            pencil_size
                                                          );
                draw_symbol_draw_sync ( &((*this_).draw_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
            {
                draw_contour_draw_rhombus ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
            {
                draw_contour_draw_rect ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
            {
                draw_contour_draw_diagram_ref ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
            {
                draw_contour_draw_package ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMMENT:
            {
                draw_contour_draw_comment ( &((*this_).draw_contour), classifier_bounds, pencil_size, cr );
            }
            break;

            default:
            {
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_composer_draw()");
            }
            break;
        }

        /* highlight */
        if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_EMPHASIS ))
        {
            GdkRGBA emph_color = pencil_size_get_emphasized_color( pencil_size );
            cairo_set_source_rgba( cr, emph_color.red, emph_color.green, emph_color.blue, emph_color.alpha );
            const geometry_rectangle_t *box = layout_visible_classifier_get_label_box_ptr( layouted_classifier );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left(box),
                              geometry_rectangle_get_top(box),
                              geometry_rectangle_get_width(box),
                              geometry_rectangle_get_height(box)
                            );
            cairo_fill (cr);
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* draw label */
        draw_label_draw_stereotype_and_name( &((*this_).draw_label),
                                             visible_classifier,
                                             layout_visible_classifier_get_label_box_ptr( layouted_classifier ),
                                             pencil_size,
                                             font_layout,
                                             cr
                                           );

#if 0
        /* draw the rectangles */
        {
            const geometry_rectangle_t *classifier_bounds;
            classifier_bounds = layout_visible_classifier_get_bounds_ptr( layouted_classifier );
            const geometry_rectangle_t *classifier_space;
            classifier_space = layout_visible_classifier_get_space_ptr( layouted_classifier );
            const geometry_rectangle_t *classifier_label_box;
            classifier_label_box = layout_visible_classifier_get_label_box_ptr( layouted_classifier );

            cairo_set_source_rgba( cr, 1.0, 0.5, 0.6, 0.75 );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_bounds ),
                              geometry_rectangle_get_top ( classifier_bounds ),
                              geometry_rectangle_get_width ( classifier_bounds ),
                              geometry_rectangle_get_height ( classifier_bounds )
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
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }
#endif

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

static inline double MAX_OF_2( double a, double b ) { return ((a>b)?a:b); }
static inline double MAX_OF_3( double a, double b, double c ) { return ((a>b)?((a>c)?a:c):((b>c)?b:c)); }

void pencil_classifier_composer_set_all_bounds ( const pencil_classifier_composer_t *this_,
                                                const data_visible_classifier_t *visible_classifier,
                                                const pencil_size_t *pencil_size,
                                                PangoLayout *font_layout,
                                                const geometry_dimensions_t *proposed_bounds,
                                                const geometry_dimensions_t *minimum_feature_space,
                                                layout_visible_classifier_t *io_classifier_layout )
{
    TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != proposed_bounds );
    assert( NULL != minimum_feature_space );
    assert( NULL != io_classifier_layout );

    /* border sizes of the classifier-shape */
    double top_border;
    double left_border;
    double bottom_border;
    double right_border;

    /* stereotype and name dimensions */
    double text_height;
    double text_width;

    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    data_classifier_type_t classifier_type;
    classifier_type = data_classifier_get_main_type( classifier );

    TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

    /* determine border sizes of the classifier-shape */
    draw_contour_get_shape_border_dimensions( &((*this_).draw_contour),
                                               classifier_type,
                                               pencil_size,
                                               &top_border,
                                               &left_border,
                                               &bottom_border,
                                               &right_border
                                             );

    /* determine stereotype and name dimensions */
    draw_label_get_stereotype_and_name_dimensions( &((*this_).draw_label),
                                                   visible_classifier,
                                                   pencil_size,
                                                   font_layout,
                                                   &text_height,
                                                   &text_width
                                                 );

    /* get the bounds and inner space rectangles to modify */
    geometry_rectangle_t *out_classifier_bounds;
    out_classifier_bounds = layout_visible_classifier_get_bounds_ptr( io_classifier_layout );
    geometry_rectangle_t *out_classifier_space;
    out_classifier_space = layout_visible_classifier_get_space_ptr( io_classifier_layout );
    geometry_rectangle_t *out_classifier_label_box;
    out_classifier_label_box = layout_visible_classifier_get_label_box_ptr( io_classifier_layout );

    bool is_fix_sized_symbol;
    is_fix_sized_symbol = layout_visible_classifier_is_fix_sized_symbol( io_classifier_layout );

    /* adapt to the size of the contained features */
    if ( ! is_fix_sized_symbol )
    {
        const double width_by_text = left_border + text_width + right_border;
        const double width_by_space = left_border + geometry_dimensions_get_width( minimum_feature_space ) + right_border;
        const double width_by_proposal = geometry_dimensions_get_width( proposed_bounds );
        const double width = MAX_OF_3 ( width_by_text, width_by_space, width_by_proposal );

        const double height_by_text_and_space = top_border + text_height + geometry_dimensions_get_height( minimum_feature_space ) + bottom_border;
        const double height_by_proposal = geometry_dimensions_get_height( proposed_bounds );
        const double height = MAX_OF_2 ( height_by_text_and_space, height_by_proposal );

        geometry_rectangle_reinit( out_classifier_bounds, 0.0, 0.0, width, height );

        if ( DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE == classifier_type )
        {
            const geometry_v_align_t DECISION_V_ALIGN = GEOMETRY_V_ALIGN_CENTER;
            top_border = geometry_v_align_get_top( &DECISION_V_ALIGN,
                                                   text_height,
                                                   0.0,
                                                   height
                                                 );
        }

        const double space_width = width - left_border - right_border;
        const double space_height = height-top_border-text_height-bottom_border;

        geometry_rectangle_reinit( out_classifier_space, left_border, top_border+text_height, space_width, space_height );
        geometry_rectangle_reinit( out_classifier_label_box, (width-text_width)/2.0, top_border, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );
    }
    else
    {
        const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
        const double symbol_width = pencil_size_get_classifier_symbol_height( pencil_size );
        geometry_rectangle_reinit( out_classifier_bounds, 0.0, 0.0, symbol_width, symbol_height );
        geometry_rectangle_reinit( out_classifier_space, 0.0, symbol_height+text_height, symbol_width, 0.0 );
        geometry_rectangle_reinit( out_classifier_label_box, (symbol_width-text_width)/2.0, symbol_height, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );
    }
    TRACE_END();
}

void pencil_classifier_composer_set_space_and_label ( const pencil_classifier_composer_t *this_,
                                                     const data_visible_classifier_t *visible_classifier,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     layout_visible_classifier_t *io_classifier_layout )
{
    TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != io_classifier_layout );

    /* border sizes of the classifier-shape */
    double top_border;
    double left_border;
    double bottom_border;
    double right_border;

    /* stereotype and name dimensions */
    double text_height;
    double text_width;

    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    data_classifier_type_t classifier_type;
    classifier_type = data_classifier_get_main_type( classifier );

    TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

    /* determine border sizes of the classifier-shape */
    draw_contour_get_shape_border_dimensions( &((*this_).draw_contour),
                                               classifier_type,
                                               pencil_size,
                                               &top_border,
                                               &left_border,
                                               &bottom_border,
                                               &right_border
                                             );

    /* determine stereotype and name dimensions */
    draw_label_get_stereotype_and_name_dimensions( &((*this_).draw_label),
                                                   visible_classifier,
                                                   pencil_size,
                                                   font_layout,
                                                   &text_height,
                                                   &text_width
                                                 );

    /* get the bounds and inner space rectangles to modify */
    const geometry_rectangle_t *classifier_bounds;
    classifier_bounds = layout_visible_classifier_get_bounds_ptr( io_classifier_layout );
    geometry_rectangle_t *out_classifier_space;
    out_classifier_space = layout_visible_classifier_get_space_ptr( io_classifier_layout );
    geometry_rectangle_t *out_classifier_label_box;
    out_classifier_label_box = layout_visible_classifier_get_label_box_ptr( io_classifier_layout );

    bool is_fix_sized_symbol;
    is_fix_sized_symbol = layout_visible_classifier_is_fix_sized_symbol( io_classifier_layout );

    /* calculate the result */
    const double left = geometry_rectangle_get_left( classifier_bounds );
    const double top = geometry_rectangle_get_top( classifier_bounds );
    const double width = geometry_rectangle_get_width( classifier_bounds );
    const double space_left = left + left_border;
    const double space_width = width - left_border - right_border;
    if ( ! is_fix_sized_symbol )
    {
        if ( DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE == classifier_type )
        {
            const geometry_v_align_t DECISION_V_ALIGN = GEOMETRY_V_ALIGN_CENTER;
            top_border = geometry_v_align_get_top( &DECISION_V_ALIGN,
                                                   text_height,
                                                   0.0,
                                                   geometry_rectangle_get_height( classifier_bounds )
                                                 );
        }

        const double space_top = geometry_rectangle_get_top( classifier_bounds ) + top_border + text_height;
        const double space_height = geometry_rectangle_get_height( classifier_bounds ) - top_border - bottom_border - text_height;

        geometry_rectangle_reinit( out_classifier_space, space_left, space_top, space_width, space_height );

        geometry_rectangle_reinit( out_classifier_label_box, left+(width-text_width)/2.0, top+top_border, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );
    }
    else
    {
        const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );

        geometry_rectangle_reinit( out_classifier_space, space_left, symbol_height+text_height, space_width, 0.0 );
        geometry_rectangle_reinit( out_classifier_label_box, left+(width-text_width)/2.0, top+symbol_height, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );
    }
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
