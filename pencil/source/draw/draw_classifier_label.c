/* File: draw_classifier_label.c; Copyright and License: see below */

#include "draw/draw_classifier_label.h"
#include "u8/u8_trace.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int DRAW_CLASSIFIER_PANGO_UNLIMITED_WIDTH = -1;
static const int DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH = -1;
#define DRAW_CLASSIFIER_LEFT_POINTING_GUILLEMENTS "\xc2\xab"
#define DRAW_CLASSIFIER_RIGHT_POINTING_GUILLEMENTS "\xc2\xbb"
#define DRAW_CLASSIFIER_COLON ":"

void draw_classifier_label_get_stereotype_and_name_dimensions( const draw_classifier_label_t *this_,
                                                               const data_visible_classifier_t *visible_classifier,
                                                               const geometry_dimensions_t *proposed_bounds,
                                                               const pencil_size_t *pencil_size,
                                                               PangoLayout *font_layout,
                                                               double *out_text_width,
                                                               double *out_text_height )
{
    U8_TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != proposed_bounds );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_text_width );
    assert( NULL != out_text_height );

    if ( data_visible_classifier_is_valid( visible_classifier ) )
    {
        const data_classifier_t *classifier;
        const data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_const( visible_classifier );
        diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );
        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );

        /* stereotype text */
        int text1_height = 0;
        int text1_width = 0;
        {
            if ( 0 != utf8string_get_length( data_classifier_get_stereotype_const( classifier ) ) )
            {
                /* prepare text */
                char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                utf8stringbuf_copy_str( stereotype_buf, DRAW_CLASSIFIER_LEFT_POINTING_GUILLEMENTS );
                utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_const( classifier ) );
                utf8stringbuf_append_str( stereotype_buf, DRAW_CLASSIFIER_RIGHT_POINTING_GUILLEMENTS );

                /* determine text width and height */
                pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
                pango_layout_set_text (font_layout, utf8stringbuf_get_string( stereotype_buf ), DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH );
                pango_layout_get_pixel_size (font_layout, &text1_width, &text1_height);
            }
        }

        /* draw name text */
        int text2_width;
        int text2_height;
        double space_for_line;
        {
            int proposed_pango_width = geometry_dimensions_get_width( proposed_bounds );

            /* prepare text */
            char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE + 1 ];
            utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE ) )
            {
                utf8stringbuf_copy_str( name_buf, DRAW_CLASSIFIER_COLON );
            }
            else
            {
                utf8stringbuf_clear( name_buf );
            }
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_const( classifier ) );

            /* determine text width and height */
            pango_layout_set_font_description (font_layout, pencil_size_get_title_font_description(pencil_size) );
            pango_layout_set_text ( font_layout,
                                    utf8stringbuf_get_string( name_buf ),
                                    DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH
                                  );
            pango_layout_set_width(font_layout, proposed_pango_width * PANGO_SCALE );
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

            /* restore pango context */
            pango_layout_set_width(font_layout, DRAW_CLASSIFIER_PANGO_UNLIMITED_WIDTH );

            /* for space between stereotype and name */
            text2_height += pencil_size_get_font_line_gap( pencil_size );

            /* for underscores under object instance names, add 2 * gap: */
            space_for_line = 2.0 * pencil_size_get_standard_object_border( pencil_size );
        }

        /* draw description text */
        int text3_width = 0;
        int text3_height = 0;
        if ( DATA_CLASSIFIER_TYPE_COMMENT == data_classifier_get_main_type ( classifier ) )
        {
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text ( font_layout,
                                    data_classifier_get_description_const( classifier ),
                                    DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH
                                  );
            pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);
        }

        *out_text_height = text1_height + text2_height + space_for_line + text3_height;
        double intermediate_max_w;
        intermediate_max_w = ( text1_width > text2_width ) ? text1_width : text2_width;
        *out_text_width = ( intermediate_max_w > text3_width ) ? intermediate_max_w : text3_width;
    }
    else
    {
        U8_LOG_ERROR("invalid visible classifier in draw_classifier_label_get_stereotype_and_name_dimensions()");
        *out_text_width = 0.0;
        *out_text_height = 0.0;
    }
    U8_TRACE_END();
}

void draw_classifier_label_draw_stereotype_and_name( const draw_classifier_label_t *this_,
                                                     const data_visible_classifier_t *visible_classifier,
                                                     const geometry_rectangle_t *label_box,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != label_box );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    /* define names for input data: */
    const data_classifier_t *classifier;
    const data_diagramelement_t *diagramelement;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );
    const data_diagramelement_flag_t display_flags = data_diagramelement_get_display_flags( diagramelement );

    const double left = geometry_rectangle_get_left( label_box );
    const double top = geometry_rectangle_get_top( label_box );
    const double width = geometry_rectangle_get_width( label_box );
    const double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* draw stereotype text */
    int text1_height = 0;
    {
        if ( 0 != utf8string_get_length( data_classifier_get_stereotype_const( classifier ) ) )
        {
            /* prepare text */
            char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
            utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
            utf8stringbuf_copy_str( stereotype_buf, DRAW_CLASSIFIER_LEFT_POINTING_GUILLEMENTS );
            utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_const( classifier ) );
            utf8stringbuf_append_str( stereotype_buf, DRAW_CLASSIFIER_RIGHT_POINTING_GUILLEMENTS );

            int text1_width;
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text ( font_layout,
                                    utf8stringbuf_get_string( stereotype_buf ),
                                    DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH
                                  );
            pango_layout_get_pixel_size (font_layout, &text1_width, &text1_height);
            cairo_move_to ( cr, left + 0.5*( width - text1_width ), top );
            pango_cairo_show_layout (cr, font_layout);
        }
    }

    /* draw name text */
    int text2_height = 0;
    {
        /* prepare text */
        char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE + 1 ];
        utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
        if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE ) )
        {
            utf8stringbuf_copy_str( name_buf, DRAW_CLASSIFIER_COLON );
        }
        else
        {
            utf8stringbuf_clear( name_buf );
        }
        utf8stringbuf_append_str( name_buf, data_classifier_get_name_const( classifier ) );

        int text2_width;
        double f_size = pencil_size_get_standard_font_size( pencil_size );
        pango_layout_set_font_description (font_layout, pencil_size_get_title_font_description(pencil_size) );
        pango_layout_set_text ( font_layout,
                                utf8stringbuf_get_string( name_buf ),
                                DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH
                              );
        pango_layout_set_width(font_layout, (width+f_size) * PANGO_SCALE );  /* add gap to avoid line breaks by rounding errors and whitespace character widths */
        pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

        /* draw text */
        cairo_move_to ( cr, left + 0.5*( width - text2_width ), top+text1_height+f_line_gap );
        pango_cairo_show_layout (cr, font_layout);

        /* restore pango context */
        pango_layout_set_width(font_layout, DRAW_CLASSIFIER_PANGO_UNLIMITED_WIDTH);

        /* underline instances */
        if ( 0 != ( display_flags & ( DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE ) ) )
        {
            cairo_move_to ( cr, left + 0.5*( width - text2_width ), top+text1_height+f_line_gap+text2_height );
            cairo_line_to ( cr, left + 0.5*( width + text2_width ), top+text1_height+f_line_gap+text2_height );
            cairo_stroke (cr);
        }
    }

    /* draw description text */
    if ( DATA_CLASSIFIER_TYPE_COMMENT == classifier_type )
    {
        int text3_width;
        int text3_height;
        pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
        pango_layout_set_text ( font_layout,
                                data_classifier_get_description_const( classifier ),
                                DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH
                              );
        pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);

        /* draw text */
        cairo_move_to ( cr, left + 0.5*( width - text3_width ), top+text1_height+f_line_gap+text2_height+f_line_gap );
        pango_cairo_show_layout (cr, font_layout);
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2023 Andreas Warnke
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
