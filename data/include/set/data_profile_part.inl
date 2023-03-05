/* File: data_profile_part.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

/* ================================ classifiers ================================ */

static inline uint32_t data_profile_part_get_visible_classifier_count ( const data_profile_part_t *this_ )
{
    return (*this_).visible_classifier_count;
}

static inline const data_visible_classifier_t *data_profile_part_get_visible_classifier_const ( const data_profile_part_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );

    const data_visible_classifier_t *result;
    if ( index < (*this_).visible_classifier_count )
    {
        result = &((*this_).visible_classifiers[index]);
    }
    else
    {
        result = NULL;
        U8_LOG_ERROR_INT( "index out of bounds (>=(*this_).visible_classifier_count)", index );
    }

    return result;
}

static inline data_visible_classifier_t *data_profile_part_get_visible_classifier_ptr ( data_profile_part_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );

    data_visible_classifier_t *result;
    if ( index < (*this_).visible_classifier_count )
    {
        result = &((*this_).visible_classifiers[index]);
    }
    else
    {
        result = NULL;
        U8_LOG_ERROR_INT( "index out of bounds (>=(*this_).visible_classifier_count)", index );
    }

    return result;
}

static inline const data_visible_classifier_t *data_profile_part_get_visible_classifier_by_id_const ( const data_profile_part_t *this_, data_row_id_t diagramelement_id )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    const data_visible_classifier_t *result = NULL;

    /* iterate over all visible classifiers */
    for ( uint32_t index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        const data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);
        assert ( data_visible_classifier_is_valid( visible_classifier ) );

        const data_diagramelement_t *diagramelement;
        diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );
        if ( data_diagramelement_get_row_id( diagramelement ) == diagramelement_id )
        {
            result = visible_classifier;
            break;
        }
    }

    return result;
}

static inline data_visible_classifier_t *data_profile_part_get_visible_classifier_by_id_ptr ( data_profile_part_t *this_, data_row_id_t diagramelement_id )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    data_visible_classifier_t *result = NULL;

    /* iterate over all visible classifiers */
    for ( uint32_t index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);
        assert ( data_visible_classifier_is_valid( visible_classifier ) );

        data_diagramelement_t *diagramelement;
        diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );
        if ( data_diagramelement_get_row_id( diagramelement ) == diagramelement_id )
        {
            result = visible_classifier;
            break;
        }
    }

    return result;
}

static inline const data_classifier_t *data_profile_part_get_classifier_by_id_const ( const data_profile_part_t *this_, data_row_id_t row_id )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    const data_classifier_t *result = NULL;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        const data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);
        assert ( data_visible_classifier_is_valid( visible_classifier ) );

        const data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_const( visible_classifier );
        if ( row_id == data_classifier_get_row_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline data_classifier_t *data_profile_part_get_classifier_by_id_ptr ( data_profile_part_t *this_, data_row_id_t row_id )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    data_classifier_t *result = NULL;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = &((*this_).visible_classifiers[index]);
        assert ( data_visible_classifier_is_valid( visible_classifier ) );

        data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_ptr( visible_classifier );
        if ( row_id == data_classifier_get_row_id( probe ) )
        {
            result = probe;
            break;
        }
    }

    return result;
}

static inline int32_t data_profile_part_get_classifier_index ( const data_profile_part_t *this_, data_row_id_t row_id )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    int32_t result = -1;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        const data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_const( &((*this_).visible_classifiers[index]) );
        if ( row_id == data_classifier_get_row_id( probe ) )
        {
            result = index;
            break;
        }
    }

    return result;
}

static inline uint32_t data_profile_part_get_classifier_index_from_pointer ( const data_profile_part_t *this_,
                                                                             const data_visible_classifier_t *classifier_ptr )
{
    assert ( NULL != classifier_ptr );  /* input parameters test */
    assert ( classifier_ptr >= &((*this_).visible_classifiers[0]) );  /* input parameters test */
    assert ( classifier_ptr < &((*this_).visible_classifiers[DATA_PROFILE_PART_MAX_STEREOTYPES]) );  /* input parameters test */
    assert ( 3 == &((*this_).visible_classifiers[3]) - (*this_).visible_classifiers );  /* compiler test */
    return ( classifier_ptr - (*this_).visible_classifiers );
}

static inline u8_error_t data_profile_part_append_classifier( data_profile_part_t *this_, const data_visible_classifier_t *new_classifier )
{
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    assert( NULL != new_classifier );
    u8_error_t result = U8_ERROR_NONE;

    const uint32_t new_index = (*this_).visible_classifier_count;
    if ( new_index < DATA_PROFILE_PART_MAX_STEREOTYPES )
    {
        data_visible_classifier_copy( &((*this_).visible_classifiers[new_index]), new_classifier );
        (*this_).visible_classifier_count ++;
    }
    else
    {
        result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
    }

    return result;
}


/*
Copyright 2023-2023 Andreas Warnke

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
