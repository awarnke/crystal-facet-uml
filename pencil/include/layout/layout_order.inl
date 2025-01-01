/* File: layout_order.inl; Copyright and License: see below */

#include "u8/u8_trace.h"

static inline void layout_order_private_init ( layout_order_t *this_, layout_order_type_t order_type, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
    (*this_).order_type = order_type;
}


static inline void layout_order_private_reinit ( layout_order_t *this_, layout_order_type_t order_type, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
    (*this_).order_type = order_type;
}

static inline void layout_order_init_x_y ( layout_order_t *this_, int32_t x_order, int32_t y_order )
{
    (*this_).first = x_order;
    (*this_).second = y_order;
    (*this_).order_type = PENCIL_LAYOUT_ORDER_TYPE_X_Y;
}

static inline void layout_order_reinit_x_y ( layout_order_t *this_, int32_t x_order, int32_t y_order )
{
    (*this_).first = x_order;
    (*this_).second = y_order;
    (*this_).order_type = PENCIL_LAYOUT_ORDER_TYPE_X_Y;
}

static inline void layout_order_init_list ( layout_order_t *this_, int32_t list_order )
{
    (*this_).first = list_order;
    (*this_).second = 0;
    (*this_).order_type = PENCIL_LAYOUT_ORDER_TYPE_LIST;
}

static inline void layout_order_reinit_list ( layout_order_t *this_, int32_t list_order )
{
    (*this_).first = list_order;
    (*this_).second = 0;
    (*this_).order_type = PENCIL_LAYOUT_ORDER_TYPE_LIST;
}

static inline void layout_order_copy ( layout_order_t *this_, const layout_order_t *original )
{
    (*this_) = (*original);
}

static inline void layout_order_replace ( layout_order_t *this_, const layout_order_t *original )
{
    (*this_) = (*original);
}

static inline void layout_order_init_empty ( layout_order_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
    (*this_).order_type = PENCIL_LAYOUT_ORDER_TYPE_NONE;
}

static inline void layout_order_reinit_empty ( layout_order_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
    (*this_).order_type = PENCIL_LAYOUT_ORDER_TYPE_NONE;
}

static inline void layout_order_destroy ( layout_order_t *this_ )
{
}

static inline layout_order_type_t layout_order_get_order_type ( const layout_order_t *this_ )
{
    return (*this_).order_type;
}

static inline int32_t layout_order_get_first ( const layout_order_t *this_ )
{
    return (*this_).first;
}

static inline int32_t layout_order_get_second ( const layout_order_t *this_ )
{
    return (*this_).second;
}

static inline void layout_order_trace ( const layout_order_t *this_ )
{
    U8_TRACE_INFO( "layout_order_t" );
    switch ( (*this_).order_type )
    {
        case PENCIL_LAYOUT_ORDER_TYPE_NONE:
        {
            U8_TRACE_INFO( "- order_type: PENCIL_LAYOUT_ORDER_TYPE_NONE" );
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_OUT_OF_RANGE:
        {
            U8_TRACE_INFO( "- order_type: PENCIL_LAYOUT_ORDER_TYPE_OUT_OF_RANGE" );
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_X_Y:
        {
            U8_TRACE_INFO( "- order_type: PENCIL_LAYOUT_ORDER_TYPE_X_Y" );
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_LIST:
        {
            U8_TRACE_INFO( "- order_type: PENCIL_LAYOUT_ORDER_TYPE_LIST" );
        }
        break;

        default:
        {
            U8_TRACE_INFO( "- order_type: <error>" );
        }
        break;
    }
    U8_TRACE_INFO_INT( "- first:", (*this_).first );
    U8_TRACE_INFO_INT( "- second:", (*this_).second );
}


/*
Copyright 2018-2025 Andreas Warnke

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
