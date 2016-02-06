/* File: gui_sketch_tools.h; Copyright and License: see below */

#ifndef GUI_SKETCH_TOOLS_H
#define GUI_SKETCH_TOOLS_H

#include <gtk/gtk.h>

enum gui_sketch_tools_tool {
    GUI_SKETCH_TOOLS_NAVIGATE = 0,
    GUI_SKETCH_TOOLS_EDIT,
    GUI_SKETCH_TOOLS_CREATE_DIAGRAM,
    GUI_SKETCH_TOOLS_CREATE_OBJECT,
};

/*!
 *  \brief data which is liked to the sketch area widget
 */
struct gui_sketch_tools_struct {
    enum gui_sketch_tools_tool selected_tool;
};

typedef struct gui_sketch_tools_struct gui_sketch_tools_t;

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
