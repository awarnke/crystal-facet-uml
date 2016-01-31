/* File: gui_main_window.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef GUI_INIT_H
#define GUI_INIT_H

/* public file for the doxygen documentation: */
/*! \file */

#include <gtk/gtk.h>

/**
 *  initializes the gui
 */
void gui_main_window_init();

void gui_main_window_destroy_event(GtkWidget *widget, gpointer data);

gboolean gui_main_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

#endif  /* GUI_INIT_H */


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
