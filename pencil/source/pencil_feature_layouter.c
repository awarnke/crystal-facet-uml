/* File: pencil_feature_layouter.c; Copyright and License: see below */

#include "pencil_feature_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_feature_layouter_init( pencil_feature_layouter_t *this_,
                                   pencil_input_data_t *input_data,
                                   pencil_input_data_layout_t *layout_data,
                                   pencil_size_t *pencil_size )
{
    TRACE_BEGIN();
    assert( NULL != input_data );
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).input_data = input_data;
    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;
    pencil_feature_painter_init( &((*this_).feature_painter) );

    TRACE_END();

}

void pencil_feature_layouter_destroy( pencil_feature_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_feature_painter_destroy( &((*this_).feature_painter) );

    TRACE_END();
}

void pencil_feature_layouter_calculate_features_bounds ( pencil_feature_layouter_t *this_,
                                                         int64_t classifier_id,
                                                         PangoLayout *font_layout,
                                                         geometry_rectangle_t *out_features_bounds )
{
    TRACE_BEGIN();
    assert( NULL != font_layout );
    assert( NULL != out_features_bounds );

    double left = 0.0;
    double top = 0.0;
    double width = 0.0;
    double height = 0.0;

    /* search all contained features */
    for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count ( (*this_).input_data ); f_idx ++ )
    {
        data_feature_t *the_feature;
        the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, f_idx );
        if ( data_feature_is_valid( the_feature ) )
        {
            if ( data_feature_get_classifier_id( the_feature ) == classifier_id )
            {
                geometry_rectangle_t min_feature_bounds;
                geometry_rectangle_init_empty( &min_feature_bounds );
                pencil_feature_painter_get_minimum_bounds ( &((*this_).feature_painter),
                                                            the_feature,
                                                            (*this_).pencil_size,
                                                            font_layout,
                                                            &min_feature_bounds
                                                        );

                double current_w = geometry_rectangle_get_width( &min_feature_bounds );
                width = ( width < current_w ) ? current_w : width;
                height += geometry_rectangle_get_height( &min_feature_bounds );

                geometry_rectangle_destroy( &min_feature_bounds );
            }
        }
        else
        {
            TSLOG_ERROR("invalid feature in array!");
        }
    }

    geometry_rectangle_reinit( out_features_bounds, left, top, width, height );
    TRACE_END();
}


/*
Copyright 2017-2018 Andreas Warnke

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
