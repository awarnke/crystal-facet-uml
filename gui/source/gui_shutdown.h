/* File: gui/shutdown.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef GUI_SHUTDOWN_H
#define GUI_SHUTDOWN_H

#include <gtk/gtk.h>

void gui_window_destroy_event(GtkWidget *widget, gpointer data);

gboolean gui_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

#endif  /* GUI_SHUTDOWN_H */
