/* File: gui_sketch_tools.h; Copyright and License: see below */

#ifndef GUI_SKETCH_TOOLS_H
#define GUI_SKETCH_TOOLS_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Informs the listener on the currently selected tool
 */

#include <gtk/gtk.h>

/*!
 *  \brief enumberation on tools
 */
enum gui_sketch_tools_tool_enum {
    GUI_SKETCH_TOOLS_NAVIGATE,
    GUI_SKETCH_TOOLS_EDIT,
    GUI_SKETCH_TOOLS_CREATE_DIAGRAM,
    GUI_SKETCH_TOOLS_CREATE_OBJECT,
};

typedef enum gui_sketch_tools_tool_enum gui_sketch_tools_tool_t;

/*!
 *  \brief attributes of the sketch area widget
 */
struct gui_sketch_tools_struct {
    gui_sketch_tools_tool_t selected_tool;
    GObject *listener;
};

typedef struct gui_sketch_tools_struct gui_sketch_tools_t;

extern const char *GUI_SKETCH_TOOLS_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_sketch_tools_t struct
 */
void gui_sketch_tools_init ( gui_sketch_tools_t *this_ );

/*!
 *  \brief destroys the gui_sketch_tools_t struct
 */
void gui_sketch_tools_destroy ( gui_sketch_tools_t *this_ );

/*!
 *  \brief gets the selected tool
 */
static inline gui_sketch_tools_tool_t gui_sketch_tools_get_selected_tool ( gui_sketch_tools_t *this_ );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_navigate_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_edit_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_create_object_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_create_diagram_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief sets the listener
 */
static inline void gui_sketch_tools_set_listener ( gui_sketch_tools_t *this_, GObject *listener );

/*!
 *  \brief removes the listener
 */
static inline void gui_sketch_tools_remove_listener ( gui_sketch_tools_t *this_ );

#include "gui_sketch_tools.inl"

#endif  /* GUI_SKETCH_TOOLS_H */


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
