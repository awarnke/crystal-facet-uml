/* File: pencil_diagram_painter.inl; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "trace.h"
#include <assert.h>

static inline void pencil_diagram_painter_init( pencil_diagram_painter_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    pencil_classifier_painter_init( &((*this_).classifier_painter) );
    pencil_marker_init( &((*this_).marker) );

    pencil_layouter_init( &((*this_).layouter), input_data );

    (*this_).input_data = input_data;

    TRACE_END();
}

static inline void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );
    pencil_marker_destroy( &((*this_).marker) );

    pencil_layouter_destroy( &((*this_).layouter) );

    (*this_).input_data = NULL;

    TRACE_END();
}

static inline void pencil_diagram_painter_layout_grid ( pencil_diagram_painter_t *this_,
                                                        pencil_input_data_t *input_data,
                                                        geometry_rectangle_t diagram_bounds )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    pencil_layouter_layout_grid ( &((*this_).layouter), input_data, diagram_bounds );

    TRACE_END();
}

static inline void pencil_diagram_layout_elements ( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_layouter_layout_elements ( &((*this_).layouter) );

    TRACE_END();
}

static inline data_id_t pencil_diagram_painter_get_object_id_at_pos ( pencil_diagram_painter_t *this_,
                                                        double x,
                                                        double y,
                                                        bool dereference )
{
    TRACE_BEGIN();

    data_id_t result;

    result = pencil_layouter_get_object_id_at_pos ( &((*this_).layouter), x, y, dereference );

    TRACE_END();
    return result;
}

static inline universal_int32_pair_t pencil_diagram_painter_get_order_at_pos ( pencil_diagram_painter_t *this_,
                                                                 double x,
                                                                 double y )
{
    TRACE_BEGIN();

    universal_int32_pair_t result;

    result = pencil_layouter_get_order_at_pos ( &((*this_).layouter), x, y );

    TRACE_END();
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
