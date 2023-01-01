/* File: pencil_classifier_1d_layouter.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_1D_LAYOUTER_H
#define PENCIL_CLASSIFIER_1D_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of classifiers and features in a 1-dimensional diagram.
 *
 *  Timing, Sequence and list diagrams are layouted 1-dimensional.
 */

#include "pencil_classifier_composer.h"
#include "pencil_size.h"
#include "pencil_layout_data.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "data_id.h"
#include "set/data_visible_set.h"
#include "u8list/universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the classifier layouter
 */
struct pencil_classifier_1d_layouter_struct {
    pencil_layout_data_t *layout_data;  /* pointer to external layout data */

    const pencil_size_t *pencil_size;  /*!< pointer to an external pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */
    const geometry_rectangle_t *diagram_draw_area;  /*!< pointer to an external drawing rectangle containing inner contents of the diagram */

    pencil_classifier_composer_t classifier_composer;  /*!< own instance of a composer object to ask for display dimensions */
};

typedef struct pencil_classifier_1d_layouter_struct pencil_classifier_1d_layouter_t;

/*!
 *  \brief initializes the classifier layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_classifier_1d_layouter_init( pencil_classifier_1d_layouter_t *this_,
                                         pencil_layout_data_t *layout_data,
                                         const pencil_size_t *pencil_size
                                       );

/*!
 *  \brief destroys the layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_1d_layouter_destroy( pencil_classifier_1d_layouter_t *this_ );

/*!
 *  \brief defines classifier bounds for list diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_1d_layouter_layout_for_list( pencil_classifier_1d_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief defines classifier bounds for sequence diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_1d_layouter_layout_for_sequence( pencil_classifier_1d_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief defines classifier bounds for timing diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 */
void pencil_classifier_1d_layouter_layout_for_timing( pencil_classifier_1d_layouter_t *this_, PangoLayout *font_layout );

/*!
 *  \brief positions the already composed classifiers as equally-spaced horizontal list to a target rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_list an array sorter object
 *  \param dest_rect an array sorter object
 *  \param v_alignment GEOMETRY_V_ALIGN_TOP, GEOMETRY_V_ALIGN_CENTER or GEOMETRY_V_ALIGN_BOTTOM
 */
void pencil_classifier_1d_layouter_private_layout_horizontal( const pencil_classifier_1d_layouter_t *this_,
                                                              const universal_array_index_sorter_t *classifier_list,
                                                              const geometry_rectangle_t *dest_rect,
                                                              geometry_v_align_t v_alignment
                                                            );

/*!
 *  \brief positions the already composed classifiers as equally-spaced vertical list to a target rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_list an array sorter object
 *  \param dest_rect an array sorter object
 *  \param h_alignment GEOMETRY_H_ALIGN_LEFT, GEOMETRY_H_ALIGN_CENTER or GEOMETRY_H_ALIGN_RIGHT
 */
void pencil_classifier_1d_layouter_private_layout_vertical( const pencil_classifier_1d_layouter_t *this_,
                                                            const universal_array_index_sorter_t *classifier_list,
                                                            const geometry_rectangle_t *dest_rect,
                                                            geometry_h_align_t h_alignment
                                                          );

/*!
 *  \brief positions the already composed classifiers linear to list_order horizontally to a target rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_list an array sorter object
 *  \param dest_rect an array sorter object
 *  \param v_alignment GEOMETRY_V_ALIGN_TOP, GEOMETRY_V_ALIGN_CENTER or GEOMETRY_V_ALIGN_BOTTOM
 */
void pencil_classifier_1d_layouter_private_linear_horizontal( const pencil_classifier_1d_layouter_t *this_,
                                                              const universal_array_index_sorter_t *classifier_list,
                                                              const geometry_rectangle_t *dest_rect,
                                                              geometry_v_align_t v_alignment
                                                            );

/*!
 *  \brief positions the already composed classifiers as linear to list_order vertically to a target rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_list an array sorter object
 *  \param dest_rect an array sorter object
 *  \param h_alignment GEOMETRY_H_ALIGN_LEFT, GEOMETRY_H_ALIGN_CENTER or GEOMETRY_H_ALIGN_RIGHT
 */
void pencil_classifier_1d_layouter_private_linear_vertical( const pencil_classifier_1d_layouter_t *this_,
                                                            const universal_array_index_sorter_t *classifier_list,
                                                            const geometry_rectangle_t *dest_rect,
                                                            geometry_h_align_t h_alignment
                                                          );

#endif  /* PENCIL_CLASSIFIER_1D_LAYOUTER_H */


/*
Copyright 2017-2023 Andreas Warnke

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
