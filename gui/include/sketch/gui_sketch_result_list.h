/* File: gui_sketch_result_list.h; Copyright and License: see below */

#ifndef GUI_SKETCH_RESULT_LIST_H
#define GUI_SKETCH_RESULT_LIST_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Draws a list showing search results
 */

#include "sketch/gui_sketch_marker.h"
#include "sketch/gui_sketch_style.h"
#include "sketch/gui_sketch_texture.h"
#include "gui_marked_set.h"
#include "gui_resources.h"
#include "gui_type_resource_list.h"
#include "shape/shape_int_rectangle.h"
#include "pos/pos_search_result_page.h"
#include "pos/pos_search_result.h"
#include "storage/data_database.h"
#include "set/data_search_result.h"
#include "set/data_search_result_list.h"
#include "ctrl_controller.h"
#include "layout/layout_order.h"
#include "gui_gtk.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for maximum values of gui_sketch_result_list_t
 */
enum gui_sketch_result_list_max_enum {
    GUI_SKETCH_RESULT_LIST_MAX_ARRAY_SIZE = 128,  /*!< maximum number of total search results */
    GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS = 128,  /*!< maximum number of displayed search results on current page */
};

/*!
 *  \brief attributes of the result list
 *
 * The result list is a subwidget to the gui_sketch_area:
 * - it knows its data to draw (list of search results)
 * - it layouts the data to show
 * - it draws the data
 * - it answers which data-object is layouted to which position
 * This subwidget does not get or respond-to user actions or listens to events of other widget;
 * also it does not trigger events to other widgets
 */
struct gui_sketch_result_list_struct {
    bool visible;  /*!< is the result list visible */
    shape_int_rectangle_t bounds;  /*!< bounding box of the result list */

    data_search_result_t result_list_buf[GUI_SKETCH_RESULT_LIST_MAX_ARRAY_SIZE];  /*!< list of all results */
    data_search_result_list_t result_list;

    /* layout information */
    pos_search_result_t element_pos[GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS];  /*!< layout positions of search results on current visible page */
    uint32_t element_count;  /*!< number of layout positions in element_pos list */

    /* helper classes to perform drawing */
    gui_sketch_style_t sketch_style;
    gui_sketch_marker_t sketch_marker;
    gui_resources_t *resources;  /*!< pointer to external resources */
    gui_type_resource_list_t selector;  /*!< own instance of a resource selector */
    gui_sketch_texture_t *texture_downloader;  /*!< pointer to external gui_sketch_texture_t */
};

typedef struct gui_sketch_result_list_struct gui_sketch_result_list_t;

/*!
 *  \brief initializes the gui_sketch_result_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param resources pointer to a graphics resource provider
 *  \param texture_downloader pointer to a gui_sketch_texture_t that can draw a texture onto a cairo drawing context
 */
void gui_sketch_result_list_init ( gui_sketch_result_list_t *this_,
                                   gui_resources_t *resources,
                                   gui_sketch_texture_t *texture_downloader
                                 );

/*!
 *  \brief destroys the gui_sketch_result_list_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_result_list_destroy ( gui_sketch_result_list_t *this_ );

/*!
 *  \brief fetches the diagram data from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param result_list list of search results and their diagram ids to load
 *  \param db_reader pointer to a database reader object
 */
static inline void gui_sketch_result_list_load_data( gui_sketch_result_list_t *this_,
                                                     const data_search_result_list_t *result_list,
                                                     data_database_reader_t *db_reader
                                                   );

/*!
 *  \brief marks the diagram data as invalid
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_result_list_invalidate_data( gui_sketch_result_list_t *this_ );

/*!
 *  \brief gets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \return returns the bounding box of this sketch card
 */
static inline shape_int_rectangle_t gui_sketch_result_list_get_bounds( gui_sketch_result_list_t *this_ );

/*!
 *  \brief sets the bounds rectangle
 *
 *  \param this_ pointer to own object attributes
 *  \param bounds bounding box of this sketch card
 */
static inline void gui_sketch_result_list_set_bounds( gui_sketch_result_list_t *this_, shape_int_rectangle_t bounds );

/*!
 *  \brief gets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \return true if this sketch card is currently visible
 */
static inline bool gui_sketch_result_list_is_visible( gui_sketch_result_list_t *this_ );

/*!
 *  \brief sets the visible flag
 *
 *  \param this_ pointer to own object attributes
 *  \param visible true if this card is currently visible, false otherwise
 */
static inline void gui_sketch_result_list_set_visible( gui_sketch_result_list_t *this_, bool visible );

/*!
 *  \brief calculates the coordinates of the search result list entries
 *
 *  \param this_ pointer to own object attributes
 *  \param cr cairo drawing context
 */
void gui_sketch_result_list_do_layout( gui_sketch_result_list_t *this_, cairo_t *cr );

/*!
 *  \brief positions the type-icon and the label
 *
 *  \param this_ pointer to own object attributes
 *  \param element pointer to the pos_search_result_t which to position
 *  \param[in,out] io_y_pos top position of the current element to be layouted; out: pos of next element
 *  \param font_layout the pango font rendering object for i18n suppoprt
 */
void gui_sketch_result_list_private_layout_element ( gui_sketch_result_list_t *this_,
                                                     pos_search_result_t *element,
                                                     int32_t *io_y_pos,
                                                     PangoLayout *font_layout
                                                   );

/*!
 *  \brief determines the object at a given position and returns its id. The object can be a diagram.
 *
 *  \param this_ pointer to own object attributes
 *  \param x x-position
 *  \param y y-position
 *  \param out_selected_id the object id of the object at the given location. The id is invalid if there is no object at the given location.
 *  \param out_diagram_id the diagram id of the object at the given location. The id is invalid if there is no object at the given location.
 */
static inline void gui_sketch_result_list_get_object_id_at_pos ( const gui_sketch_result_list_t *this_,
                                                                 int32_t x,
                                                                 int32_t y,
                                                                 data_id_t* out_selected_id,
                                                                 data_id_t* out_diagram_id
                                                               );

/*!
 * \brief gets the pos_search_result_t position object at index
 *
 *  \param this_ pointer to own object attributes
 *  \param index 0 &lt;= index &lt; element_count &lt;=  GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS
 *  \return pointer to const pos_search_result_t
 */
static inline const pos_search_result_t *gui_sketch_result_list_get_element_pos_const ( const gui_sketch_result_list_t *this_,
                                                                                        uint32_t index
                                                                                      );

/*!
 * \brief gets the number of position objects
 *
 *  \param this_ pointer to own object attributes
 *  \return element_count
 */
static inline uint32_t gui_sketch_result_list_get_element_count ( const gui_sketch_result_list_t *this_ );

/*!
 *  \brief draws the search result list
 *
 *  \param this_ pointer to own object attributes
 *  \param marker set of all objects to be marked
 *  \param cr cairo drawing context
 */
void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, const gui_marked_set_t *marker, cairo_t *cr );

/*!
 *  \brief draws a type-icon and a label
 *
 *  \param this_ pointer to own object attributes
 *  \param element pointer to the pos_search_result_t of which to draw icon and label
 *  \param marker set of all objects to be marked
 *  \param font_layout the pango font rendering object for i18n suppoprt
 *  \param cr the cairo drawing engine
 */
void gui_sketch_result_list_private_draw_element ( gui_sketch_result_list_t *this_,
                                                   const pos_search_result_t *element,
                                                   const gui_marked_set_t *marker,
                                                   PangoLayout *font_layout,
                                                   cairo_t *cr
                                                 );

#include "gui_sketch_result_list.inl"

#endif  /* GUI_SKETCH_RESULT_LIST_H */


/*
Copyright 2018-2025 Andreas Warnke

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
