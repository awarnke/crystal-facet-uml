/* File: geometry_grid.h; Copyright and License: see below */

#ifndef GEOMETRY_GRID_H
#define GEOMETRY_GRID_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Maps 1/2 dimensional order-values to 1/2 dimensional locations
 */

#include "geometry/geometry_non_linear_scale.h"

/*!
 *  \brief list of possible kinds of geometry_grid_t
 */
enum geometry_grid_kind_enum {
    GEOMETRY_GRID_KIND_0,  /*!< no dimension exists */
    GEOMETRY_GRID_KIND_X,  /*!< only x dimension exists */
    GEOMETRY_GRID_KIND_Y,  /*!< only y dimension exists */
    GEOMETRY_GRID_KIND_XY,  /*!< x/y dimensions exists */
};

typedef enum geometry_grid_kind_enum geometry_grid_kind_t;

/*!
 *  \brief attributes of a grid
 */
struct geometry_grid_struct {
    geometry_grid_kind_t kind;  /*!< number of dimensions */
    geometry_non_linear_scale_t x_scale;  /*!< mapping of order values to locations in x dimension */
    geometry_non_linear_scale_t y_scale;  /*!< mapping of order values to locations in y dimension */
};

typedef struct geometry_grid_struct geometry_grid_t;

/*!
 *  \brief initializes the geometry_grid_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param kind lnumber of dimensions
 */
static inline void geometry_grid_init ( geometry_grid_t *this_, geometry_grid_kind_t kind );

/*!
 *  \brief re-initializes the geometry_grid_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param kind number of dimensions
 */
static inline void geometry_grid_reinit ( geometry_grid_t *this_, geometry_grid_kind_t kind );

/*!
 *  \brief destroys the geometry_grid_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_grid_destroy ( geometry_grid_t *this_ );

/*!
 *  \brief prints the geometry_grid_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_grid_trace ( const geometry_grid_t *this_ );

/*!
 *  \brief gets the kind of geometry_grid_t
 *
 *  \param this_ pointer to own object attributes
 *  \return number of dimensions
 */
static inline geometry_grid_kind_t geometry_grid_get_kind ( const geometry_grid_t *this_ );

/*!
 *  \brief gets a scale for one dimension
 *
 *  \param this_ pointer to own object attributes
 *  \return mapping of order values to locations in x dimension
 */
static inline geometry_non_linear_scale_t * geometry_grid_get_x_scale_ptr ( geometry_grid_t *this_ );

/*!
 *  \brief gets a scale for one dimension
 *
 *  \param this_ pointer to own object attributes
 *  \return mapping of order values to locations in x dimension
 */
static inline const geometry_non_linear_scale_t * geometry_grid_get_x_scale_const ( const geometry_grid_t *this_ );

/*!
 *  \brief gets a scale for one dimension
 *
 *  \param this_ pointer to own object attributes
 *  \return mapping of order values to locations in y dimension
 */
static inline geometry_non_linear_scale_t * geometry_grid_get_y_scale_ptr ( geometry_grid_t *this_ );

/*!
 *  \brief gets a scale for one dimension
 *
 *  \param this_ pointer to own object attributes
 *  \return mapping of order values to locations in y dimension
 */
static inline const geometry_non_linear_scale_t * geometry_grid_get_y_scale_const ( const geometry_grid_t *this_ );

#include "geometry/geometry_grid.inl"

#endif  /* GEOMETRY_GRID_H */


/*
Copyright 2024-2024 Andreas Warnke

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
