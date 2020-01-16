/* File: gui_sketch_area.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_area.h"
#include "gui_sketch_card_layouter.h"
#include "gui_tool.h"
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
                           GtkWidget *drawing_area,
                           gui_marked_set_t *marker,
                           gui_toolbox_t *toolbox,
                           gui_simple_message_to_user_t *message_to_user,
                           gui_resources_t *resources,
                           ctrl_controller_t *controller,
                           data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != drawing_area );
    assert( NULL != marker );
    assert( NULL != toolbox );
    assert( NULL != message_to_user );
    assert( NULL != resources );
    assert( NULL != controller );
    assert( NULL != db_reader );

    /* init pointers to external objects */
    (*this_).drawing_area = drawing_area;
    (*this_).toolbox = toolbox;
    (*this_).message_to_user = message_to_user;
    (*this_).resources = resources;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;

    /* init instances of own objects */
    (*this_).card_num = 0;
    (*this_).marker = marker;
    gui_sketch_nav_tree_init( &((*this_).nav_tree), resources );
    gui_sketch_result_list_init( &((*this_).result_list) );
    gui_sketch_drag_state_init ( &((*this_).drag_state) );
    gui_sketch_overlay_init( &((*this_).overlay) );
    gui_sketch_background_init( &((*this_).background), resources );
    gui_sketch_object_creator_init ( &((*this_).object_creator), controller, db_reader, message_to_user );

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

    /* destroy instances of own objects */
    gui_sketch_object_creator_destroy ( &((*this_).object_creator) );
    gui_sketch_overlay_destroy( &((*this_).overlay) );
    gui_sketch_background_destroy( &((*this_).background) );
    gui_sketch_drag_state_destroy ( &((*this_).drag_state) );

    /* unset pointers to external objects */
    (*this_).drawing_area = NULL;
    (*this_).marker = NULL;
    (*this_).toolbox = NULL;
    (*this_).message_to_user = NULL;
    (*this_).resources = NULL;
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    TRACE_END();
}

void gui_sketch_area_show_result_list ( gui_sketch_area_t *this_, data_small_set_t *list_of_diagrams )
{
    TRACE_BEGIN();
    assert( NULL != list_of_diagrams );

    gui_sketch_result_list_load_data( &((*this_).result_list), list_of_diagrams, (*this_).db_reader );

    /* DEMO */
    if ( data_small_set_get_count( list_of_diagrams ) > 0 )
    {
        data_id_t show_id;
        show_id = data_small_set_get_id( list_of_diagrams, 0 );
        gui_marked_set_set_focused_diagram( (*this_).marker, data_id_get_row_id(&show_id) );
        gui_sketch_area_private_load_data ( this_, data_id_get_row_id(&show_id) );
    }

    TRACE_END();
}

gboolean gui_sketch_area_draw_callback( GtkWidget *widget, cairo_t *cr, gpointer data )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    guint width;
    guint height;
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );

    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);

    if ( ! data_database_reader_is_open( (*this_).db_reader ) )
    {
        shape_int_rectangle_t bounds;
        shape_int_rectangle_init( &bounds, 0, 0, width, height );
        gui_sketch_background_set_bounds( &((*this_).background), bounds );
        gui_sketch_background_draw_introduction( &((*this_).background), cr );
    }
    else
    {
        shape_int_rectangle_t bounds;
        shape_int_rectangle_init( &bounds, 0, 0, width, height );
        if ( ! gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
        {
            gui_sketch_area_private_layout_subwidgets( this_, bounds, cr );
        }
        gui_sketch_area_private_draw_subwidgets( this_, bounds, cr );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return FALSE;
}

void gui_sketch_area_private_load_data ( gui_sketch_area_t *this_, int64_t main_diagram_id )
{
    TRACE_BEGIN();
    data_error_t db_err;

    /* destroy _all_ old cards */
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
    gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]) );
    gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), main_diagram_id, (*this_).db_reader );
    (*this_).card_num = 1;
    gui_sketch_nav_tree_load_data( &((*this_).nav_tree), main_diagram_id, (*this_).db_reader );
    gui_sketch_result_list_invalidate_data( &((*this_).result_list) );
    gui_marked_set_set_focused_diagram( (*this_).marker, main_diagram_id );

    gui_tool_t selected_tool;
    selected_tool = gui_toolbox_get_selected_tool( (*this_).toolbox );
    if (( GUI_TOOLBOX_NAVIGATE == selected_tool ) || ( GUI_TOOLBOX_SEARCH == selected_tool ))
    {
        /* determine ids */
        int64_t selected_diagram_id;
        int64_t parent_diagram_id;
        data_diagram_t *selected_diag;
        selected_diag = gui_sketch_area_get_focused_diagram_ptr( this_ );
        selected_diagram_id = data_diagram_get_id( selected_diag );
        TRACE_INFO_INT( "selected_diagram_id:", selected_diagram_id );
        parent_diagram_id = data_diagram_get_parent_id( selected_diag );
        TRACE_INFO_INT( "parent_diagram_id:", parent_diagram_id );

        /* load parent even if there is no parent (-->VOID) */
        gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]) );
        gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]), parent_diagram_id, (*this_).db_reader );
        (*this_).card_num = 2;

        /* load all children (up to GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS)*/
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
                if ( (*this_).card_num < GUI_SKETCH_AREA_CONST_MAX_CARDS )
                {
                    int64_t current_child_id;
                    current_child_id = data_diagram_get_id( &((*this_).private_temp_diagram_buf[index]) );
                    gui_sketch_card_init( &((*this_).cards[(*this_).card_num]) );
                    gui_sketch_card_load_data( &((*this_).cards[(*this_).card_num]), current_child_id, (*this_).db_reader );
                    (*this_).card_num ++;
                    /* cleanup */
                    data_diagram_destroy( &((*this_).private_temp_diagram_buf[index]) );
                }
                else
                {
                    TSLOG_ERROR_INT( "more diagrams loaded than fit into cards array!", c_count );
                }
            }
        }
    }

    TRACE_END();
}

void gui_sketch_area_private_refocus_and_reload_data ( gui_sketch_area_t *this_ )
{
    TRACE_BEGIN();

    /* determine currently selected diagram id and parent id from cache for emergency-fallback */
    int64_t former_diagram_id;
    int64_t former_parent_diagram_id;
    //data_id_t former_focused_element;
    {
        data_diagram_t *former_diagram;
        former_diagram = gui_sketch_area_get_focused_diagram_ptr ( this_ );
        former_diagram_id = data_diagram_get_id( former_diagram );
        former_parent_diagram_id = data_diagram_get_parent_id( former_diagram );
        TRACE_INFO_INT_INT( "former_diagram_id, former_parent_diagram_id:", former_diagram_id, former_parent_diagram_id );
        //former_focused_element = gui_marked_set_get_focused( (*this_).marker );
    }

    /* reload diagram data */
    gui_sketch_area_private_load_data( this_, former_diagram_id );

    if (( DATA_ID_VOID_ID != former_diagram_id )
        &&( DATA_ID_VOID_ID == gui_sketch_area_get_focused_diagram_id( this_ ) ))
    {
        /* the requested diagram was not loaded, try the parent: */
        gui_sketch_area_private_load_data( this_, former_parent_diagram_id );

        if (( DATA_ID_VOID_ID != former_parent_diagram_id )
            &&( DATA_ID_VOID_ID == gui_sketch_area_get_focused_diagram_id( this_ ) ))
        {
            /* the requested diagram was not loaded, go back to root diagram: */
            gui_sketch_area_private_load_data( this_, DATA_ID_VOID_ID );
        }

        /* clear the selected set */
        gui_marked_set_clear_selected_set( (*this_).marker );
    }
    else
    {
        /* restore the focused element */
        //vi gui_marked_set_set_focused( (*this_).marker, former_focused_element );
    }

    TRACE_END();
}

static const uint32_t NAV_TREE_WIDTH = 224;
static const uint32_t RESULT_LIST_WIDTH = 192;

void gui_sketch_area_private_layout_subwidgets ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert((*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS);

    gui_tool_t selected_tool;
    selected_tool = gui_toolbox_get_selected_tool( (*this_).toolbox );

    /* fetch area bounds */
    const uint32_t width = shape_int_rectangle_get_width( &area_bounds );
    const uint32_t height = shape_int_rectangle_get_height( &area_bounds );
    const int32_t left = shape_int_rectangle_get_left( &area_bounds );
    const int32_t top = shape_int_rectangle_get_top( &area_bounds );
    TRACE_INFO_INT_INT( "width, height", width, height );

    /* layout result list */
    const bool result_list_visible = ( GUI_TOOLBOX_SEARCH == selected_tool );
    {
        shape_int_rectangle_t result_list_bounds;
        shape_int_rectangle_init( &result_list_bounds, left, top, RESULT_LIST_WIDTH, height );
        gui_sketch_result_list_set_bounds( &((*this_).result_list ), result_list_bounds );
        gui_sketch_result_list_set_visible( &((*this_).result_list), result_list_visible );
    }

    /* layout nav tree */
    const bool nav_tree_visible = ( GUI_TOOLBOX_NAVIGATE == selected_tool );
    {
        shape_int_rectangle_t nav_tree_bounds;
        shape_int_rectangle_init( &nav_tree_bounds, left, top, NAV_TREE_WIDTH, height );
        gui_sketch_nav_tree_set_bounds( &((*this_).nav_tree), nav_tree_bounds );
        gui_sketch_nav_tree_set_visible( &((*this_).nav_tree), nav_tree_visible );
    }

    /* layout cards */
    const int32_t cards_left = left + (nav_tree_visible ? NAV_TREE_WIDTH : (result_list_visible ? RESULT_LIST_WIDTH : 0 ));
    const uint32_t cards_width = (width > (cards_left-left)) ? (width - (cards_left-left)) : 0;
    {
        shape_int_rectangle_t cards_bounds;
        shape_int_rectangle_init( &cards_bounds, cards_left, top, cards_width, height );
        gui_sketch_card_layouter_t cards_layouter;
        gui_sketch_card_layouter_init ( &cards_layouter, &cards_bounds );
        gui_sketch_card_layouter_layout ( &cards_layouter, selected_tool, &((*this_).cards[0]), (*this_).card_num, cr );
        gui_sketch_card_layouter_destroy ( &cards_layouter );
    }

    /* layout background */
    {
        shape_int_rectangle_t background_bounds;
        shape_int_rectangle_init( &background_bounds, cards_left, top, cards_width, height );
        gui_sketch_background_set_bounds( &((*this_).background), background_bounds );
    }

    TRACE_END();
}

void gui_sketch_area_private_draw_subwidgets ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    gui_tool_t selected_tool;
    selected_tool = gui_toolbox_get_selected_tool( (*this_).toolbox );

    /* draw background */
    switch( selected_tool )
    {
        case GUI_TOOLBOX_SEARCH:
        {
            gui_sketch_background_draw_search( &((*this_).background), cr );
        }
        break;

        case GUI_TOOLBOX_NAVIGATE:
        {
            unsigned int depth;
            unsigned int children;
            depth = ( gui_sketch_card_is_valid( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]) ) ) ? 1 : 0;  /* currently, only root and non-root can be distinguished */
            children = (*this_).card_num-2;  /* concept of card numbers to be updated in the future */
            gui_sketch_background_draw_navigation( &((*this_).background),
                                                   depth, children,
                                                   cr
                                                 );
        }
        break;

        case GUI_TOOLBOX_EDIT:
        {
            gui_sketch_background_draw_edit( &((*this_).background), cr );
        }
        break;

        case GUI_TOOLBOX_CREATE:
        {
            gui_sketch_background_draw_create( &((*this_).background), cr );
        }
        break;

        default:
        {
            assert(false);
        }
        break;
    }

    /* draw result list */
    {
        gui_sketch_result_list_draw( &((*this_).result_list), (*this_).marker, cr );
    }

    /* draw nav tree */
    {
        gui_sketch_nav_tree_draw( &((*this_).nav_tree), (*this_).marker, cr );
    }

    /* draw all cards, backwards */
    for ( signed int card_idx = (*this_).card_num-1; card_idx >= 0; card_idx -- )
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
                             &((*this_).nav_tree),
                             cr
                           );

    TRACE_END();
}

gboolean gui_sketch_area_leave_notify_callback( GtkWidget* widget, GdkEventCrossing* evt, gpointer data )
{
    TRACE_BEGIN();
    assert( NULL != evt );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );

    if (( (*evt).type == GDK_LEAVE_NOTIFY )&&( (*evt).mode == GDK_CROSSING_NORMAL )) {

        gui_marked_set_clear_highlighted( (*this_).marker );
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
    assert( NULL != evt );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );

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
    gui_tool_t selected_tool;
    selected_tool = gui_toolbox_get_selected_tool( (*this_).toolbox );
    switch ( selected_tool )
    {
        case GUI_TOOLBOX_NAVIGATE:
        {
            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* always redraw while dragging */
                gtk_widget_queue_draw( widget );
            }
            else /* not dragging */
            {
                data_id_t object_under_mouse;
                object_under_mouse = gui_sketch_area_get_diagram_id_at_pos ( this_, x, y );
                data_id_t object_highlighted;
                object_highlighted = gui_marked_set_get_highlighted( (*this_).marker );
                if ( ! data_id_equals( &object_under_mouse, &object_highlighted ) )
                {
                    if ( data_id_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                    {
                        gui_marked_set_set_highlighted( (*this_).marker, object_under_mouse );

                        /* mark dirty rect */
                        gtk_widget_queue_draw( widget );
                    }
                }
            }
        }
        break;

        case GUI_TOOLBOX_EDIT:
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
                gui_marked_set_set_highlighted( (*this_).marker, dragged_element );

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
            else /* not dragging */
            {
                data_id_pair_t object_under_mouse;
                gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, PENCIL_TYPE_FILTER_LIFELINE, &object_under_mouse );
                data_id_t object_highlighted;
                object_highlighted = gui_marked_set_get_highlighted( (*this_).marker );
                if ( ! data_id_equals( data_id_pair_get_primary_id_ptr( &object_under_mouse ), &object_highlighted ) )
                {
                    if ( data_id_pair_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                    {
                        gui_marked_set_set_highlighted( (*this_).marker, data_id_pair_get_primary_id ( &object_under_mouse ) );

                        /* mark dirty rect */
                        gtk_widget_queue_draw( widget );
                    }
                }
            }
        }
        break;

        case GUI_TOOLBOX_SEARCH:
        {
        }
        break;

        case GUI_TOOLBOX_CREATE:
        {
            data_id_pair_t object_under_mouse;
            gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, PENCIL_TYPE_FILTER_NONE, &object_under_mouse );
            data_id_t  classifier_under_mouse;
            classifier_under_mouse = data_id_pair_get_primary_id( &object_under_mouse );

            data_id_t object_highlighted;
            object_highlighted = gui_marked_set_get_highlighted( (*this_).marker );
            if ( ! data_id_equals( &classifier_under_mouse, &object_highlighted ) )
            {
                if ( data_id_is_valid( &classifier_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                {
                    gui_marked_set_set_highlighted( (*this_).marker, classifier_under_mouse );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( widget );
                }
            }
            else if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* always redraw while dragging */
                gtk_widget_queue_draw( widget );
            }
            else
            {
                /* always redraw while moving the mouse to move the new-box-and-arrow icon */
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
    assert( NULL != evt );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );

    /* in general, hide the last message */
    gui_simple_message_to_user_hide( (*this_).message_to_user );

    if ( (*evt).button == 1 ) {
        TRACE_INFO("press");

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
        gui_tool_t selected_tool;
        selected_tool = gui_toolbox_get_selected_tool( (*this_).toolbox );
        switch ( selected_tool )
        {
            case GUI_TOOLBOX_NAVIGATE:
            {
                TRACE_INFO( "GUI_TOOLBOX_NAVIGATE" );

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
                }
                else
                {
                    TRACE_INFO("invalid clicked object at gui_sketch_area_button_press_callback");

                    gui_sketch_action_t action_button_id;
                    gui_sketch_nav_tree_get_button_at_pos ( &((*this_).nav_tree), x, y, &action_button_id );
                    if ( action_button_id != GUI_SKETCH_ACTION_NONE )
                    {
                        /* create a new diagram */
                        ctrl_error_t c_result;
                        int64_t new_diag_id;

                        {
                            const data_diagram_t *selected_diag;
                            selected_diag = gui_sketch_nav_tree_get_diagram_ptr ( &((*this_).nav_tree) );
                            switch ( action_button_id )
                            {
                                case GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM:
                                {
                                    assert( data_diagram_is_valid( selected_diag ) );
                                    int64_t parent_diagram_id;
                                    parent_diagram_id = data_diagram_get_parent_id( selected_diag );
                                    int32_t list_order;
                                    list_order = gui_sketch_nav_tree_get_siblings_highest_order ( &((*this_).nav_tree) );


                                    c_result = gui_sketch_object_creator_create_diagram ( &((*this_).object_creator),
                                                                                          parent_diagram_id,
                                                                                          list_order + 32768,
                                                                                          &new_diag_id
                                                                                        );
                                }
                                break;

                                case GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM:
                                {
                                    assert( data_diagram_is_valid( selected_diag ) );
                                    int64_t selected_diagram_id;
                                    selected_diagram_id = data_diagram_get_id( selected_diag );
                                    int32_t list_order;
                                    list_order = gui_sketch_nav_tree_get_children_highest_order ( &((*this_).nav_tree) );
                                    c_result = gui_sketch_object_creator_create_diagram ( &((*this_).object_creator),
                                                                                          selected_diagram_id,
                                                                                          list_order + 32768,
                                                                                          &new_diag_id
                                                                                        );
                                }
                                break;

                                case GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM:
                                {
                                    c_result = gui_sketch_object_creator_create_diagram ( &((*this_).object_creator),
                                                                                          DATA_ID_VOID_ID,
                                                                                          0,
                                                                                          &new_diag_id
                                                                                        );
                                }
                                break;

                                case GUI_SKETCH_ACTION_NONE:  /* and */
                                default:
                                {
                                    TSLOG_ERROR_INT("illegal action value in gui_sketch_action_t:",action_button_id);
                                    assert(false);
                                }
                                break;
                            }
                        }

                        if ( CTRL_ERROR_NONE != c_result )
                        {
                            TSLOG_ERROR("unexpected error at gui_sketch_object_creator_create_diagram");
                        }
                        else
                        {
                            /* load/reload data to be drawn */
                            gui_sketch_area_private_load_data( this_, new_diag_id );

                            /* notify listener */
                            data_id_t focused_id;
                            data_id_init( &focused_id, DATA_TABLE_DIAGRAM, new_diag_id );
                            gui_marked_set_set_focused( (*this_).marker, focused_id );
                            gui_sketch_area_private_notify_listeners( this_, focused_id );
                            gui_marked_set_clear_selected_set( (*this_).marker );
                        }
                    }
                }
            }
            break;

            case GUI_TOOLBOX_EDIT:
            {
                TRACE_INFO( "GUI_TOOLBOX_EDIT" );

                /* determine the focused object */
                data_id_pair_t focused_object;
                gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, PENCIL_TYPE_FILTER_LIFELINE, &focused_object );
                data_id_pair_trace( &focused_object );
                data_id_t focused_object_visible;
                focused_object_visible = data_id_pair_get_primary_id( &focused_object );

                /* update drag state */
                gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), focused_object );

                /* which object is currently focused? */
                data_id_t focused_visible_before;
                focused_visible_before = gui_marked_set_get_focused ( (*this_).marker );

                if ( data_id_equals ( &focused_object_visible, &focused_visible_before ) )
                {
                    /* the clicked object is already focused */
                    gui_marked_set_toggle_selected_obj( (*this_).marker, focused_object_visible );
                }
                else
                {
                    /* store focused object and notify listener */
                    gui_marked_set_set_focused ( (*this_).marker,
                                                    focused_object_visible
                                                  );

                    data_id_t real_object = focused_object_visible;
                    if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &real_object ) )
                    {
                        real_object = data_id_pair_get_secondary_id( &focused_object );
                    }
                    gui_sketch_area_private_notify_listeners( this_, real_object );
                }

                /* mark dirty rect */
                gtk_widget_queue_draw( widget );
            }
            break;

            case GUI_TOOLBOX_SEARCH:
            {
                TRACE_INFO( "GUI_TOOLBOX_SEARCH" );
            }
            break;

            case GUI_TOOLBOX_CREATE:
            {
                TRACE_INFO( "GUI_TOOLBOX_CREATE" );

                /* what is the target location? */
                gui_sketch_card_t *target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );

                if ( NULL == target_card )
                {
                    TRACE_INFO_INT_INT("No card at",x,y);

                    /* if this happens, invalidate the marked object. */
                    gui_marked_set_clear_focused( (*this_).marker );
                }
                else
                {
                    /* determine the object at click location */
                    data_id_pair_t clicked_object;
                    data_id_pair_t surrounding_object;
                    gui_sketch_area_private_get_object_ids_at_pos ( this_, x, y, PENCIL_TYPE_FILTER_NONE, &clicked_object, &surrounding_object );
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
                        gui_marked_set_set_focused( (*this_).marker,
                                                       data_id_pair_get_primary_id( &clicked_object )
                                                     );
                        gui_sketch_area_private_notify_listeners( this_, data_id_pair_get_secondary_id( &clicked_object ) );
                    }
                    else /* clicked either into inner space of a classifier or at a relation or outside any classifier */
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
                            gui_marked_set_set_focused( (*this_).marker, focused_id );
                            gui_sketch_area_private_notify_listeners( this_, focused_real_id );
                            gui_marked_set_clear_selected_set( (*this_).marker );

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

        gtk_widget_grab_focus( widget );  /* causes the text edit widgets to lose the focus */
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return TRUE;
}

gboolean gui_sketch_area_button_release_callback( GtkWidget* widget, GdkEventButton* evt, gpointer data )
{
    TRACE_BEGIN();
    assert( NULL != evt );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );

    if ( (*evt).button == 1 ) {
        TRACE_INFO("release");

        /* get position */
        int32_t x;
        int32_t y;
        x = (int32_t) (*evt).x;
        y = (int32_t) (*evt).y;
        TRACE_INFO_INT_INT("x/y",x,y);

        /* update drag coordinates */
        gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

        /* check that drag state is true */
        if ( ( ! gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
            && ( ! gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
        {
            TRACE_INFO("drag state indicates no dragging and no waiting - but button was pressed before!");
        }

        /* do action */
        gui_tool_t selected_tool;
        selected_tool = gui_toolbox_get_selected_tool( (*this_).toolbox );
        switch ( selected_tool )
        {
            case GUI_TOOLBOX_NAVIGATE:
            {
                TRACE_INFO("GUI_TOOLBOX_NAVIGATE");
                if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
                {
                    /* which diagram was dragged? */
                    data_id_pair_t *dragged_object;
                    dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                    data_id_t dragged_diagram;
                    dragged_diagram = data_id_pair_get_primary_id( dragged_object );

                    /* to which diagram-gap was it dragged to? */
                    gui_error_t gui_err;
                    data_id_t target_parent_id;
                    int32_t target_list_order;
                    shape_int_rectangle_t dummy_gap_marker_border;
                    gui_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &((*this_).nav_tree),
                                                                        x,
                                                                        y,
                                                                        &target_parent_id,
                                                                        &target_list_order,
                                                                        &dummy_gap_marker_border
                                                                      );

                    if (( DATA_TABLE_DIAGRAM == data_id_get_table( &dragged_diagram ) )
                        && ( DATA_TABLE_DIAGRAM == data_id_get_table( &target_parent_id ) )
                        && ( GUI_ERROR_NONE == gui_err ))
                    {
                        TRACE_INFO_INT( "dragged_diagram:", data_id_get_row_id( &dragged_diagram ) );
                        TRACE_INFO_INT( "target_parent_id:", data_id_get_row_id( &target_parent_id ) );
                        TRACE_INFO_INT( "target_list_order:", target_list_order );
                        bool is_descendant;
                        bool is_self;
                        gui_error_t not_found;
                        not_found = gui_sketch_nav_tree_is_descendant ( &((*this_).nav_tree),
                                                                        data_id_get_row_id( &dragged_diagram ),
                                                                        data_id_get_row_id( &target_parent_id ),
                                                                        &is_descendant
                                                                      );
                        is_self = ( data_id_get_row_id( &dragged_diagram ) == data_id_get_row_id( &target_parent_id ) );
                        if ( ( ! is_self ) && ( not_found == GUI_ERROR_NONE ) && ( ! is_descendant ) )
                        {
                            ctrl_diagram_controller_t *diag_control;
                            diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                            ctrl_error_t c_err;
                            c_err = ctrl_diagram_controller_update_diagram_list_order( diag_control,
                                                                                       data_id_get_row_id( &dragged_diagram ),
                                                                                       target_list_order
                                                                                     );
                            if ( CTRL_ERROR_NONE != c_err )
                            {
                                TSLOG_ERROR_HEX( "CTRL_ERROR_NONE !=", c_err );
                            }
                            c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control,
                                                                                      data_id_get_row_id( &dragged_diagram ),
                                                                                      data_id_get_row_id( &target_parent_id ),
                                                                                      true /* add_to_latest_undo_set */
                                                                                    );
                            if ( CTRL_ERROR_NONE != c_err )
                            {
                                TSLOG_ERROR_HEX( "CTRL_ERROR_NONE !=", c_err );
                            }
                        }
                        else if ( DATA_ID_VOID_ID == data_id_get_row_id( &target_parent_id ) )
                        {
                            /* a diagram is dragged to the root location */
                            ctrl_diagram_controller_t *diag_control2;
                            diag_control2 = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                            int64_t root_id;
                            root_id = gui_sketch_nav_tree_get_root_diagram_id ( &((*this_).nav_tree) );
                            if (( root_id != DATA_ID_VOID_ID )&&( root_id != data_id_get_row_id( &dragged_diagram ) ))
                            {
                                ctrl_error_t c_err;
                                c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control2,
                                                                                          data_id_get_row_id( &dragged_diagram ),
                                                                                          DATA_ID_VOID_ID,
                                                                                          false /* add_to_latest_undo_set */
                                                                                        );
                                if ( CTRL_ERROR_NONE != c_err )
                                {
                                    TSLOG_ERROR_HEX( "CTRL_ERROR_NONE !=", c_err );
                                }
                                c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control2,
                                                                                          root_id,
                                                                                          data_id_get_row_id( &dragged_diagram ),
                                                                                          true /* add_to_latest_undo_set */
                                                                                        );
                                if ( CTRL_ERROR_NONE != c_err )
                                {
                                    TSLOG_ERROR_HEX( "CTRL_ERROR_NONE !=", c_err );
                                }
                            }
                            else
                            {
                                TSLOG_WARNING("dragging a diagram to the root location but no root exists or dragged diagram is root?");
                            }
                        }
                        else
                        {
                            TSLOG_WARNING("diagram dragging to invalid target location");
                            /* current diagram is root */
                            gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_ANCESTOR_IS_NOT_DESCENDANT
                                                                   );
                        }
                    }
                }
                else if ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) )
                {
                    /* click on diagram without drag */
                    data_id_pair_t *dragged_object;
                    dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                    data_id_t dragged_diagram;
                    dragged_diagram = data_id_pair_get_primary_id( dragged_object );
                    if ( DATA_TABLE_DIAGRAM == data_id_get_table( &dragged_diagram ) )
                    {
                        const int64_t drag_id = data_id_get_row_id( &dragged_diagram );
                        const int64_t focus_id = gui_sketch_area_get_focused_diagram_id( this_ );
                        if ( ( focus_id != DATA_ID_VOID_ID )&&( focus_id == drag_id ) )
                        {
                            /* if clicked diagram is already the focused diagram, switch to edit mode */
                            gui_toolbox_set_selected_tool( (*this_).toolbox, GUI_TOOLBOX_EDIT );
                        }
                        else
                        {
                            /* load/reload data to be drawn */
                            gui_sketch_area_private_load_data( this_, drag_id );

                            /* notify listener */
                            gui_marked_set_set_focused( (*this_).marker, dragged_diagram );
                            gui_sketch_area_private_notify_listeners( this_, dragged_diagram );
                            gui_marked_set_clear_selected_set( (*this_).marker );

                            /* mark dirty rect */
                            gtk_widget_queue_draw( widget );
                        }
                    }
                    else
                    {
                        TSLOG_ANOMALY("GUI_TOOLBOX_NAVIGATE released mouse button but not a diagram clicked before");
                    }
                }
            }
            break;

            case GUI_TOOLBOX_EDIT:
            {
                TRACE_INFO("GUI_TOOLBOX_EDIT");

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
                            if ( CTRL_ERROR_NONE != mov_result )
                            {
                                TSLOG_ERROR( "changing order failed: ctrl_classifier_controller_update_classifier_list_order" );
                            }
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
                            if ( CTRL_ERROR_NONE != mov_result )
                            {
                                TSLOG_ERROR( "changing order failed: ctrl_classifier_controller_update_classifier_x_order_y_order" );
                            }
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
                            if ( CTRL_ERROR_NONE != mov_result )
                            {
                                TSLOG_ERROR( "changing order failed: ctrl_classifier_controller_update_relationship_list_order" );
                            }
                        }
                    }
                    else if ( DATA_TABLE_FEATURE == data_id_get_table( &dragged_element ) )
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
                            mov_result = ctrl_classifier_controller_update_feature_list_order ( classifier_control,
                                                                                                data_id_get_row_id( &dragged_element ),
                                                                                                list_order
                                                                                              );
                            if ( CTRL_ERROR_NONE != mov_result )
                            {
                                TSLOG_ERROR( "changing order failed: ctrl_classifier_controller_update_feature_list_order" );
                            }
                        }
                    }
                    else
                    {
                        TRACE_INFO("Dragged object is neither relationship nor classifier nor feature");
                    }
                }
            }
            break;

            case GUI_TOOLBOX_SEARCH:
            {
                TRACE_INFO("GUI_TOOLBOX_SEARCH");
            }
            break;

            case GUI_TOOLBOX_CREATE:
            {
                TRACE_INFO("GUI_TOOLBOX_CREATE");

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
                    gui_sketch_area_private_get_object_id_at_pos ( this_, x, y, PENCIL_TYPE_FILTER_NONE, &destination_object );
                    data_id_pair_trace( &destination_object );
                    data_id_t destination_element;
                    destination_element = data_id_pair_get_primary_id( &destination_object );
                    data_id_t destination_classifier;
                    destination_classifier = data_id_pair_get_secondary_id( &destination_object );

                    gui_sketch_card_t *target_card = gui_sketch_area_get_card_at_pos ( this_, x, y );
                    if ( data_id_is_valid( &dragged_classifier ) && data_id_is_valid( &destination_classifier ) && ( NULL != target_card ))
                    {
                        if ( ( DATA_TABLE_CLASSIFIER == data_id_get_table( &dragged_classifier ) )
                            && ( DATA_TABLE_CLASSIFIER == data_id_get_table( &destination_classifier ) ) )
                        {
                            /* get the diagram type */
                            const data_diagram_t *target_diag = gui_sketch_card_get_diagram_ptr ( target_card );
                            assert ( target_diag != NULL );
                            data_diagram_type_t diag_type = data_diagram_get_diagram_type ( target_diag );

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
                            {
                                /* propose a list order */
                                layout_order_t layout_order;
                                data_id_t fake_relationship;
                                data_id_init ( &fake_relationship, DATA_TABLE_RELATIONSHIP, DATA_ID_VOID_ID );
                                layout_order = gui_sketch_card_get_order_at_pos( target_card, fake_relationship, x, y );
                                if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                                {
                                    list_order_proposal = layout_order_get_first( &layout_order );
                                }
                                else  /* PENCIL_LAYOUT_ORDER_TYPE_NONE */
                                {
                                    list_order_proposal = gui_sketch_card_get_highest_rel_list_order( target_card ) + 32768;
                                }
                            }

                            int64_t new_relationship_id;
                            ctrl_error_t c_result;
                            c_result = gui_sketch_object_creator_create_relationship ( &((*this_).object_creator),
                                                                                       diag_type,
                                                                                       new_from_classifier_id,
                                                                                       new_from_feature_id,
                                                                                       new_to_classifier_id,
                                                                                       new_to_feature_id,
                                                                                       list_order_proposal,
                                                                                       &new_relationship_id
                                                                                     );

                            if ( CTRL_ERROR_NONE != c_result )
                            {
                                TSLOG_ANOMALY_HEX("anomaly at gui_sketch_object_creator_create_relationship",c_result);
                            }
                            else
                            {
                                /* set focused object and notify listener */
                                data_id_t focused_id;
                                data_id_init( &focused_id, DATA_TABLE_RELATIONSHIP, new_relationship_id );
                                gui_marked_set_set_focused( (*this_).marker, focused_id );
                                gui_sketch_area_private_notify_listeners( this_, focused_id );
                                gui_marked_set_clear_selected_set( (*this_).marker );
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
                            /* get the diagram type */
                            const data_diagram_t *target_diag = gui_sketch_card_get_diagram_ptr ( target_card );
                            assert ( target_diag != NULL );
                            data_diagram_type_t diag_type = data_diagram_get_diagram_type ( target_diag );

                            /* determine id of classifier to which the clicked object belongs */
                            const int64_t classifier_id = data_id_get_row_id( &dragged_classifier );

                            /* propose a list_order for the feature */
                            int32_t std_list_order_proposal = 0;
                            std_list_order_proposal = gui_sketch_card_get_highest_feat_list_order( target_card, classifier_id ) + 32768;
                            int32_t port_list_order_proposal = 0;
                            {
                                data_feature_init ( &((*this_).private_temp_fake_feature),
                                                    DATA_ID_VOID_ID,
                                                    DATA_FEATURE_TYPE_PORT,
                                                    classifier_id, /* classifier */
                                                    "FAKE_FEATURE",
                                                    "port-type",
                                                    "to determine the list order",
                                                    0 /* list_order */
                                                );
                                port_list_order_proposal = gui_sketch_card_get_feature_order_at_pos ( target_card,
                                                                                                      &((*this_).private_temp_fake_feature),
                                                                                                      x,
                                                                                                      y
                                                                                                    );
                                data_feature_destroy ( &((*this_).private_temp_fake_feature) );
                            }

                            /* create a feature */
                            int64_t new_feature_id;
                            ctrl_error_t ctrl_err;
                            ctrl_err = gui_sketch_object_creator_create_feature ( &((*this_).object_creator),
                                                                                  diag_type,
                                                                                  classifier_id,
                                                                                  std_list_order_proposal,
                                                                                  port_list_order_proposal,
                                                                                  &new_feature_id
                                                                                );

                            if ( CTRL_ERROR_NONE != ctrl_err )
                            {
                                TSLOG_ANOMALY_HEX("anomaly at gui_sketch_object_creator_create_feature",ctrl_err);
                            }
                            else
                            {
                                /* set focused object and notify listener */
                                data_id_t new_focused_id;
                                data_id_init( &new_focused_id, DATA_TABLE_FEATURE, new_feature_id );
                                gui_marked_set_set_focused( (*this_).marker, new_focused_id );
                                gui_sketch_area_private_notify_listeners( this_, new_focused_id );
                                gui_marked_set_clear_selected_set( (*this_).marker );
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
    assert( NULL != evt );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );
    gboolean result_event_handled = false;

    /* keys that have to be handled locally in the gui_sketch_area */
    /* becasue handling them globally would interfere with text entry fields */
    if ( (*evt).state == GDK_CONTROL_MASK )
    {
        if ( (*evt).keyval == GDK_KEY_x )
        {
            TRACE_INFO ( "key pressed: Ctrl-X" );
            gui_toolbox_cut( (*this_).toolbox );
            result_event_handled = true;
        }
        else if ( (*evt).keyval == GDK_KEY_c )
        {
            TRACE_INFO ( "key pressed: Ctrl-C" );
            gui_toolbox_copy( (*this_).toolbox );
            result_event_handled = true;
        }
        else if ( (*evt).keyval == GDK_KEY_v )
        {
            TRACE_INFO ( "key pressed: Ctrl-V" );
            gui_toolbox_paste( (*this_).toolbox );
            result_event_handled = true;
        }
        /* other keys are out of scope */
    }
    else if ( (*evt).state == 0 )
    {
        if ( (*evt).keyval == GDK_KEY_Delete )
        {
            TRACE_INFO ( "key pressed: DEL" );
            gui_toolbox_delete( (*this_).toolbox );
            result_event_handled = true;
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
    assert( NULL != msg );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );
    assert ( NULL != widget );

    /* load/reload data to be drawn */
    gui_sketch_area_private_refocus_and_reload_data( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( (*this_).drawing_area );

    TRACE_END();
}

void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_tool_t tool, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );
    assert ( NULL != widget );

    switch ( tool )
    {
        case GUI_TOOLBOX_NAVIGATE:
        {
            TRACE_INFO("GUI_TOOLBOX_NAVIGATE");
        }
        break;

        case GUI_TOOLBOX_EDIT:
        {
            TRACE_INFO("GUI_TOOLBOX_EDIT");
        }
        break;

        case GUI_TOOLBOX_SEARCH:
        {
            TRACE_INFO("GUI_TOOLBOX_SEARCH");
        }
        break;

        case GUI_TOOLBOX_CREATE:
        {
            TRACE_INFO("GUI_TOOLBOX_CREATE");
        }
        break;

        default:
        {
            TSLOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    /* load/reload data to be drawn - depending on the tool, other data may be needed */
    gui_sketch_area_private_refocus_and_reload_data( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( (*this_).drawing_area );

    TRACE_END();
}

void gui_sketch_area_private_notify_listeners( gui_sketch_area_t *this_, data_id_t modified_real_object_id )
{
    TRACE_BEGIN();

    TRACE_INFO( "g_signal_emit to listeners" );
    g_signal_emit( (*this_).drawing_area, gui_sketch_area_glib_signal_id, 0, &modified_real_object_id );

    TRACE_END();
}


/*
Copyright 2016-2020 Andreas Warnke

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

