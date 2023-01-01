/* File: shape_int_rectangle.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

#define SHAPE_INT_RECTANGLE_MAX(a,b) ((a) > (b) ? (a) : (b))
#define SHAPE_INT_RECTANGLE_MIN(a,b) ((a) < (b) ? (a) : (b))

static inline void shape_int_rectangle_init ( shape_int_rectangle_t *this_, int32_t left, int32_t top, uint32_t width, uint32_t height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void shape_int_rectangle_init_empty ( shape_int_rectangle_t *this_ )
{
    (*this_).left = 0;
    (*this_).top = 0;
    (*this_).width = 0;
    (*this_).height = 0;
}

static inline void shape_int_rectangle_copy ( shape_int_rectangle_t *this_, const shape_int_rectangle_t *that )
{
    assert( that != NULL );
    *this_ = *that;
}

static inline void shape_int_rectangle_replace ( shape_int_rectangle_t *this_, const shape_int_rectangle_t *that )
{
    assert( that != NULL );
    *this_ = *that;
}

static inline void shape_int_rectangle_reinit ( shape_int_rectangle_t *this_, int32_t left, int32_t top, uint32_t width, uint32_t height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void shape_int_rectangle_init_by_bounds ( shape_int_rectangle_t *this_,
                                                        const shape_int_rectangle_t *rect_a,
                                                        const shape_int_rectangle_t *rect_b )
{
    assert( NULL != rect_a );
    assert( NULL != rect_b );

    const int32_t rect_a_right = (*rect_a).left + (*rect_a).width;
    const int32_t rect_a_bottom = (*rect_a).top + (*rect_a).height;
    const int32_t rect_b_right = (*rect_b).left + (*rect_b).width;
    const int32_t rect_b_bottom = (*rect_b).top + (*rect_b).height;

    (*this_).left = SHAPE_INT_RECTANGLE_MIN( (*rect_a).left, (*rect_b).left );
    (*this_).top = SHAPE_INT_RECTANGLE_MIN( (*rect_a).top, (*rect_b).top );
    (*this_).width = SHAPE_INT_RECTANGLE_MAX( rect_a_right, rect_b_right ) - (*this_).left;
    (*this_).height = SHAPE_INT_RECTANGLE_MAX( rect_a_bottom, rect_b_bottom ) - (*this_).top;
}

static inline void shape_int_rectangle_destroy ( shape_int_rectangle_t *this_ )
{
}

static inline int32_t shape_int_rectangle_get_left ( const shape_int_rectangle_t *this_ )
{
    return (*this_).left;
}

static inline int32_t shape_int_rectangle_get_top ( const shape_int_rectangle_t *this_ )
{
    return (*this_).top;
}

static inline int32_t shape_int_rectangle_get_right ( const shape_int_rectangle_t *this_ )
{
    return (*this_).left + (*this_).width;
}

static inline int32_t shape_int_rectangle_get_bottom ( const shape_int_rectangle_t *this_ )
{
    return (*this_).top + (*this_).height;
}

static inline uint32_t shape_int_rectangle_get_width ( const shape_int_rectangle_t *this_ )
{
    return (*this_).width;
}

static inline uint32_t shape_int_rectangle_get_height ( const shape_int_rectangle_t *this_ )
{
    return (*this_).height;
}

static inline bool shape_int_rectangle_contains ( const shape_int_rectangle_t *this_, int32_t x, int32_t y )
{
    bool result;
    const int32_t right = (*this_).left + (*this_).width;
    const int32_t bottom = (*this_).top + (*this_).height;
    /*
    result = ( x >= (*this_).left )&&( y >= (*this_).top )&&( x < right )&&( y < bottom );
    causes (depending on compiler, architecture and optimization level):
    warning: assuming signed overflow does not occur when assuming that (X + c) >= X is always true [-Wstrict-overflow]
    fix:
    */
    result = ( x-(*this_).left >= 0 )&&( y-(*this_).top >= 0 )&&( x-right < 0 )&&( y-bottom < 0 );
    return result;
}

static inline void shape_int_rectangle_shrink_by_border ( shape_int_rectangle_t *this_, int32_t border )
{
    int32_t double_border;
    double_border = 2 * border;
    if ( double_border > (*this_).width )
    {
        (*this_).left += ((*this_).width/2);  /* take the h-center of the old rectangle */
        (*this_).width = 0;
    }
    else
    {
        (*this_).left += border;
        (*this_).width -= double_border;
    }
    if ( double_border > (*this_).height )
    {
        (*this_).top += ((*this_).height/2);  /* take the v-middle of the old rectangle */
        (*this_).height = 0;
    }
    else
    {
        (*this_).top += border;
        (*this_).height -= double_border;
    }
}

static inline void shape_int_rectangle_shrink_to_ratio ( shape_int_rectangle_t *this_,
                                                         uint32_t ratio_width,
                                                         uint32_t ratio_height,
                                                         shape_h_align_t h_align,
                                                         shape_v_align_t v_align  )
{
    if ( (*this_).width * ratio_height == (*this_).height * ratio_width )
    {
        /* nothing to do - and the case for the div by 0 problem of ratio_width==0 and ratio_height==0 */
    }
    else if ( (*this_).width * ratio_height > (*this_).height * ratio_width )
    {
        /* the rectangle needs to shrink at left and/or right */
        uint32_t new_width;
        new_width = ( (*this_).height * ratio_width ) / ratio_height;
        switch ( h_align )
        {
            case SHAPE_H_ALIGN_LEFT:
            {
            }
            break;

            case SHAPE_H_ALIGN_RIGHT:
            {
                (*this_).left += ((*this_).width - new_width);
            }
            break;

            case SHAPE_H_ALIGN_CENTER: /* and */
            default:
            {
                (*this_).left += ( ((*this_).width - new_width)/2 );
            }
        }
        (*this_).width = new_width;
    }
    else
    {
        /* the rectangle needs to shrink at top and/or bottom */
        uint32_t new_height;
        new_height = ( (*this_).width * ratio_height ) / ratio_width;
        switch ( v_align )
        {
            case SHAPE_V_ALIGN_TOP:
            {
            }
            break;

            case SHAPE_V_ALIGN_BOTTOM:
            {
                (*this_).top += ((*this_).height - new_height);
            }
            break;

            case SHAPE_V_ALIGN_CENTER: /* and */
            default:
            {
                (*this_).top += ( ((*this_).height - new_height)/2 );
            }
        }
        (*this_).height = new_height;
    }
}

static inline void shape_int_rectangle_trace ( const shape_int_rectangle_t *this_ )
{
    U8_TRACE_INFO( "shape_int_rectangle_t" );
    U8_TRACE_INFO_INT( "- left:", (*this_).left );
    U8_TRACE_INFO_INT( "- top:", (*this_).top );
    U8_TRACE_INFO_INT( "- width:", (*this_).width );
    U8_TRACE_INFO_INT( "- height:", (*this_).height );
}


/*
Copyright 2016-2023 Andreas Warnke

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
