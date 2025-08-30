/* File: gui_sketch_card_layouter.c; Copyright and License: see below */

#include "gui_sketch_card_layouter.h"
#include "sketch/gui_sketch_area.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void gui_sketch_card_layouter_init( gui_sketch_card_layouter_t *this_, shape_int_rectangle_t *bounds )
{
    U8_TRACE_BEGIN();
    assert( NULL != bounds );

    (*this_).bounds = *bounds;

    U8_TRACE_END();
}

void gui_sketch_card_layouter_destroy( gui_sketch_card_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

static const uint_fast32_t RATIO_WIDTH = 36;
static const uint_fast32_t RATIO_HEIGHT = 24;
static const uint_fast32_t BORDER = 8;

void gui_sketch_card_layouter_layout ( gui_sketch_card_layouter_t *this_,
                                       gui_tool_t selected_tool,
                                       gui_sketch_card_t io_cards[],
                                       uint32_t cards_num,
                                       const data_small_set_t *card_draw_list,
                                       cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != io_cards );
    assert( cards_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );

    /* fetch area bounds */
    const uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
    const uint_fast32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );
    const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    const int_fast32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
    U8_TRACE_INFO_INT_INT( "width, height", width, height );

    /* calculate card sizes */
    uint_fast32_t focus_card_height;
    uint_fast32_t focus_card_width;
    {
        const uint_fast32_t focus_card_w_space = (width * 7) / 10;
        const uint_fast32_t focus_card_h_space = (height * 4) / 10;
        if ( (focus_card_w_space * RATIO_HEIGHT) > (focus_card_h_space * RATIO_WIDTH) )
        {
            focus_card_height = focus_card_h_space;
            focus_card_width = (focus_card_height * RATIO_WIDTH) / RATIO_HEIGHT;
        }
        else
        {
            focus_card_width = focus_card_w_space;
            focus_card_height = (focus_card_width * RATIO_HEIGHT) / RATIO_WIDTH;
        }
    }
    const uint_fast32_t parent_card_height = (focus_card_height * 6) / 10;
    const uint_fast32_t parent_card_width = (focus_card_width * 6) / 10;

    /* layout cards */
    switch( selected_tool )
    {
        case GUI_TOOL_SEARCH:
        {
            shape_int_rectangle_t card_bounds;

            /* search results */
            {
                shape_int_rectangle_init( &card_bounds, left, top, width, height );
                gui_sketch_card_layouter_private_layout_to_grid( this_,
                                                                 &card_bounds,
                                                                 focus_card_height,
                                                                 io_cards,
                                                                 cards_num,
                                                                 cr
                                                               );
                /* hide cards that are not on current visible page */
                for ( uint_fast32_t idx = 0; idx < cards_num; idx ++ )
                {
                    gui_sketch_card_t* current_card = &(io_cards[idx]);
                    const data_id_t diag_id = gui_sketch_card_get_diagram_id( current_card );
                    const bool draw = data_small_set_contains( card_draw_list, diag_id );
                    gui_sketch_card_set_visible( current_card, draw );
                }
            }
        }
        break;

        case GUI_TOOL_NAVIGATE:
        {
            shape_int_rectangle_t card_bounds;

            /* self */
            assert(cards_num > GUI_SKETCH_AREA_CONST_FOCUSED_CARD);
            {
                const int_fast32_t self_left = (left + width) - focus_card_width - BORDER;
                const int_fast32_t self_top = top + BORDER + (parent_card_height/2);
                shape_int_rectangle_init( &card_bounds, self_left, self_top, focus_card_width, focus_card_height );
                shape_int_rectangle_trace( &card_bounds );
                gui_sketch_card_set_bounds( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), card_bounds );
                const bool valid_card = gui_sketch_card_is_valid( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]) );
                gui_sketch_card_do_layout( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), cr );
                gui_sketch_card_set_visible( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), valid_card );
            }

            /* parent */
            assert(cards_num > GUI_SKETCH_AREA_CONST_PARENT_CARD);
            {
                const int_fast32_t parent_left = left + BORDER;
                const int_fast32_t parent_top = top + BORDER;
                shape_int_rectangle_init( &card_bounds, parent_left, parent_top, parent_card_width, parent_card_height );
                shape_int_rectangle_trace( &card_bounds );
                gui_sketch_card_set_bounds( &(io_cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]), card_bounds );
                const bool valid_parent = gui_sketch_card_is_valid( &(io_cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]) );
                gui_sketch_card_do_layout( &(io_cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]), cr );
                gui_sketch_card_set_visible( &(io_cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]), valid_parent );
            }

            /* children */
            assert(cards_num >= GUI_SKETCH_AREA_CONST_FIRST_CHILD_CARD);
            {
                const int_fast32_t children_top = top + focus_card_height + (parent_card_height/2) + 2*BORDER;
                const uint_fast32_t children_height = (height > (children_top-top)) ? (height - (children_top-top)) : 0;
                shape_int_rectangle_init( &card_bounds, left, children_top, width, children_height );
                gui_sketch_card_layouter_private_layout_to_grid( this_,
                                                                 &card_bounds,
                                                                 parent_card_height,
                                                                 &(io_cards[GUI_SKETCH_AREA_CONST_FIRST_CHILD_CARD]),
                                                                 cards_num-GUI_SKETCH_AREA_CONST_FIRST_CHILD_CARD,
                                                                 cr
                                                               );
            }
        }
        break;

        case GUI_TOOL_EDIT:  /* or */
        case GUI_TOOL_CREATE:
        {
            shape_int_rectangle_t card_bounds;

            assert(cards_num == GUI_SKETCH_AREA_CONST_FOCUSED_CARD+1);
            {
                card_bounds = (*this_).bounds;
                shape_int_rectangle_shrink_by_border( &card_bounds, BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_CENTER, SHAPE_V_ALIGN_CENTER );
                shape_int_rectangle_trace( &card_bounds );
                gui_sketch_card_set_bounds( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), card_bounds );
                const bool valid_card = gui_sketch_card_is_valid( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]) );
                gui_sketch_card_do_layout( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), cr );
                gui_sketch_card_set_visible( &(io_cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), valid_card );
            }
        }
        break;

        default:
        {
            assert(false);
        }
        break;
    }

    U8_TRACE_END();
}

void gui_sketch_card_layouter_private_layout_to_grid ( gui_sketch_card_layouter_t *this_,
                                                       shape_int_rectangle_t *bounds,
                                                       uint_fast32_t max_card_height,
                                                       gui_sketch_card_t io_cards[],
                                                       uint_fast32_t cards_num,
                                                       cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != bounds );
    assert( NULL != io_cards );
    assert( cards_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );

    /* fetch outer bounds */
    const uint_fast32_t width = shape_int_rectangle_get_width( bounds );
    const uint_fast32_t height = shape_int_rectangle_get_height( bounds );
    const int_fast32_t left = shape_int_rectangle_get_left( bounds );
    const int_fast32_t top = shape_int_rectangle_get_top( bounds );

    /* determine number of columns and rows for layout */
    uint_fast32_t cols = 0;  /* initial value, overwritten if a card exists */
    uint_fast32_t rows = 0;  /* initial value, overwritten if a card exists */
    uint_fast32_t card_width = 0;  /* initial value, overwritten if a card exists */
    uint_fast32_t card_height = 0;  /* initial value, overwritten if a card exists */
    {
        for ( uint_fast32_t probe_rows = 1; probe_rows <= cards_num; probe_rows ++ )
        {
            assert( probe_rows > 0 );
            uint_fast32_t probe_cols = (cards_num + probe_rows - 1) / probe_rows;
            shape_int_rectangle_t probe_bounds;
            shape_int_rectangle_init( &probe_bounds, 0, 0, ((width*2)/((probe_cols*2)+1)), height/probe_rows );
            shape_int_rectangle_shrink_by_border( &probe_bounds, BORDER );
            shape_int_rectangle_shrink_to_ratio( &probe_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_LEFT, SHAPE_V_ALIGN_TOP );
            if ( card_height <= shape_int_rectangle_get_height( &probe_bounds ) )
            {
                cols = probe_cols;
                rows = probe_rows;
                card_width = shape_int_rectangle_get_width( &probe_bounds );
                card_height = shape_int_rectangle_get_height( &probe_bounds );
            }
        }
    }

    /* children shall not be bigger than the parent */
    if ( card_height > max_card_height )
    {
        assert( card_height > 0 );
        card_width = ( card_width * max_card_height ) / card_height;
        card_height = max_card_height;
    }

    /* layout cards to grid */
    for ( uint_fast32_t y = 0; y < rows; y ++ )
    {
        for ( uint_fast32_t x = 0; x < cols; x ++ )
        {
            uint_fast32_t card_idx = (y * cols) + x;
            if ( card_idx < cards_num )
            {
                assert( rows > 0 );
                const uint_fast32_t left_grid_gap = (cols==1) ? 0 : ((card_width*y)/(2*rows));  /* this gap is beautification only */
                const int_fast32_t rel_left = left_grid_gap + BORDER + (x * ( card_width + BORDER + BORDER ));
                const int_fast32_t rel_top = BORDER + (y * ( card_height + BORDER + BORDER ));
                shape_int_rectangle_t card_bounds;
                shape_int_rectangle_init( &card_bounds, left+rel_left, top+rel_top, card_width, card_height );
                shape_int_rectangle_trace( &card_bounds );
                gui_sketch_card_set_bounds( &(io_cards[card_idx]), card_bounds );
                const bool valid_card = gui_sketch_card_is_valid( &(io_cards[card_idx]) );
                gui_sketch_card_do_layout( &(io_cards[card_idx]), cr );
                gui_sketch_card_set_visible( &(io_cards[card_idx]), valid_card );
            }
        }
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

