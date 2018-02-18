/* File: pencil_input_data_layout.c; Copyright and License: see below */

#include "pencil_input_data_layout.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void pencil_input_data_layout_init( pencil_input_data_layout_t *this_ )
{
    TRACE_BEGIN();
    TRACE_INFO_INT( "sizeof(pencil_input_data_layout_t):", sizeof(pencil_input_data_layout_t) );

    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS; idx ++ )
    {
        geometry_rectangle_init_empty( &((*this_).classifier_bounds[idx]) );
        geometry_rectangle_init_empty( &((*this_).classifier_space[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES; idx ++ )
    {
        (*this_).feature_visible[idx] = PENCIL_VISIBILITY_HIDE;
        geometry_rectangle_init_empty( &((*this_).feature_bounds[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS; idx ++ )
    {
        (*this_).relationship_visible[idx] = PENCIL_VISIBILITY_HIDE;
        geometry_connector_init_empty( &((*this_).relationship_shape[idx]) );
    }

    TRACE_END();
}

void pencil_input_data_layout_destroy( pencil_input_data_layout_t *this_ )
{
    TRACE_BEGIN();

    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS; idx ++ )
    {
        geometry_rectangle_destroy( &((*this_).classifier_bounds[idx]) );
        geometry_rectangle_destroy( &((*this_).classifier_space[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES; idx ++ )
    {
        geometry_rectangle_destroy( &((*this_).feature_bounds[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS; idx ++ )
    {
        geometry_connector_destroy( &((*this_).relationship_shape[idx]) );
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
