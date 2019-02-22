/* File: draw_label.c; Copyright and License: see below */

#include "draw/draw_label.h"
#include "trace.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void draw_label_init( draw_label_t *this_ )
{
    TRACE_BEGIN();

    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void draw_label_destroy( draw_label_t *this_ )
{
    TRACE_BEGIN();

    data_rules_destroy ( &((*this_).data_rules) );

    TRACE_END();
}

void draw_label_get_stereotype_and_name_dimensions( const draw_label_t *this_,
                                                    const data_visible_classifier_t *visible_classifier,
                                                    const pencil_size_t *pencil_size,
                                                    PangoLayout *font_layout,
                                                    double *out_text_height,
                                                    double *out_text_width )
{
    TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_text_height );
    assert( NULL != out_text_width );

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
                utf8stringbuf_copy_str( stereotype_buf, "<<" );
                utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
                utf8stringbuf_append_str( stereotype_buf, ">>" );

                /* determine text width and height */
                pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
                pango_layout_set_text (font_layout, utf8stringbuf_get_string( stereotype_buf ), -1);
                pango_layout_get_pixel_size (font_layout, &text1_width, &text1_height);
            }
        }

        /* draw name text */
        int text2_width;
        int text2_height;
        double space_for_line;
        {
            bool is_always_instance;
            bool is_anonymous_instance;
            is_always_instance = data_rules_is_always_instance( &((*this_).data_rules), data_classifier_get_main_type ( classifier ) );
            is_anonymous_instance = ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ));

            /* prepare text */
            char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE + 1 ];
            utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
            if ( is_anonymous_instance && ( ! is_always_instance ) )
            {
                utf8stringbuf_copy_str( name_buf, ":" );
            }
            else
            {
                utf8stringbuf_clear( name_buf );
            }
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

            /* determine text width and height */
            pango_layout_set_font_description (font_layout, pencil_size_get_title_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( name_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

            /* for space between stereotype and name */
            text2_height += pencil_size_get_font_line_gap( pencil_size );

            /* for underscores under object instance names, add 2 * gap: */
            space_for_line = 2.0 * pencil_size_get_standard_object_border( pencil_size );
        }

        /* draw description text */
        int text3_width = 0;
        int text3_height = 0;
        if (( DATA_CLASSIFIER_TYPE_UML_COMMENT == data_classifier_get_main_type ( classifier ) )
            || ( DATA_CLASSIFIER_TYPE_REQUIREMENT == data_classifier_get_main_type ( classifier ) ))
        {
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (font_layout, data_classifier_get_description_ptr( classifier ), -1);
            pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);
        }

        *out_text_height = text1_height + text2_height + space_for_line + text3_height;
        double intermediate_max_w;
        intermediate_max_w = ( text1_width > text2_width ) ? text1_width : text2_width;
        *out_text_width = ( intermediate_max_w > text3_width ) ? intermediate_max_w : text3_width;
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in draw_label_get_stereotype_and_name_dimensions()");
        *out_text_height = 0.0;
        *out_text_width = 0.0;
    }
    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke
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
