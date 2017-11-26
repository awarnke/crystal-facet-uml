/* File: pencil_classifier_painter.inl; Copyright and License: see below */

#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <assert.h>

static inline void pencil_classifier_painter_private_get_shape_border_dimensions( const pencil_classifier_painter_t *this_,
                                                                                  data_classifier_type_t classifier_type,
                                                                                  const pencil_size_t *pencil_size,
                                                                                  double *out_top_border,
                                                                                  double *out_left_border,
                                                                                  double *out_bottom_border,
                                                                                  double *out_right_border )
{
    assert( NULL != pencil_size );
    assert( NULL != out_top_border );
    assert( NULL != out_left_border );
    assert( NULL != out_bottom_border );
    assert( NULL != out_right_border );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double double_gap = 2.0 * gap;  /* a line has the gap disance on both sides to the next object */

    switch ( classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            /* within a use case, space is limited: */
            double v_offset = pencil_size_get_standard_font_size( pencil_size );
            double h_offset = 1.5 * pencil_size_get_standard_font_size( pencil_size );

            *out_top_border = double_gap + v_offset;
            *out_left_border = double_gap + h_offset;
            *out_bottom_border = double_gap + v_offset;
            *out_right_border = double_gap + h_offset;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            /* the 3d border of a node shrinks the space */
            double offset_3d = double_gap;

            *out_top_border = double_gap + offset_3d;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap + offset_3d;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            /* the actor icon height is part of the shape border  */
            double actor_icon_height = 2.5 * pencil_size_get_larger_font_size( pencil_size );

            *out_top_border = double_gap + actor_icon_height;
            *out_left_border = gap;
            *out_bottom_border = gap;
            *out_right_border = gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

            *out_top_border = double_gap + top_ornament_height;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_FEATURE:
        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_UML_STATE:
        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        case DATA_CLASSIFIER_TYPE_UML_PART:
        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            /* standard size */
            *out_top_border = double_gap;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_painter_private_get_shape_border_dimensions()");
            *out_top_border = 0.0;
            *out_left_border = 0.0;
            *out_bottom_border = 0.0;
            *out_right_border = 0.0;
        }
        break;
    }
}

static inline void pencil_classifier_painter_private_get_stereotype_and_name_dimensions( const pencil_classifier_painter_t *this_,
                                                                                         data_visible_classifier_t *visible_classifier,
                                                                                         const pencil_size_t *pencil_size,
                                                                                         PangoLayout *font_layout,
                                                                                         double *out_text_height,
                                                                                         double *out_text_width )
{
    assert( NULL != visible_classifier );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_text_height );
    assert( NULL != out_text_width );

    if ( data_visible_classifier_is_valid( visible_classifier ) )
    {
        data_classifier_t *classifier;
        data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
        diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );
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
            /* prepare text */
            char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE+1];
            utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ))
            {
                utf8stringbuf_copy_str( name_buf, ":" );
            }
            else
            {
                utf8stringbuf_clear( name_buf );
            }
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

            /* determine text width and height */
            pango_layout_set_font_description (font_layout, pencil_size_get_larger_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( name_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

            /* for underscores under object instance names, add 2 * gap: */
            space_for_line = 2.0 * pencil_size_get_standard_object_border( pencil_size );
        }

        *out_text_height = text1_height + text2_height + space_for_line;
        *out_text_width = ( text1_width > text2_width ) ? text1_width : text2_width;
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in pencil_classifier_painter_private_get_stereotype_and_name_dimensions()");
        *out_text_height = 0.0;
        *out_text_width = 0.0;
    }
}


/*
Copyright 2016-2017 Andreas Warnke

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
