/* File: gui_attributes_editor.c; Copyright and License: see below */

#include "gui_attributes_editor.h"
#include "gui_attribute_type_of_classifier.h"
#include "gui_attribute_type_of_diagram.h"
#include "gui_attribute_type_of_feature.h"
#include "gui_attribute_type_of_relationship.h"
#include "draw/draw_stereotype_image.h"
#include "u8/u8_trace.h"
#include "ctrl_simple_changer.h"
#include "data_table.h"
#include "data_id.h"
#include "utf8stringbuf/utf8string.h"
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_attributes_editor_init ( gui_attributes_editor_t *this_,
                                  GtkLabel *id_label,
                                  GtkEntry *name_entry,
                                  GtkEntry *stereotype_entry,
                                  GtkDropDown *type_dropdown,
                                  GtkWidget *type_diag_grid,
                                  GtkWidget *type_clas_grid,
                                  GtkWidget *type_feat_grid,
                                  GtkWidget *type_rel_grid,
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
    assert( NULL != type_dropdown );
    assert( NULL != type_diag_grid );
    assert( NULL != type_clas_grid );
    assert( NULL != type_feat_grid );
    assert( NULL != type_rel_grid );
    assert( NULL != description_text_view );
    assert( NULL != commit_button );

    assert( NULL != resources );
    assert( NULL != controller );
    assert( NULL != db_reader );
    assert( NULL != message_to_user );

    (*this_).id_label = id_label;
    (*this_).name_entry = name_entry;
    (*this_).stereotype_entry = stereotype_entry;
    (*this_).type_dropdown = type_dropdown;
    (*this_).type_diag_grid = type_diag_grid;
    (*this_).type_clas_grid = type_clas_grid;
    (*this_).type_feat_grid = type_feat_grid;
    (*this_).type_rel_grid = type_rel_grid;
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

    gui_type_resource_list_init( &((*this_).type_lists), resources );
    (*this_).temp_search_me = GUI_TYPE_RESOURCE( g_object_new( gui_type_resource_get_type(), NULL ) );
    (*this_).previous_selected_object_id = DATA_ID_VOID;

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

    g_object_unref( (*this_).temp_search_me );
    gui_type_resource_list_destroy( &((*this_).type_lists) );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).message_to_user = NULL;

    (*this_).id_label = NULL;
    (*this_).name_entry = NULL;
    (*this_).stereotype_entry = NULL;
    (*this_).type_dropdown = NULL;
    (*this_).type_diag_grid = NULL;
    (*this_).type_clas_grid = NULL;
    (*this_).type_feat_grid = NULL;
    (*this_).type_rel_grid = NULL;
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
        GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
        text = gtk_entry_buffer_get_text( name_buf );
        U8_TRACE_INFO_STR( "- visible name:", text );
    }

    /* stereotype: */
    if ( U8_TRACE_ACTIVE )
    {
        const char* text;
        GtkEntry *const stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );
        GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
        text = gtk_entry_buffer_get_text( stereotype_buf );
        U8_TRACE_INFO_STR( "- visible stereotype:", text );
    }

    /* type: */
    if ( U8_TRACE_ACTIVE )
    {
        int obj_type;
        GObject *selected = gtk_drop_down_get_selected_item ( (*this_).type_dropdown );
        if ( selected == NULL )
        {
            obj_type = -1;
        }
        else
        {
            const data_type_t *obj_full_type = gui_type_resource_get_type_id( ((GuiTypeResource*)selected) );
            obj_type = data_type_get_type_as_int( obj_full_type );
        }
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

void gui_attributes_editor_type_changed_callback ( GObject* self, GParamSpec* pspec, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_attributes_editor_t *this_;
    this_ = (gui_attributes_editor_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_DROP_DOWN( self ) == (*this_).type_dropdown );

    /* if the selected object has changed, this callback is triggered by that change, */
    /* not by the user selecting another type */
    bool user_requested = data_id_equals( &((*this_).previous_selected_object_id), &((*this_).selected_object_id) );
    (*this_).previous_selected_object_id = (*this_).selected_object_id;

    /* get type id from widget */
    GObject *selected = gtk_drop_down_get_selected_item ( (*this_).type_dropdown );
    if (( selected != NULL )&& user_requested )
    {
        const data_type_t *const obj_full_type
            = gui_type_resource_get_type_id( ((GuiTypeResource*)selected) );
        const int obj_type = data_type_get_type_as_int( obj_full_type );
        U8_TRACE_INFO_INT( "selected type_id:", obj_type );

        /* commit possibly changed texts before causing update events */
        gui_attributes_editor_commit_changes( this_ );

        gui_attributes_editor_private_type_commit_changes( this_, obj_type );
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_type_of_diagram_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    const gui_attribute_type_of_diagram_t *const btn_data = (gui_attribute_type_of_diagram_t*) data;
    assert ( NULL != btn_data );
    gui_attributes_editor_t *const this_ = gui_attribute_type_of_diagram_get_editor( btn_data );
    assert ( NULL != this_ );
    const data_diagram_type_t selected_type = gui_attribute_type_of_diagram_get_selected_type( btn_data );
    U8_TRACE_INFO_INT( "selected type:", selected_type );

    /* commit possibly changed texts before causing update events */
    gui_attributes_editor_commit_changes( this_ );

    gui_attributes_editor_private_type_commit_changes ( this_, selected_type );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_type_of_classifier_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    const gui_attribute_type_of_classifier_t *const btn_data = (gui_attribute_type_of_classifier_t*) data;
    assert ( NULL != btn_data );
    gui_attributes_editor_t *const this_ = gui_attribute_type_of_classifier_get_editor( btn_data );
    assert ( NULL != this_ );
    const data_diagram_type_t selected_type = gui_attribute_type_of_classifier_get_selected_type( btn_data );
    U8_TRACE_INFO_INT( "selected type:", selected_type );

    /* commit possibly changed texts before causing update events */
    gui_attributes_editor_commit_changes( this_ );

    gui_attributes_editor_private_type_commit_changes ( this_, selected_type );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_type_of_feature_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    const gui_attribute_type_of_feature_t *const btn_data = (gui_attribute_type_of_feature_t*) data;
    assert ( NULL != btn_data );
    gui_attributes_editor_t *const this_ = gui_attribute_type_of_feature_get_editor( btn_data );
    assert ( NULL != this_ );
    const data_diagram_type_t selected_type = gui_attribute_type_of_feature_get_selected_type( btn_data );
    U8_TRACE_INFO_INT( "selected type:", selected_type );

    /* commit possibly changed texts before causing update events */
    gui_attributes_editor_commit_changes( this_ );

    gui_attributes_editor_private_type_commit_changes ( this_, selected_type );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_attributes_editor_type_of_relationship_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    const gui_attribute_type_of_relationship_t *const btn_data = (gui_attribute_type_of_relationship_t*) data;
    assert ( NULL != btn_data );
    gui_attributes_editor_t *const this_ = gui_attribute_type_of_relationship_get_editor( btn_data );
    assert ( NULL != this_ );
    const data_diagram_type_t selected_type = gui_attribute_type_of_relationship_get_selected_type( btn_data );
    U8_TRACE_INFO_INT( "selected type:", selected_type );

    /* commit possibly changed texts before causing update events */
    gui_attributes_editor_commit_changes( this_ );

    gui_attributes_editor_private_type_commit_changes ( this_, selected_type );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

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

                if ( u8_error_contains( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a classifier" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
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

                if ( u8_error_contains( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a feature" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
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

                if ( u8_error_contains( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a relationship" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
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

                if ( u8_error_contains( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
                {
                    U8_LOG_ERROR( "U8_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                if ( u8_error_more_than( db_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
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
    GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
    text = gtk_entry_buffer_get_text( name_buf );

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
            const char *const unchanged_text
                = data_classifier_get_name_const( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                u8_error_t handled_err;
                {
                    ctrl_simple_changer_t name_disambiguator;
                    ctrl_simple_changer_init( &name_disambiguator, (*this_).controller, (*this_).db_reader );
                    ctrl_err = ctrl_simple_changer_update_classifier_name( &name_disambiguator,
                                                                           data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                           text,
                                                                           &handled_err
                                                                         );
                    ctrl_simple_changer_destroy( &name_disambiguator );
                }

                if ( u8_error_contains( handled_err, U8_ERROR_DUPLICATE_NAME ) )
                {
                    gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                                       GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                       GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                       text
                                                                     );
                }
                else if ( u8_error_contains( ctrl_err, U8_ERROR_STRING_BUFFER_EXCEEDED ) )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                           );
                }
                else if ( u8_error_contains( ctrl_err, U8_ERROR_READ_ONLY_DB ) )
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
            const char *const unchanged_text
                = data_feature_get_key_const( &((*this_).private_feature_cache) );
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
            const char *const unchanged_text
                = data_relationship_get_name_const( &((*this_).private_relationship_cache) );
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
            const char *const unchanged_text
                = data_diagram_get_name_const( &((*this_).private_diagram_cache) );
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
    GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
    text = gtk_entry_buffer_get_text( stereotype_buf );

    U8_TRACE_INFO_STR( "text:", text );

    ctrl_classifier_controller_t *const class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
    ctrl_diagram_controller_t *const diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );
    u8_error_t ctrl_update_err = U8_ERROR_NONE;
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
            const char *const unchanged_text = data_classifier_get_stereotype_const( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_classifier_controller_update_classifier_stereotype( class_ctrl,
                                                                                           data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                           text
                                                                                         );
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char *const unchanged_text = data_feature_get_value_const( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_classifier_controller_update_feature_value( class_ctrl,
                                                                                   data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                   text
                                                                                 );
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const char *const unchanged_text = data_relationship_get_stereotype_const( &((*this_).private_relationship_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_classifier_controller_update_relationship_stereotype( class_ctrl,
                                                                                             data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                             text
                                                                                           );
            }
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
            const char *const unchanged_text = data_diagram_get_stereotype_const( &((*this_).private_diagram_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_diagram_controller_update_diagram_stereotype( diag_ctrl,
                                                                                     data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                     text
                                                                                   );
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    /* show warning to user in case of error: */
    if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_update_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                               );
    }
    else if ( U8_ERROR_READ_ONLY_DB == ctrl_update_err )
    {
        /* notify read-only warning to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                               );
    }
    else if ( U8_ERROR_NONE != ctrl_update_err )
    {
        U8_LOG_ERROR_HEX( "update stereotype failed:", ctrl_update_err );
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
            const data_classifier_type_t unchanged_main_type
                = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
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
            const data_feature_type_t unchanged_main_type
                = data_feature_get_main_type( &((*this_).private_feature_cache) );
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
            const data_relationship_type_t unchanged_main_type
                = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
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
            const data_diagram_type_t unchanged_type
                = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            if ( obj_type != unchanged_type )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );
                data_stat_t stat;
                data_stat_init(&stat);
                ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl,
                                                                         data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                         obj_type,
                                                                         &stat
                                                                       );
                if ( U8_ERROR_READ_ONLY_DB == ctrl_err )
                {
                    /* notify read-only warning to user */
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                           );
                }
                else
                {
                    if ( U8_ERROR_NONE != ctrl_err )
                    {
                        U8_LOG_ERROR_HEX( "update type failed:", ctrl_err );
                    }
                    gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                                        GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                        GUI_SIMPLE_MESSAGE_CONTENT_TYPE_CHANGE,
                                                                        &stat
                                                                      );
                }
                data_stat_destroy(&stat);
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

    ctrl_classifier_controller_t *const class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
    ctrl_diagram_controller_t *const diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );
    u8_error_t ctrl_update_err = U8_ERROR_NONE;
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
            const char *const unchanged_text
                = data_classifier_get_description_const( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_classifier_controller_update_classifier_description( class_ctrl,
                                                                                            data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                            text
                                                                                          );

                /* report errors in svg images in description in case of stereotype: */
                const data_classifier_type_t main_type
                    = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
                if ( main_type == DATA_CLASSIFIER_TYPE_STEREOTYPE )
                {
                    geometry_rectangle_t view_rect;
                    u8_error_info_t svg_err_info;
                    draw_stereotype_image_t svg_parser;
                    draw_stereotype_image_init( &svg_parser );
                    const u8_error_t svg_err
                        = draw_stereotype_image_parse_svg_xml( &svg_parser,
                                                               text,  /* drawing_directives */
                                                               &view_rect,
                                                               &svg_err_info
                                                             );
                    draw_stereotype_image_destroy( &svg_parser );
                    if ( svg_err == U8_ERROR_NOT_FOUND )
                    {
                        U8_TRACE_INFO( "the stereotype has no image." );
                    }
                    else if ( svg_err != U8_ERROR_NONE )
                    {
                        gui_simple_message_to_user_show_error_info( (*this_).message_to_user, &svg_err_info );
                    }
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char *const unchanged_text
                = data_feature_get_description_const( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_classifier_controller_update_feature_description( class_ctrl,
                                                                                         data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                         text
                                                                                       );
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const char *const unchanged_text
                = data_relationship_get_description_const( &((*this_).private_relationship_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_classifier_controller_update_relationship_description( class_ctrl,
                                                                                              data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                              text
                                                                                            );
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
            const char *const unchanged_text
                = data_diagram_get_description_const( &((*this_).private_diagram_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_update_err = ctrl_diagram_controller_update_diagram_description( diag_ctrl,
                                                                                      data_id_get_row_id( &((*this_).selected_object_id) ),
                                                                                      text
                                                                                    );
            }
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    /* show warning to user in case of error: */
    if ( U8_ERROR_STRING_BUFFER_EXCEEDED == ctrl_update_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                    GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                    GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                                );
    }
    else if ( U8_ERROR_READ_ONLY_DB == ctrl_update_err )
    {
        /* notify read-only warning to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                    GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                    GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                );
    }
    else if ( U8_ERROR_NONE != ctrl_update_err )
    {
        U8_LOG_ERROR_HEX( "update description failed:", ctrl_update_err );
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
            gtk_widget_set_visible( GTK_WIDGET ( name_widget ), FALSE );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_set_visible( GTK_WIDGET ( name_widget ), TRUE );

            const char *const text
                = data_classifier_get_name_const( &((*this_).private_classifier_cache) );
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_set_visible( GTK_WIDGET ( name_widget ), TRUE );

            const char *const text
                = data_feature_get_key_const( &((*this_).private_feature_cache) );
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_set_visible( GTK_WIDGET ( name_widget ), TRUE );

            const char *const text
                = data_relationship_get_name_const( &((*this_).private_relationship_cache) );
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_set_visible( GTK_WIDGET ( name_widget ), FALSE );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_set_visible( GTK_WIDGET ( name_widget ), TRUE );

            const char *const text
                = data_diagram_get_name_const( &((*this_).private_diagram_cache) );
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, text, -1 /* = n_chars */ );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid data in data_id_t." );
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( name_widget );
            gtk_entry_buffer_set_text( name_buf, "", 0 /* = n_chars */ );
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
            gtk_widget_set_visible( GTK_WIDGET ( stereotype_widget ), FALSE );
            /*gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );*/
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_set_visible( GTK_WIDGET ( stereotype_widget ), TRUE );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char *const text
                = data_classifier_get_stereotype_const( &((*this_).private_classifier_cache) );
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, text, -1 /* = n_chars */ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_set_visible( GTK_WIDGET ( stereotype_widget ), TRUE );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char *const text
                = data_feature_get_value_const( &((*this_).private_feature_cache) );
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, text, -1 /* = n_chars */ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_set_visible( GTK_WIDGET ( stereotype_widget ), TRUE );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char *const text
                = data_relationship_get_stereotype_const( &((*this_).private_relationship_cache) );
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, text, -1 /* = n_chars */ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_set_visible( GTK_WIDGET ( stereotype_widget ), FALSE );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_set_visible( GTK_WIDGET ( stereotype_widget ), TRUE );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char *const text
                = data_diagram_get_stereotype_const( &((*this_).private_diagram_cache) );
            GtkEntryBuffer *const stereotype_buf = gtk_entry_get_buffer( stereotype_widget );
            gtk_entry_buffer_set_text( stereotype_buf, text, -1 /* = n_chars */ );
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
    GtkDropDown *type_widget = (*this_).type_dropdown;

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            gtk_widget_set_visible( GTK_WIDGET ( type_widget ), FALSE );
            GListStore *const undef_type_list = gui_type_resource_list_get_undef( &((*this_).type_lists) );
            gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( undef_type_list ) );

            /* hide icon grid: */
            gtk_widget_set_visible( (*this_).type_diag_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_clas_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_feat_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_rel_grid, FALSE );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const data_classifier_type_t class_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
            GListStore *const classifier_type_list = gui_type_resource_list_get_classifiers( &((*this_).type_lists) );
            gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( classifier_type_list ) );
            guint selected_pos;
            gui_type_resource_init_classifier( (*this_).temp_search_me, class_type, "", NULL );  /* type, name, icon */
            const gboolean found = g_list_store_find_with_equal_func( classifier_type_list,
                                                                      (*this_).temp_search_me,
                                                                      (GEqualFunc)&gui_type_resource_equal,
                                                                      &selected_pos
                                                                    );
            if ( found )
            {
                gtk_drop_down_set_selected( (*this_).type_dropdown, selected_pos );
            }
            gtk_widget_set_visible( GTK_WIDGET ( type_widget ), TRUE );

            /* show classifier icon grid: */
            gtk_widget_set_visible( (*this_).type_diag_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_clas_grid, TRUE );
            gtk_widget_set_visible( (*this_).type_feat_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_rel_grid, FALSE );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const data_feature_type_t feature_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
            if ( DATA_FEATURE_TYPE_LIFELINE == feature_type )
            {
                GListStore *const lifeline_type_list = gui_type_resource_list_get_feature_lifeline( &((*this_).type_lists) );
                gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( lifeline_type_list ) );
                guint selected_pos;
                gui_type_resource_init_feature( (*this_).temp_search_me, feature_type, "", NULL ); /* type, name, icon */
                const gboolean found = g_list_store_find_with_equal_func( lifeline_type_list,
                                                                          (*this_).temp_search_me,
                                                                          (GEqualFunc)&gui_type_resource_equal,
                                                                          &selected_pos
                                                                        );
                if ( found )
                {
                    gtk_drop_down_set_selected( (*this_).type_dropdown, selected_pos );
                }
                gtk_widget_set_visible( GTK_WIDGET ( type_widget ), TRUE );

                /* hide  icon grid: */
                gtk_widget_set_visible( (*this_).type_diag_grid, FALSE );
                gtk_widget_set_visible( (*this_).type_clas_grid, FALSE );
                gtk_widget_set_visible( (*this_).type_feat_grid, FALSE );
                gtk_widget_set_visible( (*this_).type_rel_grid, FALSE );
            }
            else
            {
                GListStore *const feature_type_list = gui_type_resource_list_get_features( &((*this_).type_lists) );
                gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( feature_type_list ) );
                guint selected_pos;
                gui_type_resource_init_feature( (*this_).temp_search_me, feature_type, "", NULL ); /* type, name, icon */
                const gboolean found = g_list_store_find_with_equal_func( feature_type_list,
                                                                          (*this_).temp_search_me,
                                                                          (GEqualFunc)&gui_type_resource_equal,
                                                                          &selected_pos
                                                                        );
                if ( found )
                {
                    gtk_drop_down_set_selected( (*this_).type_dropdown, selected_pos );
                }
                gtk_widget_set_visible( GTK_WIDGET ( type_widget ), TRUE );

                /* show feature icon grid: */
                gtk_widget_set_visible( (*this_).type_diag_grid, FALSE );
                gtk_widget_set_visible( (*this_).type_clas_grid, FALSE );
                gtk_widget_set_visible( (*this_).type_feat_grid, TRUE );
                gtk_widget_set_visible( (*this_).type_rel_grid, FALSE );
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const data_relationship_type_t relationship_type
                = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
            GListStore *const relationship_type_list = gui_type_resource_list_get_relationships( &((*this_).type_lists) );
            gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( relationship_type_list ) );
            guint selected_pos;
            gui_type_resource_init_relationship( (*this_).temp_search_me, relationship_type, "", NULL ); /* type, name, icon */
            const gboolean found = g_list_store_find_with_equal_func( relationship_type_list,
                                                                      (*this_).temp_search_me,
                                                                      (GEqualFunc)&gui_type_resource_equal,
                                                                      &selected_pos
                                                                    );
            if ( found )
            {
                gtk_drop_down_set_selected( (*this_).type_dropdown, selected_pos );
            }
            gtk_widget_set_visible( GTK_WIDGET ( type_widget ), TRUE );

            /* show relationship icon grid: */
            gtk_widget_set_visible( (*this_).type_diag_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_clas_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_feat_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_rel_grid, TRUE );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_set_visible( GTK_WIDGET ( type_widget ), FALSE );
            GListStore *const undef_type_list = gui_type_resource_list_get_undef( &((*this_).type_lists) );
            gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( undef_type_list ) );

            /* hide icon grid: */
            gtk_widget_set_visible( (*this_).type_diag_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_clas_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_feat_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_rel_grid, FALSE );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            const data_diagram_type_t diag_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            GListStore *const diagram_type_list = gui_type_resource_list_get_diagrams( &((*this_).type_lists) );
            gtk_drop_down_set_model( (*this_).type_dropdown, G_LIST_MODEL( diagram_type_list ) );
            guint selected_pos;
            gui_type_resource_init_diagram( (*this_).temp_search_me, diag_type, "", NULL ); /* type, name, icon */
            const gboolean found = g_list_store_find_with_equal_func( diagram_type_list,
                                                                      (*this_).temp_search_me,
                                                                      (GEqualFunc)&gui_type_resource_equal,
                                                                      &selected_pos
                                                                    );
            if ( found )
            {
                gtk_drop_down_set_selected( (*this_).type_dropdown, selected_pos );
            }
            gtk_widget_set_visible( GTK_WIDGET ( type_widget ), TRUE );

            /* show diagram icon grid: */
            gtk_widget_set_visible( (*this_).type_diag_grid, TRUE );
            gtk_widget_set_visible( (*this_).type_clas_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_feat_grid, FALSE );
            gtk_widget_set_visible( (*this_).type_rel_grid, FALSE );
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
            gtk_widget_set_visible( GTK_WIDGET ( description_widget ), FALSE );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_set_visible( GTK_WIDGET ( description_widget ), TRUE );

            const char* text;
            text = data_classifier_get_description_const( &((*this_).private_classifier_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_set_visible( GTK_WIDGET ( description_widget ), TRUE );

            const char* text;
            text = data_feature_get_description_const( &((*this_).private_feature_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_set_visible( GTK_WIDGET ( description_widget ), TRUE );

            const char* text;
            text = data_relationship_get_description_const( &((*this_).private_relationship_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_set_visible( GTK_WIDGET ( description_widget ), FALSE );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_set_visible( GTK_WIDGET ( description_widget ), TRUE );

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
