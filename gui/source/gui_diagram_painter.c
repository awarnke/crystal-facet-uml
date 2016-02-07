/* File: gui_diagram_painter.c; Copyright and License: see below */

#include "gui_diagram_painter.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void gui_diagram_painter_init( gui_diagram_painter_t *this_ )
{
    TRACE_BEGIN();
    
    TRACE_END();
}

void gui_diagram_painter_destroy( gui_diagram_painter_t *this_ )
{
    TRACE_BEGIN();
    
    TRACE_END();
}

void gui_diagram_painter_draw ( gui_diagram_painter_t *this_, data_database_t *db, int32_t diagram_id, cairo_t *cr )
{
    TRACE_BEGIN();
    TRACE_INFO_INT("drawing diagram id",diagram_id);

    double left, top, right, bottom;
    
    cairo_clip_extents ( cr, &left, &top, &right, &bottom );

    TRACE_INFO_INT( "w", (int)(right-left) );
    TRACE_INFO_INT( "h", (int)(bottom-top) );

    TRACE_END();
}


/*
Copyright 2016-2016 Andreas Warnke

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
