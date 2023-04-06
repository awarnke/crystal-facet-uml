/* File: draw_stereotype_image.c; Copyright and License: see below */

#include "draw/draw_stereotype_image.h"
#include "pencil_layout_data.h"
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

    const data_classifier_t *const optional_stereotype
        = data_profile_part_get_stereotype_by_name_const( profile, UTF8STRINGVIEW_STR(stereotype) );
    if ( optional_stereotype != NULL )
    {
        U8_TRACE_INFO_STR( "stereotype", stereotype );
        const char *const drawing_directives = data_classifier_get_description_const( optional_stereotype );
        geometry_rectangle_t io_view_rect;
        geometry_rectangle_init_empty( &io_view_rect );
        /* TODO: set colors and linewidths */
        /* cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 ); */
        /* const double ln_w = cairo_get_line_width( cr ); */
        result |= draw_stereotype_image_private_parse_svg_xml( this_,
                                                               false,  /* draw */
                                                               drawing_directives,
                                                               &io_view_rect,
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
        cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
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
    DRAW_STEREOTYPE_IMAGE_XML_D_ATTR,  /*!< XML attribute d passed */
    DRAW_STEREOTYPE_IMAGE_XML_D_DEF,  /*!< XML tag path passed */
};

u8_error_t draw_stereotype_image_private_parse_svg_xml ( const draw_stereotype_image_t *this_,
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
    u8_error_t result = U8_ERROR_NONE;

    /* states while parsing: */
    enum draw_stereotype_image_xml_enum parser_state = DRAW_STEREOTYPE_IMAGE_XML_OUTSIDE_PATH;
    uint_fast16_t path_count = 0;

    utf8stringviewtokenizer_t tok_iterator;
    utf8stringviewtokenizer_init( &tok_iterator, UTF8STRINGVIEW_STR( drawing_directives ), UTF8STRINGVIEWTOKENMODE_TEXT );
    while( utf8stringviewtokenizer_has_next( &tok_iterator ) )
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
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
                else
                {
                    /* not a path tag, back to ouside state */
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
                else if ( utf8stringview_equals_str( tok, ">" ) )
                {
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
                    /* not a d attribute, back to inside path state */
                    parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_XML_D_DEF:
            {
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    /* process draw commands in sub statemachine */
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY );
                    result |= draw_stereotype_image_private_parse_drawing( this_,
                                                                           draw,
                                                                           &tok_iterator,
                                                                           io_view_rect,
                                                                           out_err_info,
                                                                           target_bounds,
                                                                           cr
                                                                         );
                    utf8stringviewtokenizer_set_mode( &tok_iterator, UTF8STRINGVIEWTOKENMODE_TEXT );
                    path_count ++;
                }
                /* back to inside path state */
                parser_state = DRAW_STEREOTYPE_IMAGE_XML_INSIDE_PATH_TAG;
            }
            break;

        }
    }
    utf8stringviewtokenizer_destroy( &tok_iterator );

    /* check if anything was drawn at all */
    if ( path_count == 0 )
    {
        result |= U8_ERROR_NOT_FOUND;
    }

    U8_TRACE_END_ERR(result);
    return result;
}

/*! \brief states of parsing drawing directives */
enum draw_stereotype_image_expect_enum {
    DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND,  /*!< expecting initial drawing directive */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND_OR_COORD_SEQ,  /*!< expecting drawing directive or continuation of coordinate sequence */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_X,  /*!< coordinate sequence, expecting cubic curve control_point1_X float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_Y,  /*!< coordinate sequence, expecting cubic curve control_point1_Y float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_X,  /*!< coordinate sequence, expecting cubic curve control_point2_X float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_Y,  /*!< coordinate sequence, expecting cubic curve control_point2_Y float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_X,  /*!< coordinate sequence, expecting quadratic curve control_point_X float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_Y,  /*!< coordinate sequence, expecting quadratic curve control_point_Y float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RX,  /*!< arc parameter sequence, expecting arc rx float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RY,  /*!< arc parameter sequence, expecting arc rx float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_PHI,  /*!< arc parameter sequence, expecting arc x axis rotation-angle float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_LARGE,  /*!< arc parameter sequence, expecting large arc flag */
    DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_SWEEP,  /*!< arc parameter sequence, expecting sweep arc flag */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X,  /*!< coordinate sequence, expecting end_x float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y,  /*!< coordinate sequence, expecting end_y float */
    DRAW_STEREOTYPE_IMAGE_EXPECT_EXIT,  /*!< double quotes end the processing of path drawing commands */
};

u8_error_t draw_stereotype_image_private_parse_drawing ( const draw_stereotype_image_t *this_,
                                                         bool draw,
                                                         utf8stringviewtokenizer_t *tok_iterator,
                                                         geometry_rectangle_t *io_view_rect,
                                                         u8_error_info_t *out_err_info,
                                                         const geometry_rectangle_t *target_bounds,
                                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != tok_iterator );
    assert( NULL != io_view_rect );
    assert( NULL != out_err_info );
    assert( NULL != target_bounds );
    assert( NULL != cr );
    u8_error_t result = U8_ERROR_NONE;

    /* calculate scale and shift to convert view rect to target bounds */
    const double view_width = geometry_rectangle_get_width( io_view_rect ) < 0.0001 ? 1.0 : geometry_rectangle_get_width( io_view_rect );
    const double scale_x = geometry_rectangle_get_width( target_bounds ) / view_width;
    const double shift_x = geometry_rectangle_get_left( target_bounds ) - ( scale_x * geometry_rectangle_get_left( io_view_rect ) );
    const double view_height = geometry_rectangle_get_height( io_view_rect ) < 0.0001 ? 1.0 : geometry_rectangle_get_height( io_view_rect );
    const double scale_y = geometry_rectangle_get_height( target_bounds ) / view_height;
    const double shift_y = geometry_rectangle_get_top( target_bounds ) - ( scale_y * geometry_rectangle_get_top( io_view_rect ) );

    /* states while parsing: */
    enum draw_stereotype_image_expect_enum parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND;
    char last_command = ' ';
    double command_end_x = 0.0;  /* abscissa, absolute values */
    double command_end_y = 0.0;  /* ordinate, absolute values */
    double coord_ctrl1_x = 0.0;  /* abscissa, absolute values */
    double coord_ctrl1_y = 0.0;  /* ordinate, absolute values */
    double coord_ctrl2_x = 0.0;  /* abscissa, absolute values */
    double coord_ctrl2_y = 0.0;  /* ordinate, absolute values */
    double subpath_start_x = 0.0;  /* abscissa, absolute values */
    double subpath_start_y = 0.0;  /* ordinate, absolute values */
    double command_start_x = 0.0;  /* abscissa, absolute values */
    double command_start_y = 0.0;  /* ordinate, absolute values */
    double arc_r_x = 0.0;  /* major ellipsis radius */
    double arc_r_y = 0.0;  /* minor ellipsis radius */
    double arc_phi = 0.0;  /* angle between major ellipsis radius and x-axis (unit: rad) */
    bool arc_large_arc = false;  /* true if the arc is spanning more than 180 degree / 1*pi */
    bool arc_sweep_positive = false;  /* true if the arc is traversed in positive-angle direction */

    /* init draw */
    if ( draw )
    {
        cairo_move_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
    }

    while( utf8stringviewtokenizer_has_next( tok_iterator ) && ( parser_state != DRAW_STEREOTYPE_IMAGE_EXPECT_EXIT ) )
    {
        const utf8stringview_t tok = utf8stringviewtokenizer_next( tok_iterator );
        assert( utf8stringview_get_length( tok ) > 0 );  /* otherwise this would not be a token */
#ifndef NDEBUG
        char debug_token_buf[16];
        utf8stringbuf_t debug_token = UTF8STRINGBUF(debug_token_buf);
        utf8stringbuf_copy_str( debug_token, utf8stringview_get_start( tok ) );
        U8_TRACE_INFO_STR( "token:", utf8stringbuf_get_string( debug_token ) );
#endif

        switch ( parser_state )
        {
            case DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND:
            {
                const char current = *utf8stringview_get_start( tok );
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    /* no subpath here to draw */
                    /* end of d attribute, back to caller */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_EXIT;
                }
                else if (( current=='z' )||( current=='Z' ))
                {
                    /* no line_to here because there is no subpath to end */
                    if ( draw )
                    {
                        cairo_move_to ( cr, subpath_start_x, subpath_start_y );
                    }
                    /* end of subpath */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND;
                    last_command = current;
                }
                else if (( current=='m' )||( current=='M' )||( current=='l' )||( current=='L' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='h' )||( current=='H' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='v' )||( current=='V' ))
                {
                    command_end_x = command_start_x;
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y;
                    last_command = current;
                }
                else if (( current=='c' )||( current=='C' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_X;
                    last_command = current;
                }
                else if (( current=='s' )||( current=='S' ))
                {
                    /* there was no preceding c/C/s/S command, initialize first control point to command start point: */
                    coord_ctrl1_x = command_start_x;
                    coord_ctrl1_y = command_start_y;
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_X;
                    last_command = current;
                }
                else if (( current=='q' )||( current=='Q' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_X;
                    last_command = current;
                }
                else if (( current=='t' )||( current=='T' ))
                {
                    /* there was no preceding q/Q/t/T command, initialize next control point to command start point: */
                    coord_ctrl1_x = command_start_x;
                    coord_ctrl1_y = command_start_y;
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='a' )||( current=='A' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RX;
                    last_command = current;
                }
                else
                {
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_NOT_YET_IMPLEMENTED,
                                             utf8stringviewtokenizer_get_line( tok_iterator )
                                           );
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND_OR_COORD_SEQ:
            {
                const char current = *utf8stringview_get_start( tok );
                if ( utf8stringview_equals_str( tok, "\"" ) )
                {
                    /* draw subpath */
                    if ( draw )
                    {
                        cairo_stroke (cr);
                    }
                    /* end of d attribute, back to caller */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_EXIT;
                }
                else if ( current=='z' || current=='Z' )
                {
                    /* draw subpath */
                    if ( draw )
                    {
                        cairo_line_to ( cr, subpath_start_x * scale_x + shift_x, subpath_start_y * scale_y + shift_y );
                        cairo_stroke (cr);
                    }
                    command_start_x = subpath_start_x;
                    command_start_y = subpath_start_y;
                    /* end of subpath */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND;
                    last_command = current;
                }
                else if (( current=='m' )||( current=='M' )||( current=='l' )||( current=='L' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='h' )||( current=='H' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='v' )||( current=='V' ))
                {
                    command_end_x = command_start_x;
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y;
                    last_command = current;
                }
                else if (( current=='c' )||( current=='C' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_X;
                    last_command = current;
                }
                else if (( current=='s' )||( current=='S' ))
                {
                    if (( last_command == 'c' )||( last_command == 'C' )||( last_command == 's' )||( last_command == 'S' ))
                    {
                        /* init the first control point as mirror of the last control point: */
                        coord_ctrl1_x = command_start_x + (command_start_x-coord_ctrl2_x);
                        coord_ctrl1_y = command_start_y + (command_start_y-coord_ctrl2_y);
                    }
                    else
                    {
                        coord_ctrl1_x = command_start_x;
                        coord_ctrl1_y = command_start_y;
                    }
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_X;
                    last_command = current;
                }
                else if (( current=='q' )||( current=='Q' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_X;
                    last_command = current;
                }
                else if (( current=='t' )||( current=='T' ))
                {
                    if (( last_command == 'q' )||( last_command == 'Q' )||( last_command == 't' )||( last_command == 'T' ))
                    {
                        /* init the next control point as mirror of the last control point: */
                        coord_ctrl1_x = command_start_x + (command_start_x-coord_ctrl1_x);
                        coord_ctrl1_y = command_start_y + (command_start_y-coord_ctrl1_y);
                    }
                    else
                    {
                        coord_ctrl1_x = command_start_x;
                        coord_ctrl1_y = command_start_y;
                    }
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='a' )||( current=='A' ))
                {
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RX;
                    last_command = current;
                }
                else if ( current==',' )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        /* U8_TRACE_INFO_INT( "line", utf8stringviewtokenizer_get_line( tok_iterator ) ); */
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    const double value_x_abs = is_absolute ? value : ( command_start_x + value );
                    /* do command */
                    if (( last_command=='v' )||( last_command=='V' ))
                    {
                        command_end_x = command_start_x;
                        command_end_y = is_absolute ? value : ( command_start_y + value );
                        /* draw */
                        if ( draw )
                        {
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_y = command_end_y;
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND_OR_COORD_SEQ;
                    }
                    else if (( last_command=='h' )||( last_command=='H' ))
                    {
                        command_end_x = value_x_abs;
                        command_end_y = command_start_y;
                        /* draw */
                        if ( draw )
                        {
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND_OR_COORD_SEQ;
                    }
                    else if (( last_command=='c' )||( last_command=='C' ))
                    {
                        coord_ctrl1_x = value_x_abs;
                        /* reading last_command parameters */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_Y;
                    }
                    else if (( last_command=='s' )||( last_command=='S' ))
                    {
                        /* init the first control point as mirror of the last control point: */
                        {
                            coord_ctrl1_x = command_start_x + (command_start_x-coord_ctrl2_x);
                            coord_ctrl1_y = command_start_y + (command_start_y-coord_ctrl2_y);
                        }
                        coord_ctrl2_x = value_x_abs;
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_Y;
                    }
                    else if (( last_command=='q' )||( last_command=='Q' ))
                    {
                        coord_ctrl1_x = value_x_abs;
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_Y;
                    }
                    else if (( last_command=='t' )||( last_command=='T' ))
                    {
                        /* init the next control point as mirror of the last control point: */
                        {
                            coord_ctrl1_x = command_start_x + (command_start_x-coord_ctrl1_x);
                            coord_ctrl1_y = command_start_y + (command_start_y-coord_ctrl1_y);
                        }
                        command_end_x = value_x_abs;
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y;
                    }
                    else if (( last_command=='a' )||( last_command=='A' ))
                    {
                        /* store major radius value to appropriate variable */
                        arc_r_x = value;
                        /* continue reading last_command parameters */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RY;
                    }
                    else
                    {
                        command_end_x = value_x_abs;
                        /* continue reading last_command parameters */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y;
                    }
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    command_end_x = is_absolute ? value : ( command_start_x + value );
                    /* do command */
                    if (( last_command=='h' )||( last_command=='H' ))
                    {
                        command_end_y = command_start_y;
                        /* draw */
                        if ( draw )
                        {
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        /* continue with next */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND_OR_COORD_SEQ;
                    }
                    else
                    {
                        /* continue reading more parameters for last_command */
                        parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y;
                    }
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_Y:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    command_end_y = is_absolute ? value : ( command_start_y + value );
                    /* do command */
                    if (( last_command=='m' )||( last_command=='M' ))
                    {
                        /* draw */
                        if ( draw )
                        {
                            cairo_move_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        subpath_start_x = command_end_x;
                        subpath_start_y = command_end_y;
                        command_start_x = command_end_x;
                        command_start_y = command_end_y;
                        last_command = is_absolute ? 'L' : 'l';  /* following coordinates are implicitely line-to commands */
                        /* init or update the io_view_rect parameter */
                        if ( geometry_rectangle_is_point( io_view_rect ) )
                        {
                            geometry_rectangle_init( io_view_rect, subpath_start_x, subpath_start_y, 0.0, 0.0 );
                        }
                        else
                        {
                            geometry_rectangle_embrace( io_view_rect, subpath_start_x, subpath_start_y );
                        }
                    }
                    else if (( last_command=='l' )||( last_command=='L' )||( last_command=='v' )||( last_command=='V' ))
                    {
                        /* draw */
                        if ( draw )
                        {
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        command_start_y = command_end_y;
                    }
                    else if (( last_command=='c' )||( last_command=='C' )||( last_command=='s' )||( last_command=='S' ))
                    {
                        /* draw */
                        if ( draw )
                        {
                            cairo_curve_to( cr,
                                            coord_ctrl1_x * scale_x + shift_x,
                                            coord_ctrl1_y * scale_y + shift_y,
                                            coord_ctrl2_x * scale_x + shift_x,
                                            coord_ctrl2_y * scale_y + shift_y,
                                            command_end_x * scale_x + shift_x,
                                            command_end_y * scale_y + shift_y
                                          );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, coord_ctrl1_x, coord_ctrl1_y );
                        geometry_rectangle_embrace( io_view_rect, coord_ctrl2_x, coord_ctrl2_y  );
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        command_start_y = command_end_y;
                    }
                    else if (( last_command=='q' )||( last_command=='Q' )||( last_command=='t' )||( last_command=='T' ))
                    {
                        /* draw */
                        if ( draw )
                        {
                            cairo_curve_to( cr,
                                            coord_ctrl1_x * scale_x + shift_x,
                                            coord_ctrl1_y * scale_y + shift_y,
                                            coord_ctrl1_x * scale_x + shift_x, /* same control point */
                                            coord_ctrl1_y * scale_y + shift_y, /* same control point */
                                            command_end_x * scale_x + shift_x,
                                            command_end_y * scale_y + shift_y
                                          );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, coord_ctrl1_x, coord_ctrl1_y );
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        command_start_y = command_end_y;
                    }
                    else if (( last_command=='a' )||( last_command=='A' ))
                    {
                        double center_x = 0.0;
                        double center_y = 0.0;
                        double start_angle = 0.0;
                        double delta_angle= 0.0;
                        const u8_error_t arc_err
                            = draw_stereotype_image_private_get_arc_center( this_,
                                                                            command_start_x,
                                                                            command_start_y,
                                                                            command_end_x,
                                                                            command_end_y,
                                                                            arc_large_arc,
                                                                            arc_sweep_positive,
                                                                            arc_r_x,
                                                                            arc_r_y,
                                                                            arc_phi,
                                                                            &center_x,
                                                                            &center_y,
                                                                            &start_angle,
                                                                            &delta_angle
                                                                          );

                        /* draw */
                        if ( draw )
                        {
                            if ( arc_err == U8_ERROR_NONE )
                            {
                                cairo_arc( cr,
                                           center_x * scale_x + shift_x,
                                           center_y * scale_y + shift_y,
                                           arc_r_x * scale_x,
                                           start_angle,
                                           start_angle + delta_angle
                                         );
                                /* TODO draw arc correctly, use cairo_arc_negative,
                                 * see https://www.w3.org/TR/SVG/implnote.html#ArcConversionEndpointToCenter */
                            }
                            else if ( arc_err == U8_ERROR_EDGE_CASE_PARAM )
                            {
                                cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                            }
                            else
                            {
                                cairo_move_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                            }
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        command_start_y = command_end_y;
                    }
                    else
                    {
                        U8_TRACE_INFO("inconsistent statemachine");
                        assert(false);
                    }
                    /* continue with next */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COMMAND_OR_COORD_SEQ;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_X:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_x = is_absolute ? value : ( command_start_x + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_Y;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL1_Y:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_y = is_absolute ? value : ( command_start_y + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_X;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_X:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl2_x = is_absolute ? value : ( command_start_x + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_Y;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_CTRL2_Y:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl2_y = is_absolute ? value : ( command_start_y + value );
                    /* continue with next */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_X:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_x = is_absolute ? value : ( command_start_x + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_Y;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_QCTRL_Y:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_y = is_absolute ? value : ( command_start_y + value );
                    /* continue with next */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RX:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_r_x = value;
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RY;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_RY:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_r_y = value;
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_PHI;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_PHI:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_phi = value * ( M_PI / 180.0 );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_LARGE;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_LARGE:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_large_arc = ( value > 0.0001 );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_SWEEP;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_ARC_SWEEP:
            {
                if ( utf8stringview_equals_str( tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        u8_error_info_init_line( out_err_info,
                                                 float_err,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= float_err;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_sweep_positive = ( value > 0.0001 );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_STEREOTYPE_IMAGE_EXPECT_COORD_END_X;
                }
            }
            break;

            case DRAW_STEREOTYPE_IMAGE_EXPECT_EXIT:
            {
                /* the outer while loop should already have been exited */
                assert( false );
            }
            break;
        }
    }

    /* clean up unfinished drawings */
    if ( parser_state != DRAW_STEREOTYPE_IMAGE_EXPECT_EXIT )
    {
        if ( draw )
        {
            cairo_stroke (cr);
        }
        result |= U8_ERROR_PARSER_STRUCTURE;
        /* if no other error encountered yet, report this one: */
        if ( ! u8_error_info_is_error( out_err_info ) )
        {
            u8_error_info_init_line( out_err_info,
                                     U8_ERROR_PARSER_STRUCTURE,
                                     utf8stringviewtokenizer_get_line( tok_iterator )
                                   );
        }
    }

    /* check if anything was drawn at all */
    if ( geometry_rectangle_is_point( io_view_rect ) )
    {
        result |= U8_ERROR_NOT_FOUND;
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
