/* File: geometry_connector.inl; Copyright and License: see below */

#include "trace.h"
#include <math.h>

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

static inline double geometry_connector_get_length ( const geometry_connector_t *this_ )
{
    double length;
    length = fabs( (*this_).source_end_x - (*this_).main_line_source_x )
             + fabs( (*this_).source_end_y - (*this_).main_line_source_y )
             + fabs( (*this_).main_line_source_x - (*this_).main_line_destination_x )
             + fabs( (*this_).main_line_source_y - (*this_).main_line_destination_y )
             + fabs( (*this_).main_line_destination_x - (*this_).destination_end_x )
             + fabs( (*this_).main_line_destination_y - (*this_).destination_end_y );
    return length;
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
    /* some simple pre-calculations */
    bool result = false;

    double rect_left = geometry_rectangle_get_left(rect) + 0.001;
    double rect_right = geometry_rectangle_get_right(rect) - 0.001;
    double rect_top = geometry_rectangle_get_top(rect) + 0.001;
    double rect_bottom = geometry_rectangle_get_bottom(rect) - 0.001;

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
        result = true;
        /* more logic needed here... */
    }

    return result;
}

static inline geometry_rectangle_t geometry_connector_get_bounding_rectangle ( const geometry_connector_t *this_ )
{
    geometry_rectangle_t result;
    geometry_rectangle_init( &result,
                             fmin( (*this_).source_end_x, (*this_).destination_end_x ),
                             fmin( (*this_).source_end_y, (*this_).destination_end_y ),
                             fabs( (*this_).source_end_x - (*this_).destination_end_x ),
                             fabs( (*this_).source_end_y - (*this_).destination_end_y )
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
Copyright 2017-2017 Andreas Warnke

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
