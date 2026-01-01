/* File: gui_sketch_nav_tree.h; Copyright and License: see below */

#ifndef GUI_SKETCH_NAV_TREE_H
#define GUI_SKETCH_NAV_TREE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Shows a tree view
 */

#include "sketch/gui_sketch_marker.h"
#include "sketch/gui_sketch_action.h"
#include "sketch/gui_sketch_style.h"
#include "sketch/gui_sketch_texture.h"
#include "sketch/gui_sketch_drag_state.h"
#include "gui_marked_set.h"
#include "gui_resources.h"
#include "gui_error.h"
#include "shape/shape_int_rectangle.h"
#include "pos/pos_nav_tree_node.h"
#include "pos/pos_nav_tree_gap.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "layout/layout_order.h"
#include <pango/pangocairo.h>
#include <cairo.h>
#include "gui_gtk.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants of gui_sketch_nav_tree_t
 */
enum gui_sketch_nav_tree_const_enum {
    GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS = 16,  /*!< maximum number of parents/grand-parents/grand-grand-parents */
    GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS = 96,  /*!< maximum number of sisters/brothers */
    GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN = 96,  /*!< maximum number of daughters/sons */
    GUI_SKETCH_NAV_TREE_CONST_MAX_NODES = ( GUI_SKETCH_NAV_TREE_CONST_MAX_ANCESTORS
                                          + GUI_SKETCH_NAV_TREE_CONST_MAX_SIBLINGS
                                          + GUI_SKETCH_NAV_TREE_CONST_MAX_CHILDREN
                                          + 1),  /*!< max diagrams, minus 1 for duplicate self plus 2 for new buttons */
    GUI_SKETCH_NAV_TREE_CONST_MAX_GAPS = GUI_SKETCH_NAV_TREE_CONST_MAX_NODES,  /* same as nodes, see do_layout */
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

    /* layout information */
    pos_nav_tree_node_t node_pos[GUI_SKETCH_NAV_TREE_CONST_MAX_NODES];  /*!< layout positions of diagrams and create-buttons */
    uint32_t node_count;  /*!< number of layout positions in node_pos list */
    pos_nav_tree_gap_t gap_pos[GUI_SKETCH_NAV_TREE_CONST_MAX_GAPS];  /*!< logical positions of gaps between diagrams */
    uint32_t gap_count;  /*!< number of logical positions in gap_pos list */

    /* helper classes to perform drawing */
    gui_sketch_style_t sketch_style;
    gui_sketch_marker_t sketch_marker;
    const gui_resources_t *resources;  /*!< pointer to external resources */
    gui_sketch_texture_t *texture_downloader;  /*!< pointer to external gui_sketch_texture_t */
};

typedef struct gui_sketch_nav_tree_struct gui_sketch_nav_tree_t;

/*!
 *  \brief initializes the gui_sketch_nav_tree_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param resources pointer to a graphics resource provider
 *  \param texture_downloader pointer to a gui_sketch_texture_t that can draw a texture onto a cairo drawing context
 */
void gui_sketch_nav_tree_init ( gui_sketch_nav_tree_t *this_,
                                const gui_resources_t *resources,
                                gui_sketch_texture_t *texture_downloader
                              );

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
 *  \param diagram_id id of the diagram to load, DATA_ROW_VOID to set empty data
 *  \param db_reader pointer to a database reader object
 */
void gui_sketch_nav_tree_load_data ( gui_sketch_nav_tree_t *this_, data_row_t diagram_id, data_database_reader_t *db_reader );

/*!
 *  \brief gets the id of the root diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return the id of the root diagram, DATA_ROW_VOID if no diagrams exist
 */
static inline data_row_t gui_sketch_nav_tree_get_root_diagram_id ( const gui_sketch_nav_tree_t *this_ );

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
                                                              data_row_t probe_ancestor_id,
                                                              data_row_t probe_descendant_id,
                                                              bool *out_is_descendant
                                                            );

/*!
 *  \brief calculates the layout-line indices / layouts the tree nodes
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_nav_tree_do_layout( gui_sketch_nav_tree_t *this_, cairo_t *cr );

/*!
 *  \brief layouts icon and label of a tree node
 *
 *  \param this_ pointer to own object attributes
 *  \param node the pos_nav_tree_node_t to layout
 *  \param tree_depth the depth in the tree / used for the indentation width
 *  \param[in,out] io_y_pos top position of the current node to be layouted; out: pos of next node
 *  \param font_layout the pango font rendering object
 */
void gui_sketch_nav_tree_private_layout_node ( gui_sketch_nav_tree_t *this_,
                                               pos_nav_tree_node_t *node,
                                               uint32_t tree_depth,
                                               int32_t *io_y_pos,
                                               PangoLayout *font_layout
                                             );

/*!
 *  \brief marks the diagram data as invalid
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_nav_tree_invalidate_data ( gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief gets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch card
 */
static inline shape_int_rectangle_t gui_sketch_nav_tree_get_bounds ( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box of this sketch card
 */
static inline void gui_sketch_nav_tree_set_bounds ( gui_sketch_nav_tree_t *this_, shape_int_rectangle_t bounds );

/*!
 *  \brief gets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this sketch card is currently visible
 */
static inline bool gui_sketch_nav_tree_is_visible ( const gui_sketch_nav_tree_t *this_ );

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
 *  \param out_action_id the action id of the button at the given location. GUI_SKETCH_ACTION_NONE if there is no button at the given location.
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
 * \brief gets the pos_nav_tree_node_t position object at index
 *
 *  \param this_ pointer to own object attributes
 *  \param index 0 &lt;= index &lt; node_count &lt;= GUI_SKETCH_NAV_TREE_CONST_MAX_NODES
 *  \return pointer to const pos_nav_tree_node_t
 */
static inline const pos_nav_tree_node_t *gui_sketch_nav_tree_get_node_pos_const ( const gui_sketch_nav_tree_t *this_,
                                                                                  uint32_t index
                                                                                );

/*!
 * \brief gets the number of node position objects
 *
 *  \param this_ pointer to own object attributes
 *  \return node_count
 */
static inline uint32_t gui_sketch_nav_tree_get_node_count ( const gui_sketch_nav_tree_t *this_ );

/*!
 * \brief gets the pos_nav_tree_gap_t position object at index
 *
 *  \param this_ pointer to own object attributes
 *  \param index 0 &lt;= index &lt; gap_count &lt;= GUI_SKETCH_NAV_TREE_CONST_MAX_GAPS
 *  \return pointer to const pos_nav_tree_gap_t
 */
static inline const pos_nav_tree_gap_t *gui_sketch_nav_tree_get_gap_pos_const ( const gui_sketch_nav_tree_t *this_,
                                                                                uint32_t index
                                                                              );

/*!
 * \brief gets the number of gap position objects
 *
 *  \param this_ pointer to own object attributes
 *  \return gap_count
 */
static inline uint32_t gui_sketch_nav_tree_get_gap_count ( const gui_sketch_nav_tree_t *this_ );

/*!
 *  \brief determines the position of a diagran node.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id the diagram id to be searched in the gui_sketch_nav_tree
 *  \param[out] out_node_envelope_box the envelope box of the diagram node.
 *                                    The envelope box encompasses the icon box and the label box of the search result.
 *  \return DATA_ERROR_NONE if the search result exists, U8_ERROR_NOT_FOUND if there is no fitting search result on the current page
 */
static inline u8_error_t gui_sketch_nav_tree_get_node_envelope ( gui_sketch_nav_tree_t *this_,
                                                                 const data_id_t* diagram_id,
                                                                 shape_int_rectangle_t* out_node_envelope_box
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
 *  \param node the pos_nav_tree_node_t to draw
 *  \param marker set of all objects to be marked
 *  \param font_layout the pango font rendering object for i18n suppoprt
 *  \param cr the cairo drawing engine
 */
void gui_sketch_nav_tree_private_draw_node ( gui_sketch_nav_tree_t *this_,
                                             const pos_nav_tree_node_t *node,
                                             const gui_marked_set_t *marker,
                                             PangoLayout *font_layout,
                                             cairo_t *cr
                                           );

/*!
 *  \brief draws overlay graphics for dragging a diagram to the cairo context
 *
 *  \param this_ pointer to own object attributes
 *  \param drag_state current dragging status
 *  \param cr cairo drawing context
 */
void gui_sketch_nav_tree_draw_overlay ( const gui_sketch_nav_tree_t *this_,
                                        const gui_sketch_drag_state_t *drag_state,
                                        cairo_t *cr
                                      );

#include "gui_sketch_nav_tree.inl"

#endif  /* GUI_SKETCH_NAV_TREE_H */


/*
Copyright 2018-2026 Andreas Warnke

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
