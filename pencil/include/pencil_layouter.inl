/* File: pencil_layouter.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "geometry/geometry_non_linear_scale.h"
#include <assert.h>

static inline void pencil_layouter_prepare ( pencil_layouter_t *this_ )
{
    layout_visible_set_resync( &((*this_).layout_data) );
    pencil_feature_layouter_reset( &((*this_).feature_layouter) );
}

static inline const geometry_grid_t *pencil_layouter_get_grid_const ( const pencil_layouter_t *this_ )
{
    return &((*this_).grid);
}

static inline layout_visible_set_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).layout_data);
}

static inline const layout_visible_set_t *pencil_layouter_get_layout_data_const ( const pencil_layouter_t *this_ )
{
    return &((*this_).layout_data);
}

static inline const pencil_size_t *pencil_layouter_get_pencil_size_const ( pencil_layouter_t *this_ )
{
    return &((*this_).pencil_size);
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
