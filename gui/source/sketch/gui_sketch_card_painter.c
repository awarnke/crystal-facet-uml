/* File: gui_sketch_card_painter.c; Copyright and License: see below */

#include "sketch/gui_sketch_card_painter.h"
#include "sketch/gui_sketch_snap_state.h"
#include "u8/u8_trace.h"
#include <gtk/gtk.h>
#include <assert.h>

void gui_sketch_card_painter_init( gui_sketch_card_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_sketch_style_init( &((*this_).sketch_style) );

    U8_TRACE_END();
}

void gui_sketch_card_painter_destroy( gui_sketch_card_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_sketch_style_destroy( &((*this_).sketch_style) );

    U8_TRACE_END();
}

void gui_sketch_card_painter_draw_overlay( gui_sketch_card_painter_t *this_,
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
            gui_sketch_card_painter_private_draw_edit_mode( this_, drag_state, card_under_mouse, cr );
        }
        break;

        case GUI_TOOL_CREATE:
        {
            const data_id_t highlighted = gui_marked_set_get_highlighted( marked_objects );
            const layout_subelement_kind_t highlighted_kind = gui_marked_set_get_highlighted_kind( marked_objects );
            gui_sketch_card_painter_private_draw_create_mode( this_,
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


void gui_sketch_card_painter_private_draw_edit_mode( gui_sketch_card_painter_t *this_,
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
                gui_sketch_card_painter_private_draw_grid( this_, card_under_mouse, cr );

                /* draw snap-to-grid indicator */
                const int32_t to_x = gui_sketch_drag_state_get_to_x ( drag_state );
                const int32_t to_y = gui_sketch_drag_state_get_to_y ( drag_state );
                const gui_sketch_snap_state_t snapped = gui_sketch_card_is_pos_on_grid ( card_under_mouse, to_x, to_y );
                gui_sketch_card_painter_private_draw_snap_indicator( this_, card_under_mouse, snapped, to_x, to_y, cr );
            }
        }
    }

    U8_TRACE_END();
}

void gui_sketch_card_painter_private_draw_create_mode( gui_sketch_card_painter_t *this_,
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
        gui_sketch_card_painter_private_draw_arrow( this_, from_x, from_y, to_x, to_y, cr );
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
                gui_sketch_card_painter_private_draw_grid( this_, card_under_mouse, cr );

                /* draw snap-to-grid indicator */
                const gui_sketch_snap_state_t snapped = gui_sketch_card_is_pos_on_grid( card_under_mouse, to_x, to_y );
                gui_sketch_card_painter_private_draw_snap_indicator( this_, card_under_mouse, snapped, to_x, to_y, cr );
            }

            /* draw boxes and arrow icons */
            if ( draw_new_classifier )
            {
                gui_sketch_card_painter_private_draw_new_classifier( this_, to_x, to_y, cr );
            }
            if ( draw_new_feature )
            {
                gui_sketch_card_painter_private_draw_new_feature( this_, to_x, to_y, cr );
            }
            if ( draw_new_relationship )
            {
                gui_sketch_card_painter_private_draw_new_relationship( this_, to_x, to_y, cr );
            }
        }
    }

    U8_TRACE_END();
}

void gui_sketch_card_painter_private_draw_arrow( gui_sketch_card_painter_t *this_,
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

void gui_sketch_card_painter_private_draw_new_classifier( gui_sketch_card_painter_t *this_,
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

void gui_sketch_card_painter_private_draw_new_feature( gui_sketch_card_painter_t *this_,
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

void gui_sketch_card_painter_private_draw_new_relationship( gui_sketch_card_painter_t *this_,
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

void gui_sketch_card_painter_private_draw_grid( gui_sketch_card_painter_t *this_,
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

void gui_sketch_card_painter_private_draw_snap_indicator( gui_sketch_card_painter_t *this_,
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

void gui_sketch_card_painter_private_draw_element_space( const gui_sketch_card_painter_t *this_,
                                                         const layout_subelement_id_t *subelement,
                                                         const layout_visible_set_t *layouted_set,
                                                         cairo_t *cr )
{
    assert( NULL != subelement );
    assert( NULL != layouted_set );
    assert( NULL != cr );
    const layout_diagram_t *const layout_diag = layout_visible_set_get_diagram_const( layouted_set );

    geometry_rectangle_t highlight;
    geometry_rectangle_init_empty( &highlight );
    data_id_t search_id;
    data_id_copy( &search_id, data_full_id_get_primary_id_const( layout_subelement_id_get_id_const( subelement ) ) );
    const layout_subelement_kind_t kind = layout_subelement_id_get_kind( subelement );

    /* check diagram */
    const data_diagram_t *const diag_data = layout_diagram_get_data_const( layout_diag );
    const data_id_t diag_id = data_diagram_get_data_id( diag_data );
    if ( data_id_equals( &search_id, &diag_id ) )
    {
        switch ( kind )
        {
            case LAYOUT_SUBELEMENT_KIND_LABEL:
            {
                geometry_rectangle_replace( &highlight, layout_diagram_get_label_box_const( layout_diag ) );
                geometry_rectangle_expand_4dir( &highlight, 4.0 /* delta_width */, 4.0 /* delta_height */ );
                gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
            }
            break;

            case LAYOUT_SUBELEMENT_KIND_OUTLINE:
            {
                geometry_rectangle_t empty_space;
                geometry_rectangle_replace( &highlight, layout_diagram_get_bounds_const( layout_diag ) );
                geometry_rectangle_copy( &empty_space, layout_diagram_get_draw_area_const( layout_diag ) );  /* no highlight in space */
                gui_sketch_card_painter_private_draw_border( this_, &highlight, &empty_space, cr );
            }
            break;

            default:
            case LAYOUT_SUBELEMENT_KIND_SPACE:
            {
                geometry_rectangle_replace( &highlight, layout_diagram_get_draw_area_const( layout_diag ) );
                gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
            }
            break;
        }
    }

    /* iterate over all classifiers */
    const uint32_t c_count = layout_visible_set_get_visible_classifier_count( layouted_set );
    for ( uint32_t c_index = 0; c_index < c_count; c_index ++ )
    {
        const layout_visible_classifier_t *const visible_classifier
            = layout_visible_set_get_visible_classifier_const ( layouted_set, c_index );
        const data_diagramelement_t *diagele = layout_visible_classifier_get_diagramelement_const( visible_classifier );
        const data_id_t diagele_id = data_diagramelement_get_data_id( diagele );
        if ( data_id_equals( &search_id, &diagele_id ) )
        {
            switch ( kind )
            {
                case LAYOUT_SUBELEMENT_KIND_LABEL:
                {
                    geometry_rectangle_replace( &highlight, layout_visible_classifier_get_label_box_const( visible_classifier ) );
                    geometry_rectangle_expand_4dir( &highlight, 4.0 /* delta_width */, 4.0 /* delta_height */ );
                    gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
                }
                break;

                case LAYOUT_SUBELEMENT_KIND_OUTLINE:
                {
                    geometry_rectangle_t empty_space;
                    geometry_rectangle_replace( &highlight, layout_visible_classifier_get_symbol_box_const( visible_classifier ) );
                    geometry_rectangle_copy( &empty_space, layout_visible_classifier_get_space_const( visible_classifier ) );  /* no highlight in space */
                    gui_sketch_card_painter_private_draw_border( this_, &highlight, &empty_space, cr );
                }
                break;

                default:
                case LAYOUT_SUBELEMENT_KIND_SPACE:
                {
                    geometry_rectangle_replace( &highlight, layout_visible_classifier_get_space_const( visible_classifier ) );
                    gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
                }
                break;
            }
        }
    }

    /* check all contained features */
    const uint32_t f_count = layout_visible_set_get_feature_count( layouted_set );
    for ( uint32_t f_index = 0; f_index < f_count; f_index ++ )
    {
        const layout_feature_t *const the_feature
            = layout_visible_set_get_feature_const ( layouted_set, f_index );
        const data_feature_t *feature = layout_feature_get_data_const( the_feature );
        const data_id_t feature_id = data_feature_get_data_id( feature );
        if ( data_id_equals( &search_id, &feature_id ) )
        {
            switch ( kind )
            {
                case LAYOUT_SUBELEMENT_KIND_LABEL:
                {
                    geometry_rectangle_replace( &highlight, layout_feature_get_label_box_const( the_feature ) );
                    geometry_rectangle_expand_4dir( &highlight, 4.0 /* delta_width */, 4.0 /* delta_height */ );
                    gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
                }
                break;

                default:
                case LAYOUT_SUBELEMENT_KIND_OUTLINE:
                {
                    geometry_rectangle_replace( &highlight, layout_feature_get_symbol_box_const( the_feature ) );
                    geometry_rectangle_expand_4dir( &highlight, 4.0 /* delta_width */, 4.0 /* delta_height */ );
                    gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
                }
                break;
            }
        }
    }

    const uint32_t count_relations = layout_visible_set_get_relationship_count ( layouted_set );
    for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
    {
        const layout_relationship_t *const the_relationship
            = layout_visible_set_get_relationship_const( layouted_set, rel_index );
        const data_relationship_t *const relationship = layout_relationship_get_data_const( the_relationship );
        const data_id_t relationship_id = data_relationship_get_data_id( relationship );
        if ( data_id_equals( &search_id, &relationship_id ) )
        {
            switch ( kind )
            {
                case LAYOUT_SUBELEMENT_KIND_LABEL:
                {
                    geometry_rectangle_replace( &highlight, layout_relationship_get_label_box_const( the_relationship ) );
                    geometry_rectangle_expand_4dir( &highlight, 4.0 /* delta_width */, 4.0 /* delta_height */ );
                    gui_sketch_card_painter_private_draw_rect( this_, &highlight, cr );
                }
                break;

                default:
                case LAYOUT_SUBELEMENT_KIND_OUTLINE:
                {
                    const geometry_connector_t *const relationship_shape
                        = layout_relationship_get_shape_const( the_relationship );
                    {
                        geometry_rectangle_t segment_src;
                        segment_src = geometry_connector_get_segment_bounds( relationship_shape,
                                                                             GEOMETRY_CONNECTOR_SEGMENT_SOURCE
                                                                           );
                        geometry_rectangle_expand_4dir( &segment_src, 6.0 /* delta_width */, 6.0 /* delta_height */ );
                        gui_sketch_card_painter_private_draw_rect( this_, &segment_src, cr );
                    }
                    {
                        geometry_rectangle_t segment_dst;
                        segment_dst = geometry_connector_get_segment_bounds( relationship_shape,
                                                                             GEOMETRY_CONNECTOR_SEGMENT_DESTINATION
                                                                           );
                        geometry_rectangle_expand_4dir( &segment_dst, 6.0 /* delta_width */, 6.0 /* delta_height */ );
                        gui_sketch_card_painter_private_draw_rect( this_, &segment_dst, cr );
                    }
                    {
                        geometry_rectangle_t segment_main;
                        segment_main = geometry_connector_get_segment_bounds( relationship_shape,
                                                                              GEOMETRY_CONNECTOR_SEGMENT_MAIN
                                                                            );
                        geometry_rectangle_expand_4dir( &segment_main, 6.0 /* delta_width */, 6.0 /* delta_height */ );
                        gui_sketch_card_painter_private_draw_rect( this_, &segment_main, cr );
                    }
                }
                break;
            }
        }
    }
}

void gui_sketch_card_painter_draw_paper( gui_sketch_card_painter_t *this_,
                                         gui_tool_t selected_tool,
                                         const gui_sketch_drag_state_t *drag_state,
                                         const gui_sketch_card_t *card,
                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != drag_state );
    assert( NULL != card );
    assert( NULL != cr );

    if ( gui_sketch_card_is_visible(card) )
    {
        const bool create_tool = ( selected_tool == GUI_TOOL_CREATE );

        shape_int_rectangle_t bounds = gui_sketch_card_get_bounds( card );
        const int32_t left = shape_int_rectangle_get_left( &bounds );
        const int32_t top = shape_int_rectangle_get_top( &bounds );
        const uint32_t width = shape_int_rectangle_get_width( &bounds );
        const uint32_t height = shape_int_rectangle_get_height( &bounds );

        /* draw paper */
        if ( create_tool )
        {
            static const double GRAY_R = 0.875;
            static const double GRAY_G = 0.875;
            static const double GRAY_B = 0.875;
            static const double GRAY_A = 1.0;
            cairo_set_source_rgba( cr, GRAY_R, GRAY_G, GRAY_B, GRAY_A );
        }
        else
        {
            static const double WHITE_R = 1.0;
            static const double WHITE_G = 1.0;
            static const double WHITE_B = 1.0;
            static const double WHITE_A = 1.0;
            cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );
        }
        cairo_rectangle ( cr, left, top, width, height );
        cairo_fill (cr);

        const layout_visible_set_t *const layout = gui_sketch_card_get_visible_set( card );
        if ( layout_visible_set_is_valid( layout ) && create_tool )
        {
            const int32_t mouse_x = gui_sketch_drag_state_get_to_x( drag_state );
            const int32_t mouse_y = gui_sketch_drag_state_get_to_y( drag_state );

            const layout_subelement_id_t subelement_to_highlight
                = gui_sketch_card_get_element_at_pos( card, mouse_x, mouse_y, !create_tool /* filter_lifelines */ );

            gui_sketch_card_painter_private_draw_element_space( this_, &subelement_to_highlight, layout, cr );
        }
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
