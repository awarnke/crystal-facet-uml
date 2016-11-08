/* File: gui_sketch_card.h; Copyright and License: see below */

#ifndef GUI_SKETCH_CARD_H
#define GUI_SKETCH_CARD_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Caches diagram data and draws a diagram
 */

#include "gui_sketch_marker.h"
#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "pencil_diagram_painter.h"
#include "pencil_input_data.h"
#include "universial_int32_pair.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch card
 */
struct gui_sketch_card_struct {
    bool visible;
    shape_int_rectangle_t bounds;  /*!< bounding box of the card */
    pencil_input_data_t painter_input_data;  /*!< caches the diagram data */
    pencil_diagram_painter_t painter;  /*!< own instance of a diagram painter */
};

typedef struct gui_sketch_card_struct gui_sketch_card_t;

/*!
 *  \brief initializes the gui_sketch_card_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_init ( gui_sketch_card_t *this_ );

/*!
 *  \brief destroys the gui_sketch_card_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_destroy ( gui_sketch_card_t *this_ );

/*!
 *  \brief fetches the diagram data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to load
 *  \param db_reader pointer to a database reader object
 */
static inline void gui_sketch_card_load_data( gui_sketch_card_t *this_, int64_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief marks the diagram data as invalid
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_card_invalidate_data( gui_sketch_card_t *this_ );

/*!
 *  \brief gets the valid info from the diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool gui_sketch_card_is_valid( gui_sketch_card_t *this_ );

/*!
 *  \brief gets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch card
 */
static inline shape_int_rectangle_t gui_sketch_card_get_bounds( gui_sketch_card_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box of this sketch card
 */
static inline void gui_sketch_card_set_bounds( gui_sketch_card_t *this_, shape_int_rectangle_t bounds );

/*!
 *  \brief gets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this sketch card is currently visible
 */
static inline bool gui_sketch_card_is_visible( gui_sketch_card_t *this_ );

/*!
 *  \brief sets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \param visible true if this card is currently visible, false otherwise
 */
static inline void gui_sketch_card_set_visible( gui_sketch_card_t *this_, bool visible );

/*!
 *  \brief draws a single diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param marker set of all objects to be marked
 *  \param cr cairo drawing context
 */
void gui_sketch_card_draw ( gui_sketch_card_t *this_, gui_sketch_marker_t *marker, cairo_t *cr );

/*!
 * \brief gets the address of the diagram within the painter input data of gui_sketch_card_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagram_t *gui_sketch_card_get_diagram_ptr ( gui_sketch_card_t *this_ );

/*!
 *  \brief gets the object-id of the object at a given position.
 *
 *  Either it gets the real, de-referenced object at a given position, e.g. a diagram_t or a classifier_t,
 *  or the visible object at a given position, e.g. a diagram_t or a diagramelement_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param dereference true if the real, dereferenced object shall be returned (e.g. data_classifier_t or data_diagram_t),
 *                     false if the visible object shall be returned (e.g. data_diagramelement_t or data_diagram_t)
 *  \return an object id. The id is invalid if there is no object at the given location.
 */
static inline data_id_t gui_sketch_card_get_object_id_at_pos ( gui_sketch_card_t *this_, int32_t x, int32_t y, bool dereference );

/*!
 *  \brief gets the order values at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return the x- and y- order values as first and second element of the pair
 */
static inline universial_int32_pair_t gui_sketch_card_get_order_at_pos ( gui_sketch_card_t *this_, int32_t x, int32_t y );

#include "gui_sketch_card.inl"

#endif  /* GUI_SKETCH_CARD_H */


/*
Copyright 2016-2016 Andreas Warnke

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
