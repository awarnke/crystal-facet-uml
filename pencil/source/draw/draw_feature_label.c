/* File: draw_feature_label.c; Copyright and License: see below */

#include "draw/draw_feature_label.h"
#include "u8/u8_trace.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "u8/u8_f64.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int DRAW_FEATURE_PANGO_UNLIMITED_WIDTH = -1;
static const int DRAW_LABEL_PANGO_AUTO_DETECT_LENGTH = -1;

void draw_feature_label_get_key_and_value_dimensions ( const draw_feature_label_t *this_,
                                                       const data_feature_t *feature,
                                                       const data_profile_part_t *profile,
                                                       const geometry_dimensions_t *proposed_bounds,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       geometry_dimensions_t *out_label_dim )
{
    U8_TRACE_BEGIN();
    assert( NULL != feature );
    assert( NULL != profile );
    assert( NULL != proposed_bounds );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_label_dim );

    if ( data_feature_is_valid( feature ) )
    {
        /* calc stereotype image bounds */
        const char *const feature_stereotype = data_feature_get_value_const( feature );
        const bool has_stereotype_image
            = draw_stereotype_image_exists( &((*this_).image_renderer), feature_stereotype, profile );
        const geometry_dimensions_t icon_dim
            = has_stereotype_image
            ? draw_stereotype_image_get_dimensions( &((*this_).image_renderer), pencil_size )
            : (geometry_dimensions_t){ .width = 0.0, .height = 0.0 };
        const double icon_gap = has_stereotype_image ? pencil_size_get_standard_object_border( pencil_size ) : 0.0;

        /* draw text - except for lifelines */
        int proposed_pango_width = geometry_dimensions_get_width( proposed_bounds );
        int text_width = 0;
        int text_height = 0;
        if ( DATA_FEATURE_TYPE_LIFELINE != data_feature_get_main_type (feature) )
        {
            /* prepare text */
            char label_text[DATA_FEATURE_MAX_KEY_SIZE + DATA_FEATURE_MAX_VALUE_SIZE + 2 ];
            utf8stringbuf_t label_buf = UTF8STRINGBUF(label_text);
            utf8stringbuf_copy_str( label_buf, data_feature_get_key_const( feature ) );
            if ( data_feature_has_value( feature ) && ( ! has_stereotype_image ) )
            {
                utf8stringbuf_append_str( label_buf, ": " );
                utf8stringbuf_append_str( label_buf, data_feature_get_value_const( feature ) );
            }

            /* determine text width and height */
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( label_buf ), DRAW_LABEL_PANGO_AUTO_DETECT_LENGTH );
            pango_layout_set_width(font_layout, proposed_pango_width * PANGO_SCALE );
            pango_layout_get_pixel_size (font_layout, &text_width, &text_height);
            /* restore pango context */
            pango_layout_set_width(font_layout, DRAW_FEATURE_PANGO_UNLIMITED_WIDTH);
        }

        *out_label_dim = (geometry_dimensions_t) {
            .width = geometry_dimensions_get_width( &icon_dim ) + icon_gap + text_width,
            .height = u8_f64_max2( geometry_dimensions_get_height( &icon_dim ), text_height )
        };
    }
    else
    {
        U8_LOG_ERROR("invalid feature in draw_feature_label_get_key_and_value_dimensions()");
        *out_label_dim = (geometry_dimensions_t) { .width = 0.0, .height = 0.0 };
    }
    U8_TRACE_END();
}

void draw_feature_label_draw_key_and_value ( const draw_feature_label_t *this_,
                                             const data_feature_t *feature,
                                             const data_profile_part_t *profile,
                                             const GdkRGBA *color,
                                             const geometry_rectangle_t *label_box,
                                             const pencil_size_t *pencil_size,
                                             PangoLayout *font_layout,
                                             cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != feature );
    assert( NULL != profile );
    assert( NULL != color );
    assert( NULL != label_box );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    /* calc bounds of stereotype icon */
    const char *const feature_stereotype = data_feature_get_value_const( feature );
    const bool has_stereotype_image
        = draw_stereotype_image_exists( &((*this_).image_renderer), feature_stereotype, profile );
    const geometry_rectangle_t stereotype_box
        = has_stereotype_image
        ? draw_stereotype_image_get_bounds( &((*this_).image_renderer),
                                            geometry_rectangle_get_left( label_box ),
                                            geometry_rectangle_get_top( label_box ),
                                            GEOMETRY_H_ALIGN_LEFT,
                                            GEOMETRY_V_ALIGN_TOP,
                                            pencil_size
                                          )
        : (geometry_rectangle_t){ .left = 0.0, .top = 0.0, .width = 0.0, .height = 0.0 };
    const double icon_gap = has_stereotype_image ? pencil_size_get_standard_object_border( pencil_size ) : 0.0;

    /* draw stereotype icon */
    if ( has_stereotype_image )
    {
        u8_error_info_t err_info;
        const u8_error_t stereotype_err
            = draw_stereotype_image_draw( &((*this_).image_renderer),
                                          feature_stereotype,
                                          profile,
                                          color,
                                          &err_info,
                                          &stereotype_box,
                                          cr
                                        );
        if ( u8_error_info_is_error( &err_info ) )
        {
            U8_LOG_WARNING_INT( "stereotype image: unxpected token in svg path in line",
                                u8_error_info_get_line( &err_info )
                              );
        }
        else if ( stereotype_err != U8_ERROR_NONE )
        {
            U8_LOG_WARNING_HEX( "error at drawing stereotype image", stereotype_err );
        }
    }

    /* define names for input data: */
    const double text_left
        = geometry_rectangle_get_left( label_box ) + geometry_rectangle_get_width( &stereotype_box ) + icon_gap;
    const double text_top = geometry_rectangle_get_top( label_box );
    const double text_width
        = geometry_rectangle_get_width( label_box ) - geometry_rectangle_get_width( &stereotype_box ) - icon_gap;

    /* draw text - except for lifelines */
    if ( DATA_FEATURE_TYPE_LIFELINE != data_feature_get_main_type (feature) )
    {
        /* prepare text */
        char label_text[DATA_FEATURE_MAX_KEY_SIZE + DATA_FEATURE_MAX_VALUE_SIZE + 2 ];
        utf8stringbuf_t label_buf = UTF8STRINGBUF(label_text);
        utf8stringbuf_copy_str( label_buf, data_feature_get_key_const( feature ) );
        if ( data_feature_has_value( feature ) && ( ! has_stereotype_image ) )
        {
            utf8stringbuf_append_str( label_buf, ": " );
            utf8stringbuf_append_str( label_buf, data_feature_get_value_const( feature ) );
        }

        const double f_size = pencil_size_get_standard_font_size( pencil_size );
        cairo_set_source_rgba( cr, color->red, color->green, color->blue, color->alpha );
        pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
        pango_layout_set_text (font_layout, utf8stringbuf_get_string( label_buf ), DRAW_LABEL_PANGO_AUTO_DETECT_LENGTH);
        pango_layout_set_width(font_layout, (text_width+f_size) * PANGO_SCALE );  /* add gap to avoid line breaks by rounding errors and whitespace character widths */

        /* draw text */
        cairo_move_to ( cr, text_left, text_top );
        pango_cairo_show_layout (cr, font_layout);

        /* restore pango context */
        pango_layout_set_width(font_layout, DRAW_FEATURE_PANGO_UNLIMITED_WIDTH);
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2024 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
