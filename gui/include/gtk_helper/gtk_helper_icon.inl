/* File: gtk_helper_icon.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline u8_error_t gtk_helper_icon_draw_texture( GdkTexture *icon, double left, double top, cairo_t *cr )
{
    assert( icon != NULL );
    assert( cr != NULL );
    u8_error_t result = U8_ERROR_NONE;
#if ( GTK_MAJOR_VERSION >= 4 )

    double icon_width = gdk_texture_get_width ( icon );
    double icon_height = gdk_texture_get_height ( icon );
    cairo_surface_t *surface = cairo_get_target( cr );
    assert( surface != NULL );
    const int width = cairo_image_surface_get_width( surface );
    const int height = cairo_image_surface_get_height( surface );
    const bool full_visible
        = (( left >= 0.0 )&&( top >= 0.0 )&&( ( left + icon_width ) < width )&&( ( top + icon_height ) < height ));
    if ( full_visible )
    {
        guchar *const data = cairo_image_surface_get_data( surface );
        const gsize stride = cairo_image_surface_get_stride( surface );
        gdk_texture_download( icon, data, stride );
        cairo_surface_mark_dirty( surface );
    }
    else
    {
        result = U8_ERROR_EDGE_CASE_PARAM;
    }

    /* TODO this is a workaround*/
    GdkPixbuf* test = gdk_pixbuf_get_from_texture( icon );
    gdk_cairo_set_source_pixbuf( cr, test, left, top );
    cairo_rectangle( cr, left, top, icon_width, icon_height );
    cairo_fill( cr );
    g_object_unref( test );

#else
    double icon_width = gdk_pixbuf_get_width ( icon );
    double icon_height = gdk_pixbuf_get_height ( icon );
    gdk_cairo_set_source_pixbuf( cr, bg_img, left, top );
    cairo_rectangle ( cr, left, top, icon_width, icon_height );
    cairo_fill (cr);
#endif
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
