/* File: gui_sketch_card_layouter.h; Copyright and License: see below */

#ifndef GUI_SKETCH_CARD_LAYOUTER_H
#define GUI_SKETCH_CARD_LAYOUTER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Layouts a set of cards into a bounding box
 */

#include "sketch_area/gui_sketch_card.h"
#include "gui_tool.h"
#include "shape/shape_int_rectangle.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch card layouter
 */
struct gui_sketch_card_layouter_struct {
    shape_int_rectangle_t bounds;  /*!< bounding box of the card */
};

typedef struct gui_sketch_card_layouter_struct gui_sketch_card_layouter_t;

/*!
 *  \brief initializes the gui_sketch_card_layouter_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box, into which the cards shall be layouted
 */
void gui_sketch_card_layouter_init ( gui_sketch_card_layouter_t *this_, shape_int_rectangle_t *bounds );

/*!
 *  \brief destroys the gui_sketch_card_layouter_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_layouter_destroy ( gui_sketch_card_layouter_t *this_ );

/*!
 *  \brief layouts a list of cards, layout mode is determined by selected_tool
 *
 *  \param this_ pointer to own object attributes
 *  \param selected_tool currently selected tool
 *  \param io_cards list of cards to be layouted. Bounding boxes of cards are adjusted.
 *  \param cards_num number of cards in the io_cards list
 *  \param cr cairo drawing context, needed to determine the font metrics in the given drawing context
 */
void gui_sketch_card_layouter_layout ( gui_sketch_card_layouter_t *this_,
                                       gui_tool_t selected_tool,
                                       gui_sketch_card_t io_cards[],
                                       uint32_t cards_num,
                                       cairo_t *cr
                                     );

/*!
 *  \brief layouts a list of equal cards into a grid
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box, into which the cards shall be layouted
 *  \param max_card_height maximum height of a single card
 *  \param io_cards list of cards to be layouted. Bounding boxes of cards are adjusted.
 *  \param cards_num number of cards in the io_cards list
 *  \param cr cairo drawing context
 */
void gui_sketch_card_layouter_private_layout_to_grid ( gui_sketch_card_layouter_t *this_,
                                                       shape_int_rectangle_t *bounds,
                                                       uint_fast32_t max_card_height,
                                                       gui_sketch_card_t io_cards[],
                                                       uint_fast32_t cards_num,
                                                       cairo_t *cr
                                                     );

#endif  /* GUI_SKETCH_CARD_LAYOUTER_H */


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
