/* File: geometry_connector.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "u8/u8_interval.h"
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

static inline double geometry_connector_get_destination_end_x ( const geometry_connector_t *this_ )
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
    const double source_end_length
        = fabs( (*this_).source_end_x - (*this_).main_line_source_x )
        + fabs( (*this_).source_end_y - (*this_).main_line_source_y );
    const double main_line_length
        = fabs( (*this_).main_line_source_x - (*this_).main_line_destination_x )
        + fabs( (*this_).main_line_source_y - (*this_).main_line_destination_y );
    const double dest_end_length
        = fabs( (*this_).main_line_destination_x - (*this_).destination_end_x )
        + fabs( (*this_).main_line_destination_y - (*this_).destination_end_y );
    return source_end_length + main_line_length + dest_end_length;
}

static inline double geometry_connector_get_source_length ( const geometry_connector_t *this_ )
{
    const double source_end_length
        = fabs( (*this_).source_end_x - (*this_).main_line_source_x )
        + fabs( (*this_).source_end_y - (*this_).main_line_source_y );
    return source_end_length;
}

static inline double geometry_connector_get_main_length ( const geometry_connector_t *this_ )
{
    const double main_line_length
        = fabs( (*this_).main_line_source_x - (*this_).main_line_destination_x )
        + fabs( (*this_).main_line_source_y - (*this_).main_line_destination_y );
    return main_line_length;
}

static inline double geometry_connector_get_destination_length ( const geometry_connector_t *this_ )
{
    const double dest_end_length
        = fabs( (*this_).main_line_destination_x - (*this_).destination_end_x )
        + fabs( (*this_).main_line_destination_y - (*this_).destination_end_y );
    return dest_end_length;
}

static inline bool geometry_connector_is_close ( const geometry_connector_t *this_, double x, double y, double max_distance )
{
    const bool close_to_source_end_line
        = ((( (*this_).source_end_x - max_distance < x ) && ( x < (*this_).main_line_source_x + max_distance ))
        || (( (*this_).main_line_source_x - max_distance < x ) && ( x < (*this_).source_end_x + max_distance )))
        && ((( (*this_).source_end_y - max_distance < y ) && ( y < (*this_).main_line_source_y + max_distance ))
        || (( (*this_).main_line_source_y - max_distance < y ) && ( y < (*this_).source_end_y + max_distance )));
    const bool close_to_main_line
        = ((( (*this_).main_line_destination_x - max_distance < x ) && ( x < (*this_).main_line_source_x + max_distance ))
        || (( (*this_).main_line_source_x - max_distance < x ) && ( x < (*this_).main_line_destination_x + max_distance )))
        && ((( (*this_).main_line_destination_y - max_distance < y ) && ( y < (*this_).main_line_source_y + max_distance ))
        || (( (*this_).main_line_source_y - max_distance < y ) && ( y < (*this_).main_line_destination_y + max_distance )));
    const bool close_to_destination_end_line
        = ((( (*this_).main_line_destination_x - max_distance < x ) && ( x < (*this_).destination_end_x + max_distance ))
        || (( (*this_).destination_end_x - max_distance < x ) && ( x < (*this_).main_line_destination_x + max_distance )))
        && ((( (*this_).main_line_destination_y - max_distance < y ) && ( y < (*this_).destination_end_y + max_distance ))
        || (( (*this_).destination_end_y - max_distance < y ) && ( y < (*this_).main_line_destination_y + max_distance )));
    return ( close_to_source_end_line || close_to_main_line || close_to_destination_end_line );
}

static inline bool geometry_connector_is_intersecting_rectangle ( const geometry_connector_t *this_,
                                                                  const geometry_rectangle_t *rect )
{
    geometry_rectangle_t source;
    geometry_rectangle_t main_line;
    geometry_rectangle_t destination;
    geometry_rectangle_init_by_corners( &source,
                                        (*this_).source_end_x, (*this_).source_end_y,
                                        (*this_).main_line_source_x, (*this_).main_line_source_y
                                      );
    geometry_rectangle_init_by_corners( &main_line,
                                        (*this_).main_line_source_x, (*this_).main_line_source_y,
                                        (*this_).main_line_destination_x, (*this_).main_line_destination_y
                                      );
    geometry_rectangle_init_by_corners( &destination,
                                        (*this_).main_line_destination_x, (*this_).main_line_destination_y,
                                        (*this_).destination_end_x, (*this_).destination_end_y
                                      );

    const bool result
        = geometry_rectangle_is_intersecting( &source, rect )
        || geometry_rectangle_is_intersecting( &main_line, rect )
        || geometry_rectangle_is_intersecting( &destination, rect );

    geometry_rectangle_destroy( &source );
    geometry_rectangle_destroy( &main_line );
    geometry_rectangle_destroy( &destination );

    return result;
}

static inline double geometry_connector_get_transit_length ( const geometry_connector_t *this_,
                                                             const geometry_rectangle_t *rect )
{
    double result = 0.0;

    geometry_rectangle_t source;
    geometry_rectangle_t main_line;
    geometry_rectangle_t destination;
    geometry_rectangle_init_by_corners( &source,
                                        (*this_).source_end_x, (*this_).source_end_y,
                                        (*this_).main_line_source_x, (*this_).main_line_source_y
                                      );
    geometry_rectangle_init_by_corners( &main_line,
                                        (*this_).main_line_source_x, (*this_).main_line_source_y,
                                        (*this_).main_line_destination_x, (*this_).main_line_destination_y
                                      );
    geometry_rectangle_init_by_corners( &destination,
                                        (*this_).main_line_destination_x, (*this_).main_line_destination_y,
                                        (*this_).destination_end_x, (*this_).destination_end_y
                                      );
    const int src_err = geometry_rectangle_init_by_intersect( &source, &source, rect );
    const int main_err = geometry_rectangle_init_by_intersect( &main_line, &main_line, rect );
    const int dst_err = geometry_rectangle_init_by_intersect( &destination, &destination, rect );
    if ( src_err == 0 )
    {
        result += geometry_rectangle_get_width( &source ) + geometry_rectangle_get_height( &source );
    }
    if ( main_err == 0 )
    {
        result += geometry_rectangle_get_width( &main_line ) + geometry_rectangle_get_height( &main_line );
    }
    if ( dst_err == 0 )
    {
        result += geometry_rectangle_get_width( &destination ) + geometry_rectangle_get_height( &destination );
    }
    geometry_rectangle_destroy( &source );
    geometry_rectangle_destroy( &main_line );
    geometry_rectangle_destroy( &destination );

    return result;
}

static inline uint32_t geometry_connector_count_connector_intersects ( const geometry_connector_t *this_,
                                                                       const geometry_connector_t *that )
{
    uint32_t result = 0;

    /* do some simple prechecks */
    geometry_rectangle_t this_bounds;
    geometry_rectangle_t that_bounds;

    this_bounds = geometry_connector_get_bounding_rectangle( this_ );
    that_bounds = geometry_connector_get_bounding_rectangle( that );

    if ( geometry_rectangle_is_contiguous( &this_bounds, &that_bounds ) )
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

        if ( geometry_rectangle_is_contiguous( &this_source_end, &that_source_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_main_line, &that_source_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_destination_end, &that_source_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_source_end, &that_main_line ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_main_line, &that_main_line ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_destination_end, &that_main_line ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_source_end, &that_destination_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_main_line, &that_destination_end ) )
        {
            result += 1;
        }
        if ( geometry_rectangle_is_contiguous( &this_destination_end, &that_destination_end ) )
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

static inline double geometry_connector_get_same_path_length_rect( const geometry_connector_t *this_,
                                                                   const geometry_rectangle_t *rect,
                                                                   double max_distance )
{
    double result = 0.0;

    const double left = geometry_rectangle_get_left( rect );
    const double top = geometry_rectangle_get_top( rect );
    const double right = geometry_rectangle_get_right( rect );
    const double bottom = geometry_rectangle_get_bottom( rect );
    const u8_interval_t x_length = u8_interval_new( left, right );
    const u8_interval_t y_length = u8_interval_new( top, bottom );

    /* check same path with left side of rectangle */
    {
        const double left_lo = left - max_distance;
        const double left_hi = left + max_distance;

        const bool source_end_is_on_left_side
            = ( left_lo < (*this_).source_end_x )&&( (*this_).source_end_x < left_hi );
        const bool main_line_source_is_on_left_side
            = ( left_lo < (*this_).main_line_source_x )&&( (*this_).main_line_source_x < left_hi );
        const bool main_line_end_is_on_left_side
            = ( left_lo < (*this_).main_line_destination_x )&&( (*this_).main_line_destination_x < left_hi );
        const bool destination_end_is_on_left_side
            = ( left_lo < (*this_).destination_end_x )&&( (*this_).destination_end_x < left_hi );
        if ( source_end_is_on_left_side && main_line_source_is_on_left_side )
        {
            const u8_interval_t section_y = u8_interval_new_unordered( (*this_).source_end_y, (*this_).main_line_source_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &y_length, &section_y );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_source_is_on_left_side && main_line_end_is_on_left_side )
        {
            const u8_interval_t section_y = u8_interval_new_unordered( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &y_length, &section_y );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_end_is_on_left_side && destination_end_is_on_left_side )
        {
            const u8_interval_t section_y = u8_interval_new_unordered( (*this_).main_line_destination_y, (*this_).destination_end_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &y_length, &section_y );
            result += u8_interval_get_size( &intersect );
        }
    }

    /* check same path with top side of rectangle */
    {
        const double top_lo = top - max_distance;
        const double top_hi = top + max_distance;

        const bool source_end_is_on_top_side
            = ( top_lo < (*this_).source_end_y )&&( (*this_).source_end_y < top_hi );
        const bool main_line_source_is_on_top_side
            = ( top_lo < (*this_).main_line_source_y )&&( (*this_).main_line_source_y < top_hi );
        const bool main_line_end_is_on_top_side
            = ( top_lo < (*this_).main_line_destination_y )&&( (*this_).main_line_destination_y < top_hi );
        const bool destination_end_is_on_top_side
            = ( top_lo < (*this_).destination_end_y )&&( (*this_).destination_end_y < top_hi );
        if ( source_end_is_on_top_side && main_line_source_is_on_top_side )
        {
            const u8_interval_t section_x = u8_interval_new_unordered( (*this_).source_end_x, (*this_).main_line_source_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &x_length, &section_x );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_source_is_on_top_side && main_line_end_is_on_top_side )
        {
            const u8_interval_t section_x = u8_interval_new_unordered( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &x_length, &section_x );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_end_is_on_top_side && destination_end_is_on_top_side )
        {
            const u8_interval_t section_x = u8_interval_new_unordered( (*this_).main_line_destination_x, (*this_).destination_end_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &x_length, &section_x );
            result += u8_interval_get_size( &intersect );
        }
    }
    /* check same path with right side of rectangle */
    {
        const double right_lo = right - max_distance;
        const double right_hi = right + max_distance;

        const bool source_end_is_on_right_side
            = ( right_lo < (*this_).source_end_x )&&( (*this_).source_end_x < right_hi );
        const bool main_line_source_is_on_right_side
            = ( right_lo < (*this_).main_line_source_x )&&( (*this_).main_line_source_x < right_hi );
        const bool main_line_end_is_on_right_side
            = ( right_lo < (*this_).main_line_destination_x )&&( (*this_).main_line_destination_x < right_hi );
        const bool destination_end_is_on_right_side
            = ( right_lo < (*this_).destination_end_x )&&( (*this_).destination_end_x < right_hi );
        if ( source_end_is_on_right_side && main_line_source_is_on_right_side )
        {
            const u8_interval_t section_y = u8_interval_new_unordered( (*this_).source_end_y, (*this_).main_line_source_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &y_length, &section_y );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_source_is_on_right_side && main_line_end_is_on_right_side )
        {
            const u8_interval_t section_y = u8_interval_new_unordered( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &y_length, &section_y );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_end_is_on_right_side && destination_end_is_on_right_side )
        {
            const u8_interval_t section_y = u8_interval_new_unordered( (*this_).main_line_destination_y, (*this_).destination_end_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &y_length, &section_y );
            result += u8_interval_get_size( &intersect );
        }
    }
    /* check same path with bottom side of rectangle */
    {
        const double bottom_lo = bottom - max_distance;
        const double bottom_hi = bottom + max_distance;

        const bool source_end_is_on_bottom_side
            = ( bottom_lo < (*this_).source_end_y )&&( (*this_).source_end_y < bottom_hi );
        const bool main_line_source_is_on_bottom_side
            = ( bottom_lo < (*this_).main_line_source_y )&&( (*this_).main_line_source_y < bottom_hi );
        const bool main_line_end_is_on_bottom_side
            = ( bottom_lo < (*this_).main_line_destination_y )&&( (*this_).main_line_destination_y < bottom_hi );
        const bool destination_end_is_on_bottom_side
            = ( bottom_lo < (*this_).destination_end_y )&&( (*this_).destination_end_y < bottom_hi );
        if ( source_end_is_on_bottom_side && main_line_source_is_on_bottom_side )
        {
            const u8_interval_t section_x = u8_interval_new_unordered( (*this_).source_end_x, (*this_).main_line_source_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &x_length, &section_x );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_source_is_on_bottom_side && main_line_end_is_on_bottom_side )
        {
            const u8_interval_t section_x = u8_interval_new_unordered( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &x_length, &section_x );
            result += u8_interval_get_size( &intersect );
        }
        if ( main_line_end_is_on_bottom_side && destination_end_is_on_bottom_side )
        {
            const u8_interval_t section_x = u8_interval_new_unordered( (*this_).main_line_destination_x, (*this_).destination_end_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &x_length, &section_x );
            result += u8_interval_get_size( &intersect );
        }
    }

    return result;
}

static inline double geometry_connector_get_same_path_length_conn( const geometry_connector_t *this_,
                                                                   const geometry_connector_t *that,
                                                                   double max_distance )
{
    double result = 0.0;

    const double source_x = (*that).main_line_source_x;
    const double source_y = (*that).main_line_source_y;
    const double destination_x = (*that).main_line_destination_x;
    const double destination_y = (*that).main_line_destination_y;

    const double source_x_lo = source_x - max_distance;
    const double source_x_hi = source_x + max_distance;
    const double source_y_lo = source_y - max_distance;
    const double source_y_hi = source_y + max_distance;
    const double destination_x_lo = destination_x - max_distance;
    const double destination_x_hi = destination_x + max_distance;
    const double destination_y_lo = destination_y - max_distance;
    const double destination_y_hi = destination_y + max_distance;
    const bool source_x_on_source_x
        = ( source_x_lo < (*this_).main_line_source_x )&&( (*this_).main_line_source_x < source_x_hi );
    const bool source_y_on_source_y
        = ( source_y_lo < (*this_).main_line_source_y )&&( (*this_).main_line_source_y < source_y_hi );
    const bool destination_x_on_source_x
        = ( source_x_lo < (*this_).main_line_destination_x )&&( (*this_).main_line_destination_x < source_x_hi );
    const bool destination_y_on_source_y
        = ( source_y_lo < (*this_).main_line_destination_y )&&( (*this_).main_line_destination_y < source_y_hi );
    const bool source_x_on_destination_x
        = ( destination_x_lo < (*this_).main_line_source_x )&&( (*this_).main_line_source_x < destination_x_hi );
    const bool source_y_on_destination_y
        = ( destination_y_lo < (*this_).main_line_source_y )&&( (*this_).main_line_source_y < destination_y_hi );
    const bool destination_x_on_destination_x
        = ( destination_x_lo < (*this_).main_line_destination_x )&&( (*this_).main_line_destination_x < destination_x_hi );
    const bool destination_y_on_destination_y
        = ( destination_y_lo < (*this_).main_line_destination_y )&&( (*this_).main_line_destination_y < destination_y_hi );

    const geometry_3dir_t this_dir = geometry_connector_get_directions( this_ );
    const geometry_3dir_t that_dir = geometry_connector_get_directions( that );

    /* check same path with source segment of that */
    if ( geometry_3dir_is_first_v( &that_dir ) )
    {
        const u8_interval_t source_dy = u8_interval_new_unordered( (*that).source_end_y, (*that).main_line_source_y );

        if ( geometry_3dir_is_first_v( &this_dir ) && source_x_on_source_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).source_end_y, (*this_).main_line_source_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &source_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_second_v( &this_dir ) && source_x_on_source_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &source_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_third_v( &this_dir ) && destination_x_on_source_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).main_line_destination_y, (*this_).destination_end_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &source_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
    }
    else /* geometry_3dir_is_first_h( &that_dir ) */
    {
        const u8_interval_t source_dx = u8_interval_new_unordered( (*that).source_end_x, (*that).main_line_source_x );

        if ( geometry_3dir_is_first_h( &this_dir ) && source_y_on_source_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).source_end_x, (*this_).main_line_source_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &source_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_second_h( &this_dir ) && source_y_on_source_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &source_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_third_h( &this_dir ) && destination_y_on_source_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).main_line_destination_x, (*this_).destination_end_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &source_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
    }

    /* check same path with main segment of that */
    if ( geometry_3dir_is_second_v( &that_dir ) )
    {
        const u8_interval_t main_dy = u8_interval_new_unordered( (*that).main_line_source_y, (*that).main_line_destination_y );

        if ( geometry_3dir_is_first_v( &this_dir ) && source_x_on_source_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).source_end_y, (*this_).main_line_source_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &main_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_second_v( &this_dir ) && source_x_on_source_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &main_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_third_v( &this_dir ) && destination_x_on_source_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).main_line_destination_y, (*this_).destination_end_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &main_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
    }
    else /* geometry_3dir_is_second_h( &that_dir ) */
    {
        const u8_interval_t main_dx = u8_interval_new_unordered( (*that).main_line_source_x, (*that).main_line_destination_x );

        if ( geometry_3dir_is_first_h( &this_dir ) && source_y_on_source_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).source_end_x, (*this_).main_line_source_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &main_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_second_h( &this_dir ) && source_y_on_source_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &main_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_third_h( &this_dir ) && destination_y_on_source_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).main_line_destination_x, (*this_).destination_end_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &main_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
    }

    /* check same path with destination segment of that */
    if ( geometry_3dir_is_third_v( &that_dir ) )
    {
        const u8_interval_t destination_dy = u8_interval_new_unordered( (*that).main_line_destination_y, (*that).destination_end_y );

        if ( geometry_3dir_is_first_v( &this_dir ) && source_x_on_destination_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).source_end_y, (*this_).main_line_source_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &destination_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_second_v( &this_dir ) && source_x_on_destination_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &destination_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_third_v( &this_dir ) && destination_x_on_destination_x )
        {
            const u8_interval_t section_dy = u8_interval_new_unordered( (*this_).main_line_destination_y, (*this_).destination_end_y );
            const u8_interval_t intersect = u8_interval_new_intersect( &destination_dy, &section_dy );
            result += u8_interval_get_size( &intersect );
        }
    }
    else /* geometry_3dir_is_third_h( &that_dir ) */
    {
        const u8_interval_t destination_dx = u8_interval_new_unordered( (*that).main_line_destination_x, (*that).destination_end_x );

        if ( geometry_3dir_is_first_h( &this_dir ) && source_y_on_destination_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).source_end_x, (*this_).main_line_source_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &destination_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_second_h( &this_dir ) && source_y_on_destination_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &destination_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
        if ( geometry_3dir_is_third_h( &this_dir ) && destination_y_on_destination_y )
        {
            const u8_interval_t section_dx = u8_interval_new_unordered( (*this_).main_line_destination_x, (*this_).destination_end_x );
            const u8_interval_t intersect = u8_interval_new_intersect( &destination_dx, &section_dx );
            result += u8_interval_get_size( &intersect );
        }
    }

    return result;
}

static inline geometry_rectangle_t geometry_connector_get_bounding_rectangle ( const geometry_connector_t *this_ )
{
    geometry_rectangle_t result;

    const double left = fmin ( (*this_).main_line_source_x, fmin( (*this_).source_end_x, (*this_).destination_end_x ) );
    const double right = fmax ( (*this_).main_line_source_x, fmax( (*this_).source_end_x, (*this_).destination_end_x ) );
    const double top = fmin ( (*this_).main_line_source_y, fmin( (*this_).source_end_y, (*this_).destination_end_y ) );
    const double bottom = fmax ( (*this_).main_line_source_y, fmax( (*this_).source_end_y, (*this_).destination_end_y ) );

    geometry_rectangle_init( &result,
                             left,
                             top,
                             right - left,
                             bottom - top
                           );

    return result;
}

static inline geometry_rectangle_t geometry_connector_get_segment_bounds ( const geometry_connector_t *this_,
                                                                           geometry_connector_segment_t segment_id )
{
    assert( ( segment_id == GEOMETRY_CONNECTOR_SEGMENT_SOURCE )
        ||( segment_id == GEOMETRY_CONNECTOR_SEGMENT_MAIN )||( segment_id == GEOMETRY_CONNECTOR_SEGMENT_DESTINATION ) );
    geometry_rectangle_t result;

    switch ( segment_id )
    {
        case GEOMETRY_CONNECTOR_SEGMENT_SOURCE:
        {
            const double left = fmin ( (*this_).main_line_source_x, (*this_).source_end_x );
            const double right = fmax ( (*this_).main_line_source_x, (*this_).source_end_x );
            const double top = fmin ( (*this_).main_line_source_y, (*this_).source_end_y );
            const double bottom = fmax ( (*this_).main_line_source_y, (*this_).source_end_y );
            geometry_rectangle_init( &result, left, top, right - left, bottom - top );
        }
        break;

        case GEOMETRY_CONNECTOR_SEGMENT_MAIN:
        default:
        {
            const double left = fmin ( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const double right = fmax ( (*this_).main_line_source_x, (*this_).main_line_destination_x );
            const double top = fmin ( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            const double bottom = fmax ( (*this_).main_line_source_y, (*this_).main_line_destination_y );
            geometry_rectangle_init( &result, left, top, right - left, bottom - top );
        }
        break;

        case GEOMETRY_CONNECTOR_SEGMENT_DESTINATION:
        {
            const double left = fmin ( (*this_).destination_end_x, (*this_).main_line_destination_x );
            const double right = fmax ( (*this_).destination_end_x, (*this_).main_line_destination_x );
            const double top = fmin ( (*this_).destination_end_y, (*this_).main_line_destination_y );
            const double bottom = fmax ( (*this_).destination_end_y, (*this_).main_line_destination_y );
            geometry_rectangle_init( &result, left, top, right - left, bottom - top );
        }
        break;
    }

    return result;
}

static inline geometry_3dir_t geometry_connector_get_directions ( const geometry_connector_t *this_ )
{
    const double very_small = 0.000000001;

    const double src_dx = (*this_).main_line_source_x - (*this_).source_end_x;
    const double src_dy = (*this_).main_line_source_y - (*this_).source_end_y;
    const double abs_src_dx = fabs( src_dx );
    const double abs_src_dy = fabs( src_dy );
    const bool src_exists = (( abs_src_dx > very_small )||( abs_src_dy > very_small ));
    const bool src_is_horizontal = ( abs_src_dx > abs_src_dy );

    const geometry_direction_t src_dir
       = ( ! src_exists )
       ? GEOMETRY_DIRECTION_CENTER
       : src_is_horizontal
       ? (( src_dx > 0.0 ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_LEFT )
       : (( src_dy > 0.0 ) ? GEOMETRY_DIRECTION_DOWN : GEOMETRY_DIRECTION_UP );

    const double main_dx = (*this_).main_line_destination_x - (*this_).main_line_source_x;
    const double main_dy = (*this_).main_line_destination_y - (*this_).main_line_source_y;
    const double abs_main_dx = fabs( main_dx );
    const double abs_main_dy = fabs( main_dy );
    const bool main_exists = (( abs_main_dx > very_small )||( abs_main_dy > very_small ));
    const bool main_is_horizontal = ( abs_main_dx > abs_main_dy );

    const geometry_direction_t main_dir
       = ( ! main_exists )
       ? GEOMETRY_DIRECTION_CENTER
       : main_is_horizontal
       ? (( main_dx > 0.0 ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_LEFT )
       : (( main_dy > 0.0 ) ? GEOMETRY_DIRECTION_DOWN : GEOMETRY_DIRECTION_UP );

    const double dst_dx = (*this_).destination_end_x - (*this_).main_line_destination_x;
    const double dst_dy = (*this_).destination_end_y - (*this_).main_line_destination_y;
    const double abs_dst_dx = fabs( dst_dx );
    const double abs_dst_dy = fabs( dst_dy );
    const bool dst_exists = (( abs_dst_dx > very_small )||( abs_dst_dy > very_small ));
    const bool dst_is_horizontal = ( abs_dst_dx > abs_dst_dy );

    const geometry_direction_t dst_dir
       = ( ! dst_exists )
       ? GEOMETRY_DIRECTION_CENTER
       : dst_is_horizontal
       ? (( dst_dx > 0.0 ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_LEFT )
       : (( dst_dy > 0.0 ) ? GEOMETRY_DIRECTION_DOWN : GEOMETRY_DIRECTION_UP );

    const geometry_3dir_t result = geometry_3dir_new( src_dir, main_dir, dst_dir );
    return result;
}

static inline void geometry_connector_trace ( const geometry_connector_t *this_ )
{
    static const char geometry_mnemomic[GEOMETRY_DIRECTION_MAX]
        = { '.', '<', 'F', '^', '7', '>', 'J', 'v', 'L' };

    const geometry_3dir_t pattern = geometry_connector_get_directions( this_ );
    const char pattern_as_str[6]
        = { [0] = geometry_mnemomic[pattern.first],
            [1] = ' ',
            [2] = geometry_mnemomic[pattern.second],
            [3] = ' ',
            [4] = geometry_mnemomic[pattern.third],
            [5] = '\0' };

    U8_TRACE_INFO( "geometry_connector_t" );
    U8_TRACE_INFO_INT( "- source_end_x:", (*this_).source_end_x );
    U8_TRACE_INFO_INT( "- source_end_y:", (*this_).source_end_y );
    U8_TRACE_INFO_INT( "- main_line_source_x:", (*this_).main_line_source_x );
    U8_TRACE_INFO_INT( "- main_line_source_y:", (*this_).main_line_source_y );
    U8_TRACE_INFO_INT( "- main_line_destination_x:", (*this_).main_line_destination_x );
    U8_TRACE_INFO_INT( "- main_line_destination_y:", (*this_).main_line_destination_y );
    U8_TRACE_INFO_INT( "- destination_end_x:", (*this_).destination_end_x );
    U8_TRACE_INFO_INT( "- destination_end_y:", (*this_).destination_end_y );
    U8_TRACE_INFO_STR( "- get_directions:", pattern_as_str );
}


/*
Copyright 2017-2025 Andreas Warnke

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
