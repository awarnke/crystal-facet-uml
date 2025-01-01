  /* File: draw_diagram_label.c; Copyright and License: see below */

#include "draw/draw_diagram_label.h"
#include "u8/u8_trace.h"
#include "entity/data_classifier.h"
#include "entity/data_diagramelement.h"
#include "u8/u8_f64.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int DRAW_DIAGRAM_PANGO_AUTO_DETECT_LENGTH = -1;

void draw_diagram_label_init( draw_diagram_label_t *this_ )
{
    draw_stereotype_icon_init( &((*this_).image_renderer) );
}

void draw_diagram_label_destroy( draw_diagram_label_t *this_ )
{
    draw_stereotype_icon_destroy( &((*this_).image_renderer) );
}

void draw_diagram_label_get_type_and_name_dimensions ( const draw_diagram_label_t *this_,
                                                       const data_diagram_t *diagram,
                                                       const data_profile_part_t *profile,
                                                       const geometry_dimensions_t *proposed_bounds,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       geometry_dimensions_t *out_label_dim )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagram );
    assert( NULL != profile );
    assert( NULL != proposed_bounds );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_label_dim );

    if ( data_diagram_is_valid( diagram ) )
    {
        /* calc stereotype image bounds */
        const char *const diagram_stereotype = data_diagram_get_stereotype_const( diagram );
        const bool has_stereotype_image
            = draw_stereotype_icon_exists( &((*this_).image_renderer), diagram_stereotype, profile );
        const geometry_dimensions_t icon_dim
            = has_stereotype_image
            ? draw_stereotype_icon_get_dimensions( &((*this_).image_renderer), pencil_size )
            : (geometry_dimensions_t){ .width = 0.0, .height = 0.0 };
        const double icon_gap = has_stereotype_image ? pencil_size_get_standard_object_border( pencil_size ) : 0.0;

        /* calc name text dimensions */
        /* int proposed_pango_width = geometry_dimensions_get_width( proposed_bounds ); */
        int text2_height = 0;
        int text2_width = 0;
        if ( 0 != utf8string_get_length( data_diagram_get_name_const( diagram ) ))
        {
            pango_layout_set_font_description( font_layout, pencil_size_get_standard_font_description( pencil_size ) );
            pango_layout_set_text( font_layout,
                                   data_diagram_get_name_const( diagram ),
                                   DRAW_DIAGRAM_PANGO_AUTO_DETECT_LENGTH
                                 );
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);
            text2_height += PENCIL_SIZE_FONT_ALIGN_MARGIN;  /* allow to align font with pixel border */
            text2_width += PENCIL_SIZE_FONT_ALIGN_MARGIN;
        }

        *out_label_dim = (geometry_dimensions_t) {
            .width = geometry_dimensions_get_width( &icon_dim ) + icon_gap + text2_width,
            .height = u8_f64_max2( geometry_dimensions_get_height( &icon_dim ), text2_height )
        };
    }
    else
    {
        U8_LOG_ERROR("invalid diagram in draw_diagram_label_get_type_and_name_dimensions()");
        *out_label_dim = (geometry_dimensions_t) { .width = 0.0, .height = 0.0 };
    }
    U8_TRACE_END();
}

void draw_diagram_label_draw_type_and_name ( const draw_diagram_label_t *this_,
                                             const data_diagram_t *diagram,
                                             const data_profile_part_t *profile,
                                             const GdkRGBA *color,
                                             const geometry_rectangle_t *label_box,
                                             const pencil_size_t *pencil_size,
                                             PangoLayout *font_layout,
                                             cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagram );
    assert( NULL != profile );
    assert( NULL != color );
    assert( NULL != label_box );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    /* calc bounds of stereotype icon */
    const char *const diagram_stereotype = data_diagram_get_stereotype_const( diagram );
    const bool has_stereotype_image
        = draw_stereotype_icon_exists( &((*this_).image_renderer), diagram_stereotype, profile );
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
                                          diagram_stereotype,
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
    const double text_left
        = geometry_rectangle_get_left( label_box ) + geometry_rectangle_get_width( &stereotype_box ) + icon_gap;
    const double text_top = geometry_rectangle_get_top( label_box );

    /* draw name text */
    if ( 0 != utf8string_get_length( data_diagram_get_name_const( diagram ) ))
    {
        cairo_set_source_rgba( cr, color->red, color->green, color->blue, color->alpha );
        pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
        pango_layout_set_text( font_layout,
                               data_diagram_get_name_const( diagram ),
                               DRAW_DIAGRAM_PANGO_AUTO_DETECT_LENGTH
                             );

        /* draw text */
        cairo_move_to ( cr, ceil( text_left ), ceil( text_top ) );  /* align font with pixel border */
        pango_cairo_show_layout (cr, font_layout);
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
