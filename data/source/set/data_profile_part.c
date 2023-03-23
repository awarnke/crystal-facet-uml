/* File: data_profile_part.c; Copyright and License: see below */

#include "set/data_profile_part.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void data_profile_part_init( data_profile_part_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_INFO_INT( "sizeof(data_profile_part_t):", sizeof(data_profile_part_t) );

    (*this_).stereotype_count = 0;

    U8_TRACE_END();
}

void data_profile_part_reinit( data_profile_part_t *this_ )
{
    U8_TRACE_BEGIN();

    data_profile_part_destroy( this_ );
    data_profile_part_init( this_ );

    U8_TRACE_END();
}

void data_profile_part_destroy( data_profile_part_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).stereotype_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );

    for ( int index = 0; index < (*this_).stereotype_count; index ++ )
    {
        data_classifier_destroy ( &((*this_).stereotypes[index]) );
    }
    (*this_).stereotype_count = 0;

    U8_TRACE_END();
}

u8_error_t data_profile_part_load( data_profile_part_t *this_,
                                   const data_visible_set_t *diagram_elements,
                                   data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != diagram_elements );
    u8_error_t result = U8_ERROR_NONE;

    /* re-init */
    data_profile_part_reinit( this_ );

    /* load stereotypes of classifiers */
    const uint32_t clsfy_count = data_visible_set_get_visible_classifier_count( diagram_elements );
    for ( uint32_t clsfy_index = 0; ( clsfy_index < clsfy_count )&&( result == U8_ERROR_NONE ); clsfy_index ++ )
    {
        const data_visible_classifier_t *const vis_clsfy = data_visible_set_get_visible_classifier_const( diagram_elements, clsfy_index );
        const data_classifier_t *const clsfy = data_visible_classifier_get_classifier_const( vis_clsfy );
        if ( data_classifier_has_stereotype( clsfy ) )
        {
            const char *const clsfy_stereotype = data_classifier_get_stereotype_const( clsfy );
            const utf8stringview_t clsfy_stereotype_view = UTF8STRINGVIEW_STR( clsfy_stereotype );
            const bool already_loaded
                = ( NULL != data_profile_part_get_stereotype_by_name_const( this_, clsfy_stereotype_view ) );
            if ( ! already_loaded )  /* filter duplicates */
            {
                if ( (*this_).stereotype_count < DATA_PROFILE_PART_MAX_STEREOTYPES )
                {
                    const u8_error_t db_err
                        = data_database_reader_get_classifier_by_name( db_reader,
                                                                       clsfy_stereotype,  /* : name */
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
                        U8_TRACE_INFO_STR( "stereotype does not exist:", clsfy_stereotype );
                    }
                    else if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
                    {
                        /* error at loading */
                        U8_LOG_ERROR( "A stereotype could not be loaded!" );
                        U8_TRACE_INFO_STR( "stereotype could not be loaded:", clsfy_stereotype );
                        result |= db_err;  /* collect error flags */
                    }
                    else if ( DATA_CLASSIFIER_TYPE_STEREOTYPE != c_type )
                    {
                        /* wrong-typed entry found. */
                        U8_LOG_EVENT( "A stereotype was found but is not of type stereotype." );
                        U8_TRACE_INFO_STR( "stereotype is not of type stereotype:", clsfy_stereotype );
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
        }
    }

    U8_TRACE_END_ERR(result);
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