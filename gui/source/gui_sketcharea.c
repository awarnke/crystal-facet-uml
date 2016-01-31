/* File: gui/gui_sketcharea.c; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "gui_sketcharea.h"
#include "util/trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>

/*
static void draw_rect (GdkDrawable *d, GdkGC *gc)
{
  
  GdkRegion visible = gdk_drawable_get_visible_region( d );
  
  gdk_draw_rectangle( d, gc, TRUE /* filled * /, visible.
  
  /* Draw with GDK * /
  gdk_draw_line (d, gc, 0, 0, 0, 50);
  gdk_draw_line (d, gc, 0, 50, 50, 50);
  gdk_draw_line (d, gc, 50, 50, 50, 0);
  gdk_draw_line (d, gc, 50, 0, 0, 0);
}
  
  
gboolean gui_sketcharea_expose_event_callback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    TRACE_BEGIN();


    int widget_width = widget->allocation.width;
    int widget_height = widget->allocation.height;
    
    GdkGC *gc;

    gc = gdk_gc_new (widget->window);
    draw_rect (widget->window, gc);
    g_object_unref (gc);
    
    TRACE_END();
    return TRUE;
}
*/

gboolean gui_sketcharea_draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  guint width, height;
  GdkRGBA color;

  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);
  cairo_arc (cr,
             width / 2.0, height / 2.0,
             MIN (width, height) / 2.0,
             0, 2 * G_PI);

  gtk_style_context_get_color (gtk_widget_get_style_context (widget),
                               0,
                               &color);
  gdk_cairo_set_source_rgba (cr, &color);

  cairo_fill (cr);

  return FALSE;
}


gboolean gui_sketcharea_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {
    }

    TRACE_END();
    return TRUE;
}


gboolean gui_sketcharea_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data )
{
    TRACE_BEGIN();

    int x;
    int y;
    GdkModifierType state;
    if ( evt->is_hint ) {
        /* deprecated */
        gdk_window_get_pointer( evt->window, &x, &y, &state );
    }
    else {
        x = (int) evt->x;
        y = (int) evt->y;
        state = (GdkModifierType) evt->state;
    }
    TRACE_INFO_INT_INT("x/y",x,y);
    if ( (state & GDK_BUTTON1_MASK) != 0 )
    {
        TRACE_INFO_INT("    btn",state);
    }

    TRACE_END();
    return TRUE;
}

gboolean gui_sketcharea_button_press_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();

    if ( evt->button == 1 ) {
        TRACE_INFO("press");
    }

    TRACE_END();
    return TRUE;
}

gboolean gui_sketcharea_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();

    if ( evt->button == 1 ) {
        TRACE_INFO("release");
    }

    TRACE_END();
    return TRUE;
}


