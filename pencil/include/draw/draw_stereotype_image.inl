/* File: draw_stereotype_image.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <math.h>
#include <assert.h>

static inline void draw_stereotype_image_init( draw_stereotype_image_t *this_ )
{

}

static inline void draw_stereotype_image_destroy( draw_stereotype_image_t *this_ )
{

}

static const double rel_size = 1.5;

static inline geometry_dimensions_t draw_stereotype_image_get_dimensions( const draw_stereotype_image_t *this_,
                                                                          const pencil_size_t *pencil_size )
{
    assert( pencil_size != NULL );
    geometry_dimensions_t result;

    const double image_height = rel_size * pencil_size_get_title_font_size( pencil_size );
    geometry_dimensions_init ( &result, DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT * image_height, image_height );

    return result;
}

static inline geometry_rectangle_t draw_stereotype_image_get_bounds ( const draw_stereotype_image_t *this_,
                                                                      double x,
                                                                      double y,
                                                                      geometry_h_align_t h_align,
                                                                      geometry_v_align_t v_align,
                                                                      const pencil_size_t *pencil_size )
{
    geometry_rectangle_t result;

    const double image_height = rel_size * pencil_size_get_title_font_size( pencil_size );
    const double image_width = DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT * image_height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, image_width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, image_height, y, 0.0 ),
                              image_width,
                              image_height
                            );

    return result;
}

static inline double draw_stereotype_image_private_get_angle ( const draw_stereotype_image_t *this_,
                                                               double u_x,
                                                               double u_y,
                                                               double v_x,
                                                               double v_y )
{
    const bool negative = (( u_x * v_y ) + ( u_y * v_x )) < 0.0;
    const double len_u = sqrt( ( u_x * u_x ) + ( u_y * u_y ) );
    const double len_v = sqrt( ( v_x * v_x ) + ( v_y * v_y ) );
    const double abs_angle = acos((( u_x * v_x ) + ( u_y * v_y )) / ( len_u * len_v ));
    return negative ? ( -abs_angle ) : abs_angle;
}

static inline u8_error_t draw_stereotype_image_private_get_arc_center ( const draw_stereotype_image_t *this_,
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
        const double rot_d_x = cos_phi * half_dx + sin_phi * half_dy;
        const double rot_d_y = (-sin_phi) * half_dx + cos_phi * half_dy;

        /* see B.2.5, step 3, eq 6.2 */
        double sqr_r_x = r_x * r_x;
        double sqr_r_y = r_y * r_y;
        const double sqr_rot_d_x = rot_d_x * rot_d_x;
        const double sqr_rot_d_y = rot_d_y * rot_d_y;
        const double sqr_too_small_factor = ( sqr_rot_d_x / sqr_r_x ) + ( sqr_rot_d_y / sqr_r_y );

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
        const double numerator = ( sqr_r_x * sqr_r_y ) - ( sqr_r_x * sqr_rot_d_y ) - ( sqr_r_y * sqr_rot_d_x );
        double denominator = ( sqr_r_x * sqr_rot_d_y ) + ( sqr_r_y * sqr_rot_d_x );
        if ( denominator < 0.000000000001 )
        {
            /* start and end points are equal */
            result = U8_ERROR_VALUE_OUT_OF_RANGE;
            denominator = 0.000000000001;
        }
        const double sqr_factor = numerator / denominator;
        double rot_c_x = ( r_x * rot_d_y ) / r_y;
        double rot_c_y = - ( r_y * rot_d_x ) / r_x;
        if ( sqr_factor <= 0.0 )
        {
            /* rounding error and/or just 1 solution: */
            /* nothing to do in this case */
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
        *out_start_angle = draw_stereotype_image_private_get_angle( this_,
                                                                    1.0,
                                                                    0.0,
                                                                    ( rot_d_x - rot_c_x ) / r_x,
                                                                    ( rot_d_y - rot_c_y ) / r_y
                                                                  );
        /* see B.2.4, step 4, eq 5.6 */
        double delta_angle = draw_stereotype_image_private_get_angle( this_,
                                                                      ( rot_d_x - rot_c_x ) / r_x,
                                                                      ( rot_d_y - rot_c_y ) / r_y,
                                                                      ( (-rot_d_x) - rot_c_x ) / r_x,
                                                                      ( (-rot_d_y) - rot_c_y ) / r_y
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
