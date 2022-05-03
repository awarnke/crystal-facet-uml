/* File: data_node_set.inl; Copyright and License: see below */

#include "tslog/tslog.h"
#include <assert.h>

static inline bool data_node_set_is_valid ( const data_node_set_t *this_ )
{
    return data_classifier_is_valid( &((*this_).classifier) );
}

static inline void data_node_set_invalidate ( data_node_set_t *this_ )
{
    data_classifier_reinit_empty( &((*this_).classifier) );
}

static inline void data_node_set_private_destroy_features( data_node_set_t *this_ )
{
    assert( (*this_).feature_count <= DATA_NODE_SET_MAX_FEATURES );

    for ( int index = 0; index < (*this_).feature_count; index ++ )
    {
        data_feature_destroy ( &((*this_).features[index]) );
    }

    (*this_).feature_count = 0;
}

static inline void data_node_set_private_destroy_relationships( data_node_set_t *this_ )
{
    assert( (*this_).relationship_count <= DATA_NODE_SET_MAX_RELATIONSHIPS );

    for ( int index = 0; index < (*this_).relationship_count; index ++ )
    {
        data_relationship_destroy ( &((*this_).relationships[index]) );
    }

    (*this_).relationship_count = 0;
}

static inline const data_classifier_t *data_node_set_get_classifier_const ( const data_node_set_t *this_ )
{
    return &((*this_).classifier);
}

static inline data_classifier_t *data_node_set_get_classifier_ptr ( data_node_set_t *this_ )
{
    return &((*this_).classifier);
}

static inline uint32_t data_node_set_get_feature_count ( const data_node_set_t *this_ )
{
    return (*this_).feature_count;
}

static inline const data_feature_t *data_node_set_get_feature_const ( const data_node_set_t *this_, uint32_t index )
{
    assert( (*this_).feature_count <= DATA_NODE_SET_MAX_FEATURES );

    const data_feature_t *result;
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

static inline data_feature_t *data_node_set_get_feature_ptr ( data_node_set_t *this_, uint32_t index )
{
    assert( (*this_).feature_count <= DATA_NODE_SET_MAX_FEATURES );

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

static inline const data_feature_t *data_node_set_get_feature_by_id_const ( const data_node_set_t *this_, data_row_id_t row_id )
{
    assert( (*this_).feature_count <= DATA_NODE_SET_MAX_FEATURES );
    const data_feature_t *result = NULL;

    for ( int index = 0; index < (*this_).feature_count; index ++ )
    {
        const data_feature_t *probe;
        probe = &((*this_).features[index]);
        if ( row_id == data_feature_get_row_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline data_feature_t *data_node_set_get_feature_by_id_ptr ( data_node_set_t *this_, data_row_id_t row_id )
{
    assert( (*this_).feature_count <= DATA_NODE_SET_MAX_FEATURES );
    data_feature_t *result = NULL;

    for ( int index = 0; index < (*this_).feature_count; index ++ )
    {
        data_feature_t *probe;
        probe = &((*this_).features[index]);
        if ( row_id == data_feature_get_row_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline data_feature_t *data_node_set_get_feature_list_ptr ( data_node_set_t *this_ )
{
    assert( (*this_).feature_count <= DATA_NODE_SET_MAX_FEATURES );
    return (*this_).features;
}

static inline uint32_t data_node_set_get_relationship_count ( const data_node_set_t *this_ )
{
    return (*this_).relationship_count;
}

static inline const data_relationship_t *data_node_set_get_relationship_const ( const data_node_set_t *this_, uint32_t index )
{
    assert( (*this_).relationship_count <= DATA_NODE_SET_MAX_RELATIONSHIPS );

    const data_relationship_t *result;
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

static inline data_relationship_t *data_node_set_get_relationship_ptr ( data_node_set_t *this_, uint32_t index )
{
    assert( (*this_).relationship_count <= DATA_NODE_SET_MAX_RELATIONSHIPS );

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

static inline const data_relationship_t *data_node_set_get_relationship_by_id_const ( const data_node_set_t *this_, data_row_id_t row_id )
{
    assert( (*this_).relationship_count <= DATA_NODE_SET_MAX_RELATIONSHIPS );
    const data_relationship_t *result = NULL;

    for ( int index = 0; index < (*this_).relationship_count; index ++ )
    {
        const data_relationship_t *probe;
        probe = &((*this_).relationships[index]);
        if ( row_id == data_relationship_get_row_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline data_relationship_t *data_node_set_get_relationship_by_id_ptr ( data_node_set_t *this_, data_row_id_t row_id )
{
    assert( (*this_).relationship_count <= DATA_NODE_SET_MAX_RELATIONSHIPS );
    data_relationship_t *result = NULL;

    for ( int index = 0; index < (*this_).relationship_count; index ++ )
    {
        data_relationship_t *probe;
        probe = &((*this_).relationships[index]);
        if ( row_id == data_relationship_get_row_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}


/*
Copyright 2020-2022 Andreas Warnke

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
