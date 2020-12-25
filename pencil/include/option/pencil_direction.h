/* File: pencil_direction.h; Copyright and License: see below */

#ifndef PENCIL_DIRECTION_H
#define PENCIL_DIRECTION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the flow direction of ports and pins and entry/exit states
 */

/*!
 *  \brief control flow and data flow directions
 */
enum pencil_direction_enum {
    PENCIL_DIRECTION_UNSPECIFIED,
    PENCIL_DIRECTION_IN,
    PENCIL_DIRECTION_OUT,
    PENCIL_DIRECTION_IN_OUT,
};

typedef enum pencil_direction_enum pencil_direction_t;

#endif  /* PENCIL_DIRECTION_H */


/*
Copyright 2020-2020 Andreas Warnke

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
