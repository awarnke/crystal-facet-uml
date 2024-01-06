/* File: data_profile_part.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline u8_error_t data_profile_part_private_load_stereotype ( data_profile_part_t *this_,
                                                                     const char *stereotype_name,
                                                                     data_database_reader_t *db_reader )
{
    assert( NULL != stereotype_name );
    assert( NULL != db_reader );
    u8_error_t result = U8_ERROR_NONE;

    const utf8stringview_t stereotype_name_view = UTF8STRINGVIEW_STR( stereotype_name );

    const bool already_loaded
        = ( NULL != data_profile_part_get_stereotype_by_name_const( this_, stereotype_name_view ) );
    if ( ! already_loaded )  /* filter duplicates */
    {
        if ( (*this_).stereotype_count < DATA_PROFILE_PART_MAX_STEREOTYPES )
        {
            const u8_error_t db_err
                = data_database_reader_get_classifier_by_name( db_reader,
                                                               stereotype_name,  /* : name */
                                                               &((*this_).stereotypes[(*this_).stereotype_count])
                                                             );
            const data_classifier_type_t c_type
                = data_classifier_get_main_type( &((*this_).stereotypes[(*this_).stereotype_count]) );

            if ( u8_error_contains( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
            {
                U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading stereotypes of a diagram" );
            }
            if ( u8_error_contains( db_err, U8_ERROR_NOT_FOUND ) )
            {
                /* no entry found. */
                U8_LOG_EVENT( "A stereotype does not exist." );
                U8_TRACE_INFO_STR( "stereotype does not exist:", stereotype_name );
            }
            else if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
            {
                /* error at loading */
                U8_LOG_ERROR( "A stereotype could not be loaded!" );
                U8_TRACE_INFO_STR( "stereotype could not be loaded:", stereotype_name );
                result |= db_err;  /* collect error flags */
            }
            else if ( DATA_CLASSIFIER_TYPE_STEREOTYPE != c_type )
            {
                /* wrong-typed entry found. */
                U8_LOG_EVENT( "A stereotype was found but is not of type stereotype." );
                U8_TRACE_INFO_STR( "loaded stereotype is not of type stereotype:", stereotype_name );
            }
            else
            {
                /* success */
                (*this_).stereotype_count ++;
            }
        }
        else
        {
            /* there is another stereotype to be loaded but no more space left */
            U8_LOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading stereotypes of a diagram" );
            result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
        }
    }

    return result;
}

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
    for ( uint32_t index = 0; ( index < (*this_).stereotype_count ) && ( result == NULL ); index ++ )
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

static inline void data_profile_part_trace ( const data_profile_part_t *this_ )
{
    U8_TRACE_INFO_INT( "data_profile_part_t: [length]", (*this_).stereotype_count );
    for ( uint32_t index = 0; index < (*this_).stereotype_count; index ++ )
    {
        const data_classifier_t *const stereotype = &((*this_).stereotypes[index]);
        assert ( data_classifier_is_valid( stereotype ) );
        U8_TRACE_INFO_STR( "-", data_classifier_get_name_const( stereotype ) );
    }
}


/*
Copyright 2023-2024 Andreas Warnke

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
