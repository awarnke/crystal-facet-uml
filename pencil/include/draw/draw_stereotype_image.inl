/* File: draw_stereotype_image.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void draw_stereotype_image_init( draw_stereotype_image_t *this_ )
{

}

static inline void draw_stereotype_image_destroy( draw_stereotype_image_t *this_ )
{

}

static inline geometry_dimensions_t draw_stereotype_image_get_dimensions( const draw_stereotype_image_t *this_,
                                                                          const pencil_size_t *pencil_size )
{
    assert( pencil_size != NULL );
    geometry_dimensions_t result;

    const double type_icon_height = pencil_size_get_title_font_size( pencil_size );
    geometry_dimensions_init ( &result, DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT * type_icon_height, type_icon_height );

    return result;
}

static inline geometry_rectangle_t draw_stereotype_image_get_bounds ( const draw_stereotype_image_t *this_,
                                                                      double x,
                                                                      double y,
                                                                      geometry_h_align_t h_align,
                                                                      geometry_v_align_t v_align,
                                                                      double height )
{
    geometry_rectangle_t result;

    const double width = DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT * height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
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
