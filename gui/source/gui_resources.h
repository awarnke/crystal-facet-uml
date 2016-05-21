/* File: gui_resources.h; Copyright and License: see below */

#ifndef GUI_RESOURCES_H
#define GUI_RESOURCES_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*!
 *  \brief attributes of the gui_resources_t
 */
struct gui_resources_struct {
    GdkPixbuf *crystal_facet_uml;
    GdkPixbuf *edit_commit;
    GdkPixbuf *file_new_window;
    GdkPixbuf *file_use_db;
    GdkPixbuf *message_error;
    GdkPixbuf *message_info;
    GdkPixbuf *message_warn;
    GdkPixbuf *tool_create_diagram;
    GdkPixbuf *tool_navigate;
    GdkPixbuf *tool_create_object;
    GdkPixbuf *tool_edit;
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
 *  \brief gets the tool_create_diagram
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_create_diagram ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_navigate
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_navigate ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_create_object
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_create_object ( gui_resources_t *this_ );

/*!
 *  \brief gets the tool_edit
 *
 *  \param this_ pointer to own object attributes
 */
static inline GdkPixbuf *gui_resources_get_tool_edit ( gui_resources_t *this_ );

#include "gui_resources.inl"

#endif  /* GUI_RESOURCES_H */


/*
Copyright 2016-2016 Andreas Warnke

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
