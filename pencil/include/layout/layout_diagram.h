/* File: layout_diagram.h; Copyright and License: see below */

#ifndef LAYOUT_DIAGRAM_H
#define LAYOUT_DIAGRAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines dimensions of a diagram
 */

#include "util/geometry/geometry_rectangle.h"
#include "data_diagram.h"

/*!
 *  \brief dimensions of a diagram
 */
struct layout_diagram_struct {
    geometry_rectangle_t bounds;  /*!< bounding box of the diagram */
    geometry_rectangle_t draw_area;  /*!< drawing rectangle of inner contents of the diagram */
    const data_diagram_t *data;  /*!< pointer to the data object of the diagram */
};

typedef struct layout_diagram_struct layout_diagram_t;

/*!
 *  \brief initializes the layout_diagram_t struct with a pointer to diagram_data.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data pointer to data object which is layouted
 */
static inline void layout_diagram_init ( layout_diagram_t *this_, const data_diagram_t *diagram_data );

/*!
 *  \brief destroys the layout_diagram_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void layout_diagram_destroy ( layout_diagram_t *this_ );

/*!
 *  \brief checks if the diagram data is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if (*this_).data points to valid data
 */
static inline bool layout_diagram_is_valid ( const layout_diagram_t *this_ );

/*!
 *  \brief gets the bounding box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_diagram_get_bounds_ptr ( layout_diagram_t *this_ );

/*!
 *  \brief gets the bounding box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_diagram_get_bounds_const ( const layout_diagram_t *this_ );

/*!
 *  \brief gets the bounding box of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_bounds coordinates of new bounding box
 */
static inline void layout_diagram_set_bounds ( layout_diagram_t *this_, const geometry_rectangle_t *diagram_bounds );

/*!
 *  \brief gets the draw area of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *layout_diagram_get_draw_area_ptr ( layout_diagram_t *this_ );

/*!
 *  \brief gets the draw area of the diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to geometry_rectangle_t.
 */
static inline const geometry_rectangle_t *layout_diagram_get_draw_area_const ( const layout_diagram_t *this_ );

/*!
 *  \brief gets the data_diagram_t object
 *
 *  \param this_ pointer to own object attributes
 *  \return a pointer to the corresponding data_diagram_t object
 */
static inline const data_diagram_t *layout_diagram_get_data_const ( const layout_diagram_t *this_ );

/*!
 *  \brief gets the row id of the diagram
 *
 *  This is a shortcut for data_diagram_get_id( layout_diagram_get_data_const ( X ) )
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the diagram
 */
static inline int64_t layout_diagram_get_diagram_id ( const layout_diagram_t *this_ );

#include "layout_diagram.inl"

#endif  /* LAYOUT_DIAGRAM_H */


/*
Copyright 2018-2020 Andreas Warnke

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
