/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "pencil_diagram_painter.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_table.h"
#include "trace.h"
#include "log.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdint.h>
#include <stdbool.h>

static bool gui_sketch_area_glib_signal_initialized = false;
static guint gui_sketch_area_glib_signal_id = 0;
const char *GUI_SKETCH_AREA_GLIB_SIGNAL_NAME = "cfu_object_selected";

void gui_sketch_area_private_notify_listener( gui_sketch_area_t *this_, data_table_t table, int64_t id );

void gui_sketch_area_init( gui_sketch_area_t *this_, gui_sketch_tools_t *tools, ctrl_controller_t *controller, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    (*this_).mark_active = false;
    (*this_).tools = tools;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).card_num = 0;
    (*this_).listener = NULL;

    gui_sketch_area_private_load_cards( this_, DATA_DIAGRAM_ID_UNINITIALIZED_ID );

    /* define a new signal */
    if ( ! gui_sketch_area_glib_signal_initialized )
    {
        gui_sketch_area_glib_signal_id = g_signal_new (
            GUI_SKETCH_AREA_GLIB_SIGNAL_NAME,
            G_TYPE_OBJECT,
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__POINTER,
            G_TYPE_NONE,
            1/*2*/,
            G_TYPE_INT, /* data_table_t */
            G_TYPE_INT64 /* id of the object */
        );
        gui_sketch_area_glib_signal_initialized = true;
        TRACE_INFO_INT( "g_signal_new(\"cfu_object_selected\") returned new signal id", gui_sketch_area_glib_signal_id );
    }

    TRACE_END();
}

void gui_sketch_area_destroy( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    /* destroy all cards */
    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_destroy( &((*this_).cards[idx]) );
    }
    (*this_).card_num = 0;

    TRACE_END();
}

gboolean gui_sketch_area_draw_callback( GtkWidget *widget, cairo_t *cr, gpointer data )
{
    TRACE_BEGIN();
    guint width;
    guint height;
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
        shape_int_rectangle_init( &bounds, 0, 0, width, height );
        gui_sketch_area_private_layout_cards( this_, bounds );
        gui_sketch_area_private_draw_cards( this_, bounds, cr );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return FALSE;
}

void gui_sketch_area_private_load_cards ( gui_sketch_area_t *this_, int64_t main_diagram_id )
{
    TRACE_BEGIN();
    data_error_t db_err;

    /* destroy all old cards */
    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_destroy( &((*this_).cards[idx]) );
    }
    (*this_).card_num = 0;

    /* determine diagram id of root diagram */
    if ( DATA_DIAGRAM_ID_UNINITIALIZED_ID == main_diagram_id )
    {
        /* load all without parent */
        int32_t count;
        db_err = data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                                 DATA_DIAGRAM_ID_UNINITIALIZED_ID,
                                                                 GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS,
                                                                 &count,
                                                                 &((*this_).private_temp_diagram_buf) );
        if ( DATA_ERROR_NONE != db_err )
        {
            LOG_ERROR_HEX( "data_database_reader_get_diagrams_by_parent_id failed.", db_err );
        }
        else if ( count > 1 )
        {
            LOG_ERROR_INT( "more than one root diagram exists!", count );
        }
        else if ( count < 1 )
        {
            LOG_WARNING( "no root diagram exists!" );
        }
        else
        {
            main_diagram_id = data_diagram_get_id( &((*this_).private_temp_diagram_buf[0]) );
            TRACE_INFO_INT( "main_diagram_id:", main_diagram_id );
        }

        /* cleanup */
        if ( DATA_ERROR_NONE == db_err )
        {
            for ( int index = 0; index < count; index ++ )
            {
                data_diagram_destroy( &((*this_).private_temp_diagram_buf[index]) );
            }
        }
    }

    /* load data to be drawn */
    gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_SELECTED_CARD]) );
    gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_SELECTED_CARD]), main_diagram_id, (*this_).db_reader );
    (*this_).card_num = 1;

    gui_sketch_tools_tool_t selected_tool;
    selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
    if ( GUI_SKETCH_TOOLS_NAVIGATE == selected_tool )
    {
        /* determine ids */
        int64_t selected_diagram_id;
        int64_t parent_diagram_id;
        data_diagram_t *selected_diag;
        selected_diag = gui_sketch_area_get_selected_diagram_ptr( this_ );
        selected_diagram_id = data_diagram_get_id( selected_diag );
        TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );
        parent_diagram_id = data_diagram_get_parent_id( selected_diag );
        TRACE_INFO_INT( "parent_diagram_id:", parent_diagram_id );

        /* load parent if there is any */
        gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]) );
        gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]), parent_diagram_id, (*this_).db_reader );
        (*this_).card_num = 2;

        /* load all children */
        int32_t c_count;
        db_err = data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                                 selected_diagram_id,
                                                                 GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS,
                                                                 &c_count,
                                                                 &((*this_).private_temp_diagram_buf) );
        if ( DATA_ERROR_NONE != db_err )
        {
            LOG_ERROR_HEX( "data_database_reader_get_diagrams_by_parent_id failed.", db_err );
        }
        else
        {
            for ( int index = 0; index < c_count; index ++ )
            {
                int64_t current_child_id;
                current_child_id = data_diagram_get_id( &((*this_).private_temp_diagram_buf[index]) );
                gui_sketch_card_init( &((*this_).cards[(*this_).card_num]) );
                gui_sketch_card_load_data( &((*this_).cards[(*this_).card_num]), current_child_id, (*this_).db_reader );
                (*this_).card_num ++;
                /* cleanup */
                data_diagram_destroy( &((*this_).private_temp_diagram_buf[index]) );
            }
        }
    }

    TRACE_END();
}

void gui_sketch_area_private_reload_cards ( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    /* determine currently selected id */
    int64_t selected_diagram_id;
    selected_diagram_id = gui_sketch_area_get_selected_diagram_id( this_ );
    TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

    /* reload diagram data */
    gui_sketch_area_private_load_cards( this_, selected_diagram_id );

    TRACE_END();
}

enum gui_sketch_area_layout_enum {
    GUI_SKETCH_AREA_LAYOUT_VERTICAL = 0,
    GUI_SKETCH_AREA_LAYOUT_HORIZONTAL = 1,
};
typedef enum gui_sketch_area_layout_enum gui_sketch_area_layout_t;

static const gint RATIO_WIDTH = 36;
static const gint RATIO_HEIGHT = 24;
static const gint BORDER = 10;
static const gint HALF_BORDER = 5;

void gui_sketch_area_private_layout_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds )
{
    TRACE_BEGIN();

    gui_sketch_tools_tool_t selected_tool;
    selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );

    /* pre-calculate numbers needed in case of GUI_SKETCH_TOOLS_NAVIGATE */
    uint32_t width = shape_int_rectangle_get_width( &area_bounds );
    uint32_t height = shape_int_rectangle_get_height( &area_bounds );
    int32_t left = shape_int_rectangle_get_left( &area_bounds );
    int32_t top = shape_int_rectangle_get_top( &area_bounds );
    gui_sketch_area_layout_t layout_type;
    layout_type = ( width > height ) ? GUI_SKETCH_AREA_LAYOUT_HORIZONTAL : GUI_SKETCH_AREA_LAYOUT_VERTICAL;
    int32_t children_top;
    uint32_t children_height;
    uint32_t parent_width;
    uint32_t parent_height;
    uint32_t self_width;
    uint32_t self_height;
    int32_t self_left;
    int32_t self_top;
    if ( GUI_SKETCH_AREA_LAYOUT_HORIZONTAL == layout_type )
    {
        uint32_t max_top_heigth = ( height * 2 ) / 3;
        uint32_t preferred_top_height = ( width * RATIO_HEIGHT ) / ( (RATIO_WIDTH*2)/3 + RATIO_WIDTH );
        if ( preferred_top_height > max_top_heigth )
        {
            preferred_top_height = max_top_heigth;
        }
        children_top = top + preferred_top_height;
        children_height = height - preferred_top_height;
        parent_width = ( width * 4 ) / 10;
        parent_height = preferred_top_height;
        self_width = width - parent_width;
        self_height = preferred_top_height;
        self_left = left + parent_width;
        self_top = top;
    }
    else
    {
        parent_width = width;
        parent_height = ( height * 2 ) / 10;
        self_width = width;
        self_left = left;
        self_top = top + parent_height;
        self_height = ( height * 4 ) / 10;
        children_top = self_top + self_height;
        children_height = height - self_height - parent_height;
    }

    for ( int card_idx = 0; card_idx < (*this_).card_num; card_idx ++ )
    {
        if ( ! gui_sketch_card_is_valid( &((*this_).cards[card_idx]) ))
        {
            gui_sketch_card_set_visible( &((*this_).cards[card_idx]), false );
        }
        else /* ==gui_sketch_card_is_valid */ if ( GUI_SKETCH_TOOLS_NAVIGATE == selected_tool )
        {
            shape_int_rectangle_t card_bounds;

            if ( card_idx == 0 )  /* self */
            {
                shape_int_rectangle_init( &card_bounds, self_left, self_top, self_width, self_height );
                shape_int_rectangle_shrink_by_border( &card_bounds, HALF_BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_ALIGNMENT_VERTICAL_MIDDLE | SHAPE_ALIGNMENT_HORIZONTAL_CENTER );
            }
            else if ( card_idx == 1 )  /* parent */
            {
                shape_int_rectangle_init( &card_bounds, left, top, parent_width, parent_height );
                shape_int_rectangle_shrink_by_border( &card_bounds, HALF_BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_ALIGNMENT_VERTICAL_TOP | SHAPE_ALIGNMENT_HORIZONTAL_LEFT );
            }
            else
            {
                int current_child = card_idx-2;
                int max_children = (*this_).card_num-2;
                shape_int_rectangle_init( &card_bounds, left+(width*current_child)/max_children, children_top, width/max_children, children_height );
                shape_int_rectangle_shrink_by_border( &card_bounds, HALF_BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_ALIGNMENT_VERTICAL_MIDDLE | SHAPE_ALIGNMENT_HORIZONTAL_CENTER );
            }

            gui_sketch_card_set_bounds( &((*this_).cards[card_idx]), card_bounds );
            gui_sketch_card_set_visible( &((*this_).cards[card_idx]), true );
        }
        else /* ==gui_sketch_card_is_valid and not GUI_SKETCH_TOOLS_NAVIGATE */
        {
            if ( card_idx == 0 )
            {
                shape_int_rectangle_t card_bounds;
                card_bounds = area_bounds;
                shape_int_rectangle_shrink_by_border( &card_bounds, BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_ALIGNMENT_VERTICAL_MIDDLE | SHAPE_ALIGNMENT_HORIZONTAL_CENTER );
                gui_sketch_card_set_bounds( &((*this_).cards[card_idx]), card_bounds );
                gui_sketch_card_set_visible( &((*this_).cards[card_idx]), true );
            }
            else
            {
                gui_sketch_card_set_visible( &((*this_).cards[card_idx]), false );
            }
        }
    }

    TRACE_END();
}

void gui_sketch_area_private_draw_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr )
{
    TRACE_BEGIN();
    int32_t width = shape_int_rectangle_get_width( &area_bounds );
    int32_t height = shape_int_rectangle_get_height( &area_bounds );

    /* draw background */
    cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
    cairo_rectangle ( cr, 0, 0, width, height );
    cairo_fill (cr);

    /* draw all cards */
    for ( int card_idx = 0; card_idx < (*this_).card_num; card_idx ++ )
    {
        gui_sketch_card_draw( &((*this_).cards[card_idx]), cr );
    }

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

        /* do action */
        gui_sketch_tools_tool_t selected_tool;
        selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
        switch ( selected_tool )
        {
            case GUI_SKETCH_TOOLS_NAVIGATE:
                {
                    TRACE_INFO("GUI_SKETCH_TOOLS_NAVIGATE");

                    /* search selected diagram */
                    int64_t clicked_diagram_id = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
                    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
                    {
                        gui_sketch_card_t *card;
                        card = &((*this_).cards[idx]);
                        shape_int_rectangle_t card_bounds;
                        card_bounds = gui_sketch_card_get_bounds( card );
                        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
                        {
                            data_diagram_t *selected_diag;
                            selected_diag = gui_sketch_card_get_diagram_ptr( card );
                            clicked_diagram_id = data_diagram_get_id( selected_diag );
                            TRACE_INFO_INT( "clicked_diagram_id:", clicked_diagram_id );
                        }
                    }

                    /* load diagram */
                    if ( DATA_DIAGRAM_ID_UNINITIALIZED_ID != clicked_diagram_id )
                    {
                        /* load/reload data to be drawn */
                        gui_sketch_area_private_load_cards( this_, clicked_diagram_id );

                        /* notify listener */
                        gui_sketch_area_private_notify_listener( this_, DATA_TABLE_DIAGRAM, clicked_diagram_id );

                        /* mark dirty rect */
                        guint width;
                        guint height;
                        width = gtk_widget_get_allocated_width (widget);
                        height = gtk_widget_get_allocated_height (widget);
                        gtk_widget_queue_draw_area( widget, 0, 0, width, height );
                    }
                }
                break;
            case GUI_SKETCH_TOOLS_EDIT:
                TRACE_INFO("GUI_SKETCH_TOOLS_EDIT");
                break;
            case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
                TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_DIAGRAM");
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

                    int64_t selected_diagram_id;
                    selected_diagram_id = gui_sketch_area_get_selected_diagram_id( this_ );
                    TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

                    ctrl_diagram_controller_t *diag_control;
                    diag_control = ctrl_controller_get_diagram_control ( (*this_).controller );

                    char* new_name;
                    static char *(NAMES[8]) = {"Upper Layer","Overview","Power States","Startup Sequence","Shutdown states","Boot timings","Lower Layer","Hello World"};
                    new_name = NAMES[(x+y)&0x07];

                    int64_t new_diag_id;
                    ctrl_error_t c_result;
                    c_result = ctrl_diagram_controller_create_diagram ( diag_control, selected_diagram_id, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, new_name, &new_diag_id );

                    /* load/reload data to be drawn */
                    gui_sketch_area_private_load_cards( this_, new_diag_id );

                    /* notify listener */
                    gui_sketch_area_private_notify_listener( this_, DATA_TABLE_DIAGRAM, new_diag_id );

                    /* mark dirty rect */
                    guint width;
                    guint height;
                    width = gtk_widget_get_allocated_width (widget);
                    height = gtk_widget_get_allocated_height (widget);
                    gtk_widget_queue_draw_area( widget, 0, 0, width, height );
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
    gui_sketch_area_private_reload_cards( this_ );

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

    switch ( tool )
    {
        case GUI_SKETCH_TOOLS_NAVIGATE:
            TRACE_INFO("GUI_SKETCH_TOOLS_NAVIGATE");
            break;
        case GUI_SKETCH_TOOLS_EDIT:
            TRACE_INFO("GUI_SKETCH_TOOLS_EDIT");
            break;
        case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
            TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_DIAGRAM");
            break;
        case GUI_SKETCH_TOOLS_CREATE_OBJECT:
            TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_OBJECT");
            break;
        default:
            LOG_ERROR("selected_tool is out of range");
            break;
    }

    /* load/reload data to be drawn - depending on the tool, other data may be needed */
    gui_sketch_area_private_reload_cards( this_ );

    /* mark dirty rect */
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    gtk_widget_queue_draw_area( widget, 0, 0, width, height );

    TRACE_END();
}

void gui_sketch_area_private_notify_listener( gui_sketch_area_t *this_, data_table_t table, int64_t id )
{
    TRACE_BEGIN();

    if ( (*this_).listener != NULL )
    {
        TRACE_INFO( "g_signal_emit to listener" );
        g_signal_emit( (*this_).listener, gui_sketch_area_glib_signal_id, 0, table, id );
    }

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

