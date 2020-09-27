/* File: pencil_classifier_composer.c; Copyright and License: see below */

#include "pencil_classifier_composer.h"
#include "trace.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_classifier_composer_init( pencil_classifier_composer_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    data_rules_init ( &((*this_).data_rules) );
    draw_classifier_symbol_init( &((*this_).draw_classifier_symbol) );
    draw_classifier_label_init( &((*this_).draw_classifier_label) );
    draw_classifier_contour_init( &((*this_).draw_classifier_contour) );

    TRACE_END();
}

void pencil_classifier_composer_destroy( pencil_classifier_composer_t *this_ )
{
    TRACE_BEGIN();

    draw_classifier_symbol_destroy( &((*this_).draw_classifier_symbol) );
    draw_classifier_label_destroy( &((*this_).draw_classifier_label) );
    draw_classifier_contour_destroy( &((*this_).draw_classifier_contour) );
    data_rules_destroy ( &((*this_).data_rules) );
    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_composer_draw ( const pencil_classifier_composer_t *this_,
                                       layout_visible_classifier_t *layouted_classifier,
                                       data_id_t mark_focused,
                                       data_id_t mark_highlighted,
                                       const data_small_set_t *mark_selected,
                                       pencil_layout_data_t *layout_data,
                                       const pencil_size_t *pencil_size,
                                       PangoLayout *font_layout,
                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_classifier );
    assert( NULL != layout_data );
    assert( NULL != font_layout );
    assert( NULL != cr );

    const data_visible_classifier_t *visible_classifier;
    visible_classifier = layout_visible_classifier_get_data_const( layouted_classifier );
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
    const data_classifier_t *classifier;
    const data_diagramelement_t *diagramelement;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );

    const double left = geometry_rectangle_get_left ( classifier_symbol_box );
    const double top = geometry_rectangle_get_top ( classifier_symbol_box );
    const double width = geometry_rectangle_get_width ( classifier_symbol_box );
    //const double height = geometry_rectangle_get_height ( classifier_symbol_box );
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* draw id */
    draw_classifier_label_draw_id( &((*this_).draw_classifier_label),
                        visible_classifier,
                        classifier_symbol_box,
                        pencil_size,
                        font_layout,
                        cr
                      );

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
            else if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT ))
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
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
        draw_classifier_label_draw_stereotype_and_name( &((*this_).draw_classifier_label),
                                             visible_classifier,
                                             layout_visible_classifier_get_label_box_ptr( layouted_classifier ),
                                             pencil_size,
                                             font_layout,
                                             cr
                                           );

        /* draw rectangle */
        const int border_left = left + gap;
        const int border_top = top + gap;
        const int border_width = width - 2.0 * gap;
        //const int border_height = height - 2.0 * gap;
        switch ( classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_FEATURE:
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_UML_PART:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_UML_CLASS:
            case DATA_CLASSIFIER_TYPE_UML_OBJECT:
            case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
                pencil_classifier_composer_private_draw_feature_compartments ( this_,
                                                                               layouted_classifier,
                                                                               layout_data,
                                                                               pencil_size,
                                                                               cr
                                                                             );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );

                /* draw icon */
                double icon_height = pencil_size_get_title_font_size( pencil_size );
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_classifier_symbol_get_component_bounds( &((*this_).draw_classifier_symbol),
                                                                border_left + border_width - gap,  /* x */
                                                                border_top + gap,  /* y */
                                                                GEOMETRY_H_ALIGN_RIGHT,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                icon_height
                                                              );
                draw_classifier_symbol_draw_component ( &((*this_).draw_classifier_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );

                /* draw icon */
                double icon_height = pencil_size_get_title_font_size( pencil_size );
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_classifier_symbol_get_artifact_bounds ( &((*this_).draw_classifier_symbol),
                                                                border_left + border_width - gap,  /* x */
                                                                border_top + gap,  /* y */
                                                                GEOMETRY_H_ALIGN_RIGHT,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                icon_height
                                                              );
                draw_classifier_symbol_draw_artifact ( &((*this_).draw_classifier_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_UML_STATE:
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

            case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
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

            case DATA_CLASSIFIER_TYPE_UML_NODE:
            {
                draw_classifier_contour_draw_3d_box ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTOR:
            {
                const double actor_height = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_classifier_symbol_get_actor_bounds ( &((*this_).draw_classifier_symbol),
                                                             border_left + half_width,
                                                             border_top,
                                                             GEOMETRY_H_ALIGN_CENTER,
                                                             GEOMETRY_V_ALIGN_TOP,
                                                             actor_height
                                                           );
                draw_classifier_symbol_draw_actor ( &((*this_).draw_classifier_symbol), icon_bounds, cr );
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
                icon_bounds = draw_classifier_symbol_get_circle_bounds ( &((*this_).draw_classifier_symbol),
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
                draw_classifier_symbol_draw_circle ( &((*this_).draw_classifier_symbol),
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
                const double icon_height = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_classifier_symbol_get_time_bounds ( &((*this_).draw_classifier_symbol),
                                                            border_left + half_width,
                                                            border_top,
                                                            GEOMETRY_H_ALIGN_CENTER,
                                                            GEOMETRY_V_ALIGN_TOP,
                                                            icon_height
                                                          );
                draw_classifier_symbol_draw_time ( &((*this_).draw_classifier_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
            {
                const double icon_height = pencil_size_get_classifier_symbol_height( pencil_size );
                const double half_width = 0.5 * border_width;
                geometry_rectangle_t icon_bounds;
                icon_bounds = draw_classifier_symbol_get_sync_bounds ( &((*this_).draw_classifier_symbol),
                                                            border_left + half_width,
                                                            border_top,
                                                            GEOMETRY_H_ALIGN_CENTER,
                                                            GEOMETRY_V_ALIGN_TOP,
                                                            icon_height,
                                                            pencil_size
                                                          );
                draw_classifier_symbol_draw_sync ( &((*this_).draw_classifier_symbol), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
            {
                draw_classifier_contour_draw_rhombus ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
            {
                draw_classifier_contour_draw_diagram_ref ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
            {
                draw_classifier_contour_draw_package ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMMENT:
            {
                draw_classifier_contour_draw_comment ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            default:
            {
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_composer_draw()");
            }
            break;
        }

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
        /* draw the rectangles */
        {
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_ptr( layouted_classifier );
            const geometry_rectangle_t *const classifier_label_box
                = layout_visible_classifier_get_label_box_ptr( layouted_classifier );

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

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ) )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *classifier_symbol_box, cr );
        }

        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ) )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *classifier_symbol_box, cr );
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
    double text_width;
    double text_height;

    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    data_classifier_type_t classifier_type;
    classifier_type = data_classifier_get_main_type( classifier );

    TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

    /* determine border sizes of the classifier-shape */
    draw_classifier_contour_get_shape_border_dimensions( &((*this_).draw_classifier_contour),
                                                         classifier_type,
                                                         pencil_size,
                                                         &top_border,
                                                         &left_border,
                                                         &bottom_border,
                                                         &right_border
                                                       );

    /* determine stereotype and name dimensions */
    draw_classifier_label_get_stereotype_and_name_dimensions( &((*this_).draw_classifier_label),
                                                              visible_classifier,
                                                              proposed_bounds,
                                                              pencil_size,
                                                              font_layout,
                                                              &text_width,
                                                              &text_height
                                                            );

    /* get the symbol and label boxes and inner space rectangles to modify */
    geometry_rectangle_t *out_classifier_symbol_box;
    out_classifier_symbol_box = layout_visible_classifier_get_symbol_box_ptr( io_classifier_layout );
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

        geometry_rectangle_reinit( out_classifier_symbol_box, 0.0, 0.0, width, height );

        /* update the borders based on the text dimensions */
        if ( DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE == classifier_type )
        {
            const geometry_v_align_t DECISION_V_ALIGN = GEOMETRY_V_ALIGN_CENTER;
            top_border = geometry_v_align_get_top( &DECISION_V_ALIGN,
                                                   text_height,
                                                   0.0,
                                                   height
                                                 );
            bottom_border = top_border;
        }

        /* calculate label_box */
        static const geometry_h_align_t TEXT_H_ALIGN = GEOMETRY_H_ALIGN_CENTER;
        const double text_left = geometry_h_align_get_left( &TEXT_H_ALIGN,
                                                            text_width,
                                                            left_border,
                                                            width - left_border - right_border
                                                          );
        geometry_rectangle_reinit( out_classifier_label_box, text_left, top_border, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        const double space_width = width - left_border - right_border;
        const double space_height = height-top_border-text_height-bottom_border;
        geometry_rectangle_reinit( out_classifier_space, left_border, top_border+text_height, space_width, space_height );
    }
    else
    {
        const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
        const double symbol_width = pencil_size_get_classifier_symbol_height( pencil_size );

        /* calculate bounds */
        geometry_rectangle_reinit( out_classifier_symbol_box, 0.0, 0.0, symbol_width, symbol_height );

        /* calculate label_box */
        geometry_rectangle_reinit( out_classifier_label_box, (symbol_width-text_width)/2.0, symbol_height, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        geometry_rectangle_reinit( out_classifier_space, 0.0, symbol_height+text_height, symbol_width, 0.0 );
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
    double text_width;
    double text_height;

    /* name the input data */
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    data_classifier_type_t classifier_type;
    classifier_type = data_classifier_get_main_type( classifier );

    /* get the symbol and label box and inner space rectangles to modify */
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( io_classifier_layout );
    geometry_rectangle_t *out_classifier_space;
    out_classifier_space = layout_visible_classifier_get_space_ptr( io_classifier_layout );
    geometry_rectangle_t *out_classifier_label_box;
    out_classifier_label_box = layout_visible_classifier_get_label_box_ptr( io_classifier_layout );

    TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

    /* determine border sizes of the classifier-shape */
    draw_classifier_contour_get_shape_border_dimensions( &((*this_).draw_classifier_contour),
                                               classifier_type,
                                               pencil_size,
                                               &top_border,
                                               &left_border,
                                               &bottom_border,
                                               &right_border
                                             );

    /* determine stereotype and name dimensions */
    geometry_dimensions_t proposed_bounds;
    geometry_dimensions_init( &proposed_bounds, geometry_rectangle_get_width( classifier_symbol_box ), geometry_rectangle_get_height( classifier_symbol_box ) );
    draw_classifier_label_get_stereotype_and_name_dimensions( &((*this_).draw_classifier_label),
                                                   visible_classifier,
                                                   &proposed_bounds,
                                                   pencil_size,
                                                   font_layout,
                                                   &text_width,
                                                   &text_height
                                                 );

    /* calculate the result */
    const double left = geometry_rectangle_get_left( classifier_symbol_box );
    const double top = geometry_rectangle_get_top( classifier_symbol_box );
    const double width = geometry_rectangle_get_width( classifier_symbol_box );
    const double height = geometry_rectangle_get_height( classifier_symbol_box );
    const double space_left = left + left_border;
    const double space_width = width - left_border - right_border;

    bool is_fix_sized_symbol;
    is_fix_sized_symbol = layout_visible_classifier_is_fix_sized_symbol( io_classifier_layout );
    if ( ! is_fix_sized_symbol )
    {
        /* update the borders based on the text dimensions */
        if ( DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE == classifier_type )
        {
            const geometry_v_align_t DECISION_V_ALIGN = GEOMETRY_V_ALIGN_CENTER;
            top_border = geometry_v_align_get_top( &DECISION_V_ALIGN,
                                                   text_height,
                                                   0.0,
                                                   geometry_rectangle_get_height( classifier_symbol_box )
                                                 );
            bottom_border = top_border;
        }

        /* calculate label_box */
        static const geometry_h_align_t TEXT_H_ALIGN = GEOMETRY_H_ALIGN_CENTER;
        const double text_left = geometry_h_align_get_left( &TEXT_H_ALIGN,
                                                            text_width,
                                                            left + left_border,
                                                            width - left_border - right_border
                                                          );
        geometry_rectangle_reinit( out_classifier_label_box, text_left, top+top_border, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        const double space_top = geometry_rectangle_get_top( classifier_symbol_box ) + top_border + text_height;
        const double space_height = geometry_rectangle_get_height( classifier_symbol_box ) - top_border - bottom_border - text_height;
        geometry_rectangle_reinit( out_classifier_space, space_left, space_top, space_width, space_height );

    }
    else
    {
        const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );

        /* calculate label_box */
        const double label_top = ( symbol_height + text_height > height ) ? ( top + height - text_height ) : ( top + symbol_height );
        geometry_rectangle_reinit( out_classifier_label_box, left+(width-text_width)/2.0, label_top, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        geometry_rectangle_reinit( out_classifier_space, space_left, symbol_height+text_height, space_width, 0.0 );
    }
    TRACE_END();
}

void pencil_classifier_composer_private_draw_feature_compartments ( const pencil_classifier_composer_t *this_,
                                                                    layout_visible_classifier_t *layouted_classifier,
                                                                    pencil_layout_data_t *layout_data,
                                                                    const pencil_size_t *pencil_size,
                                                                    cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != layouted_classifier );
    assert( NULL != layout_data );
    assert( NULL != pencil_size );
    assert( NULL != cr );

    /* determine number of properties and operations */
    uint32_t count_properties = 0;
    uint32_t count_operations = 0;
    {
        /* define names for input data */
        const data_row_id_t diagele_id = layout_visible_classifier_get_diagramelement_id ( layouted_classifier );

        const uint32_t num_features = pencil_layout_data_get_feature_count ( layout_data );
        for ( uint32_t f_probe_idx = 0; f_probe_idx < num_features; f_probe_idx ++ )
        {
            const layout_feature_t *f_probe_layout;
            f_probe_layout = pencil_layout_data_get_feature_ptr ( layout_data, f_probe_idx );
            assert ( NULL != f_probe_layout );
            const layout_visible_classifier_t *probe_vis_classfy;
            probe_vis_classfy = layout_feature_get_classifier_const ( f_probe_layout );
            assert ( NULL != probe_vis_classfy );

            /* check if this f_probe_layout has the same diagram element id as the_feature */
            if ( diagele_id == layout_visible_classifier_get_diagramelement_id ( probe_vis_classfy ) )
            {
                /* this is a feature of the current layouted_classifier */
                /* define names for input data */
                const data_feature_t *f_probe_data;
                f_probe_data = layout_feature_get_data_const ( f_probe_layout );
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
            }
        }
    }

    /* draw compartments if there are features */
    if (( count_properties != 0 )||( count_operations != 0 ))
    {
        /* define names for input data */
        const geometry_rectangle_t *const classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
        const geometry_rectangle_t *classifier_space;
        classifier_space = layout_visible_classifier_get_space_ptr( layouted_classifier );
        const double feature_height = pencil_size_get_standard_font_size( pencil_size )
            + pencil_size_get_font_line_gap( pencil_size );
        const double gap = pencil_size_get_standard_object_border( pencil_size );
        const double y_coordinate_1 = geometry_rectangle_get_top( classifier_space );
        const double y_coordinate_2 = geometry_rectangle_get_top( classifier_space )
            + ( count_properties * feature_height ) + ( 2.0 * gap );


        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_1,
                                                        pencil_size,
                                                        cr
                                                      );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_2,
                                                        pencil_size,
                                                        cr
                                                      );
        /*
        const double y_coordinate_3 = geometry_rectangle_get_top( classifier_space )
            + ( (count_properties+count_operations) * feature_height ) + ( 4.0 * gap );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_3,
                                                        pencil_size,
                                                        cr
                                                      );
        */
    }

    TRACE_END();
}

/*
Copyright 2016-2020 Andreas Warnke
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
