/* File: gui_sketcharea.h; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#ifndef GUI_SKETCHAREA_H
#define GUI_SKETCHAREA_H

#include <gtk/gtk.h>

/**
 *  redraws the sketcharea widget
 */
//gboolean gui_sketcharea_expose_event_callback(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean gui_sketcharea_draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);

gboolean gui_sketcharea_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data );
gboolean gui_sketcharea_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data );
gboolean gui_sketcharea_button_press_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );
gboolean gui_sketcharea_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data );

#endif  /* GUI_SKETCHAREA_H */
