/* File: pencil_input_data_layout.c; Copyright and License: see below */

#include "pencil_input_data_layout.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void pencil_input_data_layout_init( pencil_input_data_layout_t *this_ )
{
    TRACE_BEGIN();
    TRACE_INFO_INT( "sizeof(pencil_input_data_layout_t):", sizeof(pencil_input_data_layout_t) );

    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;

    TRACE_END();
}

void pencil_input_data_layout_reinit( pencil_input_data_layout_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert ( NULL != input_data );

    pencil_input_data_layout_destroy( this_ );

    (*this_).visible_classifier_count = pencil_input_data_get_visible_classifier_count( input_data );
    assert ( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );

    (*this_).feature_count = pencil_input_data_get_feature_count( input_data );
    assert ( (*this_).feature_count <= PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES );

    (*this_).relationship_count = pencil_input_data_get_relationship_count( input_data );
    assert ( (*this_).relationship_count <= PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );

    for ( int idx = 0; idx < (*this_).visible_classifier_count; idx ++ )
    {
        layout_visible_classifier_init_empty( &((*this_).visible_classifier_layout[idx]),
                                              pencil_input_data_get_visible_classifier_ptr( input_data, idx )
                                            );
    }

    for ( int idx = 0; idx < (*this_).feature_count; idx ++ )
    {
        layout_feature_init_empty( &((*this_).feature_layout[idx]),
                                   pencil_input_data_get_feature_ptr( input_data, idx )
                                 );
    }

    for ( int idx = 0; idx < (*this_).relationship_count; idx ++ )
    {
        layout_relationship_init_empty( &((*this_).relationship_layout[idx]),
                                        pencil_input_data_get_relationship_ptr( input_data, idx )
                                      );
    }

    TRACE_END();
}

void pencil_input_data_layout_destroy( pencil_input_data_layout_t *this_ )
{
    TRACE_BEGIN();

    for ( int idx = 0; idx < (*this_).visible_classifier_count; idx ++ )
    {
        layout_visible_classifier_destroy( &((*this_).visible_classifier_layout[idx]) );
    }

    for ( int idx = 0; idx < (*this_).feature_count; idx ++ )
    {
        layout_feature_destroy( &((*this_).feature_layout[idx]) );
    }

    for ( int idx = 0; idx < (*this_).relationship_count; idx ++ )
    {
        layout_relationship_destroy( &((*this_).relationship_layout[idx]) );
    }

    TRACE_END();
}


/*
Copyright 2017-2018 Andreas Warnke

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
