/* File: data_profile_part.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline uint32_t data_profile_part_get_stereotype_count ( const data_profile_part_t *this_ )
{
    return (*this_).stereotype_count;
}

static inline const data_classifier_t *data_profile_part_get_stereotype_const ( const data_profile_part_t *this_, uint32_t index )
{
    assert( (*this_).stereotype_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );

    const data_classifier_t *result;
    if ( index < (*this_).stereotype_count )
    {
        result = &((*this_).stereotypes[index]);
    }
    else
    {
        result = NULL;
        U8_LOG_ERROR_INT( "index out of bounds (>=(*this_).stereotype_count)", index );
    }

    return result;
}

static inline const data_classifier_t *data_profile_part_get_stereotype_by_name_const ( const data_profile_part_t *this_, utf8stringview_t stereotype_name )
{
    assert( (*this_).stereotype_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );
    const data_classifier_t *result = NULL;

    /* iterate over all visible classifiers */
    for ( uint32_t index = 0; ( index < (*this_).stereotype_count )&&( result == NULL ); index ++ )
    {
        const data_classifier_t *const stereotype = &((*this_).stereotypes[index]);
        assert ( data_classifier_is_valid( stereotype ) );

        if ( utf8stringview_equals_str( stereotype_name, data_classifier_get_name_const( stereotype ) ) )
        {
            result = stereotype;
        }
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
