/* File: gui_sketch_texture.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void gui_sketch_texture_init( gui_sketch_texture_t *this_ )
{
}

static inline void gui_sketch_texture_destroy( gui_sketch_texture_t *this_ )
{
}

static inline u8_error_t gui_sketch_texture_draw( gui_sketch_texture_t *this_,
                                                  GdkTexture *icon,
                                                  double left,
                                                  double top,
                                                  cairo_t *cr )
{
    assert( icon != NULL );
    assert( cr != NULL );
    u8_error_t result = U8_ERROR_NONE;

    const int icon_width = gdk_texture_get_width( icon );
    const int icon_height = gdk_texture_get_height( icon );

    /* TODO this is a workaround, there should be a way with less copying... */
    GdkPixbuf* test = gdk_pixbuf_get_from_texture( icon );
    gdk_cairo_set_source_pixbuf( cr, test, left, top );
    cairo_rectangle( cr, left, top, icon_width, icon_height );
    cairo_fill( cr );
    g_object_unref( test );

    return result;
}


/*
Copyright 2024-2024 Andreas Warnke

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
