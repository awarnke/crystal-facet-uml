  /* File: draw_relationship_label.c; Copyright and License: see below */

#include "draw/draw_relationship_label.h"
#include "u8/u8_trace.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "u8/u8_i32.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH = -1;
#define DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "\xc2\xab"
#define DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS "\xc2\xbb"

void draw_relationship_label_get_type_and_name_dimensions ( const draw_relationship_label_t *this_,
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
            = draw_stereotype_image_exists( &((*this_).image_renderer), relationship_stereotype, profile );
        const geometry_dimensions_t icon_dim
            = has_stereotype_image
            ? draw_stereotype_image_get_dimensions( &((*this_).image_renderer), pencil_size )
            : (geometry_dimensions_t){ .width = 0.0, .height = 0.0 };
        const double icon_gap = has_stereotype_image ? pencil_size_get_standard_object_border( pencil_size ) : 0.0;

        /* define names for input data */
        const double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

        /* calc dimensions of typename as stereotype, */
        /* needed for relations that look like DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY */
        int text3_height = 0;
        int text3_width = 0;
        {
            const char *type_text;
            switch ( data_relationship_get_main_type( relationship ) )
            {
                case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
                {
                    type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "extends" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
                }
                break;

                case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
                {
                    type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "includes" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
                }
                break;

                case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
                {
                    type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "deploy" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
                }
                break;

                case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
                {
                    type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "manifest" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
                }
                break;

                case DATA_RELATIONSHIP_TYPE_UML_REFINE:
                {
                    type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "refine" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
                }
                break;

                case DATA_RELATIONSHIP_TYPE_UML_TRACE:
                {
                    type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "trace" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
                }
                break;

                default:
                {
                    /* other types do not show their type */
                    type_text = NULL;
                }
                break;
            }

            if ( NULL != type_text )
            {
                pango_layout_set_font_description (font_layout, pencil_size_get_footnote_font_description(pencil_size) );
                pango_layout_set_text (font_layout, type_text, DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH);
                pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);
            }
        }

        /* calc name text dimensions */
        int text2_height = 0;
        int text2_width = 0;
        if ( 0 != utf8string_get_length( data_relationship_get_name_const( relationship ) ))
        {
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text( font_layout,
                                   data_relationship_get_name_const( relationship ),
                                   DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH
                                 );
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);
        }

        *out_label_dim = (geometry_dimensions_t){
            .width = u8_i32_max2( text2_width, text3_width ),
            .height = text3_height + f_line_gap + text2_height
        };
    }
    else
    {
        U8_LOG_ERROR("invalid relationship in draw_relationship_label_get_type_and_name_dimensions()");
        *out_label_dim = (geometry_dimensions_t) { .width = 0.0, .height = 0.0 };
    }
    U8_TRACE_END();
}

void draw_relationship_label_draw_type_and_name ( const draw_relationship_label_t *this_,
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

    /* define names for input data */
    const double center_x = geometry_rectangle_get_center_x( label_box );
    const double top = geometry_rectangle_get_top( label_box );
    const double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* draw the typename as stereotype, */
    /* needed for relations that look like DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY */
    int text3_height = 0;
    {
        const char *type_text;
        switch ( data_relationship_get_main_type( relationship ) )
        {
            case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
            {
                type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "extends" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
            {
                type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "includes" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
            {
                type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "deploy" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
            {
                type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "manifest" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_REFINE:
            {
                type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "refine" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
            }
            break;

            case DATA_RELATIONSHIP_TYPE_UML_TRACE:
            {
                type_text = DRAW_RELATIONSHIP_LEFT_GUILLEMENTS "trace" DRAW_RELATIONSHIP_RIGHT_GUILLEMENTS;
            }
            break;

            default:
            {
                /* other types do not show their type */
                type_text = NULL;
            }
            break;
        }

        if ( NULL != type_text )
        {
            int text3_width;
            cairo_set_source_rgba( cr, color->red, color->green, color->blue, color->alpha );
            pango_layout_set_font_description (font_layout, pencil_size_get_footnote_font_description(pencil_size) );
            pango_layout_set_text (font_layout, type_text, DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH);
            pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);

            /* draw text */
            cairo_move_to ( cr, center_x - 0.5*text3_width, top );
            pango_cairo_show_layout (cr, font_layout);
        }
    }

    /* draw name text */
    if ( 0 != utf8string_get_length( data_relationship_get_name_const( relationship ) ))
    {
        int text2_height;
        int text2_width;
        cairo_set_source_rgba( cr, color->red, color->green, color->blue, color->alpha );
        pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
        pango_layout_set_text( font_layout,
                               data_relationship_get_name_const( relationship ),
                               DRAW_RELATIONSHIP_PANGO_AUTO_DETECT_LENGTH
                             );
        pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

        /* draw text */
        cairo_move_to ( cr, center_x - 0.5*text2_width, top + text3_height + f_line_gap );
        pango_cairo_show_layout (cr, font_layout);
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2023 Andreas Warnke
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
