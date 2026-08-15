/* File: pencil_diagram_layouter.c; Copyright and License: see below */

#include "pencil_diagram_layouter.h"
#include "u8/u8_trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_diagram_layouter_init( pencil_diagram_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_diagram_label_init( &((*this_).draw_diagram_label) );

    U8_TRACE_END();
}

void pencil_diagram_layouter_destroy( pencil_diagram_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_diagram_label_init( &((*this_).draw_diagram_label) );

    U8_TRACE_END();
}

void pencil_diagram_layouter_do_layout ( pencil_diagram_layouter_t *this_,
                                        const data_diagram_t *the_diagram,
                                        const geometry_rectangle_t *diagram_bounds,
                                        const data_profile_part_t *profile,
                                        const pencil_size_t *pencil_size,
                                        PangoLayout *font_layout,
                                        layout_diagram_t *io_layout_diagram )
{
    U8_TRACE_BEGIN();
    assert( NULL != the_diagram );
    assert( NULL != diagram_bounds );
    assert( NULL != profile );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != io_layout_diagram );

    /* determine diagram bounds */
    const double left = geometry_rectangle_get_left ( diagram_bounds );
    const double top = geometry_rectangle_get_top ( diagram_bounds );
    const double width = geometry_rectangle_get_width ( diagram_bounds );
    const double height = geometry_rectangle_get_height ( diagram_bounds );
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* calculate label_box */
    const double text_left = left + 3.0 * gap;
    const double text_top = top + 2.0 * gap;
    const geometry_dimensions_t label_dim_proposal
        = { .width = width, .height = pencil_size_get_standard_font_size( pencil_size ) };
    geometry_dimensions_t label_dim;
    draw_diagram_label_get_type_and_name_dimensions( &((*this_).draw_diagram_label),
                                                     the_diagram,
                                                     profile,
                                                     &label_dim_proposal,
                                                     pencil_size,
                                                     font_layout,
                                                     &label_dim
                                                   );
    const double text_width = geometry_dimensions_get_width( &label_dim );
    const double text_height = geometry_dimensions_get_height( &label_dim );
    geometry_rectangle_t label_box;
    geometry_rectangle_init( &label_box, text_left, text_top, text_width, text_height );

    /* calculate space */
    const double space_left = left + 2.0 * gap;
    const double space_top = top + 4.0 * gap + text_height;
    const double space_width = width - 4.0 * gap;
    const double space_height = height - 6.0 * gap - text_height;
    geometry_rectangle_t space;
    if ( ( space_width <= 0.0 ) || ( space_height <= 0.0 ) )
    {
        geometry_rectangle_init_empty( &space );
    }
    else
    {
        geometry_rectangle_init( &space, space_left, space_top, space_width, space_height );
    }

    /* set new metrics */
    layout_diagram_set_bounds( io_layout_diagram, diagram_bounds );
    layout_diagram_set_draw_area( io_layout_diagram, &space );
    layout_diagram_set_label_box( io_layout_diagram, &label_box );

    geometry_rectangle_destroy( &space );
    geometry_rectangle_destroy( &label_box );
    U8_TRACE_END();
}


/*
Copyright 2017-2026 Andreas Warnke

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
