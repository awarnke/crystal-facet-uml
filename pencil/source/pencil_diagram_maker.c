/* File: pencil_diagram_maker.c; Copyright and License: see below */

#include "pencil_diagram_maker.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_diagram_maker_draw ( pencil_diagram_maker_t *this_,
                                 data_id_t mark_focused,
                                 data_id_t mark_highlighted,
                                 data_small_set_t *mark_selected,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    PangoLayout *layout;
    layout = pango_cairo_create_layout (cr);

    pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );
    geometry_rectangle_t *diagram_bounds = pencil_layouter_get_diagram_bounds_ptr ( &((*this_).layouter) );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );
    pencil_input_data_layout_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* draw diagram */
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
    pencil_diagram_painter_draw ( &((*this_).diagram_painter),
                                  diag,
                                  data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  pencil_size,
                                  diagram_bounds,
                                  layout,
                                  cr
                                );

    if (( width > 20.0 ) && ( height > 20.0 ))
    {
        /* draw all contained classifiers */
        pencil_diagram_maker_private_draw_classifiers ( this_,
                                                        mark_focused,
                                                        mark_highlighted,
                                                        mark_selected,
                                                        layout,
                                                        cr
                                                      );

        /* draw all contained features */
        uint32_t feat_count;
        feat_count = pencil_input_data_get_feature_count ( (*this_).input_data );
        for ( uint32_t index = 0; index < feat_count; index ++ )
        {
            data_feature_t *the_feature;
            the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, index );
            pencil_feature_painter_draw ( &((*this_).feature_painter),
                                          the_feature,
                                          data_id_equals_id( &mark_focused, DATA_TABLE_FEATURE, data_feature_get_id(the_feature) ),
                                          data_id_equals_id( &mark_highlighted, DATA_TABLE_FEATURE, data_feature_get_id( the_feature ) ),
                                          data_small_set_contains_row_id( mark_selected, DATA_TABLE_FEATURE, data_feature_get_id(the_feature) ),
                                          pencil_size,
                                          pencil_input_data_layout_get_feature_bounds_ptr ( layout_data, index ),
                                          layout,
                                          cr
                                        );
        }

        /* draw all contained relationships */
        uint32_t rel_count;
        rel_count = pencil_input_data_get_relationship_count ( (*this_).input_data );
        for ( uint32_t index = 0; index < rel_count; index ++ )
        {
            data_relationship_t *the_relationship;
            the_relationship = pencil_input_data_get_relationship_ptr ( (*this_).input_data, index );
            pencil_relationship_painter_draw ( &((*this_).relationship_painter),
                                               the_relationship,
                                               data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ),
                                               data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_id( the_relationship ) ),
                                               data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ),
                                               pencil_size,
                                               pencil_input_data_layout_get_relationship_shape_ptr ( layout_data, index ),
                                               layout,
                                               cr
                                             );
        }

    }

    g_object_unref (layout);

    TRACE_END();
}

void pencil_diagram_maker_private_draw_classifiers ( pencil_diagram_maker_t *this_,
                                                     data_id_t mark_focused,
                                                     data_id_t mark_highlighted,
                                                     data_small_set_t *mark_selected,
                                                     PangoLayout *layout,
                                                     cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    geometry_rectangle_t focused_rect;
    geometry_rectangle_init_empty( &focused_rect );
    pencil_input_data_layout_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            data_diagramelement_t *diagramelement;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
            diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );

            geometry_rectangle_t classifier_bounds;
            geometry_rectangle_copy( &classifier_bounds, pencil_input_data_layout_get_classifier_bounds_ptr ( layout_data, index ) );

            pencil_classifier_painter_draw( &((*this_).classifier_painter),
                                            visible_classifier,
                                            data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ),
                                            data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) ),
                                            data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ),
                                            pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) ),
                                            &classifier_bounds,
                                            layout,
                                            cr
                                          );

            if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ))
            {
                geometry_rectangle_replace( &focused_rect, &classifier_bounds );
            }

            geometry_rectangle_destroy( &classifier_bounds );
        }
        else
        {
            TSLOG_ERROR("invalid visible classifier in array!");
        }
    }

    /* mark focused */
    if ( ! geometry_rectangle_is_empty( &focused_rect ) )
    {
        pencil_marker_mark_focused_rectangle( &((*this_).marker), focused_rect, cr );
    }

    geometry_rectangle_destroy( &focused_rect );
    TRACE_END();
}


/*
Copyright 2016-2017 Andreas Warnke

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
