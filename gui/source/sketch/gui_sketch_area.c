/* File: gui_sketch_area.c; Copyright and License: see below */

#include "sketch/gui_sketch_area.h"
#include "gui_sketch_card_layouter.h"
#include "gui_tool.h"
#include "pencil_diagram_maker.h"
#include "geometry/geometry_rectangle.h"
#include "data_table.h"
#include "data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdint.h>
#include <stdbool.h>

void gui_sketch_area_init( gui_sketch_area_t *this_,
                           GtkWidget *drawing_area,
                           gui_marked_set_t *marker,
                           gui_toolbox_t *toolbox,
                           gui_simple_message_to_user_t *message_to_user,
                           gui_resources_t *resources,
                           ctrl_controller_t *controller,
                           data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
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

    /* init instance of requested tool-mode and diagram-ids */
    gui_sketch_request_init( &((*this_).request) );

    /* init instances of own objects */
    (*this_).card_num = 0;
    (*this_).marker = marker;
    gui_sketch_texture_init( &((*this_).texture_downloader) );
    gui_sketch_nav_tree_init( &((*this_).nav_tree), resources, &((*this_).texture_downloader) );
    gui_sketch_result_list_init( &((*this_).result_list), resources, &((*this_).texture_downloader) );
    gui_sketch_drag_state_init ( &((*this_).drag_state) );
    gui_sketch_overlay_init( &((*this_).overlay) );
    gui_sketch_background_init( &((*this_).background), resources, &((*this_).texture_downloader) );
    gui_sketch_object_creator_init ( &((*this_).object_creator), controller, db_reader, message_to_user );

    /* connect draw/update and mouse move and key signals to the controllers of this widget */
    gtk_drawing_area_set_draw_func( GTK_DRAWING_AREA((*this_).drawing_area),
                                    (GtkDrawingAreaDrawFunc) gui_sketch_area_draw_callback,
                                    this_,
                                    NULL
                                  );
    gtk_widget_set_sensitive( (*this_).drawing_area, true );
    gtk_widget_set_can_focus( (*this_).drawing_area, true );
    gtk_widget_set_focusable( (*this_).drawing_area, true );
    gtk_widget_set_can_target( (*this_).drawing_area, true );
    gtk_widget_set_focus_on_click( (*this_).drawing_area, true );

    GtkEventController *evt_move = gtk_event_controller_motion_new();
    g_signal_connect( evt_move, "enter", G_CALLBACK(gui_sketch_area_motion_notify_callback), this_ );
    g_signal_connect( evt_move, "motion", G_CALLBACK(gui_sketch_area_motion_notify_callback), this_ );
    g_signal_connect( evt_move, "leave", G_CALLBACK(gui_sketch_area_leave_notify_callback), this_ );
    gtk_widget_add_controller( (*this_).drawing_area, GTK_EVENT_CONTROLLER(evt_move) );

    GtkEventController *evt_key = gtk_event_controller_key_new();
    g_signal_connect( evt_key, "key-pressed", G_CALLBACK(gui_sketch_area_key_press_callback), this_ );
    /*
    g_signal_connect( evt_key, "modifiers", G_CALLBACK(gui_sketch_area_key_callback), this_ );
    g_signal_connect( evt_key, "key-released", G_CALLBACK(gui_sketch_area_key_release_callback), this_ );
    */
    gtk_widget_add_controller( (*this_).drawing_area, GTK_EVENT_CONTROLLER(evt_key) );

    GtkGesture *evt_button = gtk_gesture_click_new();
    g_signal_connect( evt_button, "pressed", G_CALLBACK(gui_sketch_area_button_press_callback), this_ );
    g_signal_connect( evt_button, "released", G_CALLBACK(gui_sketch_area_button_release_callback), this_ );
    gtk_widget_add_controller( (*this_).drawing_area, GTK_EVENT_CONTROLLER(evt_button) );

    /* fetch initial data from the database */
    gui_sketch_area_show_diagram( this_, DATA_ID_VOID );

    U8_TRACE_END();
}

void gui_sketch_area_destroy( gui_sketch_area_t *this_ )
{
    U8_TRACE_BEGIN();

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
    gui_sketch_texture_destroy( &((*this_).texture_downloader) );

    /* destroy instance of requested tool-mode and diagram-ids */
    gui_sketch_request_destroy( &((*this_).request) );

    /* unset pointers to external objects */
    (*this_).drawing_area = NULL;
    (*this_).marker = NULL;
    (*this_).toolbox = NULL;
    (*this_).message_to_user = NULL;
    (*this_).resources = NULL;
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    U8_TRACE_END();
}

void gui_sketch_area_show_result_list ( gui_sketch_area_t *this_, const data_search_result_list_t *result_list )
{
    U8_TRACE_BEGIN();
    assert( NULL != result_list );

    data_search_result_list_trace(result_list);

    /* copy non-duplicate diagram ids to request list */
    data_small_set_t* requested_diagrams = gui_sketch_request_get_search_result_diagrams_ptr( &((*this_).request) );
    data_small_set_clear( requested_diagrams );
    unsigned int dropped_duplicates = 0;
    unsigned int dropped_too_many = 0;
    const uint_fast32_t d_count = data_search_result_list_get_length( result_list );
    for ( uint_fast32_t index = 0; index < d_count; index ++ )
    {
        const data_search_result_t *diag_rec = data_search_result_list_get_const( result_list, index );
        const data_id_t diag_id = data_search_result_get_diagram_id( diag_rec );
        const u8_error_t d_err = data_small_set_add_obj( requested_diagrams, diag_id );
        if ( d_err == U8_ERROR_DUPLICATE_ID )
        {
            dropped_duplicates ++;
        }
        else if ( d_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED )
        {
            dropped_too_many ++;
        }
    }
    if ( (dropped_duplicates + dropped_too_many) > 0 )
    {
        U8_TRACE_INFO_INT_INT( "dropped_duplicates, dropped_too_many:", dropped_duplicates, dropped_too_many );
    }

    /* load new data */
    gui_sketch_area_private_load_cards_data ( this_ );

    /* load new data in subwidgets */
    gui_sketch_result_list_load_data( &((*this_).result_list), result_list, (*this_).db_reader );

    /* notify listener */
    gui_marked_set_clear_focused( (*this_).marker );
    gui_marked_set_clear_selected_set( (*this_).marker );

    /* mark dirty rect */
    gtk_widget_queue_draw( (*this_).drawing_area );

    U8_TRACE_END();
}

void gui_sketch_area_draw_callback( GtkDrawingArea *widget, cairo_t *cr, int width, int height, gpointer data )
{
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    assert( NULL != cr );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );
    assert( GTK_WIDGET(widget) == (*this_).drawing_area );

    gui_sketch_area_draw( this_, width, height, cr );

    U8_TRACE_END();
}

void gui_sketch_area_draw( gui_sketch_area_t *this_, int width, int height, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

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

    U8_TRACE_END();
}

void gui_sketch_area_show_diagram ( gui_sketch_area_t *this_, data_id_t main_diagram_id )
{
    U8_TRACE_BEGIN();

    data_id_trace( &main_diagram_id );
    const uint32_t src_results_cnt
        = data_small_set_get_count( gui_sketch_request_get_search_result_diagrams_const( &((*this_).request) ) );
    U8_TRACE_INFO_INT( "src_results_cnt:", src_results_cnt );

    /* determine diagram id of root diagram */
    if ( ! data_id_is_valid( &main_diagram_id ) )
    {
        /* load all without parent */
        data_small_set_t roots;
        data_small_set_init( &roots );
        const u8_error_t db_err
            = data_database_reader_get_diagram_ids_by_parent_id( (*this_).db_reader,
                                                                 DATA_ROW_ID_VOID,
                                                                 &roots
                                                               );
        const uint32_t count = data_small_set_get_count( &roots );
        if ( u8_error_contains( db_err, U8_ERROR_NO_DB ) )
        {
            U8_TRACE_INFO( "database not open.");
        }
        else if ( U8_ERROR_NONE != db_err )
        {
            U8_LOG_ERROR_HEX( "data_database_reader_get_diagrams_by_parent_id failed.", db_err );
        }
        else if ( count > 1 )
        {
            U8_LOG_ERROR_INT( "more than one root diagram exists!", count );
        }
        else if ( count < 1 )
        {
            U8_TRACE_INFO( "no root diagram exists!" );
        }
        else
        {
            main_diagram_id = data_small_set_get_id( &roots, 0 );
            U8_TRACE_INFO_INT( "main_diagram_id:", data_id_get_row_id( &main_diagram_id ));
        }

        /* cleanup */
        data_small_set_destroy( &roots );
    }

    /* store request */
    gui_sketch_request_set_focused_diagram( &((*this_).request), main_diagram_id );

    /* load data to be drawn */
    gui_sketch_area_private_load_cards_data ( this_ );

    U8_TRACE_END();
}

void gui_sketch_area_private_refocus_and_reload_data ( gui_sketch_area_t *this_ )
{
    U8_TRACE_BEGIN();

    /* determine currently selected diagram id and parent id from cache for emergency-fallback */
    const data_id_t former_diagram_id = gui_sketch_request_get_focused_diagram( &((*this_).request) );
    const data_id_t former_parent_diagram_id = gui_sketch_request_get_parent_diagram( &((*this_).request) );

    /* reload diagram data */
    gui_sketch_area_show_diagram( this_, former_diagram_id );

    if ( GUI_TOOL_SEARCH != gui_sketch_request_get_tool_mode( &((*this_).request) ) )
    {
        if ( data_id_is_valid( &former_diagram_id )
            &&( DATA_ROW_ID_VOID == gui_sketch_area_private_get_focused_diagram_id( this_ ) ))
        {
            /* the requested diagram was not loaded, try the parent: */
            gui_sketch_area_show_diagram( this_, former_parent_diagram_id );

            if ( data_id_is_valid( &former_parent_diagram_id )
                &&( DATA_ROW_ID_VOID == gui_sketch_area_private_get_focused_diagram_id( this_ ) ))
            {
                /* the requested diagram was not loaded, go back to root diagram: */
                gui_sketch_area_show_diagram( this_, DATA_ID_VOID );
            }

            /* clear the selected set */
            gui_marked_set_clear_selected_set( (*this_).marker );
        }
    }

    U8_TRACE_END();
}

void gui_sketch_area_private_load_cards_data ( gui_sketch_area_t *this_ )
{
    U8_TRACE_BEGIN();

    /* destroy _all_ old cards */
    for ( uint_fast32_t idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_destroy( &((*this_).cards[idx]) );
    }
    (*this_).card_num = 0;

    /* load new cards */
    switch ( gui_sketch_request_get_tool_mode( &((*this_).request) ) )
    {
        case GUI_TOOL_SEARCH:
        {
            const data_small_set_t* requested_diagrams
                = gui_sketch_request_get_search_result_diagrams_const( &((*this_).request) );

            const uint_fast32_t d_count = data_small_set_get_count( requested_diagrams );
            for ( uint_fast32_t index = 0; index < d_count; index ++ )
            {
                const data_id_t diag_id = data_small_set_get_id( requested_diagrams, index );
                if ( (*this_).card_num < GUI_SKETCH_AREA_CONST_MAX_CARDS )
                {
                    gui_sketch_card_init( &((*this_).cards[(*this_).card_num]) );
                    gui_sketch_card_load_data( &((*this_).cards[(*this_).card_num]), diag_id, (*this_).db_reader );
                    if ( gui_sketch_card_is_valid( &((*this_).cards[(*this_).card_num]) ) )
                    {
                        (*this_).card_num ++;
                    }
                    else
                    {
                        U8_TRACE_INFO_INT( "could not load diagram:", data_id_get_row_id(&diag_id) );
                    }
                }
                else
                {
                    U8_TRACE_INFO_INT( "max diagrams exeeded, dropping diagram:", data_id_get_row_id(&diag_id) );
                }
            }
        }
        break;

        default:
        {
            const data_id_t main_diagram_id = gui_sketch_request_get_focused_diagram( &((*this_).request) );

            gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]) );
            gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]), main_diagram_id, (*this_).db_reader );
            (*this_).card_num = 1;
            gui_sketch_nav_tree_load_data( &((*this_).nav_tree), data_id_get_row_id( &main_diagram_id ), (*this_).db_reader );

            /* determine ids */
            const data_diagram_t *selected_diag = gui_sketch_area_private_get_focused_diagram_ptr( this_ );
            const data_row_id_t selected_diagram_row_id = data_diagram_get_row_id( selected_diag );
            const data_id_t selected_diagram_id = data_diagram_get_data_id( selected_diag );
            U8_TRACE_INFO_INT( "selected_diagram_row_id:", selected_diagram_row_id );
            const data_id_t parent_diagram_id = data_diagram_get_parent_data_id( selected_diag );
            U8_TRACE_INFO_INT( "parent_diagram_id:", data_id_get_row_id( &parent_diagram_id ) );

            const data_id_t former_focused_diag = gui_marked_set_get_focused_diagram( (*this_).marker);
            gui_sketch_request_set_parent_diagram( &((*this_).request), parent_diagram_id );
            if ( ! data_id_equals_or_both_void( &former_focused_diag, &selected_diagram_id ) )
            {
                /* clear focused but keep selected_diagram_id, needed for gui_toolbox_paste */
                gui_marked_set_set_focused( (*this_).marker, DATA_FULL_ID_VOID, selected_diagram_id );
            }
            else
            {
                /* DO NOT NOTIFY CHANGES IN A POSSIBLE DATA CHANGE CALLBACK - MAY CAUSE ENDLESS RECURSION */
            }

            const gui_tool_t selected_tool
                = gui_sketch_request_get_tool_mode( &((*this_).request) );
            if ( GUI_TOOL_NAVIGATE == selected_tool )
            {

                /* load parent even if there is no parent (-->VOID) */
                gui_sketch_card_init( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]) );
                gui_sketch_card_load_data( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]), parent_diagram_id, (*this_).db_reader );
                (*this_).card_num = 2;

                /* load all children (up to GUI_SKETCH_AREA_CONST_MAX_TEMP_DIAGRAMS)*/
                data_small_set_t children;
                data_small_set_init( &children );
                const u8_error_t db_err
                    = data_database_reader_get_diagram_ids_by_parent_id( (*this_).db_reader,
                                                                         selected_diagram_row_id,
                                                                         &children
                                                                       );
                if ( u8_error_contains( db_err, U8_ERROR_NO_DB ) )
                {
                    U8_TRACE_INFO( "database not open.");
                }
                else if ( U8_ERROR_NONE != db_err )
                {
                    U8_LOG_ERROR_HEX( "data_database_reader_get_diagram_ids_by_parent_id failed.", db_err );
                }
                else
                {
                    for ( uint_fast32_t index = 0; index < data_small_set_get_count( &children ); index ++ )
                    {
                        const data_id_t child = data_small_set_get_id( &children, index );
                        if ( (*this_).card_num < GUI_SKETCH_AREA_CONST_MAX_CARDS )
                        {
                            gui_sketch_card_init( &((*this_).cards[(*this_).card_num]) );
                            gui_sketch_card_load_data( &((*this_).cards[(*this_).card_num]), child, (*this_).db_reader );
                            (*this_).card_num ++;
                        }
                        else
                        {
                            U8_LOG_ERROR_INT( "more children diagrams exist than fit into cards array:", data_id_get_row_id( &child ) );
                        }
                    }
                }
                /* cleanup */
                data_small_set_destroy( &children );
            }

        }
        break;
    }

    U8_TRACE_END();
}

static const uint32_t NAV_TREE_WIDTH = 224;
static const uint32_t RESULT_LIST_WIDTH = 240;

void gui_sketch_area_private_layout_subwidgets ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );
    assert((*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS);

    gui_tool_t selected_tool;
    selected_tool = gui_sketch_request_get_tool_mode( &((*this_).request) );

    /* fetch area bounds */
    const uint32_t width = shape_int_rectangle_get_width( &area_bounds );
    const uint32_t height = shape_int_rectangle_get_height( &area_bounds );
    const int32_t left = shape_int_rectangle_get_left( &area_bounds );
    const int32_t top = shape_int_rectangle_get_top( &area_bounds );
    U8_TRACE_INFO_INT_INT( "width, height", width, height );

    /* layout result list */
    const bool result_list_visible = ( GUI_TOOL_SEARCH == selected_tool );
    {
        shape_int_rectangle_t result_list_bounds;
        shape_int_rectangle_init( &result_list_bounds, left, top, RESULT_LIST_WIDTH, height );
        gui_sketch_result_list_set_bounds( &((*this_).result_list ), result_list_bounds );
        gui_sketch_result_list_set_visible( &((*this_).result_list), result_list_visible );
        if ( result_list_visible )
        {
            gui_sketch_result_list_do_layout( &((*this_).result_list), cr );
        }
    }

    /* layout nav tree */
    const bool nav_tree_visible = ( GUI_TOOL_NAVIGATE == selected_tool );
    {
        shape_int_rectangle_t nav_tree_bounds;
        shape_int_rectangle_init( &nav_tree_bounds, left, top, NAV_TREE_WIDTH, height );
        gui_sketch_nav_tree_set_bounds( &((*this_).nav_tree), nav_tree_bounds );
        gui_sketch_nav_tree_set_visible( &((*this_).nav_tree), nav_tree_visible );
        if ( nav_tree_visible )
        {
            gui_sketch_nav_tree_do_layout( &((*this_).nav_tree), cr );
        }
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

    U8_TRACE_END();
}

void gui_sketch_area_private_draw_subwidgets ( gui_sketch_area_t *this_, shape_int_rectangle_t area_bounds, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const gui_tool_t selected_tool = gui_sketch_request_get_tool_mode( &((*this_).request) );

    /* draw background */
    switch( selected_tool )
    {
        case GUI_TOOL_SEARCH:
        {
            gui_sketch_background_draw_search( &((*this_).background), cr );
        }
        break;

        case GUI_TOOL_NAVIGATE:
        {
            const unsigned int depth
                = ( gui_sketch_card_is_valid( &((*this_).cards[GUI_SKETCH_AREA_CONST_PARENT_CARD]) ) ) ? 1 : 0;  /* currently, only root and non-root can be distinguished */
            const unsigned int children
                = (*this_).card_num-2;
            gui_sketch_background_draw_navigation( &((*this_).background), depth, children, cr );
        }
        break;

        case GUI_TOOL_EDIT:
        {
            gui_sketch_background_draw_edit( &((*this_).background), cr );
        }
        break;

        case GUI_TOOL_CREATE:
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
        gui_sketch_card_draw_paper( &((*this_).cards[card_idx]),
                                    selected_tool,
                                    &((*this_).drag_state),
                                    cr
                                  );
        gui_sketch_card_draw( &((*this_).cards[card_idx]), (*this_).marker, cr );
    }

    /* overlay tool-helper lines */
    const int32_t mouse_x = gui_sketch_drag_state_get_to_x( &((*this_).drag_state) );
    const int32_t mouse_y = gui_sketch_drag_state_get_to_y( &((*this_).drag_state) );
    gui_sketch_overlay_draw( &((*this_).overlay),
                             selected_tool,
                             &((*this_).drag_state),
                             gui_sketch_area_private_get_card_at_pos ( this_, mouse_x, mouse_y ),
                             (*this_).marker,
                             cr
                           );
    if ( gui_sketch_nav_tree_is_visible( &((*this_).nav_tree) ) )
    {
        gui_sketch_nav_tree_draw_overlay( &((*this_).nav_tree), &((*this_).drag_state) , cr );
    }

    U8_TRACE_END();
}

void gui_sketch_area_leave_notify_callback( GtkEventControllerMotion* self, gpointer user_data )
{
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    gui_sketch_area_t *this_ = user_data;
    assert( NULL != this_ );

    gui_marked_set_clear_highlighted( (*this_).marker );
    /* mark dirty rect */
    gtk_widget_queue_draw( (*this_).drawing_area );

    U8_TRACE_END();
}

void gui_sketch_area_motion_notify_callback( GtkEventControllerMotion* self,
                                             gdouble in_x,
                                             gdouble in_y,
                                             gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_sketch_area_t *this_ = user_data;
    assert( NULL != this_ );

    gui_sketch_area_motion_notify( this_, (int)in_x, (int)in_y );

    U8_TRACE_END();
}

void gui_sketch_area_motion_notify( gui_sketch_area_t *this_, int x, int y )
{
    U8_TRACE_BEGIN();
    U8_TRACE_INFO_INT_INT( "x/y", x, y );

    /* update drag coordinates */
    gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

    /* do highlight */
    const gui_tool_t selected_tool = gui_sketch_request_get_tool_mode( &((*this_).request) );
    switch ( selected_tool )
    {
        case GUI_TOOL_SEARCH:  /* or */
        case GUI_TOOL_NAVIGATE:
        {
            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* always redraw while dragging */
                gtk_widget_queue_draw( (*this_).drawing_area );
            }
            else /* not dragging */
            {
                data_id_t object_under_mouse = DATA_ID_VOID;
                data_id_t diag_under_mouse = DATA_ID_VOID;
                gui_sketch_area_private_get_diagram_and_object_id_at_pos ( this_, x, y, &diag_under_mouse, &object_under_mouse );
                gui_sketch_action_t btn_under_mouse = GUI_SKETCH_ACTION_NONE;
                if ( selected_tool == GUI_TOOL_NAVIGATE )
                {
                    gui_sketch_nav_tree_get_button_at_pos( &((*this_).nav_tree), x, y, &btn_under_mouse );
                }

                const data_id_t object_highlighted
                    = gui_marked_set_get_highlighted( (*this_).marker );
                const data_id_t diag_highlighted
                    = gui_marked_set_get_highlighted_diagram( (*this_).marker );
                const gui_sketch_action_t btn_highlighted
                    = gui_marked_set_get_highlighted_button( (*this_).marker );
                const bool obj_both_void
                    = ( ! data_id_is_valid( &object_under_mouse ) )&&( ! data_id_is_valid( &object_highlighted ) );
                const bool obj_equal_or_both_void
                    = data_id_equals( &object_under_mouse, &object_highlighted ) || obj_both_void;
                const bool diag_both_void
                    = ( ! data_id_is_valid( &diag_under_mouse ) )&&( ! data_id_is_valid( &diag_highlighted ) );
                const bool diag_equal_or_both_void
                    = data_id_equals( &diag_under_mouse, &diag_highlighted ) || diag_both_void;
                const bool btn_changed = ( btn_under_mouse != btn_highlighted );
                if (( btn_under_mouse != GUI_SKETCH_ACTION_NONE )&&( btn_changed ))
                {
                    /* highlighted button is activated and has changed */
                    gui_marked_set_set_highlighted_button( (*this_).marker, btn_under_mouse );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( (*this_).drawing_area );
                }
                else if (( ! obj_equal_or_both_void )||( ! diag_equal_or_both_void )||( btn_changed ))
                {
                    /* highlight changed */
                    gui_marked_set_set_highlighted( (*this_).marker, object_under_mouse, diag_under_mouse );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( (*this_).drawing_area );
                }
            }
        }
        break;

        case GUI_TOOL_EDIT:
        {
            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* what is dragged? */
                const data_full_id_t *const dragged_object
                    = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                const data_id_t dragged_element
                    = data_full_id_get_primary_id( dragged_object );
                const data_id_t dragged_classifier
                    = data_full_id_get_secondary_id( dragged_object );

                /* what is the target location? */
                gui_sketch_card_t *target_card = gui_sketch_area_private_get_card_at_pos ( this_, x, y );
                if ( NULL != target_card )
                {
                    /* mark again - in case the marker was lost when mouse was outside window */
                    const data_id_t diag_id = gui_sketch_card_get_diagram_id( target_card );
                    gui_marked_set_set_highlighted( (*this_).marker, dragged_element, diag_id );

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
                    gtk_widget_queue_draw( (*this_).drawing_area );
                }
                else
                {
                    gui_marked_set_clear_highlighted( (*this_).marker );
                    U8_TRACE_INFO( "mouse click outside sketch card." );
                }
            }
            else /* not dragging */
            {
                data_full_id_t object_under_mouse;
                data_id_t diag_id;
                gui_sketch_area_private_get_element_id_at_pos( this_,
                                                               x,
                                                               y,
                                                               true /* FILTER_LIFELINE */,
                                                               &object_under_mouse,
                                                               &diag_id
                                                             );
                const data_id_t object_highlighted
                    = gui_marked_set_get_highlighted( (*this_).marker );
                const data_id_t mouseover_element
                    = data_full_id_get_primary_id( &object_under_mouse );
                if ( ! data_id_equals( &mouseover_element, &object_highlighted ) )
                {
                    if ( data_full_id_is_valid( &object_under_mouse ) || data_id_is_valid( &object_highlighted ) )
                    {
                        /* highlight changed */
                        gui_marked_set_set_highlighted( (*this_).marker, mouseover_element, diag_id );

                        /* mark dirty rect */
                        gtk_widget_queue_draw( (*this_).drawing_area );
                    }
                }
            }
        }
        break;

        case GUI_TOOL_CREATE:
        {
            const gui_sketch_card_t *const target_card = gui_sketch_area_private_get_card_at_pos ( this_, x, y );
            const layout_subelement_id_t element_id
                = ( NULL == target_card )
                ? LAYOUT_SUBELEMENT_ID_VOID
                : gui_sketch_card_get_element_at_pos( target_card, x, y, false /* FILTER_LIFELINE */ );
            const data_id_t diagram_id
                = ( NULL == target_card )
                ? DATA_ID_VOID
                : gui_sketch_card_get_diagram_id( target_card );
            const data_full_id_t *const element_under_mouse_full
                = layout_subelement_id_get_id_const( &element_id );  /* may return VOID */
            const layout_subelement_kind_t element_under_mouse_kind
                = layout_subelement_id_get_kind( &element_id );  /* may return VOID */
            const data_id_t element_under_mouse
                = data_full_id_get_primary_id( element_under_mouse_full );

            const data_id_t element_highlighted
                = gui_marked_set_get_highlighted( (*this_).marker );
            const layout_subelement_kind_t element_highlighted_kind
                = gui_marked_set_get_highlighted_kind( (*this_).marker );
            const bool highlighted_element_changed
                = ( ! data_id_equals( &element_under_mouse, &element_highlighted ) )
                || ( element_under_mouse_kind != element_highlighted_kind );
            if ( highlighted_element_changed )
            {
                if ( data_id_is_valid( &element_under_mouse ) || data_id_is_valid( &element_highlighted ) )
                {
                    gui_marked_set_set_highlighted_subelement( (*this_).marker, &element_id, diagram_id );

                    /* mark dirty rect */
                    gtk_widget_queue_draw( (*this_).drawing_area );
                }
            }
            else if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* always redraw while dragging */
                gtk_widget_queue_draw( (*this_).drawing_area );
            }
            else
            {
                /* always redraw while moving the mouse to move the new-box-and-arrow icon */
                gtk_widget_queue_draw( (*this_).drawing_area );
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    U8_TRACE_END();
}

void gui_sketch_area_button_press_callback( GtkGestureClick* self,
                                            gint n_press,
                                            gdouble x,
                                            gdouble y,
                                            gpointer user_data )
{
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    gui_sketch_area_t *this_ = user_data;
    assert( NULL != this_ );

    gui_sketch_area_button_press( this_, (int)x, (int)y );

    U8_TRACE_END();
}

void gui_sketch_area_button_press( gui_sketch_area_t *this_, int x, int y )
{
    U8_TRACE_BEGIN();

    /* in general, hide the last message */
    gui_simple_message_to_user_hide( (*this_).message_to_user );

    U8_TRACE_INFO("press");

    /* cause the text edit widgets to lose the focus so that these can store the latest changes */
    gtk_widget_grab_focus( (*this_).drawing_area );

    /* get position */
    U8_TRACE_INFO_INT_INT( "x/y", x, y );

    /* check that drag state is false */
    if ( ( gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
        || ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
    {
        U8_LOG_ERROR("drag state indicates dragging - but button was not pressed before!");
    }

    /* update drag coordinates */
    gui_sketch_drag_state_set_from ( &((*this_).drag_state), x, y );
    gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

    /* do action */
    const gui_tool_t selected_tool = gui_sketch_request_get_tool_mode( &((*this_).request) );
    switch ( selected_tool )
    {
        case GUI_TOOL_NAVIGATE:
        {
            U8_TRACE_INFO( "GUI_TOOL_NAVIGATE" );

            /* determine clicked diagram */
            data_id_t clicked_diagram_id = DATA_ID_VOID;
            data_id_t clicked_object_id = DATA_ID_VOID;
            gui_sketch_area_private_get_diagram_and_object_id_at_pos ( this_, x, y, &clicked_diagram_id, &clicked_object_id );
            data_id_trace( &clicked_diagram_id );
            data_id_trace( &clicked_object_id );

            /* load diagram */
            if ( data_id_is_valid( &clicked_diagram_id ) )
            {
                /* update drag state */
                data_full_id_t dragged_object;
                data_full_id_init_solo ( &dragged_object, clicked_diagram_id );
                gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), dragged_object );
            }
            else
            {
                U8_TRACE_INFO("invalid clicked object at gui_sketch_area_button_press_callback");

                gui_sketch_action_t action_button_id;
                gui_sketch_nav_tree_get_button_at_pos ( &((*this_).nav_tree), x, y, &action_button_id );
                if ( action_button_id != GUI_SKETCH_ACTION_NONE )
                {
                    /* create a new diagram */
                    u8_error_t c_result = U8_ERROR_NONE;
                    data_row_id_t new_diag_id = DATA_ROW_ID_VOID;

                    {
                        const data_diagram_t *selected_diag;
                        selected_diag = gui_sketch_nav_tree_get_diagram_ptr ( &((*this_).nav_tree) );
                        switch ( action_button_id )
                        {
                            case GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM:
                            {
                                assert( data_diagram_is_valid( selected_diag ) );
                                data_row_id_t parent_diagram_id;
                                parent_diagram_id = data_diagram_get_parent_row_id( selected_diag );
                                int32_t list_order;
                                list_order = gui_sketch_nav_tree_get_siblings_highest_order ( &((*this_).nav_tree) );


                                c_result = gui_sketch_object_creator_create_diagram( &((*this_).object_creator),
                                                                                     parent_diagram_id,
                                                                                     list_order + 32768,
                                                                                     &new_diag_id
                                                                                   );
                            }
                            break;

                            case GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM:
                            {
                                assert( data_diagram_is_valid( selected_diag ) );
                                data_row_id_t selected_diagram_id;
                                selected_diagram_id = data_diagram_get_row_id( selected_diag );
                                int32_t list_order;
                                list_order = gui_sketch_nav_tree_get_children_highest_order ( &((*this_).nav_tree) );
                                c_result = gui_sketch_object_creator_create_diagram( &((*this_).object_creator),
                                                                                     selected_diagram_id,
                                                                                     list_order + 32768,
                                                                                     &new_diag_id
                                                                                   );
                            }
                            break;

                            case GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM:
                            {
                                c_result = gui_sketch_object_creator_create_diagram( &((*this_).object_creator),
                                                                                     DATA_ROW_ID_VOID,
                                                                                     0,
                                                                                     &new_diag_id
                                                                                   );
                            }
                            break;

                            default:
                            {
                                U8_LOG_ERROR_INT("illegal action value in gui_sketch_action_t:",action_button_id);
                                assert(false);
                                c_result = U8_ERROR_INVALID_REQUEST;
                            }
                            break;
                        }
                    }

                    if ( U8_ERROR_NONE != c_result )
                    {
                        U8_LOG_ERROR("unexpected error at gui_sketch_object_creator_create_diagram");
                    }
                    else
                    {
                        data_id_t focused_id;
                        data_id_init( &focused_id, DATA_TABLE_DIAGRAM, new_diag_id );

                        /* load/reload data to be drawn */
                        gui_sketch_area_show_diagram( this_, focused_id );

                        /* update marked set */
                        data_full_id_t focused_full_id;
                        data_full_id_init_solo( &focused_full_id, focused_id );

                        gui_marked_set_set_focused( (*this_).marker, focused_full_id, focused_id );
                        gui_marked_set_clear_selected_set( (*this_).marker );
                    }
                }
            }
        }
        break;

        case GUI_TOOL_EDIT:
        {
            U8_TRACE_INFO( "GUI_TOOL_EDIT" );

            /* determine the focused object */
            data_full_id_t focused_object;
            data_id_t diag_id;
            gui_sketch_area_private_get_element_id_at_pos( this_,
                                                           x,
                                                           y,
                                                           true /* FILTER_LIFELINE */,
                                                           &focused_object,
                                                           &diag_id
                                                         );
            data_full_id_trace( &focused_object );

            /* update drag state */
            gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), focused_object );

            /* toggle objects according to rules of gui_marked_set_t */
            gui_marked_set_toggle_obj( (*this_).marker, focused_object, diag_id );

            /* mark dirty rect */
            gtk_widget_queue_draw( (*this_).drawing_area );
        }
        break;

        case GUI_TOOL_SEARCH:
        {
            U8_TRACE_INFO( "GUI_TOOL_SEARCH" );

            /* determine clicked diagram */
            data_id_t clicked_diagram_id = DATA_ID_VOID;
            data_id_t clicked_object_id = DATA_ID_VOID;
            gui_sketch_area_private_get_diagram_and_object_id_at_pos ( this_, x, y, &clicked_diagram_id, &clicked_object_id );
            data_id_trace( &clicked_diagram_id );
            data_id_trace( &clicked_object_id );

            /* store diagram id to drag_state */
            if ( data_id_is_valid( &clicked_diagram_id ) )
            {
                /* update drag state */
                data_full_id_t dragged_object;
                data_full_id_init_solo ( &dragged_object, clicked_diagram_id );
                gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), dragged_object );
            }

            /* which object is currently focused? */
            const data_id_t focused_before = gui_marked_set_get_focused_obj( (*this_).marker );

            if ( data_id_equals ( &clicked_object_id, &focused_before ) )
            {
                /* the clicked object is already focused */
            }
            else
            {
                /* set focused object */
                assert( DATA_TABLE_DIAGRAMELEMENT != data_id_get_table( &clicked_object_id ) );
                data_full_id_t focused_object;
                data_full_id_init_solo ( &focused_object, clicked_object_id );

                gui_marked_set_set_focused( (*this_).marker, focused_object, clicked_diagram_id );
            }
        }
        break;

        case GUI_TOOL_CREATE:
        {
            U8_TRACE_INFO( "GUI_TOOL_CREATE" );

            /* what is the target location? */
            gui_sketch_card_t *target_card = gui_sketch_area_private_get_card_at_pos ( this_, x, y );

            if ( NULL == target_card )
            {
                U8_TRACE_INFO_INT_INT("No card at",x,y);

                /* if this happens, invalidate the marked object. */
                gui_marked_set_clear_focused( (*this_).marker );
            }
            else
            {
                /* determine the object at click location */
                const data_id_t diagram_id = gui_sketch_card_get_diagram_id( target_card );
                const layout_subelement_id_t element_id
                    = gui_sketch_card_get_element_at_pos( target_card, x, y, false /* FILTER_LIFELINE */ );
                const data_full_id_t *const clicked_object
                    = layout_subelement_id_get_id_const( &element_id );
                data_full_id_trace( clicked_object );
                const layout_subelement_kind_t kind
                    = layout_subelement_id_get_kind( &element_id );
                if ( kind != LAYOUT_SUBELEMENT_KIND_SPACE )
                {
                    /* update drag state */
                    gui_sketch_drag_state_start_dragging_when_move ( &((*this_).drag_state), *clicked_object );

                    /* set focused object (either a diagramelement or a feature) */
                    gui_marked_set_set_focused( (*this_).marker, *clicked_object, diagram_id );
                }
                else /* clicked either into inner space of a classifier or of a diagram */
                {
                    /* stop dragging */
                    gui_sketch_drag_state_stop_dragging ( &((*this_).drag_state) );

                    /* get data */
                    const data_id_t *const surrounding_element
                        = data_full_id_get_primary_id_const( clicked_object );
                    assert( data_id_is_valid( surrounding_element ) );

                    /* create a new classifier */
                    data_id_t dummy_classifier;
                    data_id_init( &dummy_classifier, DATA_TABLE_CLASSIFIER, DATA_ROW_ID_VOID );
                    layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dummy_classifier, x, y );
                    const int32_t x_order = layout_order_get_first( &layout_order );
                    const int32_t y_order = layout_order_get_second( &layout_order );
                    U8_TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                    /* create a classifier or a child-classifier */
                    u8_error_t c_result;
                    data_row_id_t new_diagele_id;
                    data_row_id_t new_classifier_id;
                    if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( surrounding_element ) )
                    {
                        const data_id_t *const surrounding_classifier
                            = data_full_id_get_secondary_id_const( clicked_object );
                        assert( data_id_is_valid( surrounding_classifier ) );

                        data_row_id_t new_relationship_id;
                        c_result = gui_sketch_object_creator_create_classifier_as_child( &((*this_).object_creator),
                                                                                         data_id_get_row_id( &diagram_id ),
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
                        assert( DATA_TABLE_DIAGRAM == data_id_get_table( surrounding_element ) );
                        c_result = gui_sketch_object_creator_create_classifier( &((*this_).object_creator),
                                                                                data_id_get_row_id( &diagram_id ),
                                                                                x_order,
                                                                                y_order,
                                                                                &new_diagele_id,
                                                                                &new_classifier_id
                                                                              );
                    }

                    if ( U8_ERROR_DUPLICATE_NAME == c_result )
                    {
                        /* this should not happen: names are auto-generated */
                        gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                                           GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                           GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                           ""
                                                                         );
                    }
                    else if ( U8_ERROR_NONE != c_result )
                    {
                        U8_LOG_ERROR("unexpected error at gui_sketch_object_creator_create_classifier/_as_child");
                    }
                    else
                    {
                        /* set focused object and notify listener */
                        const data_full_id_t focused_object
                            = DATA_FULL_ID( DATA_TABLE_DIAGRAMELEMENT, new_diagele_id, new_classifier_id );
                        gui_marked_set_set_focused( (*this_).marker, focused_object, diagram_id );
                        gui_marked_set_clear_selected_set( (*this_).marker );

                        U8_TRACE_INFO_INT( "new_classifier_id:", new_classifier_id );
                    }
                }
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "selected_tool is out of range" );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_sketch_area_button_release_callback( GtkGestureClick* self,
                                              gint n_press,
                                              gdouble x,
                                              gdouble y,
                                              gpointer user_data )
{
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    gui_sketch_area_t *this_ = user_data;
    assert( NULL != this_ );

    gui_sketch_area_button_release( this_, (int)x, (int)y );

    U8_TRACE_END();
}

void gui_sketch_area_button_release( gui_sketch_area_t *this_, int x, int y )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO("release");

    /* get position */
    U8_TRACE_INFO_INT_INT("x/y",x,y);

    /* update drag coordinates */
    gui_sketch_drag_state_set_to ( &((*this_).drag_state), x, y );

    /* check that drag state is true */
    if ( ( ! gui_sketch_drag_state_is_dragging( &((*this_).drag_state) ) )
        && ( ! gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) ) )
    {
        U8_TRACE_INFO("drag state indicates no dragging and no waiting - but button was pressed before!");
    }

    /* do action */
    const gui_tool_t selected_tool
        = gui_sketch_request_get_tool_mode( &((*this_).request) );
    switch ( selected_tool )
    {
        case GUI_TOOL_NAVIGATE:
        {
            U8_TRACE_INFO("GUI_TOOL_NAVIGATE");
            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* which diagram was dragged? */
                data_full_id_t *dragged_object;
                dragged_object = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                data_id_t dragged_diagram;
                dragged_diagram = data_full_id_get_primary_id( dragged_object );

                /* to which diagram-gap was it dragged to? */
                gui_error_t gui_err;
                data_id_t target_parent_id;
                int32_t target_list_order;
                shape_int_rectangle_t dummy_gap_marker_border;
                gui_err = gui_sketch_nav_tree_get_gap_info_at_pos( &((*this_).nav_tree),
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
                    U8_TRACE_INFO_INT( "dragged_diagram:", data_id_get_row_id( &dragged_diagram ) );
                    U8_TRACE_INFO_INT( "target_parent_id:", data_id_get_row_id( &target_parent_id ) );
                    U8_TRACE_INFO_INT( "target_list_order:", target_list_order );
                    bool is_descendant;
                    bool is_self;
                    gui_error_t not_found;
                    not_found = gui_sketch_nav_tree_is_descendant( &((*this_).nav_tree),
                                                                   data_id_get_row_id( &dragged_diagram ),
                                                                   data_id_get_row_id( &target_parent_id ),
                                                                   &is_descendant
                                                                 );
                    is_self = ( data_id_get_row_id( &dragged_diagram ) == data_id_get_row_id( &target_parent_id ) );
                    if ( ( ! is_self ) && ( not_found == GUI_ERROR_NONE ) && ( ! is_descendant ) )
                    {
                        ctrl_diagram_controller_t *diag_control;
                        diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                        u8_error_t c_err;
                        c_err = ctrl_diagram_controller_update_diagram_list_order( diag_control,
                                                                                   data_id_get_row_id( &dragged_diagram ),
                                                                                   target_list_order
                                                                                 );
                        if ( U8_ERROR_NONE != c_err )
                        {
                            U8_LOG_ERROR_HEX( "U8_ERROR_NONE !=", c_err );
                        }
                        c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control,
                                                                                  data_id_get_row_id( &dragged_diagram ),
                                                                                  data_id_get_row_id( &target_parent_id ),
                                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                                );
                        if ( U8_ERROR_NONE != c_err )
                        {
                            U8_LOG_ERROR_HEX( "U8_ERROR_NONE !=", c_err );
                        }
                    }
                    else if ( DATA_ROW_ID_VOID == data_id_get_row_id( &target_parent_id ) )
                    {
                        /* a diagram is dragged to the root location */
                        ctrl_diagram_controller_t *const diag_control2
                            = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                        const data_row_id_t root_id
                            = gui_sketch_nav_tree_get_root_diagram_id ( &((*this_).nav_tree) );
                        if (( root_id != DATA_ROW_ID_VOID )&&( root_id != data_id_get_row_id( &dragged_diagram ) ))
                        {
                            u8_error_t c_err;
                            c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control2,
                                                                                      data_id_get_row_id( &dragged_diagram ),
                                                                                      DATA_ROW_ID_VOID,
                                                                                      CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                                    );
                            if ( U8_ERROR_NONE != c_err )
                            {
                                U8_LOG_ERROR_HEX( "U8_ERROR_NONE !=", c_err );
                            }
                            c_err = ctrl_diagram_controller_update_diagram_parent_id( diag_control2,
                                                                                      root_id,
                                                                                      data_id_get_row_id( &dragged_diagram ),
                                                                                      CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                                    );
                            if ( U8_ERROR_NONE != c_err )
                            {
                                U8_LOG_ERROR_HEX( "U8_ERROR_NONE !=", c_err );
                            }
                        }
                        else
                        {
                            U8_LOG_WARNING("dragging a diagram to the root location but no root exists or dragged diagram is root?");
                        }
                    }
                    else
                    {
                        U8_LOG_WARNING("diagram dragging to invalid target location");
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
                const data_full_id_t *const dragged_object
                    = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                const data_id_t dragged_diagram
                    = data_full_id_get_primary_id( dragged_object );
                if ( DATA_TABLE_DIAGRAM == data_id_get_table( &dragged_diagram ) )
                {
                    const data_row_id_t drag_id = data_id_get_row_id( &dragged_diagram );
                    const data_row_id_t focus_id = gui_sketch_area_private_get_focused_diagram_id( this_ );
                    if ( ( focus_id != DATA_ROW_ID_VOID )&&( focus_id == drag_id ) )
                    {
                        /* if clicked diagram is already the focused diagram, switch to edit mode */
                        gui_toolbox_set_selected_tool( (*this_).toolbox, GUI_TOOL_EDIT );
                    }
                    else
                    {
                        /* load/reload data to be drawn */
                        gui_sketch_area_show_diagram( this_, dragged_diagram );

                        /* set focus */
                        gui_marked_set_set_focused( (*this_).marker, *dragged_object, dragged_diagram );
                        gui_marked_set_clear_selected_set( (*this_).marker );

                        /* mark dirty rect */
                        gtk_widget_queue_draw( (*this_).drawing_area );
                    }
                }
                else
                {
                    U8_LOG_ANOMALY("GUI_TOOL_NAVIGATE released mouse button but not a diagram clicked before");
                }
            }
        }
        break;

        case GUI_TOOL_EDIT:
        {
            U8_TRACE_INFO("GUI_TOOL_EDIT");

            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* which object is selected? */
                const data_full_id_t *const dragged_object
                    = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                const data_id_t dragged_element
                    = data_full_id_get_primary_id( dragged_object );
                const data_id_t dragged_classifier
                    = data_full_id_get_secondary_id( dragged_object );

                /* what is the target location? */
                const gui_sketch_card_t *const target_card = gui_sketch_area_private_get_card_at_pos ( this_, x, y );
                if ( NULL == target_card )
                {
                    U8_TRACE_INFO_INT_INT("No card at",x,y);
                }
                else if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &dragged_element ) )
                {
                    layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_classifier, x, y );
                    if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                    {
                        int32_t list_order = layout_order_get_first( &layout_order );
                        U8_TRACE_INFO_INT( "list_order", list_order );

                        /* update db */
                        ctrl_classifier_controller_t *const classifier_control
                            = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                        const u8_error_t mov_result
                            = ctrl_classifier_controller_update_classifier_list_order( classifier_control,
                                                                                       data_id_get_row_id( &dragged_classifier ),
                                                                                       list_order
                                                                                     );
                        if ( U8_ERROR_NONE != mov_result )
                        {
                            U8_LOG_ERROR( "changing order failed: ctrl_classifier_controller_update_classifier_list_order" );
                        }
                    }
                    else if ( PENCIL_LAYOUT_ORDER_TYPE_X_Y == layout_order_get_order_type( &layout_order ) )
                    {
                        int32_t x_order = layout_order_get_first( &layout_order );
                        int32_t y_order = layout_order_get_second( &layout_order );
                        U8_TRACE_INFO_INT_INT( "x-order/y-order", x_order, y_order );

                        /* update db */
                        ctrl_classifier_controller_t *const classifier_control
                            = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                        const u8_error_t mov_result
                            = ctrl_classifier_controller_update_classifier_x_order_y_order( classifier_control,
                                                                                            data_id_get_row_id( &dragged_classifier ),
                                                                                            x_order,
                                                                                            y_order
                                                                                          );
                        if ( U8_ERROR_NONE != mov_result )
                        {
                            U8_LOG_ERROR( "changing order failed: ctrl_classifier_controller_update_classifier_x_order_y_order" );
                        }
                    }
                }
                else if ( DATA_TABLE_RELATIONSHIP == data_id_get_table( &dragged_element ) )
                {
                    layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_element, x, y );
                    if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                    {
                        int32_t list_order = layout_order_get_first( &layout_order );
                        U8_TRACE_INFO_INT( "list_order", list_order );

                        /* update db */
                        ctrl_classifier_controller_t *const classifier_control
                            = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                        const u8_error_t mov_result
                            = ctrl_classifier_controller_update_relationship_list_order( classifier_control,
                                                                                         data_id_get_row_id( &dragged_element ),
                                                                                         list_order
                                                                                       );
                        if ( U8_ERROR_NONE != mov_result )
                        {
                            U8_LOG_ERROR( "changing order failed: ctrl_classifier_controller_update_relationship_list_order" );
                        }
                    }
                }
                else if ( DATA_TABLE_FEATURE == data_id_get_table( &dragged_element ) )
                {
                    layout_order_t layout_order = gui_sketch_card_get_order_at_pos( target_card, dragged_element, x, y );
                    if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                    {
                        int32_t list_order = layout_order_get_first( &layout_order );
                        U8_TRACE_INFO_INT( "list_order", list_order );

                        /* update db */
                        ctrl_classifier_controller_t *const classifier_control
                            = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
                        const u8_error_t mov_result
                            = ctrl_classifier_controller_update_feature_list_order( classifier_control,
                                                                                    data_id_get_row_id( &dragged_element ),
                                                                                    list_order
                                                                                  );
                        if ( U8_ERROR_NONE != mov_result )
                        {
                            U8_LOG_ERROR( "changing order failed: ctrl_classifier_controller_update_feature_list_order" );
                        }
                    }
                }
                else
                {
                    U8_TRACE_INFO("Dragged object is neither relationship nor classifier nor feature");
                }
            }
        }
        break;

        case GUI_TOOL_SEARCH:
        {
            U8_TRACE_INFO("GUI_TOOL_SEARCH");

            if ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) )
            {
                /* click on diagram without drag */
                const data_full_id_t *const dragged_object
                    = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                const data_id_t dragged_diagram
                    = data_full_id_get_primary_id( dragged_object );
                if ( DATA_TABLE_DIAGRAM == data_id_get_table( &dragged_diagram ) )
                {
                    /* load/reload data to be drawn */
                    gui_sketch_area_show_diagram( this_, dragged_diagram );

                    /* clear selected set but do not change focused object */
                    gui_marked_set_clear_selected_set( (*this_).marker );

                    /* switch to edit mode */
                    gui_toolbox_set_selected_tool( (*this_).toolbox, GUI_TOOL_EDIT );
                }
                else
                {
                    U8_TRACE_INFO("GUI_TOOL_SEARCH released mouse button but not a diagram clicked before");
                }
            }
        }
        break;

        case GUI_TOOL_CREATE:
        {
            U8_TRACE_INFO("GUI_TOOL_CREATE");

            if ( gui_sketch_drag_state_is_dragging ( &((*this_).drag_state) ) )
            {
                /* which object is selected? */
                const data_full_id_t *const dragged_object
                    = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                data_full_id_trace( dragged_object );
                const data_id_t dragged_element
                    = data_full_id_get_primary_id( dragged_object );
                const data_id_t dragged_classifier
                    = data_full_id_get_secondary_id( dragged_object );

                /* which object is at the target location? */
                data_full_id_t destination_object;
                data_id_t diag_id;
                gui_sketch_area_private_get_element_id_at_pos( this_,
                                                               x,
                                                               y,
                                                               false /* FILTER_LIFELINE */,
                                                               &destination_object,
                                                               &diag_id
                                                             );
                data_full_id_trace( &destination_object );
                const data_id_t destination_element
                    = data_full_id_get_primary_id( &destination_object );
                const data_id_t destination_classifier
                    = data_full_id_get_secondary_id( &destination_object );

                gui_sketch_card_t *target_card = gui_sketch_area_private_get_card_at_pos ( this_, x, y );
                if ( data_id_is_valid( &dragged_classifier ) && data_id_is_valid( &destination_classifier ) && ( NULL != target_card ))
                {
                    if ( ( DATA_TABLE_CLASSIFIER == data_id_get_table( &dragged_classifier ) )
                        && ( DATA_TABLE_CLASSIFIER == data_id_get_table( &destination_classifier ) ) )
                    {
                        /* get the diagram type */
                        const data_diagram_t *const target_diag = gui_sketch_card_get_diagram_ptr ( target_card );
                        assert ( target_diag != NULL );
                        data_diagram_type_t diag_type = data_diagram_get_diagram_type ( target_diag );

                        /* determine source and destionation */
                        data_row_id_t new_from_classifier_id;
                        data_row_id_t new_to_classifier_id;
                        data_row_id_t new_from_feature_id;
                        data_row_id_t new_to_feature_id;
                        {
                            new_from_classifier_id = data_id_get_row_id( &dragged_classifier );
                            if ( DATA_TABLE_FEATURE == data_id_get_table( &dragged_element ) )
                            {
                                new_from_feature_id = data_id_get_row_id( &dragged_element );
                            }
                            else
                            {
                                new_from_feature_id = DATA_ROW_ID_VOID;
                            }
                            new_to_classifier_id = data_id_get_row_id( &destination_classifier );
                            if ( DATA_TABLE_FEATURE == data_id_get_table( &destination_element ) )
                            {
                                new_to_feature_id = data_id_get_row_id( &destination_element );
                            }
                            else
                            {
                                new_to_feature_id = DATA_ROW_ID_VOID;
                            }
                        }

                        /* propose a list_order for the relationship */
                        int32_t list_order_proposal = 0;
                        {
                            /* propose a list order */
                            data_id_t fake_relationship;
                            data_id_init ( &fake_relationship, DATA_TABLE_RELATIONSHIP, DATA_ROW_ID_VOID );
                            const layout_order_t layout_order
                                = gui_sketch_card_get_order_at_pos( target_card, fake_relationship, x, y );
                            if ( PENCIL_LAYOUT_ORDER_TYPE_LIST == layout_order_get_order_type( &layout_order ) )
                            {
                                list_order_proposal = layout_order_get_first( &layout_order );
                            }
                            else  /* PENCIL_LAYOUT_ORDER_TYPE_NONE */
                            {
                                list_order_proposal = gui_sketch_card_get_highest_rel_list_order( target_card ) + 32768;
                            }
                        }

                        data_row_id_t new_relationship_id;
                        const u8_error_t c_result
                            = gui_sketch_object_creator_create_relationship( &((*this_).object_creator),
                                                                             diag_type,
                                                                             new_from_classifier_id,
                                                                             new_from_feature_id,
                                                                             new_to_classifier_id,
                                                                             new_to_feature_id,
                                                                             list_order_proposal,
                                                                             &new_relationship_id
                                                                           );

                        if ( U8_ERROR_NONE != c_result )
                        {
                            U8_LOG_ANOMALY_HEX("anomaly at gui_sketch_object_creator_create_relationship",c_result);
                        }
                        else
                        {
                            /* set focused object */
                            const data_full_id_t focused_id
                                 = DATA_FULL_ID_SOLO( DATA_TABLE_RELATIONSHIP, new_relationship_id );
                            gui_marked_set_set_focused( (*this_).marker, focused_id, diag_id );
                            gui_marked_set_clear_selected_set( (*this_).marker );
                        }
                    }
                }
            }
            else if ( gui_sketch_drag_state_is_waiting_for_move( &((*this_).drag_state) ) )
            {
                /* click on classifier without drag */
                const data_full_id_t *const dragged_object
                    = gui_sketch_drag_state_get_dragged_object_ptr ( &((*this_).drag_state) );
                const data_id_t dragged_classifier
                    = data_full_id_get_secondary_id( dragged_object );

                const gui_sketch_card_t *const target_card
                    = gui_sketch_area_private_get_card_at_pos ( this_, x, y );
                if ( data_id_is_valid( &dragged_classifier ) && ( NULL != target_card ) )
                {
                    const data_id_t diag_id = gui_sketch_card_get_diagram_id( target_card );
                    if ( DATA_TABLE_CLASSIFIER == data_id_get_table( &dragged_classifier ) )
                    {
                        /* get the diagram type */
                        const data_diagram_t *const target_diag
                            = gui_sketch_card_get_diagram_const ( target_card );
                        assert ( target_diag != NULL );
                        const data_diagram_type_t diag_type = data_diagram_get_diagram_type ( target_diag );

                        /* determine id of classifier to which the clicked object belongs */
                        const data_row_id_t classifier_id = data_id_get_row_id( &dragged_classifier );

                        /* propose a list_order for the feature */
                        const int32_t std_list_order_proposal
                            = gui_sketch_card_get_highest_feat_list_order( target_card, dragged_classifier ) + 32768;
                        int32_t port_list_order_proposal = 0;
                        {
                            data_feature_init_new( &((*this_).private_temp_fake_feature),
                                                   DATA_FEATURE_TYPE_PORT,
                                                   classifier_id, /* classifier */
                                                   "FAKE_FEATURE",
                                                   "port-type",
                                                   "to determine the list order",
                                                   0 /* list_order */
                                                 );
                            port_list_order_proposal = gui_sketch_card_get_feature_order_at_pos( target_card,
                                                                                                 &((*this_).private_temp_fake_feature),
                                                                                                 x,
                                                                                                 y
                                                                                               );
                            data_feature_destroy ( &((*this_).private_temp_fake_feature) );
                        }

                        /* create a feature */
                        data_row_id_t new_feature_id;
                        const u8_error_t ctrl_err
                            = gui_sketch_object_creator_create_feature( &((*this_).object_creator),
                                                                        diag_type,
                                                                        classifier_id,
                                                                        std_list_order_proposal,
                                                                        port_list_order_proposal,
                                                                        &new_feature_id
                                                                      );

                        if ( U8_ERROR_NONE != ctrl_err )
                        {
                            U8_LOG_ANOMALY_HEX("anomaly at gui_sketch_object_creator_create_feature",ctrl_err);
                        }
                        else
                        {
                            /* set focused object */
                            const data_full_id_t new_focused_id
                                 = DATA_FULL_ID( DATA_TABLE_FEATURE, new_feature_id, classifier_id );
                            gui_marked_set_set_focused( (*this_).marker, new_focused_id, diag_id );
                            gui_marked_set_clear_selected_set( (*this_).marker );
                        }
                    }
                    else
                    {
                        U8_LOG_WARNING("unexpected state at gui_sketch_area_button_release_callback");
                    }
                }
                else
                {
                    U8_LOG_WARNING("invalid clicked object at gui_sketch_area_button_release_callback");
                }
            }
            else
            {
                U8_TRACE_INFO("according to drag-state, the button press was already handled at press-time");
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    /* stop dragging */
    gui_sketch_drag_state_stop_dragging ( &((*this_).drag_state) );

    /* mark dirty rect */
    gtk_widget_queue_draw( (*this_).drawing_area );

    U8_TRACE_END();
}

gboolean gui_sketch_area_key_press_callback( GtkEventControllerKey* self,
                                             guint keyval,
                                             guint keycode,
                                             GdkModifierType state,
                                             gpointer user_data )
{
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    gui_sketch_area_t *this_ = user_data;
    assert( NULL != this_ );

    const gboolean result_event_handled = gui_sketch_area_key_press( this_, 0 != (GDK_CONTROL_MASK&state), keyval );

    U8_TRACE_END();
    return result_event_handled;
}

bool gui_sketch_area_key_press( gui_sketch_area_t *this_, bool ctrl_state, guint keyval )
{
    U8_TRACE_BEGIN();
    gboolean result_event_handled = false;

    /* keys that have to be handled locally in the gui_sketch_area */
    /* becasue handling them globally would interfere with text entry fields */
    if ( ctrl_state )
    {
        if ( keyval == GDK_KEY_x )
        {
            U8_TRACE_INFO ( "key pressed: Ctrl-X" );
            gui_toolbox_cut( (*this_).toolbox );
            result_event_handled = true;
        }
        else if ( keyval == GDK_KEY_c )
        {
            U8_TRACE_INFO ( "key pressed: Ctrl-C" );
            gui_toolbox_copy( (*this_).toolbox );
            result_event_handled = true;
        }
        else if ( keyval == GDK_KEY_v )
        {
            U8_TRACE_INFO ( "key pressed: Ctrl-V" );
            gui_toolbox_paste( (*this_).toolbox );
            result_event_handled = true;
        }
        /* other keys are out of scope */
    }
    else
    {
        if ( keyval == GDK_KEY_Delete )
        {
            U8_TRACE_INFO ( "key pressed: DEL" );
            gui_toolbox_delete( (*this_).toolbox );
            result_event_handled = true;
        }
        /* other keys are out of scope */
    }

    U8_TRACE_END();
    return result_event_handled;
}

void gui_sketch_area_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer data )
{
    U8_TRACE_BEGIN();
    assert( NULL != msg );
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );
    assert ( NULL != widget );

    const data_change_event_type_t evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
    {
        /* abort dragging */
        gui_sketch_drag_state_reinit( &((*this_).drag_state) );

        /* do not keep search results */
        gui_sketch_result_list_invalidate_data( &((*this_).result_list) );

        /* do not keep selected or focued objects */
        gui_marked_set_reinit( (*this_).marker );

        /* reset the currently visible diagram */
        gui_sketch_request_reinit( &((*this_).request) );
    }

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_OPENED )
    {
        /* go to navigation mode, show root */
        gui_sketch_request_reinit( &((*this_).request) );
        gui_toolbox_set_selected_tool( (*this_).toolbox, GUI_TOOL_NAVIGATE );
        gui_sketch_area_show_diagram( this_, DATA_ID_VOID );
    }

    /* load/reload data to be drawn */
    gui_sketch_area_private_refocus_and_reload_data( this_ );

    /* mark dirty rect */
    gtk_widget_queue_draw( (*this_).drawing_area );

    U8_TRACE_END();
}

void gui_sketch_area_tool_changed_callback( GtkWidget *widget, gui_tool_t tool, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_sketch_area_t *this_ = data;
    assert( NULL != this_ );
    assert ( NULL != widget );

    /* info: This function is called once for activating a tool and once for deactiaving it! */
    if ( gui_sketch_request_get_tool_mode( &((*this_).request) ) != tool )
    {
        gui_sketch_request_set_tool_mode( &((*this_).request), tool );

        /* load/reload data to be drawn */
        gui_sketch_area_private_refocus_and_reload_data( this_ );

        /* mark dirty rect */
        gtk_widget_queue_draw( (*this_).drawing_area );
    }
    else
    {
        /* either the already selected tool is selected again or it is unselected */
        /* or the clear selection button was pressed */

        /* mark dirty rect */
        gtk_widget_queue_draw( (*this_).drawing_area );
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2024 Andreas Warnke

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
