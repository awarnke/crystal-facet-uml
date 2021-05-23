/* File: draw_classifier_label.c; Copyright and License: see below */

#include "draw/draw_classifier_label.h"
#include "trace.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
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
    TRACE_BEGIN();
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
            if ( 0 != utf8string_get_length( data_classifier_get_stereotype_ptr( classifier ) ) )
            {
                /* prepare text */
                char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                utf8stringbuf_copy_str( stereotype_buf, DRAW_CLASSIFIER_LEFT_POINTING_GUILLEMENTS );
                utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
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
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

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
                                    data_classifier_get_description_ptr( classifier ),
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
        TSLOG_ERROR("invalid visible classifier in draw_classifier_label_get_stereotype_and_name_dimensions()");
        *out_text_width = 0.0;
        *out_text_height = 0.0;
    }
    TRACE_END();
}

void draw_classifier_label_draw_stereotype_and_name( const draw_classifier_label_t *this_,
                                                     const data_visible_classifier_t *visible_classifier,
                                                     const geometry_rectangle_t *label_box,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     cairo_t *cr )
{
    TRACE_BEGIN();
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
        if ( 0 != utf8string_get_length( data_classifier_get_stereotype_ptr( classifier ) ) )
        {
            /* prepare text */
            char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
            utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
            utf8stringbuf_copy_str( stereotype_buf, DRAW_CLASSIFIER_LEFT_POINTING_GUILLEMENTS );
            utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
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
        utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

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
                                data_classifier_get_description_ptr( classifier ),
                                DRAW_CLASSIFIER_PANGO_AUTO_DETECT_LENGTH
                              );
        pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);

        /* draw text */
        cairo_move_to ( cr, left + 0.5*( width - text3_width ), top+text1_height+f_line_gap+text2_height+f_line_gap );
        pango_cairo_show_layout (cr, font_layout);
    }

    TRACE_END();
}

void draw_classifier_label_draw_id( const draw_classifier_label_t *this_,
                                    const data_visible_classifier_t *visible_classifier,
                                    const geometry_rectangle_t *classifier_bounds,
                                    const pencil_size_t *pencil_size,
                                    PangoLayout *font_layout,
                                    cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != classifier_bounds );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    /* define names for input data: */
    const double left = geometry_rectangle_get_left ( classifier_bounds );
    const double top = geometry_rectangle_get_top ( classifier_bounds );
    const double width = geometry_rectangle_get_width ( classifier_bounds );
    const double height = geometry_rectangle_get_height ( classifier_bounds );
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );

    /* prepare text */
    const data_id_t the_id = data_classifier_get_data_id( classifier );

    char id_buf[DATA_ID_MAX_UTF8STRING_SIZE+5];
    utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
    utf8stringbuf_copy_str( id_str, "{id=" );
    data_id_to_utf8stringbuf( &the_id, id_str );
    utf8stringbuf_append_str( id_str, "}" );

    /* determine text dimension */
    int text4_width;
    int text4_height;
    pango_layout_set_font_description (font_layout, pencil_size_get_footnote_font_description(pencil_size) );
    pango_layout_set_text (font_layout, utf8stringbuf_get_string( id_str ), -1);
    pango_layout_get_pixel_size (font_layout, &text4_width, &text4_height);

    /* position the text */
    int x_gap = 0;
    int y_gap = 0;
    switch ( classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
        case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:
        case DATA_CLASSIFIER_TYPE_COMPONENT:
        case DATA_CLASSIFIER_TYPE_PART:
        case DATA_CLASSIFIER_TYPE_INTERFACE:
        case DATA_CLASSIFIER_TYPE_PACKAGE:
        case DATA_CLASSIFIER_TYPE_CLASS:
        case DATA_CLASSIFIER_TYPE_OBJECT:
        case DATA_CLASSIFIER_TYPE_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_COMMENT:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:
        {
            /* there is a border line */
            x_gap = 2*gap;
            y_gap = 2*gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_NODE:
        {
            /* there is a 3D border line */
            x_gap = 4*gap;
            y_gap = 2*gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
        case DATA_CLASSIFIER_TYPE_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_STATE:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        {
            /* there is a border line with a round corner */
            x_gap = 7*gap;
            y_gap = 2*gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTOR:
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            /* position the text right-bottom of the icon */
            x_gap = (width - pencil_size_get_classifier_symbol_height( pencil_size ))/2 - text4_width;
            y_gap = 0;
        }
        break;

        case DATA_CLASSIFIER_TYPE_USE_CASE:
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        {
            /* there is a chance that the bottom-right corner is empty */
            x_gap = 0;
            y_gap = 0;
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            /* the send signal has a 45 degree edge */
            x_gap = height/2;
            y_gap = 2*gap;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_composer_draw()");
        }
        break;
    }

    /* draw text */
    GdkRGBA grey_color;
    grey_color = pencil_size_get_gray_out_color( pencil_size );
    cairo_set_source_rgba( cr, grey_color.red, grey_color.green, grey_color.blue, grey_color.alpha );
    cairo_move_to ( cr, left + width - text4_width - x_gap, top + height - text4_height - y_gap );
    pango_cairo_show_layout (cr, font_layout);

    TRACE_END();
}


/*
Copyright 2016-2021 Andreas Warnke
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
