/* File: gui_sketch_request.h; Copyright and License: see below */

#ifndef GUI_SKETCH_REQUEST_H
#define GUI_SKETCH_REQUEST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief states the tool-mode and the focused diagram id(s) to show in the sketch area
 */

#include "gui_tool.h"
#include "entity/data_id.h"
#include "set/data_small_set.h"

/*!
 *  \brief attributes of the request: tool-mode and diagram ids.
 */
struct gui_sketch_request_struct {
    gui_tool_t tool_mode;  /*!< the tool mode */
    data_id_t focused_diagram;  /*!< the focused diagram to display (except in search mode) */
    data_id_t parent_diagram;  /*!< the fallback diagram to display in case the focused_diagram is deleted */
    data_small_set_t search_result_diagrams;  /*!< the set of diagrams to display in search mode */
};

typedef struct gui_sketch_request_struct gui_sketch_request_t;

/*!
 *  \brief initializes the request
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_request_init( gui_sketch_request_t *this_ );

/*!
 *  \brief re-initializes the request
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_request_reinit( gui_sketch_request_t *this_ );

/*!
 *  \brief destroys the request
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_request_destroy( gui_sketch_request_t *this_ );

/*!
 *  \brief gets the attribute tool_mode
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline gui_tool_t gui_sketch_request_get_tool_mode( const gui_sketch_request_t *this_ );

/*!
 *  \brief sets the attribute tool_mode
 *
 *  \param this_ pointer to own object attributes
 *  \param tool_mode the new attribute value
 */
static inline void gui_sketch_request_set_tool_mode( gui_sketch_request_t *this_, gui_tool_t tool_mode );

/*!
 *  \brief gets the attribute focused_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline data_id_t gui_sketch_request_get_focused_diagram( const gui_sketch_request_t *this_ );

/*!
 *  \brief gets the attribute focused_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline data_row_id_t gui_sketch_request_get_focused_diagram_row_id( const gui_sketch_request_t *this_ );

/*!
 *  \brief sets the attribute focused_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param focused_diagram the new attribute value
 */
static inline void gui_sketch_request_set_focused_diagram( gui_sketch_request_t *this_, data_id_t focused_diagram );

/*!
 *  \brief sets the attribute focused_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param focused_diagram the new attribute value
 */
static inline void gui_sketch_request_set_focused_diagram_row_id( gui_sketch_request_t *this_, data_row_id_t focused_diagram );

/*!
 *  \brief gets the attribute parent_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline data_id_t gui_sketch_request_get_parent_diagram( const gui_sketch_request_t *this_ );

/*!
 *  \brief gets the attribute parent_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline data_row_id_t gui_sketch_request_get_parent_diagram_row_id( const gui_sketch_request_t *this_ );

/*!
 *  \brief sets the attribute parent_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram the new attribute value
 */
static inline void gui_sketch_request_set_parent_diagram( gui_sketch_request_t *this_, data_id_t parent_diagram );

/*!
 *  \brief sets the attribute parent_diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram the new attribute value
 */
static inline void gui_sketch_request_set_parent_diagram_row_id( gui_sketch_request_t *this_, data_row_id_t parent_diagram );

/*!
 *  \brief gets the attribute search_result_diagrams as const pointer
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline const data_small_set_t * gui_sketch_request_get_search_result_diagrams_const( const gui_sketch_request_t *this_ );

/*!
 *  \brief gets the attribute search_result_diagrams as pointer
 *
 *  \param this_ pointer to own object attributes
 *  \return the requested attribute
 */
static inline data_small_set_t * gui_sketch_request_get_search_result_diagrams_ptr( gui_sketch_request_t *this_ );


#include "gui_sketch_request.inl"

#endif  /* GUI_SKETCH_REQUEST_H */


/*
Copyright 2021-2024 Andreas Warnke

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
