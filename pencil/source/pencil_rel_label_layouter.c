/* File: pencil_rel_label_layouter.c; Copyright and License: see below */

#include "pencil_rel_label_layouter.h"
#include "geometry/geometry_point.h"
#include "geometry/geometry_direction.h"
#include "trace/trace.h"
#include "utf8stringbuf/utf8string.h"

void pencil_rel_label_layouter_init( pencil_rel_label_layouter_t *this_,
                                     pencil_layout_data_t *layout_data,
                                     pencil_size_t *pencil_size
                                   )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;
    draw_relationship_label_init( &((*this_).draw_relationship_label) );
    pencil_label_layout_helper_init ( &((*this_).label_layout_helper) );

    TRACE_END();
}

void pencil_rel_label_layouter_destroy( pencil_rel_label_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_label_layout_helper_destroy ( &((*this_).label_layout_helper) );
    draw_relationship_label_destroy( &((*this_).draw_relationship_label) );

    TRACE_END();
}

void pencil_rel_label_layouter_do_layout ( pencil_rel_label_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );
    assert( NULL != font_layout );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the relationships by their label-box layouting needs, drop invisible relations */
    pencil_rel_label_layouter_private_propose_processing_order ( this_, &sorted );

    /* layout the relationship label-boxes */
    const uint32_t count_sorted
        = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* determine pointer to relationship */
        const uint32_t index
            = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        layout_relationship_t *const current_relation
            = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
        geometry_point_t relation_middle = layout_relationship_get_middle ( current_relation );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 16;
        geometry_rectangle_t solution[16];

        /* propose options */
        pencil_rel_label_layouter_private_propose_solutions ( this_,
                                                              current_relation,
                                                              font_layout,
                                                              SOLUTIONS_MAX,
                                                              solution,
                                                              &solutions_count
                                                            );

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_label_layout_helper_select_solution ( &((*this_).label_layout_helper),
                                                         (*this_).layout_data,
                                                         relation_middle,
                                                         solutions_count,
                                                         solution,
                                                         &index_of_best
                                                       );
        }

        /* store best option to (*this_).layout_data */
        layout_relationship_set_label_box( current_relation, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_rel_label_layouter_private_propose_processing_order ( pencil_rel_label_layouter_t *this_,
                                                                  universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert( NULL != out_sorted );

    /* sort the relationships by their label-box: the less simple, the earlier it shall be processed */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        const layout_relationship_t *const current_relation
            = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
        const data_relationship_t *const relation_data
            = layout_relationship_get_data_const ( current_relation );
        assert( NULL != relation_data );

        int64_t simpleness = 0;

        /* determine simpleness by length of label */
        simpleness -= utf8string_get_length( data_relationship_get_name_const( relation_data ) );

        /* insert relation to sorted array, the simpler the more to the back */
        if (( PENCIL_VISIBILITY_SHOW == layout_relationship_get_visibility ( current_relation ) )
            || ( PENCIL_VISIBILITY_GRAY_OUT == layout_relationship_get_visibility ( current_relation ) ))
        {
            int insert_error;
            insert_error = universal_array_index_sorter_insert( out_sorted, index, simpleness );
            if ( 0 != insert_error )
            {
                TSLOG_WARNING( "not all relationship label-boxes are layouted" );
            }
        }
    }

    TRACE_END();
}

void pencil_rel_label_layouter_private_propose_solutions ( pencil_rel_label_layouter_t *this_,
                                                           layout_relationship_t *current_relation,
                                                           PangoLayout *font_layout,
                                                           uint32_t solutions_max,
                                                           geometry_rectangle_t out_solutions[],
                                                           uint32_t *out_solutions_count)
{
    TRACE_BEGIN();
    assert( NULL != current_relation );
    assert( NULL != font_layout );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    const data_relationship_t *the_relationship = layout_relationship_get_data_const( current_relation );

    {
        /* determine label dimensions */
        double text_width;
        double text_height;
        draw_relationship_label_get_type_and_name_dimensions ( &((*this_).draw_relationship_label),
                                                               the_relationship,
                                                               (*this_).pencil_size,
                                                               font_layout,
                                                               &text_width,
                                                               &text_height
                                                             );

        /* get layout data */
        const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );
        const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

        /* get connector data */
        const geometry_connector_t *const shape = layout_relationship_get_shape_const ( current_relation );
        const double source_end_x = geometry_connector_get_source_end_x ( shape );
        const double source_end_y = geometry_connector_get_source_end_y ( shape );
        const double main_line_source_x = geometry_connector_get_main_line_source_x ( shape );
        const double main_line_source_y = geometry_connector_get_main_line_source_y ( shape );
        const double main_line_destination_x = geometry_connector_get_main_line_destination_x ( shape );
        const double main_line_destination_y = geometry_connector_get_main_line_destination_y ( shape );
        const double destination_end_x = geometry_connector_get_destination_end_x ( shape );
        const double destination_end_y = geometry_connector_get_destination_end_y ( shape );
        geometry_point_t src_end;
        geometry_point_t main_src;
        geometry_point_t main_dst;
        geometry_point_t dst_end;
        geometry_point_init ( &src_end, source_end_x, source_end_y );
        geometry_point_init ( &main_src, main_line_source_x, main_line_source_y );
        geometry_point_init ( &main_dst, main_line_destination_x, main_line_destination_y );
        geometry_point_init ( &dst_end, destination_end_x, destination_end_y );
        const geometry_direction_t src_dir = geometry_point_get_direction ( &src_end, &main_src );
        const geometry_direction_t dst_dir = geometry_point_get_direction ( &main_dst, &dst_end );
        geometry_rectangle_t main_line_rect;
        geometry_rectangle_init_by_corners ( &main_line_rect, main_line_source_x, main_line_source_y, main_line_destination_x, main_line_destination_y );

        /* propose solutions */
        assert( solutions_max >= 16 );
        uint32_t solution_idx = 0;

        /* there are 0..2 solutions at the src line segment */
        if ( geometry_point_calc_chess_distance( &src_end, &main_src ) > object_dist )
        {
            /* this is a noteworthy line segment */
            if ( ( src_dir == GEOMETRY_DIRECTION_UP ) || ( src_dir == GEOMETRY_DIRECTION_DOWN ) )
            {
                /* right */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         main_line_source_x + gap,
                                         (source_end_y + main_line_source_y - text_height) / 2.0,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;

                /* left */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         main_line_source_x - text_width - gap,
                                         (source_end_y + main_line_source_y - text_height) / 2.0,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;
            }
            else
            {
                /* down */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         (source_end_x + main_line_source_x - text_width) / 2.0,
                                         main_line_source_y + gap,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;

                /* up */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         (source_end_x + main_line_source_x - text_width) / 2.0,
                                         main_line_source_y - text_height - gap,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;

            }
        }

        /* there are 4 solutions at the main line segment */
        {
            /* left */
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     geometry_rectangle_get_left(&main_line_rect) - text_width - gap,
                                     (main_line_source_y + main_line_destination_y - text_height) / 2.0,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;

            /* right */
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     geometry_rectangle_get_right(&main_line_rect) + gap,
                                     (main_line_source_y + main_line_destination_y - text_height) / 2.0,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;

            /* up */
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     (main_line_source_x + main_line_destination_x - text_width) / 2.0,
                                     geometry_rectangle_get_top(&main_line_rect) - text_height - gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;

            /* down */
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     (main_line_source_x + main_line_destination_x - text_width) / 2.0,
                                     geometry_rectangle_get_bottom(&main_line_rect) + gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
        }

        /* there are 0..2 solutions at the dst line segment */
        if ( geometry_point_calc_chess_distance( &main_dst, &dst_end ) > object_dist )
        {
            /* this is a noteworthy line segment */
            if ( ( dst_dir == GEOMETRY_DIRECTION_UP ) || ( dst_dir == GEOMETRY_DIRECTION_DOWN ) )
            {
                /* right */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         main_line_destination_x + gap,
                                         (destination_end_y + main_line_destination_y - text_height) / 2.0,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;

                /* left */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         main_line_destination_x - text_width - gap,
                                         (destination_end_y + main_line_destination_y - text_height) / 2.0,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;
            }
            else
            {
                /* down */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         (destination_end_x + main_line_destination_x - text_width) / 2.0,
                                         main_line_destination_y + gap,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;

                /* up */
                geometry_rectangle_init( &(out_solutions[solution_idx]),
                                         (destination_end_x + main_line_destination_x - text_width) / 2.0,
                                         main_line_destination_y - text_height - gap,
                                         text_width,
                                         text_height
                                       );
                solution_idx ++;
            }
        }

        /* 0..4 at main line source if source end line exists */
        if ( src_dir != GEOMETRY_DIRECTION_CENTER )
        {
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_source_x - text_width - gap,
                                     main_line_source_y - text_height - gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_source_x - text_width - gap,
                                     main_line_source_y + gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_source_x + gap,
                                     main_line_source_y - text_height - gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_source_x + gap,
                                     main_line_source_y + gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
        }

        /* 0..4 at main line destination if destination end line exists */
        if ( dst_dir != GEOMETRY_DIRECTION_CENTER )
        {
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_destination_x - text_width - gap,
                                     main_line_destination_y - text_height - gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_destination_x - text_width - gap,
                                     main_line_destination_y + gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_destination_x + gap,
                                     main_line_destination_y - text_height - gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
            geometry_rectangle_init( &(out_solutions[solution_idx]),
                                     main_line_destination_x + gap,
                                     main_line_destination_y + gap,
                                     text_width,
                                     text_height
                                   );
            solution_idx ++;
        }

        assert( solution_idx > 0 );
        assert( solution_idx <= solutions_max );
        *out_solutions_count = solution_idx;
    }

    TRACE_END();
}


/*
Copyright 2019-2022 Andreas Warnke

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
