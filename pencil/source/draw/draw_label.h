/* File: draw_label.h; Copyright and License: see below */

#ifndef DRAW_LABEL_H
#define DRAW_LABEL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Draws titles, stereotypes, underscores; performs linebreaks
 */

#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "pencil_size.h"
#include "util/geometry/geometry_rectangle.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the draw label functions
 */
struct draw_label_struct {
    int dummy;  /*!< This object is a collection of stateless drawing functions */
};

typedef struct draw_label_struct draw_label_t;

/*!
 *  \brief initializes the draw_label_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_label_init( draw_label_t *this_ );

/*!
 *  \brief destroys the draw_label_t
 *
 *  \param this_ pointer to own object attributes
 */
void draw_label_destroy( draw_label_t *this_ );

#endif  /* DRAW_LABEL_H */


/*
Copyright 2019-2019 Andreas Warnke

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
