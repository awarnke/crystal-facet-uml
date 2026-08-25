/* File: geometry_compartments.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "u8/u8_f64.h"
#include <assert.h>
#include <math.h>

static inline void geometry_compartments_init( geometry_compartments_t *this_,
                                               double standard_object_border,
                                               double preferred_object_distance )
{
    geometry_dimensions_init( &((*this_).feature_compartments), 0.0, 0.0 );
    (*this_).has_properties = false;
    (*this_).has_operations = false;
    (*this_).has_tagged_values = false;
    (*this_).port_height_on_left = 0.0;
    (*this_).port_height_on_right = 0.0;
    (*this_).port_width_on_top = 0.0;
    (*this_).port_width_on_bottom = 0.0;
    (*this_).iface_height_on_left = 0.0;
    (*this_).iface_height_on_right = 0.0;
    (*this_).iface_width_on_top = 0.0;
    (*this_).iface_width_on_bottom = 0.0;
    (*this_).standard_object_border = standard_object_border;
    (*this_).preferred_object_distance = preferred_object_distance;
}

static inline void geometry_compartments_reinit( geometry_compartments_t *this_,
                                                 double standard_object_border,
                                                 double preferred_object_distance )
{
    geometry_dimensions_destroy( &((*this_).feature_compartments) );
    geometry_compartments_init( this_, standard_object_border, preferred_object_distance );
}

static inline void geometry_compartments_copy ( geometry_compartments_t *this_, const geometry_compartments_t *original )
{
    *this_ = *original;
    geometry_dimensions_copy( &((*this_).feature_compartments), &((*original).feature_compartments) );
}

static inline void geometry_compartments_move ( geometry_compartments_t *this_, geometry_compartments_t *that )
{
    *this_ = *that;
    geometry_dimensions_copy( &((*this_).feature_compartments), &((*that).feature_compartments) );
    geometry_compartments_destroy( that );
}

static inline void geometry_compartments_replace ( geometry_compartments_t *this_, const geometry_compartments_t *original )
{
    geometry_dimensions_destroy( &((*this_).feature_compartments) );
    *this_ = *original;
    geometry_dimensions_copy( &((*this_).feature_compartments), &((*original).feature_compartments) );
}

static inline void geometry_compartments_replacemove ( geometry_compartments_t *this_, geometry_compartments_t *that )
{
    geometry_dimensions_destroy( &((*this_).feature_compartments) );
    *this_ = *that;
    geometry_dimensions_copy( &((*this_).feature_compartments), &((*that).feature_compartments) );
    geometry_compartments_destroy( that );
}

static inline geometry_compartments_t geometry_compartments_new_empty()
{
    geometry_compartments_t result;
    geometry_compartments_init( &result, 0.0, 0.0 );
    return result;
}

static inline geometry_compartments_t geometry_compartments_new( const geometry_dimensions_t *compartments,
                                                                 const geometry_dimensions_t *outer )
{
    geometry_compartments_t result;
    geometry_compartments_init( &result, 0.0, 0.0 );
    geometry_dimensions_replace( &(result.feature_compartments), compartments );
    result.port_height_on_left = geometry_dimensions_get_height( outer );
    result.port_width_on_top = geometry_dimensions_get_width( outer );
    return result;
}

static inline void geometry_compartments_destroy( geometry_compartments_t *this_ )
{
    geometry_dimensions_destroy( &((*this_).feature_compartments) );
}

static inline void geometry_compartments_add_feature( geometry_compartments_t *this_,
                                                      geometry_compartment_type_t compartment,
                                                      const geometry_dimensions_t * feature_dim )
{
    const double gap = (*this_).preferred_object_distance;
    switch ( compartment )
    {
        case GEOMETRY_COMPARTMENT_TYPE_PROPERTIES:
        {
            /* update dimensions of feature_compartments */
            double f_width = geometry_dimensions_get_width( &((*this_).feature_compartments) );
            double f_height = geometry_dimensions_get_height( &((*this_).feature_compartments) );
            if ( ! ( (*this_).has_properties || (*this_).has_operations || (*this_).has_tagged_values ) )
            {
                f_height += 2.0 * (*this_).standard_object_border;  /* gaps above and below the bottom compartment line */
            }
            if ( ! (*this_).has_properties )
            {
                (*this_).has_properties = true;
                f_height += 4.0 * (*this_).standard_object_border;  /* gaps above and below the new top compartment line */
                /* 2.0 * gap is reserved for compartment label */
            }

            const double new_feat_width
                = geometry_dimensions_get_width( feature_dim ) + 2.0 * (*this_).standard_object_border;
            f_width = u8_f64_max2( f_width, new_feat_width );
            f_height += geometry_dimensions_get_height( feature_dim );
            geometry_dimensions_reinit( &((*this_).feature_compartments), f_width, f_height );
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_OPERATIONS:
        {
            /* update dimensions of feature_compartments */
            double f_width = geometry_dimensions_get_width( &((*this_).feature_compartments) );
            double f_height = geometry_dimensions_get_height( &((*this_).feature_compartments) );
            if ( ! ( (*this_).has_properties || (*this_).has_operations || (*this_).has_tagged_values ) )
            {
                f_height += 2.0 * (*this_).standard_object_border;  /* gaps above and below the bottom compartment line */
            }
            if ( ! (*this_).has_operations )
            {
                (*this_).has_operations = true;
                f_height += 4.0 * (*this_).standard_object_border;  /* gaps above and below the new top compartment line */
                /* 2.0 * gap is reserved for compartment label */
            }

            const double new_feat_width
                = geometry_dimensions_get_width( feature_dim ) + 2.0 * (*this_).standard_object_border;
            f_width = u8_f64_max2( f_width, new_feat_width );
            f_height += geometry_dimensions_get_height( feature_dim );
            geometry_dimensions_reinit( &((*this_).feature_compartments), f_width, f_height );
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_TAGGED_VALUES:
        {
            /* update dimensions of feature_compartments */
            double f_width = geometry_dimensions_get_width( &((*this_).feature_compartments) );
            double f_height = geometry_dimensions_get_height( &((*this_).feature_compartments) );
            if ( ! ( (*this_).has_properties || (*this_).has_operations || (*this_).has_tagged_values ) )
            {
                f_height += 2.0 * (*this_).standard_object_border;  /* gaps above and below the bottom compartment line */
            }
            if ( ! (*this_).has_tagged_values )
            {
                (*this_).has_tagged_values = true;
                f_height += 4.0 * (*this_).standard_object_border;  /* gaps above and below the new top compartment line */
                /* 2.0 * gap is reserved for compartment label */
            }

            const double new_feat_width
                = geometry_dimensions_get_width( feature_dim ) + 2.0 * (*this_).standard_object_border;
            f_width = u8_f64_max2( f_width, new_feat_width );
            f_height += geometry_dimensions_get_height( feature_dim );
            geometry_dimensions_reinit( &((*this_).feature_compartments), f_width, f_height );
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_PORT_ON_LEFT:
        {
            (*this_).port_height_on_left += geometry_dimensions_get_height( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_PORT_ON_RIGHT:
        {
            (*this_).port_height_on_right += geometry_dimensions_get_height( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_PORT_ON_TOP:
        {
            (*this_).port_width_on_top += geometry_dimensions_get_width( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_PORT_ON_BOTTOM:
        {
            (*this_).port_width_on_bottom += geometry_dimensions_get_width( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT:
        {
            (*this_).iface_height_on_left += geometry_dimensions_get_height( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_IF_ON_RIGHT:
        {
            (*this_).iface_height_on_right += geometry_dimensions_get_height( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_IF_ON_TOP:
        {
            (*this_).iface_width_on_top += geometry_dimensions_get_width( feature_dim ) + gap;
        }
        break;

        case GEOMETRY_COMPARTMENT_TYPE_IF_ON_BOTTOM:
        {
            (*this_).iface_width_on_bottom += geometry_dimensions_get_width( feature_dim ) + gap;
        }
        break;

        default:  /* or */
        case GEOMETRY_COMPARTMENT_TYPE_VOID:
        {
            assert( compartment == GEOMETRY_COMPARTMENT_TYPE_VOID );
            /* ignore */
        }
        break;
    }
}

static inline const geometry_dimensions_t * geometry_compartments_get_feature_compartments ( const geometry_compartments_t *this_ )
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
    return (*this_).iface_height_on_left;
}

static inline double geometry_compartments_get_if_height_on_right( const geometry_compartments_t *this_ )
{
    return (*this_).iface_height_on_right;
}

static inline double geometry_compartments_get_if_width_on_top( const geometry_compartments_t *this_ )
{
    return (*this_).iface_width_on_top;
}

static inline double geometry_compartments_get_if_width_on_bottom( const geometry_compartments_t *this_ )
{
    return (*this_).iface_width_on_bottom;
}

static inline double geometry_compartments_get_outer_height ( const geometry_compartments_t *this_ )
{
    const double iface_height = u8_f64_max2( (*this_).iface_height_on_left, (*this_).iface_height_on_right );
    const double port_height = u8_f64_max2( (*this_).port_height_on_left, (*this_).port_height_on_right );
    const double result = u8_f64_max2( iface_height, port_height );
    return result;
}

static inline double geometry_compartments_get_outer_width ( const geometry_compartments_t *this_ )
{
    const double iface_width = u8_f64_max2( (*this_).iface_width_on_top, (*this_).iface_width_on_bottom );
    const double port_width = u8_f64_max2( (*this_).port_width_on_top, (*this_).port_width_on_bottom );
    const double result = u8_f64_max2( iface_width, port_width );
    return result;
}

static inline void geometry_compartments_trace( const geometry_compartments_t *this_ )
{
    U8_TRACE_INFO( "geometry_compartments_t" );
    geometry_dimensions_trace( &((*this_).feature_compartments) );
    U8_TRACE_INFO_INT( "- has_properties:", (int)(*this_).has_properties );
    U8_TRACE_INFO_INT( "- has_operations:", (int)(*this_).has_operations );
    U8_TRACE_INFO_INT( "- has_tagged_values:", (int)(*this_).has_tagged_values );
    U8_TRACE_INFO_INT( "- port_height_on_left:", (*this_).port_height_on_left );
    U8_TRACE_INFO_INT( "- port_height_on_right:", (*this_).port_height_on_right );
    U8_TRACE_INFO_INT( "- port_width_on_top:", (*this_).port_width_on_top );
    U8_TRACE_INFO_INT( "- port_width_on_bottom:", (*this_).port_width_on_bottom );
    U8_TRACE_INFO_INT( "- iface_height_on_left:", (*this_).iface_height_on_left );
    U8_TRACE_INFO_INT( "- iface_height_on_right:", (*this_).iface_height_on_right );
    U8_TRACE_INFO_INT( "- iface_width_on_top:", (*this_).iface_width_on_top );
    U8_TRACE_INFO_INT( "- iface_width_on_bottom:", (*this_).iface_width_on_bottom );
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
