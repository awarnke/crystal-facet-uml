/* File: layout_feature.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_feature_init_empty ( layout_feature_t *this_, data_feature_t *feature_data )
{
    geometry_rectangle_init_empty( &((*this_).feature_bounds) );
    (*this_).feature_data = feature_data;
}

static inline void layout_feature_destroy ( layout_feature_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).feature_bounds) );
    (*this_).feature_data = NULL;
}

static inline geometry_rectangle_t *layout_feature_get_bounds_ptr ( layout_feature_t *this_ )
{
    return &((*this_).feature_bounds);
}

static inline data_feature_t *layout_feature_get_data_ptr ( const layout_feature_t *this_ )
{
    assert ( NULL != (*this_).feature_data );
    return (*this_).feature_data;
}


/*
Copyright 2018-2018 Andreas Warnke

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
