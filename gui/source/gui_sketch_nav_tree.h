/* File: gui_sketch_nav_tree.h; Copyright and License: see below */

#ifndef GUI_SKETCH_NAV_TREE_H
#define GUI_SKETCH_NAV_TREE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Shows a tree view
 */

#include "gui_marked_set.h"
#include "gui_sketch_marker.h"
#include "gui_sketch_action.h"
#include "gui_resources.h"
#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "layout/layout_order.h"
#include <pango/pangocairo.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants of gui_sketch_nav_tree_t
 */
enum gui_sketch_nav_tree_const_enum {
    GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS = 12,  /*!< maximum number of parents/grand-partents/grand-grand-parents */
    GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS = 128,  /*!< maximum number of sisters/brothers */
    GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN = 128,  /*!< maximum number of daughters/sons */
};

/*!
 *  \brief attributes of the nav tree
 */
struct gui_sketch_nav_tree_struct {
    bool visible;  /*!< is the nav tree visible */
    shape_int_rectangle_t bounds;  /*!< bounding box of the nav tree */

    uint32_t ancestors_count;
    data_diagram_t ancestor_diagrams[GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS];  /*!< current diagram is at index 0 */
    uint32_t siblings_count;
    data_diagram_t sibling_diagrams[GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS];
    int32_t siblings_self_index;  /*!< index of current diagram in list of siblings, -1 in case of error */
    uint32_t children_count;
    data_diagram_t child_diagrams[GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN];
    int32_t new_child_button_index;  /*!< index/line of the "new child" button, -1 if this button does not exist */
    int32_t new_sibling_button_index;  /*!< index/line of the "new sibling" button, -1 if this button does not exist */

    PangoFontDescription *standard_font_description;  /*!< text description of standard text */
    gui_sketch_marker_t sketch_marker;
    gui_resources_t *resources;  /*!< pointer to external resources */
};

typedef struct gui_sketch_nav_tree_struct gui_sketch_nav_tree_t;

/*!
 *  \brief initializes the gui_sketch_nav_tree_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param resources pointer to a graphics resource provider
 */
void gui_sketch_nav_tree_init ( gui_sketch_nav_tree_t *this_, gui_resources_t *resources );

/*!
 *  \brief destroys the gui_sketch_nav_tree_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_nav_tree_destroy ( gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief fetches the diagram data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to load
 *  \param db_reader pointer to a database reader object
 */
void gui_sketch_nav_tree_load_data( gui_sketch_nav_tree_t *this_, int64_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief marks the diagram data as invalid
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_nav_tree_invalidate_data( gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief gets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch card
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_get_bounds( gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box of this sketch card
 */
static inline void gui_sketch_nav_tree_set_bounds( gui_sketch_nav_tree_t *this_, shape_int_rectangle_t bounds );

/*!
 *  \brief gets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this sketch card is currently visible
 */
static inline bool gui_sketch_nav_tree_is_visible( gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief sets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \param visible true if this card is currently visible, false otherwise
 */
static inline void gui_sketch_nav_tree_set_visible( gui_sketch_nav_tree_t *this_, bool visible );

/*!
 *  \brief draws a single diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param marker set of all objects to be marked
 *  \param cr cairo drawing context
 */
void gui_sketch_nav_tree_draw ( gui_sketch_nav_tree_t *this_, gui_marked_set_t *marker, cairo_t *cr );

/*!
 *  \brief gets the address of the diagram data of gui_sketch_nav_tree_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to diagram
 */
static inline data_diagram_t *gui_sketch_nav_tree_get_diagram_ptr ( gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief gets the action-id of the button at a given position.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_action_id the action id ofthe button at the given location. GUI_SKETCH_ACTION_NONE if there is no button at the given location.
 */
static inline void gui_sketch_nav_tree_get_button_at_pos ( gui_sketch_nav_tree_t *this_,
                                                           int32_t x,
                                                           int32_t y,
                                                           gui_sketch_action_t* out_action_id
                                                         );

/*!
 *  \brief gets the object-id of the object at a given position.
 *
 *  Either it gets the real, de-referenced object at a given position, e.g. a diagram_t or a classifier_t,
 *  or the visible object at a given position, e.g. a diagram_t or a diagramelement_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 */
static inline void gui_sketch_nav_tree_get_object_id_at_pos ( gui_sketch_nav_tree_t *this_,
                                                              int32_t x,
                                                              int32_t y,
                                                              data_id_t* out_selected_id
                                                            );

/*!
 *  \brief gets the bounding box of a diagram name
 *
 *  \param this_ pointer to own object attributes
 *  \param ancestor_index index of the ancestor
 *  \return bounding box of the diagram name
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_ancestor_bounds ( gui_sketch_nav_tree_t *this_,
                                                                                      uint32_t ancestor_index
                                                                                    );

/*!
 *  \brief gets the bounding box of a diagram name
 *
 *  \param this_ pointer to own object attributes
 *  \param sibling_index index of the sibling. siblings_count if the position after the last sibling shall be returned (for e.g. a "new" button).
 *  \return bounding box of the diagram name
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_sibling_bounds ( gui_sketch_nav_tree_t *this_,
                                                                                     uint32_t sibling_index
                                                                                   );

/*!
 *  \brief gets the bounding box of a diagram name
 *
 *  \param this_ pointer to own object attributes
 *  \param child_index index of the child. children_count if the position after the last child shall be returned (for e.g. a "new" button).
 *  \return bounding box of the diagram name
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_child_bounds ( gui_sketch_nav_tree_t *this_,
                                                                                   uint32_t child_index
                                                                                 );

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
static inline layout_order_t gui_sketch_nav_tree_get_order_at_pos ( gui_sketch_nav_tree_t *this_, data_id_t obj_id, int32_t x, int32_t y );

/*!
 *  \brief moves an object to an order (without modifying the database)
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id object id which to move (modify the x-/y- or list order).
 *                The object may be of type DATA_TABLE_CLASSIFIER, DATA_TABLE_FEATURE or DATA_TABLE_RELATIONSHIP.
 *  \param order layout_order_t, where to move the object to
 */
static inline void gui_sketch_nav_tree_move_object_to_order ( gui_sketch_nav_tree_t *this_, data_id_t obj_id, layout_order_t *order );

/*!
 *  \brief draws an icon and a label
 *
 *  \param this_ pointer to own object attributes
 *  \param icon_1 the icon to draw
 *  \param label_1 the label to draw - using the pango library for i18n suppoprt
 *  \param x abscissae
 *  \param y ordinate
 *  \param layout the pango font rendering object for i18n suppoprt
 *  \param cr the cairo drawing engine
 */
void gui_sketch_nav_tree_private_draw_icon_and_label( gui_sketch_nav_tree_t *this_,
                                                      GdkPixbuf *icon_1,
                                                      const char *label_1,
                                                      int x,
                                                      int y,
                                                      PangoLayout *layout,
                                                      cairo_t *cr
                                                    );

#include "gui_sketch_nav_tree.inl"

#endif  /* GUI_SKETCH_NAV_TREE_H */


/*
Copyright 2018-2018 Andreas Warnke

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
