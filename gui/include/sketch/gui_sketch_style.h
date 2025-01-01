/* File: gui_sketch_style.h; Copyright and License: see below */

#ifndef GUI_SKETCH_STYLE_H
#define GUI_SKETCH_STYLE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides a set of size values used to draw icons and boxes and text
 */

#include "pencil_size.h"
#include <pango/pangocairo.h>
#include <cairo.h>
#include <gdk/gdk.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the gui_sketch_style_t
 */
struct gui_sketch_style_struct {
    pencil_size_t pencil_size;  /*!< size, color and font settings derived from pencil package */
};

typedef struct gui_sketch_style_struct gui_sketch_style_t;

/*!
 *  \brief initializes the size values
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_style_init( gui_sketch_style_t *this_ );

/*!
 *  \brief destroys the gui_sketch_style_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_style_destroy( gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: footnote_font_size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_footnote_font_size( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the PangoFontDescription for the footnote font
 *
 *  \param this_ pointer to own object attributes
 */
static inline const PangoFontDescription *gui_sketch_style_get_footnote_font_description( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: standard_font_size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_standard_font_size( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the PangoFontDescription for the standard font
 *
 *  \param this_ pointer to own object attributes
 */
static inline const PangoFontDescription *gui_sketch_style_get_standard_font_description( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: title_font_size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_title_font_size( const gui_sketch_style_t *this_ );

/*!
 *  \brief determines PangoFontDescription for the title font
 *
 *  \param this_ pointer to own object attributes
 */
static inline const PangoFontDescription *gui_sketch_style_get_title_font_description( const gui_sketch_style_t *this_ );

/*!
 *  \brief proposes a tab size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_font_tab_size( const gui_sketch_style_t *this_ );

/*!
 *  \brief determines the gap between font lines
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_font_line_gap( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: standard_line_width
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_standard_line_width( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: bold_line_width
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_bold_line_width( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: standard_object_border aka gap
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_standard_object_border( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: preferred_object_distance
 *
 *  \param this_ pointer to own object attributes
 */
static inline double gui_sketch_style_get_preferred_object_distance( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: standard_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA gui_sketch_style_get_standard_color( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: select_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA gui_sketch_style_get_select_color( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: highlight_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA gui_sketch_style_get_highlight_color( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: focus_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA gui_sketch_style_get_focus_color( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the background-color attribute of gui_sketch_style_t: emphasized_bgcolor
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA gui_sketch_style_get_emphasized_bgcolor( const gui_sketch_style_t *this_ );

/*!
 *  \brief gets the attribute of gui_sketch_style_t: gray_out_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA gui_sketch_style_get_gray_out_color( const gui_sketch_style_t *this_ );

#include "gui_sketch_style.inl"

#endif  /* GUI_SKETCH_STYLE_H */


/*
Copyright 2021-2025 Andreas Warnke

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
