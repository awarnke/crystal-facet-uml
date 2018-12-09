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
    pango_font_description_free ( (*this_).footnote_font_description );
    (*this_).footnote_font_description = NULL;
    pango_font_description_free ( (*this_).standard_font_description );
    (*this_).standard_font_description = NULL;
    pango_font_description_free ( (*this_).title_font_description );
    (*this_).title_font_description = NULL;
}

static const char *PENCIL_SIZE_FONT_FAMILY = "Sans";

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

    (*this_).emphasized_color.red = 1.0;
    (*this_).emphasized_color.green = 1.0;
    (*this_).emphasized_color.blue = 0.3;
    (*this_).emphasized_color.alpha = 1.0;

    (*this_).gray_out_color.red = 0.7;
    (*this_).gray_out_color.green = 0.7;
    (*this_).gray_out_color.blue = 0.7;
    (*this_).gray_out_color.alpha = 1.0;

    /* guess some default values: */
    (*this_).footnote_font_size = 9.0;
    (*this_).footnote_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).footnote_font_description, PENCIL_SIZE_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).footnote_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).footnote_font_description, PANGO_WEIGHT_MEDIUM );
    pango_font_description_set_stretch ( (*this_).footnote_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).footnote_font_description, 9 * PANGO_SCALE );

    (*this_).standard_font_size = 12.0;
    (*this_).standard_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).standard_font_description, PENCIL_SIZE_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).standard_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).standard_font_description, PANGO_WEIGHT_MEDIUM );
    pango_font_description_set_stretch ( (*this_).standard_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).standard_font_description, 12 * PANGO_SCALE );

    (*this_).title_font_size = 14.0;
    (*this_).title_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).title_font_description, PENCIL_SIZE_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).title_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).title_font_description, PANGO_WEIGHT_BOLD );
    pango_font_description_set_stretch ( (*this_).title_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).title_font_description, 14 * PANGO_SCALE );

    (*this_).standard_line_width = 1.0;
    (*this_).bold_line_width = 2.0;
    (*this_).line_dash_length = 5.0;
    (*this_).standard_object_border = 2.0;
    (*this_).arrow_stroke_length = 10.0;
    (*this_).arrow_stroke_087_length = 8.66;
    (*this_).preferred_object_distance = 20.0;
}

static inline void pencil_size_reinit( pencil_size_t *this_, double width, double height )
{
    double smaller_border = (width<height) ? width : height;

    (*this_).footnote_font_size = smaller_border/80.0;
    if ( (*this_).footnote_font_size < 1.0 )
    {
        (*this_).footnote_font_size = 1.0;
    }
    pango_font_description_set_size ( (*this_).footnote_font_description, ((int)((*this_).footnote_font_size * PANGO_SCALE) ));

    (*this_).standard_font_size = smaller_border/56.0;
    if ( (*this_).standard_font_size < 1.0 )
    {
        (*this_).standard_font_size = 1.0;
    }
    pango_font_description_set_size ( (*this_).standard_font_description, ((int)((*this_).standard_font_size * PANGO_SCALE) ));

    (*this_).title_font_size = smaller_border/48.0;
    if ( (*this_).title_font_size < 1.0 )
    {
        (*this_).title_font_size = 1.0;
    }
    pango_font_description_set_size ( (*this_).title_font_description, ((int)((*this_).title_font_size * PANGO_SCALE) ));

    (*this_).standard_line_width = smaller_border/400.0;
    (*this_).bold_line_width = smaller_border/200.0;
    (*this_).line_dash_length = smaller_border/80.0;
    (*this_).standard_object_border = smaller_border/200.0;
    (*this_).arrow_stroke_length = smaller_border/67;
    (*this_).arrow_stroke_087_length = (*this_).arrow_stroke_length*0.866025403784; /* =sqrt(0.75) */
    (*this_).preferred_object_distance = smaller_border/33.0;
}

static inline double pencil_size_get_footnote_font_size( const pencil_size_t *this_ )
{
    return (*this_).footnote_font_size;
}

static inline PangoFontDescription *pencil_size_get_footnote_font_description( const pencil_size_t *this_ )
{
    return (*this_).footnote_font_description;
}

static inline double pencil_size_get_standard_font_size( const pencil_size_t *this_ )
{
    return (*this_).standard_font_size;
}

static inline PangoFontDescription *pencil_size_get_standard_font_description( const pencil_size_t *this_ )
{
    return (*this_).standard_font_description;
}

static inline double pencil_size_get_title_font_size( const pencil_size_t *this_ )
{
    return (*this_).title_font_size;
}

static inline PangoFontDescription *pencil_size_get_title_font_description( const pencil_size_t *this_ )
{
    return (*this_).title_font_description;
}

static inline double pencil_size_get_font_tab_size( const pencil_size_t *this_ )
{
    return 3.0 * (*this_).standard_font_size;
}

static inline double pencil_size_get_font_line_gap( const pencil_size_t *this_ )
{
    return (*this_).standard_line_width;  /* equals the standard line width */
}

static inline double pencil_size_get_standard_line_width( const pencil_size_t *this_ )
{
    return (*this_).standard_line_width;
}

static inline double pencil_size_get_bold_line_width( const pencil_size_t *this_ )
{
    return (*this_).bold_line_width;
}

static inline double pencil_size_get_line_dash_length( const pencil_size_t *this_ )
{
    return (*this_).line_dash_length;
}

static inline double pencil_size_get_standard_object_border( const pencil_size_t *this_ )
{
    return (*this_).standard_object_border;
}

static inline double pencil_size_get_arrow_stroke_length( const pencil_size_t *this_ )
{
    return (*this_).arrow_stroke_length;
}

static inline double pencil_size_get_arrow_stroke_087_length( const pencil_size_t *this_ )
{
    return (*this_).arrow_stroke_087_length;
}

static inline double pencil_size_get_preferred_object_distance( const pencil_size_t *this_ )
{
    return (*this_).preferred_object_distance;
}

static inline GdkRGBA pencil_size_get_standard_color( const pencil_size_t *this_ )
{
    return (*this_).standard_color;
}

static inline GdkRGBA pencil_size_get_select_color( const pencil_size_t *this_ )
{
    return (*this_).select_color;
}

static inline GdkRGBA pencil_size_get_highlight_color( const pencil_size_t *this_ )
{
    return (*this_).highlight_color;
}

static inline GdkRGBA pencil_size_get_focus_color( const pencil_size_t *this_ )
{
    return (*this_).focus_color;
}

static inline GdkRGBA pencil_size_get_emphasized_color( const pencil_size_t *this_ )
{
    return (*this_).emphasized_color;
}

static inline GdkRGBA pencil_size_get_gray_out_color( const pencil_size_t *this_ )
{
    return (*this_).gray_out_color;
}


/*
Copyright 2016-2018 Andreas Warnke

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
