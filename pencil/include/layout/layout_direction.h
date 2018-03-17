/* File: layout_direction.h; Copyright and License: see below */

#ifndef LAYOUT_DIRECTION_H
#define LAYOUT_DIRECTION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines layout directions of objects
 */

/*!
 *  \brief enumeration of directions
 */
enum layout_direction_enum {
    PENCIL_LAYOUT_DIRECTION_CENTER = 0,  /*!< the direction points to the center of the object */
    PENCIL_LAYOUT_DIRECTION_LEFT = 1,  /*!< the direction points to the left */
    PENCIL_LAYOUT_DIRECTION_UP = 2,  /*!< the direction points up */
    PENCIL_LAYOUT_DIRECTION_RIGHT = 3,  /*!< the direction points to the right */
    PENCIL_LAYOUT_DIRECTION_DOWN = 4,  /*!< the direction points down */
    PENCIL_LAYOUT_DIRECTION_MAX = 5,  /*!< maximum number of directions, can be used to iterate over all directions */
};

typedef enum layout_direction_enum layout_direction_t;

#endif  /* LAYOUT_DIRECTION_H */


/*
Copyright 2018-2018 Andreas Warnke

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
