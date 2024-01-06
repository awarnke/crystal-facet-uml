/* File: geometry_direction.h; Copyright and License: see below */

#ifndef GEOMETRY_DIRECTION_H
#define GEOMETRY_DIRECTION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines layout directions of objects
 */

/*!
 *  \brief enumeration of directions
 *
 *  Do not assume any order.
 */
enum geometry_direction_enum {
    GEOMETRY_DIRECTION_CENTER = 0,  /*!< the direction points to the center of the object */
    GEOMETRY_DIRECTION_LEFT = 1,  /*!< the direction points to the left */
    GEOMETRY_DIRECTION_UP_LEFT = 2,  /*!< the direction points up-left */
    GEOMETRY_DIRECTION_UP = 3,  /*!< the direction points up */
    GEOMETRY_DIRECTION_UP_RIGHT = 4,  /*!< the direction points up-right */
    GEOMETRY_DIRECTION_RIGHT = 5,  /*!< the direction points to the right */
    GEOMETRY_DIRECTION_DOWN_RIGHT = 6,  /*!< the direction points down-right */
    GEOMETRY_DIRECTION_DOWN = 7,  /*!< the direction points down */
    GEOMETRY_DIRECTION_DOWN_LEFT = 8,  /*!< the direction points down-left */
    GEOMETRY_DIRECTION_MAX = 9,  /*!< maximum number of directions, can be used to iterate over all directions */
};

typedef enum geometry_direction_enum geometry_direction_t;

#endif  /* GEOMETRY_DIRECTION_H */


/*
Copyright 2018-2024 Andreas Warnke

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
