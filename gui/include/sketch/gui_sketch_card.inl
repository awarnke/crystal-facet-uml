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

    const double snap_to_grid = gui_sketch_style_get_snap_to_grid( &((*this_).sketch_style) );
    pen_err = pencil_diagram_maker_get_order_at_pos ( &((*this_).painter),
                                                      obj_id,
                                                      (double) x,
                                                      (double) y,
                                                      snap_to_grid,
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

static inline const geometry_grid_t *gui_sketch_card_get_grid_const ( const gui_sketch_card_t *this_ )
{
    return pencil_diagram_maker_get_grid_const( &((*this_).painter) );
}

static inline bool gui_sketch_card_needs_layout( const gui_sketch_card_t *this_ )
{
    return (*this_).dirty_elements_layout;
}

static inline void gui_sketch_card_layout_elements( gui_sketch_card_t *this_, cairo_t *cr )
{
    pencil_diagram_maker_layout_elements ( &((*this_).painter), NULL, cr );
    (*this_).dirty_elements_layout = false;
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

static inline const layout_visible_set_t * gui_sketch_card_get_visible_set( const gui_sketch_card_t *this_ )
{
    return pencil_diagram_maker_get_layout_data_const( &((*this_).painter) );
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
Copyright 2016-2026 Andreas Warnke

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
