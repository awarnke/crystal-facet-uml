/* File: pencil_input_data.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ )
{
    return &((*this_).diagram);
}

static inline bool pencil_input_data_is_valid ( pencil_input_data_t *this_ )
{
    return data_diagram_is_valid( &((*this_).diagram) );
}

static inline void pencil_input_data_invalidate ( pencil_input_data_t *this_ )
{
    data_diagram_init_empty( &((*this_).diagram) );
}

static inline void pencil_input_data_private_destroy_visible_classifiers( pencil_input_data_t *this_ )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_visible_classifier_destroy ( &((*this_).visible_classifiers[index]) );
    }

    (*this_).visible_classifier_count = 0;
}

static inline uint32_t pencil_input_data_get_visible_classifier_count ( pencil_input_data_t *this_ )
{
    return (*this_).visible_classifier_count;
}

static inline data_visible_classifier_t *pencil_input_data_get_visible_classifier_ptr ( pencil_input_data_t *this_, uint32_t index )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

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

static inline data_classifier_t *pencil_input_data_get_classifier_ptr ( pencil_input_data_t *this_, int32_t row_id )
{
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );
    data_classifier_t *result = NULL;

    for ( int index = 0; index < (*this_).visible_classifier_count; index ++ )
    {
        data_classifier_t *probe;
        probe = data_visible_classifier_get_classifier_ptr( &((*this_).visible_classifiers[index]) );
        if ( row_id == data_classifier_get_id( probe ) )
        {
            result = probe;
        }
    }

    return result;
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
