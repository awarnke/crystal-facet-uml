/* File: pencil_input_data_layout.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

/* ================================ diagram ================================ */

static inline layout_diagram_t *pencil_input_data_layout_get_diagram_layout_ptr ( pencil_input_data_layout_t *this_ )
{
    /*assert ( (*this_).diagram_valid );*/ /* we return the pointer even if diagram_layout is not yet initialized */
    return &((*this_).diagram_layout);
}

/* ================================ classifiers ================================ */

static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    assert ( index < (*this_).visible_classifier_count );
    return layout_visible_classifier_get_bounds_ptr ( &((*this_).visible_classifier_layout[index]) );
}

static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_space_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    assert ( index < (*this_).visible_classifier_count );
    return layout_visible_classifier_get_space_ptr ( &((*this_).visible_classifier_layout[index]) );
}

static inline uint32_t pencil_input_data_layout_get_visible_classifier_count ( pencil_input_data_layout_t *this_ )
{
    return (*this_).visible_classifier_count;
}

static inline layout_visible_classifier_t *pencil_input_data_layout_get_classifier_layout_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    assert ( index < (*this_).visible_classifier_count );
    return &((*this_).visible_classifier_layout[index]);
}

/* ================================ features ================================ */

static inline geometry_rectangle_t *pencil_input_data_layout_get_feature_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_MAX_FEATURES );
    assert ( index < (*this_).feature_count );
    return layout_feature_get_bounds_ptr ( &((*this_).feature_layout[index]) );
}

static inline uint32_t pencil_input_data_layout_get_feature_count ( pencil_input_data_layout_t *this_ )
{
    return (*this_).feature_count;
}

static inline layout_feature_t *pencil_input_data_layout_get_feature_layout_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_MAX_FEATURES );
    assert ( index < (*this_).feature_count );
    return &((*this_).feature_layout[index]);
}

/* ================================ relationships ================================ */

static inline pencil_visibility_t pencil_input_data_layout_get_relationship_visibility ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    assert ( index < (*this_).relationship_count );
    return layout_relationship_get_visibility ( &((*this_).relationship_layout[index]) );
}

static inline void pencil_input_data_layout_set_relationship_visibility ( pencil_input_data_layout_t *this_, uint32_t index, pencil_visibility_t visible )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    assert ( index < (*this_).relationship_count );
    layout_relationship_set_visibility ( &((*this_).relationship_layout[index]), visible );
}

static inline geometry_connector_t *pencil_input_data_layout_get_relationship_shape_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    assert ( index < (*this_).relationship_count );
    return layout_relationship_get_shape_ptr ( &((*this_).relationship_layout[index]) );
}

static inline uint32_t pencil_input_data_layout_get_relationship_count ( pencil_input_data_layout_t *this_ )
{
    return (*this_).relationship_count;
}

static inline layout_relationship_t *pencil_input_data_layout_get_relationship_layout_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    assert ( index < (*this_).relationship_count );
    return &((*this_).relationship_layout[index]);
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
