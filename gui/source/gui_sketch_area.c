/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>

void gui_sketch_area_init ( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}

void gui_sketch_area_destroy ( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}

gboolean gui_sketch_area_draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    TRACE_BEGIN();
    guint width, height;
    GdkRGBA color;

    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    cairo_arc (cr,
             width / 2.0, height / 2.0,
             MIN (width, height) / 2.3,
             0.0, 2.0 * G_PI);

    gtk_style_context_get_color (gtk_widget_get_style_context (widget),
                               0,
                               &color);
    gdk_cairo_set_source_rgba (cr, &color);

    cairo_fill (cr);

    TRACE_END();
    return FALSE;
}


gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {
    }

    TRACE_END();
    return TRUE;
}


gboolean gui_sketch_area_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data )
{
    TRACE_BEGIN();

    int x;
    int y;
    GdkModifierType state;
    
    x = (int) evt->x;
    y = (int) evt->y;
    state = (GdkModifierType) evt->state;
    
    TRACE_INFO_INT_INT("x/y",x,y);
    if ( (state & GDK_BUTTON1_MASK) != 0 )
    {
        TRACE_INFO_INT("    btn",state);
    }

    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_button_press_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();

    if ( evt->button == 1 ) {
        TRACE_INFO("press");
    }

    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();

    if ( evt->button == 1 ) {
        TRACE_INFO("release");
    }

    TRACE_END();
    return TRUE;
}


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

