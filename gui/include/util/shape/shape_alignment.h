/* File: shape_alignment.h; Copyright and License: see below */

#ifndef SHAPE_ALIGNMENT_H
#define SHAPE_ALIGNMENT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Provides alignment constants
 */

/*!
 *  \brief enumeration of all diagram types
 */
enum shape_alignment_enum {
    SHAPE_ALIGNMENT_VERTICAL_UNSPECIFIED = 0x00,
    SHAPE_ALIGNMENT_VERTICAL_TOP = 0x01,
    SHAPE_ALIGNMENT_VERTICAL_MIDDLE = 0x02,
    SHAPE_ALIGNMENT_VERTICAL_BOTTOM = 0x03,
    SHAPE_ALIGNMENT_VERTICAL_MASK = 0x0f,
    SHAPE_ALIGNMENT_HORIZONTAL_UNSPECIFIED = 0x00,
    SHAPE_ALIGNMENT_HORIZONTAL_LEFT = 0x10,
    SHAPE_ALIGNMENT_HORIZONTAL_CENTER = 0x20,
    SHAPE_ALIGNMENT_HORIZONTAL_RIGHT = 0x30,
    SHAPE_ALIGNMENT_HORIZONTAL_MASK = 0xf0,
};

typedef enum shape_alignment_enum shape_alignment_t;

#endif  /* SHAPE_ALIGNMENT_H */


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
