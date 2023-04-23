/* File: draw_stereotype_image.c; Copyright and License: see below */

#include "draw_svg_path_data.h"
#include "draw/draw_stereotype_image.h"
#include "pencil_layout_data.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const double DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT = 1.0;

u8_error_t draw_stereotype_image_draw ( const draw_stereotype_image_t *this_,
                                        const char *stereotype,
                                        const data_profile_part_t *profile,
                                        const GdkRGBA *default_color,
                                        u8_error_info_t *out_err_info,
                                        const geometry_rectangle_t *bounds,
                                        cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != stereotype );
    assert( NULL != profile );
    assert( NULL != out_err_info );
    assert( NULL != bounds );
    assert( NULL != cr );
    u8_error_t result = U8_ERROR_NONE;
    u8_error_info_init_void( out_err_info );

    const data_classifier_t *const optional_stereotype
        = data_profile_part_get_stereotype_by_name_const( profile, UTF8STRINGVIEW_STR(stereotype) );
    if ( optional_stereotype != NULL )
    {
        U8_TRACE_INFO_STR( "stereotype", stereotype );
        const char *const drawing_directives = data_classifier_get_description_const( optional_stereotype );
        geometry_rectangle_t io_view_rect;
        geometry_rectangle_init_empty( &io_view_rect );
        result |= draw_stereotype_image_private_parse_svg_xml( this_,
                                                               false,  /* draw */
                                                               drawing_directives,
                                                               &io_view_rect,
                                                               default_color,
                                                               out_err_info,
                                                               bounds,
                                                               cr
                                                             );
        if ( result == U8_ERROR_NONE )
        {
            /* make view rect quadratic */
            const double view_width = geometry_rectangle_get_width( &io_view_rect );
            const double view_height = geometry_rectangle_get_height( &io_view_rect );
            if ( view_width > view_height)
            {
                geometry_rectangle_set_top( &io_view_rect, geometry_rectangle_get_top( &io_view_rect ) - 0.5*(view_width-view_height) );
                geometry_rectangle_set_height( &io_view_rect, view_width );
            }
            else
            {
                geometry_rectangle_set_left( &io_view_rect, geometry_rectangle_get_left( &io_view_rect ) - 0.5*(view_height-view_width) );
                geometry_rectangle_set_width( &io_view_rect, view_height );
            }
            result |= draw_stereotype_image_private_parse_svg_xml( this_,
                                                                   true,  /* draw */
                                                                   drawing_directives,
                                                                   &io_view_rect,
                                                                   default_color,
                                                                   out_err_info,
                                                                   bounds,
                                                                   cr
                                                                 );
        }
    }
    else
    {
        result = U8_ERROR_NOT_FOUND;
    }

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
    /* draw the rectangle */
    {
        cairo_set_source_rgba( cr, 1.0, 0.5, 0.6, 0.5 );
        cairo_rectangle ( cr,
                          geometry_rectangle_get_left ( bounds ),
                          geometry_rectangle_get_top ( bounds ),
                          geometry_rectangle_get_width ( bounds ),
                          geometry_rectangle_get_height ( bounds )
                        );
        cairo_stroke (cr);
        cairo_set_source_rgba( cr, (*default_color).red, (*default_color).green, (*default_color).blue, (*default_color).alpha );
    }
#endif

    U8_TRACE_END_ERR(result);
    return result;
}

/*! \brief states of parsing svg, the xml parts */
enum draw_stereotype_image_xml_enum {
    DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH,  /*!< nothing passed yet */
    DRAW_STEREOTYPE_IMAGE_XML_TAG_STARTED,  /*!< XML tag start passed */
    DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG,  /*!< XML token path passed */
    DRAW_STEREOTYPE_IMAGE_XML_D_ATTR,  /*!< XML attribute d: name passed */
    DRAW_STEREOTYPE_IMAGE_XML_D_DEF,  /*!< XML attribute d: assignment passed */
    DRAW_STEREOTYPE_IMAGE_XML_FILL_ATTR,  /*!< XML attribute fill: name passed, see svg spec 13.2. Specifying paint */
    DRAW_STEREOTYPE_IMAGE_XML_FILL_DEF,  /*!< XML attribute fill: assignment passed */
    DRAW_STEREOTYPE_IMAGE_XML_FILL_VALUE,  /*!< XML attribute fill: single or double quotes passed */
    DRAW_STEREOTYPE_IMAGE_XML_STROKE_ATTR,  /*!< XML attribute stroke: name passed, see svg spec 13.2. Specifying paint */
    DRAW_STEREOTYPE_IMAGE_XML_STROKE_DEF,  /*!< XML attribute stroke: assignment passed */
    DRAW_STEREOTYPE_IMAGE_XML_STROKE_VALUE,  /*!< XML attribute stroke: single or double quotes passed */
    DRAW_STEREOTYPE_IMAGE_XML_INSIDE_SGLQ_VALUE,  /*!< single-quoted XML attribute-value of any other attribute-name */
    DRAW_STEREOTYPE_IMAGE_XML_INSIDE_DBLQ_VALUE,  /*!< double-quoted XML attribute-value of any other attribute-name */
};

u8_error_t draw_stereotype_image_private_parse_svg_xml ( const draw_stereotype_image_t *this_,
                                                         bool draw,
                                                         const char *drawing_directives,
                                                         geometry_rectangle_t *io_view_rect,
                                                         const GdkRGBA *default_color,
                                                         u8_error_info_t *out_err_info,
                                                         const geometry_rectangle_t *target_bounds,
                                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != drawing_directives );
    assert( NULL != io_view_rect );
    assert( NULL != out_err_info );
    assert( NULL != target_bounds );
    assert( ( ! draw ) || ( NULL != cr ) );
    u8_error_t result = U8_ERROR_NONE;

    /* states while parsing: */
    enum draw_stereotype_image_xml_enum parser_state = DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH;
    uint_fast16_t path_count = 0;
    GdkRGBA stroke_color = *default_color;
    GdkRGBA fill_color = { .red = 1.0, .green = 1.0, .blue = 1.0, .alpha = 0.0 };
    char xml_attr_value_buf[64]; /* max 4 floating point numbers and rgba(%,%,%,%) string around */
    utf8stringbuf_t xml_attr_value = UTF8STRINGBUF(xml_attr_value_buf);

    utf8stringviewtokenizer_t tok_iterator;
    utf8stringviewtokenizer_init( &tok_iterator, UTF8STRINGVIEW_STR( drawing_directives ), UTF8STRINGVIEWTOKENMODE_TEXT );
    while( utf8stringviewtokenizer_has_next( &tok_iterator ) && ( result == U8_ERROR_NONE ) )
    {
        const utf8stringview_t tok = utf8stringviewtokenizer_next( &tok_iterator );

        switch ( parser_state )
        {
            case DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH:
            {
                if ( utf8stringview_equals_str( tok, "<" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_TAG_STARTED;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_TAG_STARTED:
            {
                if ( utf8stringview_equals_str( tok, "path" ) )
                {
                    /* for each new path, reset the colors to defaults */
                    stroke_color = *default_color;
                    fill_color = (GdkRGBA) { .red = 1.0, .green = 1.0, .blue = 1.0, .alpha = 0.0 };
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
                else
                {
                    /* no error, accept anythig here: */
                    /* not a path tag, back to ouside state */
                    /* TODO accept a namespace for the path maybe? */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG:
            {
                if ( utf8stringview_equals_str( tok, "d" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_D_ATTR;
                }
                else if ( utf8stringview_equals_str( tok, "stroke" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_STROKE_ATTR;
                }
                else if ( utf8stringview_equals_str( tok, "fill" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_FILL_ATTR;
                }
                else if ( utf8stringview_equals_str( tok, "\'" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_SGLQ_VALUE;
                }
                else if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_DBLQ_VALUE;
                }
                else if ( utf8stringview_equals_str( tok, "/" ) )
                {
                    /* ignore */
                }
                else if ( utf8stringview_equals_str( tok, ">" ) )
                {
                    if ( draw )
                    {
                        assert( NULL != cr );
                        /* fill and stroke */
                        if ( fill_color.alpha > 0.01 )
                        {
                            if ( stroke_color.alpha > 0.01 )
                            {
                                cairo_set_source_rgba( cr, fill_color.red, fill_color.green, fill_color.blue, fill_color.alpha );
                                cairo_fill_preserve( cr );
                                cairo_set_source_rgba( cr, stroke_color.red, stroke_color.green, stroke_color.blue, stroke_color.alpha );
                                cairo_stroke( cr );
                            }
                            else
                            {
                                cairo_set_source_rgba( cr, fill_color.red, fill_color.green, fill_color.blue, fill_color.alpha );
                                cairo_fill( cr );
                            }
                        }
                        else
                        {
                            cairo_set_source_rgba( cr, stroke_color.red, stroke_color.green, stroke_color.blue, stroke_color.alpha );
                            cairo_stroke( cr );
                        }
                    }
                    /* end of path tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_D_ATTR:
            {
                if ( utf8stringview_equals_str( tok, "=" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_D_DEF;
                }
                else
                {
                    /* this is an error */
                    /* TODO the tokenizer may have split a token, e.g. d:pre-d-post */
                    result |= U8_ERROR_PARSER_STRUCTURE;
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_PARSER_STRUCTURE,
                                             utf8stringviewtokenizer_get_line( &tok_iterator )
                                           );

                    /* not a d attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_D_DEF:
            {
                if (( utf8stringview_equals_str( tok, "\"" ) )||( utf8stringview_equals_str( tok, "\'" ) ))
                {
                    /* process draw commands in sub statemachine */
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY );
                    draw_svg_path_data_t svg_path_data;
                    draw_svg_path_data_init( &svg_path_data );
                    result |= draw_svg_path_data_parse_drawing( &svg_path_data,
                                                                draw,
                                                                &tok_iterator,
                                                                io_view_rect,
                                                                out_err_info,
                                                                target_bounds,
                                                                cr
                                                              );
                    draw_svg_path_data_destroy( &svg_path_data );
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_TEXT );
                    path_count ++;
                }
                /* back to inside path state */
                parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_STROKE_ATTR:
            {
                if ( utf8stringview_equals_str( tok, "=" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_STROKE_DEF;
                }
                else
                {
                    /* this is an error */
                    /* TODO the tokenizer may have split a token, e.g. stroke:pre-stroke-post */
                    result |= U8_ERROR_PARSER_STRUCTURE;
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_PARSER_STRUCTURE,
                                             utf8stringviewtokenizer_get_line( &tok_iterator )
                                           );

                    /* not a stroke attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_STROKE_DEF:
            {
                if (( utf8stringview_equals_str( tok, "\"" ) )||( utf8stringview_equals_str( tok, "\'" ) ))
                {
                    /* end of the value of another, ignored tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_STROKE_VALUE;
                    utf8stringbuf_clear( xml_attr_value );
                }
                else
                {
                    /* this is an error */
                    result |= U8_ERROR_PARSER_STRUCTURE;
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_PARSER_STRUCTURE,
                                             utf8stringviewtokenizer_get_line( &tok_iterator )
                                           );

                    /* not a stroke attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_STROKE_VALUE:
            {
                if (( utf8stringview_equals_str( tok, "\"" ) )||( utf8stringview_equals_str( tok, "\'" ) ))
                {
                    /* parse the color */
                    if ( utf8stringbuf_equals_str( xml_attr_value, "none" ) )
                    {
                        stroke_color = (GdkRGBA) { .red = 1.0, .green = 1.0, .blue = 1.0, .alpha = 0.0 };
                    }
                    else
                    {
                        U8_TRACE_INFO_STR( "stroke:", utf8stringbuf_get_string( xml_attr_value ) );
                        const gboolean success = gdk_rgba_parse( &stroke_color, utf8stringbuf_get_string( xml_attr_value ) );
                        if ( ! success )
                        {
                            result |= U8_ERROR_PARSER_STRUCTURE;
                            u8_error_info_init_line( out_err_info,
                                                     U8_ERROR_PARSER_STRUCTURE,
                                                     utf8stringviewtokenizer_get_line( &tok_iterator )
                                                   );
                        }
                    }
                    /* end of the value of fill tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
                else
                {
                    utf8stringbuf_append_view( xml_attr_value, tok );
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_FILL_ATTR:
            {
                if ( utf8stringview_equals_str( tok, "=" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_FILL_DEF;
                }
                else
                {
                    /* this is an error */
                    /* TODO the tokenizer may have split a token, e.g. fill:pre-fill-post */
                    result |= U8_ERROR_PARSER_STRUCTURE;
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_PARSER_STRUCTURE,
                                             utf8stringviewtokenizer_get_line( &tok_iterator )
                                           );

                    /* not a fill attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_FILL_DEF:
            {
                if (( utf8stringview_equals_str( tok, "\"" ) )||( utf8stringview_equals_str( tok, "\'" ) ))
                {
                    /* end of the value of another, ignored tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_FILL_VALUE;
                    utf8stringbuf_clear( xml_attr_value );
                }
                else
                {
                    /* this is an error */
                    result |= U8_ERROR_PARSER_STRUCTURE;
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_PARSER_STRUCTURE,
                                             utf8stringviewtokenizer_get_line( &tok_iterator )
                                           );

                    /* not a stroke attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_FILL_VALUE:
            {
                if (( utf8stringview_equals_str( tok, "\"" ) )||( utf8stringview_equals_str( tok, "\'" ) ))
                {
                    /* parse the color */
                    if ( utf8stringbuf_equals_str( xml_attr_value, "none" ) )
                    {
                        fill_color = (GdkRGBA) { .red = 1.0, .green = 1.0, .blue = 1.0, .alpha = 0.0 };
                    }
                    else
                    {
                        U8_TRACE_INFO_STR( "fill:", utf8stringbuf_get_string( xml_attr_value ) );
                        const gboolean success = gdk_rgba_parse( &fill_color, utf8stringbuf_get_string( xml_attr_value ) );
                        if ( ! success )
                        {
                            result |= U8_ERROR_PARSER_STRUCTURE;
                            u8_error_info_init_line( out_err_info,
                                                     U8_ERROR_PARSER_STRUCTURE,
                                                     utf8stringviewtokenizer_get_line( &tok_iterator )
                                                   );
                        }
                    }
                    /* end of the value of fill tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
                else
                {
                    utf8stringbuf_append_view( xml_attr_value, tok );
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_INSIDE_SGLQ_VALUE:
            {
                if ( utf8stringview_equals_str( tok, "\'" ) )
                {
                    /* end of the value of another, ignored tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
                /*
                else
                {
                    U8_TRACE_INFO_VIEW( "other single-quoted:", tok );
                }
                */
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_INSIDE_DBLQ_VALUE:
            {
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    /* end of the value of another, ignored tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
                /*
                else
                {
                    U8_TRACE_INFO_VIEW( "other double-quoted:", tok );
                }
                */
            }
            break;
        }
    }

    /* report error on unfinished drawing */
    if ( parser_state != DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH )
    {
        result |= U8_ERROR_PARSER_STRUCTURE;
        /* if no other error encountered yet, report this one: */
        if ( ! u8_error_info_is_error( out_err_info ) )
        {
            u8_error_info_init_line( out_err_info,
                                     U8_ERROR_PARSER_STRUCTURE,
                                     utf8stringviewtokenizer_get_line( &tok_iterator )
                                   );
        }
    }

    /* check if anything was drawn at all */
    if ( path_count == 0 )
    {
        result |= U8_ERROR_NOT_FOUND;
    }

    utf8stringviewtokenizer_destroy( &tok_iterator );
    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2023-2023 Andreas Warnke
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
