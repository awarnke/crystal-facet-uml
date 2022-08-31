/* File: pencil_relationship_1d_layouter.h; Copyright and License: see below */

#ifndef PENCIL_RELATIONSHIP_1D_LAYOUTER_H
#define PENCIL_RELATIONSHIP_1D_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of relationships in a diagram
 */

#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "pencil_relationship_painter.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "set/data_visible_set.h"
#include "data_id.h"
#include "u8list/universal_array_index_sorter.h"
#include "u8/u8_error.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the relationship layouter
 */
struct pencil_relationship_1d_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to an instance of layout data */
    pencil_size_t *pencil_size;  /*!< pointer to an instance of a pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */

    pencil_relationship_painter_t relationship_painter;  /*!< own instance of a painter object to ask for display dimensions */
};

typedef struct pencil_relationship_1d_layouter_struct pencil_relationship_1d_layouter_t;

/*!
 *  \brief initializes the layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_relationship_1d_layouter_init( pencil_relationship_1d_layouter_t *this_,
                                           pencil_layout_data_t *layout_data,
                                           pencil_size_t *pencil_size
                                         );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_1d_layouter_destroy( pencil_relationship_1d_layouter_t *this_ );

/*!
 *  \brief make all relationships visible.
 *
 *  Sets either normal visibility or grayed out status.
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_1d_layouter_private_make_all_visible ( pencil_relationship_1d_layouter_t *this_ );

/*!
 *  \brief defines the shapes of relationships in sequence diagrams
 *
 *  Shows only relationships that are associated to a lifeline because a sequence diagram
 *  shows only an example sequence of messages, not all dependencies.
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_1d_layouter_layout_for_sequence( pencil_relationship_1d_layouter_t *this_ );

/*!
 *  \brief defines the shapes of relationships in timing diagrams
 *
 *  Shows only relationships that are associated to a lifeline because a timing diagram
 *  shows only an example timing-sequence of messages, not all dependencies.
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_relationship_1d_layouter_layout_for_timing( pencil_relationship_1d_layouter_t *this_ );

#endif  /* PENCIL_RELATIONSHIP_1D_LAYOUTER_H */


/*
Copyright 2017-2022 Andreas Warnke

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
