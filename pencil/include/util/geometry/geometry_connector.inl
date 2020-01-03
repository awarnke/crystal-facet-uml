/* File: geometry_connector.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <math.h>

static inline void geometry_connector_init_vertical ( geometry_connector_t *this_,
                                                      double source_end_x,
                                                      double source_end_y,
                                                      double destination_end_x,
                                                      double destination_end_y,
                                                      double main_line_x )
{
    (*this_).source_end_x = source_end_x;
    (*this_).source_end_y = source_end_y;
    (*this_).main_line_source_x = main_line_x;
    (*this_).main_line_source_y = source_end_y;
    (*this_).main_line_destination_x = main_line_x;
    (*this_).main_line_destination_y = destination_end_y;
    (*this_).destination_end_x = destination_end_x;
    (*this_).destination_end_y = destination_end_y;
}

static inline void geometry_connector_init_horizontal ( geometry_connector_t *this_,
                                                        double source_end_x,
                                                        double source_end_y,
                                                        double destination_end_x,
                                                        double destination_end_y,
                                                        double main_line_y )
{
    (*this_).source_end_x = source_end_x;
    (*this_).source_end_y = source_end_y;
    (*this_).main_line_source_x = source_end_x;
    (*this_).main_line_source_y = main_line_y;
    (*this_).main_line_destination_x = destination_end_x;
    (*this_).main_line_destination_y = main_line_y;
    (*this_).destination_end_x = destination_end_x;
    (*this_).destination_end_y = destination_end_y;
}

static inline void geometry_connector_reinit_vertical ( geometry_connector_t *this_,
                                                        double source_end_x,
                                                        double source_end_y,
                                                        double destination_end_x,
                                                        double destination_end_y,
                                                        double main_line_x )
{
    (*this_).source_end_x = source_end_x;
    (*this_).source_end_y = source_end_y;
    (*this_).main_line_source_x = main_line_x;
    (*this_).main_line_source_y = source_end_y;
    (*this_).main_line_destination_x = main_line_x;
    (*this_).main_line_destination_y = destination_end_y;
    (*this_).destination_end_x = destination_end_x;
    (*this_).destination_end_y = destination_end_y;
}

static inline void geometry_connector_reinit_horizontal ( geometry_connector_t *this_,
                                                          double source_end_x,
                                                          double source_end_y,
                                                          double destination_end_x,
                                                          double destination_end_y,
                                                          double main_line_y )
{
    (*this_).source_end_x = source_end_x;
    (*this_).source_end_y = source_end_y;
    (*this_).main_line_source_x = source_end_x;
    (*this_).main_line_source_y = main_line_y;
    (*this_).main_line_destination_x = destination_end_x;
    (*this_).main_line_destination_y = main_line_y;
    (*this_).destination_end_x = destination_end_x;
    (*this_).destination_end_y = destination_end_y;
}

static inline void geometry_connector_copy ( geometry_connector_t *this_, const geometry_connector_t *original )
{
    (*this_) = (*original);
}

static inline void geometry_connector_replace ( geometry_connector_t *this_, const geometry_connector_t *original )
{
    (*this_) = (*original);
}

static inline void geometry_connector_init_empty ( geometry_connector_t *this_ )
{
    (*this_).source_end_x = 0.0;
    (*this_).source_end_y = 0.0;
    (*this_).main_line_source_x = 0.0;
    (*this_).main_line_source_y = 0.0;
    (*this_).main_line_destination_x = 0.0;
    (*this_).main_line_destination_y = 0.0;
    (*this_).destination_end_x = 0.0;
    (*this_).destination_end_y = 0.0;
}

static inline void geometry_connector_reinit_empty ( geometry_connector_t *this_ )
{
    (*this_).source_end_x = 0.0;
    (*this_).source_end_y = 0.0;
    (*this_).main_line_source_x = 0.0;
    (*this_).main_line_source_y = 0.0;
    (*this_).main_line_destination_x = 0.0;
    (*this_).main_line_destination_y = 0.0;
    (*this_).destination_end_x = 0.0;
    (*this_).destination_end_y = 0.0;
}

static inline void geometry_connector_destroy ( geometry_connector_t *this_ )
{
}

static inline double geometry_connector_get_source_end_x ( const geometry_connector_t *this_ )
{
    return (*this_).source_end_x;
}

static inline double geometry_connector_get_source_end_y ( const geometry_connector_t *this_ )
{
    return (*this_).source_end_y;
}

static inline double geometry_connector_get_main_line_source_x ( const geometry_connector_t *this_ )
{
    return (*this_).main_line_source_x;
}

static inline double geometry_connector_get_main_line_source_y ( const geometry_connector_t *this_ )
{
    return (*this_).main_line_source_y;
}

static inline double geometry_connector_get_main_line_destination_x ( const geometry_connector_t *this_ )
{
    return (*this_).main_line_destination_x;
}

static inline double geometry_connector_get_main_line_destination_y ( const geometry_connector_t *this_ )
{
    return (*this_).main_line_destination_y;
}

static inline double geometry_connector_destination_end_x ( const geometry_connector_t *this_ )
{
    return (*this_).destination_end_x;
}

static inline double geometry_connector_get_destination_end_y ( const geometry_connector_t *this_ )
{
    return (*this_).destination_end_y;
}

static inline geometry_point_t geometry_connector_calculate_waypoint ( const geometry_connector_t *this_,
                                                                       double distance_covered )
{
    geometry_point_t result;
    
    const double source_end_length = fabs( (*this_).source_end_x - (*this_).main_line_source_x )
                                     + fabs( (*this_).source_end_y - (*this_).main_line_source_y );
    const double main_line_length = fabs( (*this_).main_line_source_x - (*this_).main_line_destination_x )
                                    + fabs( (*this_).main_line_source_y - (*this_).main_line_destination_y );
    const double dest_end_length = fabs( (*this_).main_line_destination_x - (*this_).destination_end_x )
                                   + fabs( (*this_).main_line_destination_y - (*this_).destination_end_y );
  
    if ( distance_covered < source_end_length ) {
        if (( distance_covered <= 0.0 )||( source_end_length < 0.000000001 ))
        {
            geometry_point_init ( &result, (*this_).source_end_x, (*this_).source_end_y );
        }
        else 
        {
            geometry_point_init ( &result, (*this_).source_end_x, (*this_).source_end_y );
            const double segment_part1 = distance_covered / source_end_length;
            geometry_point_shift( &result, 
                                  segment_part1 * ( (*this_).main_line_source_x - (*this_).source_end_x ),
                                  segment_part1 * ( (*this_).main_line_source_y - (*this_).source_end_y )
                                );
        }
    }
    else {
        const double shifted_distance = distance_covered - source_end_length;
        if (( shifted_distance < main_line_length )&&( main_line_length >= 0.000000001 ))
        {
            geometry_point_init ( &result, (*this_).main_line_source_x, (*this_).main_line_source_y );
            const double segment_part2 = shifted_distance / main_line_length;
            geometry_point_shift( &result, 
                                  segment_part2 * ( (*this_).main_line_destination_x - (*this_).main_line_source_x ),
                                  segment_part2 * ( (*this_).main_line_destination_y - (*this_).main_line_source_y )
                                );
        }
        else 
        {
            const double shifted2_distance = shifted_distance - main_line_length;
            if (( shifted2_distance < dest_end_length )&&( dest_end_length >= 0.000000001 ))
            {
                geometry_point_init ( &result, (*this_).main_line_destination_x, (*this_).main_line_destination_y );
                const double segment_part3 = shifted2_distance / dest_end_length;
                geometry_point_shift( &result, 
                                    segment_part3 * ( (*this_).destination_end_x - (*this_).main_line_destination_x ),
                                    segment_part3 * ( (*this_).destination_end_y - (*this_).main_line_destination_y )
                                    );
            }
            else 
            {
                geometry_point_init ( &result, (*this_).destination_end_x, (*this_).destination_end_y );
            }
        }
    }
        
    return result;
}

static inline double geometry_connector_get_length ( const geometry_connector_t *this_ )
{
    double source_end_length;
    double main_line_length;
    double dest_end_length;
    source_end_length = fabs( (*this_).source_end_x - (*this_).main_line_source_x )
                        + fabs( (*this_).source_end_y - (*this_).main_line_source_y );
    main_line_length = fabs( (*this_).main_line_source_x - (*this_).main_line_destination_x )
                       + fabs( (*this_).main_line_source_y - (*this_).main_line_destination_y );
    dest_end_length = fabs( (*this_).main_line_destination_x - (*this_).destination_end_x )
                      + fabs( (*this_).main_line_destination_y - (*this_).destination_end_y );
    return source_end_length + main_line_length + dest_end_length;
}

static inline bool geometry_connector_is_close ( const geometry_connector_t *this_, double x, double y, double max_distance )
{
    bool close_to_source_end_line;
    bool close_to_main_line;
    bool close_to_destination_end_line;
    close_to_source_end_line = ((( (*this_).source_end_x - max_distance < x ) && ( x < (*this_).main_line_source_x + max_distance ))
                               || (( (*this_).main_line_source_x - max_distance < x ) && ( x < (*this_).source_end_x + max_distance )))
                               && ((( (*this_).source_end_y - max_distance < y ) && ( y < (*this_).main_line_source_y + max_distance ))
                               || (( (*this_).main_line_source_y - max_distance < y ) && ( y < (*this_).source_end_y + max_distance )));
    close_to_main_line = ((( (*this_).main_line_destination_x - max_distance < x ) && ( x < (*this_).main_line_source_x + max_distance ))
                         || (( (*this_).main_line_source_x - max_distance < x ) && ( x < (*this_).main_line_destination_x + max_distance )))
                         && ((( (*this_).main_line_destination_y - max_distance < y ) && ( y < (*this_).main_line_source_y + max_distance ))
                         || (( (*this_).main_line_source_y - max_distance < y ) && ( y < (*this_).main_line_destination_y + max_distance )));
    close_to_destination_end_line = ((( (*this_).main_line_destination_x - max_distance < x ) && ( x < (*this_).destination_end_x + max_distance ))
                                    || (( (*this_).destination_end_x - max_distance < x ) && ( x < (*this_).main_line_destination_x + max_distance )))
                                    && ((( (*this_).main_line_destination_y - max_distance < y ) && ( y < (*this_).destination_end_y + max_distance ))
                                    || (( (*this_).destination_end_y - max_distance < y ) && ( y < (*this_).main_line_destination_y + max_distance )));
    return ( close_to_source_end_line || close_to_main_line || close_to_destination_end_line );
}

static inline bool geometry_connector_is_intersecting_rectangle ( const geometry_connector_t *this_, const geometry_rectangle_t *rect )
{
    bool result;

    /* determine the rectangle (minus a small border) */
    double rect_left = geometry_rectangle_get_left(rect) + 0.001;
    double rect_right = geometry_rectangle_get_right(rect) - 0.001;
    double rect_top = geometry_rectangle_get_top(rect) + 0.001;
    double rect_bottom = geometry_rectangle_get_bottom(rect) - 0.001;

    /* do some simple pre-checks */
    if (( (*this_).source_end_x < rect_left )
        &&  ( (*this_).destination_end_x < rect_left )
        &&  ( (*this_).main_line_destination_x < rect_left ) )
    {
        result = false;
    }
    else if (( (*this_).source_end_x > rect_right )
        &&  ( (*this_).destination_end_x > rect_right )
        &&  ( (*this_).main_line_destination_x > rect_right ) )
    {
        result = false;
    }
    else if (( (*this_).source_end_y < rect_top )
        &&  ( (*this_).destination_end_y < rect_top )
        &&  ( (*this_).main_line_destination_y < rect_top ) )
    {
        result = false;
    }
    else if (( (*this_).source_end_y > rect_bottom )
        &&  ( (*this_).destination_end_y > rect_bottom )
        &&  ( (*this_).main_line_destination_y > rect_bottom ) )
    {
        result = false;
    }
    else
    {
        /* the simple pre-checks did not decide if the connector intersects the rectangle */
        result = false;

        /* check for an overlap of the first segment */
        if ( (*this_).source_end_x < rect_left )
        {
            if (( (*this_).main_line_source_x > rect_left )
                && ( (*this_).main_line_source_y > rect_top )
                && ( (*this_).main_line_source_y < rect_bottom ) )
            {
                result = true;
            }
        }
        else if ( (*this_).source_end_x > rect_right )
        {
            if (( (*this_).main_line_source_x < rect_right )
                && ( (*this_).main_line_source_y > rect_top )
                && ( (*this_).main_line_source_y < rect_bottom ) )
            {
                result = true;
            }
        }
        else
        {
            if ( (*this_).source_end_y < rect_top )
            {
                if ( (*this_).main_line_source_y > rect_top )
                {
                    result = true;
                }
            }
            else if ( (*this_).source_end_y > rect_bottom )
            {
                if ( (*this_).main_line_source_y < rect_bottom )
                {
                    result = true;
                }
            }
            else
            {
                result = true;
            }
        }

        /* check for an overlap of the last segment */
        if ( ! result )
        {
            if ( (*this_).destination_end_x < rect_left )
            {
                if (( (*this_).main_line_destination_x > rect_left )
                    && ( (*this_).main_line_destination_y > rect_top )
                    && ( (*this_).main_line_destination_y < rect_bottom ) )
                {
                    result = true;
                }
            }
            else if ( (*this_).destination_end_x > rect_right )
            {
                if (( (*this_).main_line_destination_x < rect_right )
                    && ( (*this_).main_line_destination_y > rect_top )
                    && ( (*this_).main_line_destination_y < rect_bottom ) )
                {
                    result = true;
                }
            }
            else
            {
                if ( (*this_).destination_end_y < rect_top )
                {
                    if ( (*this_).main_line_destination_y > rect_top )
                    {
                        result = true;
                    }
                }
                else if ( (*this_).destination_end_y > rect_bottom )
                {
                    if ( (*this_).main_line_destination_y < rect_bottom )
                    {
                        result = true;
                    }
                }
                else
                {
                    result = true;
                }
            }
        }

        /* check for an overlap of the middle main line */
        if ( ! result )
        {
            if ( (*this_).main_line_source_x < rect_left )
            {
                if (( (*this_).main_line_destination_x > rect_left )
                    && ( (*this_).main_line_destination_y > rect_top )
                    && ( (*this_).main_line_destination_y < rect_bottom ) )
                {
                    result = true;
                }
            }
            else if ( (*this_).main_line_source_x > rect_right )
            {
                if (( (*this_).main_line_destination_x < rect_right )
                    && ( (*this_).main_line_destination_y > rect_top )
                    && ( (*this_).main_line_destination_y < rect_bottom ) )
                {
                    result = true;
                }
            }
            else
            {
                if ( (*this_).main_line_source_y < rect_top )
                {
                    if ( (*this_).main_line_destination_y > rect_top )
                    {
                        result = true;
                    }
                }
                else if ( (*this_).main_line_source_y > rect_bottom )
                {
                    if ( (*this_).main_line_destination_y < rect_bottom )
                    {
                        result = true;
                    }
                }
                else
                {
                    result = true;
                    TSLOG_WARNING( "This case should have been covered already before." );
                }
            }
        }
    }

    return result;
}

static inline uint32_t geometry_connector_count_connector_intersects ( const geometry_connector_t *this_, const geometry_connector_t *that )
{
    uint32_t result = 0;

    /* do some simple prechecks */
    geometry_rectangle_t this_bounds;
    geometry_rectangle_t that_bounds;

    this_bounds = geometry_connector_get_bounding_rectangle( this_ );
    that_bounds = geometry_connector_get_bounding_rectangle( that );

    if ( geometry_rectangle_is_intersecting( &this_bounds, &that_bounds ) )
    {
        /* do some more complicated checks */
        geometry_rectangle_t this_source_end;
        geometry_rectangle_t this_main_line;
        geometry_rectangle_t this_destination_end;
        geometry_rectangle_t that_source_end;
        geometry_rectangle_t that_main_line;
        geometry_rectangle_t that_destination_end;

        geometry_rectangle_init_by_corners( &this_source_end,
                                            (*this_).main_line_source_x, (*this_).main_line_source_y, (*this_).source_end_x, (*this_).source_end_y
                                          );
        geometry_rectangle_init_by_corners( &this_main_line,
                                            (*this_).main_line_destination_x, (*this_).main_line_destination_y, (*this_).main_line_source_x, (*this_).main_line_source_y
                                          );
        geometry_rectangle_init_by_corners( &this_destination_end,
                                            (*this_).destination_end_x, (*this_).destination_end_y, (*this_).main_line_destination_x, (*this_).main_line_destination_y
                                          );
        geometry_rectangle_init_by_corners( &that_source_end,
                                            (*that).main_line_source_x, (*that).main_line_source_y, (*that).source_end_x, (*that).source_end_y
                                          );
        geometry_rectangle_init_by_corners( &that_main_line,
                                            (*that).main_line_destination_x, (*that).main_line_destination_y, (*that).main_line_source_x, (*that).main_line_source_y
                                          );
        geometry_rectangle_init_by_corners( &that_destination_end,
                                            (*that).destination_end_x, (*that).destination_end_y, (*that).main_line_destination_x, (*that).main_line_destination_y
                                          );

        if ( geometry_rectangle_is_intersecting( &this_source_end, &that_source_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_main_line, &that_source_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_destination_end, &that_source_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_source_end, &that_main_line ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_main_line, &that_main_line ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_destination_end, &that_main_line ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_source_end, &that_destination_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_main_line, &that_destination_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_intersecting( &this_destination_end, &that_destination_end ) )
        {
            result += 1;
        }

        geometry_rectangle_destroy( &this_source_end );
        geometry_rectangle_destroy( &this_main_line );
        geometry_rectangle_destroy( &this_destination_end );
        geometry_rectangle_destroy( &that_source_end );
        geometry_rectangle_destroy( &that_main_line );
        geometry_rectangle_destroy( &that_destination_end );
    }
    /* else no intersects, result = 0 */

    geometry_rectangle_destroy( &this_bounds );
    geometry_rectangle_destroy( &that_bounds );

    return result;
}

static inline geometry_rectangle_t geometry_connector_get_bounding_rectangle ( const geometry_connector_t *this_ )
{
    geometry_rectangle_t result;

    double left;
    double right;
    double top;
    double bottom;

    left = fmin ( (*this_).main_line_source_x, fmin( (*this_).source_end_x, (*this_).destination_end_x ) );
    right = fmax ( (*this_).main_line_source_x, fmax( (*this_).source_end_x, (*this_).destination_end_x ) );
    top = fmin ( (*this_).main_line_source_y, fmin( (*this_).source_end_y, (*this_).destination_end_y ) );
    bottom = fmax ( (*this_).main_line_source_y, fmax( (*this_).source_end_y, (*this_).destination_end_y ) );

    geometry_rectangle_init( &result,
                             left,
                             top,
                             right - left,
                             bottom - top
                           );

    return result;
}

static inline void geometry_connector_trace ( const geometry_connector_t *this_ )
{
    TRACE_INFO( "geometry_connector_t" );
    TRACE_INFO_INT( "- source_end_x:", (*this_).source_end_x );
    TRACE_INFO_INT( "- source_end_y:", (*this_).source_end_y );
    TRACE_INFO_INT( "- main_line_source_x:", (*this_).main_line_source_x );
    TRACE_INFO_INT( "- main_line_source_y:", (*this_).main_line_source_y );
    TRACE_INFO_INT( "- main_line_destination_x:", (*this_).main_line_destination_x );
    TRACE_INFO_INT( "- main_line_destination_y:", (*this_).main_line_destination_y );
    TRACE_INFO_INT( "- destination_end_x:", (*this_).destination_end_x );
    TRACE_INFO_INT( "- destination_end_y:", (*this_).destination_end_y );
}


/*
Copyright 2017-2020 Andreas Warnke

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
