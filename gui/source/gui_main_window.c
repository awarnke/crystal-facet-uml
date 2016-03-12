/* File: gui_main_window.c; Copyright and License: see below */

#include "gui_main_window.h"
#include "gui_textedit.h"
#include "gui_sketch_area.h"
#include "trace.h"
#include "data_change_notifier.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_main_window_init ( gui_main_window_t *this_, ctrl_controller_t *controller, data_database_t *database )
{
    TRACE_BEGIN();

    (*this_).window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW((*this_).window), "crystal facet uml");
    gtk_widget_set_size_request((*this_).window, 1080, 660);

    (*this_).tool_navigate = gtk_radio_tool_button_new( NULL );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_navigate), "Navigate");
    GSList *group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_navigate));

    (*this_).tool_edit = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_edit), "Edit");
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_edit));

    (*this_).tool_new_obj = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_new_obj), "New Object");
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_new_obj));

    (*this_).tool_new_view = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_new_view), "New Diagram");

    (*this_).toolbar = gtk_toolbar_new ();
    gui_sketch_tools_init( &((*this_).sketchtools_data) );

    (*this_).sketcharea = gtk_drawing_area_new();
    gtk_widget_set_size_request((*this_).sketcharea, 1080, 600);
    gtk_widget_set_events( (*this_).sketcharea, GDK_LEAVE_NOTIFY_MASK
        | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
    gtk_widget_set_hexpand ( (*this_).sketcharea, TRUE);
    gtk_widget_set_vexpand ( (*this_).sketcharea, TRUE);
    gui_sketch_area_init( &((*this_).sketcharea_data), &((*this_).sketchtools_data), controller, database );

    (*this_).layout = gtk_grid_new();
    gtk_widget_set_size_request((*this_).layout, 1080, 640);

    (*this_).clear = gtk_button_new_with_label( "clear" );
    (*this_).characterEntry = gtk_entry_new();

    TRACE_INFO("GTK+ Widgets are created.");

    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_navigate,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_edit,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_new_obj,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_new_view,-1);
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).toolbar, 0, 0, 3, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).sketcharea, 0, 1, 3, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).clear, 0, 2, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).characterEntry, 1, 2, 2, 1 );
    gtk_container_add(GTK_CONTAINER((*this_).window), (*this_).layout);

    TRACE_INFO("GTK+ Widgets are added to containers.");

    g_signal_connect( G_OBJECT((*this_).window), "delete_event", G_CALLBACK(gui_main_window_delete_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), "destroy", G_CALLBACK(gui_main_window_destroy_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "draw", G_CALLBACK (gui_sketch_area_draw_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "motion_notify_event", G_CALLBACK(gui_sketch_area_mouse_motion_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "button_press_event", G_CALLBACK(gui_sketch_area_button_press_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "button_release_event", G_CALLBACK(gui_sketch_area_button_release_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "leave_notify_event", G_CALLBACK(gui_sketch_area_leave_notify_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_data_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).clear), "clicked", G_CALLBACK(gui_textedit_clear_btn_callback), NULL );
    g_signal_connect( G_OBJECT((*this_).tool_navigate), "clicked", G_CALLBACK(gui_sketch_tools_navigate_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_edit), "clicked", G_CALLBACK(gui_sketch_tools_edit_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_new_obj), "clicked", G_CALLBACK(gui_sketch_tools_create_object_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_new_view), "clicked", G_CALLBACK(gui_sketch_tools_create_diagram_btn_callback), &((*this_).sketchtools_data) );

    TRACE_INFO("GTK+ Callbacks are connected to widget events.");

    data_change_notifier_add_listener( data_database_get_notifier_ptr( database ), G_OBJECT((*this_).sketcharea) );

    TRACE_INFO("GTK+ Widgets are registered as listeners at data module.");

    gtk_widget_show_all((*this_).window);

    TRACE_INFO("GTK+ Widgets are shown.");
    TRACE_END();
}

void gui_main_window_destroy( gui_main_window_t *this_ )
{
    TRACE_BEGIN();

    gui_sketch_area_destroy( &((*this_).sketcharea_data) );
    gui_sketch_tools_destroy( &((*this_).sketchtools_data) );

    TRACE_END();
}

void gui_main_window_destroy_event_callback(GtkWidget *widget, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_main_window_destroy( this_ );
    gtk_main_quit();

    TRACE_TIMESTAMP();
    TRACE_END();
}

gboolean gui_main_window_delete_event_callback(GtkWidget *widget, GdkEvent *event, gpointer data )
{
    TRACE_BEGIN();

    TRACE_TIMESTAMP();
    TRACE_END();
    return FALSE;  /* return false to trigger destroy event */
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
