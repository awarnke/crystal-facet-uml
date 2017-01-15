/* File: pencil_input_data.c; Copyright and License: see below */

#include "pencil_input_data.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_input_data_init( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_init_empty( &((*this_).diagram) );
    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;

    TRACE_END();
}

void pencil_input_data_destroy( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_destroy( &((*this_).diagram) );
    pencil_input_data_private_destroy_visible_classifiers( this_ );
    pencil_input_data_private_destroy_features( this_ );
    pencil_input_data_private_destroy_relationships( this_ );

    TRACE_END();
}

void pencil_input_data_load( pencil_input_data_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    if ( DATA_ID_VOID_ID == diagram_id )
    {
        /* re-init */
        data_diagram_reinit_empty( &((*this_).diagram) );

        pencil_input_data_private_destroy_visible_classifiers( this_ );
        pencil_input_data_private_destroy_features( this_ );
        pencil_input_data_private_destroy_relationships( this_ );
    }
    else
    {
        data_error_t db_err;

        data_diagram_destroy( &((*this_).diagram) );
        pencil_input_data_private_destroy_visible_classifiers( this_ );
        pencil_input_data_private_destroy_features( this_ );
        pencil_input_data_private_destroy_relationships( this_ );

        /* load diagram */
        db_err = data_database_reader_get_diagram_by_id ( db_reader, diagram_id, &((*this_).diagram) );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            data_diagram_reinit_empty( &((*this_).diagram) );
        }

        /* load classifiers */
        db_err = data_database_reader_get_classifiers_by_diagram_id ( db_reader,
                                                                      diagram_id,
                                                                      PENCIL_INPUT_DATA_MAX_CLASSIFIERS,
                                                                      &((*this_).visible_classifiers),
                                                                      &((*this_).visible_classifier_count)
                                                                    );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).visible_classifier_count = 0;
        }

        /* load features */
        db_err = data_database_reader_get_features_by_diagram_id ( db_reader,
                                                                   diagram_id,
                                                                   PENCIL_INPUT_DATA_MAX_FEATURES,
                                                                   &((*this_).features),
                                                                   &((*this_).feature_count)
                                                                 );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).feature_count = 0;
        }

        /* load relationships */
        db_err = data_database_reader_get_relationships_by_diagram_id ( db_reader,
                                                                        diagram_id,
                                                                        PENCIL_INPUT_DATA_MAX_RELATIONSHIPS,
                                                                        &((*this_).relationships),
                                                                        &((*this_).relationship_count)
                                                                      );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).relationship_count = 0;
        }
    }

    TRACE_END();
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
