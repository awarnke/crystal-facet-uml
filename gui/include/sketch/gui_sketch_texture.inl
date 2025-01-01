/* File: gui_sketch_texture.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline void gui_sketch_texture_init( gui_sketch_texture_t *this_ )
{
    (*this_).memory_fence_1 = 12345;
    (*this_).memory_fence_2 = 54321;
    (*this_).surface
        = cairo_image_surface_create_for_data( &((*this_).buf[0]),
                                               CAIRO_FORMAT_ARGB32,
                                               GUI_SKETCH_TEXTURE_MAX_WIDTH,
                                               GUI_SKETCH_TEXTURE_MAX_HEIGHT,
                                               GUI_SKETCH_TEXTURE_MAX_WIDTH * GUI_SKETCH_TEXTURE_BYTES_PER_PIXEL /* = stride */
                                             );
}

static inline void gui_sketch_texture_destroy( gui_sketch_texture_t *this_ )
{
    cairo_surface_destroy( (*this_).surface );
}

static inline u8_error_t gui_sketch_texture_draw( gui_sketch_texture_t *this_,
                                                  GdkTexture *icon,
                                                  double left,
                                                  double top,
                                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( icon != NULL );
    assert( cr != NULL );
    u8_error_t result = U8_ERROR_NONE;

    const int icon_width = gdk_texture_get_width( icon );
    const int icon_height = gdk_texture_get_height( icon );
    if (( icon_width <= GUI_SKETCH_TEXTURE_MAX_WIDTH )&&( icon_height <= GUI_SKETCH_TEXTURE_MAX_HEIGHT ))
    {
        cairo_surface_flush( (*this_).surface );
        gdk_texture_download( icon,
                              &((*this_).buf[0]),
                              GUI_SKETCH_TEXTURE_MAX_WIDTH * GUI_SKETCH_TEXTURE_BYTES_PER_PIXEL /* = stride */
                            );
        cairo_surface_mark_dirty( (*this_).surface );
        /* check that boundaries of (*this_).buf have not been overwritten: */
        assert( (*this_).memory_fence_1 == 12345 );
        assert( (*this_).memory_fence_2 == 54321 );

        cairo_set_source_surface( cr, (*this_).surface, left, top );
        cairo_rectangle( cr, left, top, icon_width, icon_height );
        cairo_fill( cr );
    }
    else
    {
        result = U8_ERROR_EDGE_CASE_PARAM;
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2024-2025 Andreas Warnke

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
