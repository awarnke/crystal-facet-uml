/* File: pencil_diagram_maker.inl; Copyright and License: see below */

#include "pencil_diagram_maker.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline void pencil_diagram_maker_init( pencil_diagram_maker_t *this_,
                                              const data_visible_set_t *input_data,
                                              const data_profile_part_t *profile )
{
    U8_TRACE_BEGIN();
    assert( NULL != input_data );
    assert( NULL != profile );

    pencil_diagram_painter_init( &((*this_).diagram_painter) );
    pencil_classifier_composer_init( &((*this_).classifier_painter) );
    pencil_relationship_painter_init( &((*this_).relationship_painter) );
    pencil_feature_painter_init( &((*this_).feature_painter) );

    (*this_).input_data = input_data;
    (*this_).profile = profile;
    pencil_layouter_init( &((*this_).layouter), input_data, profile );

    U8_TRACE_END();
}

static inline void pencil_diagram_maker_reinit( pencil_diagram_maker_t *this_,
                                                const data_visible_set_t *input_data,
                                                const data_profile_part_t *profile )
{
    U8_TRACE_BEGIN();
    assert( NULL != input_data );
    assert( NULL != profile );

    (*this_).input_data = input_data;
    (*this_).profile = profile;
    pencil_layouter_reinit( &((*this_).layouter), input_data, profile );

    U8_TRACE_END();
}

static inline void pencil_diagram_maker_destroy( pencil_diagram_maker_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_diagram_painter_destroy( &((*this_).diagram_painter) );
    pencil_classifier_composer_destroy( &((*this_).classifier_painter) );
    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );
    pencil_feature_painter_destroy( &((*this_).feature_painter) );

    pencil_layouter_destroy( &((*this_).layouter) );
    (*this_).input_data = NULL;

    U8_TRACE_END();
}

static inline void pencil_diagram_maker_define_grid( pencil_diagram_maker_t *this_,
                                                     geometry_rectangle_t diagram_bounds,
                                                     cairo_t *cr )
{
    U8_TRACE_BEGIN();

    PangoLayout *font_layout;
    font_layout = pango_cairo_create_layout (cr);

    pencil_layouter_prepare ( &((*this_).layouter) );
    pencil_layouter_define_grid ( &((*this_).layouter), diagram_bounds, font_layout );

    g_object_unref (font_layout);

    U8_TRACE_END();
}

static inline const geometry_grid_t *pencil_diagram_maker_get_grid_const ( const pencil_diagram_maker_t *this_ )
{
    return pencil_layouter_get_grid_const( &((*this_).layouter) );
}

static inline void pencil_diagram_maker_layout_elements( pencil_diagram_maker_t *this_,
                                                         data_stat_t *io_layout_stat,
                                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( cr != NULL );

    /* trace input data: */
    const data_id_t diag_id = data_diagram_get_data_id( data_visible_set_get_diagram_const( (*this_).input_data ) );
    data_id_trace( &diag_id );
    data_profile_part_trace( (*this_).profile );

    PangoLayout *font_layout;
    font_layout = pango_cairo_create_layout (cr);

    pencil_layouter_layout_elements ( &((*this_).layouter), font_layout );
    if ( io_layout_stat != NULL )
    {
        layout_visible_set_get_statistics( pencil_layouter_get_layout_data_const( &((*this_).layouter) ), io_layout_stat );
    }

    g_object_unref (font_layout);

    U8_TRACE_END();
}

static inline const layout_visible_set_t *pencil_diagram_maker_get_layout_data_const( const pencil_diagram_maker_t *this_ )
{
    return pencil_layouter_get_layout_data_const( &((*this_).layouter) );
}


/*
Copyright 2016-2024 Andreas Warnke

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
