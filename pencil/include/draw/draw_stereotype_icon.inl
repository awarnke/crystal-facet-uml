/* File: draw_stereotype_icon.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <math.h>
#include <assert.h>

static inline void draw_stereotype_icon_init( draw_stereotype_icon_t *this_ )
{
    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */
}

static inline void draw_stereotype_icon_destroy( draw_stereotype_icon_t *this_ )
{

}

static const double DRAW_STEREOTYPE_ICON_REL_SIZE = 1.5;

static inline geometry_dimensions_t draw_stereotype_icon_get_dimensions( const draw_stereotype_icon_t *this_,
                                                                          const pencil_size_t *pencil_size )
{
    assert( pencil_size != NULL );
    geometry_dimensions_t result;

    const double image_height = DRAW_STEREOTYPE_ICON_REL_SIZE * pencil_size_get_title_font_size( pencil_size );
    geometry_dimensions_init ( &result, DRAW_STEREOTYPE_ICON_WIDTH_TO_HEIGHT * image_height, image_height );

    return result;
}

static inline geometry_rectangle_t draw_stereotype_icon_get_bounds ( const draw_stereotype_icon_t *this_,
                                                                     double x,
                                                                     double y,
                                                                     geometry_h_align_t h_align,
                                                                     geometry_v_align_t v_align,
                                                                     const pencil_size_t *pencil_size )
{
    geometry_rectangle_t result;

    const double image_height = DRAW_STEREOTYPE_ICON_REL_SIZE * pencil_size_get_title_font_size( pencil_size );
    const double image_width = DRAW_STEREOTYPE_ICON_WIDTH_TO_HEIGHT * image_height;
    geometry_rectangle_init( &result,
                             geometry_h_align_get_left( &h_align, image_width, x, 0.0 ),
                             geometry_v_align_get_top( &v_align, image_height, y, 0.0 ),
                             image_width,
                             image_height
                           );

    return result;
}

static inline bool draw_stereotype_icon_exists( const draw_stereotype_icon_t *this_,
                                                const char *stereotype,
                                                const data_profile_part_t *profile )
{
    assert( stereotype != NULL );
    assert( profile != NULL );
    bool result = false;

    const utf8stringview_t stereotype_view = UTF8STRINGVIEW_STR(stereotype);
    const data_classifier_t *const optional_stereotype
        = data_profile_part_get_stereotype_by_name_const( profile, &stereotype_view );
    if ( optional_stereotype != NULL )
    {
        U8_TRACE_INFO_STR( "stereotype", stereotype );
        const char *const drawing_directives = data_classifier_get_description_const( optional_stereotype );
        result = draw_stereotype_icon_private_exists( this_, drawing_directives );
    }

    return result;
}

static inline bool draw_stereotype_icon_private_exists( const draw_stereotype_icon_t *this_,
                                                        const char *drawing_directives )
{
    assert( drawing_directives != NULL );
    /* TODO allow namespaces, e.g. <ns:path; check for end of nmtoken to prevent finding <pathfinder */
    const char *pattern = "<path";
    const bool exists = utf8string_contains_str( drawing_directives, pattern );
    return exists;
}

static inline u8_error_t draw_stereotype_icon_parse_svg_xml( const draw_stereotype_icon_t *this_,
                                                             const char *drawing_directives,
                                                             geometry_rectangle_t *out_view_rect,
                                                             u8_error_info_t *out_err_info )
{
    assert( drawing_directives != NULL );
    assert( out_view_rect != NULL );
    assert( out_err_info != NULL );

    geometry_rectangle_t target_bounds;
    geometry_rectangle_init_empty( &target_bounds );
    geometry_rectangle_init_empty( out_view_rect );
    const GdkRGBA default_color = { .red = 0.0, .green = 0.0, .blue = 0.0, .alpha = 0.0 };
    u8_error_info_init_void( out_err_info );

    const u8_error_t result
        = draw_stereotype_icon_private_parse_svg_xml( this_,
                                                      false,  /* draw */
                                                      drawing_directives,
                                                      out_view_rect,
                                                      &default_color,
                                                      out_err_info,
                                                      &target_bounds,
                                                      NULL  /* cr */
                                                    );
    return result;
}


/*
Copyright 2023-2025 Andreas Warnke

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
