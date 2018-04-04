/* File: gui_sketch_drag_state.h; Copyright and License: see below */

#ifndef GUI_SKETCH_DRAG_STATE_H
#define GUI_SKETCH_DRAG_STATE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief keeps track of the dragging state: dragging is started by button-press and mouse-movement.
 */

#include "util/id/data_id_pair.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the drag state
 */
struct gui_sketch_drag_state_struct {
    bool dragging;  /*!< true if an object is currently being dragged */
    int32_t from_x;  /*!< x coordingate of the origin */
    int32_t from_y;  /*!< y coordingate of the origin */
    int32_t to_x;  /*!< x coordingate of the destination */
    int32_t to_y;  /*!< y coordingate of the destination */
    bool start_dragging_when_move;  /*!< true if dragging shall start when moving */
    data_id_pair_t dragged_object;  /* primary_id is the dragged object or diagramemelent, secondary_id is the classifier */
};

typedef struct gui_sketch_drag_state_struct gui_sketch_drag_state_t;

/*!
 *  \brief initializes the gui_sketch_drag_state_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_drag_state_init ( gui_sketch_drag_state_t *this_ );

/*!
 *  \brief destroys the gui_sketch_drag_state_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_drag_state_destroy ( gui_sketch_drag_state_t *this_ );

/*!
 *  \brief gets the dragging attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return true if an object is currently being dragged
 */
static inline bool gui_sketch_drag_state_is_dragging ( const gui_sketch_drag_state_t *this_ );

/*!
 *  \brief sets the dragging attribute
 *
 *  \param this_ pointer to own object attributes
 *  \param dragging true if an object is currently being dragged
 */
static inline void gui_sketch_drag_state_stop_dragging ( gui_sketch_drag_state_t *this_ );

/*!
 *  \brief checks if the start_dragging_when_move attribute is set.
 *
 *  \param this_ pointer to own object attributes
 *  \return true the mouse button was pressed and the pending drag is not yet cancelled
 */
static inline bool gui_sketch_drag_state_is_waiting_for_move ( const gui_sketch_drag_state_t *this_ );

/*!
 *  \brief sets the dragging attribute to true as soon as the to position has moved a small distance
 *
 *  \param this_ pointer to own object attributes
 *  \param dragged_object primary_id is the dragged object or diagramemelent, secondary_id is the classifier
 */
static inline void gui_sketch_drag_state_start_dragging_when_move ( gui_sketch_drag_state_t *this_,
                                                                    data_id_pair_t dragged_object
                                                                  );

/*!
 *  \brief gets the from_x attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return x coordinate of the dragging origin
 */
static inline int32_t gui_sketch_drag_state_get_from_x ( const gui_sketch_drag_state_t *this_ );

/*!
 *  \brief gets the from_y attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return y coordinate of the dragging origin
 */
static inline int32_t gui_sketch_drag_state_get_from_y ( const gui_sketch_drag_state_t *this_ );

/*!
 *  \brief sets the from_x and from_y attributes
 *
 *  \param this_ pointer to own object attributes
 *  \param from_x x coordinate of the dragging origin
 *  \param from_y y coordinate of the dragging origin
 */
static inline void gui_sketch_drag_state_set_from ( gui_sketch_drag_state_t *this_, int32_t from_x, int32_t from_y );

/*!
 *  \brief gets the to_x attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return x coordinate of the dragging destination
 */
static inline int32_t gui_sketch_drag_state_get_to_x ( const gui_sketch_drag_state_t *this_ );

/*!
 *  \brief gets the to_y attribute
 *
 *  \param this_ pointer to own object attributes
 *  \return y coordinate of the dragging destination
 */
static inline int32_t gui_sketch_drag_state_get_to_y ( const gui_sketch_drag_state_t *this_ );

/*!
 *  \brief sets the to_x and to_y attributes
 *
 *  \param this_ pointer to own object attributes
 *  \param to_x x coordinate of the dragging destination
 *  \param to_y y coordinate of the dragging destination
 */
static inline void gui_sketch_drag_state_set_to ( gui_sketch_drag_state_t *this_, int32_t to_x, int32_t to_y );

#include "gui_sketch_drag_state.inl"

#endif  /* GUI_SKETCH_DRAG_STATE_H */


/*
Copyright 2016-2018 Andreas Warnke

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
