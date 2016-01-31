/* File: gui_shutdown.c; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "gui_shutdown.h"
#include "trace.h"
#include <gtk/gtk.h>

void gui_window_destroy_event(GtkWidget *widget, gpointer data)
{
    TRACE_BEGIN();
    
    gtk_main_quit();
    
    TRACE_END();
}

gboolean gui_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    TRACE_BEGIN();
    
    TRACE_END();
    return FALSE;  /* return false to trigger destroy event */
}