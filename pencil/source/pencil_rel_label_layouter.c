/* File: pencil_rel_label_layouter.c; Copyright and License: see below */

#include "pencil_rel_label_layouter.h"
#include "layout/layout_relationship_iter.h"
#include "geometry/geometry_point.h"
#include "geometry/geometry_direction.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"

void pencil_rel_label_layouter_init( pencil_rel_label_layouter_t *this_,
                                     layout_visible_set_t *layout_data,
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
    draw_relationship_label_init( &((*this_).draw_relationship_label) );
    pencil_floating_label_layouter_init ( &((*this_).label_layout_helper), pencil_size );

    U8_TRACE_END();
}

void pencil_rel_label_layouter_reinit( pencil_rel_label_layouter_t *this_,
                                       layout_visible_set_t *layout_data,
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

    U8_TRACE_END();
}

void pencil_rel_label_layouter_destroy( pencil_rel_label_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_floating_label_layouter_destroy ( &((*this_).label_layout_helper) );
    draw_relationship_label_destroy( &((*this_).draw_relationship_label) );

    U8_TRACE_END();
}

void pencil_rel_label_layouter_do_layout( pencil_rel_label_layouter_t *this_, PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    assert( NULL != font_layout );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the relationships by their label-box layouting needs, drop invisible relations */
    pencil_rel_label_layouter_private_propose_processing_order ( this_, &sorted );

    /* layout the relationship label-boxes */
    layout_relationship_iter_t relationship_iterator;
    layout_relationship_iter_init( &relationship_iterator, (*this_).layout_data, &sorted );
    while ( layout_relationship_iter_has_next( &relationship_iterator ) )
    {
        /* determine pointer to relationship */
        layout_relationship_t *const current_relation = layout_relationship_iter_next_ptr( &relationship_iterator );
        geometry_point_t relation_middle = layout_relationship_get_middle ( current_relation );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 10;
        geometry_rectangle_t solution[10];

        /* propose options */
        pencil_rel_label_layouter_private_propose_solutions( this_,
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
            pencil_floating_label_layouter_select_solution( &((*this_).label_layout_helper),
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

    layout_relationship_iter_destroy( &relationship_iterator );
    universal_array_index_sorter_destroy( &sorted );

    U8_TRACE_END();
}

void pencil_rel_label_layouter_private_propose_processing_order( pencil_rel_label_layouter_t *this_,
                                                                 universal_array_index_sorter_t *out_sorted )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_sorted );

    /* sort the relationships by their label-box: the less simple, the earlier it shall be processed */
    const uint32_t count_relations
        = layout_visible_set_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        const layout_relationship_t *const current_relation
            = layout_visible_set_get_relationship_ptr ( (*this_).layout_data, index );
        const data_relationship_t *const relation_data
            = layout_relationship_get_data_const ( current_relation );
        assert( NULL != relation_data );

        int64_t simpleness = 0;

        /* determine simpleness by length of label */
        simpleness -= utf8string_get_length( data_relationship_get_name_const( relation_data ) );

        /* insert relation to sorted array, the simpler the more to the back */
        if ( PENCIL_VISIBILITY_HIDE != layout_relationship_get_visibility ( current_relation ) )
        {
            int insert_error;
            insert_error = universal_array_index_sorter_insert( out_sorted, index, simpleness );
            if ( 0 != insert_error )
            {
                U8_LOG_WARNING( "not all relationship label-boxes are layouted" );
            }
        }
    }

    U8_TRACE_END();
}

void pencil_rel_label_layouter_private_propose_solutions( pencil_rel_label_layouter_t *this_,
                                                          layout_relationship_t *current_relation,
                                                          PangoLayout *font_layout,
                                                          uint32_t solutions_max,
                                                          geometry_rectangle_t out_solutions[],
                                                          uint32_t *out_solutions_count )
{
    U8_TRACE_BEGIN();
    assert( NULL != current_relation );
    assert( NULL != font_layout );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    const data_relationship_t *the_relationship = layout_relationship_get_data_const( current_relation );
    {
        /* determine label dimensions */
        const geometry_dimensions_t label_dim_proposal = {
            .width = 30.0 * pencil_size_get_standard_font_size( (*this_).pencil_size ),
            .height = pencil_size_get_standard_font_size( (*this_).pencil_size )
        };
        geometry_dimensions_t preferred_label_dim;
        draw_relationship_label_get_type_and_name_dimensions( &((*this_).draw_relationship_label),
                                                              the_relationship,
                                                              (*this_).profile,
                                                              &label_dim_proposal,
                                                              (*this_).pencil_size,
                                                              font_layout,
                                                              &preferred_label_dim
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
        const geometry_3dir_t connector_dirs = geometry_connector_get_directions( shape );
        const geometry_direction_t src_dir = geometry_3dir_get_first ( &connector_dirs );
        const geometry_direction_t main_dir = geometry_3dir_get_second ( &connector_dirs );
        const geometry_direction_t dst_dir = geometry_3dir_get_third ( &connector_dirs );

        /* propose solutions */
        assert( solutions_max >= 10 );
        uint32_t solution_idx = 0;

        /* there are 0 or 3 solutions at the src line segment */
        if ( geometry_point_calc_chess_distance( &src_end, &main_src ) > object_dist )
        {
            /* this is a noteworthy line segment */
            geometry_anchor_t anchor_1;
            geometry_anchor_t anchor_2;
            geometry_anchor_t anchor_3;
            geometry_anchor_t anchor_4;

            if ( ( src_dir == GEOMETRY_DIRECTION_UP ) || ( src_dir == GEOMETRY_DIRECTION_DOWN ) )
            {
                /* right */
                geometry_anchor_init( &anchor_1,
                                      main_line_source_x + gap,
                                      (source_end_y + main_line_source_y) / 2.0,
                                      GEOMETRY_H_ALIGN_LEFT,  /* the reference point is the left side of the label */
                                      GEOMETRY_V_ALIGN_CENTER
                                    );

                /* left */
                geometry_anchor_init( &anchor_2,
                                      main_line_source_x - gap,
                                      (source_end_y + main_line_source_y) / 2.0,
                                      GEOMETRY_H_ALIGN_RIGHT,  /* the reference point is the right side of the label */
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
                /* at bend to main line */
                geometry_anchor_init( &anchor_3,
                                      main_line_source_x,
                                      main_line_source_y + (( src_dir == GEOMETRY_DIRECTION_UP ) ? (-gap) : gap ),
                                      GEOMETRY_H_ALIGN_CENTER,
                                      ( src_dir == GEOMETRY_DIRECTION_UP ) ? GEOMETRY_V_ALIGN_BOTTOM : GEOMETRY_V_ALIGN_TOP
                                    );
                geometry_anchor_init( &anchor_4,
                                      main_line_source_x + (( main_dir == GEOMETRY_DIRECTION_LEFT ) ? gap : (-gap) ),
                                      main_line_source_y,
                                      ( main_dir == GEOMETRY_DIRECTION_LEFT ) ? GEOMETRY_H_ALIGN_LEFT : GEOMETRY_H_ALIGN_RIGHT,
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
            }
            else
            {
                /* down */
                geometry_anchor_init( &anchor_1,
                                      (source_end_x + main_line_source_x) / 2.0,
                                      main_line_source_y + gap,
                                      GEOMETRY_H_ALIGN_CENTER,
                                      GEOMETRY_V_ALIGN_TOP  /* the reference point is the top of the label */
                                    );

                /* up */
                geometry_anchor_init( &anchor_2,
                                      (source_end_x + main_line_source_x) / 2.0,
                                      main_line_source_y - gap,
                                      GEOMETRY_H_ALIGN_CENTER,
                                      GEOMETRY_V_ALIGN_BOTTOM  /* the reference point is the bottom of the label */
                                    );
                /* at bend to main line */
                geometry_anchor_init( &anchor_3,
                                      main_line_source_x + (( src_dir == GEOMETRY_DIRECTION_LEFT ) ? (-gap) : gap ),
                                      main_line_source_y,
                                      ( src_dir == GEOMETRY_DIRECTION_LEFT ) ? GEOMETRY_H_ALIGN_RIGHT : GEOMETRY_H_ALIGN_LEFT,
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
                geometry_anchor_init( &anchor_4,
                                      main_line_source_x,
                                      main_line_source_y + (( main_dir == GEOMETRY_DIRECTION_UP ) ? gap : (-gap) ),
                                      GEOMETRY_H_ALIGN_CENTER,
                                      ( main_dir == GEOMETRY_DIRECTION_UP ) ? GEOMETRY_V_ALIGN_TOP : GEOMETRY_V_ALIGN_BOTTOM
                                    );
            }

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_1,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_2,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_3,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_4,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;
        }

        /* there are 2 solutions at the main line segment */
        {
            geometry_anchor_t anchor_5;
            geometry_anchor_t anchor_6;

            if ( ( main_dir == GEOMETRY_DIRECTION_UP ) || ( main_dir == GEOMETRY_DIRECTION_DOWN ) )
            {
                geometry_anchor_init( &anchor_5,
                                      main_line_source_x + gap,
                                      (main_line_source_y + main_line_destination_y) / 2.0,
                                      GEOMETRY_H_ALIGN_LEFT,  /* the reference point is the left side of the label */
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
                geometry_anchor_init( &anchor_6,
                                      main_line_source_x - gap,
                                      (main_line_source_y + main_line_destination_y) / 2.0,
                                      GEOMETRY_H_ALIGN_RIGHT,  /* the reference point is the right side of the label */
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
            }
            else
            {
                geometry_anchor_init( &anchor_5,
                                      (main_line_source_x + main_line_destination_x) / 2.0,
                                      main_line_source_y + gap,
                                      GEOMETRY_H_ALIGN_CENTER,
                                      GEOMETRY_V_ALIGN_TOP  /* the reference point is the top of the label */
                                    );
                geometry_anchor_init( &anchor_6,
                                      (main_line_source_x + main_line_destination_x) / 2.0,
                                      main_line_source_y - gap,
                                      GEOMETRY_H_ALIGN_CENTER,
                                      GEOMETRY_V_ALIGN_BOTTOM  /* the reference point is the bottom of the label */
                                    );
            }

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_5,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_6,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;
        }

        /* there are 0 or 3 solutions at the dst line segment */
        if ( geometry_point_calc_chess_distance( &main_dst, &dst_end ) > object_dist )
        {
            geometry_anchor_t anchor_7;
            geometry_anchor_t anchor_8;
            geometry_anchor_t anchor_9;
            geometry_anchor_t anchor_10;

            /* this is a noteworthy line segment */
            if ( ( dst_dir == GEOMETRY_DIRECTION_UP ) || ( dst_dir == GEOMETRY_DIRECTION_DOWN ) )
            {
                /* at bend to main line */
                geometry_anchor_init( &anchor_7,
                                      main_line_destination_x + (( main_dir == GEOMETRY_DIRECTION_LEFT ) ? (-gap) : gap ),
                                      main_line_destination_y,
                                      ( main_dir == GEOMETRY_DIRECTION_LEFT ) ? GEOMETRY_H_ALIGN_RIGHT : GEOMETRY_H_ALIGN_LEFT,
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
                geometry_anchor_init( &anchor_8,
                                      main_line_destination_x,
                                      main_line_destination_y + (( dst_dir == GEOMETRY_DIRECTION_UP ) ? gap : (-gap) ),
                                      GEOMETRY_H_ALIGN_CENTER,
                                      ( dst_dir == GEOMETRY_DIRECTION_UP ) ? GEOMETRY_V_ALIGN_TOP : GEOMETRY_V_ALIGN_BOTTOM
                                    );
                /* right */
                geometry_anchor_init( &anchor_9,
                                      main_line_destination_x + gap,
                                      (destination_end_y + main_line_destination_y) / 2.0,
                                      GEOMETRY_H_ALIGN_LEFT,  /* the reference point is the left side of the label */
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
                /* left */
                geometry_anchor_init( &anchor_10,
                                      main_line_destination_x - gap,
                                      (destination_end_y + main_line_destination_y) / 2.0,
                                      GEOMETRY_H_ALIGN_RIGHT,  /* the reference point is the right side of the label */
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
            }
            else
            {
                /* at bend to main line */
                geometry_anchor_init( &anchor_7,
                                      main_line_destination_x,
                                      main_line_destination_y + (( main_dir == GEOMETRY_DIRECTION_UP ) ? (-gap) : gap ),
                                      GEOMETRY_H_ALIGN_CENTER,
                                      ( main_dir == GEOMETRY_DIRECTION_UP ) ? GEOMETRY_V_ALIGN_BOTTOM : GEOMETRY_V_ALIGN_TOP
                                    );
                geometry_anchor_init( &anchor_8,
                                      main_line_destination_x + (( dst_dir == GEOMETRY_DIRECTION_LEFT ) ? gap : (-gap) ),
                                      main_line_destination_y,
                                      ( dst_dir == GEOMETRY_DIRECTION_LEFT ) ? GEOMETRY_H_ALIGN_LEFT : GEOMETRY_H_ALIGN_RIGHT,
                                      GEOMETRY_V_ALIGN_CENTER
                                    );
                /* down */
                geometry_anchor_init( &anchor_9,
                                      (destination_end_x + main_line_destination_x) / 2.0,
                                      main_line_destination_y + gap,
                                      GEOMETRY_H_ALIGN_CENTER,
                                      GEOMETRY_V_ALIGN_TOP  /* the reference point is the top of the label */
                                    );

                /* up */
                geometry_anchor_init( &anchor_10,
                                      (destination_end_x + main_line_destination_x) / 2.0,
                                      main_line_destination_y - gap,
                                      GEOMETRY_H_ALIGN_CENTER,
                                      GEOMETRY_V_ALIGN_BOTTOM  /* the reference point is the bottom of the label */
                                    );

            }

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_7,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_8,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_9,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;

            pencil_floating_label_layouter_propose_solution_rel( &((*this_).label_layout_helper),
                                                                 (*this_).layout_data,
                                                                 &anchor_10,
                                                                 &preferred_label_dim,
                                                                 &((*this_).draw_relationship_label),
                                                                 the_relationship,
                                                                 (*this_).profile,
                                                                 font_layout,
                                                                 &(out_solutions[solution_idx])
                                                               );
            solution_idx ++;
        }

        assert( solution_idx > 0 );
        assert( solution_idx <= solutions_max );
        assert(( solution_idx == 2 )||( solution_idx == 6 )||( solution_idx == 10 ));
        *out_solutions_count = solution_idx;
    }

    U8_TRACE_END();
}


/*
Copyright 2019-2025 Andreas Warnke

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
