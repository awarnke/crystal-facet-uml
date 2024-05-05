/* File: gui_sketch_card.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include "u8/u8_f64.h"
#include <assert.h>

static inline void gui_sketch_card_load_data( gui_sketch_card_t *this_,
                                              data_id_t diagram_id,
                                              data_database_reader_t *db_reader )
{
    /* load data to be drawn */
    const u8_error_t d_err
        = data_visible_set_load( &((*this_).painter_input_data), data_id_get_row_id( &diagram_id ), db_reader );
    if ( d_err != U8_ERROR_NONE )
    {
        U8_TRACE_INFO( "gui_sketch_card_load_data called on invalid database." );
    }
    const u8_error_t p_err
        = data_profile_part_load( &((*this_).profile), &((*this_).painter_input_data), db_reader );
    if ( p_err != U8_ERROR_NONE )
    {
        U8_TRACE_INFO( "data_profile_part_load() returned error." );
    }
}

static inline void gui_sketch_card_invalidate_data( gui_sketch_card_t *this_ )
{
    data_visible_set_invalidate( &((*this_).painter_input_data) );
    data_profile_part_reinit( &((*this_).profile) );

}

static inline bool gui_sketch_card_is_valid( const gui_sketch_card_t *this_ )
{
    return data_visible_set_is_valid( &((*this_).painter_input_data) );
}

static inline shape_int_rectangle_t gui_sketch_card_get_bounds( const gui_sketch_card_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_card_set_bounds( gui_sketch_card_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_card_is_visible( const gui_sketch_card_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_card_set_visible( gui_sketch_card_t *this_, bool visible )
{
    (*this_).visible = visible;
}

static inline void gui_sketch_card_private_draw_rect ( const gui_sketch_card_t *this_,
                                                       const geometry_rectangle_t *rect,
                                                       cairo_t *cr )
{
    assert( rect != NULL );
    assert( cr != NULL );

    static const double WHITE_R = 1.0;
    static const double WHITE_G = 1.0;
    static const double WHITE_B = 1.0;
    static const double WHITE_A = 1.0;
    cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );
    cairo_rectangle( cr,
                     geometry_rectangle_get_left( rect ),
                     geometry_rectangle_get_top( rect ),
                     geometry_rectangle_get_width( rect ),
                     geometry_rectangle_get_height( rect )
                   );
    cairo_fill (cr);
}

static inline void gui_sketch_card_private_draw_border ( const gui_sketch_card_t *this_,
                                                         const geometry_rectangle_t *border,
                                                         const geometry_rectangle_t *space,
                                                         cairo_t *cr )
{
    assert( border != NULL );
    assert( space != NULL );
    assert( cr != NULL );

    /* prepare draw */
    static const double WHITE_R = 1.0;
    static const double WHITE_G = 1.0;
    static const double WHITE_B = 1.0;
    static const double WHITE_A = 1.0;
    cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );

    /* plan to draw */
    {
        const double bold_left = geometry_rectangle_get_left( border );
        const double bold_top = geometry_rectangle_get_top( border );
        const double bold_width = geometry_rectangle_get_width( border );
        const double bold_height = geometry_rectangle_get_height( border );
        const double bold_right = bold_left + bold_width;
        const double bold_bottom = bold_top + bold_height;
        const double void_left = geometry_rectangle_get_left( space );
        const double void_top = geometry_rectangle_get_top( space );
        const double void_width = geometry_rectangle_get_width( space );
        const double void_height = geometry_rectangle_get_height( space );
        const double void_right = void_left + void_width;
        const double void_bottom = void_top + void_height;
        /* fit the empty space into the hightlight space, round to make parts fit together and to not have negative widths/heights */
        const double void_left_adjusted = floor( u8_f64_min2( u8_f64_max2( void_left, bold_left ), bold_right ) );
        const double void_top_adjusted = floor( u8_f64_min2( u8_f64_max2( void_top, bold_top ), bold_bottom ) );
        const double void_right_adjusted = ceil( u8_f64_max2( u8_f64_min2( void_right, bold_right ), bold_left ) );
        const double void_bottom_adjusted = ceil( u8_f64_max2( u8_f64_min2( void_bottom, bold_bottom ), bold_top ) );
        /* draw top region */
        if ( bold_top < void_top )
        {
            cairo_rectangle( cr, bold_left, bold_top, bold_width, void_top_adjusted - bold_top );
        }
        /* draw left region */
        if ( bold_left < void_left )
        {
            cairo_rectangle( cr, bold_left, void_top_adjusted, void_left_adjusted - bold_left, void_bottom_adjusted - void_top_adjusted );
        }
        /* draw right region */
        if ( bold_right > void_right )
        {
            cairo_rectangle( cr, void_right_adjusted, void_top_adjusted, bold_right - void_right_adjusted, void_bottom_adjusted - void_top_adjusted );
        }
        /* draw bottom region */
        if ( bold_bottom > void_bottom )
        {
            cairo_rectangle( cr, bold_left, void_bottom_adjusted, bold_width, bold_bottom - void_bottom_adjusted );
        }
    }

    /* perform draw */
    cairo_fill (cr);
}

static inline const data_diagram_t * gui_sketch_card_get_diagram_const ( const gui_sketch_card_t *this_ )
{
    return data_visible_set_get_diagram_const( &((*this_).painter_input_data) );
}

static inline data_diagram_t * gui_sketch_card_get_diagram_ptr ( gui_sketch_card_t *this_ )
{
    return data_visible_set_get_diagram_ptr( &((*this_).painter_input_data) );
}

static inline data_id_t gui_sketch_card_get_diagram_id ( const gui_sketch_card_t *this_ )
{
    const data_diagram_t *const diag = gui_sketch_card_get_diagram_const( this_ );
    return (diag==NULL) ? DATA_ID_VOID : data_diagram_get_data_id(diag);
}

static inline layout_order_t gui_sketch_card_get_order_at_pos( const gui_sketch_card_t *this_,
                                                               data_id_t obj_id,
                                                               int32_t x,
                                                               int32_t y )
{
    layout_order_t result;
    pencil_error_t pen_err;

    pen_err = pencil_diagram_maker_get_order_at_pos ( &((*this_).painter),
                                                      obj_id,
                                                      (double) x,
                                                      (double) y,
                                                      &result
                                                    );

    switch ( pen_err )
    {
        case PENCIL_ERROR_NONE:
        {
            /* success */
        }
        break;
        case PENCIL_ERROR_OUT_OF_BOUNDS:
        {
            U8_TRACE_INFO( "PENCIL_ERROR_OUT_OF_BOUNDS in gui_sketch_card_get_order_at_pos" );
        }
        break;
        case PENCIL_ERROR_UNKNOWN_OBJECT:
        {
            U8_LOG_ANOMALY( "PENCIL_ERROR_UNKNOWN_OBJECT in gui_sketch_card_get_order_at_pos" );
        }
        break;
    }

    return result;
}

static inline int32_t gui_sketch_card_get_feature_order_at_pos ( const gui_sketch_card_t *this_,
                                                                 const data_feature_t *feature_ptr,
                                                                 int32_t x,
                                                                 int32_t y )
{
    assert ( NULL != feature_ptr );

    layout_order_t result;
    pencil_error_t pen_err;

    pen_err = pencil_diagram_maker_get_feature_order_at_pos ( &((*this_).painter),
                                                              feature_ptr,
                                                              (double) x,
                                                              (double) y,
                                                              &result
                                                            );

    switch ( pen_err )
    {
        case PENCIL_ERROR_NONE:
        {
            /* success */
        }
        break;
        case PENCIL_ERROR_OUT_OF_BOUNDS:
        {
            U8_TRACE_INFO( "PENCIL_ERROR_OUT_OF_BOUNDS in gui_sketch_card_get_feature_order_at_pos" );
        }
        break;
        case PENCIL_ERROR_UNKNOWN_OBJECT:
        {
            U8_LOG_ANOMALY( "PENCIL_ERROR_UNKNOWN_OBJECT in gui_sketch_card_get_feature_order_at_pos" );
        }
        break;
    }

    return layout_order_get_first( &result );
}

static inline gui_sketch_snap_state_t gui_sketch_card_is_pos_on_grid ( const gui_sketch_card_t *this_, int32_t x, int32_t y )
{
    bool x_on_grid;
    bool y_on_grid;

    pencil_diagram_maker_is_pos_on_grid( &((*this_).painter), (double) x, (double) y, &x_on_grid, &y_on_grid );

    const gui_sketch_snap_state_t result
        = x_on_grid
        ? ( y_on_grid ? GUI_SKETCH_SNAP_STATE_XY : GUI_SKETCH_SNAP_STATE_X )
        : ( y_on_grid ? GUI_SKETCH_SNAP_STATE_Y : GUI_SKETCH_SNAP_STATE_0 );
    return result;
}

static inline void gui_sketch_card_get_grid_area ( const gui_sketch_card_t *this_,
                                                   shape_int_rectangle_t *out_bounds,
                                                   uint32_t *out_x_count,
                                                   uint32_t *out_y_count
                                                 )
{
    assert( out_bounds != NULL );
    assert( out_x_count != NULL );
    assert( out_y_count != NULL );

    double x0,y0,dx,dy;

    pencil_diagram_maker_get_grid_lines( &((*this_).painter), &x0, &y0, &dx, &dy, out_x_count, out_y_count );

    shape_int_rectangle_init( out_bounds,
                              (int32_t) x0,
                              (int32_t) y0,
                              (uint32_t) ((*out_x_count>2) ? ((*out_x_count-1)*dx) : dx),
                              (uint32_t) ((*out_y_count>2) ? ((*out_y_count-1)*dy) : dy)
                            );
}

static inline void gui_sketch_card_do_layout( gui_sketch_card_t *this_, cairo_t *cr )
{
    if ( gui_sketch_card_is_valid( this_ ) )
    {
        /* layout loaded classifiers */
        const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
        const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
        const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
        const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

        geometry_rectangle_t destination;
        geometry_rectangle_init( &destination, left, top, width, height );

        pencil_diagram_maker_define_grid ( &((*this_).painter), destination, cr );
        pencil_diagram_maker_layout_elements ( &((*this_).painter), NULL, cr );
        (*this_).dirty_elements_layout = false;

        geometry_rectangle_destroy( &destination );
    }
    else
    {
        U8_TRACE_INFO( "gui_sketch_card_do_layout called on invalid card." );
    }
}

static inline int32_t gui_sketch_card_get_highest_rel_list_order( const gui_sketch_card_t *this_ )
{
    int32_t result = 0;

    if ( data_visible_set_is_valid( &((*this_).painter_input_data) ) )
    {
        for ( uint32_t rs_idx = 0; rs_idx < data_visible_set_get_relationship_count( &((*this_).painter_input_data) ); rs_idx ++ )
        {
            const data_relationship_t *relation;
            relation = data_visible_set_get_relationship_const ( &((*this_).painter_input_data), rs_idx );
            if ( data_relationship_get_list_order( relation ) > result )
            {
                result = data_relationship_get_list_order( relation );
            }
        }
    }

    return result;
}

static inline int32_t gui_sketch_card_get_highest_feat_list_order( const gui_sketch_card_t *this_, data_id_t classifier_id )
{
    int32_t result = 0;

    if ( data_visible_set_is_valid( &((*this_).painter_input_data) ) )
    {
        for ( uint32_t f_idx = 0; f_idx < data_visible_set_get_feature_count( &((*this_).painter_input_data) ); f_idx ++ )
        {
            const data_feature_t *const feat
                = data_visible_set_get_feature_const ( &((*this_).painter_input_data), f_idx );
            assert( feat != NULL );
            const data_feature_type_t f_type = data_feature_get_main_type( feat );
            if (( f_type == DATA_FEATURE_TYPE_PROPERTY )||( f_type == DATA_FEATURE_TYPE_OPERATION ))
            {
                if ( data_feature_get_classifier_row_id( feat ) == data_id_get_row_id( &classifier_id ) )
                {
                    if ( data_feature_get_list_order( feat ) > result )
                    {
                        result = data_feature_get_list_order( feat );
                    }
                }
            }
        }
    }

    return result;
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
