  /* File: draw_relationship_label.c; Copyright and License: see below */

#include "draw/draw_relationship_label.h"
#include "u8/u8_trace.h"
#include "entity/data_classifier.h"
#include "entity/data_diagramelement.h"
#include "u8/u8_i32.h"
#include "u8/u8_f64.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int DRAW_RELATIONSHIP_PANGO_UNLIMITED_WIDTH = -1;
static const int DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH = -1;
#define DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "\xc2\xab"
#define DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS "\xc2\xbb"

void draw_relationship_label_init( draw_relationship_label_t *this_ )
{
    utf8stream_writemem_init( &((*this_).text_builder), &((*this_).text_buffer), sizeof( (*this_).text_buffer) );
    draw_line_breaker_init( &((*this_).linebr) );
    draw_stereotype_icon_init( &((*this_).image_renderer) );
}

void draw_relationship_label_destroy( draw_relationship_label_t *this_ )
{
    draw_line_breaker_destroy( &((*this_).linebr) );
    const u8_error_t text_err = utf8stream_writemem_destroy( &((*this_).text_builder) );
    if ( text_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_HEX( "error at draw/draw_classifier_label: buffer too small", text_err );
    }
    draw_stereotype_icon_destroy( &((*this_).image_renderer) );
}

void draw_relationship_label_get_type_and_name_dimensions ( draw_relationship_label_t *this_,
                                                            const data_relationship_t *relationship,
                                                            const data_profile_part_t *profile,
                                                            const geometry_dimensions_t *proposed_bounds,
                                                            const pencil_size_t *pencil_size,
                                                            PangoLayout *font_layout,
                                                            geometry_dimensions_t *out_label_dim )
{
    U8_TRACE_BEGIN();
    assert( NULL != relationship );
    assert( NULL != profile );
    assert( NULL != proposed_bounds );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_label_dim );

    if ( data_relationship_is_valid( relationship ) )
    {
        /* calc stereotype image bounds */
        const char *const relationship_stereotype = data_relationship_get_stereotype_const( relationship );
        const bool has_stereotype_image
            = draw_stereotype_icon_exists( &((*this_).image_renderer), relationship_stereotype, profile );
        const geometry_dimensions_t icon_dim
            = has_stereotype_image
            ? draw_stereotype_icon_get_dimensions( &((*this_).image_renderer), pencil_size )
            : (geometry_dimensions_t){ .width = 0.0, .height = 0.0 };
        const double icon_gap = has_stereotype_image ? pencil_size_get_standard_object_border( pencil_size ) : 0.0;

        /* define names for input data */
        int proposed_pango_width = geometry_dimensions_get_width( proposed_bounds );
        const double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

        /* calc dimensions of stereotype as text */
        int text1_height = 0;
        int text1_width = 0;
        {
            const data_relationship_type_t rel_type = data_relationship_get_main_type( relationship );
            const char *const pseudo_stereotype
                = draw_relationship_label_private_stereotype_from_type( this_, rel_type );
            const bool has_pseudo_stereotype = ( ! utf8string_equals_str( pseudo_stereotype, "" ) );
            const bool has_stereotype = ( ! utf8string_equals_str( relationship_stereotype, "" ) );

            if ( ( ! has_stereotype_image ) && ( has_pseudo_stereotype || has_stereotype ) )
            {
                /* prepare text */
                char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                utf8stringbuf_copy_str( &stereotype_buf, DRAW_RELATIONSHIP_LEFT_GUILLEMENTS );
                if ( has_stereotype )
                {
                    utf8stringbuf_append_str( &stereotype_buf, relationship_stereotype );
                }
                else
                {
                    utf8stringbuf_append_str( &stereotype_buf, pseudo_stereotype );
                }
                utf8stringbuf_append_str( &stereotype_buf, DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS );

                /* determine text width and height */
                pango_layout_set_font_description( font_layout, pencil_size_get_footnote_font_description( pencil_size ) );
                pango_layout_set_text( font_layout, utf8stringbuf_get_string( &stereotype_buf ), DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH );
                pango_layout_get_pixel_size( font_layout, &text1_width, &text1_height );
                text1_height += PENCIL_SIZE_FONT_ALIGN_MARGIN;  /* allow to align font with pixel border */
                text1_width += PENCIL_SIZE_FONT_ALIGN_MARGIN;
            }
        }

        /* calc name text dimensions */
        int text2_height = 0;
        int text2_width = 0;
        if ( 0 != utf8string_get_length( data_relationship_get_name_const( relationship ) ))
        {
            u8_error_t name_err = U8_ERROR_NONE;
            utf8stream_writer_t *to_name = utf8stream_writemem_get_writer( &((*this_).text_builder) );

            /* append parts to name and insert linebreaks */
            utf8stringview_t rel_name = UTF8STRINGVIEW_STR( data_relationship_get_name_const( relationship ) );
            name_err |= draw_line_breaker_append( &((*this_).linebr), &rel_name, to_name );
            const utf8stringview_t name = utf8stream_writemem_get_view( &((*this_).text_builder) );

            pango_layout_set_font_description( font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text( font_layout,
                                   utf8stringview_get_start( &name ),
                                   utf8stringview_get_length( &name )
                                 );
            pango_layout_set_width( font_layout, proposed_pango_width * PANGO_SCALE );
            pango_layout_get_pixel_size( font_layout, &text2_width, &text2_height );
            text2_height += PENCIL_SIZE_FONT_ALIGN_MARGIN;  /* allow to align font with pixel border */
            text2_width += PENCIL_SIZE_FONT_ALIGN_MARGIN;

            /* restore pango context */
            pango_layout_set_width(font_layout, DRAW_RELATIONSHIP_PANGO_UNLIMITED_WIDTH);

            /* cleanup the text_builder */
            name_err |= utf8stream_writemem_reset( &((*this_).text_builder) );
            if ( name_err != U8_ERROR_NONE )
            {
                U8_LOG_WARNING_HEX( "error at get_dim/draw_line_breaker_append", name_err );
            }
        }

        *out_label_dim = (geometry_dimensions_t){
            .width = geometry_dimensions_get_width( &icon_dim ) + icon_gap + u8_i32_max2( text2_width, text1_width ),
            .height = u8_f64_max2( geometry_dimensions_get_height( &icon_dim ), text1_height + f_line_gap + text2_height )
        };
    }
    else
    {
        U8_LOG_ERROR("invalid relationship in draw_relationship_label_get_type_and_name_dimensions()");
        *out_label_dim = (geometry_dimensions_t) { .width = 0.0, .height = 0.0 };
    }
    U8_TRACE_END();
}

void draw_relationship_label_draw_type_and_name ( draw_relationship_label_t *this_,
                                                  const data_relationship_t *relationship,
                                                  const data_profile_part_t *profile,
                                                  const GdkRGBA *color,
                                                  const geometry_rectangle_t *label_box,
                                                  const pencil_size_t *pencil_size,
                                                  PangoLayout *font_layout,
                                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != relationship );
    assert( NULL != profile );
    assert( NULL != color );
    assert( NULL != label_box );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    /* calc bounds of stereotype icon */
    const char *const relationship_stereotype = data_relationship_get_stereotype_const( relationship );
    const bool has_stereotype_image
        = draw_stereotype_icon_exists( &((*this_).image_renderer), relationship_stereotype, profile );
    const geometry_rectangle_t stereotype_box
        = has_stereotype_image
        ? draw_stereotype_icon_get_bounds( &((*this_).image_renderer),
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
            = draw_stereotype_icon_draw( &((*this_).image_renderer),
                                          relationship_stereotype,
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

    /* define names for input data */
    const double text_width
        = geometry_rectangle_get_width( label_box ) - geometry_rectangle_get_width( &stereotype_box ) - icon_gap;
    const double center_x
        = geometry_rectangle_get_left( label_box ) + geometry_rectangle_get_width( &stereotype_box ) + icon_gap
        + 0.5 * text_width;
    const double top = geometry_rectangle_get_top( label_box );
    const double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* draw stereotype as text */
    int text1_height = 0;
    {
        const data_relationship_type_t rel_type = data_relationship_get_main_type( relationship );
        const char *const pseudo_stereotype
            = draw_relationship_label_private_stereotype_from_type( this_, rel_type );
        const bool has_pseudo_stereotype = ( ! utf8string_equals_str( pseudo_stereotype, "" ) );
        const bool has_stereotype = ( ! utf8string_equals_str( relationship_stereotype, "" ) );

        if ( ( ! has_stereotype_image ) && ( has_pseudo_stereotype || has_stereotype ) )
        {
            /* prepare text */
            char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
            utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
            utf8stringbuf_copy_str( &stereotype_buf, DRAW_RELATIONSHIP_LEFT_GUILLEMENTS );
            if ( has_stereotype )
            {
                utf8stringbuf_append_str( &stereotype_buf, relationship_stereotype );
            }
            else
            {
                utf8stringbuf_append_str( &stereotype_buf, pseudo_stereotype );
            }
            utf8stringbuf_append_str( &stereotype_buf, DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS );

            int text1_width;
            cairo_set_source_rgba( cr, color->red, color->green, color->blue, color->alpha );
            pango_layout_set_font_description( font_layout, pencil_size_get_footnote_font_description( pencil_size ) );
            pango_layout_set_text( font_layout,
                                   utf8stringbuf_get_string( &stereotype_buf ),
                                   DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH
                                 );
            pango_layout_get_pixel_size( font_layout, &text1_width, &text1_height );

            /* draw text */
            cairo_move_to( cr, ceil( center_x - 0.5*text1_width ), ceil( top ) );  /* align font with pixel border */
            pango_cairo_show_layout( cr, font_layout );
        }
    }

    /* draw name text */
    if ( 0 != utf8string_get_length( data_relationship_get_name_const( relationship ) ))
    {
        u8_error_t name_err = U8_ERROR_NONE;
        utf8stream_writer_t *to_name = utf8stream_writemem_get_writer( &((*this_).text_builder) );

        /* append parts to name and insert linebreaks */
        utf8stringview_t rel_name = UTF8STRINGVIEW_STR( data_relationship_get_name_const( relationship ) );
        name_err |= draw_line_breaker_append( &((*this_).linebr), &rel_name, to_name );
        const utf8stringview_t name = utf8stream_writemem_get_view( &((*this_).text_builder) );

        int text2_height;
        int text2_width;
        const double f_size = pencil_size_get_standard_font_size( pencil_size );
        cairo_set_source_rgba( cr, color->red, color->green, color->blue, color->alpha );
        pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
        pango_layout_set_text( font_layout,
                               utf8stringview_get_start( &name ),
                               utf8stringview_get_length( &name )
                             );
        pango_layout_set_width(font_layout, (text_width+f_size) * PANGO_SCALE );  /* add gap to avoid line breaks by rounding errors and whitespace character widths */
        pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

        /* draw text */
        cairo_move_to( cr,
                       ceil( center_x - 0.5*text2_width ),
                       ceil( top + text1_height + f_line_gap )
                     );  /* align font with pixel border */
        pango_cairo_show_layout( cr, font_layout );

        /* restore pango context */
        pango_layout_set_width(font_layout, DRAW_RELATIONSHIP_PANGO_UNLIMITED_WIDTH);

        /* cleanup the text_builder */
        name_err |= utf8stream_writemem_reset( &((*this_).text_builder) );
        if ( name_err != U8_ERROR_NONE )
        {
            U8_LOG_WARNING_HEX( "error at get_dim/draw_line_breaker_append", name_err );
        }
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2025 Andreas Warnke
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
