/* File: gui_main_window.c; Copyright and License: see below */

#include "gui_main_window.h"
#include "gui_textedit.h"
#include "gui_sketch_area.h"
#include "trace.h"
#include "storage/data_change_notifier.h"
#include "meta/meta_info.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_main_window_init ( gui_main_window_t *this_,
                            ctrl_controller_t *controller,
                            data_database_t *database,
                            data_database_reader_t *db_reader,
                            gui_resources_t *res,
                            observer_t *window_close_observer,
                            observer_t *window_open_observer )
{
    TRACE_BEGIN();

    /* init own attributes */
    (*this_).window_close_observer = window_close_observer;
    (*this_).window_open_observer = window_open_observer;
    (*this_).database = database;

    /* init window */

    (*this_).window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    const char *window_title;
    window_title = data_database_get_filename_ptr( database );
    gtk_window_set_title(GTK_WINDOW((*this_).window), ( window_title == NULL ) ? META_INFO_PROGRAM_NAME_STR : window_title );
    gtk_widget_set_size_request((*this_).window, 810, 600);

    (*this_).layout = gtk_grid_new();

    /* init the message widgets */

    (*this_).message_text_label = gtk_label_new ("");
    /*
     *    gtk_widget_set_valign (GTK_WIDGET( (*this_).message_text_label ), GTK_ALIGN_START );
     *    gtk_label_set_xalign (GTK_LABEL( (*this_).message_text_label ), 0.0 );
     */
    /*
     * the "set alignment" below is deprecated - but the two commented ones do not work ...
     */
    gtk_misc_set_alignment (GTK_MISC( (*this_).message_text_label ), 0.0, 0.0 );
    (*this_).message_icon_image = gtk_image_new_from_pixbuf ( gui_resources_get_crystal_facet_uml( res ) );
    gui_simple_message_to_user_init( &((*this_).message_to_user), (*this_).message_text_label, (*this_).message_icon_image, res );

    /* init tools */

    (*this_).file_use_db = gtk_tool_button_new( NULL, "Use_DB" );
    (*this_).file_use_db_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_use_db( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).file_use_db), (*this_).file_use_db_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_use_db), "Create/Use DB" );

    (*this_).file_export = gtk_tool_button_new( NULL, "Export" );
    (*this_).file_export_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_export( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).file_export), (*this_).file_export_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_export), "Export" );

    (*this_).file_new_window = gtk_tool_button_new( NULL, "New Window" );
    (*this_).file_new_window_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_new_window( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).file_new_window), (*this_).file_new_window_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_new_window), "New Window" );

    (*this_).tool_navigate = gtk_radio_tool_button_new( NULL );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_navigate), "Navigate");
    (*this_).tool_navigate_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_navigate( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_navigate), (*this_).tool_navigate_icon);
    GSList *group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_navigate));
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_navigate), "Navigate" );

    (*this_).tool_edit = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_edit), "Edit");
    (*this_).tool_edit_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_edit( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_edit), (*this_).tool_edit_icon);
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_edit));
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_edit), "Edit" );

    (*this_).tool_new_obj = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_new_obj), "New Object");
    (*this_).tool_new_obj_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_create_object( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_new_obj), (*this_).tool_new_obj_icon);
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_new_obj));
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_new_obj), "New Object" );

    (*this_).tool_new_view = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_new_view), "New Diagram");
    (*this_).tool_new_view_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_create_diagram( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_new_view), (*this_).tool_new_view_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_new_view), "New Diagram" );

    (*this_).edit_cut = gtk_tool_button_new( NULL, "Cut" );
    (*this_).edit_cut_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_cut( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_cut), (*this_).edit_cut_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_cut), "Cut" );

    (*this_).edit_copy = gtk_tool_button_new( NULL, "Copy" );
    (*this_).edit_copy_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_copy( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_copy), (*this_).edit_copy_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_copy), "Copy" );

    (*this_).edit_paste = gtk_tool_button_new( NULL, "Paste" );
    (*this_).edit_paste_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_paste( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_paste), (*this_).edit_paste_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_paste), "Paste" );

    (*this_).edit_delete = gtk_tool_button_new( NULL, "Delete" );
    (*this_).edit_delete_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_delete( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_delete), (*this_).edit_delete_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_delete), "Delete" );

    (*this_).edit_instantiate = gtk_tool_button_new( NULL, "Instantiate" );
    (*this_).edit_instantiate_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_instantiate( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_instantiate), (*this_).edit_instantiate_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_instantiate), "Instantiate" );

    (*this_).edit_highlight = gtk_tool_button_new( NULL, "Highlight" );
    (*this_).edit_highlight_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_highlight( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_highlight), (*this_).edit_highlight_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_highlight), "Highlight" );

    (*this_).edit_undo = gtk_tool_button_new( NULL, "Undo" );
    (*this_).edit_undo_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_undo( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_undo), (*this_).edit_undo_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_undo), "Undo" );

    (*this_).edit_redo = gtk_tool_button_new( NULL, "Redo" );
    (*this_).edit_redo_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_redo( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).edit_redo), (*this_).edit_redo_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_redo), "Redo" );

    (*this_).tool_about = gtk_tool_button_new( NULL, "About" );
    (*this_).tool_about_icon = gtk_image_new_from_pixbuf( gui_resources_get_crystal_facet_uml( res ));
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_about), (*this_).tool_about_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_about), "About" );

    (*this_).toolbar = gtk_toolbar_new ();

    /* determine the current/main clipboard */
    GtkClipboard *current_clipboard;
    {
        GdkScreen *current_screen;
        current_screen = gtk_window_get_screen ( GTK_WINDOW((*this_).window) );
        GdkDisplay *current_display;
        current_display = gdk_screen_get_display ( current_screen );
        current_clipboard = gtk_clipboard_get_for_display ( current_display, GDK_NONE ); /* GDK_SELECTION_PRIMARY does not work */
    }

    gui_sketch_marker_init( &((*this_).sketchmarker_data) );
    gui_sketch_tools_init( &((*this_).sketchtools_data),
                           (*this_).tool_navigate,
                           (*this_).tool_edit,
                           (*this_).tool_new_obj,
                           (*this_).tool_new_view,
                           current_clipboard,
                           &((*this_).sketchmarker_data),
                           &((*this_).message_to_user),
                           db_reader,
                           controller
    );

    /* init sketch area */

    (*this_).sketcharea = gtk_drawing_area_new();
    gtk_widget_set_events( (*this_).sketcharea, GDK_LEAVE_NOTIFY_MASK
        | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
    gtk_widget_set_hexpand ( (*this_).sketcharea, TRUE);
    gtk_widget_set_vexpand ( (*this_).sketcharea, TRUE);
    gtk_widget_set_can_focus( GTK_WIDGET( (*this_).sketcharea ), TRUE );  /* this allows the text entry widgets to lose the focus */
    /*gtk_widget_set_focus_on_click( GTK_WIDGET( (*this_).sketcharea ), TRUE ); not yet existing: since GTK 3.2 */
    gui_sketch_area_init( &((*this_).sketcharea_data),
                          &((*this_).sketchmarker_data),
                          &((*this_).sketchtools_data),
                          &((*this_).message_to_user),
                          res,
                          controller,
                          db_reader
                        );

    /* init text edit widgets */

    gui_textedit_init( &((*this_).text_editor), controller, db_reader, &((*this_).message_to_user) );
    GtkTreeModel *combo_types = gui_textedit_get_diagram_types_ptr( &((*this_).text_editor) );
    (*this_).type_combo_box = gtk_combo_box_new_with_model( combo_types );
    GtkCellRenderer *column;
    column = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT((*this_).type_combo_box), column, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT((*this_).type_combo_box), column, "text", 1, NULL);
    (*this_).name_entry = gtk_entry_new();
    (*this_).description_text_view = gtk_text_view_new ();
    (*this_).stereotype_entry = gtk_entry_new();
    (*this_).edit_commit_button = gtk_button_new();
    (*this_).edit_commit_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_commit( res ));
    gtk_button_set_image ( GTK_BUTTON((*this_).edit_commit_button), (*this_).edit_commit_icon );
    gtk_button_set_label ( GTK_BUTTON((*this_).edit_commit_button), NULL );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_commit_button), "Commit" );

    /* init the file chooser */

    (*this_).use_db_file_chooser = gtk_file_chooser_dialog_new ( "Select DB to use",
                                                                 GTK_WINDOW( (*this_).window ),
                                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Create/Use DB-File",
                                                                 GTK_RESPONSE_ACCEPT,
                                                                 NULL
    );
    gtk_file_chooser_set_current_name( GTK_FILE_CHOOSER( (*this_).use_db_file_chooser ), "untitled.cfu1_sqlite3" );
    gui_file_manager_init( &((*this_).file_manager), controller, database, &((*this_).message_to_user) );

    (*this_).export_file_chooser = gtk_file_chooser_dialog_new ( "Select Export Folder",
                                                                 GTK_WINDOW( (*this_).window ),
                                                                 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Export txt Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_TXT,
                                                                 "Export svg Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_SVG,
                                                                 "Export png Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_PNG,
                                                                 NULL
    );
    gui_file_exporter_init( &((*this_).file_exporter), db_reader, &((*this_).message_to_user) );

    TRACE_INFO("GTK+ Widgets are created.");

    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).file_use_db,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).file_export,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).file_new_window,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_navigate,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_edit,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_new_obj,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_new_view,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_cut,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_copy,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_paste,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_delete,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_instantiate,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_highlight,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_undo,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_redo,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_about,-1);
    /* gtk_grid_attach (GtkGrid *grid, GtkWidget *child, gint left, gint top, gint width, gint height); */
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).toolbar, 0, 0, 4, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).sketcharea, 0, 1, 4, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).stereotype_entry, 0, 2, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).name_entry, 1, 2, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).type_combo_box, 2, 2, 2, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).description_text_view, 0, 3, 3, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).edit_commit_button, 3, 3, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).message_icon_image, 0, 4, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).message_text_label, 1, 4, 3, 1 );
    gtk_container_add(GTK_CONTAINER((*this_).window), (*this_).layout);

    TRACE_INFO("GTK+ Widgets are added to containers.");

    /* inject dependencies by signals */
    g_signal_connect( G_OBJECT((*this_).window), "delete_event", G_CALLBACK(gui_main_window_delete_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), "destroy", G_CALLBACK(gui_main_window_destroy_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_main_window_data_changed_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "draw", G_CALLBACK (gui_sketch_area_draw_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "motion_notify_event", G_CALLBACK(gui_sketch_area_mouse_motion_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "button_press_event", G_CALLBACK(gui_sketch_area_button_press_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "button_release_event", G_CALLBACK(gui_sketch_area_button_release_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "leave_notify_event", G_CALLBACK(gui_sketch_area_leave_notify_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_data_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), GUI_SKETCH_TOOLS_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_tool_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).file_use_db), "clicked", G_CALLBACK(gui_main_window_use_db_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_export), "clicked", G_CALLBACK(gui_main_window_export_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_new_window), "clicked", G_CALLBACK(gui_main_window_new_window_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).tool_navigate), "clicked", G_CALLBACK(gui_sketch_tools_navigate_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_edit), "clicked", G_CALLBACK(gui_sketch_tools_edit_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_new_obj), "clicked", G_CALLBACK(gui_sketch_tools_create_object_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_new_view), "clicked", G_CALLBACK(gui_sketch_tools_create_diagram_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_cut), "clicked", G_CALLBACK(gui_sketch_tools_cut_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_copy), "clicked", G_CALLBACK(gui_sketch_tools_copy_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_paste), "clicked", G_CALLBACK(gui_sketch_tools_paste_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_delete), "clicked", G_CALLBACK(gui_sketch_tools_delete_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_instantiate), "clicked", G_CALLBACK(gui_sketch_tools_instantiate_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_highlight), "clicked", G_CALLBACK(gui_sketch_tools_highlight_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_undo), "clicked", G_CALLBACK(gui_sketch_tools_undo_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_redo), "clicked", G_CALLBACK(gui_sketch_tools_redo_btn_callback), &((*this_).sketchtools_data) );
    g_signal_connect( G_OBJECT((*this_).name_entry), "focus-out-event", G_CALLBACK(gui_textedit_name_focus_lost_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).description_text_view), "focus-out-event", G_CALLBACK(gui_textedit_description_focus_lost_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).type_combo_box), "changed", G_CALLBACK(gui_textedit_type_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), "focus-out-event", G_CALLBACK(gui_textedit_stereotype_focus_lost_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).edit_commit_button), "clicked", G_CALLBACK(gui_textedit_commit_clicked_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).name_entry), GUI_SKETCH_AREA_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_name_selected_object_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).description_text_view), GUI_SKETCH_AREA_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_description_selected_object_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).type_combo_box), GUI_SKETCH_AREA_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_type_selected_object_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), GUI_SKETCH_AREA_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_stereotype_selected_object_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).name_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_name_data_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).description_text_view), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_description_data_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).type_combo_box), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_type_data_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_textedit_stereotype_data_changed_callback), &((*this_).text_editor) );
    g_signal_connect( G_OBJECT((*this_).tool_about), "clicked", G_CALLBACK(gui_main_window_about_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "response", G_CALLBACK(gui_file_manager_use_db_response_callback), &((*this_).file_manager) );
    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "response", G_CALLBACK(gui_file_exporter_export_response_callback), &((*this_).file_exporter) );
    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );
    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );

    TRACE_INFO("GTK+ Callbacks are connected to widget events.");

    /* register observers */
    (*this_).data_notifier = data_database_get_notifier_ptr( database );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).window) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).sketcharea) );
    gui_sketch_tools_set_listener( &((*this_).sketchtools_data), G_OBJECT((*this_).sketcharea) );
    gui_sketch_area_set_listener( &((*this_).sketcharea_data), 0, G_OBJECT((*this_).name_entry) );
    gui_sketch_area_set_listener( &((*this_).sketcharea_data), 1, G_OBJECT((*this_).description_text_view) );
    gui_sketch_area_set_listener( &((*this_).sketcharea_data), 2, G_OBJECT((*this_).stereotype_entry) );
    gui_sketch_area_set_listener( &((*this_).sketcharea_data), 3, G_OBJECT((*this_).type_combo_box) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).name_entry) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).description_text_view) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).stereotype_entry) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).type_combo_box) );

    TRACE_INFO("GTK+ Widgets are registered as listeners at signal emitter.");

    gtk_widget_show_all((*this_).window);
    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    TRACE_INFO("GTK+ Widgets are shown.");
    TRACE_END();
}

void gui_main_window_destroy( gui_main_window_t *this_ )
{
    TRACE_BEGIN();

    gui_sketch_area_remove_listener( &((*this_).sketcharea_data), 0 );
    gui_sketch_area_remove_listener( &((*this_).sketcharea_data), 1 );
    gui_sketch_area_remove_listener( &((*this_).sketcharea_data), 2 );
    gui_sketch_area_remove_listener( &((*this_).sketcharea_data), 3 );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).window) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).sketcharea) );
    gui_sketch_tools_remove_listener( &((*this_).sketchtools_data) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).name_entry) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).description_text_view) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).stereotype_entry) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).type_combo_box) );

    TRACE_INFO("GTK+ Widgets are unregistered as listeners from data module.");

    gtk_widget_destroy( (*this_).use_db_file_chooser );
    gtk_widget_destroy( (*this_).export_file_chooser );

    TRACE_INFO("GTK+ hidden windows are destroyed.");

    gui_sketch_area_destroy( &((*this_).sketcharea_data) );
    gui_sketch_tools_destroy( &((*this_).sketchtools_data) );
    gui_sketch_marker_destroy( &((*this_).sketchmarker_data) );
    gui_textedit_destroy( &((*this_).text_editor) );
    gui_file_manager_destroy( &((*this_).file_manager) );
    gui_file_exporter_destroy( &((*this_).file_exporter) );
    gui_simple_message_to_user_destroy( &((*this_).message_to_user) );
    (*this_).database = NULL;

    TRACE_END();
}

void gui_main_window_destroy_event_callback( GtkWidget *widget, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    /* forward destroy request to gui_window_manager: */
    observer_notify( (*this_).window_close_observer, this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

gboolean gui_main_window_delete_event_callback( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    TRACE_BEGIN();

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* return false to trigger destroy event */
}

void gui_main_window_use_db_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gtk_widget_show_all( GTK_WIDGET( (*this_).use_db_file_chooser ) );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_export_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gtk_widget_show_all( GTK_WIDGET( (*this_).export_file_chooser ) );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_new_window_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    /* hide last message */
    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    /* forward new window request to gui_window_manager: */
    observer_notify( (*this_).window_open_observer, &((*this_).message_to_user) );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_about_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_show_message_with_string( &((*this_).message_to_user), GUI_SIMPLE_MESSAGE_TYPE_ABOUT, GUI_SIMPLE_MESSAGE_CONTENT_ABOUT, NULL );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_data_changed_callback( GtkWidget *window, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = user_data;

    if ( DATA_TABLE_VOID == data_id_get_table( id ) )
    {
        /* the database has changed */
        const char *filename = data_database_get_filename_ptr( (*this_).database );
        if ( NULL == filename )
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), META_INFO_PROGRAM_NAME_STR );
        }
        else
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), filename );
        }
    }

    TRACE_TIMESTAMP();
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
