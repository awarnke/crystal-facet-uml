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

    (*this_).visible_classifier_count = 0;

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
    assert( (*this_).visible_classifier_count <= DATA_PROFILE_PART_MAX_STEREOTYPES );

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_destroy ( &((*this_).visible_classifiers[index]) );
    }
    (*this_).visible_classifier_count = 0;

    U8_TRACE_END();
}

u8_error_t data_profile_part_load( data_profile_part_t *this_, data_row_id_t diagram_id, data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    u8_error_t result = U8_ERROR_NONE;

    if ( DATA_ROW_ID_VOID == diagram_id )
    {
        /* re-init */
        data_profile_part_reinit( this_ );
    }
    else
    {
        u8_error_t db_err;

        data_profile_part_reinit( this_ );

        /* load classifiers */
        db_err = data_database_reader_get_classifiers_by_diagram_id ( db_reader,
                                                                      diagram_id,
                                                                      DATA_PROFILE_PART_MAX_STEREOTYPES,
                                                                      &((*this_).visible_classifiers),
                                                                      &((*this_).visible_classifier_count)
                                                                    );

        if ( U8_ERROR_STRING_BUFFER_EXCEEDED == (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
        {
            U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( U8_ERROR_ARRAY_BUFFER_EXCEEDED == (db_err & U8_ERROR_ARRAY_BUFFER_EXCEEDED) )
        {
            U8_LOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED|U8_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).visible_classifier_count = 0;
        }
        result |= db_err;  /* collect error flags */
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
