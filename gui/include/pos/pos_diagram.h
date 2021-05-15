/* File: pos_diagram.h; Copyright and License: see below */

#ifndef POS_DIAGRAM_H
#define POS_DIAGRAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines location of a diagram
 */

#include "util/shape/shape_int_rectangle.h"
#include "data_diagram.h"

/*!
 *  \brief location of a diagram
 */
struct pos_diagram_struct {
    shape_int_rectangle_t icon_box;  /*!< The rectangle where the icon of the diagram is drawn */
    shape_int_rectangle_t label_box;  /*!< bounding box of the label of the diagram. */
    const data_diagram_t *data;  /*!< pointer to the data object of the diagram */
};

typedef struct pos_diagram_struct pos_diagram_t;

/*!
 *  \brief initializes the pos_diagram_t struct with a pointer to diagram_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data pointer to data object which is layouted
 */
static inline void pos_diagram_init ( pos_diagram_t *this_, const data_diagram_t *diagram_data );

/*!
 *  \brief destroys the pos_diagram_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pos_diagram_destroy ( pos_diagram_t *this_ );

/*!
 *  \brief checks if the diagram data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if (*this_).data points to valid data
 */
static inline bool pos_diagram_is_valid ( const pos_diagram_t *this_ );

/*!
 *  \brief gets the icon_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline shape_int_rectangle_t *pos_diagram_get_icon_box_ptr ( pos_diagram_t *this_ );

/*!
 *  \brief gets the icon_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_diagram_get_icon_box_const ( const pos_diagram_t *this_ );

/*!
 *  \brief gets the icon box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param icon_box coordinates of new icon_box
 */
static inline void pos_diagram_set_icon_box ( pos_diagram_t *this_, const shape_int_rectangle_t *icon_box );

/*!
 *  \brief gets the label_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline shape_int_rectangle_t *pos_diagram_get_label_box_ptr ( pos_diagram_t *this_ );

/*!
 *  \brief gets the label_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to shape_int_rectangle_t.
 */
static inline const shape_int_rectangle_t *pos_diagram_get_label_box_const ( const pos_diagram_t *this_ );

/*!
 *  \brief gets the label_box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_bounds coordinates of new label_box
 */
static inline void pos_diagram_set_label_box ( pos_diagram_t *this_, const shape_int_rectangle_t *label_box );

/*!
 *  \brief gets the data_diagram_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the corresponding data_diagram_t object
 */
static inline const data_diagram_t *pos_diagram_get_data_const ( const pos_diagram_t *this_ );

/*!
 *  \brief gets the row id of the diagram
 *
 *  This is a shortcut for data_diagram_get_row_id( pos_diagram_get_data_const ( X ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the diagram
 */
static inline data_row_id_t pos_diagram_get_diagram_id ( const pos_diagram_t *this_ );

#include "pos_diagram.inl"

#endif  /* POS_DIAGRAM_H */


/*
Copyright 2021-2021 Andreas Warnke

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
