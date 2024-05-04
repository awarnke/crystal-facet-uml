/* File: gui_marked_set.h; Copyright and License: see below */

#ifndef GUI_MARKED_SET_H
#define GUI_MARKED_SET_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief References objects that are marked (displayed as selected/focused/highlighted)
 */

#include "sketch/gui_sketch_action.h"
#include "layout/layout_subelement_id.h"
#include "layout/layout_subelement_kind.h"
#include "set/data_small_set.h"
#include "set/data_full_id.h"
#include <glib-object.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the marked set
 *
 *  A marked set keeps track of the currently focused and selected elements
 *  and knows the currently highlighted elements.
 *
 *  Even buttons (active icons) are represented to fit into the current redrawing concept.
 *
 *  Additionally it emits a signal to listeners when the focus changes.
 */
struct gui_marked_set_struct {
    data_full_id_t focused;  /*!<  references the one focused visible object, */
                             /*!<  e.g. a data_diagram_t or a data_diagramelement_t (yellow corners) */
                             /*!<  with an optional classifier id as the secondary id */
    data_id_t focused_diagram;  /*!< the focused diagram is the place where to e.g. insert pasted objects. */
                                /*!< Even if the focused object is VOID, the focused_diagram should be set */
    data_id_t highlighted;  /*!<  references the one highlighted/mouse over object */
    layout_subelement_kind_t highlighted_kind;  /*!< specifies which part of the element is highlighted */
    data_id_t highlighted_diagram;  /*!< the highlighted diagram, the diagram to zoom in when clicking on the highlighted id */
    gui_sketch_action_t highlighted_button;  /*!<  the buttons action-id if the highlighted/mouse over object is a button */
    data_small_set_t selected_set;  /*!<  references all selected objects (pink corners) */

    GObject *signal_source;  /*!<  The source gobject from which the changed-signal shall be emitted */
};

typedef struct gui_marked_set_struct gui_marked_set_t;

extern const char *GUI_MARKED_SET_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_marked_set_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_source the gobject at which listeners need to register to get changed-notifications
 */
void gui_marked_set_init ( gui_marked_set_t *this_, GObject *signal_source );

/*!
 *  \brief re-initializes the gui_marked_set_t struct
 *
 *  The signal_source stays unchanged.
 *
 *  \param this_ pointer to own object attributes
 */
void gui_marked_set_reinit ( gui_marked_set_t *this_ );

/*!
 *  \brief destroys the gui_marked_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_marked_set_destroy ( gui_marked_set_t *this_ );

/*!
 *  \brief gets the focused object id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the focused object. This is the visible object, e.g. a data_diagram_t or a data_diagramelement_t
 */
static inline data_id_t gui_marked_set_get_focused_obj ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the visible object id and the classifier that is associated to the focused object
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the visible object (and the classifier if the focused object is associated to a classifier),
 *          DATA_ID_VOID_ID otherwise
 */
static inline data_full_id_t gui_marked_set_get_focused ( const gui_marked_set_t *this_ );

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
 *  \brief gets the highlighted_kind of the highlighted subelement
 *
 *  \param this_ pointer to own object attributes
 *  \return kind of the highlighted subelement, LAYOUT_SUBELEMENT_KIND_VOID if no element was set.
 */
static inline layout_subelement_kind_t gui_marked_set_get_highlighted_kind ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the highlighted_diagram id
 *
 *  \param this_ pointer to own object attributes
 *  \return id of the highlighted diagram, DATA_ID_VOID if no diagram was set.
 */
static inline data_id_t gui_marked_set_get_highlighted_diagram ( const gui_marked_set_t *this_ );

/*!
 *  \brief gets the highlighted_button id
 *
 *  \param this_ pointer to own object attributes
 *  \return action-id of the highlighted button, GUI_SKETCH_ACTION_NONE if no button was set.
 */
static inline gui_sketch_action_t gui_marked_set_get_highlighted_button ( const gui_marked_set_t *this_ );

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
 *  Another object may loose its focus.
 *  If focused, the object becomes unfocused and unselected.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the id(s) to toggle in the selected set (primary) and to notify listeners (secondary)
 *  \param diagram_id the id of the focused diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_toggle_obj ( gui_marked_set_t *this_, data_full_id_t obj_id, data_id_t diagram_id );

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
 *  \param obj_id the id of the visible object to set as focused; can be identical to diagram_id.
 *                The secondary id is the one to be notifies as currently focused
 *  \param diagram_id the id of the focused diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_set_focused ( gui_marked_set_t *this_, data_full_id_t obj_id, data_id_t diagram_id );

/*!
 *  \brief sets the highlighted id and the highlighted_diagram id
 *
 *  The highlighted subelement kind is set to LAYOUT_SUBELEMENT_KIND_VOID.
 *  Unsets the highlighted button.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the id to set as highlighted, can be identical to diagram_id
 *  \param diagram_id the id of the highlighted diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_set_highlighted ( gui_marked_set_t *this_, data_id_t obj_id, data_id_t diagram_id );

/*!
 *  \brief sets the highlighted id, highlighted_kind and the highlighted_diagram id
 *
 *  Unsets the highlighted button.
 *
 *  \param this_ pointer to own object attributes
 *  \param subelement_id the id and kind to set as highlighted, can be identical to diagram_id
 *  \param diagram_id the id of the highlighted diagram; identical or parent to obj_id
 */
static inline void gui_marked_set_set_highlighted_subelement ( gui_marked_set_t *this_,
                                                               const layout_subelement_id_t *subelement_id,
                                                               data_id_t diagram_id
                                                             );

/*!
 *  \brief sets the highlighted button id
 *
 *  Unsets the highlighted object, highlighted_kind and highlighted_diagram id.
 *
 *  \param this_ pointer to own object attributes
 *  \param button_id the action-id of the button to set as highlighted, may be context specific
 */
static inline void gui_marked_set_set_highlighted_button ( gui_marked_set_t *this_, gui_sketch_action_t button_id );

/*!
 *  \brief un-sets the focused object id, but keeps the focused_diagram
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

/*!
 *  \brief notifies all listeners.
 *
 *  Sends the currently focused object id.
 *
 *  \param this_ pointer to own object attributes
 *  \param modified_real_object_id id of the real object that was modified (the classifier, not the diagramelement)
 */
void gui_marked_set_private_notify_listeners( gui_marked_set_t *this_, data_id_t modified_real_object_id );

#include "gui_marked_set.inl"

#endif  /* GUI_MARKED_SET_H */


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
