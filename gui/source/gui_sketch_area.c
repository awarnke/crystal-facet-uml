/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "gui_sketch_overlay.h"
#include "pencil_diagram_maker.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_table.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "universal_int32_pair.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdint.h>
#include <stdbool.h>

static bool gui_sketch_area_glib_signal_initialized = false;
static guint gui_sketch_area_glib_signal_id = 0;
const char *GUI_SKETCH_AREA_GLIB_SIGNAL_NAME = "cfu_object_selected";

void gui_sketch_area_init( gui_sketch_area_t *this_,
                           gui_sketch_marker_t *marker,
                           gui_sketch_tools_t *tools,
                           gui_simple_message_to_user_t *message_to_user,
                           gui_resources_t *res,
                           ctrl_controller_t *controller,
                           data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    gui_sketch_drag_state_init ( &((*this_).drag_state) );
    (*this_).tools = tools;
    (*this_).message_to_user = message_to_user;
    (*this_).res = res;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).card_num = 0;
    for ( int index = 0; index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS; index ++ )
    {
        (*this_).listener[index] = NULL;
    }
    (*this_).marker = marker;

    gui_sketch_area_private_load_cards( this_, DATA_ID_VOID_ID );

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
            1,
            G_TYPE_POINTER /* data_id_t* */
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

    /* reset all listeners */
    for ( int index = 0; index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS; index ++ )
    {
        (*this_).listener[index] = NULL;
    }

    (*this_).marker = NULL;
    (*this_).tools = NULL;
    (*this_).message_to_user = NULL;
    (*this_).res = NULL;
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    gui_sketch_drag_state_destroy ( &((*this_).drag_state) );

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

    }
    else if ( ! data_database_reader_is_open( (*this_).db_reader ) )
    {
        cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, 64, height );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, 0.7, 0.7, 0.7, 1.0 );
        cairo_rectangle ( cr, 64, 0, width-64, height );
        cairo_fill (cr);

        GdkPixbuf *cfu_icon = gui_resources_get_crystal_facet_uml( (*this_).res );
        double cfu_icon_width = gdk_pixbuf_get_width ( cfu_icon );
        double cfu_icon_height = gdk_pixbuf_get_height ( cfu_icon );
        gdk_cairo_set_source_pixbuf( cr, cfu_icon, 72.0, 48.0 );
        cairo_rectangle ( cr, 72, 48, 72 + cfu_icon_width, 48 + cfu_icon_height );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
        cairo_set_font_size ( cr, 12.0 );
        cairo_move_to ( cr, 72 + cfu_icon_width + 8, 48 + 14 );
        cairo_show_text ( cr, "Welcome to" );
        cairo_move_to ( cr, 72 + cfu_icon_width + 8, 48 + 2*14 );
        cairo_show_text ( cr, META_INFO_PROGRAM_NAME_STR );

        GdkPixbuf *use_db_icon = gui_resources_get_file_use_db( (*this_).res );
        double use_db_icon_width = gdk_pixbuf_get_width ( use_db_icon );
        double use_db_icon_height = gdk_pixbuf_get_height ( use_db_icon );
        gdk_cairo_set_source_pixbuf( cr, use_db_icon, 72.0, 96.0 );
        cairo_rectangle ( cr, 72, 96, 72 + use_db_icon_width, 96 + use_db_icon_height );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
        cairo_move_to ( cr, 72 + use_db_icon_width + 8, 96 + 14 );
        cairo_show_text ( cr, "To begin, please" );
        cairo_move_to ( cr, 72 + use_db_icon_width + 8, 96 + 2*14 );
        cairo_show_text ( cr, "create or open a database file first." );
    }
    else
    {
        shape_int_rectangle_t bounds;
        shape_int_rectangle_init( &bounds, 0, 0, width, height );
        if ( ! gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
        {
            gui_sketch_area_private_layout_cards( this_, bounds, cr );
        }
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
    if ( DATA_ID_VOID_ID == main_diagram_id )
    {
        /* load all without parent */
        uint32_t count;
        db_err = data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                                 DATA_ID_VOID_ID,
                                                                 GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS,
                                                                 &((*this_).private_temp_diagram_buf),
                                                                 &count
                                                               );
        if ( DATA_ERROR_NONE != ( db_err & DATA_ERROR_MASK & DATA_ERROR_NO_DB ) )
        {
            TRACE_INFO( "database not open.");
        }
        else if ( DATA_ERROR_NONE != db_err )
        {
            TSLOG_ERROR_HEX( "data_database_reader_get_diagrams_by_parent_id failed.", db_err );
        }
        else if ( count > 1 )
        {
            TSLOG_ERROR_INT( "more than one root diagram exists!", count );
        }
        else if ( count < 1 )
        {
            TSLOG_WARNING( "no root diagram exists!" );
        }
        else
        {
            main_diagram_id = data_diagram_get_id( &((*this_).private_temp_diagram_buf[0]) );
            TRACE_INFO_INT( "main_diagram_id:", main_diagram_id );
        }

        /* cleanup */
        if ( DATA_ERROR_NONE == db_err )
        {
            for ( uint32_t index = 0; index < count; index ++ )
            {
                data_diagram_destroy( &((*this_).private_temp_diagram_buf[index]) );
            }
        }
    }

    /* load data to be drawn */
    gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_SELECTED_CARD]) );
    gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_SELECTED_CARD]), main_diagram_id, (*this_).db_reader );
    (*this_).card_num = 1;
    gui_sketch_marker_set_focused_diagram( (*this_).marker, main_diagram_id );

    gui_sketch_tools_tool_t selected_tool;
    selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
    if (( GUI_SKETCH_TOOLS_NAVIGATE == selected_tool ) || ( GUI_SKETCH_TOOLS_CREATE_DIAGRAM == selected_tool ))
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
        uint32_t c_count;
        db_err = data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                                 selected_diagram_id,
                                                                 GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS,
                                                                 &((*this_).private_temp_diagram_buf),
                                                                 &c_count
                                                               );
        if ( DATA_ERROR_NONE != ( db_err & DATA_ERROR_MASK & DATA_ERROR_NO_DB ) )
        {
            TRACE_INFO( "database not open.");
        }
        else if ( DATA_ERROR_NONE != db_err )
        {
            TSLOG_ERROR_HEX( "data_database_reader_get_diagrams_by_parent_id failed.", db_err );
        }
        else
        {
            for ( uint32_t index = 0; index < c_count; index ++ )
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

void gui_sketch_area_private_layout_cards ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr )
{
    TRACE_BEGIN();

    gui_sketch_tools_tool_t selected_tool;
    selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );

    /* pre-calculate numbers needed in case of GUI_SKETCH_TOOLS_NAVIGATE and GUI_SKETCH_TOOLS_CREATE_DIAGRAM */
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
    {
        children_height = ( height * 3 ) / 10;
        children_top = height - children_height;
        parent_width = width;
        parent_height = ( height * 3 ) / 10;
        self_width = width;
        self_left = left;
        self_top = top + parent_height;
        self_height = height - children_height - parent_height;
    }

    for ( int card_idx = 0; card_idx < (*this_).card_num; card_idx ++ )
    {
        if ( ! gui_sketch_card_is_valid( &((*this_).cards[card_idx]) ))
        {
            gui_sketch_card_set_visible( &((*this_).cards[card_idx]), false );
        }
        else if (( GUI_SKETCH_TOOLS_NAVIGATE == selected_tool ) || ( GUI_SKETCH_TOOLS_CREATE_DIAGRAM == selected_tool ))
        {
            shape_int_rectangle_t card_bounds;

            if ( card_idx == 0 )  /* self */
            {
                shape_int_rectangle_init( &card_bounds, self_left, self_top, self_width, self_height );
                shape_int_rectangle_shrink_by_border( &card_bounds, HALF_BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_CENTER, SHAPE_V_ALIGN_CENTER );
            }
            else if ( card_idx == 1 )  /* parent */
            {
                shape_int_rectangle_init( &card_bounds, left, top, parent_width, parent_height );
                shape_int_rectangle_shrink_by_border( &card_bounds, HALF_BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_CENTER, SHAPE_V_ALIGN_CENTER );
            }
            else
            {
                int current_child = card_idx-2;
                int max_children = (*this_).card_num-2;
                shape_int_rectangle_init( &card_bounds, left+(width*current_child)/max_children, children_top, width/max_children, children_height );
                shape_int_rectangle_shrink_by_border( &card_bounds, HALF_BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_CENTER, SHAPE_V_ALIGN_CENTER );
            }

            gui_sketch_card_set_bounds( &((*this_).cards[card_idx]), card_bounds );
            gui_sketch_card_do_layout( &((*this_).cards[card_idx]), cr );
            gui_sketch_card_set_visible( &((*this_).cards[card_idx]), true );
        }
        else /* ==gui_sketch_card_is_valid and not GUI_SKETCH_TOOLS_NAVIGATE and not GUI_SKETCH_TOOLS_CREATE_DIAGRAM */
        {
            if ( card_idx == 0 )
            {
                shape_int_rectangle_t card_bounds;
                card_bounds = area_bounds;
                shape_int_rectangle_shrink_by_border( &card_bounds, BORDER );
                shape_int_rectangle_shrink_to_ratio( &card_bounds, RATIO_WIDTH, RATIO_HEIGHT, SHAPE_H_ALIGN_CENTER, SHAPE_V_ALIGN_CENTER );
                gui_sketch_card_set_bounds( &((*this_).cards[card_idx]), card_bounds );
                gui_sketch_card_do_layout( &((*this_).cards[card_idx]), cr );
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

    gui_sketch_tools_tool_t selected_tool;
    selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );

    /* draw background */
    if (( GUI_SKETCH_TOOLS_NAVIGATE == selected_tool ) || ( GUI_SKETCH_TOOLS_CREATE_DIAGRAM == selected_tool ))
    {
        cairo_set_source_rgba( cr, 0.4, 0.4, 0.4, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, (height*3)/10 );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, (height*3)/10, width, (height*4)/10 );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, 0.4, 0.4, 0.4, 1.0 );
        cairo_rectangle ( cr, 0, (height*7)/10, width, (height*3)/10 );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
        cairo_set_font_size ( cr, 12.0 );
        cairo_move_to ( cr, 8, 8 + 14 );
        cairo_show_text ( cr, "parent diagram" );
        cairo_move_to ( cr, 8, (height*3)/10 + 8 + 14 );
        cairo_show_text ( cr, "current diagram" );
        cairo_move_to ( cr, 8, (height*7)/10 + 8 + 14 );
        cairo_show_text ( cr, "children diagrams" );
    }
    else
    {
        cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
        cairo_rectangle ( cr, 0, 0, width, height );
        cairo_fill (cr);
    }

    /* draw all cards */
    for ( int card_idx = 0; card_idx < (*this_).card_num; card_idx ++ )
    {
        gui_sketch_card_draw( &((*this_).cards[card_idx]), (*this_).marker, cr );
    }

    /* overlay tool-helper lines */
    int32_t mouse_x = gui_sketch_drag_state_get_to_x ( &((*this_).drag_state) );
    int32_t mouse_y = gui_sketch_drag_state_get_to_y ( &((*this_).drag_state) );
    gui_sketch_overlay_t overlay;
    gui_sketch_overlay_init( &overlay );
    gui_sketch_overlay_draw( &overlay,
                             selected_tool,
                             &((*this_).drag_state),
                             gui_sketch_area_get_card_at_pos ( this_, mouse_x, mouse_y ),
                             cr
                           );
    gui_sketch_overlay_destroy( &overlay );

    TRACE_END();
}

gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {

        gui_sketch_marker_clear_highlighted( (*this_).marker );
        /* mark dirty rect */
        gtk_widget_queue_draw( widget );

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

    TRACE_INFO_INT_INT( "x/y", x, y );
    gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

    if ( (state & GDK_BUTTON1_MASK) != 0 )
    {
        TRACE_INFO( "GDK_BUTTON1_MASK" );
    }

    /* do highlight */
    gui_sketch_tools_tool_t selected_tool;
    selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
    switch ( selected_tool )
    {
        case GUI_SKETCH_TOOLS_NAVIGATE:
        {
            data_id_t object_under_mouse;
            object_under_mouse = gui_sketch_area_get_diagram_id_at_pos ( this_, x, y );
            data_id_t object_highlighted;
            object_highlighted = gui_sketch_marker_get_highlighted( (*this_).marker );
            if ( ! data_id_equals( &object_under_mouse, &object_highlighted ) )
            {
                if ( data_id_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                {
                    gui_sketch_marker_set_highlighted( (*this_).marker, object_under_mouse );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( widget );
                }
            }
        }
        break;

        case GUI_SKETCH_TOOLS_EDIT:
        {
            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* what is dragged? */
                data_id_t focused_real;
                focused_real = gui_sketch_marker_get_focused_real_object ( (*this_).marker );
                /* mark again - in case the marker was lost */
                data_id_t focused;
                focused = gui_sketch_marker_get_focused ( (*this_).marker );
                gui_sketch_marker_set_highlighted( (*this_).marker, focused );

                /* what is the target location? */
                gui_sketch_card_t *target = gui_sketch_area_get_card_at_pos ( this_, x, y );
                if ( NULL != target )
                {
                    universal_int32_pair_t order = gui_sketch_card_get_order_at_pos( target, x, y );
                    int32_t x_order = universal_int32_pair_get_first( &order );
                    int32_t y_order = universal_int32_pair_get_second( &order );
                    TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                    /* move the object in the display-cache accordingly */
                    if ( DATA_TABLE_CLASSIFIER == data_id_get_table( &focused_real ) )
                    {
                        gui_sketch_card_move_classifier_to_order( target, data_id_get_row_id( &focused_real ), x_order, y_order );

                        /* mark dirty rect */
                        gtk_widget_queue_draw( widget );
                    }
                }
            }
            else
            {
                pencil_visible_object_id_t object_under_mouse;
                pencil_visible_object_id_t object_surrounding_mouse;
                gui_sketch_area_get_object_id_at_pos ( this_, x, y, &object_under_mouse, &object_surrounding_mouse );
                data_id_t object_highlighted;
                object_highlighted = gui_sketch_marker_get_highlighted( (*this_).marker );
                if ( ! data_id_equals( pencil_visible_object_id_get_visible_id_ptr( &object_under_mouse ), &object_highlighted ) )
                {
                    if ( pencil_visible_object_id_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                    {
                        gui_sketch_marker_set_highlighted( (*this_).marker, pencil_visible_object_id_get_visible_id ( &object_under_mouse ) );

                        /* mark dirty rect */
                        gtk_widget_queue_draw( widget );
                    }
                }
            }
        }
        break;

        case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
        {
        }
        break;

        case GUI_SKETCH_TOOLS_CREATE_OBJECT:
        {
            pencil_visible_object_id_t object_under_mouse;
            pencil_visible_object_id_t object_surrounding_mouse;
            gui_sketch_area_get_object_id_at_pos ( this_, x, y, &object_under_mouse, &object_surrounding_mouse );
            data_id_t object_highlighted;
            object_highlighted = gui_sketch_marker_get_highlighted( (*this_).marker );
            if ( ! data_id_equals( pencil_visible_object_id_get_visible_id_ptr( &object_under_mouse ), &object_highlighted ) )
            {
                if ( pencil_visible_object_id_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                {
                    gui_sketch_marker_set_highlighted( (*this_).marker, pencil_visible_object_id_get_visible_id ( &object_under_mouse ) );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( widget );
                }
            }
            else if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* always redraw while dragging */
                gtk_widget_queue_draw( widget );
            }
        }
        break;

        default:
        {
            TSLOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_button_press_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    /* in general, hide the last message */
    gui_simple_message_to_user_hide( (*this_).message_to_user );

    if ( evt->button == 1 ) {
        TRACE_INFO("press");

        gtk_widget_grab_focus( widget );  /* causes the text edit widgets to lose the focus */

        /* get position */
        int32_t x;
        int32_t y;
        x = (int32_t) evt->x;
        y = (int32_t) evt->y;
        TRACE_INFO_INT_INT( "x/y", x, y );

        /* check that drag state is false */
        if ( ( gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
            || ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
        {
            TSLOG_ERROR("drag state indicates dragging - but button was not pressed before!");
        }

        /* update drag state */
        gui_sketch_drag_state_set_from ( &((*this_).drag_state), x, y );
        gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );
        gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state) );

        /* do action */
        gui_sketch_tools_tool_t selected_tool;
        selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
        switch ( selected_tool )
        {
            case GUI_SKETCH_TOOLS_NAVIGATE:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_NAVIGATE" );

                /* search selected diagram */
                data_id_t clicked_diagram_id;
                clicked_diagram_id = gui_sketch_area_get_diagram_id_at_pos ( this_, x, y );
                data_id_trace( &clicked_diagram_id );

                /* load diagram */
                if ( data_id_is_valid( &clicked_diagram_id ) )
                {
                    /* load/reload data to be drawn */
                    gui_sketch_area_private_load_cards( this_, data_id_get_row_id( &clicked_diagram_id ) );

                    /* notify listener */
                    gui_sketch_marker_set_focused( (*this_).marker, clicked_diagram_id, clicked_diagram_id );
                    gui_sketch_area_private_notify_listener( this_ );
                    gui_sketch_marker_clear_selected_set( (*this_).marker );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( widget );
                }
            }
            break;

            case GUI_SKETCH_TOOLS_EDIT:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_EDIT" );

                /* determine the focused object */
                pencil_visible_object_id_t focused_object;
                pencil_visible_object_id_t focus_surrounding_object;
                gui_sketch_area_get_object_id_at_pos ( this_, x, y, &focused_object, &focus_surrounding_object );
                pencil_visible_object_id_trace( &focused_object );
                pencil_visible_object_id_trace( &focus_surrounding_object );

                /* store focused object and notify listener */
                gui_sketch_marker_set_focused( (*this_).marker,
                                               pencil_visible_object_id_get_visible_id( &focused_object ),
                                               pencil_visible_object_id_get_model_id( &focused_object )
                                             );
                gui_sketch_area_private_notify_listener( this_ );

                gui_sketch_marker_toggle_selected_obj( (*this_).marker, pencil_visible_object_id_get_visible_id( &focused_object ) );

                /* mark dirty rect */
                gtk_widget_queue_draw( widget );
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_CREATE_DIAGRAM" );
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_OBJECT:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_CREATE_OBJECT" );

                /* what is the target location? */
                gui_sketch_card_t *target = gui_sketch_area_get_card_at_pos ( this_, x, y );

                /* determine the object at click location */
                pencil_visible_object_id_t clicked_object;
                pencil_visible_object_id_t click_surrounding_object;
                gui_sketch_area_get_object_id_at_pos ( this_, x, y, &clicked_object, &click_surrounding_object );
                pencil_visible_object_id_trace( &clicked_object );
                pencil_visible_object_id_trace( &click_surrounding_object );

                if ( NULL == target )
                {
                    TRACE_INFO_INT_INT("No card at",x,y);
                    /* if this happens, we should invalidate the marked object. */
                    gui_sketch_marker_clear_focused( (*this_).marker );
                }
                else if ( DATA_TABLE_CLASSIFIER == data_id_get_table( pencil_visible_object_id_get_model_id_ptr( &clicked_object ) ) )
                {
                    /* store focused object and notify listener */
                    gui_sketch_marker_set_focused( (*this_).marker,
                                                   pencil_visible_object_id_get_visible_id( &clicked_object ),
                                                   pencil_visible_object_id_get_model_id( &clicked_object )
                                                 );
                    gui_sketch_area_private_notify_listener( this_ );
                }
                else
                {
                    /* stop dragging */
                    gui_sketch_drag_state_set_dragging ( &((*this_).drag_state), false );

                    /* create a new classifier */
                    data_diagram_t *target_diag = gui_sketch_card_get_diagram_ptr ( target );
                    int64_t selected_diagram_id = data_diagram_get_id( target_diag );
                    TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

                    universal_int32_pair_t order = gui_sketch_card_get_order_at_pos( target, x, y );
                    int32_t x_order = universal_int32_pair_get_first( &order );
                    int32_t y_order = universal_int32_pair_get_second( &order );
                    TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                    ctrl_classifier_controller_t *classifier_control;
                    classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                    static char *(NAMES[8]) = {"off-","on-","debug-","persistence-","communication-","bootloader-","driver-","application-"};
                    static uint8_t my_counter = 0;
                    char newname_buf[24];
                    utf8stringbuf_t full_new_name = UTF8STRINGBUF( newname_buf );
                    utf8stringbuf_copy_str( full_new_name, NAMES[(x+y)&0x07] );
                    utf8stringbuf_append_int( full_new_name, my_counter++ );

                    int64_t new_classifier_id;
                    int64_t new_diagele_id;
                    ctrl_error_t c_result;
                    c_result = ctrl_classifier_controller_create_classifier_in_diagram ( classifier_control,
                                                                                         selected_diagram_id,
                                                                                         DATA_CLASSIFIER_TYPE_BLOCK,
                                                                                         utf8stringbuf_get_string( full_new_name ),
                                                                                         x_order,
                                                                                         y_order,
                                                                                         &new_diagele_id,
                                                                                         &new_classifier_id
                                                                                       );
                    if ( CTRL_ERROR_DUPLICATE_NAME == c_result )
                    {
                        gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                             GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                             GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                             utf8stringbuf_get_string( full_new_name )
                                                                           );
                    }
                    else
                    {
                        /* set focused object and notify listener */
                        data_id_t focused_id;
                        data_id_t focused_real_id;
                        data_id_init( &focused_id, DATA_TABLE_DIAGRAMELEMENT, new_diagele_id );
                        data_id_init( &focused_real_id, DATA_TABLE_CLASSIFIER, new_classifier_id );
                        gui_sketch_marker_set_focused( (*this_).marker, focused_id, focused_real_id );
                        gui_sketch_area_private_notify_listener( this_ );
                        gui_sketch_marker_clear_selected_set( (*this_).marker );
                    }

                    TRACE_INFO_INT( "new_classifier_id:", new_classifier_id );
                }
            }
            break;

            default:
            {
                TSLOG_ERROR( "selected_tool is out of range" );
            }
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

        /* get position */
        int32_t x;
        int32_t y;
        x = (int32_t) evt->x;
        y = (int32_t) evt->y;
        TRACE_INFO_INT_INT("x/y",x,y);

        /* check that drag state is true */
        if ( ( ! gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
            && ( ! gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
        {
            TRACE_INFO("drag state indicates no dragging and no waiting - but button was pressed before!");
        }

        /* do action */
        gui_sketch_tools_tool_t selected_tool;
        selected_tool = gui_sketch_tools_get_selected_tool( (*this_).tools );
        switch ( selected_tool )
        {
            case GUI_SKETCH_TOOLS_NAVIGATE:
            {
                TRACE_INFO("GUI_SKETCH_TOOLS_NAVIGATE");
            }
            break;

            case GUI_SKETCH_TOOLS_EDIT:
            {
                TRACE_INFO("GUI_SKETCH_TOOLS_EDIT");

                if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
                {

                    /* which object is selected? */
                    data_id_t focused_visible;
                    data_id_t focused_real;
                    focused_visible = gui_sketch_marker_get_focused ( (*this_).marker );
                    focused_real = gui_sketch_marker_get_focused_real_object ( (*this_).marker );
                    data_id_trace( &focused_visible );
                    data_id_trace( &focused_real );

                    /* what is the target location? */
                    gui_sketch_card_t *target = gui_sketch_area_get_card_at_pos ( this_, x, y );
                    if ( NULL == target )
                    {
                        TRACE_INFO_INT_INT("No card at",x,y);
                    }
                    else if ( DATA_TABLE_CLASSIFIER != data_id_get_table( &focused_real ) )
                    {
                        TRACE_INFO("Dragged object is no classifier");
                    }
                    else
                    {
                        universal_int32_pair_t order = gui_sketch_card_get_order_at_pos( target, x, y );
                        int32_t x_order = universal_int32_pair_get_first( &order );
                        int32_t y_order = universal_int32_pair_get_second( &order );
                        TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                        /* update db */
                        ctrl_classifier_controller_t *classifier_control;
                        classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                        ctrl_error_t mov_result;
                        mov_result = ctrl_classifier_controller_update_classifier_x_order_y_order ( classifier_control,
                                                                                                    data_id_get_row_id( &focused_real ),
                                                                                                    x_order,
                                                                                                    y_order );
                    }
                }
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
            {
                TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_DIAGRAM");

                guint height;
                height = gtk_widget_get_allocated_height (widget);
                if ( y*10 < height*7 )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_CLICK_IN_CHILDREN_AREA
                                                           );
                }
                else
                {
                    int64_t selected_diagram_id;
                    selected_diagram_id = gui_sketch_area_get_selected_diagram_id( this_ );
                    TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

                    ctrl_diagram_controller_t *diag_control;
                    diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                    char* new_name;
                    static char *(NAMES[8]) = {"Upper Layer","Overview","Power States","Startup Sequence","Shutdown states","Boot timings","Lower Layer","Hello World"};
                    new_name = NAMES[(x+y)&0x07];

                    int64_t new_diag_id;
                    ctrl_error_t c_result;
                    c_result = ctrl_diagram_controller_create_child_diagram ( diag_control, selected_diagram_id, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, new_name, &new_diag_id );

                    /* load/reload data to be drawn */
                    gui_sketch_area_private_load_cards( this_, new_diag_id );

                    /* notify listener */
                    data_id_t focused_id;
                    data_id_init( &focused_id, DATA_TABLE_DIAGRAM, new_diag_id );
                    gui_sketch_marker_set_focused( (*this_).marker, focused_id, focused_id );
                    gui_sketch_area_private_notify_listener( this_ );
                    gui_sketch_marker_clear_selected_set( (*this_).marker );

                    /* change the selected tool */
                    gui_sketch_tools_set_selected_tool( (*this_).tools, GUI_SKETCH_TOOLS_NAVIGATE );
                }
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_OBJECT:
            {
                TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_OBJECT");

                if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
                {
                    /* which object is selected? */
                    data_id_t focused_real;
                    focused_real = gui_sketch_marker_get_focused_real_object ( (*this_).marker );

                    /* which object is at the target location? */
                    data_id_t destination_real;
                    pencil_visible_object_id_t destination_object;
                    pencil_visible_object_id_t dest_surrounding_object;

                    gui_sketch_area_get_object_id_at_pos ( this_, x, y, &destination_object, &dest_surrounding_object );
                    destination_real = pencil_visible_object_id_get_model_id( &destination_object );

                    if ( data_id_is_valid( &focused_real ) && data_id_is_valid( &destination_real ) )
                    {
                        if ( ( DATA_TABLE_CLASSIFIER == data_id_get_table( &focused_real ) )
                            && ( DATA_TABLE_CLASSIFIER == data_id_get_table( &destination_real ) ) )
                        {
                            /* get classifier controller */
                            ctrl_classifier_controller_t *classifier_control;
                            classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                            /* propose a list_order for the relationship */
                            int32_t list_order_proposal = 0;
                            gui_sketch_card_t *target = gui_sketch_area_get_card_at_pos ( this_, x, y );
                            if ( NULL != target )
                            {
                                list_order_proposal = gui_sketch_card_get_highest_list_order( target ) + 1024;
                            }

                            /* define relationship */
                            data_relationship_t new_relationship;
                            data_error_t d_err;
                            d_err = data_relationship_init ( &new_relationship,
                                                             DATA_ID_VOID_ID,
                                                             DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
                                                             data_id_get_row_id( &focused_real ),
                                                             data_id_get_row_id( &destination_real ),
                                                             "depends on", /* =relationship_name */
                                                             "", /* =relationship_description */
                                                             list_order_proposal
                                                           );

                            /* create relationship */
                            int64_t new_relationship_id;
                            ctrl_error_t c_result;
                            c_result = ctrl_classifier_controller_create_relationship ( classifier_control,
                                                                                        &new_relationship,
                                                                                        false, /*=add_to_latest_undo_set*/
                                                                                        &new_relationship_id
                                                                                      );
                            data_relationship_destroy ( &new_relationship );

                            if ( CTRL_ERROR_NONE != c_result )
                            {
                                TSLOG_ERROR("unexpected error at ctrl_classifier_controller_create_relationship");
                            }
                            else
                            {
                                /* set focused object and notify listener */
                                data_id_t focused_id;
                                data_id_init( &focused_id, DATA_TABLE_RELATIONSHIP, new_relationship_id );
                                gui_sketch_marker_set_focused( (*this_).marker, focused_id, focused_id );
                                gui_sketch_area_private_notify_listener( this_ );
                                gui_sketch_marker_clear_selected_set( (*this_).marker );
                            }
                        }
                    }
                }
                else if ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) )
                {
                    /* click on classifier without drag */
                    data_id_t focused_real;
                    focused_real = gui_sketch_marker_get_focused_real_object ( (*this_).marker );
                    if ( data_id_is_valid( &focused_real ) )
                    {
                        if ( DATA_TABLE_CLASSIFIER == data_id_get_table( &focused_real ) )
                        {
                            /* get classifier controller */
                            ctrl_classifier_controller_t *classifier_control;
                            classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                            /* propose a list_order for the feature */
                            int32_t list_order_proposal = 0;
                            gui_sketch_card_t *target = gui_sketch_area_get_card_at_pos ( this_, x, y );
                            if ( NULL != target )
                            {
                                list_order_proposal = gui_sketch_card_get_highest_list_order( target ) + 1024;
                            }

                            /* define feature */
                            data_feature_t new_feature;
                            data_error_t data_err;
                            data_err = data_feature_init ( &new_feature,
                                                           DATA_ID_VOID_ID, /* feature_id */
                                                           DATA_FEATURE_TYPE_OPERATION,
                                                           data_id_get_row_id( &focused_real ),
                                                           "get_state",
                                                           "uint32_t(*)(void)",
                                                           "",
                                                           list_order_proposal
                                                         );

                            /* create feature */
                            int64_t new_feature_id;
                            ctrl_error_t ctrl_err;
                            ctrl_err = ctrl_classifier_controller_create_feature ( classifier_control,
                                                                                   &new_feature,
                                                                                   false, /*=add_to_latest_undo_set*/
                                                                                   &new_feature_id
                                                                                 );
                            data_feature_destroy ( &new_feature );

                            if ( CTRL_ERROR_DUPLICATE_NAME == ctrl_err )
                            {
                                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                                     GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                                     GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                                     "get_state"
                                );
                            }
                            else
                            {
                                /* set focused object and notify listener */
                                data_id_t focused_id;
                                data_id_init( &focused_id, DATA_TABLE_FEATURE, new_feature_id );
                                gui_sketch_marker_set_focused( (*this_).marker, focused_id, focused_id );
                                gui_sketch_area_private_notify_listener( this_ );
                                gui_sketch_marker_clear_selected_set( (*this_).marker );
                            }
                        }
                    }
                }
            }
            break;

            default:
            {
                TSLOG_ERROR("selected_tool is out of range");
            }
            break;
        }

        /* stop dragging */
        gui_sketch_drag_state_set_dragging ( &((*this_).drag_state), false );

        /* mark dirty rect */
        gtk_widget_queue_draw( widget );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return TRUE;
}

void gui_sketch_area_data_changed_callback( GtkWidget *widget, data_id_t *object_id, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    /* load/reload data to be drawn */
    gui_sketch_area_private_reload_cards( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( widget );

    TRACE_END();
}

void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_sketch_tools_tool_t tool, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    switch ( tool )
    {
        case GUI_SKETCH_TOOLS_NAVIGATE:
        {
            TRACE_INFO("GUI_SKETCH_TOOLS_NAVIGATE");
        }
        break;

        case GUI_SKETCH_TOOLS_EDIT:
        {
            TRACE_INFO("GUI_SKETCH_TOOLS_EDIT");
        }
        break;

        case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
        {
            TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_DIAGRAM");
        }
        break;

        case GUI_SKETCH_TOOLS_CREATE_OBJECT:
        {
            TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_OBJECT");
        }
        break;

        default:
        {
            TSLOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    /* load/reload data to be drawn - depending on the tool, other data may be needed */
    gui_sketch_area_private_reload_cards( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( widget );

    TRACE_END();
}

void gui_sketch_area_private_notify_listener( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    data_id_t full_id;
    full_id = gui_sketch_marker_get_focused_real_object( (*this_).marker );

    for ( int index = 0; index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS; index ++ )
    {
        if ( (*this_).listener != NULL )
        {
            TRACE_INFO_INT( "g_signal_emit to listener", index );
            g_signal_emit( (*this_).listener[index], gui_sketch_area_glib_signal_id, 0, &full_id );
        }
    }

    TRACE_END();
}


/*
Copyright 2016-2017 Andreas Warnke

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

