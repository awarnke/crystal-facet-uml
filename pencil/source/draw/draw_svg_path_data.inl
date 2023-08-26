/* File: draw_svg_path_data.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <math.h>
#include <assert.h>

static inline void draw_svg_path_data_init( draw_svg_path_data_t *this_ )
{

}

static inline void draw_svg_path_data_destroy( draw_svg_path_data_t *this_ )
{

}

static inline u8_error_t draw_svg_path_data_parse_bounds ( const draw_svg_path_data_t *this_,
                                                           utf8stringviewtokenizer_t *tok_iterator,
                                                           geometry_rectangle_t *out_view_rect,
                                                           u8_error_info_t *out_err_info
                                                         )
{
    const geometry_rectangle_t dummy_target = { .left = 0.0, .top = 0.0, .width = 0.0, .height = 0.0 };
    const u8_error_t result
        = draw_svg_path_data_private_parse( this_,
                                            false,  /* draw */
                                            tok_iterator,
                                            out_view_rect,
                                            out_err_info,
                                            &dummy_target,
                                            NULL  /* cr */
                                          );
    return result;
}

static inline u8_error_t draw_svg_path_data_draw ( const draw_svg_path_data_t *this_,
                                                   utf8stringviewtokenizer_t *tok_iterator,
                                                   const geometry_rectangle_t *in_view_rect,
                                                   u8_error_info_t *out_err_info,
                                                   const geometry_rectangle_t *target_bounds,
                                                   cairo_t *cr
                                                 )
{
    geometry_rectangle_t non_const_view = *in_view_rect;
    const u8_error_t result
        = draw_svg_path_data_private_parse( this_,
                                            true,  /* draw */
                                            tok_iterator,
                                            &non_const_view,
                                            out_err_info,
                                            target_bounds,
                                            cr
                                          );
    return result;
}

/*!
 *  \brief parses the svg path data drawing commands and draws these into the target_bounds rect
 *
 *  \param this_ pointer to own object attributes
 *  \param draw false if only the view_rect shall be determined, true if the drawing_directives shall be drawn
 *  \param tok_iterator token iterator of drawing directives. The first token shall be the first draw command.
 *                      double quotes or end-of-stream end the processing, the double quotes token is already consumed.
 *                      the token iterator must be in mode UTF8STRINGVIEWTOKENMODE_FLOAT_ONLY.
 *  \param[in,out] io_view_rect bounding rectangle of the svg drawing_directives;
 *                              in all cases this is provided as output,
 *                              in case of draw==true this is needed as input,
 *                              in case of draw==false this is written as output.
 *  \param[out] out_err_info pointer to an error_info_t data struct;
 *                           in case of U8_ERROR_PARSER_STRUCTURE, it provides an error description when returning
 *  \param target_bounds bounding rectangle of the cairo drawing directives, to which io_view_rect is scaled to.
 *  \param cr a cairo drawing context or NULL in case draw==false
 *  \return U8_ERROR_NONE if the image was drawn,
 *          U8_ERROR_NOT_FOUND if no image was found,
 *          U8_ERROR_PARSER_STRUCTURE if expected tokens in the input image were missing or in wrong order
 */


static inline double draw_svg_path_data_private_get_angle ( const draw_svg_path_data_t *this_,
                                                            double u_x,
                                                            double u_y,
                                                            double v_x,
                                                            double v_y )
{
    const bool negative = (( u_x * v_y ) - ( u_y * v_x )) < 0.0;
    const double len_u = sqrt( ( u_x * u_x ) + ( u_y * u_y ) );
    const double len_v = sqrt( ( v_x * v_x ) + ( v_y * v_y ) );
    const double abs_angle = acos((( u_x * v_x ) + ( u_y * v_y )) / ( len_u * len_v ));
    return negative ? ( -abs_angle ) : abs_angle;
}

static inline u8_error_t draw_svg_path_data_private_get_arc_center ( const draw_svg_path_data_t *this_,
                                                                     double start_x,
                                                                     double start_y,
                                                                     double end_x,
                                                                     double end_y,
                                                                     bool large_arc,
                                                                     bool sweep_positive_dir,
                                                                     double r_x,
                                                                     double r_y,
                                                                     double phi,
                                                                     double *out_center_x,
                                                                     double *out_center_y,
                                                                     double *out_start_angle,
                                                                     double *out_delta_angle )
{
    U8_TRACE_BEGIN();
    assert( out_center_x != NULL );
    assert( out_center_y != NULL );
    assert( out_start_angle != NULL );
    assert( out_delta_angle != NULL );
    u8_error_t result = U8_ERROR_NONE;

    /* see https://www.w3.org/TR/SVG/implnote.html#ArcConversionEndpointToCenter */

    /* see B.2.5, step 2, eq 6.1 */
    r_x = fabs( r_x );
    r_y = fabs( r_y );

    /* see B.2.5, step 1 */
    if (( r_x < 0.000001 )||( r_y < 0.000001 ))
    {
        /* This is a valid case but cannot be handled by this function */
        result = U8_ERROR_EDGE_CASE_PARAM;
    }

    if ( result == U8_ERROR_NONE )
    {
        /* see B.2.4, step 1, eq 5.1 */
        const double cos_phi = cos( phi );
        const double sin_phi = sin( phi );
        const double half_dx = ( start_x - end_x ) / 2.0;
        const double half_dy = ( start_y - end_y ) / 2.0;
        const double rot_half_dx = cos_phi * half_dx + sin_phi * half_dy;
        const double rot_half_dy = (-sin_phi) * half_dx + cos_phi * half_dy;

        /* see B.2.5, step 3, eq 6.2 */
        double sqr_r_x = r_x * r_x;
        double sqr_r_y = r_y * r_y;
        const double sqr_rot_half_dx = rot_half_dx * rot_half_dx;
        const double sqr_rot_half_dy = rot_half_dy * rot_half_dy;
        const double sqr_too_small_factor = ( sqr_rot_half_dx / sqr_r_x ) + ( sqr_rot_half_dy / sqr_r_y );

        /* see B.2.5, step 3, eq 6.3 */
        if ( sqr_too_small_factor > 1.0 )
        {
            const double too_small_factor = sqrt( sqr_too_small_factor );
            r_x = too_small_factor * r_x;
            r_y = too_small_factor * r_y;
            sqr_r_x = r_x * r_x;
            sqr_r_y = r_y * r_y;
        }

        /* see B.2.4, step 2, eq 5.2 */
        const double numerator = ( sqr_r_x * sqr_r_y ) - ( sqr_r_x * sqr_rot_half_dy ) - ( sqr_r_y * sqr_rot_half_dx );
        double denominator = ( sqr_r_x * sqr_rot_half_dy ) + ( sqr_r_y * sqr_rot_half_dx );
        if ( denominator < 0.000000000001 )
        {
            /* start and end points are equal */
            result = U8_ERROR_VALUE_OUT_OF_RANGE;
            denominator = 0.000000000001;
        }
        const double sqr_factor = numerator / denominator;
        double rot_c_x = ( r_x * rot_half_dy ) / r_y;
        double rot_c_y = - ( r_y * rot_half_dx ) / r_x;
        if ( sqr_factor <= 0.0 )
        {
            /* case: rounding error and/or just 1 solution; factor is 0.0 */
            rot_c_x = 0.0;
            rot_c_y = 0.0;
        }
        else
        {
            const double factor = sqrt( sqr_factor );
            if ( large_arc != sweep_positive_dir )
            {
                rot_c_x = factor * rot_c_x;
                rot_c_y = factor * rot_c_y;
            }
            else
            {
                rot_c_x = ( -factor ) * rot_c_x;
                rot_c_y = ( -factor ) * rot_c_y;
            }
        }

        /* see B.2.4, step 3, eq 5.3 */
        const double half_way_x = ( start_x + end_x ) / 2.0;
        const double half_way_y = ( start_y + end_y ) / 2.0;
        *out_center_x = (( cos_phi * rot_c_x ) + ( (-sin_phi) * rot_c_y )) + half_way_x;
        *out_center_y = (( sin_phi * rot_c_x ) + ( cos_phi * rot_c_y )) + half_way_y;

        /* see B.2.4, step 4, eq 5.5 */
        *out_start_angle = draw_svg_path_data_private_get_angle( this_,
                                                                 1.0,
                                                                 0.0,
                                                                 ( rot_half_dx - rot_c_x ) / r_x,
                                                                 ( rot_half_dy - rot_c_y ) / r_y
                                                               );
        /* see B.2.4, step 4, eq 5.6 */
        const double delta_angle = draw_svg_path_data_private_get_angle( this_,
                                                                         ( rot_half_dx - rot_c_x ) / r_x,
                                                                         ( rot_half_dy - rot_c_y ) / r_y,
                                                                         ( (-rot_half_dx) - rot_c_x ) / r_x,
                                                                         ( (-rot_half_dy) - rot_c_y ) / r_y
                                                                       );
        *out_delta_angle
            = sweep_positive_dir
            ? ( ( delta_angle < 0.0 ) ? ( delta_angle + ( 2.0 * M_PI ) ) : delta_angle )
            : ( ( delta_angle > 0.0 ) ? ( delta_angle - ( 2.0 * M_PI ) ) : delta_angle );
    }

    U8_TRACE_END_ERR(result);
    return U8_ERROR_NONE;
}


/*
Copyright 2023-2023 Andreas Warnke

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
