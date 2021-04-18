/* File: gui_sketch_nav_tree.h; Copyright and License: see below */

#ifndef GUI_SKETCH_NAV_TREE_H
#define GUI_SKETCH_NAV_TREE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Shows a tree view
 */

#include "sketch_area/gui_sketch_marker.h"
#include "sketch_area/gui_sketch_action.h"
#include "gui_marked_set.h"
#include "gui_resources.h"
#include "gui_error.h"
#include "util/shape/shape_int_rectangle.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "layout/layout_order.h"
#include <pango/pangocairo.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>

extern const int GUI_SKETCH_NAV_TREE_LINE_HEIGHT;  /*!< height of a diagram-name/entry in pixels */
extern const int GUI_SKETCH_NAV_TREE_INDENT;  /*!< left indentation space of a diagram-name/entry in pixels */

/*!
 *  \brief constants of gui_sketch_nav_tree_t
 */
enum gui_sketch_nav_tree_const_enum {
    GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS = 16,  /*!< maximum number of parents/grand-partents/grand-grand-parents */
    GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS = 96,  /*!< maximum number of sisters/brothers */
    GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN = 96,  /*!< maximum number of daughters/sons */
};

/*!
 *  \brief attributes of the nav tree
 * 
 * The nav tree is a subwidget to the gui_sketch_area:
 * - it knows its data to draw (tree-parts of diagrams)
 * - it layouts the data to show
 * - it draws the data
 * - it answers which data-object is layouted to which position
 * This subwidget does not get or respond-to user actions or listens to events of other widgets;
 * also it does not trigger events to other widgets
 */
struct gui_sketch_nav_tree_struct {
    bool visible;  /*!< is the nav tree visible */
    shape_int_rectangle_t bounds;  /*!< bounding box of the nav tree */

    /* diagram data to be shown */
    uint32_t ancestors_count;  /*!< number of ancestors, incuding self */
    data_diagram_t ancestor_diagrams[GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS];  /*!< current diagram is at index 0 */
    uint32_t siblings_count;
    data_diagram_t sibling_diagrams[GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS];
    int32_t siblings_self_index;  /*!< index of current diagram in list of siblings, -1 in case of error */
    uint32_t children_count;
    data_diagram_t child_diagrams[GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN];

    /* layout information, what is shown where, initialized by gui_sketch_nav_tree_private_do_layout */
    uint32_t line_idx_ancestors_start;  /*! always 0, even if there are no ancestors */
    uint32_t line_cnt_ancestors;  /*!< number of ancestors, excluding self */
    uint32_t line_idx_siblings_start;  /*! line of first sibling; undefined in case of program-internal error */
    uint32_t line_cnt_siblings_to_incl_self;  /*! lines of siblings to and including self */
    uint32_t line_idx_siblings_next_after_self;  /*! line of next sibling after self; undefined if there is no next sibling */
    uint32_t line_cnt_siblings_after_self;  /*! lines of siblings after self, excluding self */
    uint32_t line_idx_self;  /*! line of self; undefined in case of program-internal error */
    uint32_t line_idx_children_start;  /*! line of first child; undefined if there are no children */
    uint32_t line_cnt_children;  /*! identical to children_count */
    int32_t line_idx_new_child;  /*! line of new child button; -1 if there is no such button */
    int32_t line_idx_new_sibling;  /*! line of new sibling button; -1 if there is no such button */
    int32_t line_idx_new_root;  /*! 0 equals line of new root button; -1 if there is no such button */

    /* helper classes to perform drawing */
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
 *  and calls gui_sketch_nav_tree_private_do_layout to ensure consistency of own attributes
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to load
 *  \param db_reader pointer to a database reader object
 */
void gui_sketch_nav_tree_load_data( gui_sketch_nav_tree_t *this_, data_row_id_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief gets the id of the root diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the root diagram, DATA_ROW_ID_VOID if no diagrams exist
 */
static inline data_row_id_t gui_sketch_nav_tree_get_root_diagram_id ( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief gets the highest list order of the sibling diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \return the highest list order of the siblings diagrams, 0 if no sibling diagrams exist
 */
static inline int32_t gui_sketch_nav_tree_get_siblings_highest_order ( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief gets the highest list order of the children diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \return the highest list order of the children diagrams, 0 if no children diagrams exist
 */
static inline int32_t gui_sketch_nav_tree_get_children_highest_order ( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief checks if one diagram is a direct child or indirect descendant of the other
 *
 *  \param this_ pointer to own object attributes
 *  \param probe_ancestor_id the id of the possible parent/ancestor diagram.
 *  \param probe_descendant_id the id of the possible child/descendant diagram.
 *  \param out_is_descendant true if probe_ancestor_id is an ancestor of probe_descendant_id.
 *  \return GUI_ERROR_NONE if both ids are in the cache, GUI_ERROR_UNKNOWN_OBJECT otherwise
 */
static inline gui_error_t gui_sketch_nav_tree_is_descendant ( const gui_sketch_nav_tree_t *this_,
                                                              data_row_id_t probe_ancestor_id,
                                                              data_row_id_t probe_descendant_id,
                                                              bool *out_is_descendant
                                                            );

/*!
 *  \brief calculates the layout-line indices
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_nav_tree_private_do_layout( gui_sketch_nav_tree_t *this_ );

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
static inline shape_int_rectangle_t gui_sketch_nav_tree_get_bounds( const gui_sketch_nav_tree_t *this_ );

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
static inline bool gui_sketch_nav_tree_is_visible( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief sets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \param visible true if this card is currently visible, false otherwise
 */
static inline void gui_sketch_nav_tree_set_visible( gui_sketch_nav_tree_t *this_, bool visible );

/*!
 *  \brief gets the address of the diagram data of gui_sketch_nav_tree_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to diagram
 */
static inline const data_diagram_t *gui_sketch_nav_tree_get_diagram_ptr ( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief gets the action-id of the button at a given position.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_action_id the action id ofthe button at the given location. GUI_SKETCH_ACTION_NONE if there is no button at the given location.
 */
static inline void gui_sketch_nav_tree_get_button_at_pos ( const gui_sketch_nav_tree_t *this_,
                                                           int32_t x,
                                                           int32_t y,
                                                           gui_sketch_action_t *out_action_id
                                                         );

/*!
 *  \brief gets the diagram-id of the diagram_t at a given position.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id at the given location. The id is invalid if there is no object at the given location.
 */
void gui_sketch_nav_tree_get_object_id_at_pos ( const gui_sketch_nav_tree_t *this_,
                                                int32_t x,
                                                int32_t y,
                                                data_id_t *out_selected_id
                                              );

/*!
 *  \brief gets the gap information at a given position.
 *
 *  Either it gets the real, de-referenced object at a given position, e.g. a diagram_t or a classifier_t,
 *  or the visible object at a given position, e.g. a diagram_t or a diagramelement_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_parent_id the parent object id at the given location. The id is invalid if the root location was chosen.
 *  \param out_list_order the list_order at the given location.
 *  \param out_gap_line the line coordinates to show a gap cursor bar.
 *  \return GUI_ERROR_NONE if x/y points to a valid gap, GUI_ERROR_OUT_OF_BOUNDS in case of out of bounds error
 */
gui_error_t gui_sketch_nav_tree_get_gap_info_at_pos ( const gui_sketch_nav_tree_t *this_,
                                                      int32_t x,
                                                      int32_t y,
                                                      data_id_t *out_parent_id,
                                                      int32_t *out_list_order,
                                                      shape_int_rectangle_t *out_gap_line
                                                    );

/*!
 *  \brief gets the bounding box of a diagram name
 *
 *  \param this_ pointer to own object attributes
 *  \param ancestor_index index of the ancestor
 *  \return bounding box of the diagram name
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_ancestor_bounds ( const gui_sketch_nav_tree_t *this_,
                                                                                      uint32_t ancestor_index
                                                                                    );

/*!
 *  \brief gets the bounding box of a diagram name
 *
 *  \param this_ pointer to own object attributes
 *  \param sibling_index index of the sibling. siblings_count if the position after the last sibling shall be returned (for e.g. a "new" button).
 *  \return bounding box of the diagram name
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_sibling_bounds ( const gui_sketch_nav_tree_t *this_,
                                                                                     uint32_t sibling_index
                                                                                   );

/*!
 *  \brief gets the bounding box of a diagram name
 *
 *  \param this_ pointer to own object attributes
 *  \param child_index index of the child. children_count if the position after the last child shall be returned (for e.g. a "new" button).
 *  \return bounding box of the diagram name
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_private_get_child_bounds ( const gui_sketch_nav_tree_t *this_,
                                                                                   uint32_t child_index
                                                                                 );

/*!
 *  \brief draws a single diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param marker set of all objects to be marked
 *  \param cr cairo drawing context
 */
void gui_sketch_nav_tree_draw ( gui_sketch_nav_tree_t *this_, gui_marked_set_t *marker, cairo_t *cr );

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
                                                      const GdkPixbuf *icon_1,
                                                      const char *label_1,
                                                      int x,
                                                      int y,
                                                      PangoLayout *layout,
                                                      cairo_t *cr
                                                    );

#include "gui_sketch_nav_tree.inl"

#endif  /* GUI_SKETCH_NAV_TREE_H */


/*
Copyright 2018-2021 Andreas Warnke

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
