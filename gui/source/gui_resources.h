/* File: gui_resources.h; Copyright and License: see below */

#ifndef GUI_RESOURCES_H
#define GUI_RESOURCES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides bitmap resources to the gui module
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*!
 *  \brief attributes of the gui_resources_t
 */
struct gui_resources_struct {
    GdkPixbuf *crystal_facet_uml;
    GdkPixbuf *edit_commit;
    GdkPixbuf *edit_copy;
    GdkPixbuf *edit_cut;
    GdkPixbuf *edit_delete;
    GdkPixbuf *edit_paste;
    GdkPixbuf *edit_redo;
    GdkPixbuf *edit_undo;
    GdkPixbuf *edit_instantiate;
    GdkPixbuf *edit_highlight;
    GdkPixbuf *edit_reset;
    GdkPixbuf *file_export;
    GdkPixbuf *file_new_window;
    GdkPixbuf *file_use_db;
    GdkPixbuf *message_error;
    GdkPixbuf *message_info;
    GdkPixbuf *message_warn;
    GdkPixbuf *message_user_doc;
    GdkPixbuf *tool_search;
    GdkPixbuf *tool_navigate;
    GdkPixbuf *tool_create;
    GdkPixbuf *tool_edit;
    GdkPixbuf *navigate_breadcrumb_folder;
    GdkPixbuf *navigate_closed_folder;
    GdkPixbuf *navigate_create_child;
    GdkPixbuf *navigate_create_sibling;
    GdkPixbuf *navigate_open_folder;
};

typedef struct gui_resources_struct gui_resources_t;

/*!
 *  \brief initializes the gui_resources_t
 *
 *  \param this_ pointer to own object attributes
 */
void gui_resources_init ( gui_resources_t *this_ );

/*!
 *  \brief destroys the gui_resources_t
 *
 *  \param this_ pointer to own object attributes
 */
void gui_resources_destroy ( gui_resources_t *this_ );

/*!
 *  \brief gets the crystal_facet_uml
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_crystal_facet_uml ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_commit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_commit ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_copy
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_copy ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_cut
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_cut ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_delete
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_delete ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_instantiate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_instantiate ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_highlight
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_highlight ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_paste
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_paste ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_redo
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_redo ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_undo
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_undo ( gui_resources_t *this_ );

/*!
 *  \brief gets the edit_reset
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_edit_reset ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_export
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_export ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_new_window
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_new_window ( gui_resources_t *this_ );

/*!
 *  \brief gets the file_use_db
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_file_use_db ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_error
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_error ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_info
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_info ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_warn
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_warn ( gui_resources_t *this_ );

/*!
 *  \brief gets the message_user_doc
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_message_user_doc ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_search
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_search ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_navigate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_navigate ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_create
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_create ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_edit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_edit ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_breadcrumb_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_breadcrumb_folder ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_closed_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_closed_folder ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_open_folder
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_open_folder ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_child
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_child ( gui_resources_t *this_ );

/*!
 *  \brief gets the navigate_create_sibling
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_navigate_create_sibling ( gui_resources_t *this_ );

#include "gui_resources.inl"

#endif  /* GUI_RESOURCES_H */


/*
Copyright 2016-2018 Andreas Warnke

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
