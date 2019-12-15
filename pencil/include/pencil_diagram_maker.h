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
#include "option/pencil_type_filter.h"
#include "layout/layout_order.h"
#include "set/data_id_pair.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "set/data_small_set.h"
#include "data_id.h"
#include "set/data_visible_set.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the diagram painter
 */
struct pencil_diagram_maker_struct {
    const data_visible_set_t *input_data;  /*!< pointer to an external data cache */

    pencil_layouter_t layouter;  /* own instance of a layouter */

    pencil_diagram_painter_t diagram_painter;  /*!< own instance of a diagram_painter */
    pencil_classifier_composer_t classifier_painter;  /*!< own instance of a classifier painter */
    pencil_relationship_painter_t relationship_painter;  /*!< own instance of a relationship_painter */
    pencil_feature_painter_t feature_painter;  /*!< own instance of a feature_painter */
};

typedef struct pencil_diagram_maker_struct pencil_diagram_maker_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 */
static inline void pencil_diagram_maker_init( pencil_diagram_maker_t *this_, const data_visible_set_t *input_data );

/*!
 *  \brief re-initializes the painter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be drawn
 */
static inline void pencil_diagram_maker_reinit( pencil_diagram_maker_t *this_, const data_visible_set_t *input_data );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_diagram_maker_destroy( pencil_diagram_maker_t *this_ );

/*!
 *  \brief defines coordinates
 *
 *  Resets internal caches, changes in input_data are ok.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_bounds the diagram_bounds rectangle where to draw the diagram
 */
static inline void pencil_diagram_maker_define_grid ( pencil_diagram_maker_t *this_,
                                                      geometry_rectangle_t diagram_bounds
                                                    );

/*!
 *  \brief layouts the chosen diagram contents into the diagram_bounds area
 *
 *  No reset of internal caches, no structural changes in input_data are allowed - only updates of attributes.
 *
 *  \param this_ pointer to own object attributes
 *  \param cr a cairo drawing context, used to determine the font metrics in the given drawing context
 */
static inline void pencil_diagram_maker_layout_elements ( pencil_diagram_maker_t *this_, cairo_t *cr );

/*!
 *  \brief draws the chosen diagram contents into the diagram_bounds area of the cairo drawing context
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
 *  \brief gets the object-id of the object at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter a filter for object types. E.g. PENCIL_TYPE_FILTER_LIFELINE will return the classifier instead of the lifeline-feature.
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 *  \param out_surrounding_id the id of the embracing object at the given location. The id is invalid if there is no object at the given location.
 *  \return PENCIL_ERROR_OUT_OF_BOUNDS if the given position x, y is not in the diagram.
 */
static inline pencil_error_t pencil_diagram_maker_get_object_id_at_pos ( pencil_diagram_maker_t *this_,
                                                                         double x,
                                                                         double y,
                                                                         pencil_type_filter_t filter,
                                                                         data_id_pair_t* out_selected_id,
                                                                         data_id_pair_t* out_surrounding_id
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
static inline void pencil_diagram_maker_is_pos_on_grid ( pencil_diagram_maker_t *this_,
                                                         double x,
                                                         double y,
                                                         bool *out_x_on_grid,
                                                         bool *out_y_on_grid
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
pencil_error_t pencil_diagram_maker_get_order_at_pos ( pencil_diagram_maker_t *this_,
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
pencil_error_t pencil_diagram_maker_get_feature_order_at_pos ( pencil_diagram_maker_t *this_,
                                                               const data_feature_t *feature_ptr,
                                                               double x,
                                                               double y,
                                                               layout_order_t* out_layout_order
                                                             );

#include "pencil_diagram_maker.inl"

#endif  /* PENCIL_DIAGRAM_MAKER_H */


/*
Copyright 2016-2019 Andreas Warnke

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
