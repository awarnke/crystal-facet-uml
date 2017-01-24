/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   data_diagram_t *the_diagram,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   pencil_size_t *pencil_size,
                                   geometry_rectangle_t *diagram_bounds,
                                   PangoLayout *layout,
                                   cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_diagram );
    assert( NULL != layout );
    assert( NULL != cr );
    assert( NULL != diagram_bounds );

    TSLOG_ERROR("not yet implemented");

    TRACE_END();
}


/*
Copyright 2017-2017 Andreas Warnke

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
