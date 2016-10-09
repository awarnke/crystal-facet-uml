/* File: geometry_non_linear_scale.h; Copyright and License: see below */

#ifndef GEOMETRY_NON_LINEAR_SCALE_H
#define GEOMETRY_NON_LINEAR_SCALE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Maps integer numbers to coordinates
 */

/*!
 *  \brief attributes of a non_linear_scale
 */
struct geometry_non_linear_scale_struct {
    double left;
    double top;
    double width;
    double height;
};

typedef struct geometry_non_linear_scale_struct geometry_non_linear_scale_t;

/*!
 *  \brief initializes the geometry_non_linear_scale_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param left left coordinate of the rectangle
 *  \param top top coordinate of the rectangle
 *  \param width width of the rectangle
 *  \param height height of the rectangle
 */
static inline void geometry_non_linear_scale_init ( geometry_non_linear_scale_t *this_, double left, double top, double width, double height );

/*!
 *  \brief re-initializes the geometry_non_linear_scale_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param left left coordinate of the rectangle
 *  \param top top coordinate of the rectangle
 *  \param width width of the rectangle
 *  \param height height of the rectangle
 */
static inline void geometry_non_linear_scale_reinit ( geometry_non_linear_scale_t *this_, double left, double top, double width, double height );

/*!
 *  \brief initializes the geometry_non_linear_scale_t struct to empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_non_linear_scale_init_empty ( geometry_non_linear_scale_t *this_ );

/*!
 *  \brief re-initializes the geometry_non_linear_scale_t struct to empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline void geometry_non_linear_scale_reinit_empty ( geometry_non_linear_scale_t *this_ );

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
static inline void geometry_non_linear_scale_trace ( geometry_non_linear_scale_t *this_ );

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
