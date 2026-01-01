/* File: draw_svg_path_data.c; Copyright and License: see below */

#include "draw_svg_path_data.h"
#include "layout/layout_visible_set.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! \brief states of parsing drawing directives */
enum draw_svg_path_data_expect_enum {
    DRAW_SVG_PATH_DATA_EXPECT_COMMAND,  /*!< expecting initial drawing directive */
    DRAW_SVG_PATH_DATA_EXPECT_COMMAND_OR_COORD_SEQ,  /*!< expecting drawing directive or continuation of coordinate sequence */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_X,  /*!< coordinate sequence, expecting cubic curve control_point1_X float */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_Y,  /*!< coordinate sequence, expecting cubic curve control_point1_Y float */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_X,  /*!< coordinate sequence, expecting cubic curve control_point2_X float */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_Y,  /*!< coordinate sequence, expecting cubic curve control_point2_Y float */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_X,  /*!< coordinate sequence, expecting quadratic curve control_point_X float */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_Y,  /*!< coordinate sequence, expecting quadratic curve control_point_Y float */
    DRAW_SVG_PATH_DATA_EXPECT_ARC_RX,  /*!< arc parameter sequence, expecting arc rx float */
    DRAW_SVG_PATH_DATA_EXPECT_ARC_RY,  /*!< arc parameter sequence, expecting arc rx float */
    DRAW_SVG_PATH_DATA_EXPECT_ARC_PHI,  /*!< arc parameter sequence, expecting arc x axis rotation-angle float */
    DRAW_SVG_PATH_DATA_EXPECT_ARC_LARGE,  /*!< arc parameter sequence, expecting large arc flag */
    DRAW_SVG_PATH_DATA_EXPECT_ARC_SWEEP,  /*!< arc parameter sequence, expecting sweep arc flag */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X,  /*!< coordinate sequence, expecting end_x float */
    DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y,  /*!< coordinate sequence, expecting end_y float */
    DRAW_SVG_PATH_DATA_EXPECT_EXIT,  /*!< double quotes end the processing of path drawing commands */
};

u8_error_t draw_svg_path_data_private_parse ( const draw_svg_path_data_t *this_,
                                              bool draw,
                                              utf8stringviewtokenizer_t *tok_iterator,
                                              geometry_rectangle_t *io_view_rect,
                                              u8_error_info_t *out_err_info,
                                              const geometry_rectangle_t *target_bounds,
                                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != tok_iterator );
    assert( UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY == utf8stringviewtokenizer_get_mode( tok_iterator ) );
    assert( NULL != io_view_rect );
    assert( NULL != out_err_info );
    assert( NULL != target_bounds );
    assert( ( ! draw ) || ( NULL != cr ) );
    u8_error_t result = U8_ERROR_NONE;

    /* calculate scale and shift to convert view rect to target bounds */
    const double view_width = geometry_rectangle_get_width( io_view_rect ) < 0.0001 ? 1.0 : geometry_rectangle_get_width( io_view_rect );
    const double scale_x = geometry_rectangle_get_width( target_bounds ) / view_width;
    const double shift_x = geometry_rectangle_get_left( target_bounds ) - ( scale_x * geometry_rectangle_get_left( io_view_rect ) );
    const double view_height = geometry_rectangle_get_height( io_view_rect ) < 0.0001 ? 1.0 : geometry_rectangle_get_height( io_view_rect );
    const double scale_y = geometry_rectangle_get_height( target_bounds ) / view_height;
    const double shift_y = geometry_rectangle_get_top( target_bounds ) - ( scale_y * geometry_rectangle_get_top( io_view_rect ) );

    /* states while parsing: */
    enum draw_svg_path_data_expect_enum parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND;
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
    bool view_rect_drop_0_0 = true;  /* if io_view_rect only contains the initial point (0,0), true states that */
                                     /* this (0,0) point shall be ignored when updating the io_view_rect. */

    /* init draw */
    if ( draw )
    {
        assert( NULL != cr );
        cairo_move_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
    }

    while( utf8stringviewtokenizer_has_next( tok_iterator )
           && ( parser_state != DRAW_SVG_PATH_DATA_EXPECT_EXIT )
           && ( result == U8_ERROR_NONE ) )
    {
        const utf8stringview_t tok = utf8stringviewtokenizer_next( tok_iterator );
        assert( utf8stringview_get_length( &tok ) > 0 );  /* otherwise this would not be a token */
        U8_TRACE_INFO_VIEW( "token:", tok );

        switch ( parser_state )
        {
            case DRAW_SVG_PATH_DATA_EXPECT_COMMAND:
            {
                const char current = *utf8stringview_get_start( &tok );
                if (( utf8stringview_equals_str( &tok, "\"" ) )||( utf8stringview_equals_str( &tok, "\'" ) ))
                {
                    /* no subpath here to draw */
                    /* end of d attribute, back to caller */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_EXIT;
                }
                else if (( current=='z' )||( current=='Z' ))
                {
                    /* no line_to here because there is no subpath to end */
                    if ( draw )
                    {
                        assert( NULL != cr );
                        cairo_move_to ( cr, subpath_start_x, subpath_start_y );
                    }
                    /* end of subpath */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND;
                    last_command = current;
                }
                else if (( current=='m' )||( current=='M' )||( current=='l' )||( current=='L' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='h' )||( current=='H' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='v' )||( current=='V' ))
                {
                    command_end_x = command_start_x;
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y;
                    last_command = current;
                }
                else if (( current=='c' )||( current=='C' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_X;
                    last_command = current;
                }
                else if (( current=='s' )||( current=='S' ))
                {
                    /* there was no preceding c/C/s/S command, initialize first control point to command start point: */
                    coord_ctrl1_x = command_start_x;
                    coord_ctrl1_y = command_start_y;
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_X;
                    last_command = current;
                }
                else if (( current=='q' )||( current=='Q' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_X;
                    last_command = current;
                }
                else if (( current=='t' )||( current=='T' ))
                {
                    /* there was no preceding q/Q/t/T command, initialize next control point to command start point: */
                    coord_ctrl1_x = command_start_x;
                    coord_ctrl1_y = command_start_y;
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='a' )||( current=='A' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_RX;
                    last_command = current;
                }
                else
                {
                    /* unexpected token */
                    u8_error_info_init_line( out_err_info,
                                             U8_ERROR_PARSER_STRUCTURE,
                                             utf8stringviewtokenizer_get_line( tok_iterator )
                                           );
                    result |= U8_ERROR_PARSER_STRUCTURE;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COMMAND_OR_COORD_SEQ:
            {
                const char current = *utf8stringview_get_start( &tok );
                if (( utf8stringview_equals_str( &tok, "\"" ) )||( utf8stringview_equals_str( &tok, "\'" ) ))
                {
                    /* end of d attribute, back to caller */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_EXIT;
                }
                else if ( current=='z' || current=='Z' )
                {
                    /* close subpath */
                    if ( draw )
                    {
                        assert( NULL != cr );
                        cairo_line_to ( cr, subpath_start_x * scale_x + shift_x, subpath_start_y * scale_y + shift_y );
                    }
                    command_start_x = subpath_start_x;
                    command_start_y = subpath_start_y;
                    /* end of subpath */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND;
                    last_command = current;
                }
                else if (( current=='m' )||( current=='M' )||( current=='l' )||( current=='L' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='h' )||( current=='H' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='v' )||( current=='V' ))
                {
                    command_end_x = command_start_x;
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y;
                    last_command = current;
                }
                else if (( current=='c' )||( current=='C' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_X;
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
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_X;
                    last_command = current;
                }
                else if (( current=='q' )||( current=='Q' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_X;
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
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                    last_command = current;
                }
                else if (( current=='a' )||( current=='A' ))
                {
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_RX;
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
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        /* U8_TRACE_INFO_INT( "line", utf8stringviewtokenizer_get_line( tok_iterator ) ); */
                        assert( byte_length == utf8stringview_get_length( &tok ) );
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
                            assert( NULL != cr );
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_y = command_end_y;
                        /* continue with next */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND_OR_COORD_SEQ;
                    }
                    else if (( last_command=='h' )||( last_command=='H' ))
                    {
                        command_end_x = value_x_abs;
                        command_end_y = command_start_y;
                        /* draw */
                        if ( draw )
                        {
                            assert( NULL != cr );
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        /* continue with next */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND_OR_COORD_SEQ;
                    }
                    else if (( last_command=='c' )||( last_command=='C' ))
                    {
                        coord_ctrl1_x = value_x_abs;
                        /* reading last_command parameters */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_Y;
                    }
                    else if (( last_command=='s' )||( last_command=='S' ))
                    {
                        /* init the first control point as mirror of the last control point: */
                        {
                            coord_ctrl1_x = command_start_x + (command_start_x-coord_ctrl2_x);
                            coord_ctrl1_y = command_start_y + (command_start_y-coord_ctrl2_y);
                        }
                        coord_ctrl2_x = value_x_abs;
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_Y;
                    }
                    else if (( last_command=='q' )||( last_command=='Q' ))
                    {
                        coord_ctrl1_x = value_x_abs;
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_Y;
                    }
                    else if (( last_command=='t' )||( last_command=='T' ))
                    {
                        /* init the next control point as mirror of the last control point: */
                        {
                            coord_ctrl1_x = command_start_x + (command_start_x-coord_ctrl1_x);
                            coord_ctrl1_y = command_start_y + (command_start_y-coord_ctrl1_y);
                        }
                        command_end_x = value_x_abs;
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y;
                    }
                    else if (( last_command=='a' )||( last_command=='A' ))
                    {
                        /* store major radius value to appropriate variable */
                        arc_r_x = value;
                        /* continue reading last_command parameters */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_RY;
                    }
                    else
                    {
                        command_end_x = value_x_abs;
                        /* continue reading last_command parameters */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y;
                    }
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
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
                            assert( NULL != cr );
                            cairo_line_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        geometry_rectangle_embrace( io_view_rect, command_end_x, command_end_y );
                        command_start_x = command_end_x;
                        /* continue with next */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND_OR_COORD_SEQ;
                    }
                    else
                    {
                        /* continue reading more parameters for last_command */
                        parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y;
                    }
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_END_Y:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    command_end_y = is_absolute ? value : ( command_start_y + value );
                    /* do command */
                    if (( last_command=='m' )||( last_command=='M' ))
                    {
                        /* draw */
                        if ( draw )
                        {
                            assert( NULL != cr );
                            cairo_move_to ( cr, command_end_x * scale_x + shift_x, command_end_y * scale_y + shift_y );
                        }
                        /* update state */
                        subpath_start_x = command_end_x;
                        subpath_start_y = command_end_y;
                        command_start_x = command_end_x;
                        command_start_y = command_end_y;
                        last_command = is_absolute ? 'L' : 'l';  /* following coordinates are implicitely line-to commands */
                        /* init or update the io_view_rect parameter */
                        if ( geometry_rectangle_is_point( io_view_rect ) && view_rect_drop_0_0 )
                        {
                            /* when we have the first coordinate, the io_view_rect is not a point anymore... */
                            geometry_rectangle_init( io_view_rect, subpath_start_x, subpath_start_y, 0.0, 0.0 );
                            view_rect_drop_0_0 = false;
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
                            assert( NULL != cr );
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
                            assert( NULL != cr );
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
                            assert( NULL != cr );
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
                            = draw_svg_path_data_private_get_arc_center( this_,
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
                            assert( NULL != cr );
                            if ( arc_err == U8_ERROR_NONE )
                            {
                                const double ellipsis_ratio = ( arc_r_x > 0.001 ) ? ( arc_r_y / arc_r_x ) : 1000.0;
                                cairo_matrix_t orig_matrix;
                                cairo_get_matrix( cr, &orig_matrix );
                                /* scale and shift viewport to icon size */
                                cairo_translate( cr, shift_x, shift_y );
                                cairo_scale( cr, scale_x, scale_y );
                                /* setup SVG transformations */
                                cairo_translate( cr, center_x, center_y );
                                cairo_rotate( cr, arc_phi );
                                cairo_scale( cr, 1.0, ellipsis_ratio );
                                if ( delta_angle < 0.0 )
                                {
                                    cairo_arc_negative( cr,
                                                        0.0,
                                                        0.0,
                                                        arc_r_x,
                                                        start_angle,
                                                        start_angle + delta_angle
                                                      );
                                }
                                else
                                {
                                    cairo_arc( cr,
                                               0.0,
                                               0.0,
                                               arc_r_x,
                                               start_angle,
                                               start_angle + delta_angle
                                             );
                                }
                                cairo_set_matrix( cr, &orig_matrix );
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
                        geometry_rectangle_embrace( io_view_rect, center_x, center_y - arc_r_y );
                        geometry_rectangle_embrace( io_view_rect, center_x - arc_r_x, center_y );
                        geometry_rectangle_embrace( io_view_rect, center_x, center_y + arc_r_y );
                        geometry_rectangle_embrace( io_view_rect, center_x + arc_r_x, center_y );
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
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COMMAND_OR_COORD_SEQ;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_X:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_x = is_absolute ? value : ( command_start_x + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_Y;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL1_Y:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_y = is_absolute ? value : ( command_start_y + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_X;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_X:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl2_x = is_absolute ? value : ( command_start_x + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_Y;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_CTRL2_Y:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl2_y = is_absolute ? value : ( command_start_y + value );
                    /* continue with next */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_X:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_x = is_absolute ? value : ( command_start_x + value );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_Y;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_COORD_QCTRL_Y:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    const bool is_absolute = ( last_command <= 'Z' );
                    coord_ctrl1_y = is_absolute ? value : ( command_start_y + value );
                    /* continue with next */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_ARC_RX:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_r_x = value;
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_RY;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_ARC_RY:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_r_y = value;
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_PHI;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_ARC_PHI:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_phi = value * ( M_PI / 180.0 );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_LARGE;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_ARC_LARGE:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_large_arc = ( value > 0.0001 );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_ARC_SWEEP;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_ARC_SWEEP:
            {
                if ( utf8stringview_equals_str( &tok, "," ) )
                {
                    /* skip */
                }
                else
                {
                    /* read coordinate */
                    /* utf8string_parse_float is faster than utf8stringview_parse_float; only it may not stop at view end but at terminating 0 after svg-path */
                    double value = 0.0;
                    unsigned int byte_length;
                    const u8_error_t float_err
                        = utf8string_parse_float( utf8stringview_get_start( &tok ), &byte_length, &value );
                    if ( float_err != U8_ERROR_NONE )
                    {
                        /* float_err could be a UTF8ERROR_NOT_FOUND or a UTF8ERROR_OUT_OF_RANGE */
                        u8_error_info_init_line( out_err_info,
                                                 U8_ERROR_PARSER_STRUCTURE,
                                                 utf8stringviewtokenizer_get_line( tok_iterator )
                                               );
                        result |= U8_ERROR_PARSER_STRUCTURE;
                    }
                    else
                    {
                        assert( byte_length == utf8stringview_get_length( &tok ) );
                    }
                    /* store value to appropriate variable */
                    arc_sweep_positive = ( value > 0.0001 );
                    /* continue reading last_command parameters */
                    parser_state = DRAW_SVG_PATH_DATA_EXPECT_COORD_END_X;
                }
            }
            break;

            case DRAW_SVG_PATH_DATA_EXPECT_EXIT:  /* or */
            default:
            {
                /* the outer while loop should already have been exited */
                assert( false );
            }
            break;
        }
    }

    /* report error on unfinished drawing */
    if (( result == U8_ERROR_NONE )&&( parser_state != DRAW_SVG_PATH_DATA_EXPECT_EXIT ))
    {
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
    if ( geometry_rectangle_is_point( io_view_rect ) && view_rect_drop_0_0 )
    {
        result |= U8_ERROR_NOT_FOUND;
    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2023-2026 Andreas Warnke
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
