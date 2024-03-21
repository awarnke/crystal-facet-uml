/* File: pencil_size.h; Copyright and License: see below */

#ifndef PENCIL_SIZE_H
#define PENCIL_SIZE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides a set of size values used to draw a diagram
 */

#include <pango/pangocairo.h>
#include <cairo.h>
#include <gdk/gdk.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief enumeration of alignment buffers for text rendering
 */
enum pencil_size_font_enum {
    PENCIL_SIZE_FONT_ALIGN_MARGIN = 1,  /*!< to align font rendering on pixel borders, 1 pixel margin is needed */
};

/*!
 *  \brief attributes of the pencil_size_t
 */
struct pencil_size_struct {
    double footnote_font_size;  /*!< text size of footnote text */
    PangoFontDescription *footnote_font_description;  /*!< text description of footnote text */
    double standard_font_size;  /*!< text size of standard text */
    PangoFontDescription *standard_font_description;  /*!< text description of standard text */
    double title_font_size;  /*!< text size of title text */
    PangoFontDescription *title_font_description;  /*!< text description of title text */
    double standard_line_width;  /*!< line width of standard lines */
    double bold_line_width;  /*!< line width of bold lines */
    double line_dash_length;  /*!< dash length of a short-dashed line */
    double standard_object_border;  /*!< gap between the bounding rectangle and the outer line of an object */
    double arrow_stroke_length;  /*!< length of a stroke to paint an arrow tip */
    double arrow_stroke_087_length;  /*!< sqrt(0.75)=0.8660 parts of the length of a stroke to paint an arrow */
    double preferred_object_distance;  /*!< preferred distance between two objects or object and connector */
    double classifier_symbol_height;  /*!< height of actors, timeouts, fork, join, start, end */
    GdkRGBA standard_color;  /*!< foreground color of all standard objects */
    GdkRGBA select_color;  /*!< foreground color of all currently selected objects (user selected set): pink */
    GdkRGBA highlight_color;  /*!< foreground color of the current highlighted/mouse over object: turquoise */
    GdkRGBA focus_color;  /*!< foreground color of the one currently focused object: yellow */
    GdkRGBA emphasized_bgcolor;  /*!< background color of the durable-emphasized objects: yellow */
    GdkRGBA gray_out_color;  /*!< foreground color of the durable-greyed_out objects: gray */
};

typedef struct pencil_size_struct pencil_size_t;

/*!
 *  \brief initializes the size values
 *
 *  \param this_ pointer to own object attributes
 *  \param width of the area where to draw the diagram
 *  \param height of the area where to draw the diagram
 */
static inline void pencil_size_init( pencil_size_t *this_, double width, double height );

/*!
 *  \brief initializes the size values by defaults
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_size_init_empty( pencil_size_t *this_ );

/*!
 *  \brief destroys the pencil_size_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_size_destroy( pencil_size_t *this_ );

/*!
 *  \brief re-initializes the pencil_size_t
 *
 *  \param this_ pointer to own object attributes
 *  \param width of the area where to draw the diagram
 *  \param height of the area where to draw the diagram
 */
static inline void pencil_size_reinit( pencil_size_t *this_, double width, double height  );

/*!
 *  \brief gets the attribute of pencil_size_t: footnote_font_size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_footnote_font_size( const pencil_size_t *this_ );

/*!
 *  \brief gets the PangoFontDescription for the footnote font
 *
 *  \param this_ pointer to own object attributes
 */
static inline const PangoFontDescription *pencil_size_get_footnote_font_description( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: standard_font_size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_standard_font_size( const pencil_size_t *this_ );

/*!
 *  \brief gets the PangoFontDescription for the standard font
 *
 *  \param this_ pointer to own object attributes
 */
static inline const PangoFontDescription *pencil_size_get_standard_font_description( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: title_font_size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_title_font_size( const pencil_size_t *this_ );

/*!
 *  \brief determines PangoFontDescription for the title font
 *
 *  \param this_ pointer to own object attributes
 */
static inline const PangoFontDescription *pencil_size_get_title_font_description( const pencil_size_t *this_ );

/*!
 *  \brief proposes a tab size
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_font_tab_size( const pencil_size_t *this_ );

/*!
 *  \brief determines the gap between font lines
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_font_line_gap( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: standard_line_width
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_standard_line_width( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: bold_line_width
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_bold_line_width( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: line_dash_length
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_line_dash_length( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: standard_object_border aka gap
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_standard_object_border( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: arrow_stroke_length
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_arrow_stroke_length( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: arrow_stroke_087_length
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_arrow_stroke_087_length( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: preferred_object_distance
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_preferred_object_distance( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: classifier_symbol_height
 *
 *  \param this_ pointer to own object attributes
 */
static inline double pencil_size_get_classifier_symbol_height( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: standard_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA pencil_size_get_standard_color( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: select_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA pencil_size_get_select_color( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: highlight_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA pencil_size_get_highlight_color( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: focus_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA pencil_size_get_focus_color( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: emphasized_bgcolor
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA pencil_size_get_emphasized_bgcolor( const pencil_size_t *this_ );

/*!
 *  \brief gets the attribute of pencil_size_t: gray_out_color
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkRGBA pencil_size_get_gray_out_color( const pencil_size_t *this_ );

#include "pencil_size.inl"

#endif  /* PENCIL_SIZE_H */


/*
Copyright 2016-2024 Andreas Warnke

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
