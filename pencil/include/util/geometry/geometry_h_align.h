/* File: geometry_h_align.h; Copyright and License: see below */

#ifndef GEOMETRY_H_ALIGN_H
#define GEOMETRY_H_ALIGN_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the horizontal alignment of geometry objects
 */

/*!
 *  \brief alignment constants of vertical alignment
 *
 */
enum geometry_h_align_enum {
    GEOMETRY_H_ALIGN_LEFT,
    GEOMETRY_H_ALIGN_CENTER,
    GEOMETRY_H_ALIGN_RIGHT,
};

typedef enum geometry_h_align_enum geometry_h_align_t;

/*!
 *  \brief gets the left coordinate of an aligned object
 *
 *  \param this_ pointer to own object attributes
 *  \param width width of object to be aligned
 *  \param reference_left left coordinate of object to which to align to
 *  \param reference_width width of object to which to align to
 *  \return left coordinate of aligned object
 */
static inline double geometry_h_align_get_left ( const geometry_h_align_t *this_, 
                                                 double width, 
                                                 double reference_left, 
                                                 double reference_width
                                               );

#include "util/geometry/geometry_h_align.inl"

#endif  /* GEOMETRY_H_ALIGN_H */


/*
Copyright 2017-2019 Andreas Warnke

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
