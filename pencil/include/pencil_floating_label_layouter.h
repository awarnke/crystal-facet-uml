/* File: pencil_floating_label_layouter.h; Copyright and License: see below */

#ifndef PENCIL_FLOATING_LABEL_LAYOUTER_H
#define PENCIL_FLOATING_LABEL_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Evaluates proposed label positions in a diagram
 */

#include "draw/draw_relationship_label.h"
#include "draw/draw_feature_label.h"
#include "pencil_size.h"
#include "layout/layout_visible_set.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_dimensions.h"
#include "geometry/geometry_anchor.h"

/*!
 *  \brief attributes of the label layout helper
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_floating_label_layouter_struct {
    const pencil_size_t *pencil_size;  /*!< the pencil size proposes preferred distances */
};

typedef struct pencil_floating_label_layouter_struct pencil_floating_label_layouter_t;

/*!
 *  \brief initializes the label layout helper
 *
 *  \param this_ pointer to own object attributes
 *  \param pencil_size proposal for preferred distances
 */
void pencil_floating_label_layouter_init( pencil_floating_label_layouter_t *this_, const pencil_size_t *pencil_size );

/*!
 *  \brief destroys the label layout helper
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_floating_label_layouter_destroy( pencil_floating_label_layouter_t *this_ );

/*!
 *  \brief proposes one solution to layout a label rectangle for a layout_feature_t
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data data that is already (partly) layouted - by which to determine a suitable space
 *  \param anchor point where the object is located to which the label belongs
 *  \param preferred_dim preferred dimension of the label in case there is enough space
 *  \param draw_estimator drawing estimator that can estimate the drawing bounds given the available width
 *  \param feature data that is already (partly) layouted - by which to select a solution
 *  \param profile profile-part that provides the stereotype definition of the element to be drawn
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[out] out_solution a solution
 */
void pencil_floating_label_layouter_propose_solution_feat ( const pencil_floating_label_layouter_t *this_,
                                                            layout_visible_set_t *layout_data,
                                                            const geometry_anchor_t *anchor,
                                                            const geometry_dimensions_t *preferred_dim,
                                                            const draw_feature_label_t *draw_estimator,
                                                            const layout_feature_t *feature,
                                                            const data_profile_part_t *profile,
                                                            PangoLayout *font_layout,
                                                            geometry_rectangle_t *out_solution
                                                          );

/*!
 *  \brief proposes one solution to layout a label rectangle for a layout_relationship_t
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data data that is already (partly) layouted - by which to determine a suitable space
 *  \param anchor point where the object is located to which the label belongs
 *  \param preferred_dim preferred dimension of the label in case there is enough space
 *  \param draw_estimator drawing estimator that can estimate the drawing bounds given the available width
 *  \param relation data that is already (partly) layouted - by which to select a solution
 *  \param profile profile-part that provides the stereotype definition of the element to be drawn
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[out] out_solution a solution
 */
void pencil_floating_label_layouter_propose_solution_rel ( const pencil_floating_label_layouter_t *this_,
                                                           layout_visible_set_t *layout_data,
                                                           const geometry_anchor_t *anchor,
                                                           const geometry_dimensions_t *preferred_dim,
                                                           const draw_relationship_label_t *draw_estimator,
                                                           const layout_relationship_t *relation,
                                                           const data_profile_part_t *profile,
                                                           PangoLayout *font_layout,
                                                           geometry_rectangle_t *out_solution
                                                         );

/*!
 *  \brief proposes one solution to layout a label rectangle for a layout_feature_t or a layout_relationship_t
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data data that is already (partly) layouted - by which to determine a suitable space
 *  \param anchor point where the object is located to which the label belongs
 *  \param preferred_dim preferred dimension of the label in case there is enough space
 *  \param draw_estimator_feat drawing estimator that can estimate the drawing bounds given the available width or NULL
 *  \param feature data that is already (partly) layouted - by which to select a solution or NULL
 *  \param draw_estimator_rel drawing estimator that can estimate the drawing bounds given the available width or NULL
 *  \param relation data that is already (partly) layouted - by which to select a solution or NULL
 *  \param profile profile-part that provides the stereotype definition of the element to be drawn
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param[out] out_solution a solution
 */
static inline void pencil_floating_label_layouter_private_propose_solution ( const pencil_floating_label_layouter_t *this_,
                                                                             layout_visible_set_t *layout_data,
                                                                             const geometry_anchor_t *anchor,
                                                                             const geometry_dimensions_t *preferred_dim,
                                                                             const draw_feature_label_t *draw_estimator_feat,
                                                                             const layout_feature_t *feature,
                                                                             const draw_relationship_label_t *draw_estimator_rel,
                                                                             const layout_relationship_t *relation,
                                                                             const data_profile_part_t *profile,
                                                                             PangoLayout *font_layout,
                                                                             geometry_rectangle_t *out_solution
                                                                           );

/*!
 *  \brief selects one solution to layout a label rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param layout_data data that is already (partly) layouted - by which to select a solution
 *  \param target_point point where the object is located to which the label belongs
 *  \param solutions_count number of proposed solutions; at least 1 shall be provided
 *  \param solutions array of solutions
 *  \param out_index_of_best index of the best solution; must not be NULL.
 */
void pencil_floating_label_layouter_select_solution ( pencil_floating_label_layouter_t *this_,
                                                      layout_visible_set_t *layout_data,
                                                      geometry_point_t target_point,
                                                      uint32_t solutions_count,
                                                      const geometry_rectangle_t solutions[],
                                                      uint32_t *out_index_of_best
                                                    );

#include "pencil_floating_label_layouter.inl"

#endif  /* PENCIL_FLOATING_LABEL_LAYOUTER_H */


/*
Copyright 2019-2025 Andreas Warnke

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
