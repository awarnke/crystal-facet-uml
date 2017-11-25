/* File: pencil_input_data.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ )
{
    return &((*this_).diagram);
}

static inline bool pencil_input_data_is_valid ( pencil_input_data_t *this_ )
{
    return data_diagram_is_valid( &((*this_).diagram) );
}

static inline void pencil_input_data_invalidate ( pencil_input_data_t *this_ )
{
    data_diagram_reinit_empty( &((*this_).diagram) );
}

static inline void pencil_input_data_private_destroy_visible_classifiers( pencil_input_data_t *this_ )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_destroy ( &((*this_).visible_classifiers[index]) );
    }

    (*this_).visible_classifier_count = 0;
}

static inline void pencil_input_data_private_destroy_features( pencil_input_data_t *this_ )
{
    assert( (*this_).feature_count <= PENCIL_INPUT_DATA_MAX_FEATURES );

    for ( int index = 0; index < (*this_).feature_count; index ++ )
    {
        data_feature_destroy ( &((*this_).features[index]) );
    }

    (*this_).feature_count = 0;
}

static inline void pencil_input_data_private_destroy_relationships( pencil_input_data_t *this_ )
{
    assert( (*this_).relationship_count <= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );

    for ( int index = 0; index < (*this_).relationship_count; index ++ )
    {
        data_relationship_destroy ( &((*this_).relationships[index]) );
    }

    (*this_).relationship_count = 0;
}

static inline uint32_t pencil_input_data_get_visible_classifier_count ( pencil_input_data_t *this_ )
{
    return (*this_).visible_classifier_count;
}

static inline data_visible_classifier_t *pencil_input_data_get_visible_classifier_ptr ( pencil_input_data_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    data_visible_classifier_t *result;
    if ( index < (*this_).visible_classifier_count )
    {
        result = &((*this_).visible_classifiers[index]);
    }
    else
    {
        result = NULL;
        TSLOG_ERROR_INT( "index out of bounds (>=(*this_).visible_classifier_count)", index );
    }

    return result;
}

static inline data_classifier_t *pencil_input_data_get_classifier_ptr ( pencil_input_data_t *this_, int64_t row_id )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    data_classifier_t *result = NULL;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_ptr( &((*this_).visible_classifiers[index]) );
        if ( row_id == data_classifier_get_id( probe ) )
        {
            result = probe;
        }
    }

    return result;
}

static inline int32_t pencil_input_data_get_classifier_index ( pencil_input_data_t *this_, int64_t row_id )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    int32_t result = -1;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_ptr( &((*this_).visible_classifiers[index]) );
        if ( row_id == data_classifier_get_id( probe ) )
        {
            result = index;
        }
    }

    return result;
}

static inline uint32_t pencil_input_data_get_feature_count ( pencil_input_data_t *this_ )
{
    return (*this_).feature_count;
}

static inline data_feature_t *pencil_input_data_get_feature_ptr ( pencil_input_data_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_FEATURES );

    data_feature_t *result;
    if ( index < (*this_).feature_count )
    {
        result = &((*this_).features[index]);
    }
    else
    {
        result = NULL;
        TSLOG_ERROR_INT( "index out of bounds (>=(*this_).feature_count)", index );
    }

    return result;
}

static inline data_feature_t *pencil_input_data_get_feature_list_ptr ( pencil_input_data_t *this_ )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_FEATURES );
    return (*this_).features;
}

static inline uint32_t pencil_input_data_get_relationship_count ( pencil_input_data_t *this_ )
{
    return (*this_).relationship_count;
}

static inline data_relationship_t *pencil_input_data_get_relationship_ptr ( pencil_input_data_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );

    data_relationship_t *result;
    if ( index < (*this_).relationship_count )
    {
        result = &((*this_).relationships[index]);
    }
    else
    {
        result = NULL;
        TSLOG_ERROR_INT( "index out of bounds (>=(*this_).relationship_count)", index );
    }

    return result;
}

static inline bool pencil_input_data_is_parent_by_index ( pencil_input_data_t *this_, uint32_t parent_index, uint32_t child_index )
{
    assert( parent_index < PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    assert( child_index < PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    bool result = false;

    data_visible_classifier_t *visible_parent;
    data_visible_classifier_t *visible_child;
    visible_parent = pencil_input_data_get_visible_classifier_ptr ( this_, parent_index );
    visible_child = pencil_input_data_get_visible_classifier_ptr ( this_, child_index );
    data_classifier_t *parent;
    data_classifier_t *child;
    parent = data_visible_classifier_get_classifier_ptr( visible_parent );
    child = data_visible_classifier_get_classifier_ptr( visible_child );

    result = pencil_input_data_is_parent ( this_, data_classifier_get_id( parent ), data_classifier_get_id( child ) );

    return result;
}

static inline bool pencil_input_data_is_ancestor_by_index ( pencil_input_data_t *this_, uint32_t ancestor_index, uint32_t descendant_index )
{
    assert( ancestor_index < PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    assert( descendant_index < PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    bool result = false;

    data_visible_classifier_t *visible_ancestor;
    data_visible_classifier_t *visible_descendant;
    visible_ancestor = pencil_input_data_get_visible_classifier_ptr ( this_, ancestor_index );
    visible_descendant = pencil_input_data_get_visible_classifier_ptr ( this_, descendant_index );
    data_classifier_t *ancestor;
    data_classifier_t *descendant;
    ancestor = data_visible_classifier_get_classifier_ptr( visible_ancestor );
    descendant = data_visible_classifier_get_classifier_ptr( visible_descendant );

    result = pencil_input_data_is_ancestor ( this_, data_classifier_get_id( ancestor ), data_classifier_get_id( descendant ) );
    
    return result;
}


/*
Copyright 2016-2017 Andreas Warnke

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
