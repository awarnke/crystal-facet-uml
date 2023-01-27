/* File: gui_attributes_editor.c; Copyright and License: see below */

#include "gui_attributes_editor.h"
#include "gtk_helper/gtk_helper_tree_model.h"
#include "u8/u8_trace.h"
#include "data_table.h"
#include "data_id.h"
#include "utf8stringbuf/utf8string.h"
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_attributes_editor_init ( gui_attributes_editor_t *this_,
                                  GtkLabel *id_label,
                                  GtkEntry *name_entry,
                                  GtkEntry *stereotype_entry,
                                  GtkComboBox *type_combo_box,
                                  GtkIconView *type_icon_grid,
                                  GtkTextView *description_text_view,
                                  GtkButton *commit_button,
                                  gui_resources_t *resources,
                                  ctrl_controller_t *controller,
                                  data_database_reader_t *db_reader,
                                  data_database_t *database,
                                  gui_simple_message_to_user_t *message_to_user )
{
    U8_TRACE_BEGIN();
    assert( NULL != id_label );
    assert( NULL != name_entry );
    assert( NULL != stereotype_entry );
    assert( NULL != type_combo_box );
    assert( NULL != type_icon_grid );
    assert( NULL != description_text_view );
    assert( NULL != commit_button );

    assert( NULL != resources );
    assert( NULL != controller );
    assert( NULL != db_reader );
    assert( NULL != message_to_user );

    (*this_).id_label = id_label;
    (*this_).name_entry = name_entry;
    (*this_).stereotype_entry = stereotype_entry;
    (*this_).type_combo_box = type_combo_box;
    (*this_).type_icon_grid = type_icon_grid;
    (*this_).description_text_view = description_text_view;
    (*this_).commit_button = commit_button;

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).database = database;
    (*this_).message_to_user = message_to_user;
    (*this_).sync_dir = GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI;
    data_id_init_void( &((*this_).latest_created_id) );
    data_id_init_void( &((*this_).second_latest_id) );
    data_diagram_init_empty( &((*this_).private_diagram_cache) );
    data_classifier_init_empty( &((*this_).private_classifier_cache) );
    data_feature_init_empty( &((*this_).private_feature_cache) );
    data_relationship_init_empty( &((*this_).private_relationship_cache) );
    data_id_init_void( &((*this_).selected_object_id) );

    gui_attributes_editor_types_init( &((*this_).type_lists), resources );

    /* update widgets */
    gui_attributes_editor_update_widgets( this_ );

    U8_TRACE_END();
}

void gui_attributes_editor_destroy ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    data_id_destroy( &((*this_).selected_object_id) );
    data_diagram_destroy( &((*this_).private_diagram_cache) );
    data_classifier_destroy( &((*this_).private_classifier_cache) );
    data_feature_destroy( &((*this_).private_feature_cache) );
    data_relationship_destroy( &((*this_).private_relationship_cache) );
    data_id_destroy( &((*this_).second_latest_id) );
    data_id_destroy( &((*this_).latest_created_id) );

    gui_attributes_editor_types_destroy( &((*this_).type_lists) );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).message_to_user = NULL;

    (*this_).id_label = NULL;
    (*this_).name_entry = NULL;
    (*this_).stereotype_entry = NULL;
    (*this_).type_combo_box = NULL;
    (*this_).type_icon_grid = NULL;
    (*this_).description_text_view = NULL;

    U8_TRACE_END();
}

void gui_attributes_editor_update_widgets ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    if ( (*this_).sync_dir == GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI )
    {
        gui_attributes_editor_private_id_update_view( this_ );
        gui_attributes_editor_private_name_update_view ( this_ );
        gui_attributes_editor_private_stereotype_update_view( this_ );
        gui_attributes_editor_private_description_update_view( this_ );
        gui_attributes_editor_private_type_update_view( this_ );
    }
    else
    {
        /* widgets are not updated in GUI TO DB mode */
        U8_LOG_WARNING( "gui_attributes_editor_update_widgets called in GUI_ATTRIBUTES_EDITOR_SYNC_DIR_GUI_TO_DB mode!" );
    }

    U8_TRACE_END();
}

void gui_attributes_editor_commit_changes ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    const gui_attributes_editor_sync_dir_t original_dir = (*this_).sync_dir;
    if ( (*this_).sync_dir == GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI )
    {
        U8_LOG_EVENT( "gui_attributes_editor entering state GUI_ATTRIBUTES_EDITOR_SYNC_DIR_GUI_TO_DB." );
        (*this_).sync_dir = GUI_ATTRIBUTES_EDITOR_SYNC_DIR_GUI_TO_DB;
    }

    gui_attributes_editor_private_name_commit_changes ( this_ );
    gui_attributes_editor_private_stereotype_commit_changes ( this_ );
    gui_attributes_editor_private_description_commit_changes ( this_ );

    if ( original_dir == GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI )
    {
        U8_LOG_EVENT( "gui_attributes_editor leaving state GUI_ATTRIBUTES_EDITOR_SYNC_DIR_GUI_TO_DB." );
        (*this_).sync_dir = GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI;
        gui_attributes_editor_private_load_object( this_, (*this_).selected_object_id );
        gui_attributes_editor_update_widgets( this_ );
    }

    U8_TRACE_END();
}

void gui_attributes_editor_trace ( const gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO( "gui_attributes_editor_t" );
    U8_TRACE_INFO( "- selected_object_id:" );
    data_id_trace( &((*this_).selected_object_id) );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            U8_TRACE_INFO( "- cached object: none" );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            U8_TRACE_INFO( "- cached object:" );
            data_classifier_trace( &((*this_).private_classifier_cache) );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            U8_TRACE_INFO( "- cached object:" );
            data_feature_trace( &((*this_).private_feature_cache) );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            U8_TRACE_INFO( "- cached object:" );
            data_relationship_trace( &((*this_).private_relationship_cache) );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            U8_TRACE_INFO( "- cached object: ANOMALY, UNEXPECTED VALUE OF data_table_t" );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            U8_TRACE_INFO( "- cached object:" );
            data_diagram_trace( &((*this_).private_diagram_cache) );
        }
        break;

        default:
        {
            U8_TRACE_INFO( "- cached object: ERROR, ILLEGAL ENUM VALUE OF data_table_t" );
        }
        break;
    }

    /* id: */
    if ( U8_TRACE_ACTIVE )
    {
        GtkLabel *id_widget;
        const char* text;
        id_widget = GTK_LABEL( (*this_).id_label );
        text = gtk_label_get_text( id_widget );

        U8_TRACE_INFO_STR( "- visible id:", text );
    }

    /* name: */
    if ( U8_TRACE_ACTIVE )
    {
        const char* text;
        GtkEntry *const name_widget = GTK_ENTRY( (*this_).name_entry );
#if ( GTK_MAJOR_VERSION >= 4 )
        GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
        text = gtk_entry_buffer_get_text( name_buf );
#else
        text = gtk_entry_get_text( name_widget );
#endif
        U8_TRACE_INFO_STR( "- visible name:", text );
    }

    /* stereotype: */
    if ( U8_TRACE_ACTIVE )
    {
        const char* text;
        GtkEntry *const stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );
#if ( GTK_MAJOR_VERSION >= 4 )
        GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
        text = gtk_entry_buffer_get_text( stereotype_buf );
#else
        text = gtk_entry_get_text( stereotype_widget );
#endif
        U8_TRACE_INFO_STR( "- visible stereotype:", text );
    }

    /* type: */
    if ( U8_TRACE_ACTIVE )
    {
        GtkComboBox *type_widget;
        int obj_type;
        int index;
        type_widget = GTK_COMBO_BOX( (*this_).type_combo_box );
        index = gtk_combo_box_get_active ( type_widget );
        obj_type = gtk_helper_tree_model_get_id( gtk_combo_box_get_model( type_widget ), 0, index );
        U8_TRACE_INFO_INT( "- visible type id:", obj_type );
    }

    /* description: */
    if ( U8_TRACE_ACTIVE )
    {
        GtkTextView *description_widget;
        GtkTextBuffer *buffer;
        description_widget = GTK_TEXT_VIEW( (*this_).description_text_view );
        buffer = gtk_text_view_get_buffer ( description_widget );
        GtkTextIter start;
        GtkTextIter end;
        gtk_text_buffer_get_start_iter ( buffer, &start );
        gtk_text_buffer_get_end_iter ( buffer, &end );
        const char* text;
        text = gtk_text_buffer_get_text ( buffer, &start, &end, false );
        U8_TRACE_INFO_STR( "- visible description:", text );
    }

    U8_TRACE_INFO_STR( "- sync mode:", ((*this_).sync_dir==GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI)?"db-2-gui":"GUI_TO_DB" );

    U8_TRACE_END();
}

/* ================================ USER INPUT CALLBACKS ================================ */

#if ( GTK_MAJOR_VERSION >= 4 )
void gui_attributes_editor_name_focus_left_callback( GtkEventControllerFocus* self, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER(self) ) == GTK_WIDGET( (*this_).name_entry ) );

    //gui_attributes_editor_private_name_commit_changes( this_ );
    gui_attributes_editor_commit_changes( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}
#else
gboolean gui_attributes_editor_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( GTK_ENTRY( widget ) == GTK_ENTRY( (*this_).name_entry ) );

    //gui_attributes_editor_private_name_commit_changes( this_ );
    gui_attributes_editor_commit_changes( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}
#endif

void gui_attributes_editor_name_enter_callback ( GtkEntry *widget, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( GTK_ENTRY( widget ) == GTK_ENTRY( (*this_).name_entry ) );

    //gui_attributes_editor_private_name_commit_changes( this_ );
    gui_attributes_editor_commit_changes ( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

#if ( GTK_MAJOR_VERSION >= 4 )
void gui_attributes_editor_stereotype_focus_left_callback( GtkEventControllerFocus* self, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER(self) ) == GTK_WIDGET( (*this_).stereotype_entry ) );

    //gui_attributes_editor_private_stereotype_commit_changes( this_ );
    gui_attributes_editor_commit_changes( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}
#else
gboolean gui_attributes_editor_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( GTK_ENTRY(widget) == GTK_ENTRY( (*this_).stereotype_entry ) );

    //gui_attributes_editor_private_stereotype_commit_changes( this_ );
    gui_attributes_editor_commit_changes( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}
#endif

void gui_attributes_editor_stereotype_enter_callback ( GtkEntry *widget, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( GTK_ENTRY(widget) == GTK_ENTRY( (*this_).stereotype_entry ) );

    //gui_attributes_editor_private_stereotype_commit_changes( this_ );
    gui_attributes_editor_commit_changes ( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_type_changed_callback ( GtkComboBox *widget, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_attributes_editor_t *this_;
    this_ = (gui_attributes_editor_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_COMBO_BOX( widget ) == GTK_COMBO_BOX( (*this_).type_combo_box ) );

    /* get type id from widget */
    GtkComboBox *type_widget;
    int obj_type;
    int index;
    type_widget = GTK_COMBO_BOX( (*this_).type_combo_box );
    index = gtk_combo_box_get_active ( type_widget );
    U8_TRACE_INFO_INT( "selected:", index );
    obj_type = gtk_helper_tree_model_get_id( gtk_combo_box_get_model( type_widget ), 0, index );

    /* commit possibly changed texts before causing update events */
    gui_attributes_editor_commit_changes( this_ );

    gui_attributes_editor_private_type_commit_changes( this_, obj_type );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_type_shortlist_callback ( GtkIconView *iconview, GtkTreePath *path, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_attributes_editor_t *this_;
    this_ = (gui_attributes_editor_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_ICON_VIEW( iconview ) == GTK_ICON_VIEW( (*this_).type_icon_grid ) );

    /* get index from path */
    assert( NULL != path );
    assert( gtk_tree_path_get_depth(path) == 1);
    const gint * const indices = gtk_tree_path_get_indices(path);
    assert( NULL != indices );
    const uint32_t index = indices[0];
    U8_TRACE_INFO_INT( "selected:", index );
    int obj_type;
    obj_type = gtk_helper_tree_model_get_id( gtk_icon_view_get_model( (*this_).type_icon_grid ), 0, index );

    /* commit possibly changed texts before causing update events */
    gui_attributes_editor_commit_changes( this_ );

    gui_attributes_editor_private_type_commit_changes ( this_, obj_type );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

#if ( GTK_MAJOR_VERSION >= 4 )
void gui_attributes_editor_description_focus_left_callback( GtkEventControllerFocus* self, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER(self) ) == GTK_WIDGET( (*this_).description_text_view ) );

    gui_attributes_editor_commit_changes( this_ );
    //gui_attributes_editor_private_description_commit_changes( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}
#else
gboolean gui_attributes_editor_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( user_data != NULL );
    gui_attributes_editor_t *const this_ = (gui_attributes_editor_t*) user_data;
    assert( GTK_TEXT_VIEW( widget ) == GTK_TEXT_VIEW( (*this_).description_text_view ) );

    gui_attributes_editor_commit_changes( this_ );
    //gui_attributes_editor_private_description_commit_changes( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}
#endif

/* ================================ SELECTION or MODEL CHANGED CALLBACKS ================================ */

void gui_attributes_editor_focused_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_attributes_editor_t *this_;
    this_ = (gui_attributes_editor_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals( &((*this_).selected_object_id), id ) )
    {
        /* store all changes on the old object */
        gui_attributes_editor_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_attributes_editor_private_load_object( this_, *id );

        /* update all widgets now */
        gui_attributes_editor_update_widgets( this_ );

        /* select the name so that typing replaces it immediately */
        /* latest_created_id allows to check if selected id new */
        if ( ( data_id_equals( &((*this_).latest_created_id), &((*this_).selected_object_id) ) )
           || ( data_id_equals( &((*this_).second_latest_id), &((*this_).selected_object_id) ) ) )
        {
            gtk_widget_grab_focus( GTK_WIDGET((*this_).name_entry) );
            /* the grab focus may cause focus-lost signals - which update the widgets */
            gtk_editable_select_region( GTK_EDITABLE((*this_).name_entry), 0, -1 );
            U8_TRACE_INFO( "gui_attributes_editor called gtk_widget_grab_focus" );
        }
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_attributes_editor_t *this_;
    this_ = (gui_attributes_editor_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        /* store all changes on the old object */
        gui_attributes_editor_commit_changes( this_ );
    }
    else if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
    {
        data_id_reinit_void( &((*this_).latest_created_id) );
        data_id_reinit_void( &((*this_).second_latest_id) );
        /* data_id_t nothing;
        data_id_init_void( &nothing );
        gui_attributes_editor_private_load_object( this_, nothing );  / * clear cached data */
        gui_attributes_editor_private_load_object( this_, DATA_ID_VOID );  /* clear cached data */
        gui_attributes_editor_update_widgets ( this_ );
    }
    else if ( data_id_equals( &id, &((*this_).selected_object_id) ) )
    {
        /* DO NOT STORE DATA IN A DATA CHANGED CALLBACK - MAY CAUSE ENDLESS RECURSION */
        data_change_message_trace( msg );
        if ( (*this_).sync_dir == GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI )
        {
            /* overwrite/update displayed values */
            gui_attributes_editor_private_load_object( this_, id );  /* checks if object still exists */
            gui_attributes_editor_update_widgets ( this_ );
        }
    }
    else if ( evt_type == DATA_CHANGE_EVENT_TYPE_CREATE )
    {
        /* diagram elements should not be remembered, only the new classifier is important */
        if ( DATA_TABLE_DIAGRAMELEMENT != data_id_get_table( &id ) )
        {
            (*this_).second_latest_id = (*this_).latest_created_id;
            (*this_).latest_created_id = id;
        }
    };

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

/* ================================ PRIVATE METHODS ================================ */

void gui_attributes_editor_private_load_object ( gui_attributes_editor_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();

    /* before overwriting the current data, trace this_: */
    gui_attributes_editor_trace( this_ );
    if ( (*this_).sync_dir != GUI_ATTRIBUTES_EDITOR_SYNC_DIR_DB_TO_GUI )
    {
        U8_LOG_WARNING( "gui_attributes_editor_private_load_object called in GUI_ATTRIBUTES_EDITOR_SYNC_DIR_GUI_TO_DB mode!" );
    }

    switch ( data_id_get_table(&id) )
    {
        case DATA_TABLE_VOID:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
            data_id_reinit_void( &((*this_).selected_object_id) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            /* load the object */
            {
                u8_error_t db_err;

                data_classifier_destroy( &((*this_).private_classifier_cache) );
                db_err = data_database_reader_get_classifier_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_classifier_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a classifier" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            /* load the object */
            {
                u8_error_t db_err;

                data_feature_destroy( &((*this_).private_feature_cache) );
                db_err = data_database_reader_get_feature_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_feature_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a feature" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_feature_reinit_empty( &((*this_).private_feature_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );

            /* load the object */
            {
                u8_error_t db_err;

                data_relationship_destroy( &((*this_).private_relationship_cache) );
                db_err = data_database_reader_get_relationship_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_relationship_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a relationship" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
            data_id_reinit_void( &((*this_).selected_object_id) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            /* load the object */
            {
                u8_error_t db_err;

                data_diagram_destroy( &((*this_).private_diagram_cache) );
                db_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_diagram_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == (db_err & U8_ERROR_STRING_BUFFER_EXCEEDED) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( U8_ERROR_NONE != (db_err & ~(U8_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );

            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
            data_id_reinit_void( &((*this_).selected_object_id) );
        }
        break;
    }

    /* after loading the current data, trace this_: */
    gui_attributes_editor_trace( this_ );

    U8_TRACE_END();
}

void gui_attributes_editor_private_name_commit_changes ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    const char* text;
    GtkEntry *const name_widget = GTK_ENTRY( (*this_).name_entry );
#if ( GTK_MAJOR_VERSION >= 4 )
    GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
    text = gtk_entry_buffer_get_text( name_buf );
#else
    text = gtk_entry_get_text( name_widget );
#endif

    U8_TRACE_INFO_STR( "text:", text );

    u8_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            U8_TRACE_INFO( "no object selected where name can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const char* unchanged_text;
            unchanged_text = data_classifier_get_name_const( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_name( class_ctrl,
                                                                              data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                              text
                                                                            );
                if ( U8_ERROR_DUPLICATE_NAME == ctrl_err )
                {
                    gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                                       GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                       GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                       text
                                                                     );
                }
                else if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update name failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char* unchanged_text;
            unchanged_text = data_feature_get_key_const( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_key( class_ctrl,
                                                                          data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                          text
                                                                        );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update key/name failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const char* unchanged_text;
            unchanged_text = data_relationship_get_name_const( &((*this_).private_relationship_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_relationship_name( class_ctrl,
                                                                                data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                text
                                                                              );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update name failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            U8_LOG_WARNING( "no object selected where name can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            const char* unchanged_text;
            unchanged_text = data_diagram_get_name_const( &((*this_).private_diagram_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_diagram_controller_update_diagram_name( diag_ctrl,
                                                                        data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                        text
                                                                      );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update name failed:", ctrl_err );
                }
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_attributes_editor_private_stereotype_commit_changes ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    const char* text;
    GtkEntry *const stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );
#if ( GTK_MAJOR_VERSION >= 4 )
    GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
    text = gtk_entry_buffer_get_text( stereotype_buf );
#else
    text = gtk_entry_get_text( stereotype_widget );
#endif

    U8_TRACE_INFO_STR( "text:", text );

    u8_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            U8_TRACE_INFO( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const char* unchanged_text;
            unchanged_text = data_classifier_get_stereotype_const( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update stereotype failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char* unchanged_text;
            unchanged_text = data_feature_get_value_const( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_value ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update value/stereotype failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            U8_TRACE_INFO( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            U8_LOG_WARNING( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            U8_TRACE_INFO( "no object selected where stereotype can be updated." );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_attributes_editor_private_type_commit_changes ( gui_attributes_editor_t *this_, int obj_type )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO_INT( "obj_type:", obj_type );

    u8_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            U8_TRACE_INFO( "no object selected where type can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            data_classifier_type_t unchanged_main_type;
            unchanged_main_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
            if ( obj_type != unchanged_main_type )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_main_type ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update main type failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            data_feature_type_t unchanged_main_type;
            unchanged_main_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
            if ( obj_type != unchanged_main_type )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_main_type ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update main type failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            data_relationship_type_t unchanged_main_type;
            unchanged_main_type = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
            if ( obj_type != unchanged_main_type )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_relationship_main_type ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update main type failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            U8_LOG_WARNING( "no object selected where type can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            data_diagram_type_t unchanged_type;
            unchanged_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            if ( obj_type != unchanged_type )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update type failed:", ctrl_err );
                }
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_attributes_editor_private_description_commit_changes ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();
    GtkTextView *description_widget;
    GtkTextBuffer *buffer;
    description_widget = GTK_TEXT_VIEW( (*this_).description_text_view );
    buffer = gtk_text_view_get_buffer ( description_widget );

    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter ( buffer, &start );
    gtk_text_buffer_get_end_iter ( buffer, &end );

    const char* text;
    text = gtk_text_buffer_get_text ( buffer, &start, &end, false );

    U8_TRACE_INFO_STR( "text:", text );

    u8_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            U8_TRACE_INFO( "no object selected where description can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const char* unchanged_text;
            unchanged_text = data_classifier_get_description_const( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_description ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char* unchanged_text;
            unchanged_text = data_feature_get_description_const( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_description( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const char* unchanged_text;
            unchanged_text = data_relationship_get_description_const( &((*this_).private_relationship_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_relationship_description( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            U8_LOG_WARNING( "no object selected where description can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            const char* unchanged_text;
            unchanged_text = data_diagram_get_description_const( &((*this_).private_diagram_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_diagram_controller_update_diagram_description ( diag_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else if ( U8_ERROR_NONE != ctrl_err )
                {
                    U8_LOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_attributes_editor_private_id_update_view ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();
    GtkLabel *id_widget;
    id_widget = GTK_LABEL( (*this_).id_label );

    char data_id_string_buf[DATA_ID_MAX_UTF8STRING_SIZE];
    utf8stringbuf_t data_id_string = UTF8STRINGBUF( data_id_string_buf );
    utf8stringbuf_clear( data_id_string );
    if ( data_id_is_valid( &((*this_).selected_object_id) ) )
    {
        utf8error_t str_err = data_id_to_utf8stringbuf ( &((*this_).selected_object_id), data_id_string );
        if ( UTF8ERROR_SUCCESS != str_err )
        {
            U8_LOG_ERROR_HEX( "data_id_to_utf8stringbuf failed:", str_err );
        }
    }
    gtk_label_set_text ( id_widget, utf8stringbuf_get_string( data_id_string ) );

    U8_TRACE_END();
}

void gui_attributes_editor_private_name_update_view ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();
    GtkEntry *name_widget;
    name_widget = GTK_ENTRY( (*this_).name_entry );

    switch ( data_id_get_table( &((*this_).selected_object_id )) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( name_widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char *const text
                = data_classifier_get_name_const( &((*this_).private_classifier_cache) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
#endif
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char *const text
                = data_feature_get_key_const( &((*this_).private_feature_cache) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
#endif
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char *const text
                = data_relationship_get_name_const( &((*this_).private_relationship_cache) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
#endif
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( name_widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char *const text
                = data_diagram_get_name_const( &((*this_).private_diagram_cache) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
#endif
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, "", 0 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), "" );
#endif
        }
        break;
    }

    U8_TRACE_END();
}

void gui_attributes_editor_private_stereotype_update_view ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();
    GtkEntry *stereotype_widget;
    stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( stereotype_widget ) );
            /*gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );*/
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char *const text
                = data_classifier_get_stereotype_const( &((*this_).private_classifier_cache) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, text, -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), text );
#endif
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char *const text
                = data_feature_get_value_const( &((*this_).private_feature_cache) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, text, -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), text );
#endif
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            /*gtk_widget_hide( GTK_WIDGET ( stereotype_widget ) );*/
            /* -- do not hide - otherwise the user interface looks inhomogenous -- */
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, "    -- n/a --", -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );
#endif
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide( GTK_WIDGET ( stereotype_widget ) );
            /*gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );*/
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            /*gtk_widget_hide( GTK_WIDGET ( stereotype_widget ) );*/
            /* -- do not hide - otherwise the user interface looks inhomogenous -- */
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, "    -- n/a --", -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );
#endif
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_attributes_editor_private_type_update_view ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();
    GtkComboBox *type_widget;
    type_widget = GTK_COMBO_BOX( (*this_).type_combo_box );
    GtkIconView *type_icon_grid;
    type_icon_grid = (*this_).type_icon_grid;

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            gtk_widget_hide ( GTK_WIDGET ( type_widget ) );
            const GtkListStore * const undef_type_list = gui_attributes_editor_types_get_undef( &((*this_).type_lists) );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( undef_type_list ) );
            /* prevent that a user accidentally enters a type for a non-existing object */

            /* the icon grid: */
            gtk_widget_hide ( GTK_WIDGET ( type_icon_grid ) );
            gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( undef_type_list ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const data_classifier_type_t class_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
            const GtkListStore * const classifier_type_list = gui_attributes_editor_types_get_classifiers( &((*this_).type_lists) );
            const int index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( classifier_type_list ), 0, class_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( classifier_type_list ) );
            if ( index != -1 ) {
                /* this set_active call is critical because it causes a callback to gui_attributes_editor_type_changed_callback */
                gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
            }
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            /* the icon grid: */
            gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( classifier_type_list ) );
            gtk_widget_show ( GTK_WIDGET ( type_icon_grid ) );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const data_feature_type_t feature_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
            if ( DATA_FEATURE_TYPE_LIFELINE == feature_type )
            {
                const GtkListStore * const lifeline_type_list = gui_attributes_editor_types_get_feature_lifeline( &((*this_).type_lists) );
                const int index2 = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( lifeline_type_list ), 0, feature_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( lifeline_type_list ) );
                if ( index2 != -1 ) {
                    /* this set_active call is critical because it causes a callback to gui_attributes_editor_type_changed_callback */
                    gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index2 );
                }
                gtk_widget_show ( GTK_WIDGET ( type_widget ) );

                /* the icon grid: */
                gtk_widget_hide ( GTK_WIDGET ( type_icon_grid ) );
                gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( lifeline_type_list ) );
            }
            else
            {
                const GtkListStore * const feature_type_list = gui_attributes_editor_types_get_features( &((*this_).type_lists) );
                const int index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( feature_type_list ), 0, feature_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( feature_type_list ) );
                if ( index != -1 ) {
                    /* this set_active call is critical because it causes a callback to gui_attributes_editor_type_changed_callback */
                    gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
                }
                gtk_widget_show ( GTK_WIDGET ( type_widget ) );

                /* the icon grid: */
                gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( feature_type_list ) );
                gtk_widget_show ( GTK_WIDGET ( type_icon_grid ) );
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const data_relationship_type_t relationship_type = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
            const GtkListStore * const relationship_type_list = gui_attributes_editor_types_get_relationships( &((*this_).type_lists) );
            const int index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( relationship_type_list ), 0, relationship_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( relationship_type_list ) );
            if ( index != -1 ) {
                /* this set_active call is critical because it causes a callback to gui_attributes_editor_type_changed_callback */
                gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
            }
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            /* the icon grid: */
            gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( relationship_type_list ) );
            gtk_widget_show ( GTK_WIDGET ( type_icon_grid ) );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( type_widget ) );
            const GtkListStore * const undef_type_list = gui_attributes_editor_types_get_undef( &((*this_).type_lists) );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( undef_type_list ) );

            /* the icon grid: */
            gtk_widget_hide ( GTK_WIDGET ( type_icon_grid ) );
            gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( undef_type_list ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            const data_diagram_type_t diag_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            const GtkListStore * const diagram_type_list = gui_attributes_editor_types_get_diagrams( &((*this_).type_lists) );
            const int index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( diagram_type_list ), 0, diag_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( diagram_type_list ) );
            if ( index != -1 ) {
                /* this set_active call is critical because it causes a callback to gui_attributes_editor_type_changed_callback */
                gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
            }
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            /* the icon grid: */
            gtk_icon_view_set_model( type_icon_grid, GTK_TREE_MODEL( diagram_type_list ) );
            gtk_widget_show ( GTK_WIDGET ( type_icon_grid ) );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

//#if (( GTK_MAJOR_VERSION == 4 ) && ( GTK_MINOR_VERSION < 7 ))
#if ( GTK_MAJOR_VERSION >= 4 )
    /* workaround for https://gitlab.gnome.org/GNOME/gtk/-/issues/4856 */
    gtk_widget_set_size_request( GTK_WIDGET(type_icon_grid), 7*40, 5*30 );
    //gtk_layout_manager_layout_changed( gtk_widget_get_layout_manager( GTK_WIDGET(type_icon_grid) ));
#else
#endif

    U8_TRACE_END();
}

void gui_attributes_editor_private_description_update_view ( gui_attributes_editor_t *this_ )
{
    U8_TRACE_BEGIN();

    GtkTextView *description_widget;
    GtkTextBuffer *buffer;
    description_widget = GTK_TEXT_VIEW( (*this_).description_text_view );
    buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW( description_widget ) );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( description_widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_classifier_get_description_const( &((*this_).private_classifier_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_feature_get_description_const( &((*this_).private_feature_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_relationship_get_description_const( &((*this_).private_relationship_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( description_widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_diagram_get_description_const( &((*this_).private_diagram_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

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
