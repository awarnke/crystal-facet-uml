/* File: geometry_compartments.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <math.h>

static inline void geometry_compartments_init_empty( geometry_compartments_t *this_ )
{
    geometry_rectangle_init_empty( &((*this_).feature_compartments) );
    (*this_).port_height_on_left = 0.0;
    (*this_).port_height_on_right = 0.0;
    (*this_).port_width_on_top = 0.0;
    (*this_).port_width_on_bottom = 0.0;
    (*this_).if_height_on_left = 0.0;
    (*this_).if_height_on_right = 0.0;
    (*this_).if_width_on_top = 0.0;
    (*this_).if_width_on_bottom = 0.0;
    (*this_).inner_features_height = 0.0;
}

static inline void geometry_compartments_reinit_empty( geometry_compartments_t *this_ )
{
    geometry_compartments_init_empty( this_ );
}

static inline void geometry_compartments_copy ( geometry_compartments_t *this_, const geometry_compartments_t *original )
{
    *this_ = *original;
    geometry_rectangle_copy( &((*this_).feature_compartments), &((*original).feature_compartments) );
}

static inline void geometry_compartments_move ( geometry_compartments_t *this_, geometry_compartments_t *that )
{
    *this_ = *that;
    geometry_rectangle_copy( &((*this_).feature_compartments), &((*that).feature_compartments) );
    geometry_compartments_destroy( that );
}

static inline void geometry_compartments_replace ( geometry_compartments_t *this_, const geometry_compartments_t *original )
{
    geometry_rectangle_destroy( &((*this_).feature_compartments) );
    *this_ = *original;
    geometry_rectangle_copy( &((*this_).feature_compartments), &((*original).feature_compartments) );
}

static inline void geometry_compartments_replacemove ( geometry_compartments_t *this_, geometry_compartments_t *that )
{
    geometry_rectangle_destroy( &((*this_).feature_compartments) );
    *this_ = *that;
    geometry_rectangle_copy( &((*this_).feature_compartments), &((*that).feature_compartments) );
    geometry_compartments_destroy( that );
}

static inline void geometry_compartments_destroy( geometry_compartments_t *this_ )
{
    geometry_rectangle_destroy( &((*this_).feature_compartments) );
}

static inline void geometry_compartments_add_feature( geometry_compartments_t *this_,
                                                      geometry_compartments_type_t compartment,
                                                      const geometry_rectangle_t * feature_bounds,
                                                      double preferred_object_distance )
{
    const double gap = preferred_object_distance;
    switch ( compartment )
    {
        case GEOMETRY_COMPARTMENTS_PROPERTIES:
        {
            (*this_).inner_features_height += geometry_rectangle_get_height( feature_bounds );
        }
        break;

        case GEOMETRY_COMPARTMENTS_OPERATIONS:
        {
            (*this_).inner_features_height += geometry_rectangle_get_height( feature_bounds );
        }
        break;

        case GEOMETRY_COMPARTMENTS_TAGGED_VALUES:
        {
            (*this_).inner_features_height += geometry_rectangle_get_height( feature_bounds );
        }
        break;

        case GEOMETRY_COMPARTMENTS_PORT_ON_LEFT:
        {
            (*this_).port_height_on_left += geometry_rectangle_get_height( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_PORT_ON_RIGHT:
        {
            (*this_).port_height_on_right += geometry_rectangle_get_height( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_PORT_ON_TOP:
        {
            (*this_).port_width_on_top += geometry_rectangle_get_width( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_PORT_ON_BOTTOM:
        {
            (*this_).port_width_on_bottom += geometry_rectangle_get_width( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_IF_ON_LEFT:
        {
            (*this_).if_height_on_left += geometry_rectangle_get_height( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_IF_ON_RIGHT:
        {
            (*this_).if_height_on_right += geometry_rectangle_get_height( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_IF_ON_TOP:
        {
            (*this_).if_width_on_top += geometry_rectangle_get_width( feature_bounds ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENTS_IF_ON_BOTTOM:
        {
            (*this_).if_width_on_bottom += geometry_rectangle_get_width( feature_bounds ) + gap;
        }
        break;

        default:
        {
            assert( false );
        }
        break;
    }
}

static inline const geometry_rectangle_t * geometry_compartments_get_feature_compartments ( const geometry_compartments_t *this_ )
{
    return &((*this_).feature_compartments);
}

static inline double geometry_compartments_get_port_height_on_left( const geometry_compartments_t *this_ )
{
    return (*this_).port_height_on_left;
}

static inline double geometry_compartments_get_port_height_on_right( const geometry_compartments_t *this_ )
{
    return (*this_).port_height_on_right;
}

static inline double geometry_compartments_get_port_width_on_top( const geometry_compartments_t *this_ )
{
    return (*this_).port_width_on_top;
}

static inline double geometry_compartments_get_port_width_on_bottom( const geometry_compartments_t *this_ )
{
    return (*this_).port_width_on_bottom;
}

static inline double geometry_compartments_get_if_height_on_left( const geometry_compartments_t *this_ )
{
    return (*this_).if_height_on_left;
}

static inline double geometry_compartments_get_if_height_on_right( const geometry_compartments_t *this_ )
{
    return (*this_).if_height_on_right;
}

static inline double geometry_compartments_get_if_width_on_top( const geometry_compartments_t *this_ )
{
    return (*this_).if_width_on_top;
}

static inline double geometry_compartments_get_if_width_on_bottom( const geometry_compartments_t *this_ )
{
    return (*this_).if_width_on_bottom;
}

static inline double geometry_compartments_get_inner_features_height( const geometry_compartments_t *this_ )
{
    return (*this_).inner_features_height;
}

static inline void geometry_compartments_trace( const geometry_compartments_t *this_ )
{
    U8_TRACE_INFO( "geometry_compartments_t" );
    geometry_rectangle_trace( &((*this_).feature_compartments) );
    U8_TRACE_INFO_INT( "- port_height_on_left:", (*this_).port_height_on_left );
    U8_TRACE_INFO_INT( "- port_height_on_right:", (*this_).port_height_on_right );
    U8_TRACE_INFO_INT( "- port_width_on_top:", (*this_).port_width_on_top );
    U8_TRACE_INFO_INT( "- port_width_on_bottom:", (*this_).port_width_on_bottom );
    U8_TRACE_INFO_INT( "- if_height_on_left:", (*this_).if_height_on_left );
    U8_TRACE_INFO_INT( "- if_height_on_right:", (*this_).if_height_on_right );
    U8_TRACE_INFO_INT( "- if_width_on_top:", (*this_).if_width_on_top );
    U8_TRACE_INFO_INT( "- if_width_on_bottom:", (*this_).if_width_on_bottom );
    U8_TRACE_INFO_INT( "- inner_features_height:", (*this_).inner_features_height );
}


/*
 * Copyright 2026-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
