/* File: pencil_rel_label_layouter.c; Copyright and License: see below */

#include "pencil_rel_label_layouter.h"
#include "trace.h"
#include "util/string/utf8string.h"

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

    TRACE_END();
}

void pencil_rel_label_layouter_destroy( pencil_rel_label_layouter_t *this_ )
{
    TRACE_BEGIN();

    draw_relationship_label_destroy( &((*this_).draw_relationship_label) );

    TRACE_END();
}

void pencil_rel_label_layouter_do_layout ( pencil_rel_label_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the relationships by their label-box layouting needs, drop invisible relations */
    pencil_rel_label_layouter_private_propose_processing_order ( this_, &sorted );

    /* layout the relationship label-boxes */
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* determine pointer to relationship */
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        layout_relationship_t *current_relation;
        current_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 8;
        geometry_rectangle_t solution[8];

        /* propose options */
        pencil_rel_label_layouter_private_propose_solutions ( this_,
                                                              current_relation,
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
            pencil_rel_label_layouter_private_select_solution ( this_,
                                                                current_relation,
                                                                solutions_count,
                                                                solution,
                                                                &index_of_best
                                                              );
        }

        /* store best option to (*this_).layout_data */
        geometry_rectangle_t *relationship_label_box;
        relationship_label_box = layout_relationship_get_label_box_ptr( current_relation );
        geometry_rectangle_copy( relationship_label_box, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_rel_label_layouter_private_propose_processing_order ( pencil_rel_label_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert( NULL != out_sorted );

    /* sort the relationships by their label-box: the less simple, the earlier it shall be processed */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        const layout_relationship_t *current_relation;
        current_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
        const data_relationship_t *relation_data = layout_relationship_get_data_ptr ( current_relation );
        assert( NULL != relation_data );

        int64_t simpleness = 0;

        /* determine simpleness by length of label */
        simpleness -= utf8string_get_length( data_relationship_get_name_ptr( relation_data ) );

        /* insert relation to sorted array, the simpler the more to the back */
        if ( PENCIL_VISIBILITY_SHOW == layout_relationship_get_visibility ( current_relation ) )
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
                                                           uint32_t solutions_max,
                                                           geometry_rectangle_t out_solutions[],
                                                           uint32_t *out_solutions_count)
{
    TRACE_BEGIN();
    assert( NULL != current_relation );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    {
        /* dummy box */
        assert( solutions_max >= 1 );
        const geometry_connector_t * shape = layout_relationship_get_shape_ptr ( current_relation );

        geometry_rectangle_init( &(out_solutions[0]),
                                 geometry_connector_get_main_line_source_x(shape) + 20,
                                 geometry_connector_get_main_line_source_y(shape) + 20,
                                 100,
                                 20
                               );
        *out_solutions_count = 1;

    }

    TRACE_END();
}

void pencil_rel_label_layouter_private_select_solution ( pencil_rel_label_layouter_t *this_,
                                                         layout_relationship_t *current_relation,
                                                         uint32_t solutions_count,
                                                         const geometry_rectangle_t solutions[],
                                                         uint32_t *out_index_of_best)
{
    TRACE_BEGIN();
    assert( NULL != current_relation );
    assert( NULL != solutions );
    assert( solutions_count >= 1 );
    assert( NULL != out_index_of_best );

    static unsigned int random;
    random ++;
    *out_index_of_best = random % solutions_count;

    TRACE_END();
}


/*
Copyright 2019-2019 Andreas Warnke

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
