/* File: geometry_non_linear_scale.h; Copyright and License: see below */

#ifndef GEOMETRY_NON_LINEAR_SCALE_H
#define GEOMETRY_NON_LINEAR_SCALE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Maps order-values to locations
 */

#include <stdint.h>

/*!
 *  \brief constants of geometry_non_linear_scale_t
 */
enum geometry_non_linear_scale_max_enum {
    GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS = 32,  /*!< maximum number of points by which the non-linear scale function is defined */
};

/*!
 *  \brief attributes of a non_linear_scale
 */
struct geometry_non_linear_scale_struct {
    uint32_t num_points;  /*!< number of valid elements in the arrays location and order */
    double location[ GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS ];  /*!< array containing locations for which an order is defined */
    int32_t order[ GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS ];  /*!< array containing order-values which correspond to a location */
};

typedef struct geometry_non_linear_scale_struct geometry_non_linear_scale_t;

/*!
 *  \brief initializes the geometry_non_linear_scale_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param lower_bound lower bound location, defines the lowest location value
 *  \param upper_bound upper bound location, defines the upper-most location value
 */
void geometry_non_linear_scale_init ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound );

/*!
 *  \brief re-initializes the geometry_non_linear_scale_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param lower_bound lower bound location, defines the lowest location value
 *  \param upper_bound upper bound location, defines the upper-most location value
 */
static inline void geometry_non_linear_scale_reinit ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound );

/*!
 *  \brief destroys the geometry_non_linear_scale_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_non_linear_scale_destroy ( geometry_non_linear_scale_t *this_ );

/*!
 *  \brief prints the geometry_non_linear_scale_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
void geometry_non_linear_scale_trace ( geometry_non_linear_scale_t *this_ );

/*!
 *  \brief adds an order-value to the non-linear scale, updates the locations of all defined points
 *
 *  \param this_ pointer to own object attributes
 *  \param order oder value for which a location shall be inserted to the scale
 */
void geometry_non_linear_scale_add_order ( geometry_non_linear_scale_t *this_, int32_t order );

/*!
 *  \brief gets a location given an order-value
 *
 *  \param this_ pointer to own object attributes
 *  \param order oder value for which the location shall be determined
 */
static inline double geometry_non_linear_scale_get_location ( geometry_non_linear_scale_t *this_, int32_t order );

/*!
 *  \brief gets an order-value given a location
 *
 *  \param this_ pointer to own object attributes
 *  \param location location for which the order shall be determined
 *  \param snap_interval maximum distance from location to next point which snaps the location to the points location
 */
static inline int32_t geometry_non_linear_scale_get_order ( geometry_non_linear_scale_t *this_, double location, double snap_interval );

#include "util/geometry/geometry_non_linear_scale.inl"

#endif  /* GEOMETRY_NON_LINEAR_SCALE_H */


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
