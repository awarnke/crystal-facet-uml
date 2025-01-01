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

/*!
 *  \brief gets first direction
 *
 *  \param this_ pointer to own object attributes
 *  \return the first direction
 */
static inline geometry_direction_t geometry_3dir_get_first ( const geometry_3dir_t *this_ );

/*!
 *  \brief gets second direction
 *
 *  \param this_ pointer to own object attributes
 *  \return the second direction
 */
static inline geometry_direction_t geometry_3dir_get_second ( const geometry_3dir_t *this_ );

/*!
 *  \brief gets third direction
 *
 *  \param this_ pointer to own object attributes
 *  \return the third direction
 */
static inline geometry_direction_t geometry_3dir_get_third ( const geometry_3dir_t *this_ );

/*!
 *  \brief checks if first direction is horizontal
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the first direction is GEOMETRY_DIRECTION_LEFT or GEOMETRY_DIRECTION_RIGHT
 */
static inline bool geometry_3dir_is_first_h ( const geometry_3dir_t *this_ );

/*!
 *  \brief checks if first direction is vertical
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the first direction is GEOMETRY_DIRECTION_UP or GEOMETRY_DIRECTION_DOWN
 */
static inline bool geometry_3dir_is_first_v ( const geometry_3dir_t *this_ );

/*!
 *  \brief checks if second direction is horizontal
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the second direction is GEOMETRY_DIRECTION_LEFT or GEOMETRY_DIRECTION_RIGHT
 */
static inline bool geometry_3dir_is_second_h ( const geometry_3dir_t *this_ );

/*!
 *  \brief checks if second direction is vertical
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the second direction is GEOMETRY_DIRECTION_UP or GEOMETRY_DIRECTION_DOWN
 */
static inline bool geometry_3dir_is_second_v ( const geometry_3dir_t *this_ );

/*!
 *  \brief checks if third direction is horizontal
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the third direction is GEOMETRY_DIRECTION_LEFT or GEOMETRY_DIRECTION_RIGHT
 */
static inline bool geometry_3dir_is_third_h ( const geometry_3dir_t *this_ );

/*!
 *  \brief checks if third direction is vertical
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the third direction is GEOMETRY_DIRECTION_UP or GEOMETRY_DIRECTION_DOWN
 */
static inline bool geometry_3dir_is_third_v ( const geometry_3dir_t *this_ );

#include "geometry/geometry_3dir.inl"

#endif  /* GEOMETRY_3DIR_H */


/*
Copyright 2021-2025 Andreas Warnke

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
