/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "gui_diagram_painter.h"
#include "trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>

void gui_sketch_area_init( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();
    
    data_database_init( &((*this_).database) );
    
    TRACE_END();
}

void gui_sketch_area_destroy( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();
    
    data_database_destroy( &((*this_).database) );
    
    TRACE_END();
}

static const gint RATIO_WIDTH = 36;
static const gint RATIO_HEIGHT = 24;

gboolean gui_sketch_area_draw_callback( GtkWidget *widget, cairo_t *cr, gpointer data )
{
    TRACE_BEGIN();
    guint width, height;
    GdkRGBA color;
    gui_sketch_area_t *this_ = data;
    gint paper_left, paper_top, paper_width, paper_height;

    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    
    if (( width < 48 )||( height < 48 )) {
        /* window is too small, output a dark-grey rectangle */
	cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, height );
        cairo_fill (cr);
    }
    else 
    {
        if ( width * RATIO_HEIGHT > height * RATIO_WIDTH )
	{
	    paper_top = 10;
	    paper_height = height - 20;
	    paper_width = ( height * RATIO_WIDTH ) / RATIO_HEIGHT;
	    paper_left = ( width - paper_width ) / 2;
	}
        else
	{
	    paper_left = 10;
	    paper_width = width - 20;
	    paper_height = ( width * RATIO_HEIGHT ) / RATIO_WIDTH;
	    paper_top = ( height - paper_height ) / 2;
	}
      
        /* draw border */
	cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, paper_top );
        cairo_fill (cr);
        cairo_rectangle ( cr, 0, paper_top+paper_height, width, height-paper_top-paper_height );
        cairo_fill (cr);
        cairo_rectangle ( cr, 0, paper_top, paper_left, paper_height );
        cairo_fill (cr);
        cairo_rectangle ( cr, paper_left+paper_width, paper_top, width-paper_left-paper_width, paper_height );
        cairo_fill (cr);
      
	/* draw paper */
	cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
        cairo_rectangle ( cr, paper_left, paper_top, paper_width, paper_height );
        cairo_fill (cr);
	
	/* draw the current diagram */
	cairo_save (cr);
        cairo_rectangle ( cr, paper_left, paper_top, paper_width, paper_height );
	cairo_clip (cr);
	gui_diagram_painter_draw ( &((*this_).database), 0, cr );
	/*cairo_reset_clip (cr);*/
	cairo_restore (cr);
    }
    TRACE_END();
    return FALSE;
}


gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {
    }

    TRACE_END();
    return TRUE;
}


gboolean gui_sketch_area_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

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
    gui_sketch_area_t *this_ = data;

    if ( evt->button == 1 ) {
        TRACE_INFO("press");
    }

    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

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

