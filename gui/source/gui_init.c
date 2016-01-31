/* File: gui/gui_init.c; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "gui_init.h"
#include "gui_textedit.h"
#include "gui_sketcharea.h"
#include "util/trace.h"
#include "gui_shutdown.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

static GtkWidget *window;
static GtkWidget *sketcharea;
static GtkWidget *layout;

static GtkWidget *toolbar;
static GtkToolItem *tool_navigate;
static GtkToolItem *tool_edit;
static GtkToolItem *tool_new_obj;
static GtkToolItem *tool_new_view;

static GtkWidget *clear;

static GtkWidget *characterEntry;

static gui_textedit_t gui_textedit = GUI_TEXTEDIT( gui_textedit, NULL, NULL );

void gui_init () {
    TRACE_BEGIN();
 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "s t o n e u m l");
    gtk_widget_set_size_request(window, 1080, 660);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
    
    tool_navigate = gtk_radio_tool_button_new( NULL );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON(tool_navigate), "Navigate");
    GSList *group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON (tool_navigate));
    
    tool_edit = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON(tool_edit), "Edit");
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON (tool_edit));
    
    tool_new_obj = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON(tool_new_obj), "New Object");
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON (tool_new_obj));
    
    tool_new_view = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON(tool_new_view), "New View");
    
    toolbar = gtk_toolbar_new ();

    sketcharea = gtk_drawing_area_new();
    gtk_widget_set_size_request(sketcharea, 1080, 640);
    gtk_widget_set_events( sketcharea, GDK_LEAVE_NOTIFY_MASK 
        | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
    gtk_widget_set_hexpand (sketcharea, TRUE);
    gtk_widget_set_vexpand (sketcharea, TRUE);
 
    layout = gtk_grid_new();
    gtk_widget_set_size_request(layout, 1080, 640);

    clear = gtk_button_new_with_label( "clear" );
    characterEntry = gtk_entry_new();

    TRACE_INFO("GTK+ Widgets are created.");
    
    gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),tool_navigate,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),tool_edit,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),tool_new_obj,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),tool_new_view,-1);
    gtk_grid_attach( GTK_GRID(layout), toolbar, 0, 0, 3, 1 );
    gtk_grid_attach( GTK_GRID(layout), sketcharea, 0, 1, 3, 1 );
    gtk_grid_attach( GTK_GRID(layout), clear, 0, 2, 1, 1 );
    gtk_grid_attach( GTK_GRID(layout), characterEntry, 1, 2, 2, 1 );
    gtk_container_add(GTK_CONTAINER(window), layout);
   
    TRACE_INFO("GTK+ Widgets are added to containers.");
    
    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gui_window_delete_event), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gui_window_destroy_event), NULL);
    //g_signal_connect(G_OBJECT(sketcharea), "expose_event", G_CALLBACK(gui_sketcharea_expose_event_callback), sketcharea );
    g_signal_connect (G_OBJECT (sketcharea), "draw", G_CALLBACK (gui_sketcharea_draw_callback), NULL);    
    g_signal_connect( G_OBJECT(sketcharea), "motion_notify_event", G_CALLBACK(gui_sketcharea_mouse_motion_callback), &gui_textedit );
    g_signal_connect( G_OBJECT(sketcharea), "button_press_event", G_CALLBACK(gui_sketcharea_button_press_callback), &gui_textedit );
    g_signal_connect( G_OBJECT(sketcharea), "button_release_event", G_CALLBACK(gui_sketcharea_button_release_callback), &gui_textedit );
    g_signal_connect( G_OBJECT(sketcharea), "leave_notify_event", G_CALLBACK(gui_sketcharea_leave_notify_callback), &gui_textedit );
    g_signal_connect(G_OBJECT(clear), "clicked", G_CALLBACK(btn_clear_callback), &gui_textedit );

    TRACE_INFO("GTK+ Callbacks are connected to widget events.");

    gtk_widget_show_all(window);

    TRACE_INFO("GTK+ Widgets are shown.");
    TRACE_END();
}


