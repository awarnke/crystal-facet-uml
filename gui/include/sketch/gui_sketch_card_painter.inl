/* File: gui_sketch_card_painter.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include "u8/u8_f64.h"
#include <assert.h>

static inline void gui_sketch_card_painter_private_draw_rect ( const gui_sketch_card_painter_t *this_,
                                                               const geometry_rectangle_t *rect,
                                                               cairo_t *cr )
{
    assert( rect != NULL );
    assert( cr != NULL );

    static const double WHITE_R = 1.0;
    static const double WHITE_G = 1.0;
    static const double WHITE_B = 1.0;
    static const double WHITE_A = 1.0;
    cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );
    cairo_rectangle( cr,
                     geometry_rectangle_get_left( rect ),
                     geometry_rectangle_get_top( rect ),
                     geometry_rectangle_get_width( rect ),
                     geometry_rectangle_get_height( rect )
                   );
    cairo_fill (cr);
}

static inline void gui_sketch_card_painter_private_draw_border ( const gui_sketch_card_painter_t *this_,
                                                                 const geometry_rectangle_t *border,
                                                                 const geometry_rectangle_t *space,
                                                                 cairo_t *cr )
{
    assert( border != NULL );
    assert( space != NULL );
    assert( cr != NULL );

    /* prepare draw */
    static const double WHITE_R = 1.0;
    static const double WHITE_G = 1.0;
    static const double WHITE_B = 1.0;
    static const double WHITE_A = 1.0;
    cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );

    /* plan to draw */
    {
        const double bold_left = geometry_rectangle_get_left( border );
        const double bold_top = geometry_rectangle_get_top( border );
        const double bold_width = geometry_rectangle_get_width( border );
        const double bold_height = geometry_rectangle_get_height( border );
        const double bold_right = bold_left + bold_width;
        const double bold_bottom = bold_top + bold_height;
        const double void_left = geometry_rectangle_get_left( space );
        const double void_top = geometry_rectangle_get_top( space );
        const double void_width = geometry_rectangle_get_width( space );
        const double void_height = geometry_rectangle_get_height( space );
        const double void_right = void_left + void_width;
        const double void_bottom = void_top + void_height;
        /* fit the empty space into the hightlight space, round to make parts fit together and to not have negative widths/heights */
        const double void_left_adjusted = floor( u8_f64_min2( u8_f64_max2( void_left, bold_left ), bold_right ) );
        const double void_top_adjusted = floor( u8_f64_min2( u8_f64_max2( void_top, bold_top ), bold_bottom ) );
        const double void_right_adjusted = ceil( u8_f64_max2( u8_f64_min2( void_right, bold_right ), bold_left ) );
        const double void_bottom_adjusted = ceil( u8_f64_max2( u8_f64_min2( void_bottom, bold_bottom ), bold_top ) );
        /* draw top region */
        if ( bold_top < void_top )
        {
            cairo_rectangle( cr, bold_left, bold_top, bold_width, void_top_adjusted - bold_top );
        }
        /* draw left region */
        if ( bold_left < void_left )
        {
            cairo_rectangle( cr, bold_left, void_top_adjusted, void_left_adjusted - bold_left, void_bottom_adjusted - void_top_adjusted );
        }
        /* draw right region */
        if ( bold_right > void_right )
        {
            cairo_rectangle( cr, void_right_adjusted, void_top_adjusted, bold_right - void_right_adjusted, void_bottom_adjusted - void_top_adjusted );
        }
        /* draw bottom region */
        if ( bold_bottom > void_bottom )
        {
            cairo_rectangle( cr, bold_left, void_bottom_adjusted, bold_width, bold_bottom - void_bottom_adjusted );
        }
    }

    /* perform draw */
    cairo_fill (cr);
}


/*
Copyright 2016-2026 Andreas Warnke

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
