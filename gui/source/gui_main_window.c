/* File: gui_main_window.c; Copyright and License: see below */

#include "gui_main_window.h"
#include "gui_clipboard.h"
#include "gui_type_resource.h"
#include "u8/u8_trace.h"
#include "entity/data_diagram_type.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship_type.h"
#include "storage/data_database.h"
#include "storage/data_change_notifier.h"
#include "meta/meta_info.h"
#include "gui_gtk.h"
#include "gui_gdk.h"
#include <stdio.h>
#include <stdbool.h>

void gui_main_window_init( gui_main_window_t *this_,
                           ctrl_controller_t *controller,
                           io_data_file_t *data_file,
                           data_database_reader_t *db_reader,
                           gui_resources_t *resources,
                           GtkApplication *gtk_app,
                           observer_t *window_close_observer,
                           observer_t *window_open_observer )
{
    U8_TRACE_BEGIN();
    assert( gtk_app != NULL );

    /* init own attributes */
    (*this_).window_close_observer = window_close_observer;
    (*this_).window_open_observer = window_open_observer;
    (*this_).data_file = data_file;
    (*this_).resources = resources;

    /* init window */
    {
        (*this_).window = gtk_application_window_new( gtk_app );
        const char *window_title;
        window_title = io_data_file_get_filename_const( data_file );
        gtk_window_set_title(GTK_WINDOW( (*this_).window ), ( window_title == NULL ) ? META_INFO_PROGRAM_NAME_STR : window_title );
        gtk_widget_set_size_request( (*this_).window, 800, 400 );
        gtk_window_set_default_size( GTK_WINDOW( (*this_).window ), 16*70, 9*70 );
    }

    /* init the message widgets */
    gui_main_window_private_init_simple_message_to_user( this_ );
    gui_simple_message_to_user_init( &((*this_).message_to_user), (*this_).message_text_label, (*this_).message_icon_image, resources );

    /* init the keyboard shortcuts */
    {
        (*this_).keyboard_shortcut_ctrl = GTK_SHORTCUT_CONTROLLER(gtk_shortcut_controller_new());
        gtk_widget_add_controller( (*this_).window, GTK_EVENT_CONTROLLER((*this_).keyboard_shortcut_ctrl) );
    }

    /* init tools */
    gui_main_window_private_init_toolbox( this_ );

    /* determine the current/main clipboard */
    GdkClipboard *current_clipboard = NULL;
    {
        current_clipboard = gtk_widget_get_clipboard( GTK_WIDGET((*this_).window) );  /* idea taken from gtk demo */
    }

    gui_marked_set_init( &((*this_).marker_data),
                         G_OBJECT((*this_).window),
                         &gui_sketch_area_show_diagram,
                         &((*this_).sketcharea_data)
                       );
    gui_toolbox_init( &((*this_).tools_data),
                      GTK_WIDGET( (*this_).tool_row ),
                      gui_button_get_widget_ptr( &((*this_).view_navigate) ),
                      gui_button_get_widget_ptr( &((*this_).view_edit) ),
                      gui_button_get_widget_ptr( &((*this_).view_create) ),
                      gui_button_get_widget_ptr( &((*this_).view_search) ),
                      current_clipboard,
                      &((*this_).marker_data),
                      &((*this_).message_to_user),
                      db_reader,
                      controller
                    );

    /* init search widgets and sketch area */
    observer_t search_result_observer;
    observer_init( &search_result_observer,
                   &((*this_).sketcharea_data),
                   (void(*)(void*,void*))&gui_sketch_area_show_result_list,
                   "search_results_available"
                 );
    gui_main_window_private_init_search_and_sketch_area( this_ );
    gui_search_runner_init( &((*this_).search_runner),
                            &((*this_).message_to_user),
                            db_reader,
                            io_data_file_get_database_ptr( data_file ),
                            search_result_observer
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
                          &((*this_).search_runner),
                          &((*this_).message_to_user),
                          resources,
                          controller,
                          db_reader
                        );

    /* init attribute editor widgets */
    gui_main_window_private_init_attributes_editor( this_ );

    gui_attributes_editor_init( &((*this_).attributes_editor),
                                GTK_LABEL( (*this_).id_label ),
                                GTK_ENTRY( (*this_).name_entry ),
                                GTK_ENTRY( (*this_).stereotype_entry ),
                                (*this_).type_dropdown,
                                GTK_WIDGET( (*this_).type_diag_grid ),
                                GTK_WIDGET( (*this_).type_clas_grid ),
                                GTK_WIDGET( (*this_).type_feat_grid ),
                                GTK_WIDGET( (*this_).type_rel_grid ),
                                GTK_TEXT_VIEW( (*this_).description_text_view ),
                                gui_button_get_button_ptr( &((*this_).file_save ) ),
                                resources,
                                controller,
                                db_reader,
                                io_data_file_get_database_ptr( data_file ),
                                &((*this_).message_to_user)
                              );

    /* init the file choosers */
    gui_file_action_init( &((*this_).file_action),
                          controller,
                          data_file,
                          &((*this_).message_to_user)
                        );
    gui_file_use_db_dialog_init( &((*this_).file_use_db_dialog),
                                 GTK_WINDOW( (*this_).window ),
                                 &((*this_).file_action)
                               );
    gui_file_export_dialog_init( &((*this_).file_export_dialog),
                                 io_data_file_get_database_ptr( data_file ),
                                 db_reader,
                                 GTK_WINDOW( (*this_).window ),
                                 &((*this_).message_to_user)
                               );

    U8_TRACE_INFO("GTK+ Widgets are created.");

    (*this_).two_panes = gtk_paned_new( GTK_ORIENTATION_HORIZONTAL );
    gtk_paned_set_start_child( GTK_PANED((*this_).two_panes), (*this_).sketch_stack_column );
    gtk_paned_set_resize_start_child( GTK_PANED((*this_).two_panes), true );
    gtk_paned_set_shrink_start_child( GTK_PANED((*this_).two_panes), false );
    gtk_paned_set_end_child( GTK_PANED((*this_).two_panes), (*this_).attr_edit_column );
    gtk_paned_set_resize_end_child( GTK_PANED((*this_).two_panes), false );
    gtk_paned_set_shrink_end_child( GTK_PANED((*this_).two_panes), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).sketch_stack_column ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).sketch_stack_column ), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).attr_edit_column ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).attr_edit_column ), false );
    gtk_paned_set_position( GTK_PANED((*this_).two_panes), 11*70 );

    (*this_).main_stack_column = gtk_box_new( GTK_ORIENTATION_VERTICAL, /*spacing:*/ 0 );
    gtk_box_append( GTK_BOX((*this_).main_stack_column), GTK_WIDGET((*this_).tool_row) );
    (*this_).options_layout = gui_file_export_dialog_get_options( &((*this_).file_export_dialog) );
    gtk_box_append( GTK_BOX((*this_).main_stack_column), (*this_).options_layout );
    gtk_box_append( GTK_BOX((*this_).main_stack_column), GTK_WIDGET((*this_).two_panes) );
    gtk_box_append( GTK_BOX((*this_).main_stack_column), GTK_WIDGET((*this_).message_row) );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).tool_row ), false );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).tool_row ), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).two_panes ), true );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).two_panes ), true );
    gtk_widget_set_vexpand( GTK_WIDGET( (*this_).message_row ), false );
    gtk_widget_set_hexpand( GTK_WIDGET( (*this_).message_row ), true );

    gtk_window_set_child( GTK_WINDOW((*this_).window), (*this_).main_stack_column );
    gtk_window_set_resizable( GTK_WINDOW((*this_).window), true );

    U8_TRACE_INFO("GTK+ Widgets are added to containers.");

    /* inject dependencies by signals */
    /* parameter info: g_signal_connect( instance-that-emits-the-signal, signal-name, callback-handler, data-to-be-passed-to-callback-handler) */
    g_signal_connect( G_OBJECT((*this_).window), "close-request", G_CALLBACK(gui_main_window_delete_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), "destroy", G_CALLBACK(gui_main_window_destroy_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), "state-flags-changed", G_CALLBACK( gui_main_window_state_callback ), this_ );
    g_signal_connect( G_OBJECT((*this_).window), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_main_window_data_changed_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).sketcharea), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_data_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).tool_row), GUI_TOOLBOX_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_tool_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).file_new) ), "clicked", G_CALLBACK(gui_main_window_new_db_btn_callback), this_ );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).file_open) ), "clicked", G_CALLBACK(gui_main_window_open_db_btn_callback), this_ );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).file_save) ), "clicked", G_CALLBACK(gui_main_window_save_btn_callback), this_ );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).file_export) ), "clicked", G_CALLBACK(gui_main_window_export_btn_callback), this_ );

    g_signal_connect( gui_button_get_widget_ptr( &((*this_).view_new_window) ), "clicked", G_CALLBACK(gui_main_window_new_window_btn_callback), this_ );

    gui_toolbox_t *const tools = &((*this_).tools_data);
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).view_navigate) ), "clicked", G_CALLBACK(gui_toolbox_navigate_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).view_edit) ), "clicked", G_CALLBACK(gui_toolbox_edit_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).view_create) ), "clicked", G_CALLBACK(gui_toolbox_create_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).view_search) ), "clicked", G_CALLBACK(gui_toolbox_search_btn_callback), tools );

    /* The search id button is connected to two callback functions: one to switch to search mode, one to perform a search: */
    g_signal_connect( G_OBJECT((*this_).id_search_btn),
                      "clicked",
                      G_CALLBACK(gui_toolbox_search_id_btn_callback),
                      tools
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

    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_undo) ), "clicked", G_CALLBACK(gui_toolbox_undo_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_redo) ), "clicked", G_CALLBACK(gui_toolbox_redo_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_cut) ), "clicked", G_CALLBACK(gui_toolbox_cut_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_copy) ), "clicked", G_CALLBACK(gui_toolbox_copy_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_paste) ), "clicked", G_CALLBACK(gui_toolbox_paste_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_delete) ), "clicked", G_CALLBACK(gui_toolbox_delete_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_instantiate) ), "clicked", G_CALLBACK(gui_toolbox_instantiate_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_highlight) ), "clicked", G_CALLBACK(gui_toolbox_highlight_btn_callback), tools );
    g_signal_connect( gui_button_get_widget_ptr( &((*this_).edit_reset) ), "clicked", G_CALLBACK(gui_toolbox_reset_btn_callback), tools );

    g_signal_connect( G_OBJECT((*this_).name_entry),
                      "activate",
                      G_CALLBACK(gui_attributes_editor_name_enter_callback),
                      &((*this_).attributes_editor)
                    );
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
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), "activate", G_CALLBACK(gui_attributes_editor_stereotype_enter_callback), &((*this_).attributes_editor) );
    g_signal_connect_after( G_OBJECT((*this_).type_dropdown), "notify::selected", G_CALLBACK(gui_attributes_editor_type_changed_callback), &((*this_).attributes_editor));
    for( int_fast32_t diag_idx = 0; diag_idx < GUI_TYPE_RESOURCE_LIST_DIAGRAMS; diag_idx ++ )
    {
        g_signal_connect( G_OBJECT((*this_).type_diag_btn[ diag_idx ]),
                          "clicked",
                          G_CALLBACK( gui_attributes_editor_type_of_diagram_btn_callback ),
                          &((*this_).type_diag_data[diag_idx])
                        );
    }
    for( int_fast32_t clas_idx = 0; clas_idx < GUI_TYPE_RESOURCE_LIST_CLASSIFIERS; clas_idx ++ )
    {
        g_signal_connect( G_OBJECT((*this_).type_clas_btn[ clas_idx ]),
                          "clicked",
                          G_CALLBACK( gui_attributes_editor_type_of_classifier_btn_callback ),
                          &((*this_).type_clas_data[clas_idx])
                        );
    }
    for( int_fast32_t feat_idx = 0; feat_idx < GUI_TYPE_RESOURCE_LIST_INV_FEATURES; feat_idx ++ )
    {
        g_signal_connect( G_OBJECT((*this_).type_feat_btn[ feat_idx ]),
                          "clicked",
                          G_CALLBACK( gui_attributes_editor_type_of_feature_btn_callback ),
                          &((*this_).type_feat_data[feat_idx])
                        );
    }
    for( int_fast32_t rel_idx = 0; rel_idx < GUI_TYPE_RESOURCE_LIST_RELATIONS; rel_idx ++ )
    {
        g_signal_connect( G_OBJECT((*this_).type_rel_btn[ rel_idx ]),
                          "clicked",
                          G_CALLBACK( gui_attributes_editor_type_of_relationship_btn_callback ),
                          &((*this_).type_rel_data[rel_idx])
                        );
    }
    g_signal_connect( G_OBJECT((*this_).window), GUI_MARKED_SET_GLIB_SIGNAL_NAME, G_CALLBACK(gui_attributes_editor_focused_object_changed_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).name_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_attributes_editor_data_changed_callback), &((*this_).attributes_editor) );
        /* ^-- name_entry is the  proxy for all widgets of attributes_editor */

    g_signal_connect( gui_button_get_widget_ptr( &((*this_).help_about) ), "clicked", G_CALLBACK(gui_main_window_about_btn_callback), this_ );

    U8_TRACE_INFO("GTK+ Callbacks are connected to widget events.");

    /* register observers */

    (*this_).data_notifier = data_database_get_notifier_ptr( io_data_file_get_database_ptr( data_file ) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).window) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).sketcharea) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).name_entry) );
        /* ^-- name_entry is the  proxy for all widgets of attributes_editor */
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).search_entry) );

    U8_TRACE_INFO("GTK+ Widgets are registered as listeners at signal emitter.");

    gtk_widget_set_visible( (*this_).window, TRUE );
    gtk_widget_set_sensitive( GTK_WIDGET((*this_).window), TRUE );  /* idea taken from gtk demo */

    GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).window) );
    gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */

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
    gui_file_action_destroy( &((*this_).file_action) );

    gui_search_request_destroy( &((*this_).search_request) );
    gui_search_runner_destroy( &((*this_).search_runner) );
    gui_sketch_area_destroy( &((*this_).sketcharea_data) );
    gui_toolbox_destroy( &((*this_).tools_data) );
    gui_marked_set_destroy( &((*this_).marker_data) );
    gui_attributes_editor_destroy( &((*this_).attributes_editor) );
    gui_simple_message_to_user_destroy( &((*this_).message_to_user) );
    (*this_).data_file = NULL;
    (*this_).resources = NULL;

    gui_button_destroy( &((*this_).file_new) );
    gui_button_destroy( &((*this_).file_open) );
    gui_button_destroy( &((*this_).file_save) );
    gui_button_destroy( &((*this_).file_export) );
    gui_button_destroy( &((*this_).view_new_window) );
    gui_button_destroy( &((*this_).view_navigate) );
    gui_button_destroy( &((*this_).view_edit) );
    gui_button_destroy( &((*this_).view_create) );
    gui_button_destroy( &((*this_).view_search) );
    gui_button_destroy( &((*this_).edit_undo) );
    gui_button_destroy( &((*this_).edit_redo) );
    gui_button_destroy( &((*this_).edit_cut) );
    gui_button_destroy( &((*this_).edit_copy) );
    gui_button_destroy( &((*this_).edit_paste) );
    gui_button_destroy( &((*this_).edit_delete) );
    gui_button_destroy( &((*this_).edit_instantiate) );
    gui_button_destroy( &((*this_).edit_highlight) );
    gui_button_destroy( &((*this_).edit_reset) );
    gui_button_destroy( &((*this_).help_about) );

    U8_TRACE_END();
}

static inline void gtk_button_set_image( GtkButton *btn, GtkWidget *icon )
{
    gtk_button_set_child( btn, icon );
}

void gui_main_window_private_init_toolbox( gui_main_window_t *this_ )
{
    U8_TRACE_BEGIN();
    const gui_resources_t *const res = (*this_).resources;

    gui_button_init( &((*this_).file_new),
                     GDK_PAINTABLE( gui_resources_get_file_new( res ) ),
                     "new",
                     "New"
                    );

    gui_button_init( &((*this_).file_open),
                     GDK_PAINTABLE( gui_resources_get_file_open( res ) ),
                     "open",
                     "Open"
                    );

    /* save icon depends if latest changes are already saved */
    const bool is_saved = io_data_file_is_in_sync( (*this_).data_file );
    gui_button_init( &((*this_).file_save),
                     is_saved
                     ? GDK_PAINTABLE( gui_resources_get_file_saved( res ) )
                     : GDK_PAINTABLE( gui_resources_get_file_save( res ) ),
                     "save",
                     "Save (Ctrl-S)"
                    );
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

    gui_button_init( &((*this_).file_export),
                     GDK_PAINTABLE( gui_resources_get_file_export( res ) ),
                     "export",
                     "Export"
                    );

    (*this_).tool_sect_1_icon = gtk_image_new_from_paintable( GDK_PAINTABLE ( gui_resources_get_tool_sect( res ) ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).tool_sect_1_icon), 12 /*=w*/ , 32 /*=h*/ );
    gtk_image_set_pixel_size( GTK_IMAGE((*this_).tool_sect_1_icon), 32 );
    gtk_widget_set_halign( (*this_).tool_sect_1_icon, GTK_ALIGN_START );

    gui_button_init( &((*this_).view_new_window),
                     GDK_PAINTABLE( gui_resources_get_view_new_window( res ) ),
                     "win",
                     "New Window"
                    );

    gui_button_init_toggle( &((*this_).view_navigate),
                            GDK_PAINTABLE( gui_resources_get_view_navigate( res ) ),
                            "nav",
                            "Navigate"
                           );

    gui_button_init_toggle( &((*this_).view_edit),
                            GDK_PAINTABLE( gui_resources_get_view_edit( res ) ),
                            "edit",
                            "Edit"
                           );
    gtk_toggle_button_set_group( GTK_TOGGLE_BUTTON( gui_button_get_widget_ptr( &((*this_).view_edit) ) ),
                                 GTK_TOGGLE_BUTTON( gui_button_get_widget_ptr( &((*this_).view_navigate) ) )
                               );

    gui_button_init_toggle( &((*this_).view_create),
                            GDK_PAINTABLE( gui_resources_get_view_create( res ) ),
                            "create",
                            "Create"
                           );
    gtk_toggle_button_set_group( GTK_TOGGLE_BUTTON( gui_button_get_widget_ptr( &((*this_).view_create) ) ),
                                 GTK_TOGGLE_BUTTON( gui_button_get_widget_ptr( &((*this_).view_edit) ) )
                               );

    gui_button_init_toggle( &((*this_).view_search),
                            GDK_PAINTABLE( gui_resources_get_view_search( res ) ),
                            "search",
                            "Search"
                           );
    gtk_toggle_button_set_group( GTK_TOGGLE_BUTTON( gui_button_get_widget_ptr( &((*this_).view_search) ) ),
                                 GTK_TOGGLE_BUTTON( gui_button_get_widget_ptr( &((*this_).view_create) ) )
                               );

    (*this_).tool_sect_2_icon = gtk_image_new_from_paintable( GDK_PAINTABLE ( gui_resources_get_tool_sect( res ) ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).tool_sect_2_icon), 12 /*=w*/ , 32 /*=h*/ );
    gtk_image_set_pixel_size( GTK_IMAGE((*this_).tool_sect_2_icon), 32 );
    gtk_widget_set_halign( (*this_).tool_sect_2_icon, GTK_ALIGN_START );

    gui_button_init( &((*this_).edit_undo),
                     GDK_PAINTABLE( gui_resources_get_edit_undo( res ) ),
                     "undo",
                     "Undo (Ctrl-Z)"
                    );
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

    gui_button_init( &((*this_).edit_redo),
                     GDK_PAINTABLE( gui_resources_get_edit_redo( res ) ),
                     "redo",
                     "Redo (Ctrl-Y)"
                   );
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

    gui_button_init( &((*this_).edit_cut),
                     GDK_PAINTABLE( gui_resources_get_edit_cut( res ) ),
                     "cut",
                     "Cut (Ctrl-X)"
                   );
    /*
    GDK_KEY_x + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    gui_button_init( &((*this_).edit_copy),
                     GDK_PAINTABLE( gui_resources_get_edit_copy( res ) ),
                     "copy",
                     "Copy (Ctrl-C)"
                   );
    /*
    GDK_KEY_c + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    gui_button_init( &((*this_).edit_paste),
                     GDK_PAINTABLE( gui_resources_get_edit_paste( res ) ),
                     "paste",
                     "Paste (Ctrl-V)"
                   );
    /*
    GK_KEY_v + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    gui_button_init( &((*this_).edit_delete),
                     GDK_PAINTABLE( gui_resources_get_edit_delete( res ) ),
                     "del",
                     "Delete (DEL)"
                   );
    /*
    GDK_KEY_Delete handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    gui_button_init( &((*this_).edit_instantiate),
                     GDK_PAINTABLE( gui_resources_get_edit_instantiate( res ) ),
                     "inst",
                     "Instantiate"
                   );

    gui_button_init( &((*this_).edit_highlight),
                     GDK_PAINTABLE( gui_resources_get_edit_highlight( res ) ),
                     "mark",
                     "Mark"
                   );

    gui_button_init( &((*this_).edit_reset),
                     GDK_PAINTABLE( gui_resources_get_edit_reset( res ) ),
                     "zero",
                     "Reset Selection"
                   );

    gui_button_init( &((*this_).help_about),
                     GDK_PAINTABLE( gui_resources_get_crystal_facet_uml( res ) ),
                     "info",
                     "About"
                   );

    /* insert widgets to box container */
    {
        (*this_).tool_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 4 );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).file_new) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).file_open) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).file_save) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).file_export) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).tool_sect_1_icon) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).view_new_window) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).view_search) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).view_navigate) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).view_edit) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).view_create) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), GTK_WIDGET((*this_).tool_sect_2_icon) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_undo) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_redo) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_cut) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_copy) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_paste) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_delete) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_instantiate) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_highlight) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).edit_reset) ) );
        gtk_box_append( GTK_BOX((*this_).tool_row), gui_button_get_widget_ptr( &((*this_).help_about) ) );
    }

    U8_TRACE_END();
}

void gui_main_window_private_init_attributes_editor( gui_main_window_t *this_ )
{
    U8_TRACE_BEGIN();
    const gui_resources_t *const res = (*this_).resources;

    (*this_).attr_section_icon = gtk_image_new_from_paintable( GDK_PAINTABLE ( gui_resources_get_edit_attributes_sect( res ) ) );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).attr_section_icon), 48 /*=w*/ , 12 /*=h*/ );
    gtk_image_set_pixel_size( GTK_IMAGE((*this_).attr_section_icon), 48 );
    gtk_widget_set_halign( (*this_).attr_section_icon, GTK_ALIGN_START );

    (*this_).id_label = gtk_label_new( "" );
    gtk_label_set_selectable( GTK_LABEL( (*this_).id_label ), true );
    (*this_).name_label = gtk_label_new( "Name:" );
    (*this_).description_label = gtk_label_new( "Description:" );
    (*this_).stereotype_label = gtk_label_new( "Stereotype:" );
    (*this_).type_label = gtk_label_new( "Type:" );
    gtk_label_set_xalign( GTK_LABEL( (*this_).id_label ), 1.0 ); /* align right */
    gtk_label_set_xalign( GTK_LABEL( (*this_).name_label ), 0.0 );
    gtk_label_set_xalign( GTK_LABEL( (*this_).description_label ), 0.0 );
    gtk_label_set_xalign( GTK_LABEL( (*this_).stereotype_label ), 0.0 );
    gtk_label_set_xalign( GTK_LABEL( (*this_).type_label ), 0.0 );

    (*this_).id_search_btn = gtk_button_new();
    (*this_).id_search_btn_icon = gtk_image_new_from_paintable( GDK_PAINTABLE( gui_resources_get_search_search( res ) ) );
    gtk_button_set_image( GTK_BUTTON((*this_).id_search_btn), (*this_).id_search_btn_icon );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).id_search_btn), "Find Occurrences" );
    gtk_widget_set_size_request( GTK_WIDGET((*this_).id_search_btn_icon), 32 /*=w*/ , 32 /*=h*/ );

    /* insert widgets to box container */
    {
        (*this_).id_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 8 );
        gtk_box_append( GTK_BOX((*this_).id_row), GTK_WIDGET((*this_).attr_section_icon) );
        gtk_box_append( GTK_BOX((*this_).id_row), GTK_WIDGET((*this_).id_label) );
        gtk_box_append( GTK_BOX((*this_).id_row), GTK_WIDGET((*this_).id_search_btn) );

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
    gtk_text_view_set_bottom_margin( GTK_TEXT_VIEW( (*this_).description_text_view ), 12 );
    gtk_text_view_set_left_margin( GTK_TEXT_VIEW( (*this_).description_text_view ), 12 );
    gtk_text_view_set_right_margin( GTK_TEXT_VIEW( (*this_).description_text_view ), 12 );
    gtk_text_view_set_top_margin( GTK_TEXT_VIEW( (*this_).description_text_view ), 12 );
    (*this_).description_scroll_win = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child( GTK_SCROLLED_WINDOW((*this_).description_scroll_win), (*this_).description_text_view );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( (*this_).description_scroll_win ),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC
                                  );

    GListStore *model = NULL;  /* the model is set later */
    GtkExpression *expression = NULL;  /* not needed: there is no search entry that could find list elements */
    (*this_).type_dropdown = GTK_DROP_DOWN( gtk_drop_down_new ( G_LIST_MODEL( model ), expression ) );
    GtkBuilderScope *scope = gtk_builder_cscope_new();
    static const char bytes[] =
    "<interface>\n"
    "<template class=\"GtkListItem\">\n"
    "  <property name=\"child\">\n"
    "    <object class=\"GtkPaned\">\n"
    "      <property name=\"start-child\">\n"
    "        <object class=\"GtkImage\">\n"
    "          <property name=\"width-request\">32</property>\n"
    "          <property name=\"height-request\">24</property>\n"
    "          <binding name=\"paintable\">\n"
    "            <lookup name=\"icon\" type=\"GuiTypeResource\">\n"
    "              <lookup name=\"item\">GtkListItem</lookup>\n"
    "            </lookup>\n"
    "          </binding>\n"
    "        </object>\n"
    "      </property>\n"
    "      <property name=\"end-child\">\n"
    "        <object class=\"GtkLabel\">\n"
    "          <property name=\"xalign\">0</property>\n"
    "          <binding name=\"label\">\n"
    "            <lookup name=\"name\" type=\"GuiTypeResource\">\n"
    "              <lookup name=\"item\">GtkListItem</lookup>\n"
    "            </lookup>\n"
    "          </binding>\n"
    "        </object>\n"
    "      </property>\n"
    "      <property name=\"position\">48</property>\n"
    "    </object>\n"
    "  </property>\n"
    "</template>\n"
    "</interface>\n";
    GBytes *byteptr = g_bytes_new( &bytes, sizeof(bytes)-sizeof(char) );
    GtkListItemFactory *factory = gtk_builder_list_item_factory_new_from_bytes( scope, byteptr );
    gtk_drop_down_set_factory( (*this_).type_dropdown, factory );
    g_object_unref( scope );
    g_object_unref( factory );
    g_bytes_unref( byteptr );

    static gui_type_resource_list_t res_select;
    gui_type_resource_list_init( &res_select, res );
    {
        (*this_).type_diag_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_diag_grid ), GTK_ALIGN_END );
        const gui_type_resource_t (*diag_data)[];
        unsigned int diag_data_length;
        gui_type_resource_list_get_all_diagram_types( &res_select, &diag_data, &diag_data_length );
        assert( diag_data_length == GUI_TYPE_RESOURCE_LIST_DIAGRAMS );
        for( int_fast32_t diag_idx = 0; diag_idx < GUI_TYPE_RESOURCE_LIST_DIAGRAMS; diag_idx ++ )
        {
            const gui_type_resource_t *const type_data = &((*diag_data)[diag_idx]);
            const data_diagram_type_t diag_type = data_type_get_diagram_type( gui_type_resource_get_type_id( type_data ) );
            GdkTexture *const diag_icon = gui_type_resource_get_icon( type_data );
            const char *const diag_name = gui_type_resource_get_name( type_data );

            gui_attribute_type_of_diagram_init( &((*this_).type_diag_data[diag_idx]), diag_type, &((*this_).attributes_editor) );
            (*this_).type_diag_img[ diag_idx ] = GTK_IMAGE( gtk_image_new_from_paintable( GDK_PAINTABLE( diag_icon ) ) );
            gtk_widget_set_size_request( GTK_WIDGET( (*this_).type_diag_img[ diag_idx ] ), 32 /*=w*/ , 24 /*=h*/ );
            (*this_).type_diag_btn[ diag_idx ] = GTK_BUTTON( gtk_button_new() );
            gtk_button_set_image( (*this_).type_diag_btn[ diag_idx ], GTK_WIDGET( (*this_).type_diag_img[ diag_idx ] ) );
            gtk_widget_set_tooltip_text( GTK_WIDGET( (*this_).type_diag_btn[ diag_idx ] ), diag_name );
            gtk_grid_attach( (*this_).type_diag_grid, GTK_WIDGET( (*this_).type_diag_btn[ diag_idx ] ), diag_idx%7, diag_idx/7, 1, 1 );
        }

        (*this_).type_clas_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_clas_grid ), GTK_ALIGN_END );
        const gui_type_resource_t (*clas_data)[];
        unsigned int clas_data_length;
        gui_type_resource_list_get_all_classifier_types( &res_select, &clas_data, &clas_data_length );
        assert( clas_data_length == GUI_TYPE_RESOURCE_LIST_CLASSIFIERS );
        for( int_fast32_t clas_idx = 0; clas_idx < GUI_TYPE_RESOURCE_LIST_CLASSIFIERS; clas_idx ++ )
        {
            const gui_type_resource_t *const type_data = &((*clas_data)[clas_idx]);
            const data_classifier_type_t clas_type = data_type_get_classifier_type( gui_type_resource_get_type_id( type_data ) );
            GdkTexture *const clas_icon = gui_type_resource_get_icon( type_data );
            const char *const clas_name = gui_type_resource_get_name( type_data );

            gui_attribute_type_of_classifier_init( &((*this_).type_clas_data[clas_idx]), clas_type, &((*this_).attributes_editor) );
            (*this_).type_clas_img[ clas_idx ] = GTK_IMAGE( gtk_image_new_from_paintable( GDK_PAINTABLE( clas_icon ) ) );
            gtk_widget_set_size_request( GTK_WIDGET( (*this_).type_clas_img[ clas_idx ] ), 32 /*=w*/ , 24 /*=h*/ );
            (*this_).type_clas_btn[ clas_idx ] = GTK_BUTTON( gtk_button_new() );
            gtk_button_set_image( (*this_).type_clas_btn[ clas_idx ], GTK_WIDGET( (*this_).type_clas_img[ clas_idx ] ) );
            gtk_widget_set_tooltip_text( GTK_WIDGET( (*this_).type_clas_btn[ clas_idx ] ), clas_name );
            gtk_grid_attach( (*this_).type_clas_grid, GTK_WIDGET( (*this_).type_clas_btn[ clas_idx ] ), clas_idx%7, clas_idx/7, 1, 1 );
        }

        (*this_).type_feat_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_feat_grid ), GTK_ALIGN_END );
        const gui_type_resource_t (*feat_data)[];
        unsigned int feat_data_length;
        gui_type_resource_list_get_invariant_feature_types( &res_select, &feat_data, &feat_data_length );
        assert( feat_data_length == GUI_TYPE_RESOURCE_LIST_INV_FEATURES );
        for( int_fast32_t feat_idx = 0; feat_idx < GUI_TYPE_RESOURCE_LIST_INV_FEATURES; feat_idx ++ )  /* ignore the lifeline */
        {
            const gui_type_resource_t *const type_data = &((*feat_data)[feat_idx]);
            const data_feature_type_t feat_type = data_type_get_feature_type( gui_type_resource_get_type_id( type_data ) );
            GdkTexture *const feat_icon = gui_type_resource_get_icon( type_data );
            const char *const feat_name = gui_type_resource_get_name( type_data );

            gui_attribute_type_of_feature_init( &((*this_).type_feat_data[feat_idx]), feat_type, &((*this_).attributes_editor) );
            (*this_).type_feat_img[ feat_idx ] = GTK_IMAGE( gtk_image_new_from_paintable( GDK_PAINTABLE( feat_icon ) ) );
            gtk_widget_set_size_request( GTK_WIDGET( (*this_).type_feat_img[ feat_idx ] ), 32 /*=w*/ , 24 /*=h*/ );
            (*this_).type_feat_btn[ feat_idx ] = GTK_BUTTON( gtk_button_new() );
            gtk_button_set_image( (*this_).type_feat_btn[ feat_idx ], GTK_WIDGET( (*this_).type_feat_img[ feat_idx ] ) );
            gtk_widget_set_tooltip_text( GTK_WIDGET( (*this_).type_feat_btn[ feat_idx ] ), feat_name );
            gtk_grid_attach( (*this_).type_feat_grid, GTK_WIDGET( (*this_).type_feat_btn[ feat_idx ] ), feat_idx%7, feat_idx/7, 1, 1 );
        }

        (*this_).type_rel_grid = GTK_GRID( gtk_grid_new() );
        gtk_widget_set_halign( GTK_WIDGET( (*this_).type_rel_grid ), GTK_ALIGN_END );
        const gui_type_resource_t (*rel_data)[];
        unsigned int rel_data_length;
        gui_type_resource_list_get_all_relationship_types( &res_select, &rel_data, &rel_data_length );
        assert( rel_data_length == GUI_TYPE_RESOURCE_LIST_RELATIONS );
        for( int_fast32_t rel_idx = 0; rel_idx < GUI_TYPE_RESOURCE_LIST_RELATIONS; rel_idx ++ )
        {
            const gui_type_resource_t *const type_data = &((*rel_data)[rel_idx]);
            const data_relationship_type_t rel_type = data_type_get_relationship_type( gui_type_resource_get_type_id( type_data ) );
            GdkTexture *const rel_icon = gui_type_resource_get_icon( type_data );
            const char *const rel_name = gui_type_resource_get_name( type_data );

            gui_attribute_type_of_relationship_init( &((*this_).type_rel_data[rel_idx]), rel_type, &((*this_).attributes_editor) );
            (*this_).type_rel_img[ rel_idx ] = GTK_IMAGE( gtk_image_new_from_paintable( GDK_PAINTABLE( rel_icon ) ) );
            gtk_widget_set_size_request( GTK_WIDGET( (*this_).type_rel_img[ rel_idx ] ), 32 /*=w*/ , 24 /*=h*/ );
            (*this_).type_rel_btn[ rel_idx ] = GTK_BUTTON( gtk_button_new() );
            gtk_button_set_image( (*this_).type_rel_btn[ rel_idx ], GTK_WIDGET( (*this_).type_rel_img[ rel_idx ] ) );
            gtk_widget_set_tooltip_text( GTK_WIDGET( (*this_).type_rel_btn[ rel_idx ] ), rel_name );
            gtk_grid_attach( (*this_).type_rel_grid, GTK_WIDGET( (*this_).type_rel_btn[ rel_idx ] ), rel_idx%7, rel_idx/7, 1, 1 );
        }
    }
    gui_type_resource_list_destroy( &res_select );

    /* insert widgets to box container */
    {
        (*this_).attr_edit_column = gtk_box_new( GTK_ORIENTATION_VERTICAL, /*spacing:*/ 4 );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).id_row) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).name_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).name_entry) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).stereotype_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).stereotype_entry) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_dropdown) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_diag_grid) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_clas_grid) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_feat_grid) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).type_rel_grid) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).description_label) );
        gtk_box_append( GTK_BOX((*this_).attr_edit_column), GTK_WIDGET((*this_).description_scroll_win) );

        gtk_widget_set_vexpand( GTK_WIDGET( (*this_).description_scroll_win ), true );
        gtk_widget_set_hexpand( GTK_WIDGET( (*this_).description_scroll_win ), true );
    }

    U8_TRACE_END();
}

void gui_main_window_private_init_simple_message_to_user( gui_main_window_t *this_ )
{
    U8_TRACE_BEGIN();
    const gui_resources_t *const res = (*this_).resources;

    (*this_).message_text_label = gtk_label_new( "" );
    gtk_label_set_xalign(GTK_LABEL( (*this_).message_text_label ), 0.0 );

    (*this_).message_icon_image = gtk_image_new_from_paintable( GDK_PAINTABLE ( gui_resources_get_crystal_facet_uml( res ) ) );    gtk_widget_set_size_request( GTK_WIDGET((*this_).message_icon_image), 32 /*=w*/ , 32 /*=h*/ );

    /* insert widgets to box container */
    {
        (*this_).message_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 20 );
        gtk_box_append( GTK_BOX((*this_).message_row), GTK_WIDGET((*this_).message_icon_image) );
        gtk_box_append( GTK_BOX((*this_).message_row), GTK_WIDGET((*this_).message_text_label) );

        gtk_widget_set_vexpand( GTK_WIDGET( (*this_).message_icon_image ), false );
        gtk_widget_set_hexpand( GTK_WIDGET( (*this_).message_icon_image ), false );
        gtk_widget_set_vexpand( GTK_WIDGET( (*this_).message_text_label ), false );
        gtk_widget_set_hexpand( GTK_WIDGET( (*this_).message_text_label ), true );
    }

    U8_TRACE_END();
}

void gui_main_window_private_init_search_and_sketch_area( gui_main_window_t *this_ )
{
    U8_TRACE_BEGIN();
    const gui_resources_t *const res = (*this_).resources;

    /* init search widgets */
    {
        (*this_).search_label = gtk_label_new ( "Search:" );
        gtk_label_set_xalign( GTK_LABEL( (*this_).search_label ), 1.0 ); /* align right */
        gtk_label_set_yalign( GTK_LABEL( (*this_).search_label ), 0.5 ); /* align middle-height */

        (*this_).search_entry = gtk_entry_new();

        (*this_).search_button = gtk_button_new();
        (*this_).search_btn_icon = gtk_image_new_from_paintable( GDK_PAINTABLE( gui_resources_get_search_search( res ) ) );
        gtk_button_set_image( GTK_BUTTON((*this_).search_button), (*this_).search_btn_icon );
        gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).search_button), "Search" );
        gtk_widget_set_size_request( GTK_WIDGET((*this_).search_btn_icon), 32 /*=w*/ , 32 /*=h*/ );
    }

    /* insert widgets to box container */
    {
        (*this_).search_row = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, /*spacing:*/ 4 );
        gtk_box_append( GTK_BOX((*this_).search_row), GTK_WIDGET((*this_).search_label) );
        gtk_box_append( GTK_BOX((*this_).search_row), GTK_WIDGET((*this_).search_entry) );
        gtk_box_append( GTK_BOX((*this_).search_row), GTK_WIDGET((*this_).search_button) );

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
        gtk_widget_set_can_focus( GTK_WIDGET( (*this_).sketcharea ), TRUE );  /* this allows the text entry widgets to lose the focus */
        gtk_widget_set_size_request( GTK_WIDGET( (*this_).sketcharea ), 600, 360);  /* set a minimum initial size */
        /*gtk_widget_set_focus_on_click( GTK_WIDGET( (*this_).sketcharea ), TRUE ); not yet existing: since GTK 3.2 */
    }

    /* insert widgets to box container */
    {
        (*this_).sketch_stack_column = gtk_box_new( GTK_ORIENTATION_VERTICAL, /*spacing:*/ 0 );
        gtk_box_append( GTK_BOX((*this_).sketch_stack_column), GTK_WIDGET((*this_).search_row) );
        gtk_box_append( GTK_BOX((*this_).sketch_stack_column), GTK_WIDGET((*this_).sketcharea) );

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

gboolean gui_main_window_delete_event_callback( GtkWindow *widget, gpointer data )
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

    const u8_error_t err = gui_file_action_save( &((*this_).file_action) );
    if ( err == U8_ERROR_NONE )
    {
        GtkImage *const save_image = gui_button_get_icon_ptr( &((*this_).file_save) );
        const GdkPaintable *const shown_icon = gtk_image_get_paintable( save_image );
        GdkPaintable *const saved_icon = GDK_PAINTABLE( gui_resources_get_file_saved( (*this_).resources ) );
        if ( shown_icon != saved_icon )
        {
            gtk_image_set_from_paintable( save_image, saved_icon );
            U8_TRACE_INFO("icon of save button set to SAVED(/)");
        }
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

gboolean gui_main_window_save_shortcut_callback( GtkWidget* widget, GVariant* args, gpointer user_data )
{
    gui_main_window_save_btn_callback( (GtkButton*)widget, user_data );
    return TRUE;
}

void gui_main_window_export_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    const bool was_visible = gtk_widget_get_visible( (*this_).options_layout );
    gtk_widget_set_visible( (*this_).options_layout, ! was_visible );

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

    /* update the window title */
    if (( DATA_CHANGE_EVENT_TYPE_DB_OPENED == data_change_message_get_event( msg ) )
        || ( DATA_CHANGE_EVENT_TYPE_DB_CLOSED == data_change_message_get_event( msg ) ))
    {
        /* the database has changed */
        const char *filename = io_data_file_get_filename_const( (*this_).data_file );
        if ( NULL == filename )
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), META_INFO_PROGRAM_NAME_STR );
        }
        else
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), filename );
        }
    }

    /* update the save button status indicator */
    GtkImage *const save_image = gui_button_get_icon_ptr( &((*this_).file_save) );
    const GdkPaintable *const shown_icon = gtk_image_get_paintable( save_image );

    if (( DATA_CHANGE_EVENT_TYPE_DB_OPENED == data_change_message_get_event( msg ) )
        || ( DATA_CHANGE_EVENT_TYPE_DB_CLOSED == data_change_message_get_event( msg ) ))
    {
        GdkPaintable *const saved_icon = GDK_PAINTABLE( gui_resources_get_file_saved( (*this_).resources ) );
        if ( shown_icon != saved_icon )
        {
            gtk_image_set_from_paintable( save_image, saved_icon );
            U8_TRACE_INFO("icon of save button set to SAVED(/)");
        }
    }
    else if ( DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE != data_change_message_get_event( msg ) )
    {
        GdkPaintable *const save_icon = GDK_PAINTABLE( gui_resources_get_file_save( (*this_).resources ) );
        if ( shown_icon != save_icon )
        {
            gtk_image_set_from_paintable( save_image, save_icon );
            U8_TRACE_INFO("icon of save button set to NOT-SAVED(*)");
        }
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_main_window_state_callback( GtkWidget* window, GtkStateFlags old_flags, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_main_window_t *this_ = user_data;

    /* GTK_STATE_FLAG_ACTIVE is close to what is needed */
    /* but it is sometimes not called when window loses focus */
    /* and it is sometimes activated multiple times when window gains focus */

    GtkStateFlags new_flags = gtk_widget_get_state_flags ( window );
    GtkStateFlags changed = old_flags ^ new_flags;

    if ( 0 != ( changed & GTK_STATE_FLAG_BACKDROP ) )
    {
        U8_TRACE_INFO_INT("GTK_STATE_FLAGS & GTK_STATE_FLAG_BACKDROP:", changed & GTK_STATE_FLAG_BACKDROP );

        /* check if latest changes are already saved */
        const bool is_saved = io_data_file_is_in_sync( (*this_).data_file );

        /* update the icon of the save button */
        GtkImage *const save_image = gui_button_get_icon_ptr( &((*this_).file_save) );
        const GdkPaintable *const shown_icon = gtk_image_get_paintable( save_image );

        GdkPaintable *const current_icon
            = is_saved
            ? GDK_PAINTABLE( gui_resources_get_file_saved( (*this_).resources ) )
            : ( 0 != ( new_flags & GTK_STATE_FLAG_BACKDROP ) )
            ? GDK_PAINTABLE( gui_resources_get_file_unsaved( (*this_).resources ) )
            : GDK_PAINTABLE( gui_resources_get_file_save( (*this_).resources ) );

        /* update the save button status indicator */
        if ( shown_icon != current_icon )
        {
            gtk_image_set_from_paintable( save_image, current_icon );
            U8_TRACE_INFO("icon of save button updated");
        }
        // U8_TRACE_FLUSH();
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}


/*
Copyright 2016-2025 Andreas Warnke

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
