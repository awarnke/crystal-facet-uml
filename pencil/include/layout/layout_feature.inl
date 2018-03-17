/* File: layout_feature.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_feature_init ( layout_feature_t *this_,
                                         const data_feature_t *feature_data,
                                         layout_visible_classifier_t *classifier )
{
    assert ( NULL != feature_data );
    assert ( NULL != classifier );

    geometry_rectangle_init_empty( &((*this_).bounds) );
    (*this_).direction = PENCIL_LAYOUT_DIRECTION_CENTER;
    (*this_).data = feature_data;
    (*this_).classifier = classifier;
}

static inline void layout_feature_destroy ( layout_feature_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).bounds) );
    (*this_).data = NULL;
}

static inline geometry_rectangle_t *layout_feature_get_bounds_ptr ( layout_feature_t *this_ )
{
    return &((*this_).bounds);
}

static inline void layout_feature_set_bounds ( layout_feature_t *this_, const geometry_rectangle_t *feature_bounds )
{
    geometry_rectangle_replace( &((*this_).bounds), feature_bounds );
}

static inline layout_direction_t layout_feature_get_direction ( const layout_feature_t *this_ )
{
    return (*this_).direction;
}

static inline void layout_feature_set_direction ( layout_feature_t *this_, layout_direction_t direction )
{
    (*this_).direction = direction;
}

static inline const data_feature_t *layout_feature_get_data_ptr ( const layout_feature_t *this_ )
{
    return (*this_).data;
}

static inline layout_visible_classifier_t *layout_feature_get_classifier_ptr ( layout_feature_t *this_ )
{
    return (*this_).classifier;
}

static inline int64_t layout_feature_get_feature_id ( const layout_feature_t *this_ )
{
    return data_feature_get_id( (*this_).data );
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
