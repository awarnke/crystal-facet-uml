/* File: data_node_set.c; Copyright and License: see below */

#include "set/data_node_set.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void data_node_set_init( data_node_set_t *this_ )
{
    TRACE_BEGIN();
    TRACE_INFO_INT( "sizeof(data_node_set_t):", sizeof(data_node_set_t) );

    data_classifier_init_empty( &((*this_).classifier) );
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;

    TRACE_END();
}

void data_node_set_destroy( data_node_set_t *this_ )
{
    TRACE_BEGIN();

    data_classifier_destroy( &((*this_).classifier) );
    data_node_set_private_destroy_features( this_ );
    data_node_set_private_destroy_relationships( this_ );

    TRACE_END();
}

u8_error_t data_node_set_load( data_node_set_t *this_, data_row_id_t classifier_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    u8_error_t result = U8_ERROR_NONE;

    if ( DATA_ROW_ID_VOID == classifier_id )
    {
        /* re-init */
        data_classifier_reinit_empty( &((*this_).classifier) );

        data_node_set_private_destroy_features( this_ );
        data_node_set_private_destroy_relationships( this_ );
    }
    else
    {
        u8_error_t db_err;

        data_classifier_destroy( &((*this_).classifier) );
        data_node_set_private_destroy_features( this_ );
        data_node_set_private_destroy_relationships( this_ );

        /* load classifier */
        db_err = data_database_reader_get_classifier_by_id ( db_reader, classifier_id, &((*this_).classifier) );

        if ( U8_ERROR_NONE != (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
        {
            TSLOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & U8_ERROR_ARRAY_BUFFER_EXCEEDED) )
        {
            TSLOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED|U8_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            data_classifier_reinit_empty( &((*this_).classifier) );
        }
        result |= db_err;  /* collect error flags */

        /* load features */
        db_err = data_database_reader_get_features_by_classifier_id ( db_reader,
                                                                      classifier_id,
                                                                      DATA_NODE_SET_MAX_FEATURES,
                                                                      &((*this_).features),
                                                                      &((*this_).feature_count)
                                                                    );

        if ( U8_ERROR_NONE != (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
        {
            TSLOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & U8_ERROR_ARRAY_BUFFER_EXCEEDED) )
        {
            TSLOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED|U8_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).feature_count = 0;
        }
        result |= db_err;  /* collect error flags */

        /* load relationships */
        db_err = data_database_reader_get_relationships_by_classifier_id ( db_reader,
                                                                           classifier_id,
                                                                           DATA_NODE_SET_MAX_RELATIONSHIPS,
                                                                           &((*this_).relationships),
                                                                           &((*this_).relationship_count)
                                                                         );

        if ( U8_ERROR_NONE != (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
        {
            TSLOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & U8_ERROR_ARRAY_BUFFER_EXCEEDED) )
        {
            TSLOG_ERROR( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED|U8_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).relationship_count = 0;
        }
        result |= db_err;  /* collect error flags */
    }

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2020-2021 Andreas Warnke

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
