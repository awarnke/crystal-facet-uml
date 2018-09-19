/* File: gui_sketch_area.c; Copyright and License: see below */

#include "gui_sketch_area.h"
#include "pencil_diagram_maker.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_table.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdint.h>
#include <stdbool.h>

static bool gui_sketch_area_glib_signal_initialized = false;
static guint gui_sketch_area_glib_signal_id = 0;
const char *GUI_SKETCH_AREA_GLIB_SIGNAL_NAME = "cfu_object_selected";

void gui_sketch_area_init( gui_sketch_area_t *this_,
                           gui_marker_t *marker,
                           gui_tools_t *tools,
                           gui_simple_message_to_user_t *message_to_user,
                           gui_resources_t *resources,
                           ctrl_controller_t *controller,
                           data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    /* init pointers to external objects */
    (*this_).tools = tools;
    (*this_).message_to_user = message_to_user;
    (*this_).resources = resources;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;

    /* init instances of own objects */
    (*this_).card_num = 0;
    gui_sketch_nav_tree_init( &((*this_).nav_tree) );
    gui_sketch_result_list_init( &((*this_).result_list) );
    gui_sketch_drag_state_init ( &((*this_).drag_state) );
    for ( int index = 0; index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS; index ++ )
    {
        (*this_).listener[index] = NULL;
    }
    (*this_).marker = marker;
    gui_sketch_overlay_init( &((*this_).overlay) );
    gui_sketch_background_init( &((*this_).background), resources );
    gui_sketch_object_creator_init ( &((*this_).object_creator), controller, db_reader );

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

    /* fetch initial data from the database */
    gui_sketch_area_private_load_data( this_, DATA_ID_VOID_ID );

    TRACE_END();
}

void gui_sketch_area_destroy( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    /* destroy instances of own objects */
    gui_sketch_result_list_destroy( &((*this_).result_list) );
    gui_sketch_nav_tree_destroy( &((*this_).nav_tree) );

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

    /* destroy instances of own objects */
    gui_sketch_object_creator_destroy ( &((*this_).object_creator) );
    gui_sketch_overlay_destroy( &((*this_).overlay) );
    gui_sketch_background_destroy( &((*this_).background) );
    gui_sketch_drag_state_destroy ( &((*this_).drag_state) );

    /* unset pointers to external objects */
    (*this_).marker = NULL;
    (*this_).tools = NULL;
    (*this_).message_to_user = NULL;
    (*this_).resources = NULL;
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

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

    if ( ! data_database_reader_is_open( (*this_).db_reader ) )
    {
        gui_sketch_background_draw_introduction( &((*this_).background), 0, 0, width, height, cr );
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

void gui_sketch_area_private_load_data ( gui_sketch_area_t *this_, int64_t main_diagram_id )
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
    gui_marker_set_focused_diagram( (*this_).marker, main_diagram_id );

    gui_tools_tool_t selected_tool;
    selected_tool = gui_tools_get_selected_tool( (*this_).tools );
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

void gui_sketch_area_private_reload_data ( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    /* determine currently selected id */
    int64_t selected_diagram_id;
    selected_diagram_id = gui_sketch_area_get_selected_diagram_id( this_ );
    TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

    /* reload diagram data */
    gui_sketch_area_private_load_data( this_, selected_diagram_id );

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

    gui_tools_tool_t selected_tool;
    selected_tool = gui_tools_get_selected_tool( (*this_).tools );

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

    gui_tools_tool_t selected_tool;
    selected_tool = gui_tools_get_selected_tool( (*this_).tools );

    /* draw background */
    if (( GUI_SKETCH_TOOLS_NAVIGATE == selected_tool ) || ( GUI_SKETCH_TOOLS_CREATE_DIAGRAM == selected_tool ))
    {
        unsigned int depth;
        unsigned int children;
        depth = ( gui_sketch_card_is_valid( &((*this_).cards[1]) ) ) ? 1 : 0;  /* currently, only root and non-root can be distinguished */
        children = (*this_).card_num-2;  /* concept of card numbers to be updated in the future */
        gui_sketch_background_draw_navigation( &((*this_).background),
                                               depth, children, (GUI_SKETCH_TOOLS_CREATE_DIAGRAM == selected_tool),
                                               0, 0, width, height, cr );
    }
    else
    {
        gui_sketch_background_draw_edit( &((*this_).background),
                                         (GUI_SKETCH_TOOLS_CREATE_OBJECT == selected_tool),
                                         0, 0, width, height, cr );
    }

    /* draw all cards */
    for ( int card_idx = 0; card_idx < (*this_).card_num; card_idx ++ )
    {
        gui_sketch_card_draw( &((*this_).cards[card_idx]), (*this_).marker, cr );
    }

    /* overlay tool-helper lines */
    int32_t mouse_x = gui_sketch_drag_state_get_to_x ( &((*this_).drag_state) );
    int32_t mouse_y = gui_sketch_drag_state_get_to_y ( &((*this_).drag_state) );
    gui_sketch_overlay_draw( &((*this_).overlay),
                             selected_tool,
                             &((*this_).drag_state),
                             gui_sketch_area_get_card_at_pos ( this_, mouse_x, mouse_y ),
                             cr
                           );

    TRACE_END();
}

gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {

        gui_marker_clear_highlighted( (*this_).marker );
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

    x = (int32_t) (*evt).x;
    y = (int32_t) (*evt).y;
    state = (GdkModifierType) (*evt).state;
    TRACE_INFO_INT_INT( "x/y", x, y );

    /* update drag coordinates */
    gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

    if ( (state & GDK_BUTTON1_MASK) != 0 )
    {
        TRACE_INFO( "GDK_BUTTON1_MASK" );
    }

    /* do highlight */
    gui_tools_tool_t selected_tool;
    selected_tool = gui_tools_get_selected_tool( (*this_).tools );
    switch ( selected_tool )
    {
        case GUI_SKETCH_TOOLS_NAVIGATE:
        {
            data_id_t object_under_mouse;
            object_under_mouse = gui_sketch_area_get_diagram_id_at_pos ( this_, x, y );
            data_id_t object_highlighted;
            object_highlighted = gui_marker_get_highlighted( (*this_).marker );
            if ( ! data_id_equals( &object_under_mouse, &object_highlighted ) )
            {
                if ( data_id_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                {
                    gui_marker_set_highlighted( (*this_).marker, object_under_mouse );

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
                data_id_pair_t *dragged_object;
                dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                data_id_t dragged_element;
                dragged_element = data_id_pair_get_primary_id( dragged_object );
                data_id_t dragged_classifier;
                dragged_classifier = data_id_pair_get_secondary_id( dragged_object );

                /* mark again - in case the marker was lost */
                gui_marker_set_highlighted( (*this_).marker, dragged_element );

                /* what is the target location? */
                gui_sketch_card_t *target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );
                if ( NULL != target_card )
                {
                    layout_order_t layout_order;
                    if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &dragged_element ) )
                    {
                        layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_classifier, x, y );
                        layout_order_trace( &layout_order );
                        gui_sketch_card_move_object_to_order( target_card, dragged_classifier, &layout_order );
                    }
                    else
                    {
                        layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_element, x, y );
                        layout_order_trace( &layout_order );
                        gui_sketch_card_move_object_to_order( target_card, dragged_element, &layout_order );
                    }

                    /* mark dirty rect */
                    gtk_widget_queue_draw( widget );
                }
                else
                {
                    TRACE_INFO( "mouse click outside sketch card." );
                }
            }
            else
            {
                data_id_pair_t object_under_mouse;
                gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, &object_under_mouse );
                data_id_t object_highlighted;
                object_highlighted = gui_marker_get_highlighted( (*this_).marker );
                if ( ! data_id_equals( data_id_pair_get_primary_id_ptr( &object_under_mouse ), &object_highlighted ) )
                {
                    if ( data_id_pair_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                    {
                        gui_marker_set_highlighted( (*this_).marker, data_id_pair_get_primary_id ( &object_under_mouse ) );

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
            data_id_pair_t object_under_mouse;
            gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, &object_under_mouse );
            data_id_t  classifier_under_mouse;
            classifier_under_mouse = data_id_pair_get_primary_id( &object_under_mouse );

            data_id_t object_highlighted;
            object_highlighted = gui_marker_get_highlighted( (*this_).marker );
            if ( ! data_id_equals( &classifier_under_mouse, &object_highlighted ) )
            {
                if ( data_id_is_valid( &classifier_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                {
                    gui_marker_set_highlighted( (*this_).marker, classifier_under_mouse );

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

    if ( (*evt).button == 1 ) {
        TRACE_INFO("press");

        gtk_widget_grab_focus( widget );  /* causes the text edit widgets to lose the focus */

        /* get position */
        int32_t x;
        int32_t y;
        x = (int32_t) (*evt).x;
        y = (int32_t) (*evt).y;
        TRACE_INFO_INT_INT( "x/y", x, y );

        /* check that drag state is false */
        if ( ( gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
            || ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
        {
            TSLOG_ERROR("drag state indicates dragging - but button was not pressed before!");
        }

        /* update drag coordinates */
        gui_sketch_drag_state_set_from ( &((*this_).drag_state), x, y );
        gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

        /* do action */
        gui_tools_tool_t selected_tool;
        selected_tool = gui_tools_get_selected_tool( (*this_).tools );
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
                    /* update drag state */
                    data_id_pair_t dragged_object;
                    data_id_pair_init_solo ( &dragged_object, clicked_diagram_id );
                    gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), dragged_object );

                    /* load/reload data to be drawn */
                    gui_sketch_area_private_load_data( this_, data_id_get_row_id( &clicked_diagram_id ) );

                    /* notify listener */
                    gui_marker_set_focused( (*this_).marker, clicked_diagram_id );
                    gui_sketch_area_private_notify_listener( this_, clicked_diagram_id );
                    gui_marker_clear_selected_set( (*this_).marker );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( widget );
                }
                else
                {
                    TRACE_INFO("invalid clicked object at gui_sketch_area_button_press_callback");
                }
            }
            break;

            case GUI_SKETCH_TOOLS_EDIT:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_EDIT" );

                /* determine the focused object */
                data_id_pair_t focused_object;
                gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, &focused_object );
                data_id_pair_trace( &focused_object );
                data_id_t focused_object_visible;
                focused_object_visible = data_id_pair_get_primary_id( &focused_object );

                /* update drag state */
                gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), focused_object );

                /* which object is currently focused? */
                data_id_t focused_visible_before;
                focused_visible_before = gui_marker_get_focused ( (*this_).marker );

                if ( data_id_equals ( &focused_object_visible, &focused_visible_before ) )
                {
                    /* the clicked object is already focused */
                    gui_marker_toggle_selected_obj( (*this_).marker, focused_object_visible );
                }
                else
                {
                    /* store focused object and notify listener */
                    gui_marker_set_focused ( (*this_).marker,
                                                    focused_object_visible
                                                  );

                    data_id_t real_object = focused_object_visible;
                    if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &real_object ) )
                    {
                        real_object = data_id_pair_get_secondary_id( &focused_object );
                    }
                    gui_sketch_area_private_notify_listener( this_, real_object );
                }

                /* mark dirty rect */
                gtk_widget_queue_draw( widget );
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_CREATE_DIAGRAM" );

                /* determine active diagram */
                int64_t selected_diagram_id;
                selected_diagram_id = gui_sketch_area_get_selected_diagram_id( this_ );
                TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

                /* update drag state */
                data_id_pair_t dragged_object;
                data_id_pair_init_by_table_and_id ( &dragged_object,
                                                    DATA_TABLE_DIAGRAM,
                                                    selected_diagram_id,
                                                    DATA_TABLE_VOID,
                                                    DATA_ID_VOID_ID
                                                  );
                gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), dragged_object );
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_OBJECT:
            {
                TRACE_INFO( "GUI_SKETCH_TOOLS_CREATE_OBJECT" );

                /* what is the target location? */
                gui_sketch_card_t *target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );

                if ( NULL == target_card )
                {
                    TRACE_INFO_INT_INT("No card at",x,y);

                    /* if this happens, invalidate the marked object. */
                    gui_marker_clear_focused( (*this_).marker );
                }
                else
                {
                    /* determine the object at click location */
                    data_id_pair_t clicked_object;
                    data_id_pair_t surrounding_object;
                    gui_sketch_area_private_get_object_ids_at_pos ( this_, x, y, &clicked_object, &surrounding_object );
                    data_id_pair_trace( &clicked_object );
                    data_id_pair_trace( &surrounding_object );
                    data_id_t *clicked_classifier;
                    data_id_t *surrounding_classifier;
                    clicked_classifier = data_id_pair_get_secondary_id_ptr( &clicked_object );
                    surrounding_classifier = data_id_pair_get_secondary_id_ptr( &surrounding_object );

                    if ( DATA_TABLE_CLASSIFIER == data_id_get_table( clicked_classifier ) )
                    {
                        /* update drag state */
                        gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), clicked_object );

                        /* set focused object (either a diagramelement or a feature) and notify listener */
                        gui_marker_set_focused( (*this_).marker,
                                                       data_id_pair_get_primary_id( &clicked_object )
                                                     );
                        gui_sketch_area_private_notify_listener( this_, data_id_pair_get_secondary_id( &clicked_object ) );
                    }
                    else /* clicked either into inner space of a classifier or outside any classifier */
                    {
                        /* stop dragging */
                        gui_sketch_drag_state_stop_dragging ( &((*this_).drag_state) );

                        /* create a new classifier */
                        data_diagram_t *target_diag = gui_sketch_card_get_diagram_ptr ( target_card );
                        int64_t selected_diagram_id = data_diagram_get_id( target_diag );
                        TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );

                        data_id_t dummy_classifier;
                        data_id_init( &dummy_classifier, DATA_TABLE_CLASSIFIER, DATA_ID_VOID_ID );
                        layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dummy_classifier, x, y );
                        int32_t x_order = layout_order_get_first( &layout_order );
                        int32_t y_order = layout_order_get_second( &layout_order );
                        TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                        /* create a classifier or a child-classifier */
                        ctrl_error_t c_result;
                        int64_t new_diagele_id;
                        int64_t new_classifier_id;
                        if ( DATA_TABLE_CLASSIFIER == data_id_get_table( surrounding_classifier ) )
                        {
                            int64_t new_relationship_id;
                            c_result = gui_sketch_object_creator_create_classifier_as_child ( &((*this_).object_creator),
                                                                                              selected_diagram_id,
                                                                                              data_id_get_row_id( surrounding_classifier ),
                                                                                              x_order,
                                                                                              y_order,
                                                                                              &new_diagele_id,
                                                                                              &new_classifier_id,
                                                                                              &new_relationship_id
                                                                                            );
                        }
                        else
                        {
                            c_result = gui_sketch_object_creator_create_classifier ( &((*this_).object_creator),
                                                                                     selected_diagram_id,
                                                                                     x_order,
                                                                                     y_order,
                                                                                     &new_diagele_id,
                                                                                     &new_classifier_id
                                                                                   );
                        }

                        if ( CTRL_ERROR_DUPLICATE_NAME == c_result )
                        {
                            gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                                 GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                                 ""
                                                                               );
                        }
                        else if ( CTRL_ERROR_NONE != c_result )
                        {
                            TSLOG_ERROR("unexpected error at gui_sketch_object_creator_create_classifier/_as_child");
                        }
                        else
                        {
                            /* set focused object and notify listener */
                            data_id_t focused_id;
                            data_id_t focused_real_id;
                            data_id_init( &focused_id, DATA_TABLE_DIAGRAMELEMENT, new_diagele_id );
                            data_id_init( &focused_real_id, DATA_TABLE_CLASSIFIER, new_classifier_id );
                            gui_marker_set_focused( (*this_).marker, focused_id );
                            gui_sketch_area_private_notify_listener( this_, focused_real_id );
                            gui_marker_clear_selected_set( (*this_).marker );

                            TRACE_INFO_INT( "new_classifier_id:", new_classifier_id );
                        }
                    }
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

    if ( (*evt).button == 1 ) {
        TRACE_INFO("release");

        /* get position */
        int32_t x;
        int32_t y;
        x = (int32_t) (*evt).x;
        y = (int32_t) (*evt).y;
        TRACE_INFO_INT_INT("x/y",x,y);

        /* check that drag state is true */
        if ( ( ! gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
            && ( ! gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
        {
            TRACE_INFO("drag state indicates no dragging and no waiting - but button was pressed before!");
        }

        /* do action */
        gui_tools_tool_t selected_tool;
        selected_tool = gui_tools_get_selected_tool( (*this_).tools );
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
                    data_id_pair_t *dragged_object;
                    dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                    data_id_t dragged_element;
                    dragged_element = data_id_pair_get_primary_id( dragged_object );
                    data_id_t dragged_classifier;
                    dragged_classifier = data_id_pair_get_secondary_id( dragged_object );

                    /* what is the target location? */
                    gui_sketch_card_t *target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );
                    if ( NULL == target_card )
                    {
                        TRACE_INFO_INT_INT("No card at",x,y);
                    }
                    else if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &dragged_element ) )
                    {
                        layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_classifier, x, y );
                        if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                        {
                            int32_t list_order = layout_order_get_first( &layout_order );
                            TRACE_INFO_INT( "list_order", list_order );

                            /* update db */
                            ctrl_classifier_controller_t *classifier_control;
                            classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                            ctrl_error_t mov_result;
                            mov_result = ctrl_classifier_controller_update_classifier_list_order ( classifier_control,
                                                                                                   data_id_get_row_id( &dragged_classifier ),
                                                                                                   list_order
                                                                                                 );
                        }
                        else if ( PENCIL_LAYOUT_ORDER_TYPE_X_Y == layout_order_get_order_type( &layout_order ) )
                        {
                            int32_t x_order = layout_order_get_first( &layout_order );
                            int32_t y_order = layout_order_get_second( &layout_order );
                            TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                            /* update db */
                            ctrl_classifier_controller_t *classifier_control;
                            classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                            ctrl_error_t mov_result;
                            mov_result = ctrl_classifier_controller_update_classifier_x_order_y_order ( classifier_control,
                                                                                                        data_id_get_row_id( &dragged_classifier ),
                                                                                                        x_order,
                                                                                                        y_order
                                                                                                    );
                        }
                    }
                    else if ( DATA_TABLE_RELATIONSHIP == data_id_get_table( &dragged_element ) )
                    {
                        layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_element, x, y );
                        if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                        {
                            int32_t list_order = layout_order_get_first( &layout_order );
                            TRACE_INFO_INT( "list_order", list_order );

                            /* update db */
                            ctrl_classifier_controller_t *classifier_control;
                            classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                            ctrl_error_t mov_result;
                            mov_result = ctrl_classifier_controller_update_relationship_list_order ( classifier_control,
                                                                                                    data_id_get_row_id( &dragged_element ),
                                                                                                    list_order
                                                                                                );
                        }
                    }
                    else
                    {
                        TRACE_INFO("Dragged object is neither relationship nor classifier");
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

                    ctrl_error_t c_result;
                    int64_t new_diag_id;
                    c_result = gui_sketch_object_creator_create_diagram ( &((*this_).object_creator),
                                                                          selected_diagram_id,
                                                                          &new_diag_id
                                                                        );

                    if ( CTRL_ERROR_NONE != c_result )
                    {
                        TSLOG_ERROR("unexpected error at gui_sketch_object_creator_create_classifier");
                    }
                    else
                    {
                        /* load/reload data to be drawn */
                        gui_sketch_area_private_load_data( this_, new_diag_id );

                        /* notify listener */
                        data_id_t focused_id;
                        data_id_init( &focused_id, DATA_TABLE_DIAGRAM, new_diag_id );
                        gui_marker_set_focused( (*this_).marker, focused_id );
                        gui_sketch_area_private_notify_listener( this_, focused_id );
                        gui_marker_clear_selected_set( (*this_).marker );

                        /* change the selected tool */
                        gui_tools_set_selected_tool( (*this_).tools, GUI_SKETCH_TOOLS_NAVIGATE );
                    }
                }
            }
            break;

            case GUI_SKETCH_TOOLS_CREATE_OBJECT:
            {
                TRACE_INFO("GUI_SKETCH_TOOLS_CREATE_OBJECT");

                if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
                {
                    /* which object is selected? */
                    data_id_pair_t *dragged_object;
                    dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                    data_id_pair_trace( dragged_object );
                    data_id_t dragged_element;
                    dragged_element = data_id_pair_get_primary_id( dragged_object );
                    data_id_t dragged_classifier;
                    dragged_classifier = data_id_pair_get_secondary_id( dragged_object );

                    /* which object is at the target location? */
                    data_id_pair_t destination_object;
                    gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, &destination_object );
                    data_id_pair_trace( &destination_object );
                    data_id_t destination_element;
                    destination_element = data_id_pair_get_primary_id( &destination_object );
                    data_id_t destination_classifier;
                    destination_classifier = data_id_pair_get_secondary_id( &destination_object );

                    if ( data_id_is_valid( &dragged_classifier ) && data_id_is_valid( &destination_classifier ) )
                    {
                        if ( ( DATA_TABLE_CLASSIFIER == data_id_get_table( &dragged_classifier ) )
                            && ( DATA_TABLE_CLASSIFIER == data_id_get_table( &destination_classifier ) ) )
                        {
                            /* determine source and destionation */
                            int64_t new_from_classifier_id;
                            int64_t new_to_classifier_id;
                            int64_t new_from_feature_id;
                            int64_t new_to_feature_id;
                            {
                                new_from_classifier_id = data_id_get_row_id( &dragged_classifier );
                                if ( DATA_TABLE_FEATURE == data_id_get_table( &dragged_element ) )
                                {
                                    new_from_feature_id = data_id_get_row_id( &dragged_element );
                                }
                                else
                                {
                                    new_from_feature_id = DATA_ID_VOID_ID;
                                }
                                new_to_classifier_id = data_id_get_row_id( &destination_classifier );
                                if ( DATA_TABLE_FEATURE == data_id_get_table( &destination_element ) )
                                {
                                    new_to_feature_id = data_id_get_row_id( &destination_element );
                                }
                                else
                                {
                                    new_to_feature_id = DATA_ID_VOID_ID;
                                }
                            }

                            /* propose a list_order for the relationship */
                            int32_t list_order_proposal = 0;
                            gui_sketch_card_t *target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );
                            if ( NULL != target_card )
                            {
                                /* propose a list order */
                                layout_order_t layout_order;
                                data_id_t new_relationship;
                                data_id_init ( &new_relationship, DATA_TABLE_RELATIONSHIP, DATA_ID_VOID_ID );
                                layout_order = gui_sketch_card_get_order_at_pos( target_card, new_relationship, x, y );
                                if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                                {
                                    list_order_proposal = layout_order_get_first( &layout_order );
                                }
                                else
                                {
                                    list_order_proposal = gui_sketch_card_get_highest_list_order( target_card ) + 1024;
                                }
                            }

                            int64_t new_relationship_id;
                            ctrl_error_t c_result;
                            c_result = gui_sketch_object_creator_create_relationship ( &((*this_).object_creator),
                                                                                       new_from_classifier_id,
                                                                                       new_to_classifier_id,
                                                                                       list_order_proposal,
                                                                                       new_from_feature_id,
                                                                                       new_to_feature_id,
                                                                                       &new_relationship_id
                                                                                     );

                            if ( CTRL_ERROR_NONE != c_result )
                            {
                                TSLOG_ERROR("unexpected error at gui_sketch_object_creator_create_relationship");
                            }
                            else
                            {
                                /* set focused object and notify listener */
                                data_id_t focused_id;
                                data_id_init( &focused_id, DATA_TABLE_RELATIONSHIP, new_relationship_id );
                                gui_marker_set_focused( (*this_).marker, focused_id );
                                gui_sketch_area_private_notify_listener( this_, focused_id );
                                gui_marker_clear_selected_set( (*this_).marker );
                            }
                        }
                    }
                }
                else if ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) )
                {
                    /* click on classifier without drag */
                    data_id_pair_t *dragged_object;
                    dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                    data_id_t dragged_classifier;
                    dragged_classifier = data_id_pair_get_secondary_id( dragged_object );

                    gui_sketch_card_t *target_card;
                    target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );
                    if ( data_id_is_valid( &dragged_classifier ) && ( NULL != target_card ) )
                    {
                        if ( DATA_TABLE_CLASSIFIER == data_id_get_table( &dragged_classifier ) )
                        {
                            /* create a feature */
                            /* propose a list_order for the feature */
                            int32_t list_order_proposal = 0;
                            list_order_proposal = gui_sketch_card_get_highest_list_order( target_card ) + 1024;

                            int64_t new_feature_id;
                            ctrl_error_t ctrl_err;
                            ctrl_err = gui_sketch_object_creator_create_feature ( &((*this_).object_creator),
                                                                                  data_id_get_row_id( &dragged_classifier ),
                                                                                  list_order_proposal,
                                                                                  &new_feature_id
                                                                                );

                            if ( CTRL_ERROR_NONE != ctrl_err )
                            {
                                TSLOG_ERROR("unexpected error at gui_sketch_object_creator_create_feature");
                            }
                            else
                            {
                                /* set focused object and notify listener */
                                data_id_t new_focused_id;
                                data_id_init( &new_focused_id, DATA_TABLE_FEATURE, new_feature_id );
                                gui_marker_set_focused( (*this_).marker, new_focused_id );
                                gui_sketch_area_private_notify_listener( this_, new_focused_id );
                                gui_marker_clear_selected_set( (*this_).marker );
                            }
                        }
                        else
                        {
                            TSLOG_WARNING("unexpected state at gui_sketch_area_button_release_callback");
                        }
                    }
                    else
                    {
                        TSLOG_WARNING("invalid clicked object at gui_sketch_area_button_release_callback");
                    }
                }
                else
                {
                    TRACE_INFO("according to drag-state, the button press was already handled at press-time");
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
        gui_sketch_drag_state_stop_dragging ( &((*this_).drag_state) );

        /* mark dirty rect */
        gtk_widget_queue_draw( widget );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_key_press_callback( GtkWidget* widget, GdkEventKey* evt, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;
    gboolean result_event_handled = false;

    if ( (*evt).state == GDK_CONTROL_MASK )
    {
        if ( (*evt).keyval == GDK_KEY_x )
        {
            TRACE_INFO ( "key pressed: Ctrl-X" );
            gui_tools_cut( (*this_).tools );
            result_event_handled = true;
        }
        else if ( (*evt).keyval == GDK_KEY_c )
        {
            TRACE_INFO ( "key pressed: Ctrl-C" );
            gui_tools_copy( (*this_).tools );
            result_event_handled = true;
        }
        else if ( (*evt).keyval == GDK_KEY_v )
        {
            TRACE_INFO ( "key pressed: Ctrl-V" );
            gui_tools_paste( (*this_).tools );
            result_event_handled = true;
        }
        /* other keys are out of scope */
    }
    else if ( (*evt).state == 0 )
    {
        if ( (*evt).keyval == GDK_KEY_Delete )
        {
            TRACE_INFO ( "key pressed: DEL" );
            gui_tools_delete( (*this_).tools );
            result_event_handled = true;
        }
        else if ( (*evt).keyval == GDK_KEY_F7 )
        {
            TRACE_INFO ( "key pressed: F7" );
            if ( GUI_SKETCH_TOOLS_NAVIGATE == gui_tools_get_selected_tool( (*this_).tools ) )
            {
                TRACE_INFO ( "entry point for shift-node-up" );
                /* todo */

                data_diagram_t *old_current;
                old_current = gui_sketch_card_get_diagram_ptr ( &((*this_).cards[0]) );

                int64_t main_diagram_id = data_diagram_get_id( old_current );
                TRACE_INFO_INT( "main_diagram_id:", main_diagram_id );
                if ( DATA_ID_VOID_ID != main_diagram_id )
                {
                    data_diagram_t *old_parent;
                    old_parent = gui_sketch_card_get_diagram_ptr ( &((*this_).cards[1]) );

                    int64_t parent_diagram_id = data_diagram_get_parent_id( old_current );
                    TRACE_INFO_INT( "parent_diagram_id:", parent_diagram_id );
                    if ( DATA_ID_VOID_ID != parent_diagram_id )
                    {

                        parent_diagram_id = data_diagram_get_id( old_parent );
                        TRACE_INFO_INT( "parent_diagram_id:", parent_diagram_id );
                        int64_t grandparent_diagram_id = data_diagram_get_parent_id( old_parent );
                        TRACE_INFO_INT( "grandparent_diagram_id:", grandparent_diagram_id );

                        ctrl_diagram_controller_t *diag_control;
                        diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                        ctrl_error_t c_err;
                        c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control,
                                                                                  main_diagram_id,
                                                                                  grandparent_diagram_id,
                                                                                  false /* add_to_latest_undo_set */
                                                                                );
                        if ( CTRL_ERROR_NONE != c_err )
                        {
                            TSLOG_ERROR_HEX( "CTRL_ERROR_NONE !=", c_err );
                        }
                        c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control,
                                                                                  parent_diagram_id,
                                                                                  main_diagram_id,
                                                                                  true /* add_to_latest_undo_set */
                                                                                );
                        if ( CTRL_ERROR_NONE != c_err )
                        {
                            TSLOG_ERROR_HEX( "CTRL_ERROR_NONE !=", c_err );
                        }
                    }
                    else
                    {
                        /* current diagram is root */
                        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                 GUI_SIMPLE_MESSAGE_CONTENT_ROOT_CANNOT_MOVE
                                                               );
                    }
                }
                else
                {
                    /* current diagram is invalid */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_ROOT_CANNOT_MOVE
                                                           );
                }
            }
        }
        /* other keys are out of scope */
    }
    else
    {
        /* other states are out of scope */
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return result_event_handled;
}

void gui_sketch_area_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;

    /* load/reload data to be drawn */
    gui_sketch_area_private_reload_data( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( widget );

    TRACE_END();
}

void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_tools_tool_t tool, gpointer data )
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
    gui_sketch_area_private_reload_data( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( widget );

    TRACE_END();
}

void gui_sketch_area_private_notify_listener( gui_sketch_area_t *this_, data_id_t modified_real_object_id )
{
    TRACE_BEGIN();

    for ( int index = 0; index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS; index ++ )
    {
        if ( (*this_).listener != NULL )
        {
            TRACE_INFO_INT( "g_signal_emit to listener", index );
            g_signal_emit( (*this_).listener[index], gui_sketch_area_glib_signal_id, 0, &modified_real_object_id );
        }
    }

    TRACE_END();
}


/*
Copyright 2016-2018 Andreas Warnke

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

