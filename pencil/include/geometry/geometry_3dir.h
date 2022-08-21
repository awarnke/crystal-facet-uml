/* File: geometry_3dir.h; Copyright and License: see below */

#ifndef GEOMETRY_3DIR_H
#define GEOMETRY_3DIR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a struct of 3 directions
 */

#include "geometry/geometry_direction.h"

/*!
 *  \brief struct of 3 directions
 */
struct geometry_3dir_struct {
    geometry_direction_t first;
    geometry_direction_t second;
    geometry_direction_t third;
};

typedef struct geometry_3dir_struct geometry_3dir_t;

/*!
 *  \brief checks if two geometry_3dir_t objects are equal
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if both geometry_3dir_t are equal
 */
static inline bool geometry_3dir_equals ( const geometry_3dir_t *this_, const geometry_3dir_t *that );

#include "geometry/geometry_3dir.inl"

#endif  /* GEOMETRY_3DIR_H */


/*
Copyright 2021-2022 Andreas Warnke

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
