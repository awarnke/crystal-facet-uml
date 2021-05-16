/* File: gui_sketch_style.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static const char *const GUI_SKETCH_STYLE_FONT_FAMILY = "Sans";

static inline void gui_sketch_style_init( gui_sketch_style_t *this_ )
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
    (*this_).highlight_color.green = 0.6;
    (*this_).highlight_color.blue = 0.4;
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
    pango_font_description_set_family_static ( (*this_).footnote_font_description, GUI_SKETCH_STYLE_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).footnote_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).footnote_font_description, PANGO_WEIGHT_MEDIUM );
    pango_font_description_set_stretch ( (*this_).footnote_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).footnote_font_description, 9 * PANGO_SCALE );

    (*this_).standard_font_size = 12.0;
    (*this_).standard_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).standard_font_description, GUI_SKETCH_STYLE_FONT_FAMILY );
    pango_font_description_set_style ( (*this_).standard_font_description, PANGO_STYLE_NORMAL );
    pango_font_description_set_weight ( (*this_).standard_font_description, PANGO_WEIGHT_MEDIUM );
    pango_font_description_set_stretch ( (*this_).standard_font_description, PANGO_STRETCH_NORMAL );
    pango_font_description_set_size ( (*this_).standard_font_description, 12 * PANGO_SCALE );

    (*this_).title_font_size = 14.0;
    (*this_).title_font_description = pango_font_description_new ();
    pango_font_description_set_family_static ( (*this_).title_font_description, GUI_SKETCH_STYLE_FONT_FAMILY );
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
    (*this_).classifier_symbol_height = 24.0;
}

static inline void gui_sketch_style_destroy( gui_sketch_style_t *this_ )
{
    pango_font_description_free ( (*this_).footnote_font_description );
    (*this_).footnote_font_description = NULL;
    pango_font_description_free ( (*this_).standard_font_description );
    (*this_).standard_font_description = NULL;
    pango_font_description_free ( (*this_).title_font_description );
    (*this_).title_font_description = NULL;
}

static inline double gui_sketch_style_get_footnote_font_size( const gui_sketch_style_t *this_ )
{
    return (*this_).footnote_font_size;
}

static inline PangoFontDescription *gui_sketch_style_get_footnote_font_description( const gui_sketch_style_t *this_ )
{
    return (*this_).footnote_font_description;
}

static inline double gui_sketch_style_get_standard_font_size( const gui_sketch_style_t *this_ )
{
    return (*this_).standard_font_size;
}

static inline PangoFontDescription *gui_sketch_style_get_standard_font_description( const gui_sketch_style_t *this_ )
{
    return (*this_).standard_font_description;
}

static inline double gui_sketch_style_get_title_font_size( const gui_sketch_style_t *this_ )
{
    return (*this_).title_font_size;
}

static inline PangoFontDescription *gui_sketch_style_get_title_font_description( const gui_sketch_style_t *this_ )
{
    return (*this_).title_font_description;
}

static inline double gui_sketch_style_get_font_tab_size( const gui_sketch_style_t *this_ )
{
    return 3.0 * (*this_).standard_font_size;
}

static inline double gui_sketch_style_get_font_line_gap( const gui_sketch_style_t *this_ )
{
    return (*this_).standard_line_width;  /* equals the standard line width */
}

static inline double gui_sketch_style_get_standard_line_width( const gui_sketch_style_t *this_ )
{
    return (*this_).standard_line_width;
}

static inline double gui_sketch_style_get_bold_line_width( const gui_sketch_style_t *this_ )
{
    return (*this_).bold_line_width;
}

static inline double gui_sketch_style_get_line_dash_length( const gui_sketch_style_t *this_ )
{
    return (*this_).line_dash_length;
}

static inline double gui_sketch_style_get_standard_object_border( const gui_sketch_style_t *this_ )
{
    return (*this_).standard_object_border;
}

static inline double gui_sketch_style_get_arrow_stroke_length( const gui_sketch_style_t *this_ )
{
    return (*this_).arrow_stroke_length;
}

static inline double gui_sketch_style_get_arrow_stroke_087_length( const gui_sketch_style_t *this_ )
{
    return (*this_).arrow_stroke_087_length;
}

static inline double gui_sketch_style_get_preferred_object_distance( const gui_sketch_style_t *this_ )
{
    return (*this_).preferred_object_distance;
}

static inline double gui_sketch_style_get_classifier_symbol_height( const gui_sketch_style_t *this_ )
{
    return (*this_).classifier_symbol_height;
}

static inline GdkRGBA gui_sketch_style_get_standard_color( const gui_sketch_style_t *this_ )
{
    return (*this_).standard_color;
}

static inline GdkRGBA gui_sketch_style_get_select_color( const gui_sketch_style_t *this_ )
{
    return (*this_).select_color;
}

static inline GdkRGBA gui_sketch_style_get_highlight_color( const gui_sketch_style_t *this_ )
{
    return (*this_).highlight_color;
}

static inline GdkRGBA gui_sketch_style_get_focus_color( const gui_sketch_style_t *this_ )
{
    return (*this_).focus_color;
}

static inline GdkRGBA gui_sketch_style_get_emphasized_color( const gui_sketch_style_t *this_ )
{
    return (*this_).emphasized_color;
}

static inline GdkRGBA gui_sketch_style_get_gray_out_color( const gui_sketch_style_t *this_ )
{
    return (*this_).gray_out_color;
}


/*
Copyright 2021-2021 Andreas Warnke

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
