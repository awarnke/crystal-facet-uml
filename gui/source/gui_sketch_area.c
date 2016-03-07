/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "gui_diagram_painter.h"
#include "trace.h"
#include "log.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdint.h>

void gui_sketch_area_init( gui_sketch_area_t *this_, gui_sketch_tools_t *tools, ctrl_controller_t *controller, data_database_t *database )
{
    TRACE_BEGIN();

    (*this_).mark_active = false;
    (*this_).tools = tools;
    (*this_).database = database;
    (*this_).controller = controller;
    (*this_).paper_visible = false;
    data_database_reader_init( &((*this_).db_reader), database );
    gui_diagram_painter_init( &((*this_).painter) );

    data_diagram_t my_diag;
    data_error_t err;
    err= data_database_reader_get_diagram_by_id ( &((*this_).db_reader), /*id*/ 5 , &my_diag );

    data_diagram_t diags[7];
    int32_t count;
    err = data_database_reader_get_diagrams_by_parent_id ( &((*this_).db_reader), /*parent_id*/ 0 , 7, &count, &diags );

    TRACE_END();
}

void gui_sketch_area_destroy( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    gui_diagram_painter_destroy( &((*this_).painter) );
    data_database_reader_destroy( &((*this_).db_reader) );

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

    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);

    if (( width < 48 )||( height < 48 )) {
        /* window is too small, output a dark-grey rectangle */

        (*this_).paper_visible = false;

	cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, height );
        cairo_fill (cr);
    }
    else
    {
        (*this_).paper_visible = true;

        if ( width * RATIO_HEIGHT > height * RATIO_WIDTH )
	{
            (*this_).paper_top = 10;
            (*this_).paper_height = height - 20;
            (*this_).paper_width = ( height * RATIO_WIDTH ) / RATIO_HEIGHT;
            (*this_).paper_left = ( width - (*this_).paper_width ) / 2;
	}
        else
	{
            (*this_).paper_left = 10;
            (*this_).paper_width = width - 20;
            (*this_).paper_height = ( width * RATIO_HEIGHT ) / RATIO_WIDTH;
            (*this_).paper_top = ( height - (*this_).paper_height ) / 2;
	}

        /* draw border */
	cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, (*this_).paper_top );
        cairo_fill (cr);
        cairo_rectangle ( cr, 0, (*this_).paper_top+(*this_).paper_height, width, height-(*this_).paper_top-(*this_).paper_height );
        cairo_fill (cr);
        cairo_rectangle ( cr, 0, (*this_).paper_top, (*this_).paper_left, (*this_).paper_height );
        cairo_fill (cr);
        cairo_rectangle ( cr, (*this_).paper_left+(*this_).paper_width, (*this_).paper_top, width-(*this_).paper_left-(*this_).paper_width, (*this_).paper_height );
        cairo_fill (cr);

        /* draw paper */
	cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
        cairo_rectangle ( cr, (*this_).paper_left, (*this_).paper_top, (*this_).paper_width, (*this_).paper_height );
        cairo_fill (cr);

	/* draw the current diagram */
	cairo_save (cr);
        cairo_rectangle ( cr, (*this_).paper_left, (*this_).paper_top, (*this_).paper_width, (*this_).paper_height );
	cairo_clip (cr);
        gui_diagram_painter_draw ( &((*this_).painter), (*this_).database, 0, cr );
	/*cairo_reset_clip (cr);*/
	cairo_restore (cr);

        /* draw marking line */
        if ( (*this_).mark_active )
        {
            gint left, right, top, bottom;
            if ( (*this_).mark_start_x < (*this_).mark_end_x )
            {
                left = (*this_).mark_start_x;
                right = (*this_).mark_end_x;
            }
            else
            {
                left = (*this_).mark_end_x;
                right = (*this_).mark_start_x;
            }
            if ( (*this_).mark_start_y < (*this_).mark_end_y )
            {
                top = (*this_).mark_start_y;
                bottom = (*this_).mark_end_y;
            }
            else
            {
                top = (*this_).mark_end_y;
                bottom = (*this_).mark_start_y;
            }
            cairo_set_source_rgba( cr, 1.0, 1.0, 0.0, 0.5 );
            cairo_rectangle ( cr, left, top, right-left, bottom-top );
            cairo_fill (cr);
        }
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return FALSE;
}

static inline void gui_sketch_area_queue_draw_mark_area( GtkWidget* widget, gui_sketch_area_t *this_ );
static inline void gui_sketch_area_queue_draw_mark_area( GtkWidget* widget, gui_sketch_area_t *this_ )
{
    gint left, right, top, bottom;
    if ( (*this_).mark_start_x < (*this_).mark_end_x )
    {
        left = (*this_).mark_start_x;
        right = (*this_).mark_end_x;
    }
    else
    {
        left = (*this_).mark_end_x;
        right = (*this_).mark_start_x;
    }
    if ( (*this_).mark_start_y < (*this_).mark_end_y )
    {
        top = (*this_).mark_start_y;
        bottom = (*this_).mark_end_y;
    }
    else
    {
        top = (*this_).mark_end_y;
        bottom = (*this_).mark_start_y;
    }

    /* mark dirty rect */
    gtk_widget_queue_draw_area( widget, left, top, right-left, bottom-top );
}

gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return TRUE;
}


gboolean gui_sketch_area_mouse_motion_callback( GtkWidget* widget, GdkEventMotion* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    int32_t x;
    int32_t y;
    GdkModifierType state;

    x = (int32_t) evt->x;
    y = (int32_t) evt->y;
    state = (GdkModifierType) evt->state;

    TRACE_INFO_INT_INT("x/y",x,y);

    if ( (*this_).mark_active )
    {
        /* mark dirty rect */
        gui_sketch_area_queue_draw_mark_area( widget, this_ );

        (*this_).mark_end_x = x;
        (*this_).mark_end_y = y;

        /* mark dirty rect */
        gui_sketch_area_queue_draw_mark_area( widget, this_ );
    }

    if ( (state & GDK_BUTTON1_MASK) != 0 )
    {
        TRACE_INFO("GDK_BUTTON1_MASK");
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

        int32_t x;
        int32_t y;
        x = (int32_t) evt->x;
        y = (int32_t) evt->y;
        TRACE_INFO_INT_INT("x/y",x,y);

        (*this_).mark_active = true;
        (*this_).mark_start_x = x;
        (*this_).mark_start_y = y;
        (*this_).mark_end_x = x;
        (*this_).mark_end_y = y;

        /* mark dirty rect */
        gui_sketch_area_queue_draw_mark_area( widget, this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    if ( evt->button == 1 ) {
        TRACE_INFO("release");

        int32_t x;
        int32_t y;
        x = (int32_t) evt->x;
        y = (int32_t) evt->y;
        TRACE_INFO_INT_INT("x/y",x,y);

        (*this_).mark_active = false;

        /* mark dirty rect */
        gui_sketch_area_queue_draw_mark_area( widget, this_ );

        /* do action */
        gui_sketch_tools_tool_t selected_tool;
        selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
        switch ( selected_tool )
        {
            case GUI_SKETCH_TOOLS_NAVIGATE:
                TRACE_INFO("GUI_SKETCH_TOOLS_NAVIGATE");
                break;
            case GUI_SKETCH_TOOLS_EDIT:
                TRACE_INFO("GUI_SKETCH_TOOLS_EDIT");
                break;
            case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
                {
                    TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_DIAGRAM");

                    ctrl_diagram_controller_t *diag_control;
                    diag_control = ctrl_controller_get_diagram_control ( (*this_).controller );

                    int64_t new_diag_id;
                    ctrl_error_t c_result;
                    c_result = ctrl_diagram_controller_create_diagram ( diag_control, /*parent_diagram_id*/ 0, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, "Hello World.", &new_diag_id );

                }
                break;
            case GUI_SKETCH_TOOLS_CREATE_OBJECT:
                TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_OBJECT");
                break;
            default:
                LOG_ERROR("selected_tool is out of range");
                break;
        }
    }

    TRACE_TIMESTAMP();
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

