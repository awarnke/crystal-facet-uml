/* File: data_visible_set.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline data_diagram_t *data_visible_set_get_diagram_ptr ( data_visible_set_t *this_ )
{
    return &((*this_).diagram);
}

static inline bool data_visible_set_is_valid ( const data_visible_set_t *this_ )
{
    return data_diagram_is_valid( &((*this_).diagram) );
}

static inline void data_visible_set_invalidate ( data_visible_set_t *this_ )
{
    data_diagram_reinit_empty( &((*this_).diagram) );
}

static inline void data_visible_set_private_destroy_visible_classifiers( data_visible_set_t *this_ )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_destroy ( &((*this_).visible_classifiers[index]) );
    }

    (*this_).visible_classifier_count = 0;
}

static inline void data_visible_set_private_destroy_features( data_visible_set_t *this_ )
{
    assert( (*this_).feature_count <= DATA_VISIBLE_SET_MAX_FEATURES );

    for ( int index = 0; index < (*this_).feature_count; index ++ )
    {
        data_feature_destroy ( &((*this_).features[index]) );
    }

    (*this_).feature_count = 0;
}

static inline void data_visible_set_private_destroy_relationships( data_visible_set_t *this_ )
{
    assert( (*this_).relationship_count <= DATA_VISIBLE_SET_MAX_RELATIONSHIPS );

    for ( int index = 0; index < (*this_).relationship_count; index ++ )
    {
        data_relationship_destroy ( &((*this_).relationships[index]) );
    }

    (*this_).relationship_count = 0;
}

static inline uint32_t data_visible_set_get_visible_classifier_count ( data_visible_set_t *this_ )
{
    return (*this_).visible_classifier_count;
}

static inline data_visible_classifier_t *data_visible_set_get_visible_classifier_ptr ( data_visible_set_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );

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

static inline data_visible_classifier_t *data_visible_set_get_visible_classifier_by_id_ptr ( data_visible_set_t *this_, int64_t diagramelement_id )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );
    data_visible_classifier_t *result = NULL;

    /* iterate over all visible classifiers */
    for ( uint32_t index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);
        assert ( data_visible_classifier_is_valid( visible_classifier ) );

        data_diagramelement_t *diagramelement;
        diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );
        if ( data_diagramelement_get_id( diagramelement ) == diagramelement_id )
        {
            result = visible_classifier;
            break;
        }
    }

    return result;
}

static inline data_classifier_t *data_visible_set_get_classifier_by_id_ptr ( data_visible_set_t *this_, int64_t row_id )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );
    data_classifier_t *result = NULL;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);
        assert ( data_visible_classifier_is_valid( visible_classifier ) );

        data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_ptr( visible_classifier );
        if ( row_id == data_classifier_get_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline int32_t data_visible_set_get_classifier_index ( data_visible_set_t *this_, int64_t row_id )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );
    int32_t result = -1;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_ptr( &((*this_).visible_classifiers[index]) );
        if ( row_id == data_classifier_get_id( probe ) )
        {
            result = index;
            break;
        }
    }

    return result;
}

static inline uint32_t data_visible_set_get_classifier_index_from_pointer ( const data_visible_set_t *this_,
                                                                             const data_visible_classifier_t *classifier_ptr )
{
    assert ( NULL != classifier_ptr );  /* input parameters test */
    assert ( classifier_ptr >= &((*this_).visible_classifiers[0]) );  /* input parameters test */
    assert ( classifier_ptr < &((*this_).visible_classifiers[DATA_VISIBLE_SET_MAX_CLASSIFIERS]) );  /* input parameters test */
    assert ( 3 == &((*this_).visible_classifiers[3]) - (*this_).visible_classifiers );  /* compiler test */
    return ( classifier_ptr - (*this_).visible_classifiers );
}

static inline uint32_t data_visible_set_get_feature_count ( data_visible_set_t *this_ )
{
    return (*this_).feature_count;
}

static inline data_feature_t *data_visible_set_get_feature_ptr ( data_visible_set_t *this_, uint32_t index )
{
    assert( (*this_).feature_count <= DATA_VISIBLE_SET_MAX_FEATURES );

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

static inline data_feature_t *data_visible_set_get_feature_by_id_ptr ( data_visible_set_t *this_, int64_t row_id )
{
    assert( (*this_).feature_count <= DATA_VISIBLE_SET_MAX_FEATURES );
    data_feature_t *result = NULL;

    for ( int index = 0; index < (*this_).feature_count; index ++ )
    {
        data_feature_t *probe;
        probe = &((*this_).features[index]);
        if ( row_id == data_feature_get_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline data_feature_t *data_visible_set_get_feature_list_ptr ( data_visible_set_t *this_ )
{
    assert( (*this_).feature_count <= DATA_VISIBLE_SET_MAX_FEATURES );
    return (*this_).features;
}

static inline uint32_t data_visible_set_get_relationship_count ( data_visible_set_t *this_ )
{
    return (*this_).relationship_count;
}

static inline data_relationship_t *data_visible_set_get_relationship_ptr ( data_visible_set_t *this_, uint32_t index )
{
    assert( (*this_).relationship_count <= DATA_VISIBLE_SET_MAX_RELATIONSHIPS );

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

static inline data_relationship_t *data_visible_set_get_relationship_by_id_ptr ( data_visible_set_t *this_, int64_t row_id )
{
    assert( (*this_).relationship_count <= DATA_VISIBLE_SET_MAX_RELATIONSHIPS );
    data_relationship_t *result = NULL;

    for ( int index = 0; index < (*this_).relationship_count; index ++ )
    {
        data_relationship_t *probe;
        probe = &((*this_).relationships[index]);
        if ( row_id == data_relationship_get_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline bool data_visible_set_is_ancestor_by_index ( const data_visible_set_t *this_, uint32_t ancestor_index, uint32_t descendant_index )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );
    assert( ancestor_index < (*this_).visible_classifier_count );
    assert( descendant_index < (*this_).visible_classifier_count );

    return (*this_).containment_cache[ancestor_index][descendant_index];
}

static inline uint32_t data_visible_set_count_ancestors_of_index ( const data_visible_set_t *this_, uint32_t classifier_index )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );
    assert( classifier_index < (*this_).visible_classifier_count );

    uint32_t result = 0;

    for ( uint32_t ancestor_index = 0; ancestor_index < (*this_).visible_classifier_count; ancestor_index ++ )
    {
        if ( (*this_).containment_cache[ancestor_index][classifier_index] )
        {
            result ++;
        }
    }

    return result;
}

static inline uint32_t data_visible_set_count_descendants_of_index ( const data_visible_set_t *this_, uint32_t classifier_index )
{
    assert( (*this_).visible_classifier_count <= DATA_VISIBLE_SET_MAX_CLASSIFIERS );
    assert( classifier_index < (*this_).visible_classifier_count );

    uint32_t result = 0;

    for ( uint32_t descendant_index = 0; descendant_index < (*this_).visible_classifier_count; descendant_index ++ )
    {
        if ( (*this_).containment_cache[classifier_index][descendant_index] )
        {
            result ++;
        }
    }

    return result;
}


/*
Copyright 2016-2019 Andreas Warnke

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
