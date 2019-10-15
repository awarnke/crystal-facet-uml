/* File: gui_sketch_card_layouter.c; Copyright and License: see below */

#include "gui_sketch_card_layouter.h"
#include "sketch_area/gui_sketch_area.h"
#include "trace.h"
#include "tslog.h"

void gui_sketch_card_layouter_init( gui_sketch_card_layouter_t *this_, shape_int_rectangle_t *bounds )
{
    TRACE_BEGIN();
    assert( NULL != bounds );

    (*this_).bounds = *bounds;

    TRACE_END();
}

void gui_sketch_card_layouter_destroy( gui_sketch_card_layouter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

static const gint RATIO_WIDTH = 36;
static const gint RATIO_HEIGHT = 24;
static const gint BORDER = 8;

void gui_sketch_card_layouter_layout ( gui_sketch_card_layouter_t *this_,
                                       gui_tool_t selected_tool,
                                       gui_sketch_card_t io_cards[],
                                       uint32_t cards_num,
                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( cards_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );

    /* fetch area bounds */
    const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );
    const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
    TRACE_INFO_INT_INT( "width, height", width, height );

    /* calculate card sizes */
    uint32_t focus_card_height;
    uint32_t focus_card_width;
    {
        const uint32_t focus_card_w_space = (width * 7) / 10;
        const uint32_t focus_card_h_space = (height * 4) / 10;
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
    const uint32_t parent_card_height = (focus_card_height * 6) / 10;
    const uint32_t parent_card_width = (focus_card_width * 6) / 10;

    /* layout cards */
    switch( selected_tool )
    {
        case GUI_TOOLBOX_SEARCH:  /* or */
        case GUI_TOOLBOX_NAVIGATE:
        {
            shape_int_rectangle_t card_bounds;

            /* self */
            assert(cards_num > GUI_SKETCH_AREA_CONST_FOCUSED_CARD);
            {
                const int32_t self_left = (left + width) - focus_card_width - BORDER;
                const int32_t self_top = top + BORDER + (parent_card_height/2);
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
                const int32_t parent_left = left + BORDER;
                const int32_t parent_top = top + BORDER;
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
                const int32_t children_top = top + focus_card_height + (parent_card_height/2) + 2*BORDER;
                const uint32_t children_height = (height > (children_top-top)) ? (height - (children_top-top)) : 0;
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

        case GUI_TOOLBOX_EDIT:  /* or */
        case GUI_TOOLBOX_CREATE:
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

    TRACE_END();
}

void gui_sketch_card_layouter_private_layout_to_grid ( gui_sketch_card_layouter_t *this_,
                                                       shape_int_rectangle_t *bounds,
                                                       uint32_t max_card_height,
                                                       gui_sketch_card_t io_cards[],
                                                       uint32_t cards_num,
                                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( cards_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );

    /* fetch outer bounds */
    const uint32_t width = shape_int_rectangle_get_width( bounds );
    const uint32_t height = shape_int_rectangle_get_height( bounds );
    const int32_t left = shape_int_rectangle_get_left( bounds );
    const int32_t top = shape_int_rectangle_get_top( bounds );
    TRACE_INFO_INT_INT( "width, height", width, height );

    const uint32_t card_width = RATIO_WIDTH * 2;
    const uint32_t card_height = RATIO_HEIGHT * 2;

    /* layout all cards in range */
    for ( int card_idx = 0; card_idx < cards_num; card_idx ++ )
    {
        shape_int_rectangle_t card_bounds;

        {
            shape_int_rectangle_init( &card_bounds,
                                      left + (((width-card_width)*card_idx)/(cards_num)),
                                      top + (((height-card_height)*card_idx)/(cards_num)),
                                      card_width,
                                      card_height
                                    );
            /*
            shape_int_rectangle_init( &card_bounds, cards_left+(cards_width*card_idx)/cards_num, children_top, cards_width/cards_num, children_height );
            shape_int_rectangle_shrink_by_border( &card_bounds, BORDER/2 );
            shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_CENTER,               SHAPE_V_ALIGN_CENTER );
            */
        }

        shape_int_rectangle_trace( &card_bounds );
        gui_sketch_card_set_bounds( &(io_cards[card_idx]), card_bounds );
        const bool valid_card = gui_sketch_card_is_valid( &(io_cards[card_idx]) );
        gui_sketch_card_do_layout( &(io_cards[card_idx]), cr );
        gui_sketch_card_set_visible( &(io_cards[card_idx]), valid_card );
    }

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

