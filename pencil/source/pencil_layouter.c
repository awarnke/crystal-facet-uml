/* File: pencil_layouter.c; Copyright and License: see below */

#include "pencil_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_layouter_init( pencil_layouter_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    geometry_rectangle_init_empty( &((*this_).diagram_bounds) );
    pencil_size_init_empty( &((*this_).pencil_size) );
    geometry_rectangle_init_empty( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 1.0 );
    geometry_rectangle_init_empty( &((*this_).default_classifier_size) );

    pencil_input_data_layout_init( &((*this_).layout_data) );
    (*this_).input_data = input_data;

    TRACE_END();
}

void pencil_layouter_destroy( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    geometry_rectangle_destroy( &((*this_).diagram_bounds) );
    pencil_size_destroy( &((*this_).pencil_size) );
    geometry_rectangle_destroy( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_destroy( &((*this_).x_scale) );
    geometry_non_linear_scale_destroy( &((*this_).y_scale) );
    geometry_rectangle_destroy( &((*this_).default_classifier_size) );

    pencil_input_data_layout_destroy( &((*this_).layout_data) );
    (*this_).input_data = NULL;

    TRACE_END();
}



void pencil_layouter_layout_grid ( pencil_layouter_t *this_,
                                   pencil_input_data_t *input_data,
                                   geometry_rectangle_t diagram_bounds )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    /* update the pointer to the input data */
    (*this_).input_data = input_data;

    /* update the bounding rectangle */
    geometry_rectangle_replace( &((*this_).diagram_bounds), &diagram_bounds );

    /* calculate the pencil-sizes and the drawing rectangle */
    double left = geometry_rectangle_get_left ( &((*this_).diagram_bounds) );
    double top = geometry_rectangle_get_top ( &((*this_).diagram_bounds) );
    double right = geometry_rectangle_get_right ( &((*this_).diagram_bounds) );
    double bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_bounds) );
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );
    pencil_size_reinit( &((*this_).pencil_size), width, height );
    double gap = pencil_size_get_standard_object_border( &((*this_).pencil_size) );
    double f_size = pencil_size_get_standard_font_size( &((*this_).pencil_size) );
    double f_line_gap = pencil_size_get_font_line_gap( &((*this_).pencil_size) );
    geometry_rectangle_reinit( &((*this_).diagram_draw_area), left+gap, top+gap+f_size+f_line_gap, width-2.0*gap, height-2.0*gap-f_size-f_line_gap );

    /* calculate the axis scales */
    double draw_left = geometry_rectangle_get_left ( &((*this_).diagram_draw_area) );
    double draw_top = geometry_rectangle_get_top ( &((*this_).diagram_draw_area) );
    double draw_right = geometry_rectangle_get_right ( &((*this_).diagram_draw_area) );
    double draw_bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_draw_area) );
    double draw_width = geometry_rectangle_get_width ( &((*this_).diagram_draw_area) );
    double draw_height = geometry_rectangle_get_height ( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_reinit( &((*this_).x_scale), draw_left + 0.04*draw_width, draw_right - 0.04*draw_width );
    geometry_non_linear_scale_reinit( &((*this_).y_scale), draw_top +0.04*draw_height, draw_bottom - 0.04*draw_height );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( input_data, index );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );

            /* adjust the non-linear scales for this classifier */
            geometry_non_linear_scale_add_order ( &((*this_).x_scale), data_classifier_get_x_order( classifier ) );
            geometry_non_linear_scale_add_order ( &((*this_).y_scale), data_classifier_get_y_order( classifier ) );
        }
    }

    TRACE_END();
}

void pencil_layouter_layout_elements ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* adjust the default classifier rectangle */
    uint32_t count_clasfy;
    count_clasfy = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    {
        double diagram_area = geometry_rectangle_get_area( &((*this_).diagram_draw_area) );
        double classifier_area;
        if ( count_clasfy > 0 )
        {
            classifier_area = diagram_area / count_clasfy * (0.10);
        }
        else
        {
            classifier_area = diagram_area * (0.10);
        }
        double half_width = sqrt(classifier_area);
        double half_height = half_width / 2.1;
        geometry_rectangle_reinit( &((*this_).default_classifier_size), -half_width, -half_height, 2.0 * half_width, 2.0 * half_height );
    }

    /* store the classifier bounds into input_data_layouter_t */
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        data_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

        if (( visible_classifier2 != NULL ) && ( data_visible_classifier_is_valid( visible_classifier2 ) ))
        {
            data_classifier_t *classifier2;
            classifier2 = data_visible_classifier_get_classifier_ptr( visible_classifier2 );

            /* get the bounds rectangle to modify */
            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );

            /* overwrite directly internal attributes of (*this_).layout_data */
            geometry_rectangle_replace( classifier_bounds, &((*this_).default_classifier_size) );
            int32_t order_x = data_classifier_get_x_order( classifier2 );
            int32_t order_y = data_classifier_get_y_order( classifier2 );
            double center_x = geometry_non_linear_scale_get_location( &((*this_).x_scale), order_x );
            double center_y = geometry_non_linear_scale_get_location( &((*this_).y_scale), order_y );
            geometry_rectangle_shift( classifier_bounds, center_x, center_y );

            /* get the inner space rectangle to modify */
            geometry_rectangle_t *classifier_space;
            classifier_space = pencil_input_data_layout_get_classifier_space_ptr( &((*this_).layout_data), index );

            /* overwrite directly internal attributes of (*this_).layout_data */
            bool has_stereotype = data_classifier_has_stereotype( classifier2 );
            double space_left = geometry_rectangle_get_left( classifier_bounds )
                                + 2.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size ) );
            double space_width = geometry_rectangle_get_width( classifier_bounds )
                                 - 4.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size ) );
            double space_height = geometry_rectangle_get_height( classifier_bounds )
                                  - 4.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size ) )
                                  - pencil_size_get_larger_font_size( &((*this_).pencil_size ) );
            /* for underscores under object instance names: */
            space_height = space_height
                           - 2.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size ) );
            if ( has_stereotype )
            {
                space_height = space_height
                               - pencil_size_get_font_line_gap( &((*this_).pencil_size ) )
                               - pencil_size_get_standard_font_size( &((*this_).pencil_size ) );
                /* this is an approximation to fix the differnce between font size and the actual line height */
                space_height = space_height
                               - 2.0 * pencil_size_get_font_line_gap( &((*this_).pencil_size ) );
            }
            double space_top = geometry_rectangle_get_bottom( classifier_bounds )
                               - space_height
                               - 2.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size ) );
            geometry_rectangle_reinit( classifier_space, space_left, space_top, space_width, space_height );
        }
    }

    /* calculate the relationship shapes */
    {
        uint32_t count_relations;
        count_relations = pencil_input_data_get_relationship_count ( (*this_).input_data );

        for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
        {
            data_relationship_t *current_relation;
            current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, rel_index );

            geometry_connector_t *relationship_shape;
            relationship_shape = pencil_input_data_layout_get_relationship_shape_ptr( &((*this_).layout_data), rel_index );

            int32_t source_index;
            int32_t dest_index;
            source_index = pencil_input_data_get_classifier_index( (*this_).input_data,
                                                                   data_relationship_get_from_classifier_id(current_relation)
                                                                 );
            dest_index = pencil_input_data_get_classifier_index( (*this_).input_data,
                                                                 data_relationship_get_to_classifier_id(current_relation)
                                                               );
            geometry_rectangle_t *source_rect;
            geometry_rectangle_t *dest_rect;
            if (( -1 != source_index ) && ( -1 != dest_index ))
            {
                source_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), source_index );
                dest_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), dest_index );
                pencil_layouter_private_connect_rectangles( this_, source_rect, dest_rect, relationship_shape );
            }
            else if ( -1 != source_index )
            {
                source_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), source_index );
                double y_coord = geometry_rectangle_get_top(source_rect) + 0.5*geometry_rectangle_get_height(source_rect);
                geometry_connector_reinit_horizontal ( relationship_shape,
                                                       geometry_rectangle_get_right(source_rect),
                                                       y_coord,
                                                       geometry_rectangle_get_right(source_rect) + 20.0,
                                                       y_coord,
                                                       y_coord
                                                     );
            }
            else if ( -1 != dest_index )
            {
                dest_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), dest_index );
                double y_coord = geometry_rectangle_get_top(dest_rect) + 0.5*geometry_rectangle_get_height(dest_rect);
                geometry_connector_reinit_horizontal ( relationship_shape,
                                                       geometry_rectangle_get_left(dest_rect) - 20.0,
                                                       y_coord,
                                                       geometry_rectangle_get_left(dest_rect),
                                                       y_coord,
                                                       y_coord
                                                     );
            }
            else
            {
                TSLOG_ERROR( "relationship has neither a source nor a destination in the current diagram." );
            }
        }
    }

    TRACE_END();
}

void pencil_layouter_private_connect_rectangles ( pencil_layouter_t *this_,
                                                  const geometry_rectangle_t *source_rect,
                                                  const geometry_rectangle_t *dest_rect,
                                                  geometry_connector_t *out_connection )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert( NULL != out_connection );

    double x_dist;
    double y_dist;

    if ( geometry_rectangle_get_left(source_rect) > geometry_rectangle_get_right(dest_rect) )
    {
        /* source is right of destination, x_dist is negative */
        x_dist = geometry_rectangle_get_right(dest_rect) - geometry_rectangle_get_left(source_rect);
    }
    else if ( geometry_rectangle_get_right(source_rect) < geometry_rectangle_get_left(dest_rect) )
    {
        /* source is left of destination, x_dist is positive */
        x_dist = geometry_rectangle_get_left(dest_rect) - geometry_rectangle_get_right(source_rect);
    }
    else
    {
        /* overlap */
        x_dist = 0.0;
    }

    if ( geometry_rectangle_get_top(source_rect) > geometry_rectangle_get_bottom(dest_rect) )
    {
        /* source is below destination, y_dist is negative */
        y_dist = geometry_rectangle_get_bottom(dest_rect) - geometry_rectangle_get_top(source_rect);
    }
    else if ( geometry_rectangle_get_bottom(source_rect) < geometry_rectangle_get_top(dest_rect) )
    {
        /* source is atop of destination, y_dist is positive */
        y_dist = geometry_rectangle_get_top(dest_rect) - geometry_rectangle_get_bottom(source_rect);
    }
    else
    {
        /* overlap */
        y_dist = 0.0;
    }

    if ( fabs(x_dist) > fabs(y_dist) )
    {
        /* main line is vertical */
        if ( x_dist < 0.0 )
        {
            geometry_connector_reinit_vertical ( out_connection,
                                                 geometry_rectangle_get_left(source_rect),
                                                 geometry_rectangle_get_y_center(source_rect),
                                                 geometry_rectangle_get_right(dest_rect),
                                                 geometry_rectangle_get_y_center(dest_rect),
                                                 0.5*( geometry_rectangle_get_left(source_rect)+geometry_rectangle_get_right(dest_rect) )
                                               );
        }
        else
        {
            geometry_connector_reinit_vertical ( out_connection,
                                                 geometry_rectangle_get_right(source_rect),
                                                 geometry_rectangle_get_y_center(source_rect),
                                                 geometry_rectangle_get_left(dest_rect),
                                                 geometry_rectangle_get_y_center(dest_rect),
                                                 0.5*( geometry_rectangle_get_right(source_rect)+geometry_rectangle_get_left(dest_rect) )
                                               );
        }
    }
    else
    {
        /* main line is horizontal */
        if ( y_dist < 0.0 )
        {
            geometry_connector_reinit_horizontal ( out_connection,
                                                   geometry_rectangle_get_x_center(source_rect),
                                                   geometry_rectangle_get_top(source_rect),
                                                   geometry_rectangle_get_x_center(dest_rect),
                                                   geometry_rectangle_get_bottom(dest_rect),
                                                   0.5*( geometry_rectangle_get_top(source_rect)+geometry_rectangle_get_bottom(dest_rect) )
                                                 );
        }
        else
        {
            geometry_connector_reinit_horizontal ( out_connection,
                                                   geometry_rectangle_get_x_center(source_rect),
                                                   geometry_rectangle_get_bottom(source_rect),
                                                   geometry_rectangle_get_x_center(dest_rect),
                                                   geometry_rectangle_get_top(dest_rect),
                                                   0.5*( geometry_rectangle_get_bottom(source_rect)+geometry_rectangle_get_top(dest_rect) )
                                                 );
        }

    }

    TRACE_END();
}

data_id_t pencil_layouter_get_object_id_at_pos ( pencil_layouter_t *this_,
                                                 double x,
                                                 double y,
                                                 bool dereference )
{
    TRACE_BEGIN();

    data_id_t result;
    data_id_init_void( &result );
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );

    if ( geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) && data_diagram_is_valid(diag) )
    {
        /* check the relationship shapes */
        {
            uint32_t count_relations;
            count_relations = pencil_input_data_get_relationship_count ( (*this_).input_data );

            for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
            {
                geometry_connector_t *relationship_shape;
                relationship_shape = pencil_input_data_layout_get_relationship_shape_ptr( &((*this_).layout_data), rel_index );

                if ( geometry_connector_is_close( relationship_shape, x, y, 3.0 /*=distance*/ ) )
                {
                    data_relationship_t *current_relation;
                    current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, rel_index );

                    data_id_reinit( &result, DATA_TABLE_RELATIONSHIP, data_relationship_get_id( current_relation ) );
                }
            }
        }

        /* determine a classifier at the given position */
        if ( ! data_id_is_valid( &result ) )
        {
            result = pencil_layouter_private_get_classifier_id_at_pos( this_, x, y, dereference );
        }

        /* fallback: return the diagram */
        if ( ! data_id_is_valid( &result ) )
        {
            data_id_reinit( &result, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) );
        }
    }
    else
    {
        TRACE_INFO( "no object at given location or no diagram chosen" );
        data_id_init_void( &result );
    }




    TRACE_END();
    return result;
}

data_id_t pencil_layouter_private_get_classifier_id_at_pos ( pencil_layouter_t *this_,
                                                             double x,
                                                             double y,
                                                             bool dereference )
{
    TRACE_BEGIN();
    data_id_t result;
    data_id_init_void( &result );

    if ( geometry_rectangle_contains( &((*this_).diagram_draw_area), x, y ) )
    {
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

                geometry_rectangle_t *classifier_bounds;
                geometry_rectangle_t *classifier_space;
                classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr ( &((*this_).layout_data), index );
                classifier_space = pencil_input_data_layout_get_classifier_space_ptr ( &((*this_).layout_data), index );

                if ( geometry_rectangle_contains( classifier_bounds, x, y ) )
                {
                    if ( geometry_rectangle_contains( classifier_space, x, y ) )
                    {
                        /* check all contained features */
                        uint32_t linenumber = 0;
                        double lineheight = pencil_size_get_standard_font_size( &((*this_).pencil_size) )
                        + 2.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size) );
                        for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count ( (*this_).input_data ); f_idx ++ )
                        {
                            data_feature_t *the_feature;
                            the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, f_idx );
                            if ( data_feature_get_classifier_id( the_feature ) == data_classifier_get_id( classifier ) )
                            {
                                geometry_rectangle_t feature_bounds;
                                feature_bounds = pencil_layouter_get_feature_bounds( this_,
                                                                                     data_classifier_get_id( classifier ),
                                                                                     index,
                                                                                     f_idx,
                                                                                     linenumber
                                                                                   );
                                if ( geometry_rectangle_contains( &feature_bounds, x, y ) )
                                {
                                    data_id_reinit( &result, DATA_TABLE_FEATURE, data_feature_get_id( the_feature ) );
                                }
                                linenumber ++;
                                geometry_rectangle_destroy( &feature_bounds );
                            }
                        }
                    }
                    if ( ! data_id_is_valid( &result ) )
                    {
                        if ( dereference )
                        {
                            data_id_reinit( &result, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );
                        }
                        else
                        {
                            data_id_reinit( &result, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) );
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR("invalid visible classifier in array!");
            }
        }
    }

    TRACE_END();
    return result;
}


/*
Copyright 2017-2017 Andreas Warnke

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
