/* File: layout_relationship.inl; Copyright and License: see below */

#include <assert.h>

static inline void layout_relationship_init ( layout_relationship_t *this_,
                                              const data_relationship_t *relationship_data,
                                              layout_visible_classifier_t *from_classifier,
                                              layout_visible_classifier_t *to_classifier,
                                              layout_feature_t *from_feature,
                                              layout_feature_t *to_feature )
{
    assert ( NULL != relationship_data );
    assert ( NULL != from_classifier );
    assert ( NULL != to_classifier );

    (*this_).visible = PENCIL_VISIBILITY_HIDE;
    geometry_connector_init_empty( &((*this_).shape) );
    (*this_).data = relationship_data;
    (*this_).from_classifier = from_classifier;
    (*this_).to_classifier = to_classifier;
    (*this_).from_feature = from_feature;
    (*this_).to_feature = to_feature;
}

static inline void layout_relationship_destroy ( layout_relationship_t *this_ )
{
    geometry_connector_destroy( &((*this_).shape) );
    (*this_).data = NULL;
}

static inline pencil_visibility_t layout_relationship_get_visibility ( const layout_relationship_t *this_ )
{
    return (*this_).visible;
}

static inline void layout_relationship_set_visibility ( layout_relationship_t *this_, pencil_visibility_t visible )
{
    (*this_).visible = visible;
}

static inline geometry_connector_t *layout_relationship_get_shape_ptr ( layout_relationship_t *this_ )
{
    return &((*this_).shape);
}

static inline const data_relationship_t *layout_relationship_get_data_ptr ( const layout_relationship_t *this_ )
{
    assert ( NULL != (*this_).data );
    return (*this_).data;
}

static inline layout_visible_classifier_t *layout_relationship_get_from_classifier_ptr ( layout_relationship_t *this_ )
{
    return (*this_).from_classifier;
}

static inline layout_visible_classifier_t *layout_relationship_get_to_classifier_ptr ( layout_relationship_t *this_ )
{
    return (*this_).to_classifier;
}

static inline layout_feature_t *layout_relationship_get_from_feature_ptr ( layout_relationship_t *this_ )
{
    return (*this_).from_feature;
}

static inline layout_feature_t *layout_relationship_get_to_feature_ptr ( layout_relationship_t *this_ )
{
    return (*this_).to_feature;
}

static inline int64_t layout_relationship_get_relationship_id ( const layout_relationship_t *this_ )
{
    return data_relationship_get_id( (*this_).data );
}

static inline const geometry_rectangle_t *layout_relationship_get_from_bounds_const ( const layout_relationship_t *this_ )
{
    const geometry_rectangle_t *result;

    if ( NULL != (*this_).from_feature )
    {
        result = layout_feature_get_bounds_ptr( (*this_).from_feature );
    }
    else
    {
        result = layout_visible_classifier_get_bounds_ptr( (*this_).from_classifier );
    }

    return result;
}

static inline const geometry_rectangle_t *layout_relationship_get_to_bounds_const ( const layout_relationship_t *this_ )
{
    const geometry_rectangle_t *result;

    if ( NULL != (*this_).to_feature )
    {
        result = layout_feature_get_bounds_ptr( (*this_).to_feature );
    }
    else
    {
        result = layout_visible_classifier_get_bounds_ptr( (*this_).to_classifier );
    }

    return result;
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
