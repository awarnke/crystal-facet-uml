/* File: gui_marked_set.h; Copyright and License: see below */

#ifndef GUI_MARKED_SET_H
#define GUI_MARKED_SET_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief References objects that are marked (displayed as selected/focused/highlighted)
 */

#include "set/data_small_set.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch marker
 */
struct gui_marked_set_struct {
    data_id_t focused;  /*!<  references the one focused visible object, e.g. a data_diagram_t or a data_diagramelement_t (yellow corners) */
    data_id_t focused_diagram;  /*!< the focused diagram, the place where to e.g. insert pasted objects */
    data_id_t highlighted;  /*!<  references the one highlighted/mouse over object */
    data_id_t highlighted_diagram;  /*!< the highlighted diagram, the diagram to zoom in when clicking on the highlighted id */
    data_small_set_t selected_set;  /*!<  references all selected objects (pink corners) */
};

typedef struct gui_marked_set_struct gui_marked_set_t;

/*!
 *  \brief initializes the gui_marked_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_marked_set_init ( gui_marked_set_t *this_ );

/*!
 *  \brief re-initializes the gui_marked_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_marked_set_reinit ( gui_marked_set_t *this_ );

/*!
 *  \brief destroys the gui_marked_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_marked_set_destroy ( gui_marked_set_t *this_ );

/*!
 *  \brief gets the focused object id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the focused object. This is the visible object, e.g. a data_diagram_t or a data_diagramelement_t
 */
static inline data_id_t gui_marked_set_get_focused ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the focused_diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the focused diagram.
 */
static inline data_id_t gui_marked_set_get_focused_diagram ( const gui_marked_set_t *this_ );

/*!
 *  \brief checks if focused object id is valid
 *
 *  \param this_ pointer to own object attributes
 *  \return true if focused object id is valid
 */
static inline bool gui_marked_set_has_focus ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the highlighted object id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the highlighted object.
 */
static inline data_id_t gui_marked_set_get_highlighted ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the highlighted_diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the highlighted diagram, DATA_ID_VOID if no diagram was set.
 */
static inline data_id_t gui_marked_set_get_highlighted_diagram ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the selected object ids.
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to immutable selected set.
 */
static inline const data_small_set_t *gui_marked_set_get_selected_set_const ( const gui_marked_set_t *this_ );

/*!
 *  \brief adds or removes the object id from the selected set.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the id to toggle in the selected set
 */
static inline void gui_marked_set_toggle_selected_obj ( gui_marked_set_t *this_, data_id_t obj_id );

/*!
 *  \brief toggles the current focus and adds or removes the object id from the selected set.
 *
 *  If unfocused, the object becomes focused and selected.
 *  Another object may loose focus.
 *  If focused, the object becomes unfocused and unselected.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the id to toggle in the selected set
 *  \param diagram_id the id of the focused diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_toggle_obj ( gui_marked_set_t *this_, data_id_t obj_id, data_id_t diagram_id );

/*!
 *  \brief clears the selected set.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_marked_set_clear_selected_set ( gui_marked_set_t *this_ );

/*!
 *  \brief sets the focused object id and focused_diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the id of the visible object to set as focused; can be identical to diagram_id
 *  \param diagram_id the id of the focused diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_set_focused ( gui_marked_set_t *this_,
                                                data_id_t obj_id,
                                                data_id_t diagram_id
                                              );

/*!
 *  \brief sets the highlighted object id and the highlighted_diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the id to set as highlighted, can be identical to diagram_id
 *  \param diagram_id the id of the highlighted diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_set_highlighted ( gui_marked_set_t *this_,
                                                    data_id_t obj_id,
                                                    data_id_t diagram_id
                                                  );

/*!
 *  \brief un-sets the focused object id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_marked_set_clear_focused ( gui_marked_set_t *this_ );

/*!
 *  \brief un-sets the highlighted object id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_marked_set_clear_highlighted ( gui_marked_set_t *this_ );

#include "gui_marked_set.inl"

#endif  /* GUI_MARKED_SET_H */


/*
Copyright 2016-2022 Andreas Warnke

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
