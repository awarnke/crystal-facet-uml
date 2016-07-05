/* File: pencil_size.inl; Copyright and License: see below */

#include "log.h"
#include <assert.h>

static inline void pencil_size_init( pencil_size_t *this_ )
{
    (*this_).standard_font_size = 9.0;
    (*this_).larger_font_size = 12.0;
    (*this_).standard_line_width = 1.0;
    (*this_).bold_line_width = 2.0;
    (*this_).standard_object_border = 2.0;

    (*this_).standard_color.red = 0.0;
    (*this_).standard_color.green = 0.0;
    (*this_).standard_color.blue = 0.0;
    (*this_).standard_color.alpha = 1.0;

    (*this_).selected_color.red = 1.0;
    (*this_).selected_color.green = 0.3;
    (*this_).selected_color.blue = 0.8;
    (*this_).selected_color.alpha = 1.0;

    (*this_).highlighted_color.red = 0.0;
    (*this_).highlighted_color.green = 0.8;
    (*this_).highlighted_color.blue = 0.6;
    (*this_).highlighted_color.alpha = 1.0;

    (*this_).focused_color.red = 0.9;
    (*this_).focused_color.green = 0.85;
    (*this_).focused_color.blue = 0.0;
    (*this_).focused_color.alpha = 1.0;
}

static inline void pencil_size_destroy( pencil_size_t *this_ )
{
}

static inline void pencil_size_reinit( pencil_size_t *this_ )
{
}

static inline double pencil_size_get_standard_font_size( pencil_size_t *this_ )
{
    return (*this_).standard_font_size;
}

static inline double pencil_size_get_larger_font_size( pencil_size_t *this_ )
{
    return (*this_).larger_font_size;
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

static inline GdkRGBA pencil_size_get_selected_color( pencil_size_t *this_ )
{
    return (*this_).selected_color;
}

static inline GdkRGBA pencil_size_get_highlighted_color( pencil_size_t *this_ )
{
    return (*this_).highlighted_color;
}

static inline GdkRGBA pencil_size_get_focused_color( pencil_size_t *this_ )
{
    return (*this_).focused_color;
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
