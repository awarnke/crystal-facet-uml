/* File: pencil_classifier_composer.c; Copyright and License: see below */

#include "pencil_classifier_composer.h"
#include "u8/u8_trace.h"
#include "u8/u8_f64.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_classifier_composer_init( pencil_classifier_composer_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_classifier_icon_init( &((*this_).draw_classifier_icon) );
    draw_classifier_label_init( &((*this_).draw_classifier_label) );
    draw_classifier_contour_init( &((*this_).draw_classifier_contour) );
    draw_stereotype_icon_init( &((*this_).draw_stereotype_icon) );

    U8_TRACE_END();
}

void pencil_classifier_composer_destroy( pencil_classifier_composer_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_classifier_icon_destroy( &((*this_).draw_classifier_icon) );
    draw_classifier_label_destroy( &((*this_).draw_classifier_label) );
    draw_classifier_contour_destroy( &((*this_).draw_classifier_contour) );
    draw_stereotype_icon_destroy( &((*this_).draw_stereotype_icon) );

    U8_TRACE_END();
}

pencil_error_t pencil_classifier_composer_set_envelope_box( pencil_classifier_composer_t *this_,
                                                            const geometry_rectangle_t *envelope,
                                                            bool shows_contained_children,
                                                            const geometry_compartments_t *features_dimensions,
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

    pencil_error_t result_err = PENCIL_ERROR_NONE;

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier );
    const bool has_stereotype_icon
        = draw_stereotype_icon_exists( &((*this_).draw_stereotype_icon), classifier_stereotype, profile );
    const double half_gap = 0.5 * pencil_size_get_preferred_object_distance( pencil_size );

    U8_TRACE_INFO_INT("calculating bounds of classifier id, type:", data_classifier_get_row( classifier ) );
    U8_TRACE_INFO_INT_INT("calculating bounds of classifier type, children:", classifier_type, shows_contained_children?1:0 );

    /* determine inner_area of the classifier-shape */
    const geometry_dimensions_t *const compartment_dim
        = geometry_compartments_get_feature_compartments( features_dimensions );
    const geometry_rectangle_t inner_area
        = draw_classifier_contour_calc_inner_area( &((*this_).draw_classifier_contour),
                                                   classifier_type,
                                                   envelope,
                                                   pencil_size
                                                 );
    /* take number of ports into account: geometry_compartments_get_outer_width */
    const double inner_to_outer_width_delta
        = geometry_rectangle_get_width( envelope ) - geometry_rectangle_get_width( &inner_area ) ;
    const double inner_to_outer_height_delta
        = geometry_rectangle_get_height( envelope ) - geometry_rectangle_get_height( &inner_area ) ;
    const double needed_outer_width = geometry_compartments_get_outer_width( features_dimensions );
    const double needed_inner_width = needed_outer_width - inner_to_outer_width_delta;
    U8_TRACE_INFO_INT_INT("needed_outer/inner_width :", (int) needed_outer_width, (int) needed_inner_width );
    const double needed_outer_height = geometry_compartments_get_outer_height( features_dimensions );
    const double needed_inner_height = needed_outer_height - inner_to_outer_height_delta;
    U8_TRACE_INFO_INT_INT("needed_outer/inner_height :", (int) needed_outer_height, (int) needed_inner_height );
    const double preferred_inner_width = u8_f64_max3( geometry_rectangle_get_width( &inner_area ),
                                                      geometry_dimensions_get_width( compartment_dim ),
                                                      needed_inner_width
                                                    );
    const geometry_rectangle_t preferred_inner_area
        = geometry_rectangle_new( geometry_rectangle_get_left( &inner_area ),
                                  geometry_rectangle_get_top( &inner_area ),
                                  preferred_inner_width,
                                  geometry_rectangle_get_height( &inner_area )
                                );
    U8_TRACE_INFO("==== preferred_inner_area ====" );
    geometry_rectangle_trace( &preferred_inner_area );

    /* determine border sizes of the label (and optionally the right-aligned icon) */
    geometry_rectangle_t label_rect;
    geometry_rectangle_init_empty( &label_rect );
    geometry_rectangle_t icon_rect;
    geometry_rectangle_init_empty( &icon_rect );
    geometry_rectangle_t label_compartment;
    geometry_rectangle_init_empty( &label_compartment );
    const pencil_error_t area_too_small
        = pencil_classifier_composer_private_get_label_box( this_,
                                                            visible_classifier,
                                                            shows_contained_children,
                                                            has_stereotype_icon,
                                                            &preferred_inner_area,
                                                            pencil_size,
                                                            font_layout,
                                                            &label_rect,
                                                            &icon_rect,
                                                            &label_compartment
                                                          );
    result_err |= area_too_small;

    /* set label_box */
    layout_visible_classifier_set_label_box( io_classifier_layout, &label_rect );
    layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                GEOMETRY_H_ALIGN_CENTER,
                                                GEOMETRY_V_ALIGN_TOP
                                              );

    /* set icon_box */
    layout_visible_classifier_set_icon_box( io_classifier_layout, &icon_rect );

    /* position feature compartments box below label box */
    const double new_inner_left = u8_f64_min2( geometry_rectangle_get_left( &preferred_inner_area ),
                                               geometry_rectangle_get_left( &label_compartment )
                                             );
    const geometry_rectangle_t compartments_rect
        = geometry_rectangle_new( new_inner_left,
                                  geometry_rectangle_get_bottom( &label_compartment ),
                                  geometry_dimensions_get_width( compartment_dim ),
                                  geometry_dimensions_get_height( compartment_dim )
                                );
    layout_visible_classifier_set_compartments( io_classifier_layout, &compartments_rect );

    /* calculate space */
    const double label_and_features_height
        = geometry_rectangle_get_height( &label_compartment )
        + geometry_dimensions_get_height( compartment_dim );
    const double classifier_space_height
        = u8_f64_max3( geometry_rectangle_get_height( &preferred_inner_area ) - label_and_features_height,
                       half_gap,
                       needed_inner_height - label_and_features_height
                     );
    const geometry_rectangle_t classifier_space
        = geometry_rectangle_new( new_inner_left,
                                  geometry_rectangle_get_top( &preferred_inner_area ) + label_and_features_height,
                                  geometry_rectangle_get_width( &preferred_inner_area ),
                                  classifier_space_height
                                );
    if ( classifier_space_height
         > geometry_rectangle_get_height( &preferred_inner_area ) - label_and_features_height + 0.000000001 )
    {
        result_err |= PENCIL_ERROR_OUT_OF_BOUNDS;
    }
    layout_visible_classifier_set_space( io_classifier_layout, &classifier_space );

    /* set symbol */
    const bool has_contour
        = draw_classifier_contour_has_contour( &((*this_).draw_classifier_contour), classifier_type );
    if ( has_contour )
    {
        U8_TRACE_INFO("calculating symbol box as envelope around label and space..." );
        /* calculate symbol bounds */
        const double new_inner_width = u8_f64_max3( geometry_rectangle_get_width( &label_compartment ),
                                                    geometry_rectangle_get_width( &compartments_rect ),
                                                    geometry_rectangle_get_width( &classifier_space )
                                                  );
        const double new_inner_height = geometry_rectangle_get_height( &label_compartment )
                                      + geometry_rectangle_get_height( &compartments_rect )
                                      + geometry_rectangle_get_height( &classifier_space );
        const geometry_rectangle_t new_inner_area
            = geometry_rectangle_new( new_inner_left,
                                      geometry_rectangle_get_top( &label_compartment ),
                                      new_inner_width,
                                      new_inner_height
                                    );
        const geometry_rectangle_t new_envelope
            = draw_classifier_contour_calc_outer_bounds( &((*this_).draw_classifier_contour),
                                                         classifier_type,
                                                         &new_inner_area,
                                                         pencil_size
                                                       );
        layout_visible_classifier_set_symbol_box( io_classifier_layout, &new_envelope );
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
                                         geometry_rectangle_get_left( &preferred_inner_area ),
                                         geometry_rectangle_get_width( &preferred_inner_area )
                                       );
        const double symbol_top = geometry_rectangle_get_top( &label_compartment ) - symbol_height;
        const geometry_rectangle_t classifier_symbol_box
             = geometry_rectangle_new( symbol_left, symbol_top, symbol_width, symbol_height );
        layout_visible_classifier_set_symbol_box( io_classifier_layout, &classifier_symbol_box );
    }

    U8_TRACE_INFO("==== symbol_box   ====" );
    geometry_rectangle_trace( layout_visible_classifier_get_symbol_box_const( io_classifier_layout ) );
    U8_TRACE_INFO("==== label_box    ====" );
    geometry_rectangle_trace( &label_rect );
    U8_TRACE_INFO("==== icon_box     ====" );
    geometry_rectangle_trace( &icon_rect );
    U8_TRACE_INFO("==== compartments =====" );
    geometry_rectangle_trace( layout_visible_classifier_get_compartments_const( io_classifier_layout ) );
    U8_TRACE_INFO("==== space        =====" );
    geometry_rectangle_trace( &classifier_space );

    if ( result_err != PENCIL_ERROR_NONE )
    {
        /* shift: center to requested position after resizing beyond requested size */
        const geometry_rectangle_t current_envelope
            = layout_visible_classifier_get_envelope_box( io_classifier_layout );
        const double shift_to_right = geometry_rectangle_get_center_x( envelope ) - geometry_rectangle_get_center_x( &current_envelope );
        const double shift_to_bottom = geometry_rectangle_get_center_y( envelope ) - geometry_rectangle_get_center_y( &current_envelope );
        const geometry_offset_t offset = geometry_offset_new( shift_to_right, shift_to_bottom );
        layout_visible_classifier_shift( io_classifier_layout, &offset );
    }

    geometry_rectangle_destroy( &label_rect );
    geometry_rectangle_destroy( &icon_rect );
    geometry_rectangle_destroy( &label_compartment );

    U8_TRACE_END_ERR( result_err );
    return result_err;
}

pencil_error_t pencil_classifier_composer_expand_space( pencil_classifier_composer_t *this_,
                                                        const geometry_rectangle_t *space,
                                                        bool shows_contained_children,
                                                        const geometry_compartments_t *features_dimensions,
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

    pencil_error_t result_err = PENCIL_ERROR_NONE;

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier );
    const bool has_stereotype_icon
        = draw_stereotype_icon_exists( &((*this_).draw_stereotype_icon), classifier_stereotype, profile );

    U8_TRACE_INFO_INT("expanding bounds of classifier id:", data_classifier_get_row( classifier ) );
    U8_TRACE_INFO_INT_INT("expanding bounds of classifier type, children:", classifier_type, shows_contained_children?1:0 );

    /* determine inner_area of the classifier-shape */
    const geometry_dimensions_t *const compartment_dim
        = geometry_compartments_get_feature_compartments( features_dimensions );
    const geometry_rectangle_t preliminry_envelope
        = draw_classifier_contour_calc_outer_bounds( &((*this_).draw_classifier_contour),
                                                     classifier_type,
                                                     space,
                                                     pencil_size
                                                   );
    /* take number of ports into account: geometry_compartments_get_outer_width */
    const double inner_to_outer_width_delta
        = geometry_rectangle_get_width( &preliminry_envelope ) - geometry_rectangle_get_width( space ) ;
    const double inner_to_outer_height_delta
        = geometry_rectangle_get_height( &preliminry_envelope ) - geometry_rectangle_get_height( space ) ;
    const double needed_outer_width = geometry_compartments_get_outer_width( features_dimensions );
    const double needed_inner_width = needed_outer_width - inner_to_outer_width_delta;
    U8_TRACE_INFO_INT_INT("needed_outer/inner_width :", (int) needed_outer_width, (int) needed_inner_width );
    const double needed_outer_height = geometry_compartments_get_outer_height( features_dimensions );
    const double needed_inner_height = needed_outer_height - inner_to_outer_height_delta;
    U8_TRACE_INFO_INT_INT("needed_outer/inner_height :", (int) needed_outer_height, (int) needed_inner_height );
    const double preferred_inner_width = u8_f64_max3( geometry_rectangle_get_width( space ),
                                                      geometry_dimensions_get_width( compartment_dim ),
                                                      needed_inner_width
                                                    );
    const geometry_rectangle_t preferred_inner_area
        = geometry_rectangle_new( geometry_rectangle_get_left( space ),
                                  geometry_rectangle_get_top( space ),
                                  preferred_inner_width,
                                  geometry_dimensions_get_height( compartment_dim ) + geometry_rectangle_get_height( space )
                                );
    U8_TRACE_INFO("==== preferred_inner_area ====" );
    geometry_rectangle_trace( &preferred_inner_area );

    /* determine border sizes of the label (and optionally the right-aligned icon) */
    geometry_rectangle_t label_rect;
    geometry_rectangle_init_empty( &label_rect );
    geometry_rectangle_t icon_rect;
    geometry_rectangle_init_empty( &icon_rect );
    geometry_rectangle_t label_compartment;
    geometry_rectangle_init_empty( &label_compartment );
    const pencil_error_t area_too_small
        = pencil_classifier_composer_private_get_label_box( this_,
                                                            visible_classifier,
                                                            shows_contained_children,
                                                            has_stereotype_icon,
                                                            &preferred_inner_area,
                                                            pencil_size,
                                                            font_layout,
                                                            &label_rect,
                                                            &icon_rect,
                                                            &label_compartment
                                                          );
    result_err |= area_too_small;
    if ( area_too_small != PENCIL_ERROR_NONE )
    {
        U8_TRACE_INFO("new width is defined by label-and-icon, not by requested inner space" );
    }

    /* shift the label and icon because preferred_inner_area was a fake; */
    /* the top side is at space top instead of label top */
    const double delta_top
        = geometry_rectangle_get_bottom( &label_compartment )
        + geometry_dimensions_get_height( compartment_dim )
        - geometry_rectangle_get_top( space );
    geometry_rectangle_set_top( &label_rect, geometry_rectangle_get_top( &label_rect ) - delta_top );
    geometry_rectangle_set_top( &icon_rect, geometry_rectangle_get_top( &icon_rect ) - delta_top );
    geometry_rectangle_set_top( &label_compartment, geometry_rectangle_get_top( &label_compartment ) - delta_top );

    /* determine inner area */
    const double inner_left = u8_f64_min2( geometry_rectangle_get_left( space ),
                                           geometry_rectangle_get_left( &label_compartment )
                                         );
    const double inner_height
        = u8_f64_max2( geometry_rectangle_get_height( &label_compartment ) + geometry_dimensions_get_height( compartment_dim )
                       + geometry_rectangle_get_height( space ),
                       needed_inner_height
        );
    const double inner_width = u8_f64_max2( preferred_inner_width, geometry_rectangle_get_width( &label_compartment ) );
    const geometry_rectangle_t inner_area
        = geometry_rectangle_new( inner_left,
                                  geometry_rectangle_get_top( &label_compartment ),
                                  inner_width,
                                  inner_height
                                );
    if (( inner_width > geometry_rectangle_get_width( space ) + 0.000000001 )
        ||( inner_height > geometry_rectangle_get_height( &label_compartment )
        + geometry_dimensions_get_height( compartment_dim ) + geometry_rectangle_get_height( space ) + 0.000000001 ))
    {
        /* features or label have expanded the space */
        result_err |= PENCIL_ERROR_OUT_OF_BOUNDS;
    }

    /* set label_box */
    layout_visible_classifier_set_label_box( io_classifier_layout, &label_rect );
    layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                GEOMETRY_H_ALIGN_CENTER,
                                                GEOMETRY_V_ALIGN_TOP
                                              );
    /* set icon_box */
    layout_visible_classifier_set_icon_box( io_classifier_layout, &icon_rect );

    /* position feature compartments box below label box */
    const geometry_rectangle_t compartments_rect
        = geometry_rectangle_new( geometry_rectangle_get_left( &inner_area ),
                                  geometry_rectangle_get_bottom( &label_compartment ),
                                  geometry_dimensions_get_width( compartment_dim ),
                                  geometry_dimensions_get_height( compartment_dim )
                                );
    layout_visible_classifier_set_compartments( io_classifier_layout, &compartments_rect );

    /* calculate space */
    /* get the symbol and label boxes and inner space rectangles to modify */
    const double adjusted_space_height
        = inner_height - geometry_rectangle_get_height( &label_compartment )
        - geometry_dimensions_get_height( compartment_dim );
    const geometry_rectangle_t classifier_space
        = geometry_rectangle_new( geometry_rectangle_get_left( &inner_area ),
                                  geometry_rectangle_get_top( space ),
                                  geometry_rectangle_get_width( &inner_area ),
                                  adjusted_space_height
                                );
    layout_visible_classifier_set_space( io_classifier_layout, &classifier_space );

    /* sizes of geometric objects are determined, */
    /* now position the geometric objects */
    const bool has_contour
        = draw_classifier_contour_has_contour( &((*this_).draw_classifier_contour), classifier_type );

    if ( has_contour )
    {
        U8_TRACE_INFO("calculating symbol box as envelope around label and space..." );

        /* calculate symbol bounds */
        const geometry_rectangle_t envelope
            = draw_classifier_contour_calc_outer_bounds( &((*this_).draw_classifier_contour),
                                                         classifier_type,
                                                         &inner_area,
                                                         pencil_size
                                                       );
        layout_visible_classifier_set_symbol_box( io_classifier_layout, &envelope );
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
                                         geometry_rectangle_get_left( &inner_area ),
                                         geometry_rectangle_get_width( &inner_area )
                                       );
        const double symbol_top = geometry_rectangle_get_top( &inner_area ) - symbol_height;
        const geometry_rectangle_t classifier_symbol_box
            = geometry_rectangle_new( symbol_left, symbol_top, symbol_width, symbol_height );
        layout_visible_classifier_set_symbol_box( io_classifier_layout, &classifier_symbol_box );
    }

    U8_TRACE_INFO("==== symbol_box   ====" );
    geometry_rectangle_trace( layout_visible_classifier_get_symbol_box_const( io_classifier_layout ) );
    U8_TRACE_INFO("==== label_box    ====" );
    geometry_rectangle_trace( &label_rect );
    U8_TRACE_INFO("==== icon_box     ====" );
    geometry_rectangle_trace( &icon_rect );
    U8_TRACE_INFO("==== compartments =====" );
    geometry_rectangle_trace( layout_visible_classifier_get_compartments_const( io_classifier_layout ) );
    U8_TRACE_INFO("==== space        =====" );
    geometry_rectangle_trace( layout_visible_classifier_get_space_const( io_classifier_layout ) );

    if ( result_err != PENCIL_ERROR_NONE )
    {
        /* shift: center to requested position after resizing beyond requested size */
        const double shift_to_right = geometry_rectangle_get_center_x( space ) - geometry_rectangle_get_center_x( &classifier_space );
        const double shift_to_bottom = geometry_rectangle_get_center_y( space ) - geometry_rectangle_get_center_y( &classifier_space );
        const geometry_offset_t offset = geometry_offset_new( shift_to_right, shift_to_bottom );
        layout_visible_classifier_shift( io_classifier_layout, &offset );
    }

    geometry_rectangle_destroy( &label_rect );
    geometry_rectangle_destroy( &icon_rect );
    geometry_rectangle_destroy( &label_compartment );

    U8_TRACE_END_ERR(result_err);
    return result_err;
}

pencil_error_t pencil_classifier_composer_private_get_label_box( pencil_classifier_composer_t *this_,
                                                                 const data_visible_classifier_t *visible_classifier,
                                                                 bool shows_contained_children,
                                                                 bool has_stereotype_icon,
                                                                 const geometry_rectangle_t *inner_area,
                                                                 const pencil_size_t *pencil_size,
                                                                 PangoLayout *font_layout,
                                                                 geometry_rectangle_t *out_label_box,
                                                                 geometry_rectangle_t *out_icon_box,
                                                                 geometry_rectangle_t *out_label_compartment )
{
    U8_TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != inner_area );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_label_box );
    assert( NULL != out_icon_box );
    assert( NULL != out_label_compartment );

    pencil_error_t result_err = PENCIL_ERROR_NONE;

    /* get classifier type */
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );

    /* get standard gap size */
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* determine icon space */
    const geometry_dimensions_t icon_dim
        = has_stereotype_icon
        ? draw_stereotype_icon_get_dimensions( &((*this_).draw_stereotype_icon),
                                               pencil_size
                                             )
        : draw_classifier_icon_get_icon_dimensions( &((*this_).draw_classifier_icon),
                                                    classifier_type,
                                                    pencil_size
                                                  );

    /* determine stereotype and name dimensions */
    geometry_dimensions_t label_dim;
    const bool has_contour
        = draw_classifier_contour_has_contour( &((*this_).draw_classifier_contour), classifier_type );
    const double icon_gap
        = ( ! has_contour ) ? 0.0 : ( geometry_dimensions_get_width( &icon_dim ) < 0.000001 ) ? 0.0 : gap;
    const double proposed_label_width
        = has_contour
        ? geometry_rectangle_get_width( inner_area ) - geometry_dimensions_get_width( &icon_dim ) - icon_gap
        : geometry_rectangle_get_width( inner_area );
    const double proposed_label_height = geometry_rectangle_get_height( inner_area );
    const geometry_dimensions_t proposed_label_dim = { .width = proposed_label_width, .height = proposed_label_height };
    draw_classifier_label_get_stereotype_and_name_dimensions( &((*this_).draw_classifier_label),
                                                              visible_classifier,
                                                              ( ! has_stereotype_icon ),
                                                              &proposed_label_dim,
                                                              1,  /* max_recursions */
                                                              pencil_size,
                                                              font_layout,
                                                              &label_dim
                                                            );
    const double text_width = geometry_dimensions_get_width( &label_dim );
    const double text_height = geometry_dimensions_get_height( &label_dim );
    if ( text_width > (proposed_label_width + 0.0001) )
    {
        U8_TRACE_INFO_INT_INT( "label does not fit to provided width",
                               (int) text_width,
                               (int) proposed_label_width
                             );
        result_err = PENCIL_ERROR_OUT_OF_BOUNDS;
    }
    if ( text_height > (proposed_label_height + 0.0001) )
    {
        U8_TRACE_INFO_INT_INT( "label does not fit to provided height",
                               (int) text_height,
                               (int) proposed_label_height
                             );
        result_err = PENCIL_ERROR_OUT_OF_BOUNDS;
    }

    if ( has_contour )
    {
        double top_border = geometry_rectangle_get_top( inner_area );

        /* calculate label_compartment */
        const double min_required_width = text_width + icon_gap + geometry_dimensions_get_width( &icon_dim );
        const double comp_width = u8_f64_max2( min_required_width, geometry_rectangle_get_width( inner_area ) );
        const geometry_h_align_t compartment_h_align = GEOMETRY_H_ALIGN_CENTER;
        const double comp_left = geometry_h_align_get_left( &compartment_h_align,
                                                            comp_width,
                                                            geometry_rectangle_get_left( inner_area ),
                                                            geometry_rectangle_get_width( inner_area )
                                                          );
        geometry_rectangle_reinit( out_label_compartment, comp_left, top_border, comp_width, text_height );

        /* calculate label_box and icon_box */
        const bool is_package_with_contents = (classifier_type == DATA_CLASSIFIER_TYPE_PACKAGE) && shows_contained_children;
        if ( is_package_with_contents )
        {
            /* re-calculate the outer bounds (envelope) to see the diff to top of packages, ignore inner_area */
            const geometry_rectangle_t envelope
                = draw_classifier_contour_calc_outer_bounds( &((*this_).draw_classifier_contour),
                                                             classifier_type,
                                                             out_label_compartment,
                                                             pencil_size
                                                           );
            double comp_top = geometry_rectangle_get_top( &envelope ) + 2.0 * gap;

            /* calculate label_box */
            geometry_rectangle_reinit( out_label_box,
                                       comp_left,
                                       comp_top,
                                       text_width,
                                       text_height
                                     );
            /* calculate icon_box */
            geometry_rectangle_reinit( out_icon_box,
                                       comp_left + text_width + icon_gap,
                                       comp_top,
                                       geometry_dimensions_get_width( &icon_dim ),
                                       geometry_dimensions_get_height( &icon_dim )
                                     );
        }
        else
        {
            const geometry_h_align_t text_h_align = GEOMETRY_H_ALIGN_CENTER;
            const double text_left = geometry_h_align_get_left( &text_h_align,
                                                                text_width,
                                                                geometry_rectangle_get_left( inner_area ),
                                                                geometry_rectangle_get_width( inner_area ) - icon_gap - geometry_dimensions_get_width( &icon_dim )
                                                              );
            /* calculate label_box */
            geometry_rectangle_reinit( out_label_box,
                                       text_left,
                                       top_border,
                                       text_width,
                                       text_height
                                     );
            /* calculate icon_box */
            geometry_rectangle_reinit( out_icon_box,
                                       comp_left + comp_width - geometry_dimensions_get_width( &icon_dim ),
                                       top_border,
                                       geometry_dimensions_get_width( &icon_dim ),
                                       geometry_dimensions_get_height( &icon_dim )
                                     );
        }
    }
    else
    {
        /* calculate text position */
        const geometry_h_align_t h_align_center = GEOMETRY_H_ALIGN_CENTER;
        const double text_left = geometry_h_align_get_left( &h_align_center,
                                                            text_width,
                                                            geometry_rectangle_get_left( inner_area ),
                                                            geometry_rectangle_get_width( inner_area )
                                                          );
        const double text_top = geometry_rectangle_get_top( inner_area );

        /* calculate label_compartment */
        const double comp_width = u8_f64_max2( text_width, geometry_rectangle_get_width( inner_area ) );
        const geometry_h_align_t compartment_h_align = GEOMETRY_H_ALIGN_CENTER;
        const double comp_left = geometry_h_align_get_left( &compartment_h_align,
                                                            comp_width,
                                                            geometry_rectangle_get_left( inner_area ),
                                                            geometry_rectangle_get_width( inner_area )
                                                          );
        geometry_rectangle_reinit( out_label_compartment, comp_left, text_top, comp_width, text_height );

        /* calculate label_box */
        geometry_rectangle_reinit( out_label_box, text_left, text_top, text_width, text_height );
        /* calculate icon_box */
        const double icon_left = geometry_h_align_get_left( &h_align_center,
                                                            geometry_dimensions_get_width( &icon_dim ),
                                                            geometry_rectangle_get_left( inner_area ),
                                                            geometry_rectangle_get_width( inner_area )
                                                          );
        geometry_rectangle_reinit( out_icon_box,
                                   icon_left,
                                   text_top - gap - geometry_dimensions_get_height( &icon_dim ),
                                   geometry_dimensions_get_width( &icon_dim ),
                                   geometry_dimensions_get_height( &icon_dim )
                                 );
    }

    U8_TRACE_END_ERR( result_err );
    return result_err;
}


/*
Copyright 2016-2026 Andreas Warnke
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
