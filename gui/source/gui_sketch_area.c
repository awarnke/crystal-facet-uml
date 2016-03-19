/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "pencil_diagram_painter.h"
#include "util/geometry/geometry_rectangle.h"
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
    data_database_reader_init( &((*this_).db_reader), database );
    gui_sketch_card_init( &((*this_).card) );

    /* load data to be drawn */
    gui_sketch_card_load_data( &((*this_).card), 1, &((*this_).db_reader) );

    /* just a test */
    data_error_t db_err;
    data_diagram_t diags[7];
    int32_t count;
    db_err = data_database_reader_get_diagrams_by_parent_id ( &((*this_).db_reader), /*parent_id*/ 0 , 7, &count, &diags );

    TRACE_END();
}

void gui_sketch_area_destroy( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    gui_sketch_card_destroy( &((*this_).card) );
    data_database_reader_destroy( &((*this_).db_reader) );

    TRACE_END();
}

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

	cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, height );
        cairo_fill (cr);
    }
    else
    {
        shape_int_rectangle_t bounds;
        shape_int_rectangle_init( &bounds, 0.0d, 0.0d, (double) width, (double) height );

        gui_sketch_tools_tool_t selected_tool;
        selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
        switch ( selected_tool )
        {
            case GUI_SKETCH_TOOLS_NAVIGATE:
                gui_sketch_area_private_draw_navigation_table( this_, bounds, cr );
                break;
            case GUI_SKETCH_TOOLS_EDIT:
                gui_sketch_area_private_draw_single_diagram( this_, bounds, cr );
                break;
            case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
                gui_sketch_area_private_draw_single_diagram( this_, bounds, cr );
                break;
            case GUI_SKETCH_TOOLS_CREATE_OBJECT:
                gui_sketch_area_private_draw_single_diagram( this_, bounds, cr );
                break;
            default:
                LOG_ERROR("selected_tool is out of range");
                break;
        }

    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return FALSE;
}

enum gui_sketch_area_layout_enum {
    GUI_SKETCH_AREA_LAYOUT_VERTICAL = 0,
    GUI_SKETCH_AREA_LAYOUT_HORIZONTAL = 1,
};
typedef enum gui_sketch_area_layout_enum gui_sketch_area_layout_t;

static const gint RATIO_WIDTH = 36;
static const gint RATIO_HEIGHT = 24;

void gui_sketch_area_private_draw_navigation_table ( gui_sketch_area_t *this_, shape_int_rectangle_t bounds, cairo_t *cr )
{
    TRACE_BEGIN();
    shape_int_rectangle_t parent_bounds;
    shape_int_rectangle_t self_bounds;
    shape_int_rectangle_t children_bounds;
    gui_sketch_area_layout_t layout_type;

    int32_t width = shape_int_rectangle_get_width( &bounds );
    int32_t height = shape_int_rectangle_get_height( &bounds );
    int32_t left = shape_int_rectangle_get_left( &bounds );
    int32_t top = shape_int_rectangle_get_top( &bounds );
    layout_type = ( width > height ) ? GUI_SKETCH_AREA_LAYOUT_HORIZONTAL : GUI_SKETCH_AREA_LAYOUT_VERTICAL;

    if ( GUI_SKETCH_AREA_LAYOUT_HORIZONTAL == layout_type )
    {
        int32_t max_top_heigth = ( height * 2 ) / 3;
        int32_t preferred_top_height = ( width * RATIO_HEIGHT ) / ( (RATIO_WIDTH*2)/3 + RATIO_WIDTH );
        if ( preferred_top_height > max_top_heigth )
        {
            preferred_top_height = max_top_heigth;
        }
        shape_int_rectangle_init(
            &parent_bounds,
            left,
            top,
            (width*2)/5,
            preferred_top_height
        );
        shape_int_rectangle_init(
            &self_bounds,
            left+(width*2)/5,
            top,
            (width*3)/5,
            preferred_top_height
        );
        shape_int_rectangle_init(
            &children_bounds,
            left,
            top+preferred_top_height,
            width,
            height-preferred_top_height
        );

        cairo_set_source_rgba( cr, 0.7, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, shape_int_rectangle_get_left( &parent_bounds ), shape_int_rectangle_get_top( &parent_bounds ), shape_int_rectangle_get_width( &parent_bounds ), shape_int_rectangle_get_height( &parent_bounds ) );
        cairo_fill (cr);
    }
    else
    {
    }

    bounds.top += bounds.height/3;
    bounds.height = bounds.height/3;
    gui_sketch_area_private_draw_single_diagram( this_, bounds, cr );

    TRACE_END();
}

void gui_sketch_area_private_draw_single_diagram ( gui_sketch_area_t *this_, shape_int_rectangle_t bounds, cairo_t *cr )
{
    TRACE_BEGIN();
    int32_t border = 10.0d;
    int32_t width = shape_int_rectangle_get_width( &bounds );
    int32_t height = shape_int_rectangle_get_height( &bounds );

    int32_t paper_top;
    int32_t paper_height;
    int32_t paper_width;
    int32_t paper_left;

    if ( (width-2*border) * RATIO_HEIGHT > (height-2*border) * RATIO_WIDTH )
    {
        paper_top = border;
        paper_height = height - 2*border;
        paper_width = ( (height-2*border) * RATIO_WIDTH ) / RATIO_HEIGHT;
        paper_left = ( width - paper_width ) / 2;
    }
    else
    {
        paper_left = border;
        paper_width = width - 2*border;
        paper_height = ( (width-2*border) * RATIO_HEIGHT ) / RATIO_WIDTH;
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

    shape_int_rectangle_t card_bounds;
    shape_int_rectangle_init( &card_bounds, paper_left, paper_top, paper_width, paper_height );
    gui_sketch_card_set_bounds( &((*this_).card), card_bounds );
    gui_sketch_card_draw( &((*this_).card), cr );

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

    TRACE_END();
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
    static const double BORDER = 1.0;
    gtk_widget_queue_draw_area( widget, left-BORDER, top-BORDER, right-left+BORDER+BORDER, bottom-top+BORDER+BORDER );
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

void gui_sketch_area_data_changed_callback( GtkWidget *widget, void *unused, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;
    guint width;
    guint height;

    /* load/reload data to be drawn */
    gui_sketch_card_load_data( &((*this_).card), 1, &((*this_).db_reader) );

    /* mark dirty rect */
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    gtk_widget_queue_draw_area( widget, 0, 0, width, height );

    TRACE_END();
}

void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_sketch_tools_tool_t tool, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;
    guint width;
    guint height;

    /* mark dirty rect */
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    gtk_widget_queue_draw_area( widget, 0, 0, width, height );

    TRACE_END();
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

