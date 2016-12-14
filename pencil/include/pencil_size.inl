/* File: pencil_size.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void pencil_size_init( pencil_size_t *this_, double width, double height )
{
    pencil_size_init_empty( this_ );
    pencil_size_reinit( this_, width, height);
}

static inline void pencil_size_destroy( pencil_size_t *this_ )
{
    pango_font_description_free ( (*this_).standard_font_desc );
    (*this_).standard_font_desc = NULL;
    pango_font_description_free ( (*this_).larger_font_desc );
    (*this_).larger_font_desc = NULL;
}

static inline void pencil_size_init_empty( pencil_size_t *this_ )
{
    (*this_).standard_color.red = 0.0;
    (*this_).standard_color.green = 0.0;
    (*this_).standard_color.blue = 0.0;
    (*this_).standard_color.alpha = 1.0;

    (*this_).select_color.red = 1.0;
    (*this_).select_color.green = 0.3;
    (*this_).select_color.blue = 0.8;
    (*this_).select_color.alpha = 1.0;

    (*this_).highlight_color.red = 0.0;
    (*this_).highlight_color.green = 0.8;
    (*this_).highlight_color.blue = 0.6;
    (*this_).highlight_color.alpha = 1.0;

    (*this_).focus_color.red = 0.9;
    (*this_).focus_color.green = 0.85;
    (*this_).focus_color.blue = 0.0;
    (*this_).focus_color.alpha = 1.0;

    /* guess some default values: */
    (*this_).standard_font_size = 9.0;
    (*this_).standard_font_desc = pango_font_description_from_string ("Sans 9px");;
    (*this_).larger_font_size = 12.0;
    (*this_).larger_font_desc = pango_font_description_from_string ("Sans 12px");;
    (*this_).standard_line_width = 1.0;
    (*this_).bold_line_width = 2.0;
    (*this_).standard_object_border = 2.0;

}

static inline void pencil_size_reinit( pencil_size_t *this_, double width, double height )
{
    double smaller_border = (width<height) ? width : height;
    (*this_).standard_font_size = smaller_border/40.0;
    (*this_).larger_font_size = smaller_border/32.0;
    (*this_).standard_line_width = smaller_border/400.0;
    (*this_).bold_line_width = smaller_border/200.0;
    (*this_).standard_object_border = smaller_border/200.0;
}

static inline double pencil_size_get_standard_font_size( pencil_size_t *this_ )
{
    return (*this_).standard_font_size;
}

static inline double pencil_size_get_standard_font_ascent( pencil_size_t *this_ )
{
    return (PENCIL_SIZE_CONST_ASCENT_PERCENT / 100.0) * (*this_).standard_font_size;
}

static inline double pencil_size_get_larger_font_size( pencil_size_t *this_ )
{
    return (*this_).larger_font_size;
}

static inline double pencil_size_get_larger_font_ascent( pencil_size_t *this_ )
{
    return (PENCIL_SIZE_CONST_ASCENT_PERCENT / 100.0) * (*this_).larger_font_size;
}

static inline double pencil_size_get_font_line_gap( pencil_size_t *this_ )
{
    return (*this_).standard_line_width;  /* equals the standard line width */
}

static inline double pencil_size_get_standard_line_width( pencil_size_t *this_ )
{
    return (*this_).standard_line_width;
}

static inline double pencil_size_get_bold_line_width( pencil_size_t *this_ )
{
    return (*this_).bold_line_width;
}

static inline double pencil_size_get_standard_object_border( pencil_size_t *this_ )
{
    return (*this_).standard_object_border;
}

static inline GdkRGBA pencil_size_get_standard_color( pencil_size_t *this_ )
{
    return (*this_).standard_color;
}

static inline GdkRGBA pencil_size_get_select_color( pencil_size_t *this_ )
{
    return (*this_).select_color;
}

static inline GdkRGBA pencil_size_get_highlight_color( pencil_size_t *this_ )
{
    return (*this_).highlight_color;
}

static inline GdkRGBA pencil_size_get_focus_color( pencil_size_t *this_ )
{
    return (*this_).focus_color;
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
