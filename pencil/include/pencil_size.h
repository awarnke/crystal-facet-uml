/* File: pencil_size.h; Copyright and License: see below */

#ifndef PENCIL_SIZE_H
#define PENCIL_SIZE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides a set of size values used to draw one diagram
 */

#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the pencil_input_data_t
 */
struct pencil_size_struct {
    double standard_font_size;
    double bigger_font_size;
    double standard_line_width;
    double bold_line_width;
    double standard_object_border;
};

typedef struct pencil_size_struct pencil_size_t;

/*!
 *  \brief initializes the size values
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_size_init( pencil_size_t *this_ );

/*!
 *  \brief destroys the pencil_size_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_size_destroy( pencil_size_t *this_ );

/*!
 *  \brief re-initializes the pencil_size_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_size_reinit( pencil_size_t *this_ );

#include "pencil_size.inl"

#endif  /* PENCIL_SIZE_H */


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
