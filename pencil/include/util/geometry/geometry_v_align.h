/* File: geometry_v_align.h; Copyright and License: see below */

#ifndef GEOMETRY_V_ALIGN_H
#define GEOMETRY_V_ALIGN_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the vertical alignment of geometry objects
 */

/*!
 *  \brief alignment constants of vertical alignment
 *
 */
enum geometry_v_align_enum {
    GEOMETRY_V_ALIGN_TOP,
    GEOMETRY_V_ALIGN_CENTER,
    GEOMETRY_V_ALIGN_BOTTOM,
};

typedef enum geometry_v_align_enum geometry_v_align_t;

/*!
 *  \brief gets the top coordinate of an aligned object
 *
 *  \param this_ pointer to own object attributes
 *  \param height height of object to be aligned
 *  \param reference_top top coordinate of object to which to align to
 *  \param reference_height height of object to which to align to
 *  \return top coordinate of aligned object
 */
static inline double geometry_v_align_get_top ( const geometry_v_align_t *this_, double height, double reference_top, double reference_height );

#include "util/geometry/geometry_v_align.inl"

#endif  /* GEOMETRY_V_ALIGN_H */


/*
Copyright 2017-2017 Andreas Warnke

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
