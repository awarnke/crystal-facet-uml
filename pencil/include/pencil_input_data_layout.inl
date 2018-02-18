/* File: pencil_input_data_layout.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

/* ================================ classifiers ================================ */

static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    return &((*this_).classifier_bounds[index]);
}

static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_space_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    return &((*this_).classifier_space[index]);
}

/* ================================ features ================================ */

static inline pencil_visibility_t pencil_input_data_layout_get_feature_visibility ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_MAX_FEATURES );
    return (*this_).feature_visible[index];
}

static inline void pencil_input_data_layout_set_feature_visibility ( pencil_input_data_layout_t *this_, uint32_t index, pencil_visibility_t visible )
{
    assert( index < PENCIL_INPUT_DATA_MAX_FEATURES );
    ((*this_).feature_visible[index]) = visible;
}

static inline geometry_rectangle_t *pencil_input_data_layout_get_feature_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_MAX_FEATURES );
    return &((*this_).feature_bounds[index]);
}

/* ================================ relationships ================================ */

static inline pencil_visibility_t pencil_input_data_layout_get_relationship_visibility ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    return (*this_).relationship_visible[index];
}

static inline void pencil_input_data_layout_set_relationship_visibility ( pencil_input_data_layout_t *this_, uint32_t index, pencil_visibility_t visible )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    ((*this_).relationship_visible[index]) = visible;
}

static inline geometry_connector_t *pencil_input_data_layout_get_relationship_shape_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    return &((*this_).relationship_shape[index]);
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
