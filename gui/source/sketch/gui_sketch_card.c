/* File: gui_sketch_card.c; Copyright and License: see below */

#include "sketch/gui_sketch_card.h"
#include "pencil_diagram_maker.h"
#include "geometry/geometry_rectangle.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <gdk/gdk.h>

void gui_sketch_card_init( gui_sketch_card_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).visible = false;
    (*this_).dirty_elements_layout = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );
    data_visible_set_init( &((*this_).painter_input_data) );
    data_profile_part_init( &((*this_).profile) );
    pencil_diagram_maker_init( &((*this_).painter), &((*this_).painter_input_data), &((*this_).profile) );
    gui_sketch_marker_init( &((*this_).sketch_marker), false );

    U8_TRACE_END();
}

void gui_sketch_card_destroy( gui_sketch_card_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_sketch_marker_destroy( &((*this_).sketch_marker) );
    pencil_diagram_maker_destroy( &((*this_).painter) );
    data_profile_part_destroy( &((*this_).profile) );
    data_visible_set_destroy( &((*this_).painter_input_data) );
    shape_int_rectangle_destroy(&((*this_).bounds));

    U8_TRACE_END();
}

gui_sketch_location_thing_t gui_sketch_card_get_element_at_pos( const gui_sketch_card_t *this_,
                                                                int32_t x,
                                                                int32_t y,
                                                                bool filter_lifelines )
{
    U8_TRACE_BEGIN();
    static const int32_t snap_distance = 3;  /* snap_distance is the maximum distance to the next connector line when to select the connector */
    gui_sketch_location_thing_t result;
    gui_sketch_location_thing_init_void( &result );

    const layout_visible_set_t *const layout
        = pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
    const layout_diagram_t *const the_diagram
        = layout_visible_set_get_diagram_const( layout );
    const data_diagram_t *const diagram_data
        = layout_diagram_get_data_const ( the_diagram );

    /* get bounding box */
    const geometry_rectangle_t *diagram_bounds;
    diagram_bounds = layout_diagram_get_bounds_const( the_diagram );

    const bool pos_on_bounds
        = geometry_rectangle_contains( diagram_bounds, (double) x, (double) y );
    if ( pos_on_bounds )
    {
        /* check the relationship shapes */
        {
            result = gui_sketch_card_private_get_relationship_at_pos( this_,
                                                                      x,
                                                                      y,
                                                                      snap_distance
                                                                    );
        }

        /* determine a feature at the given position */
        if ( ! gui_sketch_location_thing_is_valid( &result ) )
        {
            result = gui_sketch_card_private_get_feature_at_pos( this_,
                                                                 x,
                                                                 y,
                                                                 filter_lifelines
                                                               );
        }

        /* determine a classifier at the given position */
        if ( ! gui_sketch_location_thing_is_valid( &result ) )
        {
            result = gui_sketch_card_private_get_classifier_at_pos( this_,
                                                                    x,
                                                                    y
                                                                  );
        }

        /* fallback: return the diagram */
        if ( ! gui_sketch_location_thing_is_valid( &result ) )
        {
            const geometry_rectangle_t *const diagram_space
                = layout_diagram_get_draw_area_const( the_diagram );
            const geometry_rectangle_t *const diagram_label
                = layout_diagram_get_label_box_const( the_diagram );
            const bool pos_on_space
                = geometry_rectangle_contains( diagram_space, (double) x, (double) y );
            const bool pos_on_label
                = geometry_rectangle_contains( diagram_label, (double) x, (double) y );
            const data_id_t diagram_id = data_diagram_get_data_id( diagram_data );
            const gui_sketch_location_thing_kind_t kind
                = ( pos_on_space
                ? GUI_SKETCH_LOCATION_THING_KIND_SPACE
                : ( pos_on_label ? GUI_SKETCH_LOCATION_THING_KIND_LABEL : GUI_SKETCH_LOCATION_THING_KIND_OUTLINE ) );
            gui_sketch_location_thing_reinit_solo( &result, kind, &diagram_id );
        }
#if 0
        if ( ! data_full_id_is_valid( out_surrounding_id ) )
        {
            data_full_id_reinit_by_table_and_id ( out_surrounding_id,
                                                  DATA_TABLE_DIAGRAM,
                                                  data_diagram_get_row_id(diagram_data),
                                                  DATA_TABLE_VOID,
                                                  DATA_ROW_ID_VOID
                                                );
        }
#endif
    }
    else
    {
        U8_TRACE_INFO( "given location outside diagram or no diagram chosen" );
    }

    U8_TRACE_END();
    return result;
}

gui_sketch_location_thing_t gui_sketch_card_private_get_classifier_at_pos( const gui_sketch_card_t *this_,
                                                                           int32_t x,
                                                                           int32_t y )
{
    U8_TRACE_BEGIN();
    gui_sketch_location_thing_t result;
    gui_sketch_location_thing_init_void( &result );

    /* get draw area */
    const layout_visible_set_t *const layout = pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
    const layout_diagram_t *const the_diagram
        = layout_visible_set_get_diagram_const( layout );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( the_diagram );

    if ( geometry_rectangle_contains( diagram_draw_area, (double) x, (double) y ) )
    {
        /* iterate over all classifiers */
        const uint32_t count
            = layout_visible_set_get_visible_classifier_count ( layout );
        double surrounding_classifier_area = geometry_rectangle_get_area( diagram_draw_area );

        for ( uint32_t index = 0; index < count; index ++ )
        {
            const layout_visible_classifier_t *const visible_classifier
                = layout_visible_set_get_visible_classifier_const ( layout, index );
            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const ( visible_classifier );
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const ( visible_classifier );
            const geometry_rectangle_t *const classifier_label_box
                = layout_visible_classifier_get_label_box_const( visible_classifier );

            const bool pos_on_symbol
                = geometry_rectangle_contains( classifier_symbol_box, (double) x, (double) y );
            const bool pos_on_label
                = geometry_rectangle_contains( classifier_label_box, (double) x, (double) y );
            if ( pos_on_symbol || pos_on_label )
            {
                const bool pos_on_space
                    = geometry_rectangle_contains( classifier_space, (double) x, (double) y );
                if ( pos_on_space )
                {
                    /* surrounding classifier is found. select it if it is the smallest found area */
                    const double current_classifier_area = geometry_rectangle_get_area( classifier_space );
                    if ( current_classifier_area < surrounding_classifier_area )
                    {
                        surrounding_classifier_area = current_classifier_area;

                        data_full_id_t found_id;
                        data_full_id_init_by_table_and_id( &found_id,
                                                           DATA_TABLE_DIAGRAMELEMENT,
                                                           layout_visible_classifier_get_diagramelement_id( visible_classifier ),
                                                           DATA_TABLE_CLASSIFIER,
                                                           layout_visible_classifier_get_classifier_id( visible_classifier )
                                                         );
                        gui_sketch_location_thing_reinit( &result, GUI_SKETCH_LOCATION_THING_KIND_SPACE, &found_id );
                    }
                }
                else
                {
                    /* classifier is found */
                    data_full_id_t found_id;
                    data_full_id_init_by_table_and_id( &found_id,
                                                       DATA_TABLE_DIAGRAMELEMENT,
                                                       layout_visible_classifier_get_diagramelement_id( visible_classifier ),
                                                       DATA_TABLE_CLASSIFIER,
                                                       layout_visible_classifier_get_classifier_id( visible_classifier )
                                                     );
                    const gui_sketch_location_thing_kind_t kind
                        = ( pos_on_label ? GUI_SKETCH_LOCATION_THING_KIND_LABEL : GUI_SKETCH_LOCATION_THING_KIND_OUTLINE );
                    gui_sketch_location_thing_reinit( &result, kind, &found_id );
                }
            }
        }
    }

    U8_TRACE_END();
    return result;
}

gui_sketch_location_thing_t gui_sketch_card_private_get_feature_at_pos( const gui_sketch_card_t *this_,
                                                                        int32_t x,
                                                                        int32_t y,
                                                                        bool filter_lifelines )
{
    U8_TRACE_BEGIN();
    gui_sketch_location_thing_t result;
    gui_sketch_location_thing_init_void( &result );

    /* check all contained features */
    const layout_visible_set_t *const layout = pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
    const uint32_t f_count = layout_visible_set_get_feature_count( layout );
    for ( uint32_t f_idx = 0; f_idx < f_count; f_idx ++ )
    {
        const layout_feature_t *const the_feature
            = layout_visible_set_get_feature_const ( layout, f_idx );
        const geometry_rectangle_t *const feature_symbol_box
            = layout_feature_get_symbol_box_const ( the_feature );
        const geometry_rectangle_t *const feature_label_box
            = layout_feature_get_label_box_const( the_feature );

        const bool pos_on_symbol
            = geometry_rectangle_contains( feature_symbol_box, (double) x, (double) y );
        const bool pos_on_label
            = geometry_rectangle_contains( feature_label_box, (double) x, (double) y );
        if ( pos_on_symbol || pos_on_label )
        {
            /* feature is found */
            const data_feature_t *const data_feature
                = layout_feature_get_data_const ( the_feature );
            const layout_visible_classifier_t *const layout_classifier
                = layout_feature_get_classifier_const ( the_feature );
            data_full_id_t found_id;
            if (( filter_lifelines )
                &&( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( data_feature ) ))
            {
                data_full_id_init_by_table_and_id( &found_id,
                                                   DATA_TABLE_DIAGRAMELEMENT,
                                                   layout_visible_classifier_get_diagramelement_id( layout_classifier ),
                                                   DATA_TABLE_CLASSIFIER,
                                                   layout_visible_classifier_get_classifier_id( layout_classifier )
                                                 );
            }
            else
            {
                data_full_id_init_by_table_and_id( &found_id,
                                                   DATA_TABLE_FEATURE,
                                                   layout_feature_get_feature_id( the_feature ),  /* feature or lifeline */
                                                   DATA_TABLE_CLASSIFIER,
                                                   data_feature_get_classifier_row_id( data_feature )
                                                 );
            }

            const gui_sketch_location_thing_kind_t kind
                = ( pos_on_label ? GUI_SKETCH_LOCATION_THING_KIND_LABEL : GUI_SKETCH_LOCATION_THING_KIND_OUTLINE );
            gui_sketch_location_thing_reinit( &result, kind, &found_id );
#if 0
            data_full_id_reinit_by_table_and_id ( out_surrounding_id,
                                                  DATA_TABLE_DIAGRAMELEMENT,
                                                  layout_visible_classifier_get_diagramelement_id( layout_classifier ),
                                                  DATA_TABLE_CLASSIFIER,
                                                  layout_visible_classifier_get_classifier_id( layout_classifier )
                                                );
#endif
        }
    }

    U8_TRACE_END();
    return result;
}

gui_sketch_location_thing_t gui_sketch_card_private_get_relationship_at_pos( const gui_sketch_card_t *this_,
                                                                             int32_t x,
                                                                             int32_t y,
                                                                             int32_t snap_distance )
{
    U8_TRACE_BEGIN();
    gui_sketch_location_thing_t result;
    gui_sketch_location_thing_init_void( &result );

    const layout_visible_set_t *const layout = pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
    const uint32_t count_relations
        = layout_visible_set_get_relationship_count ( layout );
    uint32_t matching_relations_found = 0;
    for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
    {
        const layout_relationship_t *const the_relationship
            = layout_visible_set_get_relationship_const( layout, rel_index );
        const geometry_connector_t *const relationship_shape
            = layout_relationship_get_shape_const( the_relationship );
        const geometry_rectangle_t *const rel_label_box
            = layout_relationship_get_label_box_const( the_relationship );

        const bool pos_on_shape
            = geometry_connector_is_close( relationship_shape, (double) x, (double) y, (double) snap_distance );
        const bool pos_on_label
            = geometry_rectangle_contains( rel_label_box, (double) x, (double) y );
        if ( pos_on_shape || pos_on_label )
        {
            /* ensure that every relation at that location can be selected by small mouse movements */
            if ( ((uint32_t)(x+y))%(matching_relations_found+1) == 0 )
            {
                const data_relationship_t *const relation_data
                    = layout_relationship_get_data_const( the_relationship );
                const data_id_t relation_id = data_relationship_get_data_id( relation_data );
                const gui_sketch_location_thing_kind_t kind
                    = ( pos_on_label ? GUI_SKETCH_LOCATION_THING_KIND_LABEL : GUI_SKETCH_LOCATION_THING_KIND_OUTLINE );
                gui_sketch_location_thing_reinit_solo( &result, kind, &relation_id );
            }
            matching_relations_found ++;
        }
    }

    U8_TRACE_END();
    return result;
}

static const double WHITE_R = 1.0;
static const double WHITE_G = 1.0;
static const double WHITE_B = 1.0;
static const double WHITE_A = 1.0;

void gui_sketch_card_private_draw_location_space( const gui_sketch_card_t *this_, const gui_sketch_location_thing_t *location, cairo_t *cr )
{
    const layout_visible_set_t *const layout = pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
    const layout_diagram_t *const layout_diag = layout_visible_set_get_diagram_const( layout );

    geometry_rectangle_t highlight;
    geometry_rectangle_init_empty( &highlight );
    data_id_t search_id;
    data_id_copy( &search_id, data_full_id_get_primary_id_const( gui_sketch_location_thing_get_id_const( location ) ) );
    const gui_sketch_location_thing_kind_t kind = gui_sketch_location_thing_get_kind( location );

    /* check diagram */
    const data_diagram_t *const diag_data = layout_diagram_get_data_const( layout_diag );
    //const data_diagram_type_t diag_type = data_diagram_get_diagram_type( diag_data );
    const data_id_t diag_id = data_diagram_get_data_id( diag_data );
    if ( data_id_equals( &search_id, &diag_id ) )
    {
        switch ( kind )
        {
            case GUI_SKETCH_LOCATION_THING_KIND_LABEL:
            {
                geometry_rectangle_replace( &highlight, layout_diagram_get_label_box_const( layout_diag ) );
                geometry_rectangle_expand_4dir( &highlight, 2.0 /* delta_width */, 2.0 /* delta_height */ );
            }
            break;

            case GUI_SKETCH_LOCATION_THING_KIND_OUTLINE:
            {
                geometry_rectangle_replace( &highlight, layout_diagram_get_bounds_const( layout_diag ) );
            }
            break;

            default:
            case GUI_SKETCH_LOCATION_THING_KIND_SPACE:
            {
                geometry_rectangle_replace( &highlight, layout_diagram_get_draw_area_const( layout_diag ) );
            }
            break;
        }
    }

    /* iterate over all classifiers */
    const uint32_t c_count = layout_visible_set_get_visible_classifier_count( layout );
    for ( uint32_t c_index = 0; c_index < c_count; c_index ++ )
    {
        const layout_visible_classifier_t *const visible_classifier
            = layout_visible_set_get_visible_classifier_const ( layout, c_index );
        //const data_classifier_t *classifier = layout_visible_classifier_get_classifier_const( visible_classifier );
        const data_diagramelement_t *diagele = layout_visible_classifier_get_diagramelement_const( visible_classifier );
        //const data_id_t classifier_id = data_classifier_get_data_id( classifier );
        const data_id_t diagele_id = data_diagramelement_get_data_id( diagele );
        if ( data_id_equals( &search_id, &diagele_id ) )
        {
            switch ( kind )
            {
                case GUI_SKETCH_LOCATION_THING_KIND_LABEL:
                {
                    geometry_rectangle_replace( &highlight, layout_visible_classifier_get_label_box_const( visible_classifier ) );
                    geometry_rectangle_expand_4dir( &highlight, 2.0 /* delta_width */, 2.0 /* delta_height */ );
                }
                break;

                case GUI_SKETCH_LOCATION_THING_KIND_OUTLINE:
                {
                    geometry_rectangle_replace( &highlight, layout_visible_classifier_get_symbol_box_const( visible_classifier ) );
                }
                break;

                default:
                case GUI_SKETCH_LOCATION_THING_KIND_SPACE:
                {
                    geometry_rectangle_replace( &highlight, layout_visible_classifier_get_space_const( visible_classifier ) );
                }
                break;
            }
        }
    }

    /* check all contained features */
    const uint32_t f_count = layout_visible_set_get_feature_count( layout );
    for ( uint32_t f_index = 0; f_index < f_count; f_index ++ )
    {
        const layout_feature_t *const the_feature
            = layout_visible_set_get_feature_const ( layout, f_index );
        const data_feature_t *feature = layout_feature_get_data_const( the_feature );
        const data_id_t feature_id = data_feature_get_data_id( feature );
        if ( data_id_equals( &search_id, &feature_id ) )
        {
            //const geometry_rectangle_t *const feature_symbol_box
            //    = layout_feature_get_symbol_box_const ( the_feature );
            geometry_rectangle_replace( &highlight, layout_feature_get_label_box_const( the_feature ) );
            switch ( kind )
            {
                case GUI_SKETCH_LOCATION_THING_KIND_LABEL:
                {
                    geometry_rectangle_replace( &highlight, layout_feature_get_label_box_const( the_feature ) );
                    geometry_rectangle_expand_4dir( &highlight, 2.0 /* delta_width */, 2.0 /* delta_height */ );
                }
                break;

                default:
                case GUI_SKETCH_LOCATION_THING_KIND_OUTLINE:
                {
                    geometry_rectangle_replace( &highlight, layout_feature_get_symbol_box_const( the_feature ) );
                    geometry_rectangle_expand_4dir( &highlight, 2.0 /* delta_width */, 2.0 /* delta_height */ );
                }
                break;
            }
        }
    }

    const uint32_t count_relations = layout_visible_set_get_relationship_count ( layout );
    for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
    {
        const layout_relationship_t *const the_relationship
            = layout_visible_set_get_relationship_const( layout, rel_index );
        const data_relationship_t *const relationship = layout_relationship_get_data_const( the_relationship );
        const data_id_t relationship_id = data_relationship_get_data_id( relationship );
        if ( data_id_equals( &search_id, &relationship_id ) )
        {
            geometry_rectangle_replace( &highlight, layout_relationship_get_label_box_const( the_relationship ) );
            switch ( kind )
            {
                case GUI_SKETCH_LOCATION_THING_KIND_LABEL:
                {
                    geometry_rectangle_replace( &highlight, layout_relationship_get_label_box_const( the_relationship ) );
                    geometry_rectangle_expand_4dir( &highlight, 2.0 /* delta_width */, 2.0 /* delta_height */ );
                }
                break;

                default:
                case GUI_SKETCH_LOCATION_THING_KIND_OUTLINE:
                {
                    const geometry_connector_t *const relationship_shape
                        = layout_relationship_get_shape_const( the_relationship );
                    {
                        geometry_rectangle_t segment_src;
                        segment_src = geometry_connector_get_segment_bounds( relationship_shape,
                                                                             GEOMETRY_CONNECTOR_SEGMENT_SOURCE
                                                                           );
                        geometry_rectangle_expand_4dir( &segment_src, 6.0 /* delta_width */, 6.0 /* delta_height */ );

                        cairo_set_source_rgba( cr, 0.8, 0.8, 0.8, WHITE_A );
                        cairo_rectangle( cr,
                                        geometry_rectangle_get_left( &segment_src ),
                                        geometry_rectangle_get_top( &segment_src ),
                                        geometry_rectangle_get_width( &segment_src ),
                                        geometry_rectangle_get_height( &segment_src )
                                    );
                        cairo_fill (cr);
                    }
                    {
                        geometry_rectangle_t segment_dst;
                        segment_dst = geometry_connector_get_segment_bounds( relationship_shape,
                                                                             GEOMETRY_CONNECTOR_SEGMENT_DESTINATION
                                                                           );
                        geometry_rectangle_expand_4dir( &segment_dst, 6.0 /* delta_width */, 6.0 /* delta_height */ );

                        cairo_set_source_rgba( cr, 0.8, 0.8, 0.8, WHITE_A );
                        cairo_rectangle( cr,
                                        geometry_rectangle_get_left( &segment_dst ),
                                        geometry_rectangle_get_top( &segment_dst ),
                                        geometry_rectangle_get_width( &segment_dst ),
                                        geometry_rectangle_get_height( &segment_dst )
                                    );
                        cairo_fill (cr);
                    }
                    {
                        highlight = geometry_connector_get_segment_bounds( relationship_shape,
                                                                           GEOMETRY_CONNECTOR_SEGMENT_MAIN
                                                                         );
                        geometry_rectangle_expand_4dir( &highlight, 6.0 /* delta_width */, 6.0 /* delta_height */ );
                    }
                }
                break;
            }
        }
    }

    /* draw whatever has been found */
    {
        cairo_set_source_rgba( cr, 0.8, 0.8, 0.8, WHITE_A );
        cairo_rectangle( cr,
                         geometry_rectangle_get_left( &highlight ),
                         geometry_rectangle_get_top( &highlight ),
                         geometry_rectangle_get_width( &highlight ),
                         geometry_rectangle_get_height( &highlight )
                       );
        cairo_fill (cr);
    }
}

void gui_sketch_card_draw_paper( gui_sketch_card_t *this_,
                                 gui_tool_t selected_tool,
                                 const gui_sketch_drag_state_t *drag_state,
                                 cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        /* layout elements if necessary */
        if ( (*this_).dirty_elements_layout )
        {
            pencil_diagram_maker_layout_elements ( &((*this_).painter), NULL, cr );
            (*this_).dirty_elements_layout = false;
        }

        const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
        const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
        const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
        const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

        /* draw paper */
        cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );
        cairo_rectangle ( cr, left, top, width, height );
        cairo_fill (cr);

#ifndef NDEBUG
        /* draw warnings at overlaps in debug mode */
        pencil_diagram_maker_show_overlaps ( &((*this_).painter), NULL, cr );
#endif

        const bool create_tool = ( selected_tool == GUI_TOOL_CREATE );
        const layout_visible_set_t *const layout = pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
        if ( layout_visible_set_is_valid( layout ) && create_tool )
        {
            const int32_t mouse_x = gui_sketch_drag_state_get_to_x( drag_state );
            const int32_t mouse_y = gui_sketch_drag_state_get_to_y( drag_state );

            const gui_sketch_location_thing_t thing_to_highlight
                = gui_sketch_card_get_element_at_pos( this_, mouse_x, mouse_y, !create_tool /* filter_lifelines */ );

            gui_sketch_card_private_draw_location_space( this_, &thing_to_highlight, cr );
        }
    }

    U8_TRACE_END();
}

void gui_sketch_card_draw( gui_sketch_card_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        /* layout elements if necessary */
        if ( (*this_).dirty_elements_layout )
        {
            pencil_diagram_maker_layout_elements ( &((*this_).painter), NULL, cr );
            (*this_).dirty_elements_layout = false;
        }

        /* get marked ids */
        const data_id_t mark_focused = gui_marked_set_get_focused_obj( marker );
        const data_id_t mark_highlighted = gui_marked_set_get_highlighted( marker );
        const data_small_set_t *mark_selected_set = gui_marked_set_get_selected_set_const( marker );

        /* draw highlighting */
        const data_diagram_t *const diag = data_visible_set_get_diagram_const ( &((*this_).painter_input_data) );
        const data_id_t diag_id = data_diagram_get_data_id( diag );
        gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                        diag_id,
                                        marker,
                                        (*this_).bounds,
                                        cr
                                      );

        /* draw the current diagram */
        pencil_diagram_maker_draw ( &((*this_).painter),
                                    mark_focused,
                                    mark_highlighted,
                                    mark_selected_set,
                                    cr
                                  );
    }

    U8_TRACE_END();
}

void gui_sketch_card_move_object_to_order( gui_sketch_card_t *this_,
                                           data_id_t obj_id,
                                           const layout_order_t *order )
{
    U8_TRACE_BEGIN();
    assert( NULL != order );

    const data_table_t table = data_id_get_table ( &obj_id );
    const data_row_id_t row_id = data_id_get_row_id ( &obj_id );

    layout_order_type_t order_type = layout_order_get_order_type( order );
    switch ( order_type )
    {
        case PENCIL_LAYOUT_ORDER_TYPE_X_Y:
        {
            switch ( table )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    const int32_t x_order = layout_order_get_first( order );
                    const int32_t y_order = layout_order_get_second( order );

                    data_classifier_t *const move_me
                        = data_visible_set_get_classifier_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        U8_LOG_WARNING( "pencil input data does not contain the object to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_classifier_set_x_order( move_me, x_order );
                        data_classifier_set_y_order( move_me, y_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    U8_LOG_WARNING( "object to be x/y-moved has no x/y coordinates: feature" );
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    U8_LOG_WARNING( "object to be x/y-moved has no x/y coordinates: relationship" );
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    U8_LOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* pencil cannot move diagrams */
                    U8_LOG_WARNING( "object to be x/y-moved has unexpected type: diagram" );
                }
                break;

                default:
                {
                    U8_LOG_WARNING( "object to be x/y-moved has illegal type" );
                }
                break;
            }
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_LIST:
        {
            switch ( table )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    const int32_t list_order = layout_order_get_first( order );

                    data_classifier_t *const move_me = data_visible_set_get_classifier_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        U8_LOG_WARNING( "pencil input data does not contain the classifier to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_classifier_set_list_order( move_me, list_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    const int32_t list_order = layout_order_get_first( order );

                    data_feature_t *const move_me = data_visible_set_get_feature_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        U8_LOG_WARNING( "pencil input data does not contain the feature to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_feature_set_list_order( move_me, list_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    const int32_t list_order = layout_order_get_first( order );

                    data_relationship_t *const move_me = data_visible_set_get_relationship_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        U8_LOG_WARNING( "pencil input data does not contain the relationship to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_relationship_set_list_order( move_me, list_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    U8_LOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* pencil cannot move diagrams */
                    U8_LOG_WARNING( "object to be x/y-moved has unexpected type: diagram" );
                }
                break;

                default:
                {
                    U8_LOG_WARNING( "object to be x/y-moved has illegal type" );
                }
                break;
            }
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_NONE:
        {
            /* nothing to do */
            /* no error */
            U8_LOG_ANOMALY( "object to be moved has no movement data" );
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_OUT_OF_RANGE:
        default:
        {
            U8_LOG_WARNING( "object to be x/y-moved has illegal movement data" );
        }
        break;
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2024 Andreas Warnke

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
