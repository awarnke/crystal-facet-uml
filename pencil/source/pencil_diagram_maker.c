/* File: pencil_diagram_maker.c; Copyright and License: see below */

#include "pencil_diagram_maker.h"
#include "trace/trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_diagram_maker_draw ( pencil_diagram_maker_t *this_,
                                 data_id_t mark_focused,
                                 data_id_t mark_highlighted,
                                 const data_small_set_t *mark_selected,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    PangoLayout *layout;
    layout = pango_cairo_create_layout (cr);

    /* get layout data object */
    const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const ( &((*this_).layouter) );

    /* get diagram bounds */
    const layout_diagram_t *const diagram_layout = pencil_layout_data_get_diagram_const( layout_data );
    const geometry_rectangle_t *const diagram_bounds = layout_diagram_get_bounds_const ( diagram_layout );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );

    const pencil_size_t *const pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );

    /* draw diagram */
    const data_diagram_t *diag = data_visible_set_get_diagram_const( (*this_).input_data );
    pencil_diagram_painter_draw ( &((*this_).diagram_painter),
                                  diagram_layout,
                                  data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAM, data_diagram_get_row_id(diag) ),
                                  data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAM, data_diagram_get_row_id(diag) ),
                                  data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAM, data_diagram_get_row_id(diag) ),
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

        /* draw all contained features */
        pencil_diagram_maker_private_draw_features ( this_,
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
                                                     const data_small_set_t *mark_selected,
                                                     PangoLayout *layout,
                                                     cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const ( &((*this_).layouter) );

    /* iterate over all classifiers */
    const uint32_t count = pencil_layout_data_get_visible_classifier_count ( layout_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        const layout_visible_classifier_t *const classifier_layout = pencil_layout_data_get_visible_classifier_const( layout_data, index );

        const pencil_size_t *const pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );

        pencil_classifier_composer_draw( &((*this_).classifier_painter),
                                         classifier_layout,
                                         mark_focused,
                                         mark_highlighted,
                                         mark_selected,
                                         layout_data,
                                         pencil_size,
                                         layout,
                                         cr
                                       );
    }

    TRACE_END();
}

void pencil_diagram_maker_private_draw_features ( pencil_diagram_maker_t *this_,
                                                  data_id_t mark_focused,
                                                  data_id_t mark_highlighted,
                                                  const data_small_set_t *mark_selected,
                                                  PangoLayout *layout,
                                                  cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const ( &((*this_).layouter) );

    /* iterate over all features */
    const uint32_t count = pencil_layout_data_get_feature_count ( layout_data );
    for ( uint32_t f_idx = 0; f_idx < count; f_idx ++ )
    {
        /* get feature */
        const layout_feature_t *const the_feature = pencil_layout_data_get_feature_const( layout_data, f_idx );

        /* determine display flags of diagramelement */
        const layout_visible_classifier_t *const classifier_layout = layout_feature_get_classifier_const ( the_feature );
        const data_diagramelement_t *const diagramelement = layout_visible_classifier_get_diagramelement_const( classifier_layout );
        const data_diagramelement_flag_t display_flags = data_diagramelement_get_display_flags( diagramelement );

        /* draw features */
        pencil_feature_painter_draw ( &((*this_).feature_painter),
                                      the_feature,
                                      data_id_equals_id( &mark_focused, DATA_TABLE_FEATURE, layout_feature_get_feature_id(the_feature) ),
                                      data_id_equals_id( &mark_highlighted, DATA_TABLE_FEATURE, layout_feature_get_feature_id( the_feature ) ),
                                      data_small_set_contains_row_id( mark_selected, DATA_TABLE_FEATURE, layout_feature_get_feature_id(the_feature) ),
                                      (0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT )),
                                      pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) ),
                                      layout,
                                      cr
                                    );
    }

    TRACE_END();
}

void pencil_diagram_maker_private_draw_relationships ( pencil_diagram_maker_t *this_,
                                                       data_id_t mark_focused,
                                                       data_id_t mark_highlighted,
                                                       const data_small_set_t *mark_selected,
                                                       PangoLayout *layout,
                                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const ( &((*this_).layouter) );

    const uint32_t rel_count = pencil_layout_data_get_relationship_count ( layout_data );
    for ( uint32_t index = 0; index < rel_count; index ++ )
    {
        pencil_visibility_t show_relation;
        const layout_relationship_t *const relationship_layout = pencil_layout_data_get_relationship_const ( layout_data, index );
        const data_relationship_t *const the_relationship = layout_relationship_get_data_const ( relationship_layout );
        show_relation = layout_relationship_get_visibility ( relationship_layout );
        if ( PENCIL_VISIBILITY_IMPLICIT == show_relation )
        {
            if ( data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_row_id(the_relationship) )
                || data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_row_id(the_relationship) )
                || data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_row_id(the_relationship) ) )
            {
                /* the implicit relationship is focused or marked or highlighted */
                show_relation = PENCIL_VISIBILITY_SHOW;
            }
            else
            {
                if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &mark_highlighted ) )
                {
                    const data_row_id_t diagramelement_id = data_id_get_row_id( &mark_highlighted );
                    const data_visible_classifier_t *visible_clsfy = data_visible_set_get_visible_classifier_by_id_const ( (*this_).input_data, diagramelement_id );
                    if ( visible_clsfy != NULL )
                    {
                        if ( data_visible_classifier_is_valid( visible_clsfy ) )
                        {
                            const data_classifier_t *classifier = data_visible_classifier_get_classifier_const( visible_clsfy );
                            if (( data_classifier_get_row_id( classifier ) == data_relationship_get_from_classifier_row_id( the_relationship ) )
                                ||( data_classifier_get_row_id( classifier ) == data_relationship_get_to_classifier_row_id( the_relationship ) ))
                            {
                                /* the implicit relationship has highlighted from or to classifier */
                                show_relation = PENCIL_VISIBILITY_SHOW;
                            }
                        }
                    }

                }
            }
        }
        if (( PENCIL_VISIBILITY_SHOW == show_relation )||( PENCIL_VISIBILITY_GRAY_OUT == show_relation ))
        {
            const pencil_size_t *const pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );
            pencil_relationship_painter_draw ( &((*this_).relationship_painter),
                                               relationship_layout,
                                               data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_row_id(the_relationship) ),
                                               data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_row_id( the_relationship ) ),
                                               data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_row_id(the_relationship) ),
                                               pencil_size,
                                               layout,
                                               cr
                                             );
        }
    }

    TRACE_END();
}

pencil_error_t pencil_diagram_maker_get_order_at_pos ( const pencil_diagram_maker_t *this_,
                                                       data_id_t obj_id,
                                                       double x,
                                                       double y,
                                                       layout_order_t* out_layout_order )
{
    TRACE_BEGIN();
    assert( NULL != out_layout_order );

    pencil_error_t result = PENCIL_ERROR_NONE;

    const data_table_t table = data_id_get_table ( &obj_id );
    switch ( table )
    {
        case DATA_TABLE_CLASSIFIER:
        {
            const data_row_id_t classifier_id = data_id_get_row_id ( &obj_id );
            const data_classifier_t *const the_classifier
                = data_visible_set_get_classifier_by_id_const ( (*this_).input_data, classifier_id );
            const data_classifier_type_t c_type
                = (NULL == the_classifier)
                ? DATA_CLASSIFIER_TYPE_CLASS  /* for new or unknown objects, assume class */
                : data_classifier_get_main_type( the_classifier );
            result = pencil_layouter_get_classifier_order_at_pos ( &((*this_).layouter),
                                                                   c_type,
                                                                   x,
                                                                   y,
                                                                   (*this_).snap_to_grid_distance,
                                                                   out_layout_order
                                                                 );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const data_row_id_t feature_id = data_id_get_row_id ( &obj_id );
            const data_feature_t *const the_feature
                = data_visible_set_get_feature_by_id_const ( (*this_).input_data, feature_id );
            if( NULL != the_feature )
            {
                result = pencil_layouter_get_feature_order_at_pos ( &((*this_).layouter),
                                                                    the_feature,
                                                                    x,
                                                                    y,
                                                                    out_layout_order
                                                                  );
            }
            else
            {
                TSLOG_ANOMALY( "feature to move does not exist in input_data." );
                layout_order_init_empty( out_layout_order );
                result = PENCIL_ERROR_UNKNOWN_OBJECT;
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            result = pencil_layouter_get_relationship_order_at_pos ( &((*this_).layouter),
                                                                     x,
                                                                     y,
                                                                     out_layout_order
                                                                   );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            TSLOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            /* pencil cannot move diagrams */
            TSLOG_WARNING( "object to be re-positioned has unexpected type: diagram" );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
        break;

        default:
        {
            TSLOG_WARNING( "object to be re-positioned has illegal type" );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
        break;
    }

    TRACE_END_ERR(result);
    return result;
}

pencil_error_t pencil_diagram_maker_get_feature_order_at_pos ( const pencil_diagram_maker_t *this_,
                                                               const data_feature_t *feature_ptr,
                                                               double x,
                                                               double y,
                                                               layout_order_t* out_layout_order )
{
    TRACE_BEGIN();
    assert( NULL != feature_ptr );
    assert( NULL != out_layout_order );

    const pencil_error_t result
        = pencil_layouter_get_feature_order_at_pos ( &((*this_).layouter),
                                                     feature_ptr,
                                                     x,
                                                     y,
                                                     out_layout_order
                                                   );

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2016-2022 Andreas Warnke

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
