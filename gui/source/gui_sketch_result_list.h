/* File: gui_sketch_result_list.h; Copyright and License: see below */

#ifndef GUI_SKETCH_RESULT_LIST_H
#define GUI_SKETCH_RESULT_LIST_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Draws a list showing search results
 */

#include "gui_marked_set.h"
#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "layout/layout_order.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the result list
 */
struct gui_sketch_result_list_struct {
    bool visible;  /*!< is the result list visible */
    shape_int_rectangle_t bounds;  /*!< bounding box of the result list */
};

typedef struct gui_sketch_result_list_struct gui_sketch_result_list_t;

/*!
 *  \brief initializes the gui_sketch_result_list_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_result_list_init ( gui_sketch_result_list_t *this_ );

/*!
 *  \brief destroys the gui_sketch_result_list_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_result_list_destroy ( gui_sketch_result_list_t *this_ );

/*!
 *  \brief fetches the diagram data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to load
 *  \param db_reader pointer to a database reader object
 */
static inline void gui_sketch_result_list_load_data( gui_sketch_result_list_t *this_, int64_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief marks the diagram data as invalid
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_result_list_invalidate_data( gui_sketch_result_list_t *this_ );

/*!
 *  \brief gets the valid info from the diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool gui_sketch_result_list_is_valid( gui_sketch_result_list_t *this_ );

/*!
 *  \brief gets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch card
 */
static inline shape_int_rectangle_t gui_sketch_result_list_get_bounds( gui_sketch_result_list_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box of this sketch card
 */
static inline void gui_sketch_result_list_set_bounds( gui_sketch_result_list_t *this_, shape_int_rectangle_t bounds );

/*!
 *  \brief gets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this sketch card is currently visible
 */
static inline bool gui_sketch_result_list_is_visible( gui_sketch_result_list_t *this_ );

/*!
 *  \brief sets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \param visible true if this card is currently visible, false otherwise
 */
static inline void gui_sketch_result_list_set_visible( gui_sketch_result_list_t *this_, bool visible );

/*!
 *  \brief draws a single diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param marker set of all objects to be marked
 *  \param cr cairo drawing context
 */
void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, gui_marked_set_t *marker, cairo_t *cr );

/*!
 * \brief gets the address of the diagram within the painter input data of gui_sketch_result_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to diagram
 */
static inline data_diagram_t *gui_sketch_result_list_get_diagram_ptr ( gui_sketch_result_list_t *this_ );

/*!
 *  \brief gets the object-id of the object at a given position.
 *
 *  Either it gets the real, de-referenced object at a given position, e.g. a diagram_t or a classifier_t,
 *  or the visible object at a given position, e.g. a diagram_t or a diagramelement_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 */
static inline void gui_sketch_result_list_get_object_id_at_pos ( gui_sketch_result_list_t *this_,
                                                          int32_t x,
                                                          int32_t y,
                                                          data_id_t* out_selected_id
                                                        );

/*!
 *  \brief gets the order value at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id object id for which to determine the list order.
 *                The object may be of type DATA_TABLE_CLASSIFIER, DATA_TABLE_FEATURE or DATA_TABLE_RELATIONSHIP.
 *  \param x x-position
 *  \param y y-position
 *  \return the list order value at the given location
 */
static inline layout_order_t gui_sketch_result_list_get_order_at_pos ( gui_sketch_result_list_t *this_, data_id_t obj_id, int32_t x, int32_t y );

/*!
 *  \brief moves an object to an order (without modifying the database)
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id object id which to move (modify the x-/y- or list order).
 *                The object may be of type DATA_TABLE_CLASSIFIER, DATA_TABLE_FEATURE or DATA_TABLE_RELATIONSHIP.
 *  \param order layout_order_t, where to move the object to
 */
static inline void gui_sketch_result_list_move_object_to_order ( gui_sketch_result_list_t *this_, data_id_t obj_id, layout_order_t *order );

#include "gui_sketch_result_list.inl"

#endif  /* GUI_SKETCH_RESULT_LIST_H */


/*
Copyright 2018-2018 Andreas Warnke

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
