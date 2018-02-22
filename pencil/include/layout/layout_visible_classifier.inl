/* File: layout_visible_classifier.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_visible_classifier_init_empty ( layout_visible_classifier_t *this_, data_visible_classifier_t *visible_classifier_data )
{
    geometry_rectangle_init_empty( &((*this_).classifier_bounds) );
    geometry_rectangle_init_empty( &((*this_).classifier_space) );
    (*this_).visible_classifier_data = visible_classifier_data;
}

static inline void layout_visible_classifier_destroy ( layout_visible_classifier_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).classifier_bounds) );
    geometry_rectangle_destroy( &((*this_).classifier_space) );
    (*this_).visible_classifier_data = NULL;
}

static inline geometry_rectangle_t *layout_visible_classifier_get_bounds_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).classifier_bounds);
}

static inline geometry_rectangle_t *layout_visible_classifier_get_space_ptr ( layout_visible_classifier_t *this_ )
{
    return &((*this_).classifier_space);
}

static inline data_visible_classifier_t *layout_visible_classifier_get_data_ptr ( layout_visible_classifier_t *this_ )
{
    assert ( NULL != (*this_).visible_classifier_data );
    return (*this_).visible_classifier_data;
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
