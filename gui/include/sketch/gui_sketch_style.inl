/* File: gui_sketch_style.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static const char *const GUI_SKETCH_STYLE_FONT_FAMILY = "Sans";

static inline void gui_sketch_style_init( gui_sketch_style_t *this_ )
{
    pencil_size_init_empty( &((*this_).pencil_size) );
    /* the grid is snapped to rather easily, there is no risk of accidentially selecting a close-by grid position */
    (*this_).snap_to_grid_distance = 8.000001;
    /* a relationship must be selected rather accurately, to avoid not being able to select something close by */
    (*this_).snap_to_relationship = 3;
}

static inline void gui_sketch_style_destroy( gui_sketch_style_t *this_ )
{
    pencil_size_destroy( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_snap_to_grid( const gui_sketch_style_t *this_ )
{
    return (*this_).snap_to_grid_distance;
}

static inline int32_t gui_sketch_style_get_snap_to_relationship( const gui_sketch_style_t *this_ )
{
    return (*this_).snap_to_relationship;
}

static inline double gui_sketch_style_get_footnote_font_size( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_footnote_font_size( &((*this_).pencil_size) );
}

static inline const PangoFontDescription *gui_sketch_style_get_footnote_font_description( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_footnote_font_description( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_standard_font_size( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_standard_font_size( &((*this_).pencil_size) );
}

static inline const PangoFontDescription *gui_sketch_style_get_standard_font_description( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_standard_font_description( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_title_font_size( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_title_font_size( &((*this_).pencil_size) );
}

static inline const PangoFontDescription *gui_sketch_style_get_title_font_description( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_title_font_description( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_font_tab_size( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_font_tab_size( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_font_line_gap( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_font_line_gap( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_standard_line_width( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_standard_line_width( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_bold_line_width( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_bold_line_width( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_standard_object_border( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_standard_object_border( &((*this_).pencil_size) );
}

static inline double gui_sketch_style_get_preferred_object_distance( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_preferred_object_distance( &((*this_).pencil_size) );
}

static inline GdkRGBA gui_sketch_style_get_standard_color( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_standard_color( &((*this_).pencil_size) );
}

static inline GdkRGBA gui_sketch_style_get_select_color( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_select_color( &((*this_).pencil_size) );
}

static inline GdkRGBA gui_sketch_style_get_highlight_color( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_highlight_color( &((*this_).pencil_size) );
}

static inline GdkRGBA gui_sketch_style_get_focus_color( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_focus_color( &((*this_).pencil_size) );
}

static inline GdkRGBA gui_sketch_style_get_emphasized_bgcolor( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_emphasized_bgcolor( &((*this_).pencil_size) );
}

static inline GdkRGBA gui_sketch_style_get_gray_out_color( const gui_sketch_style_t *this_ )
{
    return pencil_size_get_gray_out_color( &((*this_).pencil_size) );
}


/*
Copyright 2021-2026 Andreas Warnke

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
