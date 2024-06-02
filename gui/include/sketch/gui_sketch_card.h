/* File: gui_sketch_card.h; Copyright and License: see below */

#ifndef GUI_SKETCH_CARD_H
#define GUI_SKETCH_CARD_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Caches diagram data and draws a diagram
 */

#include "sketch/gui_sketch_marker.h"
#include "sketch/gui_sketch_snap_state.h"
#include "sketch/gui_sketch_drag_state.h"
#include "layout/layout_subelement_id.h"
#include "gui_marked_set.h"
#include "gui_tool.h"
#include "shape/shape_int_rectangle.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "pencil_diagram_maker.h"
#include "set/data_visible_set.h"
#include "set/data_full_id.h"
#include "set/data_profile_part.h"
#include "layout/layout_order.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch card
 */
struct gui_sketch_card_struct {
    bool visible;  /*!< is the card visible */
    shape_int_rectangle_t bounds;  /*!< bounding box of the card */
    data_visible_set_t painter_input_data;  /*!< caches the diagram data */
    data_profile_part_t profile;  /*!< caches the stereotypes referenced from the current diagram */
    pencil_diagram_maker_t painter;  /*!< own instance of a diagram painter */
    bool dirty_elements_layout;  /*!< marker that elements need to be layouted */

    /* helper classes to perform drawing */
    gui_sketch_marker_t sketch_marker;
};

typedef struct gui_sketch_card_struct gui_sketch_card_t;

/*!
 *  \brief initializes the gui_sketch_card_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_init ( gui_sketch_card_t *this_ );

/*!
 *  \brief destroys the gui_sketch_card_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_card_destroy ( gui_sketch_card_t *this_ );

/*!
 *  \brief fetches the diagram data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to load
 *  \param db_reader pointer to a database reader object
 */
static inline void gui_sketch_card_load_data( gui_sketch_card_t *this_,
                                              data_id_t diagram_id,
                                              data_database_reader_t *db_reader );

/*!
 *  \brief marks the diagram data as invalid
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_card_invalidate_data( gui_sketch_card_t *this_ );

/*!
 *  \brief gets the valid info from the diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool gui_sketch_card_is_valid( const gui_sketch_card_t *this_ );

/*!
 *  \brief gets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch card
 */
static inline shape_int_rectangle_t gui_sketch_card_get_bounds( const gui_sketch_card_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box of this sketch card
 */
static inline void gui_sketch_card_set_bounds( gui_sketch_card_t *this_, shape_int_rectangle_t bounds );

/*!
 *  \brief gets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this sketch card is currently visible
 */
static inline bool gui_sketch_card_is_visible( const gui_sketch_card_t *this_ );

/*!
 *  \brief sets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \param visible true if this card is currently visible, false otherwise
 */
static inline void gui_sketch_card_set_visible( gui_sketch_card_t *this_, bool visible );

/*!
 *  \brief gets the id and kind of the element-part at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter_lifelines a filter for object types. true will return the classifier instead of the lifeline-feature.
 *  \return id and kind of element-part at the given position. The id is invalid if there is no element at the given position.
 */
layout_subelement_id_t gui_sketch_card_get_element_at_pos ( const gui_sketch_card_t *this_,
                                                            int32_t x,
                                                            int32_t y,
                                                            bool filter_lifelines
                                                          );

/*!
 *  \brief gets the id and kind of the classifier-part at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return id and kind of element-part at the given position. The id is invalid if there is no element at the given position.
 */
layout_subelement_id_t gui_sketch_card_private_get_classifier_at_pos ( const gui_sketch_card_t *this_,
                                                                       int32_t x,
                                                                       int32_t y
                                                                     );

/*!
 *  \brief gets the id and kind of the feature-part at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param filter_lifelines a filter for object types. true will return the classifier instead of the lifeline-feature.
 *  \return id and kind of element-part at the given position. The id is invalid if there is no element at the given position.
 */
layout_subelement_id_t gui_sketch_card_private_get_feature_at_pos ( const gui_sketch_card_t *this_,
                                                                    int32_t x,
                                                                    int32_t y,
                                                                    bool filter_lifelines
                                                                  );

/*!
 *  \brief gets the id and kind of the relationship-part at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param snap_distance maximum distance to the next connector line when to select the connector
 *  \return id and kind of element-part at the given position. The id is invalid if there is no element at the given position.
 */
layout_subelement_id_t gui_sketch_card_private_get_relationship_at_pos ( const gui_sketch_card_t *this_,
                                                                         int32_t x,
                                                                         int32_t y,
                                                                         int32_t snap_distance
                                                                       );

/*!
 *  \brief draws a single diagram
 *
 *  If gui_sketch_card_is_visible() is false, this method does nothing.
 *
 *  \param this_ pointer to own object attributes
 *  \param marker set of all objects to be marked
 *  \param cr cairo drawing context
 */
void gui_sketch_card_draw ( gui_sketch_card_t *this_, gui_marked_set_t *marker, cairo_t *cr );

/*!
 * \brief gets the address of the diagram within the painter input data of gui_sketch_card_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to diagram, never NULL
 */
static inline const data_diagram_t * gui_sketch_card_get_diagram_const ( const gui_sketch_card_t *this_ );

/*!
 * \brief gets the address of the diagram within the painter input data of gui_sketch_card_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to diagram, never NULL
 */
static inline data_diagram_t * gui_sketch_card_get_diagram_ptr ( gui_sketch_card_t *this_ );

/*!
 * \brief gets the id of the diagram shown in gui_sketch_card_t
 *
 *  \param this_ pointer to own object attributes
 *  \return id of diagram, DATA_ID_VOID if none
 */
static inline data_id_t gui_sketch_card_get_diagram_id ( const gui_sketch_card_t *this_ );

/*!
 *  \brief gets the order value at a given position
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id object id for which to determine the list order.
 *                The object may be of type DATA_TABLE_CLASSIFIER, DATA_TABLE_FEATURE or DATA_TABLE_RELATIONSHIP.
 *  \param x x-position
 *  \param y y-position
 *  \return the list order value at the given location
 */
static inline layout_order_t gui_sketch_card_get_order_at_pos ( const gui_sketch_card_t *this_,
                                                                data_id_t obj_id,
                                                                int32_t x,
                                                                int32_t y );

/*!
 *  \brief gets the order value at a given position for a feature
 *
 *  Avoid using this function, prefer to use gui_sketch_card_get_order_at_pos()
 *  unless the feature is new/fake and does not yet have an ID.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr feature for which to determine the list order; must not be NULL.
 *  \param x x-position
 *  \param y y-position
 *  \return the list order value at the given location
 */
static inline int32_t gui_sketch_card_get_feature_order_at_pos ( const gui_sketch_card_t *this_,
                                                                 const data_feature_t *feature_ptr,
                                                                 int32_t x,
                                                                 int32_t y
                                                               );

/*!
 *  \brief determines if the given position is on a grid line
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \return a pair of bool values indicating if x- and y- position values are on grid lines
 */
static inline gui_sketch_snap_state_t gui_sketch_card_is_pos_on_grid ( const gui_sketch_card_t *this_, int32_t x, int32_t y );

/*!
 *  \brief determines the grid lines
 *
 *  \param this_ pointer to own object attributes
 *  \param out_bounds bounding rectangle where the grid lines are distributed equal-distance
 *  \param out_x_count number of x-position grid lines (vertical lines)
 *  \param out_y_count number of y-position grid lines (horizontal lines)
 */
static inline void gui_sketch_card_get_grid_area ( const gui_sketch_card_t *this_,
                                                   shape_int_rectangle_t *out_bounds,
                                                   uint32_t *out_x_count,
                                                   uint32_t *out_y_count
                                                 );

/*!
 *  \brief moves an object to an order (without modifying the database)
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id object id which to move (modify the x-/y- or list order).
 *                The object may be of type DATA_TABLE_CLASSIFIER, DATA_TABLE_FEATURE or DATA_TABLE_RELATIONSHIP.
 *  \param order layout_order_t, where to move the object to
 */
void gui_sketch_card_move_object_to_order ( gui_sketch_card_t *this_,
                                            data_id_t obj_id,
                                            const layout_order_t *order
                                          );

/*!
 *  \brief gets the dirty_elements_layout flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the elements on this this sketch card need to be re-layouted
 */
static inline bool gui_sketch_card_needs_layout( const gui_sketch_card_t *this_ );

/*!
 *  \brief lays out the elements, resets the dirty_elements_layout flag
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context, needed to determine the font metrics
 */
static inline void gui_sketch_card_layout_elements( gui_sketch_card_t *this_, cairo_t *cr );

/*!
 *  \brief lays out the diagram (grid and elements)
 *
 *  This method may be called even if gui_sketch_card_is_valid() is false.
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context, needed to determine the font metrics
 */
static inline void gui_sketch_card_do_layout( gui_sketch_card_t *this_, cairo_t *cr );

/*!
 *  \brief gets the layouted visible_set
 *
 *  \param this_ pointer to own object attributes
 *  \return visible_set containing the layouter elements on this this sketch card
 */
static inline const layout_visible_set_t * gui_sketch_card_get_visible_set( const gui_sketch_card_t *this_ );

/*!
 *  \brief determines the highest list order of relationships in the current diagram.
 *
 *  \param this_ pointer to own object attributes
 *  \result highest list order. May be used to append a new item to the end of the list.
 */
static inline int32_t gui_sketch_card_get_highest_rel_list_order( const gui_sketch_card_t *this_ );

/*!
 *  \brief determines the highest list order of features in the current diagram.
 *
 *  only Features of type DATA_FEATURE_TYPE_PROPERTY, DATA_FEATURE_TYPE_OPERATION
 *  are taken into account because their order ids reflect list position,
 *  neither a coordiante (e.g. a PORT) nor irrelevant (e.g. LIFELINES)
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id only features of this classifier are taken into account
 *  \result highest list order. May be used to append a new item to the end of the list.
 */
static inline int32_t gui_sketch_card_get_highest_feat_list_order( const gui_sketch_card_t *this_, data_id_t classifier_id );

#include "gui_sketch_card.inl"

#endif  /* GUI_SKETCH_CARD_H */


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
