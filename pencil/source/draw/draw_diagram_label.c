  /* File: draw_diagram_label.c; Copyright and License: see below */

#include "draw/draw_diagram_label.h"
#include "u8/u8_trace.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int DRAW_DIAGRAM_PANGO_AUTO_DETECT_LENGTH = -1;

void draw_diagram_label_get_type_and_name_dimensions ( const draw_diagram_label_t *this_,
                                                       const data_diagram_t *diagram,
                                                       const data_profile_part_t *profile,
                                                       const geometry_dimensions_t *proposed_bounds,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       double *out_text_width,
                                                       double *out_text_height )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagram );
    assert( NULL != profile );
    assert( NULL != proposed_bounds );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_text_width );
    assert( NULL != out_text_height );

    if ( data_diagram_is_valid( diagram ) )
    {
        /* calc name text dimensions */
        int text2_height = 0;
        int text2_width = 0;
        if ( 0 != utf8string_get_length( data_diagram_get_name_const( diagram ) ))
        {
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text( font_layout,
                                   data_diagram_get_name_const( diagram ),
                                   DRAW_DIAGRAM_PANGO_AUTO_DETECT_LENGTH
                                 );
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);
        }

        *out_text_height = text2_height;
        *out_text_width = text2_width;
    }
    else
    {
        U8_LOG_ERROR("invalid diagram in draw_diagram_label_get_type_and_name_dimensions()");
        *out_text_width = 0.0;
        *out_text_height = 0.0;
    }
    U8_TRACE_END();
}

void draw_diagram_label_draw_type_and_name ( const draw_diagram_label_t *this_,
                                             const data_diagram_t *diagram,
                                             const data_profile_part_t *profile,
                                             const geometry_rectangle_t *label_box,
                                             const pencil_size_t *pencil_size,
                                             PangoLayout *font_layout,
                                             cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagram );
    assert( NULL != profile );
    assert( NULL != label_box );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    /* define names for input data */
    const double center_x = geometry_rectangle_get_center_x( label_box );
    const double top = geometry_rectangle_get_top( label_box );

    /* draw name text */
    if ( 0 != utf8string_get_length( data_diagram_get_name_const( diagram ) ))
    {
        int text2_height;
        int text2_width;
        pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
        pango_layout_set_text( font_layout,
                               data_diagram_get_name_const( diagram ),
                               DRAW_DIAGRAM_PANGO_AUTO_DETECT_LENGTH
                             );
        pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

        /* draw text */
        cairo_move_to ( cr, center_x - 0.5*text2_width, top );
        pango_cairo_show_layout (cr, font_layout);
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2023 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
