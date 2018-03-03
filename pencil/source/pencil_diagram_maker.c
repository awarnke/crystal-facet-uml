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

    /* get layout data object */
    pencil_input_data_layout_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* get diagram bounds */
    layout_diagram_t *diagram_layout;
    diagram_layout = pencil_input_data_layout_get_diagram_ptr( layout_data );
    geometry_rectangle_t *diagram_bounds = layout_diagram_get_bounds_ptr ( diagram_layout );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );

    pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );

    /* draw diagram */
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
    pencil_diagram_painter_draw ( &((*this_).diagram_painter),
                                  diagram_layout,
                                  data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  pencil_size,
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

        /* draw all contained relationships */
        pencil_diagram_maker_private_draw_relationships ( this_,
                                                          mark_focused,
                                                          mark_highlighted,
                                                          mark_selected,
                                                          layout,
                                                          cr
                                                        );
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

    pencil_input_data_layout_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_layout_get_classifier_count ( layout_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        layout_visible_classifier_t *classifier_layout;
        classifier_layout = pencil_input_data_layout_get_classifier_ptr( layout_data, index );
        const data_visible_classifier_t *visible_classifier;
        visible_classifier = layout_visible_classifier_get_data_ptr ( classifier_layout );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            const data_classifier_t *classifier;
            const data_diagramelement_t *diagramelement;
            classifier = data_visible_classifier_get_classifier_const( visible_classifier );
            diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );

            data_diagramelement_flag_t display_flags;
            display_flags = data_diagramelement_get_display_flags( diagramelement );

            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr ( layout_data, index );
            pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );

            pencil_classifier_painter_draw( &((*this_).classifier_painter),
                                            classifier_layout,
                                            mark_focused,
                                            mark_highlighted,
                                            mark_selected,
                                            pencil_size,
                                            layout,
                                            cr
                                          );

            /* draw all contained features */
            uint32_t linenumber = 0;
            for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count ( (*this_).input_data ); f_idx ++ )
            {
                data_feature_t *the_feature;
                the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, f_idx );
                if ( data_feature_get_classifier_id( the_feature ) == data_classifier_get_id( classifier ) )
                {
                    geometry_rectangle_t feature_bounds;
                    feature_bounds = pencil_layouter_get_feature_bounds( &((*this_).layouter),
                                                                         data_classifier_get_id( classifier ),
                                                                         index,
                                                                         f_idx,
                                                                         linenumber
                    );
                    layout_feature_t *feature_workaround;
                    feature_workaround = pencil_input_data_layout_get_feature_ptr( layout_data, f_idx );
                    layout_feature_set_bounds( feature_workaround, &feature_bounds );
                    pencil_feature_painter_draw ( &((*this_).feature_painter),
                                                  feature_workaround,
                                                  data_id_equals_id( &mark_focused, DATA_TABLE_FEATURE, data_feature_get_id(the_feature) ),
                                                  data_id_equals_id( &mark_highlighted, DATA_TABLE_FEATURE, data_feature_get_id( the_feature ) ),
                                                  data_small_set_contains_row_id( mark_selected, DATA_TABLE_FEATURE, data_feature_get_id(the_feature) ),
                                                  (0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GREY_OUT )),
                                                  pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) ),
                                                  layout,
                                                  cr
                                                );
                    linenumber ++;
                    geometry_rectangle_destroy( &feature_bounds );
                }
            }

        }
        else
        {
            TSLOG_ERROR("invalid visible classifier in array!");
        }
    }

    TRACE_END();
}

void pencil_diagram_maker_private_draw_relationships ( pencil_diagram_maker_t *this_,
                                                       data_id_t mark_focused,
                                                       data_id_t mark_highlighted,
                                                       data_small_set_t *mark_selected,
                                                       PangoLayout *layout,
                                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    pencil_input_data_layout_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    uint32_t rel_count;
    rel_count = pencil_input_data_layout_get_relationship_count ( layout_data );
    for ( uint32_t index = 0; index < rel_count; index ++ )
    {
        pencil_visibility_t show_relation;
        const data_relationship_t *the_relationship;
        layout_relationship_t *relationship_layout;
        relationship_layout = pencil_input_data_layout_get_relationship_ptr ( layout_data, index );
        the_relationship = layout_relationship_get_data_ptr ( relationship_layout );
        show_relation = layout_relationship_get_visibility ( relationship_layout );
        if ( PENCIL_VISIBILITY_IMPLICIT == show_relation )
        {
            if ( data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) )
                || data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) )
                || data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ) )
            {
                /* the implicit relationship is focused or marked or highlighted */
                show_relation = PENCIL_VISIBILITY_SHOW;
            }
            else
            {
                if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &mark_highlighted ) )
                {
                    int64_t diagramelement_id = data_id_get_row_id( &mark_highlighted );
                    data_visible_classifier_t *visible_clsfy;
                    visible_clsfy = pencil_input_data_get_visible_classifier_by_id_ptr ( (*this_).input_data, diagramelement_id );
                    if ( visible_clsfy != NULL )
                    {
                        if ( data_visible_classifier_is_valid( visible_clsfy ) )
                        {
                            data_classifier_t *classifier;
                            classifier = data_visible_classifier_get_classifier_ptr( visible_clsfy );
                            if (( data_classifier_get_id( classifier ) == data_relationship_get_from_classifier_id( the_relationship ) )
                                ||( data_classifier_get_id( classifier ) == data_relationship_get_to_classifier_id( the_relationship ) ))
                            {
                                /* the implicit relationship has highlighted from or to classifier */
                                show_relation = PENCIL_VISIBILITY_SHOW;
                            }
                        }
                    }

                }
            }
        }
        if ( PENCIL_VISIBILITY_SHOW == show_relation )
        {
            pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );
            pencil_relationship_painter_draw ( &((*this_).relationship_painter),
                                               relationship_layout,
                                               data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ),
                                               data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_id( the_relationship ) ),
                                               data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ),
                                               pencil_size,
                                               layout,
                                               cr
            );
        }
    }

    TRACE_END();
}


/*
Copyright 2016-2018 Andreas Warnke

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
