/* File: pencil_diagram_maker.h; Copyright and License: see below */

#ifndef PENCIL_DIAGRAM_MAKER_H
#define PENCIL_DIAGRAM_MAKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Layouts and paints a diagram with all its contents into a cairo drawing context
 */

#include "pencil_layouter.h"
#include "pencil_marker.h"
#include "pencil_diagram_painter.h"
#include "pencil_classifier_composer.h"
#include "pencil_relationship_painter.h"
#include "pencil_size.h"
#include "pencil_error.h"
#include "layout/layout_order.h"
#include "geometry/geometry_rectangle.h"
#include "geometry/geometry_non_linear_scale.h"
#include "set/data_full_id.h"
#include "set/data_profile_part.h"
#include "set/data_small_set.h"
#include "set/data_stat.h"
#include "set/data_visible_set.h"
#include "data_diagram.h"
#include "data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the diagram painter
 */
struct pencil_diagram_maker_struct {
    const data_visible_set_t *input_data;  /*!< pointer to an external elements-data cache */
    const data_profile_part_t *profile;  /*!< pointer to an external stereotype-data cache */

    pencil_layouter_t layouter;  /* own instance of a layouter */

    pencil_diagram_painter_t diagram_painter;  /*!< own instance of a diagram_painter */
    pencil_classifier_composer_t classifier_painter;  /*!< own instance of a classifier painter */
    pencil_relationship_painter_t relationship_painter;  /*!< own instance of a relationship_painter */
    pencil_feature_painter_t feature_painter;  /*!< own instance of a feature_painter */

    double snap_to_grid_distance;  /*!< plus/minus some pixels shall snap to grid. */
                                   /*!< This is the expected accuracy for mouse input devices */
};

typedef struct pencil_diagram_maker_struct pencil_diagram_maker_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 *  \param profile pointer to the profile-part that provides the stereotypes of the elements to be drawn
 */
static inline void pencil_diagram_maker_init ( pencil_diagram_maker_t *this_,
                                               const data_visible_set_t *input_data,
                                               const data_profile_part_t *profile
                                             );

/*!
 *  \brief re-initializes the painter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 *  \param profile pointer to the profile-part that provided the stereotypes of the elements to be drawn
 */
static inline void pencil_diagram_maker_reinit ( pencil_diagram_maker_t *this_,
                                                 const data_visible_set_t *input_data,
                                                 const data_profile_part_t *profile
                                               );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_diagram_maker_destroy ( pencil_diagram_maker_t *this_ );

/*!
 *  \brief defines coordinates
 *
 *  Resets internal caches, any changes in input_data are ok, the profile needs to be updated accordingly.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_bounds the diagram_bounds rectangle where to draw the diagram
 *  \param cr a cairo drawing context, used to determine the font metrics in the given drawing context
 */
static inline void pencil_diagram_maker_define_grid ( pencil_diagram_maker_t *this_,
                                                      geometry_rectangle_t diagram_bounds,
                                                      cairo_t *cr
                                                    );

/*!
 *  \brief returns the geometry_grid_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline const geometry_grid_t *pencil_diagram_maker_get_grid_const ( const pencil_diagram_maker_t *this_ );

/*!
 *  \brief layouts the chosen diagram contents into the diagram_bounds area
 *
 *  No reset of internal caches, no structural changes in input_data are allowed;
 *  only updates of attributes like order values.
 *
 *  \param this_ pointer to own object attributes
 *  \param io_layout_stat pointer to already initialized statistics object where layouting statistics are added
 *                        or NULL if no statistics of interest
 *  \param cr a cairo drawing context, used to determine the font metrics in the given drawing context
 */
static inline void pencil_diagram_maker_layout_elements ( pencil_diagram_maker_t *this_,
                                                          data_stat_t *io_layout_stat,
                                                          cairo_t *cr
                                                        );

/*!
 *  \brief draws the chosen diagram contents into the diagram_bounds area of the cairo drawing context
 *
 *  This function may be called only after a call to pencil_diagram_maker_layout_elements()
 *
 *  \param this_ pointer to own object attributes
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param cr a cairo drawing context
 */
void pencil_diagram_maker_draw ( pencil_diagram_maker_t *this_,
                                 data_id_t mark_focused,
                                 data_id_t mark_highlighted,
                                 const data_small_set_t *mark_selected,
                                 cairo_t *cr
                               );

/*!
 *  \brief draws the classifiers and contained features into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_diagram_maker_private_draw_classifiers ( pencil_diagram_maker_t *this_,
                                                     data_id_t mark_focused,
                                                     data_id_t mark_highlighted,
                                                     const data_small_set_t *mark_selected,
                                                     PangoLayout *layout,
                                                     cairo_t *cr
                                                   );

/*!
 *  \brief draws the features of classifiers into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_diagram_maker_private_draw_features ( pencil_diagram_maker_t *this_,
                                                  data_id_t mark_focused,
                                                  data_id_t mark_highlighted,
                                                  const data_small_set_t *mark_selected,
                                                  PangoLayout *layout,
                                                  cairo_t *cr
                                                );

/*!
 *  \brief draws the relationships into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param mark_focused id of the object that is to be marked as "focused"
 *  \param mark_highlighted id of the object that is to be marked as "highlighted"
 *  \param mark_selected set of objects that are to be marked as "selected"
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_diagram_maker_private_draw_relationships ( pencil_diagram_maker_t *this_,
                                                       data_id_t mark_focused,
                                                       data_id_t mark_highlighted,
                                                       const data_small_set_t *mark_selected,
                                                       PangoLayout *layout,
                                                       cairo_t *cr
                                                     );

/*!
 *  \brief determines if the given position is on a grid line
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_x_on_grid flag indicating if the given x position is on a grid line
 *  \param out_y_on_grid flag indicating if the given y position is on a grid line
 */
static inline void pencil_diagram_maker_is_pos_on_grid ( const pencil_diagram_maker_t *this_,
                                                         double x,
                                                         double y,
                                                         bool *out_x_on_grid,
                                                         bool *out_y_on_grid
                                                       );

/*!
 *  \brief determines the grid lines
 *
 *  \param this_ pointer to own object attributes
 *  \param out_x0 x-position of leftmost grid line
 *  \param out_y0 y-position of topmost grid line
 *  \param out_dx width between grid lines
 *  \param out_dy height between grid lines
 *  \param out_x_count number of x-position grid lines
 *  \param out_y_count number of y-position grid lines
 */
static inline void pencil_diagram_maker_get_grid_lines ( const pencil_diagram_maker_t *this_,
                                                         double *out_x0,
                                                         double *out_y0,
                                                         double *out_dx,
                                                         double *out_dy,
                                                         uint32_t *out_x_count,
                                                         uint32_t *out_y_count
                                                       );

/*!
 *  \brief gets the layout order at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id object for which to determine the layout order
 *  \param x x-position
 *  \param y y-position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram,
 *          PENCIL_ERROR_UNKNOWN_OBJECT if the object is not in the diagram
 */
pencil_error_t pencil_diagram_maker_get_order_at_pos ( const pencil_diagram_maker_t *this_,
                                                       data_id_t obj_id,
                                                       double x,
                                                       double y,
                                                       layout_order_t* out_layout_order
                                                     );

/*!
 *  \brief gets the layout order at a given position for a feature
 *
 *  Avoid using this function, prefer to use pencil_diagram_maker_get_order_at_pos()
 *  unless the feature is new/fake and does not yet have an ID.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr feature for which to determine the layout order; not NULL
 *  \param x x-position
 *  \param y y-position
 *  \param out_layout_order order at given position
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram,
 *          PENCIL_ERROR_UNKNOWN_OBJECT if the parent classifier is not in the diagram
 */
pencil_error_t pencil_diagram_maker_get_feature_order_at_pos ( const pencil_diagram_maker_t *this_,
                                                               const data_feature_t *feature_ptr,
                                                               double x,
                                                               double y,
                                                               layout_order_t* out_layout_order
                                                             );

/*!
 *  \brief returns the layout_visible_set_t object
 *
 *  \param this_ pointer to own object attributes
 */
static inline const layout_visible_set_t *pencil_diagram_maker_get_layout_data_const ( const pencil_diagram_maker_t *this_ );

#include "pencil_diagram_maker.inl"

#endif  /* PENCIL_DIAGRAM_MAKER_H */


/*
Copyright 2016-2024 Andreas Warnke

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
