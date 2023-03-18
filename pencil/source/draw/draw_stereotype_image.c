/* File: draw_stereotype_image.c; Copyright and License: see below */

#include "draw/draw_stereotype_image.h"
#include "pencil_layout_data.h"
#include "utf8stringbuf/utf8stringviewtokenizer.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const double DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT = 1.0;

u8_error_t draw_stereotype_image_draw ( const draw_stereotype_image_t *this_,
                                        const char *stereotype,
                                        const data_profile_part_t *profile,
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

    /* determine linewith to avoid that drawings overlap to the outside of bounds */
    const double ln_w = cairo_get_line_width( cr );

    /* calculate artifact bounds */
    const double art_left = geometry_rectangle_get_left( bounds );
    const double art_right = geometry_rectangle_get_right( bounds );
    const double art_top = geometry_rectangle_get_top( bounds );
    const double art_bottom = geometry_rectangle_get_bottom( bounds );
    const double art_height = geometry_rectangle_get_height( bounds );
    //const double art_width = geometry_rectangle_get_width( bounds );
    const double art_corner_edge = art_height * 0.6;

    /* draw the icon */
    cairo_set_source_rgba( cr, 0.7, 0.7, 0.7, 1.0 );
    cairo_rectangle ( cr,
                      geometry_rectangle_get_left ( bounds ),
                      geometry_rectangle_get_top ( bounds ),
                      geometry_rectangle_get_width ( bounds ),
                      geometry_rectangle_get_height ( bounds )
                    );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.8, 0.5, 0.0, 1.0 );
    cairo_move_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_line_to ( cr, art_left, art_top );
    cairo_line_to ( cr, art_left, art_bottom );
    cairo_line_to ( cr, art_right, art_bottom );
    cairo_line_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_stroke (cr);

    const data_classifier_t *const optional_stereotype
        = data_profile_part_get_stereotype_by_name_const( profile, UTF8STRINGVIEW_STR(stereotype) );
    if ( optional_stereotype != NULL )
    {
        const char *const drawing_directives = data_classifier_get_description_const( optional_stereotype );
        geometry_rectangle_t io_view_rect;
        geometry_rectangle_init_empty( &io_view_rect );
        result |= draw_stereotype_image_private_parse_drawing( this_,
                                                               true,  /* draw */
                                                               drawing_directives,
                                                               &io_view_rect,
                                                               out_err_info,
                                                               bounds,
                                                               cr
                                                             );
        result |= draw_stereotype_image_private_parse_drawing( this_,
                                                               true,  /* draw */
                                                               drawing_directives,
                                                               &io_view_rect,
                                                               out_err_info,
                                                               bounds,
                                                               cr
                                                             );
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
        cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
    }
#endif

    U8_TRACE_END_ERR(result);
    return result;
}

/*! \brief states of parsing drawing directives */
enum draw_stereotype_image_dd_enum {
    DRAW_STEREOTYPE_IMAGE_DD_OUTSIDE_PATH,  /*!< nothing passed yet */
    DRAW_STEREOTYPE_IMAGE_DD_XML_TAG_STARTED,  /*!< XML tag start passed */
    DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG,  /*!< XML token path passed */
    DRAW_STEREOTYPE_IMAGE_DD_XML_D_ATTR,  /*!< XML attribute d passed */
    DRAW_STEREOTYPE_IMAGE_DD_XML_D_DEF,  /*!< XML tag path passed */
    DRAW_STEREOTYPE_IMAGE_DD_COMMAND,  /*!< within d quoted value, expecting drawing directive */
    DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ,  /*!< within d quoted value, expecting drawing directive or continuation of coordinate sequence */
    DRAW_STEREOTYPE_IMAGE_DD_COORD_0,  /*!< within d quoted value, coordinate sequence, expecting 1st float */
    DRAW_STEREOTYPE_IMAGE_DD_COORD_1,  /*!< within d quoted value, coordinate sequence, expecting 2nd float */
    DRAW_STEREOTYPE_IMAGE_DD_COORD_2,  /*!< within d quoted value, coordinate sequence, expecting 3rd float */
    DRAW_STEREOTYPE_IMAGE_DD_COORD_3,  /*!< within d quoted value, coordinate sequence, expecting 4th float */
    DRAW_STEREOTYPE_IMAGE_DD_COORD_4,  /*!< within d quoted value, coordinate sequence, expecting 3rd float */
    DRAW_STEREOTYPE_IMAGE_DD_COORD_5,  /*!< within d quoted value, coordinate sequence, expecting 4th float */
};

u8_error_t draw_stereotype_image_private_parse_drawing ( const draw_stereotype_image_t *this_,
                                                         bool draw,
                                                         const char *drawing_directives,
                                                         geometry_rectangle_t *io_view_rect,
                                                         u8_error_info_t *out_err_info,
                                                         const geometry_rectangle_t *target_bounds,
                                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != drawing_directives );
    assert( NULL != io_view_rect );
    assert( NULL != out_err_info );
    assert( NULL != target_bounds );
    assert( NULL != cr );
    u8_error_t result = U8_ERROR_NOT_FOUND;

    /* copy the initial input view rect*/
    geometry_rectangle_t view_rect = *io_view_rect;

    /* states while parsing: */
    enum draw_stereotype_image_dd_enum parser_state = DRAW_STEREOTYPE_IMAGE_DD_OUTSIDE_PATH;
    char last_command = ' ';
    double coord_1_x = 0.0;  /* abscissa */
    double coord_1_y = 0.0;  /* oridnate */
    double coord_2_x = 0.0;
    double coord_2_y = 0.0;
    double coord_3_x = 0.0;
    double coord_3_y = 0.0;
    double subpath_start_x = 0.0;
    double subpath_start_y = 0.0;
    double command_start_x = 0.0;
    double command_start_y = 0.0;
    utf8stringviewtokenizer_t tok_iterator;
    utf8stringviewtokenizer_init( &tok_iterator, UTF8STRINGVIEW_STR( drawing_directives ), UTF8STRINGVIEWTOKENMODE_TEXT );

    while( utf8stringviewtokenizer_has_next( &tok_iterator ) )
    {
        const utf8stringview_t tok = utf8stringviewtokenizer_next( &tok_iterator );
        assert( utf8stringview_get_length( tok ) > 0 );  /* otherwise this would not be a token */

        switch ( parser_state )
        {
            case DRAW_STEREOTYPE_IMAGE_DD_OUTSIDE_PATH:
            {
                if ( utf8stringview_equals_str( tok, "<" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_XML_TAG_STARTED;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_XML_TAG_STARTED:
            {
                if ( utf8stringview_equals_str( tok, "path" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG;
                }
                else
                {
                    /* not a path tag, back to ouside state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_OUTSIDE_PATH;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG:
            {
                if ( utf8stringview_equals_str( tok, "d" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_XML_D_ATTR;
                }
                else if ( utf8stringview_equals_str( tok, ">" ) )
                {
                    /* end of path tag */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_OUTSIDE_PATH;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_XML_D_ATTR:
            {
                if ( utf8stringview_equals_str( tok, "=" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_XML_D_DEF;
                }
                else
                {
                    /* not a d attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_XML_D_DEF:
            {
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND;
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_FLOAT );
                }
                else
                {
                    /* not a d attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COMMAND:
            {
                const char current = *utf8stringview_get_start( tok );
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    /* no subpath here to draw */
                    /* end of d attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG;
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_TEXT );
                }
                else if ( current=='z' || current=='Z' )
                {
                    /* no line_to here because there is no subpath to end */
                    cairo_move_to ( cr, subpath_start_x, subpath_start_y );
                    /* end of subpath */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND;
                    last_command = current;
                }
                else if ( current=='m' || current=='M' || current=='l' || current=='L' )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_0;
                    last_command = current;
                }
                else if ( current=='c' || current=='C' )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_0;
                    last_command = current;
                }
                else
                {
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_NOT_YET_IMPLEMENTED,
                                             utf8stringviewtokenizer_get_line( &tok_iterator )
                                           );
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ:
            {
                const char current = *utf8stringview_get_start( tok );
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    /* draw subpath */
                    cairo_stroke (cr);
                    /* end of d attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_INSIDE_PATH_TAG;
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_TEXT );
                }
                else if ( current=='z' || current=='Z' )
                {
                    /* draw subpath */
                    cairo_line_to ( cr, subpath_start_x, subpath_start_y );
                    cairo_stroke (cr);
                    command_start_x = subpath_start_x;
                    command_start_y = subpath_start_y;
                    /* end of subpath */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND;
                    last_command = current;
                }
                else if ( current=='m' || current=='M' || current=='l' || current=='L' )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_0;
                    last_command = current;
                }
                else if ( current=='c' || current=='C' )
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_0;
                    last_command = current;
                }
                else if ( current==',' )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_1_x );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }

                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_1;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COORD_0:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_1_x );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_1;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COORD_1:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_1_y );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* do command */
                    if ( last_command=='m' )
                    {
                        /* draw */
                        cairo_move_to ( cr, command_start_x + coord_1_x, command_start_y + coord_1_y );
                        /* update state */
                        subpath_start_x = command_start_x + coord_1_x;
                        subpath_start_y = command_start_y + coord_1_y;
                        command_start_x = subpath_start_x;
                        command_start_y = subpath_start_y;
                        last_command = 'l';  /* following coordinates are implicitely line-to commands */
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ;
                    }
                    else if ( last_command=='M' )
                    {
                        /* draw */
                        cairo_move_to ( cr, coord_1_x, coord_1_y );
                        /* update state */
                        subpath_start_x = coord_1_x;
                        subpath_start_y = coord_1_y;
                        command_start_x = subpath_start_x;
                        command_start_y = subpath_start_y;
                        last_command = 'L';  /* following coordinates are implicitely line-to commands */
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ;
                    }
                    else if ( last_command=='l' )
                    {
                        /* draw */
                        cairo_line_to ( cr, command_start_x + coord_1_x, command_start_y + coord_1_y );
                        /* update state */
                        command_start_x = command_start_x + coord_1_x;
                        command_start_y = command_start_y + coord_1_y;
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ;
                    }
                    else if ( last_command=='L' )
                    {
                        /* draw */
                        cairo_line_to ( cr, coord_1_x, coord_1_y );
                        /* update state */
                        command_start_x = coord_1_x;
                        command_start_y = coord_1_y;
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ;
                    }
                    else
                    {
                        parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_2;
                    }
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COORD_2:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_2_x );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_3;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COORD_3:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_2_y );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_4;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COORD_4:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_3_x );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COORD_5;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_DD_COORD_5:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &coord_3_y );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( &tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* do commands */
                    if ( last_command=='c' )
                    {
                        /* draw */
                        cairo_curve_to( cr,
                                        command_start_x + coord_1_x,
                                        command_start_y + coord_1_y,
                                        command_start_x + coord_2_x,
                                        command_start_y + coord_2_y,
                                        command_start_x + coord_3_x,
                                        command_start_y + coord_3_y
                                      );
                        /* update state */
                        command_start_x = command_start_x + coord_3_x;
                        command_start_y = command_start_y + coord_3_y;
                    }
                    else if ( last_command=='C' )
                    {
                        /* draw */
                        cairo_line_to( cr, coord_3_x, coord_3_y );
                        /* update state */
                        command_start_x = coord_3_x;
                        command_start_y = coord_3_y;
                    }
                    /* continue with next */
                    parser_state = DRAW_STEREOTYPE_IMAGE_DD_COMMAND_OR_COORD_SEQ;
                }
            }
            break;
        }
    }
    utf8stringviewtokenizer_destroy( &tok_iterator );

    /* clean up unfinished drawings */
    if ( parser_state != DRAW_STEREOTYPE_IMAGE_DD_OUTSIDE_PATH )
    {
        cairo_stroke (cr);
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
