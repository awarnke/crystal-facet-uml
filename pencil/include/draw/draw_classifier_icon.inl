/* File: draw_classifier_icon.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline void draw_classifier_icon_init( draw_classifier_icon_t *this_ )
{
    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */
}

static inline void draw_classifier_icon_destroy( draw_classifier_icon_t *this_ )
{

}

static inline geometry_dimensions_t draw_classifier_icon_get_icon_dimensions( const draw_classifier_icon_t *this_,
                                                                              data_classifier_type_t classifier_type,
                                                                              const pencil_size_t *pencil_size )
{
    assert( pencil_size != NULL );
    geometry_dimensions_t result;

    if ( classifier_type == DATA_CLASSIFIER_TYPE_COMPONENT )
    {
        const double type_icon_height = pencil_size_get_title_font_size( pencil_size );
        geometry_dimensions_init ( &result, DRAW_CLASSIFIER_ICON_COMPONENT_ICON_WIDTH_TO_HEIGHT * type_icon_height, type_icon_height );
    }
    else if ( classifier_type == DATA_CLASSIFIER_TYPE_ARTIFACT )
    {
        const double type_icon_height = pencil_size_get_title_font_size( pencil_size );
        geometry_dimensions_init ( &result, DRAW_CLASSIFIER_ICON_ARTIFACT_ICON_WIDTH_TO_HEIGHT * type_icon_height, type_icon_height );
    }
    else if ( classifier_type == DATA_CLASSIFIER_TYPE_COMMENT )
    {
        /* comments do not have icons - but a folded corner at the same place - therefore this placeholder dimension */
        const double corner_width = pencil_size_get_standard_font_size( pencil_size );
        geometry_dimensions_init ( &result, corner_width, corner_width );
    }
    else
    {
        geometry_dimensions_init ( &result, 0.0, 0.0 );
    }

    return result;
}

static inline geometry_rectangle_t draw_classifier_icon_get_component_bounds ( const draw_classifier_icon_t *this_,
                                                                               double x,
                                                                               double y,
                                                                               geometry_h_align_t h_align,
                                                                               geometry_v_align_t v_align,
                                                                               double height )
{
    geometry_rectangle_t result;

    const double width = DRAW_CLASSIFIER_ICON_COMPONENT_ICON_WIDTH_TO_HEIGHT * height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_classifier_icon_get_artifact_bounds ( const draw_classifier_icon_t *this_,
                                                                              double x,
                                                                              double y,
                                                                              geometry_h_align_t h_align,
                                                                              geometry_v_align_t v_align,
                                                                              double height )
{
    geometry_rectangle_t result;

    const double width = DRAW_CLASSIFIER_ICON_ARTIFACT_ICON_WIDTH_TO_HEIGHT * height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_classifier_icon_get_actor_bounds ( const draw_classifier_icon_t *this_,
                                                                           double x,
                                                                           double y,
                                                                           geometry_h_align_t h_align,
                                                                           geometry_v_align_t v_align,
                                                                           double height )
{
    geometry_rectangle_t result;

    const double width = height/3.0;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_classifier_icon_get_circle_bounds ( const draw_classifier_icon_t *this_,
                                                                            double x,
                                                                            double y,
                                                                            geometry_h_align_t h_align,
                                                                            geometry_v_align_t v_align,
                                                                            double height )
{
    geometry_rectangle_t result;

    const double width = height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_classifier_icon_get_time_bounds ( const draw_classifier_icon_t *this_,
                                                                          double x,
                                                                          double y,
                                                                          geometry_h_align_t h_align,
                                                                          geometry_v_align_t v_align,
                                                                          double height )
{
    geometry_rectangle_t result;

    const double width = height/1.732050808;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_classifier_icon_get_sync_bounds ( const draw_classifier_icon_t *this_,
                                                                          double x,
                                                                          double y,
                                                                          geometry_h_align_t h_align,
                                                                          geometry_v_align_t v_align,
                                                                          double height,
                                                                          const pencil_size_t *pencil_size )
{
    assert( pencil_size != NULL );
    geometry_rectangle_t result;

    const double width = 3.0 * pencil_size_get_bold_line_width( pencil_size );
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}


/*
Copyright 2019-2026 Andreas Warnke

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
