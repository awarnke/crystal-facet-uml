/* File: pencil_feature_layouter.c; Copyright and License: see below */

#include "pencil_feature_layouter.h"
#include "layout/layout_quality.h"
#include "u8/u8_f64.h"
#include "u8/u8_trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_feature_layouter_init( pencil_feature_layouter_t *this_,
                                   layout_visible_set_t *layout_data,
                                   const data_profile_part_t *profile,
                                   const pencil_size_t *pencil_size )
{
    U8_TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != profile );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).profile = profile;
    (*this_).pencil_size = pencil_size;
    data_rules_init( &((*this_).rules) );
    (*this_).label_dimensions_initialized = false;
    pencil_feature_painter_init( &((*this_).feature_painter) );

    U8_TRACE_END();
}

void pencil_feature_layouter_reset( pencil_feature_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).label_dimensions_initialized = false;

    U8_TRACE_END();
}

void pencil_feature_layouter_destroy( pencil_feature_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    data_rules_destroy( &((*this_).rules) );
    (*this_).label_dimensions_initialized = false;
    pencil_feature_painter_destroy( &((*this_).feature_painter) );

    U8_TRACE_END();
}

void pencil_feature_layouter_do_layout ( pencil_feature_layouter_t *this_, PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) LAYOUT_VISIBLE_SET_MAX_FEATURES );

    /* establish precondition: precalculate the dimensions of labels */
    if ( ! (*this_).label_dimensions_initialized )
    {
        pencil_feature_layouter_private_init_label_dimensions( this_, font_layout );
    }
    /* get diagram draw area */
    const layout_diagram_t *const diagram_layout
        = layout_visible_set_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );
    const data_diagram_t *const diagram_data
        = layout_diagram_get_data_const ( diagram_layout );
    const data_diagram_type_t diag_type
        = data_diagram_get_diagram_type ( diagram_data );

    /* layout the unsorted features */
    const uint32_t count_features
        = layout_visible_set_get_feature_count ( (*this_).layout_data );
    for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
    {
        layout_feature_t *const feature_layout
            = layout_visible_set_get_feature_ptr ( (*this_).layout_data, f_idx );
        const data_feature_t *const the_feature
            = layout_feature_get_data_const ( feature_layout );
        const layout_visible_classifier_t *const layout_classifier
            = layout_feature_get_classifier_const ( feature_layout );
        const data_classifier_t *const classifier
            = layout_visible_classifier_get_classifier_const( layout_classifier );
        const data_classifier_type_t classifier_type
            = data_classifier_get_main_type( classifier );

        const geometry_rectangle_t *const c_symbol_box
            = layout_visible_classifier_get_symbol_box_const ( layout_classifier );
        const geometry_rectangle_t *const c_envelope_box
            = layout_visible_classifier_get_envelope_box_const ( layout_classifier );
        switch ( data_feature_get_main_type (the_feature) )
        {
            case DATA_FEATURE_TYPE_LIFELINE:
            {
                /* layout lifeline feature next to parent classifier */
                pencil_feature_layouter_private_layout_lifeline ( this_,
                                                                  diagram_draw_area,
                                                                  diag_type,
                                                                  classifier_type,
                                                                  c_symbol_box,
                                                                  c_envelope_box,
                                                                  feature_layout
                                                                );
            }
            break;

            case DATA_FEATURE_TYPE_PORT:  /* or */
            case DATA_FEATURE_TYPE_IN_PORT_PIN:  /* or */
            case DATA_FEATURE_TYPE_OUT_PORT_PIN:  /* or */
            case DATA_FEATURE_TYPE_ENTRY:  /* or */
            case DATA_FEATURE_TYPE_EXIT:
            {
                /* layout port feature onto parent classifier box */
                pencil_feature_layouter_private_layout_port_pin ( this_,
                                                                  classifier_type,
                                                                  c_symbol_box,
                                                                  the_feature,
                                                                  font_layout,
                                                                  feature_layout
                                                                );
            }
            break;

            case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:  /* or */
            case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
            {
                /* layout interface feature close to parent classifier */
                pencil_feature_layouter_private_layout_interface ( this_,
                                                                   c_symbol_box,
                                                                   the_feature,
                                                                   font_layout,
                                                                   feature_layout
                                                                 );
            }
            break;

            case DATA_FEATURE_TYPE_PROPERTY:  /* or */
            case DATA_FEATURE_TYPE_OPERATION:  /* or */
            case DATA_FEATURE_TYPE_TAGGED_VALUE:
            {
                /* layout property or operation feature within the space area, also the tagged values */
                const geometry_rectangle_t *const c_space = layout_visible_classifier_get_space_const ( layout_classifier );
                pencil_feature_layouter_private_layout_compartment ( this_,
                                                                     c_space,
                                                                     font_layout,
                                                                     feature_layout
                                                                   );
            }
            break;

            default:
            {
                U8_LOG_ANOMALY("unknown feature type in pencil_feature_layouter_do_layout");
                /* this may happen if a new database file has been read by an old program version */
                /* layout like property or operation or tagged values */
                const geometry_rectangle_t *const c_space = layout_visible_classifier_get_space_const ( layout_classifier );
                pencil_feature_layouter_private_layout_compartment ( this_,
                                                                     c_space,
                                                                     font_layout,
                                                                     feature_layout
                                                                   );
            }
            break;
        }
    }

    U8_TRACE_END();
}

void pencil_feature_layouter_calculate_features_bounds( pencil_feature_layouter_t *this_,
                                                        data_row_t diagramelement_id,
                                                        PangoLayout *font_layout,
                                                        geometry_dimensions_t *out_features_bounds )
{
    U8_TRACE_BEGIN();
    assert( NULL != font_layout );
    assert( NULL != out_features_bounds );

    /* establish precondition: precalculate the dimensions of labels */
    if ( ! (*this_).label_dimensions_initialized )
    {
        pencil_feature_layouter_private_init_label_dimensions( this_, font_layout );
    }

    double width = 0.0;
    double height = 0.0;

    /* search all contained features */
    const uint32_t count_features
        = layout_visible_set_get_feature_count( (*this_).layout_data );
    for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
    {
        const layout_feature_t *const feature_layout
            = layout_visible_set_get_feature_ptr( (*this_).layout_data, f_idx );
        const data_feature_t *const the_feature
            = layout_feature_get_data_const( feature_layout );
        const layout_visible_classifier_t *const layout_classifier
            = layout_feature_get_classifier_const( feature_layout );
        const data_feature_type_t the_feature_type = data_feature_get_main_type( the_feature );

        if (( diagramelement_id == layout_visible_classifier_get_diagramelement_id( layout_classifier ) )
            && data_feature_type_inside_compartment( the_feature_type ) )
        {
            /* feature label sizes are already precalculated */
            assert( (*this_).label_dimensions_initialized );
            const geometry_rectangle_t *const label_box = layout_feature_get_label_box_const( feature_layout );

            /* update width and height */
            width = u8_f64_max2( width, geometry_rectangle_get_width( label_box ) );
            height += geometry_rectangle_get_height( label_box );
        }
    }

    const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );
    const double sum_of_gaps = 6.0 * gap;  /* gaps above and below each of the 3 compartment lines */

    geometry_dimensions_reinit( out_features_bounds, width + 2.0 * gap, height + sum_of_gaps );
    U8_TRACE_END();
}

void pencil_feature_layouter_private_init_label_dimensions( pencil_feature_layouter_t *this_,
                                                            PangoLayout *font_layout
                                                          )
{
    U8_TRACE_BEGIN();
    assert ( NULL != font_layout );

    const uint32_t count_features
        = layout_visible_set_get_feature_count( (*this_).layout_data );
    for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
    {
        layout_feature_t *const feature_layout
            = layout_visible_set_get_feature_ptr( (*this_).layout_data, f_idx );
        const data_feature_t *const the_feature
            = layout_feature_get_data_const( feature_layout );
        const data_feature_type_t the_feature_type = data_feature_get_main_type( the_feature );

        if ( data_feature_type_inside_compartment( the_feature_type ) )
        {
            geometry_dimensions_t min_feature_bounds;
            geometry_dimensions_init_empty( &min_feature_bounds );
            pencil_feature_painter_get_minimum_bounds( &((*this_).feature_painter),
                                                       the_feature,
                                                       (*this_).profile,
                                                       (*this_).pencil_size,
                                                       font_layout,
                                                       &min_feature_bounds
                                                     );

            const geometry_rectangle_t label_box = {
                .left = 0.0,
                .top = 0.0,
                .width = geometry_dimensions_get_width( &min_feature_bounds ),
                .height = geometry_dimensions_get_height( &min_feature_bounds ),
            };
            layout_feature_set_label_box( feature_layout, &label_box );
            geometry_dimensions_destroy( &min_feature_bounds );
        }
    }

    (*this_).label_dimensions_initialized = true;
    U8_TRACE_END();
}

void pencil_feature_layouter_private_layout_lifeline ( pencil_feature_layouter_t *this_,
                                                       const geometry_rectangle_t *diagram_space,
                                                       data_diagram_type_t diagram_type,
                                                       data_classifier_type_t classifier_type,
                                                       const geometry_rectangle_t *classifier_symbol_box,
                                                       const geometry_rectangle_t *classifier_envelope_box,
                                                       layout_feature_t *out_feature_layout )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diagram_space );
    assert ( NULL != classifier_symbol_box );
    assert ( NULL != classifier_envelope_box );
    assert ( NULL != out_feature_layout );

    /* get preferred object distance */
    const double obj_dist
        = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double feature_width = obj_dist;

    const bool lifeline_has_semantics
        = data_rules_classifier_has_scenario_semantics( &((*this_).rules),
                                                        diagram_type,
                                                        classifier_type
                                                      );

    if (( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == diagram_type ) && lifeline_has_semantics )
    {
        layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_RIGHT );
        const double c_right = geometry_rectangle_get_right( classifier_envelope_box );
        const double c_top = geometry_rectangle_get_top( classifier_envelope_box );
        const double c_height = geometry_rectangle_get_height( classifier_envelope_box );
        const double dda_right = geometry_rectangle_get_right ( diagram_space );
        geometry_rectangle_t lifeline_bounds;
        geometry_rectangle_init ( &lifeline_bounds,
                                  c_right,
                                  c_top + ( 0.5 * ( c_height - feature_width ) ),
                                  dda_right - c_right - obj_dist,
                                  feature_width
                                );
        layout_feature_set_symbol_box ( out_feature_layout, &lifeline_bounds );
        layout_feature_set_label_box ( out_feature_layout, &lifeline_bounds );
    }
    else if (( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == diagram_type ) && lifeline_has_semantics )
    {
        layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_DOWN );
        const double c_bottom = geometry_rectangle_get_bottom( classifier_envelope_box );
        const double c_left = geometry_rectangle_get_left( classifier_envelope_box );
        const double c_width = geometry_rectangle_get_width( classifier_envelope_box );
        const double dda_bottom = geometry_rectangle_get_bottom ( diagram_space );
        geometry_rectangle_t lifeline_bounds;
        geometry_rectangle_init ( &lifeline_bounds,
                                  c_left + ( 0.5 * ( c_width - feature_width ) ),
                                  c_bottom,
                                  feature_width,
                                  dda_bottom - c_bottom - obj_dist
                                );
        layout_feature_set_symbol_box ( out_feature_layout, &lifeline_bounds );
        layout_feature_set_label_box ( out_feature_layout, &lifeline_bounds );
    }
    else /*if (( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == diagram_type )||( DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM == diagram_type ))*/
    {
        layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_CENTER );
        layout_feature_set_symbol_box ( out_feature_layout, classifier_symbol_box );
        layout_feature_set_label_box ( out_feature_layout, classifier_symbol_box );
    }

    U8_TRACE_END();
}

void pencil_feature_layouter_private_layout_port_pin ( pencil_feature_layouter_t *this_,
                                                       data_classifier_type_t classifier_type,
                                                       const geometry_rectangle_t *classifier_symbol_box,
                                                       const data_feature_t *the_feature,
                                                       PangoLayout *font_layout,
                                                       layout_feature_t *out_feature_layout )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_symbol_box );
    assert ( NULL != the_feature );
    assert ( NULL != font_layout );
    assert ( NULL != out_feature_layout );

    /* get preferred object distance */
    const double gap
        = pencil_size_get_standard_object_border( (*this_).pencil_size );

    const double port_icon_size
        = pencil_size_get_standard_font_size( (*this_).pencil_size );

    /* determine the coordinates of the box-line */
    geometry_rectangle_t classifier_box;
    geometry_rectangle_copy( &classifier_box, classifier_symbol_box );
    geometry_rectangle_shift( &classifier_box, gap, gap );
    geometry_rectangle_enlarge( &classifier_box, -2.0*gap, -2.0*gap );

    const int32_t list_order
        = data_feature_get_list_order( the_feature );

    /* position the port icon */
    const bool is_sysml_constraint_block = (classifier_type == DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK);
    const bool is_behavioral = data_classifier_type_is_behavioral( classifier_type );
    const data_feature_type_t feat_type = data_feature_get_main_type( the_feature );
    const bool is_state_entry_exit
        = (( feat_type == DATA_FEATURE_TYPE_ENTRY )||( feat_type == DATA_FEATURE_TYPE_EXIT ));
    const double outwards_distance
        = is_sysml_constraint_block ? 0.0 : (is_behavioral&&(!is_state_entry_exit)) ? port_icon_size : (0.5*port_icon_size);
    double port_icon_left;
    double port_icon_top;
    const double show_arrow_in = ( feat_type == DATA_FEATURE_TYPE_IN_PORT_PIN );
    const double show_arrow_out = ( feat_type == DATA_FEATURE_TYPE_OUT_PORT_PIN );
    geometry_direction_t arrow_dir = GEOMETRY_DIRECTION_CENTER;
    if ( list_order < 0 )
    {
        static const int32_t INT32_MIN_HALF = INT32_MIN/2;
        if ( list_order < INT32_MIN_HALF )  /* SHOW ON RIGHT BORDER */
        {
            const double y_pos_rel
                = (list_order - INT32_MIN_HALF) / ((double)(INT32_MIN_HALF));
            port_icon_left = geometry_rectangle_get_right( &classifier_box ) - port_icon_size + outwards_distance;
            port_icon_top = geometry_rectangle_get_top( &classifier_box ) + y_pos_rel * ( geometry_rectangle_get_height( &classifier_box ) - port_icon_size );
            arrow_dir = show_arrow_in ? GEOMETRY_DIRECTION_LEFT : show_arrow_out ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_CENTER;
        }
        else  /* SHOW ON TOP BORDER */
        {
            const double x_pos_rel
                = (list_order) / ((double)(INT32_MIN_HALF));
            port_icon_left = geometry_rectangle_get_left( &classifier_box ) + x_pos_rel * ( geometry_rectangle_get_width( &classifier_box ) - port_icon_size );
            port_icon_top = geometry_rectangle_get_top( &classifier_box ) - outwards_distance;
            arrow_dir = show_arrow_in ? GEOMETRY_DIRECTION_DOWN : show_arrow_out ? GEOMETRY_DIRECTION_UP : GEOMETRY_DIRECTION_CENTER;
        }
    }
    else
    {
        static const int32_t INT32_MAX_HALF = (INT32_MAX/2)+1;  /* round to ceiling */
        if ( list_order < INT32_MAX_HALF )  /* SHOW ON LEFT BORDER */
        {
            const double y_pos_rel
                = (list_order) / ((double)(INT32_MAX_HALF));
            port_icon_left = geometry_rectangle_get_left( &classifier_box ) - outwards_distance;
            port_icon_top = geometry_rectangle_get_top( &classifier_box ) + y_pos_rel * ( geometry_rectangle_get_height( &classifier_box ) - port_icon_size );
            arrow_dir = show_arrow_in ? GEOMETRY_DIRECTION_RIGHT : show_arrow_out ? GEOMETRY_DIRECTION_LEFT : GEOMETRY_DIRECTION_CENTER;
        }
        else  /* SHOW ON BOTTOM BORDER */
        {
            const double x_pos_rel
                = (list_order - INT32_MAX_HALF) / ((double)(INT32_MAX_HALF));
            port_icon_left = geometry_rectangle_get_left( &classifier_box ) + x_pos_rel * ( geometry_rectangle_get_width( &classifier_box ) - port_icon_size );
            port_icon_top = geometry_rectangle_get_bottom( &classifier_box ) - port_icon_size + outwards_distance;
            arrow_dir = show_arrow_in ? GEOMETRY_DIRECTION_UP : show_arrow_out ? GEOMETRY_DIRECTION_DOWN : GEOMETRY_DIRECTION_CENTER;
        }
    }

    /* set feature bounding box */
    geometry_rectangle_t f_bounds;
    geometry_rectangle_init ( &f_bounds,
                              port_icon_left,
                              port_icon_top,
                              port_icon_size,
                              port_icon_size
                            );
    layout_feature_set_symbol_box ( out_feature_layout, &f_bounds );
    layout_feature_set_label_box ( out_feature_layout, &f_bounds );
    layout_feature_set_icon_direction ( out_feature_layout, arrow_dir );

    U8_TRACE_END();
}

void pencil_feature_layouter_private_layout_interface ( pencil_feature_layouter_t *this_,
                                                        const geometry_rectangle_t *classifier_symbol_box,
                                                        const data_feature_t *the_feature,
                                                        PangoLayout *font_layout,
                                                        layout_feature_t *out_feature_layout )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_symbol_box );
    assert ( NULL != the_feature );
    assert ( NULL != font_layout );
    assert ( NULL != out_feature_layout );

    /* get preferred object size + distance */
    const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );
    const double interface_icon_size = pencil_size_get_standard_font_size( (*this_).pencil_size );
    const double interface_distance
        = 2.001 * pencil_size_get_preferred_object_distance( (*this_).pencil_size ) + gap;  /* min dist formula as in relationship layouter */

    const int32_t list_order = data_feature_get_list_order( the_feature );

    /* position the interface icon */
    if ( list_order < 0 )
    {
        static const int32_t INT32_MIN_HALF = INT32_MIN/2;
        if ( list_order < INT32_MIN_HALF )  /* SHOW ON RIGHT BORDER */
        {
            const double y_pos_rel
                = (list_order - INT32_MIN_HALF) / ((double)(INT32_MIN_HALF));
            geometry_rectangle_t f_bounds;
            geometry_rectangle_init ( &f_bounds,
                                      geometry_rectangle_get_right( classifier_symbol_box ) + interface_distance,
                                      geometry_rectangle_get_top( classifier_symbol_box ) + y_pos_rel * ( geometry_rectangle_get_height( classifier_symbol_box ) - interface_icon_size ),
                                      interface_icon_size,
                                      interface_icon_size
                                    );
            layout_feature_set_symbol_box ( out_feature_layout, &f_bounds );
            layout_feature_set_label_box ( out_feature_layout, &f_bounds );
            layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_RIGHT );
        }
        else  /* SHOW ON TOP BORDER */
        {
            const double x_pos_rel
                = (list_order) / ((double)(INT32_MIN_HALF));
            geometry_rectangle_t f_bounds;
            geometry_rectangle_init ( &f_bounds,
                                      geometry_rectangle_get_left( classifier_symbol_box ) + x_pos_rel * ( geometry_rectangle_get_width( classifier_symbol_box ) - interface_icon_size ),
                                      geometry_rectangle_get_top( classifier_symbol_box ) - interface_distance - interface_icon_size,
                                      interface_icon_size,
                                      interface_icon_size
                                    );
            layout_feature_set_symbol_box ( out_feature_layout, &f_bounds );
            layout_feature_set_label_box ( out_feature_layout, &f_bounds );
            layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_UP );
        }
    }
    else
    {
        static const int32_t INT32_MAX_HALF = (INT32_MAX/2)+1;  /* round to ceiling */
        if ( list_order < INT32_MAX_HALF )  /* SHOW ON LEFT BORDER */
        {
            const double y_pos_rel
                = (list_order) / ((double)(INT32_MAX_HALF));
            geometry_rectangle_t f_bounds;
            geometry_rectangle_init ( &f_bounds,
                                      geometry_rectangle_get_left( classifier_symbol_box ) - interface_distance - interface_icon_size,
                                      geometry_rectangle_get_top( classifier_symbol_box ) + y_pos_rel * ( geometry_rectangle_get_height( classifier_symbol_box ) - interface_icon_size ),
                                      interface_icon_size,
                                      interface_icon_size
                                    );
            layout_feature_set_symbol_box ( out_feature_layout, &f_bounds );
            layout_feature_set_label_box ( out_feature_layout, &f_bounds );
            layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_LEFT );
        }
        else  /* SHOW ON BOTTOM BORDER */
        {
            const double x_pos_rel
                = (list_order - INT32_MAX_HALF) / ((double)(INT32_MAX_HALF));
            geometry_rectangle_t f_bounds;
            geometry_rectangle_init ( &f_bounds,
                                      geometry_rectangle_get_left( classifier_symbol_box ) + x_pos_rel * ( geometry_rectangle_get_width( classifier_symbol_box ) - interface_icon_size ),
                                      geometry_rectangle_get_bottom( classifier_symbol_box ) + interface_distance,
                                      interface_icon_size,
                                      interface_icon_size
                                    );
            layout_feature_set_symbol_box ( out_feature_layout, &f_bounds );
            layout_feature_set_label_box ( out_feature_layout, &f_bounds );
            layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_DOWN );
        }
    }

    if ( DATA_FEATURE_TYPE_PROVIDED_INTERFACE == data_feature_get_main_type (the_feature) )
    {
        /* a provided interface has no direction, it is a circle */
        layout_feature_set_icon_direction ( out_feature_layout, GEOMETRY_DIRECTION_CENTER );
    }

    U8_TRACE_END();
}

void pencil_feature_layouter_private_layout_compartment ( pencil_feature_layouter_t *this_,
                                                          const geometry_rectangle_t *classifier_space,
                                                          PangoLayout *font_layout,
                                                          layout_feature_t *io_feature_layout )
{
    U8_TRACE_BEGIN();
    assert( NULL != classifier_space );
    assert( NULL != font_layout );
    assert( NULL != io_feature_layout );

    /* feature label sizes are already precalculated */
    assert( (*this_).label_dimensions_initialized );

    /* define names for input data */
    const layout_visible_classifier_t * const vis_classfy
        = layout_feature_get_classifier_const( io_feature_layout );
    assert( NULL != vis_classfy );
    const data_row_t diagele_id = layout_visible_classifier_get_diagramelement_id( vis_classfy );
    const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );
    const data_feature_t *const the_feature = layout_feature_get_data_const( io_feature_layout );

    /* calculate the feat_top by summing up the heights of the features above */
    double feat_top = geometry_rectangle_get_top( classifier_space );
    const uint32_t num_features = layout_visible_set_get_feature_count( (*this_).layout_data );
    for ( uint32_t f_probe_idx = 0; f_probe_idx < num_features; f_probe_idx ++ )
    {
        const layout_feature_t *const f_probe_layout
            = layout_visible_set_get_feature_ptr( (*this_).layout_data, f_probe_idx );
        assert( NULL != f_probe_layout );
        const layout_visible_classifier_t *const probe_vis_classfy
            = layout_feature_get_classifier_const( f_probe_layout );
        assert( NULL != probe_vis_classfy );

        /* check if this f_probe_layout has the same diagram element id as the_feature */
        if ( diagele_id == layout_visible_classifier_get_diagramelement_id( probe_vis_classfy ) )
        {
            /* this is a feature of the same visible_classifier_t */
            /* define names for input data */
            const data_feature_t *f_probe_data;
            f_probe_data = layout_feature_get_data_const( f_probe_layout );
            assert( NULL != f_probe_data );
            const data_feature_type_t f_probe_type = data_feature_get_main_type ( f_probe_data );
            if ( data_feature_type_inside_compartment( f_probe_type ) )
            {
                const bool is_above
                    = (( data_feature_get_list_order( f_probe_data ) < data_feature_get_list_order( the_feature ))
                    || (( data_feature_get_list_order( f_probe_data ) == data_feature_get_list_order( the_feature ) )
                    && ( data_feature_get_row_id( f_probe_data ) < data_feature_get_row_id( the_feature ) )));
                if ( is_above )
                {
                    const geometry_rectangle_t *const probe_label_box
                        = layout_feature_get_label_box_const( f_probe_layout );
                    feat_top += geometry_rectangle_get_height( probe_label_box );
                }
            }
        }
    }

    /* determine compartments above the current */
    const data_feature_type_t f_type = data_feature_get_main_type (the_feature);
    const uint32_t count_compartments_above
        = ( DATA_FEATURE_TYPE_PROPERTY == f_type )
        ? 0
        : ( DATA_FEATURE_TYPE_OPERATION == f_type )
        ? 1
        : ( DATA_FEATURE_TYPE_TAGGED_VALUE == f_type )
        ? 2  /* first compartment for properties, second for operations, third for tagged values */
        : 2; /* the last compartment is for all unknown feature types. */
             /* this may happen if a new database file has been read by an old program version */
             /* note that today, there is no separator line yet - so the number is the same */
    feat_top += ( count_compartments_above * 2 * gap ) + 2.0 * gap;

    /* determine the bounds of the feature */
    const geometry_rectangle_t *const feat_label_box
        = layout_feature_get_label_box_const( io_feature_layout );

    /* layout feature into parent classifier */
    const geometry_rectangle_t label_box = {
        .left = geometry_rectangle_get_left( classifier_space ) + gap,
        .top = feat_top,
        .width = geometry_rectangle_get_width( feat_label_box ),
        .height = geometry_rectangle_get_height( feat_label_box )
    };
    const geometry_rectangle_t feat_bounds = {
        .left = geometry_rectangle_get_left( classifier_space ),
        .top = feat_top,
        .width = geometry_rectangle_get_width( feat_label_box ) + 2.0 * gap,
        .height = geometry_rectangle_get_height( feat_label_box )
    };

    layout_feature_set_label_box( io_feature_layout, &label_box );
    layout_feature_set_symbol_box( io_feature_layout, &feat_bounds );
    layout_feature_set_icon_direction( io_feature_layout, GEOMETRY_DIRECTION_CENTER );  /* dummy direction */

    U8_TRACE_END();
}


/*
Copyright 2017-2025 Andreas Warnke

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
