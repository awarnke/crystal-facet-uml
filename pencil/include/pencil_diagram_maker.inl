/* File: pencil_diagram_maker.inl; Copyright and License: see below */

#include "pencil_diagram_maker.h"
#include "trace.h"
#include <assert.h>

static inline void pencil_diagram_maker_init( pencil_diagram_maker_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    pencil_diagram_painter_init( &((*this_).diagram_painter) );
    pencil_classifier_composer_init( &((*this_).classifier_painter) );
    pencil_relationship_painter_init( &((*this_).relationship_painter) );
    pencil_feature_painter_init( &((*this_).feature_painter) );

    pencil_layouter_init( &((*this_).layouter), input_data );

    (*this_).input_data = input_data;

    TRACE_END();
}

static inline void pencil_diagram_maker_destroy( pencil_diagram_maker_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_painter_destroy( &((*this_).diagram_painter) );
    pencil_classifier_composer_destroy( &((*this_).classifier_painter) );
    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );
    pencil_feature_painter_destroy( &((*this_).feature_painter) );

    pencil_layouter_destroy( &((*this_).layouter) );

    (*this_).input_data = NULL;

    TRACE_END();
}

static inline void pencil_diagram_maker_layout_grid ( pencil_diagram_maker_t *this_,
                                                      pencil_input_data_t *input_data,
                                                      geometry_rectangle_t diagram_bounds )
{
    pencil_layouter_reinit ( &((*this_).layouter), input_data );
    pencil_layouter_layout_grid ( &((*this_).layouter), diagram_bounds );
}

static inline void pencil_diagram_maker_layout_elements ( pencil_diagram_maker_t *this_, cairo_t *cr )
{
    PangoLayout *font_layout;
    font_layout = pango_cairo_create_layout (cr);

    pencil_layouter_layout_elements ( &((*this_).layouter), font_layout );

    g_object_unref (font_layout);
}

static inline pencil_error_t pencil_diagram_maker_get_object_id_at_pos ( pencil_diagram_maker_t *this_,
                                                                         double x,
                                                                         double y,
                                                                         pencil_type_filter_t filter,
                                                                         data_id_pair_t* out_selected_id,
                                                                         data_id_pair_t* out_surrounding_id )
{
    return pencil_layouter_get_object_id_at_pos ( &((*this_).layouter),
                                                  x,
                                                  y,
                                                  3.0,
                                                  filter,
                                                  out_selected_id,
                                                  out_surrounding_id
                                                );
}

static const double snap_to_grid_distance_for_drag_marker = 3.13;  /* smaller than snap_to_grid_distance_for_dropping */
                                                                   /* to ensure object really snaps when marked so */

static inline void pencil_diagram_maker_is_pos_on_grid ( pencil_diagram_maker_t *this_,
                                                         double x,
                                                         double y,
                                                         bool *out_x_on_grid,
                                                         bool *out_y_on_grid )
{
    pencil_layouter_is_pos_on_grid ( &((*this_).layouter),
                                     x,
                                     y,
                                     snap_to_grid_distance_for_drag_marker,
                                     out_x_on_grid,
                                     out_y_on_grid
                                   );
}


/*
Copyright 2016-2019 Andreas Warnke

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
