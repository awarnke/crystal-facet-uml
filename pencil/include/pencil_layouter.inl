/* File: pencil_layouter.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline pencil_input_data_layout_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).layout_data);
}

static inline pencil_size_t *pencil_layouter_get_pencil_size_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).pencil_size);
}

static inline geometry_rectangle_t *pencil_layouter_get_diagram_bounds_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).diagram_bounds);
}

static inline geometry_rectangle_t pencil_layouter_get_feature_bounds ( pencil_layouter_t *this_,
                                                                        int64_t classifier_id,
                                                                        uint32_t c_index,
                                                                        uint32_t f_index,
                                                                        uint32_t line_index )
{
    geometry_rectangle_t result;

    geometry_rectangle_t *classifier_space;
    classifier_space = pencil_input_data_layout_get_classifier_space_ptr ( &((*this_).layout_data), c_index );

    double lineheight;
    lineheight = pencil_size_get_standard_font_size( &((*this_).pencil_size) )
                 + pencil_size_get_font_line_gap( &((*this_).pencil_size) );

    geometry_rectangle_init ( &result,
                              geometry_rectangle_get_left( classifier_space ),
                              geometry_rectangle_get_top( classifier_space ) + line_index * lineheight,
                              geometry_rectangle_get_width( classifier_space ),
                              lineheight
    );

    return result;
}

static inline pencil_error_t pencil_layouter_get_order_at_pos ( pencil_layouter_t *this_,
                                                                double x,
                                                                double y,
                                                                double snap_distance,
                                                                int32_t *out_order_x,
                                                                int32_t *out_order_y )
{
    assert ( NULL != out_order_x );
    assert ( NULL != out_order_y );
    pencil_error_t result = PENCIL_ERROR_NONE;

    *out_order_x = geometry_non_linear_scale_get_order( &((*this_).x_scale), x, snap_distance );
    *out_order_y = geometry_non_linear_scale_get_order( &((*this_).y_scale), y, snap_distance );
    if ( ! geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) )
    {
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }

    return result;
}

static inline void pencil_layouter_is_pos_on_grid ( pencil_layouter_t *this_,
                                                    double x,
                                                    double y,
                                                    double snap_distance,
                                                    bool *out_x_on_grid,
                                                    bool *out_y_on_grid )
{
    assert ( NULL != out_x_on_grid );
    assert ( NULL != out_y_on_grid );

    double x_dist = geometry_non_linear_scale_get_closest_fix_location( &((*this_).x_scale), x ) - x;
    double y_dist = geometry_non_linear_scale_get_closest_fix_location( &((*this_).y_scale), y ) - y;

    *out_x_on_grid = ((-snap_distance < x_dist)&&( x_dist < snap_distance));
    *out_y_on_grid = ((-snap_distance < y_dist)&&( y_dist < snap_distance));
}


/*
Copyright 2017-2017 Andreas Warnke

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
