/* File: gui_main_window.c; Copyright and License: see below */

#include "gui_main_window.h"
#include "gui_clipboard.h"
#include "u8/u8_trace.h"
#include "data_diagram_type.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include "storage/data_database.h"
#include "storage/data_change_notifier.h"
#include "meta/meta_info.h"
#ifndef NDEBUG
#include "u8stream/universal_stream_output_stream.h"
#include "utf8stream/utf8stream_writer.h"
#endif  /* not NDEBUG */
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_main_window_init( gui_main_window_t *this_,
                           ctrl_controller_t *controller,
                           io_data_file_t *data_file,
                           data_database_reader_t *db_reader,
                           gui_resources_t *res,
                           GtkApplication *gtk_app,
                           observer_t *window_close_observer,
                           observer_t *window_open_observer )
{
    U8_TRACE_BEGIN();
#if ( GTK_MAJOR_VERSION >= 4 )
    assert( gtk_app != NULL );
#else
    assert( gtk_app == NULL );
#endif

    /* init own attributes */
    (*this_).window_close_observer = window_close_observer;
    (*this_).window_open_observer = window_open_observer;
    (*this_).data_file = data_file;
#ifndef NDEBUG
    (*this_).controller = controller;
#endif  /* not NDEBUG */

    /* init window */
    {
#if ( GTK_MAJOR_VERSION >= 4 )
        (*this_).window = gtk_application_window_new( gtk_app );
#else
        (*this_).window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
#endif
        const char *window_title;
        window_title = io_data_file_get_filename_ptr( data_file );
        gtk_window_set_title(GTK_WINDOW( (*this_).window ), ( window_title == NULL ) ? META_INFO_PROGRAM_NAME_STR : window_title );
        gtk_widget_set_size_request( (*this_).window, 800, 400 );
        gtk_window_set_default_size( GTK_WINDOW( (*this_).window ), 16*70, 9*70 );
        gtk_window_set_icon( GTK_WINDOW( (*this_).window ), gui_resources_get_crystal_facet_uml( res ) );
    }

    /* init the message widgets */
    gui_main_window_private_init_simple_message_to_user( this_, res );
    gui_simple_message_to_user_init( &((*this_).message_to_user), (*this_).message_text_label, (*this_).message_icon_image, res );

    /* init the keyboard shortcuts */
    {
#if ( GTK_MAJOR_VERSION >= 4 )
        (*this_).keyboard_shortcut_ctrl = GTK_SHORTCUT_CONTROLLER(gtk_shortcut_controller_new());
        gtk_widget_add_controller( (*this_).window, GTK_EVENT_CONTROLLER((*this_).keyboard_shortcut_ctrl) );
#else
        (*this_).keyboard_shortcut_group = gtk_accel_group_new();
        gtk_window_add_accel_group(GTK_WINDOW( (*this_).window ), (*this_).keyboard_shortcut_group);
#endif
    }

    /* init tools */
    gui_main_window_private_init_toolbox( this_, res );

    /* determine the current/main clipboard */
    GdkClipboard *current_clipboard = NULL;
    {
#if ( GTK_MAJOR_VERSION >= 4 )
        current_clipboard = gtk_widget_get_clipboard( GTK_WIDGET((*this_).window) );  /* idea taken from gtk demo */
        /*
        GdkDisplay *const current_display = gdk_display_get_default();
        if ( current_display != NULL )
        {
            U8_TRACE_INFO_STR( "GdkDisplay:", gdk_display_get_name( current_display ) );
            current_clipboard = gdk_display_get_primary_clipboard( current_display );
            Alternative: current_clipboard = gdk_display_get_clipboard( current_display );
        }
        else
        {
            U8_LOG_WARNING( "No display could be found. No clipboard is available." );
        }
        */
#else
        GdkScreen *const current_screen = gtk_window_get_screen( GTK_WINDOW( (*this_).window ) );
        GdkDisplay *const current_display = gdk_screen_get_display( current_screen );
        current_clipboard = gtk_clipboard_get_for_display( current_display, GDK_NONE ); /* GDK_SELECTION_PRIMARY does not work */
#endif
    }

    gui_marked_set_init( &((*this_).marker_data), G_OBJECT((*this_).window) );
    gui_toolbox_init( &((*this_).tools_data),
                      GTK_WIDGET((*this_).tool_row),
                      GTK_WIDGET((*this_).view_navigate),
                      GTK_WIDGET((*this_).view_edit),
                      GTK_WIDGET((*this_).view_create),
                      GTK_WIDGET((*this_).view_search),
                      current_clipboard,
                      &((*this_).marker_data),
                      &((*this_).message_to_user),
                      db_reader,
                      controller
                    );

    /* init search widgets and sketch area */
    gui_main_window_private_init_search_and_sketch_area( this_, res );
    gui_search_runner_init( &((*this_).search_runner),
                            &((*this_).message_to_user),
                            db_reader,
                            io_data_file_get_database_ptr( data_file ),
                            &((*this_).sketcharea_data)
                          );
    gui_search_request_init( &((*this_).search_request),
                             (*this_).search_label,
                             (*this_).search_entry,
                             (*this_).search_button,
                             &((*this_).marker_data),
                             &((*this_).search_runner)
                           );
    gui_sketch_area_init( &((*this_).sketcharea_data),
                          (*this_).sketcharea,
                          &((*this_).marker_data),
                          &((*this_).tools_data),
                          &((*this_).message_to_user),
                          res,
                          controller,
                          db_reader
                        );

    /* init attribute editor widgets */
    gui_main_window_private_init_attributes_editor( this_, res );

    gui_attributes_editor_init( &((*this_).attributes_editor),
                                GTK_LABEL( (*this_).id_label ),
                                GTK_ENTRY( (*this_).name_entry ),
                                GTK_ENTRY( (*this_).stereotype_entry ),
                                GTK_COMBO_BOX( (*this_).type_combo_box ),
#if 0
                                GTK_ICON_VIEW( (*this_).type_icon_grid ),
#endif
                                GTK_WIDGET( (*this_).type_diag_grid ),
                                GTK_WIDGET( (*this_).type_clas_grid ),
                                GTK_WIDGET( (*this_).type_feat_grid ),
                                GTK_WIDGET( (*this_).type_rel_grid ),
                                GTK_TEXT_VIEW( (*this_).description_text_view ),
                                GTK_BUTTON( (*this_).file_save_button ),
                                res,
                                controller,
                                db_reader,
                                io_data_file_get_database_ptr( data_file ),
                                &((*this_).message_to_user)
                              );

    /* init the file choosers */
    gui_file_use_db_dialog_init( &((*this_).file_use_db_dialog),
                                 controller,
                                 data_file,
                                 GTK_WINDOW( (*this_).window ),
                                 &((*this_).message_to_user)
                               );
    gui_file_export_dialog_init( &((*this_).file_export_dialog),
                                 io_data_file_get_database_ptr( data_file ),
                                 db_reader,
                                 GTK_WINDOW( (*this_).window ),
                                 &((*this_).message_to_user)
                               );

    U8_TRACE_INFO("GTK+ Widgets are created.");

    (*this_).two_panes = gtk_paned_new( GTK_ORIENTATION_HORIZONTAL );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_paned_set_start_child( GTK_PANED((*this_).two_panes), (*this_).sketch_stack_column );
    gtk_paned_set_resize_start_child( GTK_PANED((*this_).two_panes), true );
    gtk_paned_set_shrink_start_child( GTK_PANED((*this_).two_panes), false );
    gtk_paned_set_end_child( GTK_PANED((*this_).two_panes), (*this_).attr_edit_column );
    gtk_paned_set_resize_end_child( GTK_PANED((*this_).two_panes), false );
    gtk_paned_set_shrink_end_child( GTK_PANED((*this_).two_panes), true );
#else
    gtk_paned_pack1( GTK_PANED((*this_).two_panes), (*this_).sketch_stack_column, true, false );
    gtk_paned_pack2( GTK_PANED((*this_).two_panes), (*this_).attr_edit_column, false, true );
#endif
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).sketch_stack_column ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).sketch_stack_column ), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).attr_edit_column ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).attr_edit_column ), false );
    gtk_paned_set_position( GTK_PANED((*this_).two_panes), 11*70 );

    (*this_).main_stack_column = gtk_box_new( GTK_ORIENTATION_VERTICAL, /*spacing:*/ 0 );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_box_append( GTK_BOX((*this_).main_stack_column), GTK_WIDGET((*this_).tool_row) );
    gtk_box_append( GTK_BOX((*this_).main_stack_column), GTK_WIDGET((*this_).two_panes) );
    gtk_box_append( GTK_BOX((*this_).main_stack_column), GTK_WIDGET((*this_).message_row) );
#else
    gtk_container_add( GTK_CONTAINER((*this_).main_stack_column), GTK_WIDGET((*this_).tool_row) );
    gtk_container_add( GTK_CONTAINER((*this_).main_stack_column), GTK_WIDGET((*this_).two_panes) );
    gtk_container_add( GTK_CONTAINER((*this_).main_stack_column), GTK_WIDGET((*this_).message_row) );
#endif
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).tool_row ), false );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).tool_row ), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).two_panes ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).two_panes ), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).message_row ), false );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).message_row ), true );

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_set_child( GTK_WINDOW((*this_).window), (*this_).main_stack_column );
    gtk_window_set_resizable( GTK_WINDOW((*this_).window), true );
#else
    gtk_container_add(GTK_CONTAINER((*this_).window), (*this_).main_stack_column);
#endif

    U8_TRACE_INFO("GTK+ Widgets are added to containers.");

    /* inject dependencies by signals */
    /* parameter info: g_signal_connect( instance-that-emits-the-signal, signal-name, callback-handler, data-to-be-passed-to-callback-handler) */
#if ( GTK_MAJOR_VERSION >= 4 )
    g_signal_connect( G_OBJECT((*this_).window), "close-request", G_CALLBACK(gui_main_window_delete_event_callback), this_ );
#else
    g_signal_connect( G_OBJECT((*this_).window), "delete-event", G_CALLBACK(gui_main_window_delete_event_callback), this_ );
#endif
    g_signal_connect( G_OBJECT((*this_).window), "destroy", G_CALLBACK(gui_main_window_destroy_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_main_window_data_changed_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).sketcharea), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_data_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).tool_row), GUI_TOOLBOX_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_tool_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).file_new_db), "clicked", G_CALLBACK(gui_main_window_new_db_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_use_db), "clicked", G_CALLBACK(gui_main_window_open_db_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_save_button), "clicked", G_CALLBACK(gui_main_window_save_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_export), "clicked", G_CALLBACK(gui_main_window_export_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).view_new_window), "clicked", G_CALLBACK(gui_main_window_new_window_btn_callback), this_ );

    g_signal_connect( G_OBJECT((*this_).view_navigate), "clicked", G_CALLBACK(gui_toolbox_navigate_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).view_edit), "clicked", G_CALLBACK(gui_toolbox_edit_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).view_create), "clicked", G_CALLBACK(gui_toolbox_create_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).view_search), "clicked", G_CALLBACK(gui_toolbox_search_btn_callback), &((*this_).tools_data) );

    /* The search id button is connected to two callback functions: one to switch to search mode, one to perform a search: */
    g_signal_connect( G_OBJECT((*this_).id_search_btn),
                      "clicked",
                      G_CALLBACK(gui_toolbox_search_id_btn_callback),
                      &((*this_).tools_data)
                    );
    g_signal_connect( G_OBJECT((*this_).id_search_btn),
                      "clicked",
                      G_CALLBACK(gui_search_request_id_search_callback),
                      &((*this_).search_request)
                    );

    g_signal_connect( G_OBJECT((*this_).tool_row), GUI_TOOLBOX_GLIB_SIGNAL_NAME, G_CALLBACK(gui_search_request_tool_changed_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).search_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_search_request_data_changed_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).search_entry), "activate", G_CALLBACK(gui_search_request_search_start_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).search_button), "clicked", G_CALLBACK(gui_search_request_search_start_callback), &((*this_).search_request) );

    g_signal_connect( G_OBJECT((*this_).edit_undo), "clicked", G_CALLBACK(gui_toolbox_undo_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_redo), "clicked", G_CALLBACK(gui_toolbox_redo_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_cut), "clicked", G_CALLBACK(gui_toolbox_cut_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_copy), "clicked", G_CALLBACK(gui_toolbox_copy_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_paste), "clicked", G_CALLBACK(gui_toolbox_paste_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_delete), "clicked", G_CALLBACK(gui_toolbox_delete_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_instantiate), "clicked", G_CALLBACK(gui_toolbox_instantiate_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_highlight), "clicked", G_CALLBACK(gui_toolbox_highlight_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_reset), "clicked", G_CALLBACK(gui_toolbox_reset_btn_callback), &((*this_).tools_data) );

    g_signal_connect( G_OBJECT((*this_).name_entry),
                      "activate",
                      G_CALLBACK(gui_attributes_editor_name_enter_callback),
                      &((*this_).attributes_editor)
                    );
#if ( GTK_MAJOR_VERSION >= 4 )
    {
        GtkEventControllerFocus *evt_focus_n = GTK_EVENT_CONTROLLER_FOCUS(gtk_event_controller_focus_new());
        g_signal_connect( evt_focus_n,
                          "leave",
                          G_CALLBACK(gui_attributes_editor_name_focus_left_callback),
                          &((*this_).attributes_editor)
                        );
        gtk_widget_add_controller( (*this_).name_entry, GTK_EVENT_CONTROLLER(evt_focus_n) );
    }
    {
        GtkEventControllerFocus *evt_focus_s = GTK_EVENT_CONTROLLER_FOCUS(gtk_event_controller_focus_new());
        g_signal_connect( evt_focus_s,
                          "leave",
                          G_CALLBACK(gui_attributes_editor_stereotype_focus_left_callback),
                          &((*this_).attributes_editor)
                        );
        gtk_widget_add_controller( (*this_).stereotype_entry, GTK_EVENT_CONTROLLER(evt_focus_s) );
    }
    {
        GtkEventControllerFocus *evt_focus_d = GTK_EVENT_CONTROLLER_FOCUS(gtk_event_controller_focus_new());
        g_signal_connect( evt_focus_d,
                          "leave",
                          G_CALLBACK(gui_attributes_editor_description_focus_left_callback),
                          &((*this_).attributes_editor)
                        );
        gtk_widget_add_controller( (*this_).description_text_view, GTK_EVENT_CONTROLLER(evt_focus_d) );
    }
#else
    g_signal_connect( G_OBJECT((*this_).name_entry),
                      "focus-out-event",
                      G_CALLBACK(gui_attributes_editor_name_focus_lost_callback),
                      &((*this_).attributes_editor)
                    );
    g_signal_connect( G_OBJECT((*this_).stereotype_entry),
                      "focus-out-event",
                      G_CALLBACK(gui_attributes_editor_stereotype_focus_lost_callback),
                      &((*this_).attributes_editor)
                    );
    g_signal_connect( G_OBJECT((*this_).description_text_view),
                      "focus-out-event",
                      G_CALLBACK(gui_attributes_editor_description_focus_lost_callback),
                      &((*this_).attributes_editor)
                    );
#endif
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), "activate", G_CALLBACK(gui_attributes_editor_stereotype_enter_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).type_combo_box), "changed", G_CALLBACK(gui_attributes_editor_type_changed_callback), &((*this_).attributes_editor) );
#if 0
    g_signal_connect( G_OBJECT((*this_).type_icon_grid), "item-activated", G_CALLBACK(gui_attributes_editor_type_shortlist_callback), &((*this_).attributes_editor) );
#endif
    g_signal_connect( G_OBJECT((*this_).window), GUI_MARKED_SET_GLIB_SIGNAL_NAME, G_CALLBACK(gui_attributes_editor_focused_object_changed_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).name_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_attributes_editor_data_changed_callback), &((*this_).attributes_editor) );
        /* ^-- name_entry is the  proxy for all widgets of attributes_editor */

    g_signal_connect( G_OBJECT((*this_).tool_about), "clicked", G_CALLBACK(gui_main_window_about_btn_callback), this_ );

    U8_TRACE_INFO("GTK+ Callbacks are connected to widget events.");

    /* register observers */

    (*this_).data_notifier = data_database_get_notifier_ptr( io_data_file_get_database_ptr( data_file ) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).window) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).sketcharea) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).name_entry) );
        /* ^-- name_entry is the  proxy for all widgets of attributes_editor */
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).search_entry) );

    U8_TRACE_INFO("GTK+ Widgets are registered as listeners at signal emitter.");

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_widget_show((*this_).window);
    gtk_widget_set_sensitive( GTK_WIDGET((*this_).window), TRUE );  /* idea taken from gtk demo */

    GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).window) );
    gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */
#else
    gtk_widget_show_all((*this_).window);
#endif
    gui_attributes_editor_update_widgets( &((*this_).attributes_editor) );  /* hide some widgets again */
#ifdef NDEBUG
    gui_simple_message_to_user_hide( &((*this_).message_to_user) );
#else
    gui_simple_message_to_user_show_message ( &((*this_).message_to_user), GUI_SIMPLE_MESSAGE_TYPE_INFO, GUI_SIMPLE_MESSAGE_CONTENT_DEBUG_MODE );
#endif
    gui_search_request_hide( &((*this_).search_request) );
    U8_TRACE_INFO("GTK+ Widgets are shown.");
    U8_TRACE_END();
}

void gui_main_window_destroy( gui_main_window_t *this_ )
{
    U8_TRACE_BEGIN();

    /* Note: The widgets may be destroyed already. A cast by G_OBJECT is therefore illegal. */
    data_change_notifier_remove_listener( (*this_).data_notifier, (GObject*)(*this_).window );
    data_change_notifier_remove_listener( (*this_).data_notifier, (GObject*)(*this_).name_entry );
    data_change_notifier_remove_listener( (*this_).data_notifier, (GObject*)(*this_).sketcharea );
    data_change_notifier_remove_listener( (*this_).data_notifier, (GObject*)(*this_).search_entry );

    U8_TRACE_INFO("GTK+ Widgets are unregistered as listeners from data module.");
    gui_file_export_dialog_destroy ( &((*this_).file_export_dialog) );
    gui_file_use_db_dialog_destroy ( &((*this_).file_use_db_dialog) );
    U8_TRACE_INFO("GTK+ hidden windows are destroyed.");

    gui_search_request_destroy( &((*this_).search_request) );
    gui_search_runner_destroy( &((*this_).search_runner) );
    gui_sketch_area_destroy( &((*this_).sketcharea_data) );
    gui_toolbox_destroy( &((*this_).tools_data) );
    gui_marked_set_destroy( &((*this_).marker_data) );
    gui_attributes_editor_destroy( &((*this_).attributes_editor) );
    gui_simple_message_to_user_destroy( &((*this_).message_to_user) );
    (*this_).data_file = NULL;
#ifndef NDEBUG
    (*this_).controller = NULL;
#endif  /* not NDEBUG */

    U8_TRACE_END();
}

#if ( GTK_MAJOR_VERSION >= 4 )
static inline void gtk_button_set_image( GtkButton *btn, GtkWidget *icon )
{
    gtk_button_set_child( btn, icon );
}
#else
#endif

void gui_main_window_private_init_toolbox( gui_main_window_t *this_, gui_resources_t *res )
{
    U8_TRACE_BEGIN();

    (*this_).file_new_db_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_new_db( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).file_new_db_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).file_new_db = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).file_new_db), (*this_).file_new_db_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_new_db), "New DB" );

    (*this_).file_use_db_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_use_db( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).file_use_db_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).file_use_db = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).file_use_db), (*this_).file_use_db_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_use_db), "Open DB" );

    (*this_).file_save_button = gtk_button_new();
    (*this_).file_save_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_save( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).file_save_icon), 32 /*=w*/ , 32 /*=h*/ );
    gtk_button_set_image( GTK_BUTTON((*this_).file_save_button), (*this_).file_save_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_save_button), "Save (Ctrl-S)" );
#if ( GTK_MAJOR_VERSION >= 4 )
    GtkShortcutTrigger *commit_trig = gtk_shortcut_trigger_parse_string( "<Control>S" );
    GtkShortcutAction *commit_act = gtk_callback_action_new( &gui_main_window_save_shortcut_callback,
                                                             this_,
                                                             NULL
                                                           );
    GtkShortcut* ctrl_s = gtk_shortcut_new_with_arguments( commit_trig,
                                                           commit_act,
                                                           NULL /* = format_string */
                                                         );
    gtk_shortcut_controller_add_shortcut( (*this_).keyboard_shortcut_ctrl, ctrl_s );
#else
    gtk_widget_add_accelerator( GTK_WIDGET((*this_).file_save_button),
                                "clicked",
                                (*this_).keyboard_shortcut_group,
                                GDK_KEY_s,
                                GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE
                              );
#endif

#if 0
    (*this_).file_save_as_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_save_as( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).file_save_as_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).file_save_as = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).file_save_as), (*this_).file_save_as_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_save_as), "Save as" );
#endif

    (*this_).file_export_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_export( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).file_export_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).file_export = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).file_export), (*this_).file_export_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_export), "Export" );

    (*this_).tool_sect_1_icon = gtk_image_new_from_pixbuf ( gui_resources_get_tool_sect( res ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).tool_sect_1_icon), 12 /*=w*/ , 32 /*=h*/ );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_image_set_pixel_size( GTK_IMAGE((*this_).tool_sect_1_icon), 32 );
#else
#endif
    gtk_widget_set_halign( (*this_).tool_sect_1_icon, GTK_ALIGN_START );

     (*this_).tool_sect_2_icon = gtk_image_new_from_pixbuf ( gui_resources_get_tool_sect( res ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).tool_sect_2_icon), 12 /*=w*/ , 32 /*=h*/ );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_image_set_pixel_size( GTK_IMAGE((*this_).tool_sect_2_icon), 32 );
#else
#endif
    gtk_widget_set_halign( (*this_).tool_sect_2_icon, GTK_ALIGN_START );

    (*this_).view_new_window_icon = gtk_image_new_from_pixbuf( gui_resources_get_view_new_window( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).view_new_window_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).view_new_window = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).view_new_window), (*this_).view_new_window_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).view_new_window), "New Window" );

    (*this_).view_navigate_icon = gtk_image_new_from_pixbuf( gui_resources_get_view_navigate( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).view_navigate_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).view_edit_icon = gtk_image_new_from_pixbuf( gui_resources_get_view_edit( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).view_edit_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).view_create_icon = gtk_image_new_from_pixbuf( gui_resources_get_view_create( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).view_create_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).view_search_icon = gtk_image_new_from_pixbuf( gui_resources_get_view_search( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).view_search_icon), 32 /*=w*/ , 32 /*=h*/ );
#if ( GTK_MAJOR_VERSION >= 4 )
    (*this_).view_navigate = gtk_toggle_button_new();
    gtk_button_set_child( GTK_BUTTON((*this_).view_navigate), (*this_).view_navigate_icon );

    (*this_).view_edit = gtk_toggle_button_new();
    gtk_button_set_child( GTK_BUTTON((*this_).view_edit), (*this_).view_edit_icon );
    gtk_toggle_button_set_group( GTK_TOGGLE_BUTTON((*this_).view_edit), GTK_TOGGLE_BUTTON((*this_).view_navigate) );

    (*this_).view_create = gtk_toggle_button_new();
    gtk_button_set_child( GTK_BUTTON((*this_).view_create), (*this_).view_create_icon );
    gtk_toggle_button_set_group( GTK_TOGGLE_BUTTON((*this_).view_create), GTK_TOGGLE_BUTTON((*this_).view_edit) );

    (*this_).view_search = gtk_toggle_button_new();
    gtk_button_set_child( GTK_BUTTON((*this_).view_search), (*this_).view_search_icon );
    gtk_toggle_button_set_group( GTK_TOGGLE_BUTTON((*this_).view_search), GTK_TOGGLE_BUTTON((*this_).view_create) );
#else
    GSList *group;

    (*this_).view_navigate = GTK_WIDGET(gtk_radio_tool_button_new( NULL ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).view_navigate), (*this_).view_navigate_icon);
    group = gtk_radio_tool_button_get_group(GTK_RADIO_TOOL_BUTTON ((*this_).view_navigate));

    (*this_).view_edit = GTK_WIDGET(gtk_radio_tool_button_new( group ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).view_edit), (*this_).view_edit_icon);
    group = gtk_radio_tool_button_get_group(GTK_RADIO_TOOL_BUTTON ((*this_).view_edit));

    (*this_).view_create = GTK_WIDGET(gtk_radio_tool_button_new( group ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).view_create), (*this_).view_create_icon);
    group = gtk_radio_tool_button_get_group(GTK_RADIO_TOOL_BUTTON ((*this_).view_create));

    (*this_).view_search = GTK_WIDGET(gtk_radio_tool_button_new( group ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).view_search), (*this_).view_search_icon);
#endif
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).view_navigate), "Navigate" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).view_edit), "Edit" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).view_create), "Create" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).view_search), "Search" );

    (*this_).edit_undo_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_undo( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_undo_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_undo = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_undo), (*this_).edit_undo_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_undo), "Undo (Ctrl-Z)" );
#if ( GTK_MAJOR_VERSION >= 4 )
    GtkShortcutTrigger *undo_trig = gtk_shortcut_trigger_parse_string( "<Control>Z" );
    GtkShortcutAction *undo_act = gtk_callback_action_new( &gui_toolbox_undo_shortcut_callback,
                                                           &((*this_).tools_data),
                                                           NULL
                                                         );
    GtkShortcut* ctrl_z = gtk_shortcut_new_with_arguments( undo_trig,
                                                           undo_act,
                                                           NULL /* = format_string */
                                                         );
    gtk_shortcut_controller_add_shortcut( (*this_).keyboard_shortcut_ctrl, ctrl_z );
#else
    gtk_widget_add_accelerator( GTK_WIDGET((*this_).edit_undo),
                                "clicked",
                                (*this_).keyboard_shortcut_group,
                                GDK_KEY_z,
                                GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE
                              );

#endif

    (*this_).edit_redo_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_redo( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_redo_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_redo = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_redo), (*this_).edit_redo_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_redo), "Redo (Ctrl-Y)" );
#if ( GTK_MAJOR_VERSION >= 4 )
    GtkShortcutTrigger *redo_trig = gtk_shortcut_trigger_parse_string( "<Control>Y" );
    GtkShortcutAction *redo_act = gtk_callback_action_new( &gui_toolbox_redo_shortcut_callback,
                                                           &((*this_).tools_data),
                                                           NULL
                                                         );
    GtkShortcut* ctrl_y = gtk_shortcut_new_with_arguments( redo_trig,
                                                           redo_act,
                                                           NULL /* = format_string */
                                                         );
    gtk_shortcut_controller_add_shortcut( (*this_).keyboard_shortcut_ctrl, ctrl_y );
#else
    gtk_widget_add_accelerator( GTK_WIDGET((*this_).edit_redo),
                                "clicked",
                                (*this_).keyboard_shortcut_group,
                                GDK_KEY_y,
                                GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE
                              );
#endif

    (*this_).edit_cut_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_cut( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_cut_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_cut = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_cut), (*this_).edit_cut_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_cut), "Cut (Ctrl-X)" );
    /*
    GDK_KEY_x + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_copy_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_copy( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_copy_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_copy = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_copy), (*this_).edit_copy_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_copy), "Copy (Ctrl-C)" );
    /*
    GDK_KEY_c + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_paste_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_paste( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_paste_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_paste = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_paste), (*this_).edit_paste_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_paste), "Paste (Ctrl-V)" );
    /*
    GK_KEY_v + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_delete_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_delete( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_delete_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_delete = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_delete), (*this_).edit_delete_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_delete), "Delete (DEL)" );
    /*
    GDK_KEY_Delete handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_instantiate_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_instantiate( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_instantiate_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_instantiate = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_instantiate), (*this_).edit_instantiate_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_instantiate), "Instantiate" );

    (*this_).edit_highlight_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_highlight( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_highlight_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_highlight = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_highlight), (*this_).edit_highlight_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_highlight), "Highlight" );

    (*this_).edit_reset_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_reset( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).edit_reset_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).edit_reset = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).edit_reset), (*this_).edit_reset_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_reset), "Reset Selection" );

    (*this_).tool_about_icon = gtk_image_new_from_pixbuf( gui_resources_get_crystal_facet_uml( res ));
    gtk_widget_set_size_request( GTK_WIDGET((*this_).tool_about_icon), 32 /*=w*/ , 32 /*=h*/ );
    (*this_).tool_about = GTK_BUTTON(gtk_button_new());
    gtk_button_set_image( GTK_BUTTON((*this_).tool_about), (*this_).tool_about_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_about), "About" );

    /* insert widgets to box container */
    {
        (*this_).tool_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 4 );
#if ( GTK_MAJOR_VERSION >= 4 )
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).file_new_db) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).file_use_db) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).file_save_button) );
        /* gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).file_save_as) ); */
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).file_export) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).tool_sect_1_icon) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).view_new_window) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).view_search) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).view_navigate) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).view_edit) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).view_create) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).tool_sect_2_icon) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_undo) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_redo) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_cut) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_copy) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_paste) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_delete) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_instantiate) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_highlight) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).edit_reset) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).tool_about) );
#else
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).file_new_db) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).file_use_db) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).file_save_button) );
        /* gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).file_save_as) ); */
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).file_export) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).tool_sect_1_icon) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).view_new_window) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).view_search) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).view_navigate) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).view_edit) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).view_create) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).tool_sect_2_icon) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_undo) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_redo) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_cut) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_copy) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_paste) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_delete) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_instantiate) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_highlight) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).edit_reset) );
        gtk_container_add( GTK_CONTAINER((*this_).tool_row), GTK_WIDGET((*this_).tool_about) );
#endif
    }

    U8_TRACE_END();
}

void gui_main_window_private_init_attributes_editor( gui_main_window_t *this_, gui_resources_t *res )
{
    U8_TRACE_BEGIN();

    (*this_).attr_section_icon = gtk_image_new_from_pixbuf ( gui_resources_get_edit_attributes_sect( res ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).attr_section_icon), 48 /*=w*/ , 12 /*=h*/ );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_image_set_pixel_size( GTK_IMAGE((*this_).attr_section_icon), 48 );
#else
#endif
    gtk_widget_set_halign( (*this_).attr_section_icon, GTK_ALIGN_START );

    (*this_).id_label = gtk_label_new( "" );
    gtk_label_set_selectable( GTK_LABEL( (*this_).id_label ), true );
    (*this_).name_label = gtk_label_new( "Name:" );
    (*this_).description_label = gtk_label_new( "Description:" );
    (*this_).stereotype_label = gtk_label_new( "Stereotype:" );
    (*this_).type_label = gtk_label_new( "Type:" );
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION >= 4 ))
    gtk_label_set_xalign( GTK_LABEL( (*this_).id_label ), 1.0 ); /* align right */
    gtk_label_set_xalign( GTK_LABEL( (*this_).name_label ), 0.0 );
    gtk_label_set_xalign( GTK_LABEL( (*this_).description_label ), 0.0 );
    gtk_label_set_xalign( GTK_LABEL( (*this_).stereotype_label ), 0.0 );
    gtk_label_set_xalign( GTK_LABEL( (*this_).type_label ), 0.0 );
#else
    gtk_misc_set_alignment( GTK_MISC( (*this_).id_label ), 1.0, 0.0 );
    gtk_misc_set_alignment( GTK_MISC( (*this_).name_label ), 0.0, 0.0 );
    gtk_misc_set_alignment( GTK_MISC( (*this_).description_label ), 0.0, 0.0 );
    gtk_misc_set_alignment( GTK_MISC( (*this_).stereotype_label ), 0.0, 0.0 );
    gtk_misc_set_alignment( GTK_MISC( (*this_).type_label ), 0.0, 0.0 );
#endif

    (*this_).id_search_btn = gtk_button_new();
    (*this_).id_search_btn_icon = gtk_image_new_from_pixbuf( gui_resources_get_search_search( res ));
    gtk_button_set_image( GTK_BUTTON((*this_).id_search_btn), (*this_).id_search_btn_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).id_search_btn), "Find Occurrences" );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).id_search_btn_icon), 32 /*=w*/ , 32 /*=h*/ );

    /* insert widgets to box container */
    {
        (*this_).id_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 8 );
#if ( GTK_MAJOR_VERSION >= 4 )
        gtk_box_append( GTK_BOX((*this_).id_row), GTK_WIDGET((*this_).attr_section_icon) );
        gtk_box_append( GTK_BOX((*this_).id_row), GTK_WIDGET((*this_).id_label) );
        gtk_box_append( GTK_BOX((*this_).id_row), GTK_WIDGET((*this_).id_search_btn) );
#else
        gtk_container_add( GTK_CONTAINER((*this_).id_row), GTK_WIDGET((*this_).attr_section_icon) );
        gtk_container_add( GTK_CONTAINER((*this_).id_row), GTK_WIDGET((*this_).id_label) );
        gtk_container_add( GTK_CONTAINER((*this_).id_row), GTK_WIDGET((*this_).id_search_btn) );
#endif
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).attr_section_icon ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).attr_section_icon ), false );
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).id_label ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).id_label ), true );
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).id_search_btn ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).id_search_btn ), false );
    }

    (*this_).name_entry = gtk_entry_new();

    (*this_).stereotype_entry = gtk_entry_new();

    (*this_).description_text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode( GTK_TEXT_VIEW( (*this_).description_text_view ),
                                 GTK_WRAP_WORD_CHAR
                               );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).description_text_view ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).description_text_view ), false );
    //gtk_widget_set_size_request( GTK_WIDGET((*this_).description_text_view), 128 /*=w*/ , 48 /*=h*/ );
    /* need own scroll window container */
#if ( GTK_MAJOR_VERSION >= 4 )
    (*this_).description_scroll_win = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child( GTK_SCROLLED_WINDOW((*this_).description_scroll_win), (*this_).description_text_view );
#else
    (*this_).description_scroll_win = gtk_scrolled_window_new( NULL, NULL );
    gtk_container_set_border_width( GTK_CONTAINER( (*this_).description_text_view ), 12 );
    gtk_container_add( GTK_CONTAINER( (*this_).description_scroll_win ), (*this_).description_text_view );
#endif
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( (*this_).description_scroll_win ),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC
                                  );

    (*this_).type_combo_box = gtk_combo_box_new();
    GtkCellRenderer *column1;
    column1 = gtk_cell_renderer_pixbuf_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT((*this_).type_combo_box), column1, /*expand:*/ FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT((*this_).type_combo_box), column1, "pixbuf", 2, NULL);
    GtkCellRenderer *column2;
    column2 = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT((*this_).type_combo_box), column2, /*expand:*/ TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT((*this_).type_combo_box), column2, "text", 1, NULL);

#if 0
    (*this_).type_icon_grid = gtk_icon_view_new();
    gtk_widget_set_halign( (*this_).type_icon_grid, GTK_ALIGN_END );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_icon_view_set_activate_on_single_click( GTK_ICON_VIEW((*this_).type_icon_grid), true );
#else
#endif
    gtk_icon_view_set_tooltip_column( GTK_ICON_VIEW((*this_).type_icon_grid), 1 );
    gtk_icon_view_set_pixbuf_column( GTK_ICON_VIEW((*this_).type_icon_grid), 2 );
    gtk_icon_view_set_selection_mode( GTK_ICON_VIEW((*this_).type_icon_grid), GTK_SELECTION_NONE );
    gtk_icon_view_set_item_padding( GTK_ICON_VIEW((*this_).type_icon_grid), 2 );
    gtk_icon_view_set_margin( GTK_ICON_VIEW((*this_).type_icon_grid), 0 );
    gtk_icon_view_set_column_spacing( GTK_ICON_VIEW((*this_).type_icon_grid), 4 );
    gtk_icon_view_set_row_spacing( GTK_ICON_VIEW((*this_).type_icon_grid), 0 );
    gtk_icon_view_set_columns( GTK_ICON_VIEW((*this_).type_icon_grid), 7 );
#if ( GTK_MAJOR_VERSION >= 4 )
    /* workaround for endless loop in gtk layouting, possibly fixed in 4.6.9? */
    GtkWidget *grid_frame = gtk_grid_new();
    gtk_grid_attach( GTK_GRID(grid_frame), (*this_).type_icon_grid, 0, 0, 1, 1 );
    gtk_widget_set_halign( grid_frame, GTK_ALIGN_END );
    //gtk_orientable_set_orientation( GTK_ORIENTABLE(grid_frame), GTK_ORIENTATION_HORIZONTAL );
    gtk_widget_set_vexpand( GTK_WIDGET(grid_frame), FALSE );
    gtk_widget_set_hexpand( GTK_WIDGET(grid_frame), TRUE );
#else
#endif
#endif

    static gui_resource_selector_t res_select;
    gui_resource_selector_init( &res_select, res );
    {
        (*this_).type_diag_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_diag_grid ), GTK_ALIGN_END );
        for( int_fast32_t diag_idx = 0; diag_idx < DATA_DIAGRAM_TYPE_COUNT; diag_idx ++ )
        {
            const data_diagram_type_t diag_type = DATA_DIAGRAM_TYPE_ARRAY[ diag_idx ];
            gui_attribute_type_of_diagram_init( &((*this_).type_diag_data[diag_idx]), diag_type, &((*this_).attributes_editor) );
            const GdkPixbuf *const diag_icon = gui_resource_selector_get_diagram_icon( &res_select, diag_type );
            (*this_).type_diag_img[ diag_idx ] = GTK_IMAGE( gtk_image_new_from_pixbuf( (void*) diag_icon ) );
            gtk_widget_set_size_request( GTK_WIDGET( (*this_).type_diag_img[ diag_idx ] ), 32 /*=w*/ , 24 /*=h*/ );
            (*this_).type_diag_btn[ diag_idx ] = GTK_BUTTON( gtk_button_new() );
            gtk_button_set_image( (*this_).type_diag_btn[ diag_idx ], GTK_WIDGET( (*this_).type_diag_img[ diag_idx ] ) );
            gtk_widget_set_tooltip_text( GTK_WIDGET( (*this_).type_diag_btn[ diag_idx ] ), "TODO" );
            gtk_grid_attach( (*this_).type_diag_grid, GTK_WIDGET( (*this_).type_diag_btn[ diag_idx ] ), diag_idx%7, diag_idx/7, 1, 1 );
        }
        (*this_).type_clas_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_clas_grid ), GTK_ALIGN_END );
        {

        }
        (*this_).type_feat_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_feat_grid ), GTK_ALIGN_END );
        {

        }
        (*this_).type_rel_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_rel_grid ), GTK_ALIGN_END );
        {

        }
    }
    gui_resource_selector_destroy( &res_select );

#if 0
    GtkGrid   *type_diag_grid;
    GtkImage  *( type_diag_img[ DATA_DIAGRAM_TYPE_COUNT ] );
    GtkButton *( type_diag_btn[ DATA_DIAGRAM_TYPE_COUNT ] );
    gui_attribute_type_of_diagram_t type_diag_data[ DATA_DIAGRAM_TYPE_COUNT ];
    GtkGrid   *type_clas_grid;
    GtkImage  *( type_clas_img[ DATA_CLASSIFIER_TYPE_COUNT ] );
    GtkButton *( type_clas_btn[ DATA_CLASSIFIER_TYPE_COUNT ] );
    gui_attribute_type_of_classifier_t type_clas_data[ DATA_CLASSIFIER_TYPE_COUNT ];
    GtkGrid   *type_feat_grid;
    GtkImage  *( type_feat_img[ DATA_FEATURE_TYPE_COUNT ] );
    GtkButton *( type_feat_btn[ DATA_FEATURE_TYPE_COUNT ] );
    gui_attribute_type_of_feature_t type_feat_data[ DATA_FEATURE_TYPE_COUNT ];
    GtkGrid   *type_rel_grid;
    GtkImage  *( type_rel_img[ DATA_RELATIONSHIP_TYPE_COUNT ] );
    GtkButton *( type_rel_btn[ DATA_RELATIONSHIP_TYPE_COUNT ] );
    gui_attribute_type_of_relationship_t type_rel_data[ DATA_RELATIONSHIP_TYPE_COUNT ];

    /* TODO Just a test, how to get rid of the unsupporrted gtk_icon_view_new() */
    GtkWidget *(img[7]);
    GtkWidget *(btn[7]);
    GtkWidget *quick_grid = gtk_grid_new();
    //gtk_grid_attach( GTK_GRID(quick_grid), gtk_label_new( "  Quick:" ), 0, 0, 1, 1 );
    gtk_widget_set_halign( quick_grid, GTK_ALIGN_END );
    for ( int idx = 0; idx < 7; idx ++ )
    {
        img[idx] = gtk_image_new_from_pixbuf( (void*) gui_resources_get_type_clas_class( res ));
        gtk_widget_set_size_request( img[idx], 32 /*=w*/ , 24 /*=h*/ );
        btn[idx] = gtk_button_new();
        gtk_button_set_image( GTK_BUTTON(btn[idx]), img[idx] );
        gtk_widget_set_tooltip_text( btn[idx], "New DB" );
        gtk_grid_attach( GTK_GRID(quick_grid), btn[idx], idx+1, 0, 1, 1 );
    }
#endif

    /* insert widgets to box container */
    {
        (*this_).attr_edit_column = gtk_box_new( GTK_ORIENTATION_VERTICAL, /*spacing:*/ 4 );
#if ( GTK_MAJOR_VERSION >= 4 )
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).id_row) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).name_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).name_entry) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).stereotype_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).stereotype_entry) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_label) );
#if 0
        //gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_icon_grid) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET(grid_frame) );
#endif
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_combo_box) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET( (*this_).type_diag_grid ) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).description_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).description_scroll_win) );
#else
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).id_row) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).name_label) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).name_entry) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).stereotype_label) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).stereotype_entry) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).type_label) );
#if 0
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).type_icon_grid) );
#endif
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).type_combo_box) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET( (*this_).type_diag_grid ) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).description_label) );
        gtk_container_add( GTK_CONTAINER((*this_).attr_edit_column), GTK_WIDGET((*this_).description_scroll_win) );
#endif
        gtk_widget_set_vexpand( GTK_WIDGET( (*this_).description_scroll_win ), true );
        gtk_widget_set_hexpand( GTK_WIDGET( (*this_).description_scroll_win ), true );
    }

    U8_TRACE_END();
}

void gui_main_window_private_init_simple_message_to_user( gui_main_window_t *this_, gui_resources_t *res )
{
    U8_TRACE_BEGIN();

    (*this_).message_text_label = gtk_label_new( "" );
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION >= 4 ))
    gtk_label_set_xalign(GTK_LABEL( (*this_).message_text_label ), 0.0 );
#else
    gtk_misc_set_alignment(GTK_MISC( (*this_).message_text_label ), 0.0, 0.0 );
#endif

    (*this_).message_icon_image = gtk_image_new_from_pixbuf ( gui_resources_get_crystal_facet_uml( res ) );
#if ( GTK_MAJOR_VERSION >= 4 )
    /* TODO check if GtkPicture is better suited than GtkImage because of setting the size */
    // gtk_widget_set_size_request( GTK_WIDGET((*this_).message_icon_image), 32 /*=w*/ , 32 /*=h*/ );
    // gtk_image_set_pixel_size( GTK_IMAGE((*this_).message_icon_image), 32 );
#else
#endif
    gtk_widget_set_size_request( GTK_WIDGET((*this_).message_icon_image), 32 /*=w*/ , 32 /*=h*/ );

    /* insert widgets to box container */
    {
        (*this_).message_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 20 );
#if ( GTK_MAJOR_VERSION >= 4 )
        gtk_box_append( GTK_BOX((*this_).message_row), GTK_WIDGET((*this_).message_icon_image) );
        gtk_box_append( GTK_BOX((*this_).message_row), GTK_WIDGET((*this_).message_text_label) );
#else
        gtk_container_add( GTK_CONTAINER((*this_).message_row), GTK_WIDGET((*this_).message_icon_image) );
        gtk_container_add( GTK_CONTAINER((*this_).message_row), GTK_WIDGET((*this_).message_text_label) );
#endif
        gtk_widget_set_vexpand( GTK_WIDGET( (*this_).message_icon_image ), false );
        gtk_widget_set_hexpand( GTK_WIDGET( (*this_).message_icon_image ), false );
        gtk_widget_set_vexpand( GTK_WIDGET( (*this_).message_text_label ), false );
        gtk_widget_set_hexpand( GTK_WIDGET( (*this_).message_text_label ), true );
    }

    U8_TRACE_END();
}

void gui_main_window_private_init_search_and_sketch_area( gui_main_window_t *this_, gui_resources_t *res )
{
    U8_TRACE_BEGIN();

    /* init search widgets */
    {
        (*this_).search_label = gtk_label_new ( "Search:" );
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION > 3 ))
        gtk_label_set_xalign( GTK_LABEL( (*this_).search_label ), 1.0 ); /* align right */
        gtk_label_set_yalign( GTK_LABEL( (*this_).search_label ), 0.5 ); /* align middle-height */
#else
        gtk_misc_set_alignment( GTK_MISC( (*this_).search_label ), 1.0, 0.5 );
#endif

        (*this_).search_entry = gtk_entry_new();

        (*this_).search_button = gtk_button_new();
        (*this_).search_btn_icon = gtk_image_new_from_pixbuf( gui_resources_get_search_search( res ));
        gtk_button_set_image( GTK_BUTTON((*this_).search_button), (*this_).search_btn_icon );
        gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).search_button), "Search" );
        gtk_widget_set_size_request( GTK_WIDGET((*this_).search_btn_icon), 32 /*=w*/ , 32 /*=h*/ );
    }

    /* insert widgets to box container */
    {
        (*this_).search_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 4 );
#if ( GTK_MAJOR_VERSION >= 4 )
        gtk_box_append( GTK_BOX((*this_).search_row), GTK_WIDGET((*this_).search_label) );
        gtk_box_append( GTK_BOX((*this_).search_row), GTK_WIDGET((*this_).search_entry) );
        gtk_box_append( GTK_BOX((*this_).search_row), GTK_WIDGET((*this_).search_button) );
#else
        gtk_container_add( GTK_CONTAINER((*this_).search_row), GTK_WIDGET((*this_).search_label) );
        gtk_container_add( GTK_CONTAINER((*this_).search_row), GTK_WIDGET((*this_).search_entry) );
        gtk_container_add( GTK_CONTAINER((*this_).search_row), GTK_WIDGET((*this_).search_button) );
#endif
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_label ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_label ), false );
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_entry ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_entry ), true );
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_button ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_button ), false );
    }

    /* init sketch area */
    {
        (*this_).sketcharea = gtk_drawing_area_new();
#if ( GTK_MAJOR_VERSION >= 4 )
#else
        gtk_widget_set_events( (*this_).sketcharea, GDK_LEAVE_NOTIFY_MASK
            | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
#endif
        gtk_widget_set_can_focus( GTK_WIDGET( (*this_).sketcharea ), TRUE );  /* this allows the text entry widgets to lose the focus */
        gtk_widget_set_size_request( GTK_WIDGET( (*this_).sketcharea ), 600, 360);  /* set a minimum initial size */
        /*gtk_widget_set_focus_on_click( GTK_WIDGET( (*this_).sketcharea ), TRUE ); not yet existing: since GTK 3.2 */
    }

    /* insert widgets to box container */
    {
        (*this_).sketch_stack_column = gtk_box_new( GTK_ORIENTATION_VERTICAL, /*spacing:*/ 0 );
#if ( GTK_MAJOR_VERSION >= 4 )
        gtk_box_append( GTK_BOX((*this_).sketch_stack_column), GTK_WIDGET((*this_).search_row) );
        gtk_box_append( GTK_BOX((*this_).sketch_stack_column), GTK_WIDGET((*this_).sketcharea) );
#else
        gtk_container_add( GTK_CONTAINER((*this_).sketch_stack_column), GTK_WIDGET((*this_).search_row) );
        gtk_container_add( GTK_CONTAINER((*this_).sketch_stack_column), GTK_WIDGET((*this_).sketcharea) );
#endif
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_row ), false );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_row ), true );
        gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).sketcharea ), true );
        gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).sketcharea ), true );
    }

    U8_TRACE_END();
}

void gui_main_window_destroy_event_callback( GtkWidget *widget, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;
    assert( (*this_).window == (void*)widget );

    /* forward destroy request to gui_window_manager: */
    observer_notify( (*this_).window_close_observer, this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

#if ( GTK_MAJOR_VERSION >= 4 )
gboolean gui_main_window_delete_event_callback( GtkWindow *widget, gpointer data )
#else
gboolean gui_main_window_delete_event_callback( GtkWidget *widget, GdkEvent *event, gpointer data )
#endif
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;
    assert( (*this_).window == (void*)widget );
    (void) this_;  /* unused in case of NDEBUG */

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
    return false;  /* return false to trigger destroy event */
}

void gui_main_window_new_db_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gui_file_use_db_dialog_show( &((*this_).file_use_db_dialog), false /* open_existing*/ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_main_window_open_db_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gui_file_use_db_dialog_show( &((*this_).file_use_db_dialog), true /* open_existing*/ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_main_window_save_btn_callback( GtkButton *button, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_;
    this_ = (gui_main_window_t*) user_data;
    assert( this_ != NULL );

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gui_attributes_editor_commit_changes ( &((*this_).attributes_editor) );

    u8_error_t d_err;
    d_err = U8_ERROR_NONE;
    d_err |= io_data_file_trace_stats( (*this_).data_file );
    d_err |= io_data_file_sync_to_disk( (*this_).data_file );
    d_err |= io_data_file_trace_stats( (*this_).data_file );
    if ( U8_ERROR_NONE != d_err )
    {
        gui_simple_message_to_user_show_message( &((*this_).message_to_user),
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_WRITE_ERROR
        );
    }
    else
    {
#ifndef NDEBUG
        /* in debug mode, also check consistency of database */
        universal_stream_output_stream_t out_stream;
        universal_stream_output_stream_init( &out_stream, stdout );
        universal_output_stream_t *const out_base = universal_stream_output_stream_get_output_stream( &out_stream );
        utf8stream_writer_t out_report;
        utf8stream_writer_init( &out_report, out_base );
        uint32_t found_errors;
        uint32_t fixed_errors;
        ctrl_controller_repair_database( (*this_).controller, false /* no repair, just test */, &found_errors, &fixed_errors, &out_report );
        if (( found_errors != 0 ) || ( fixed_errors != 0 ))
        {
            gui_simple_message_to_user_show_message_with_quantity( &((*this_).message_to_user),
                                                                   GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                   GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT,
                                                                   found_errors
                                                                 );
        }
        utf8stream_writer_destroy( &out_report );
        universal_stream_output_stream_destroy( &out_stream );
#endif
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

#if ( GTK_MAJOR_VERSION >= 4 )
gboolean gui_main_window_save_shortcut_callback( GtkWidget* widget, GVariant* args, gpointer user_data )
{
    gui_main_window_save_btn_callback( (GtkButton*)widget, user_data );
    return TRUE;
}
#else
#endif

void gui_main_window_export_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gui_file_export_dialog_show ( &((*this_).file_export_dialog) );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_main_window_new_window_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    /* hide last message */
    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    /* forward new window request to gui_window_manager: */
    observer_notify( (*this_).window_open_observer, &((*this_).message_to_user) );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_main_window_about_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    if ( GUI_SIMPLE_MESSAGE_TYPE_ABOUT == gui_simple_message_to_user_get_type_id( &((*this_).message_to_user) ) )
    {
        /* hide the already visible about message */
        gui_simple_message_to_user_hide( &((*this_).message_to_user) );
    }
    else
    {
        gui_simple_message_to_user_show_message( &((*this_).message_to_user),
                                                 GUI_SIMPLE_MESSAGE_TYPE_ABOUT,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_ABOUT
                                               );
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_main_window_data_changed_callback( GtkWidget *window, data_change_message_t *msg, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = user_data;

    if (( DATA_CHANGE_EVENT_TYPE_DB_OPENED == data_change_message_get_event( msg ) )
        || ( DATA_CHANGE_EVENT_TYPE_DB_CLOSED == data_change_message_get_event( msg ) ))
    {
        /* the database has changed */
        const char *filename = io_data_file_get_filename_ptr( (*this_).data_file );
        if ( NULL == filename )
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), META_INFO_PROGRAM_NAME_STR );
        }
        else
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), filename );
        }
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}


/*
Copyright 2016-2023 Andreas Warnke

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
