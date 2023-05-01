/* File: pencil_relationship_1d_layouter.c; Copyright and License: see below */

#include "pencil_relationship_1d_layouter.h"
#include "u8/u8_trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

void pencil_relationship_1d_layouter_init( pencil_relationship_1d_layouter_t *this_,
                                           pencil_layout_data_t *layout_data,
                                           const data_profile_part_t *profile,
                                           const pencil_size_t *pencil_size )
{
    U8_TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != profile );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).profile = profile;
    (*this_).pencil_size = pencil_size;

    pencil_relationship_painter_init( &((*this_).relationship_painter) );

    U8_TRACE_END();
}

void pencil_relationship_1d_layouter_destroy( pencil_relationship_1d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );

    U8_TRACE_END();
}

void pencil_relationship_1d_layouter_private_make_all_visible ( pencil_relationship_1d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    /* determine visibility */
    const uint32_t count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *const the_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
        const layout_visible_classifier_t *const from_layout = layout_relationship_get_from_classifier_ptr ( the_relation );
        const layout_visible_classifier_t *const to_layout = layout_relationship_get_to_classifier_ptr ( the_relation );
        assert( from_layout != NULL );
        assert( to_layout != NULL );
        const data_visible_classifier_t *const from_data = layout_visible_classifier_get_data_const( from_layout );
        const data_visible_classifier_t *const to_data = layout_visible_classifier_get_data_const( to_layout );
        const data_diagramelement_t *const from_diagele = data_visible_classifier_get_diagramelement_const( from_data );
        const data_diagramelement_t *const to_diagele = data_visible_classifier_get_diagramelement_const( to_data );
        const data_diagramelement_flag_t from_flags = data_diagramelement_get_display_flags ( from_diagele );
        const data_diagramelement_flag_t to_flags = data_diagramelement_get_display_flags ( to_diagele );
        if (( 0 != ( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT & from_flags ))
            || ( 0 != ( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT & to_flags )))
        {
            layout_relationship_set_visibility ( the_relation, PENCIL_VISIBILITY_GRAY_OUT );
        }
        else
        {
            layout_relationship_set_visibility ( the_relation, PENCIL_VISIBILITY_SHOW );
        }
    }

    U8_TRACE_END();
}

void pencil_relationship_1d_layouter_layout_for_sequence( pencil_relationship_1d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_relationship_1d_layouter_private_make_all_visible( this_ );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_const( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* layout the relationships */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    U8_TRACE_INFO_INT( "count_relations:", count_relations );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /* get the relationship to layout */
        layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );

        /* adjust visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }

        /* calculate layout */
        {
            /* determine y-coordinate */
            const data_relationship_t *const the_relationdata
                = layout_relationship_get_data_const( the_relationship );
            int32_t list_order = data_relationship_get_list_order( the_relationdata );
            const double y_value_rel = (list_order/((double)UINT32_MAX))+0.5;
            const double draw_top = geometry_rectangle_get_top(diagram_draw_area);
            const double draw_bottom = geometry_rectangle_get_bottom(diagram_draw_area);
            const double y_value = ( (draw_bottom - draw_top) * y_value_rel ) + draw_top;

            /* get source and destination rectangles */
            const geometry_rectangle_t *const source_rect
                = layout_relationship_get_from_symbol_box_const ( the_relationship );
            const geometry_rectangle_t *const dest_rect
                = layout_relationship_get_to_symbol_box_const ( the_relationship );

            /* calculate coordinates */
            const double src_left = geometry_rectangle_get_left(source_rect);
            const double src_center_x = geometry_rectangle_get_center_x(source_rect);
            const double src_right = geometry_rectangle_get_right(source_rect);
            const double src_top = geometry_rectangle_get_top(source_rect);
            /*const double src_center_y = geometry_rectangle_get_center_y(source_rect);*/
            const double src_bottom = geometry_rectangle_get_bottom(source_rect);

            const double dst_left = geometry_rectangle_get_left(dest_rect);
            const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);
            const double dst_right = geometry_rectangle_get_right(dest_rect);
            const double dst_top = geometry_rectangle_get_top(dest_rect);
            /*const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);*/
            const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

            /* is interaction relation */
            const data_relationship_type_t rel_type
                = data_relationship_get_main_type( the_relationdata );
            const bool is_interaction
                = ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL );

            const double src_from_y
               = ( y_value < src_top ) ? src_top : ( y_value > src_bottom ) ? src_bottom : y_value;
            const double dst_to_y
               = ( y_value < dst_top ) ? dst_top : ( y_value > dst_bottom ) ? dst_bottom : y_value;
            const double src_from_x
                = (( y_value < src_top )||( y_value > src_bottom )|| is_interaction )
                ? src_center_x
                : ( src_center_x < dst_center_x )
                ? src_right
                : src_left;
            const double dst_to_x
                = (( y_value < dst_top )||( y_value > dst_bottom )|| is_interaction )
                ? dst_center_x
                : ( src_center_x < dst_center_x )
                ? dst_left
                : dst_right;

            /* determine minimum arrow size for message/call to self */
            const double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

            /* define relation */
            geometry_connector_t relationship_shape;
            if ( fabs( src_center_x - dst_center_x ) < 0.0001 )
            {
                /* message/call to self */
                geometry_connector_init_vertical ( &relationship_shape,
                                                   src_center_x,
                                                   src_from_y - (good_dist/2.0),
                                                   dst_center_x,
                                                   dst_to_y + (good_dist/2.0),
                                                   src_center_x + good_dist /* the main connector shall be right to the start/end points */
                                                 );
            }
            else
            {
                /* normal message/call */
                geometry_connector_init_horizontal ( &relationship_shape,
                                                     src_from_x,
                                                     src_from_y,
                                                     dst_to_x,
                                                     dst_to_y,
                                                     y_value
                                                   );
            }
            layout_relationship_set_shape( the_relationship, &relationship_shape );
            geometry_connector_destroy( &relationship_shape );
        }
    }

    U8_TRACE_END();
}

void pencil_relationship_1d_layouter_layout_for_timing( pencil_relationship_1d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_relationship_1d_layouter_private_make_all_visible( this_ );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* layout the relationships */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    U8_TRACE_INFO_INT( "count_relations:", count_relations );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /* get the relationship to layout */
        layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );

        /* adjust visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }

        /* calculate layout */
        {
            /* determine x-coordinate */
            const data_relationship_t *const the_relationdata
               = layout_relationship_get_data_const( the_relationship );
            const int32_t list_order = data_relationship_get_list_order ( the_relationdata );
            const double x_value_rel = (list_order/((double)UINT32_MAX))+0.5;
            const double draw_left = geometry_rectangle_get_left(diagram_draw_area);
            const double draw_right = geometry_rectangle_get_right(diagram_draw_area);
            const double x_value = ( (draw_right - draw_left) * x_value_rel ) + draw_left;

            /* get source and destination rectangles */
            const geometry_rectangle_t *const source_rect
                = layout_relationship_get_from_symbol_box_const( the_relationship );
            const geometry_rectangle_t *const dest_rect
                = layout_relationship_get_to_symbol_box_const( the_relationship );

            /* calculate coordinates */
            const double src_left = geometry_rectangle_get_left(source_rect);
            /*const double src_center_x = geometry_rectangle_get_center_x(source_rect);*/
            const double src_right = geometry_rectangle_get_right(source_rect);
            const double src_top = geometry_rectangle_get_top(source_rect);
            const double src_center_y = geometry_rectangle_get_center_y(source_rect);
            const double src_bottom = geometry_rectangle_get_bottom(source_rect);

            const double dst_left = geometry_rectangle_get_left(dest_rect);
            /*const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);*/
            const double dst_right = geometry_rectangle_get_right(dest_rect);
            const double dst_top = geometry_rectangle_get_top(dest_rect);
            const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);
            const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

            /* is interaction relation */
            const data_relationship_type_t rel_type
                = data_relationship_get_main_type( the_relationdata );
            const bool is_interaction
                = ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL );

            const double src_from_x
                = ( x_value < src_left ) ? src_left : ( x_value > src_right ) ? src_right : x_value;
            const double dst_to_x
                = ( x_value < dst_left ) ? dst_left : ( x_value > dst_right ) ? dst_right : x_value;
            const double src_from_y
                = (( x_value < src_left )||( x_value > src_right )|| is_interaction )
                ? src_center_y
                : ( src_center_y < dst_center_y )
                ? src_bottom
                : src_top;
            const double dst_to_y
                = (( x_value < dst_left )||( x_value > dst_right )|| is_interaction )
                ? dst_center_y
                : ( src_center_y < dst_center_y )
                ? dst_top
                : dst_bottom;

            /* determine minimum arrow size for self transition */
            const double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

            /* define relation */
            geometry_connector_t relationship_shape;
            if ( fabs( src_center_y - dst_center_y ) < 0.0001 )
            {
                /* transition to self */
                geometry_connector_init_horizontal ( &relationship_shape,
                                                     src_from_x - (good_dist/2.0),
                                                     src_center_y,
                                                     dst_to_x + (good_dist/2.0),
                                                     dst_center_y,
                                                     src_center_y - good_dist /* the main connector shall be above the start/end points */
                                                   );
            }
            else
            {
                /* normal transition */
                geometry_connector_init_vertical ( &relationship_shape,
                                                   src_from_x,
                                                   src_from_y,
                                                   dst_to_x,
                                                   dst_to_y,
                                                   x_value
                                                 );
            }
            layout_relationship_set_shape( the_relationship, &relationship_shape );
            geometry_connector_destroy( &relationship_shape );
        }
    }

    U8_TRACE_END();
}


/*
Copyright 2017-2023 Andreas Warnke

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
