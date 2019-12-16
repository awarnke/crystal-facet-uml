/* File: pencil_layouter.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void pencil_layouter_prepare ( pencil_layouter_t *this_ )
{
    pencil_layout_data_resync( &((*this_).layout_data) );
}

static inline pencil_layout_data_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).layout_data);
}

static inline pencil_size_t *pencil_layouter_get_pencil_size_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).pencil_size);
}

static inline void pencil_layouter_is_pos_on_grid ( const pencil_layouter_t *this_,
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
Copyright 2017-2019 Andreas Warnke

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
