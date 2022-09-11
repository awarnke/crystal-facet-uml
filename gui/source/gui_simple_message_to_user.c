/* File: gui_simple_message_to_user.c; Copyright and License: see below */

#include "gui_simple_message_to_user.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdbool.h>
#include <assert.h>

void gui_simple_message_to_user_init ( gui_simple_message_to_user_t *this_, GtkWidget *text_label, GtkWidget *icon_image, gui_resources_t *res )
{
    TRACE_BEGIN();
    assert ( text_label != NULL );
    assert ( icon_image != NULL );
    assert ( res != NULL );

    (*this_).type_id = GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE;
    (*this_).text_label = text_label;
    (*this_).icon_image = icon_image;
    (*this_).res = res;

    (*this_).private_temp_str = utf8stringbuf_init( sizeof((*this_).private_temp_buf), (*this_).private_temp_buf );
    utf8stringbuf_clear( (*this_).private_temp_str );

    TRACE_END();
}

void gui_simple_message_to_user_destroy ( gui_simple_message_to_user_t *this_ )
{
    TRACE_BEGIN();

    (*this_).text_label = NULL;
    (*this_).icon_image = NULL;
    (*this_).res = NULL;

    TRACE_END();
}

void gui_simple_message_to_user_show_message ( gui_simple_message_to_user_t *this_,
                                               gui_simple_message_type_t type_id,
                                               gui_simple_message_content_t content_id )
{
    TRACE_BEGIN();

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    switch ( content_id )
    {
        case GUI_SIMPLE_MESSAGE_CONTENT_ABOUT:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str,
                                        "This is " META_INFO_PROGRAM_NAME_STR " version "
                                    );
            utf8stringbuf_append_str( (*this_).private_temp_str,
                                        META_VERSION_STR
                                    );
            utf8stringbuf_append_str( (*this_).private_temp_str,
                                        "\n"
                                        "License: " META_INFO_LICENSE_STR "\n"
                                        "Copyright: " META_INFO_COPYRIGHT_STR "\n"
                                        "Thanks to all who have contributed to improving and deploying this tool."
                                    );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "Maximum string length exceeded." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "Nothing selected." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_FOCUS:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "No element is focused." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "Some objects could not be deleted: They are still referenced/used." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_UNDO:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "No more actions to be un-done." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_UNDO_NOT_POSSIBLE:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "No more actions can be un-done: undo list at limit." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_REDO:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "No more actions to be re-done." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "No input data." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_SET_PARTLY_UNSUITABLE:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "Operation cannot be performed on all elements in the set." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_WRITE_ERROR:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "Database could not be written to the database file." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_ANCESTOR_IS_NOT_DESCENDANT:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str,
                                      "An ancestor (parent/root) diagram cannot move to a descendant (child) location.\n"
                                      "Instead, try to move a descendant out to an ancestor or sibling location"
                                    );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_DEBUG_MODE:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str,
                                      "This software was compiled in DEBUG mode.\nIt may be slower than the RELEASE version.\n"
                                      "Confidential information may be printed to syslog."
                                    );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_RELATIONSHIPS:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "The current diagram type does not allow one to create relationships." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_NO_FEATURES:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "The current diagram type does not allow one to create features." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_FEATURELESS_CLASSIFIER:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "The current classifier type does not allow one to create features." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_IS_ALWAYS_INSTANCE:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "The current classifier type does not allow one to remove the instance flag." );
        }
        break;

        case GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY:
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "The database file is open in read only mode." );
        }
        break;

        default:
        {
            TSLOG_ERROR("unexptected gui_simple_message_content_t");
        }
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));
    TSLOG_EVENT( utf8stringbuf_get_string( (*this_).private_temp_str ) );

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_quantity ( gui_simple_message_to_user_t *this_,
                                                             gui_simple_message_type_t type_id,
                                                             const gui_simple_message_content_quantity_t *content_id,
                                                             int quantity )
{
    TRACE_BEGIN();
    assert( content_id != NULL );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Current database is inconsistent; errors: " );
        utf8stringbuf_append_int( (*this_).private_temp_str, quantity );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Maximum number of windows already open: " );
        utf8stringbuf_append_int( (*this_).private_temp_str, quantity );
    }
    else
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_line ( gui_simple_message_to_user_t *this_,
                                                         gui_simple_message_type_t type_id,
                                                         const gui_simple_message_content_position_t *content_id,
                                                         int stream_line )
{
    TRACE_BEGIN();
    assert( content_id != NULL );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Invalid input data at line " );
        utf8stringbuf_append_int( (*this_).private_temp_str, stream_line );
    }
    else
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_name ( gui_simple_message_to_user_t *this_,
                                                         gui_simple_message_type_t type_id,
                                                         const gui_simple_message_content_name_t *content_id,
                                                         const char *name )
{
    TRACE_BEGIN();
    assert( content_id != NULL );
    assert( name != NULL );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Chosen database file could not be opened/created:\n" );
        utf8stringbuf_append_str( (*this_).private_temp_str, name );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Chosen database file opened/created with warning:\n" );
        utf8stringbuf_append_str( (*this_).private_temp_str, name );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Export failed, destination path: " );
        utf8stringbuf_append_str( (*this_).private_temp_str, name );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Name already in use (use copy and paste to insert the existing object): " );
        utf8stringbuf_append_str( (*this_).private_temp_str, name );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_LOADING_WAIT )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_LOADING_WAIT" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Loading DB file ...\n" );
        utf8stringbuf_append_str( (*this_).private_temp_str, name );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_EXPORTING_WAIT )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_EXPORTING_WAIT" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Exporting files ...\n" );
        utf8stringbuf_append_str( (*this_).private_temp_str, name );
    }
    else
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_names ( gui_simple_message_to_user_t *this_,
                                                          gui_simple_message_type_t type_id,
                                                          const gui_simple_message_content_names_t *content_id,
                                                          const char *list_of_names )
{
    TRACE_BEGIN();
    assert( content_id != NULL );
    assert( list_of_names != NULL );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_error ( gui_simple_message_to_user_t *this_,
                                                          gui_simple_message_type_t type_id,
                                                          const gui_simple_message_content_error_t *content_id,
                                                          const char *error_message )
{
    TRACE_BEGIN();
    assert( content_id != NULL );
    assert( error_message != NULL );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "This feature is not yet implemented: " );
        utf8stringbuf_append_str( (*this_).private_temp_str, error_message );
    }
    else
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

const char *const (gui_simple_message_to_user_private_table_name[DATA_STAT_TABLES_MAX])
    = {"void","classifiers","features","relations","classifier-occurrences","diagrams"};
const char *const (gui_simple_message_to_user_private_series_name4change[DATA_STAT_SERIES_MAX])
    = {"created","modified","deleted","ignored","warning","error"};
const char *const (gui_simple_message_to_user_private_series_name4other[DATA_STAT_SERIES_MAX])
    = {"exported","un/selected","deleted","ignored","warning","error"};

void gui_simple_message_to_user_show_message_with_stat ( gui_simple_message_to_user_t *this_,
                                                         gui_simple_message_type_t type_id,
                                                         const gui_simple_message_content_stat_t *content_id,
                                                         const data_stat_t *stat
                                                       )
{
    TRACE_BEGIN();
    assert( stat != NULL );
    assert( content_id != NULL );
    data_stat_trace( stat );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Selection cut to clipboard: \n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, true, (*this_).private_temp_str );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Selection copied to clipboard: \n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, true, (*this_).private_temp_str );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Clipboard pasted: \n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, false, (*this_).private_temp_str );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_DELETE )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_DELETE" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Selection deleted: \n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, false, (*this_).private_temp_str );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_UNDO )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_UNDO" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Undo success: \n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, false, (*this_).private_temp_str );
    }
    else if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_REDO )
    {
        TSLOG_EVENT( "GUI_SIMPLE_MESSAGE_CONTENT_REDO" );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Redo success: \n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, false, (*this_).private_temp_str );
    }
    else
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_names_and_stat( gui_simple_message_to_user_t *this_,
                                                                  gui_simple_message_type_t type_id,
                                                                  const gui_simple_message_content_names_stat_t *content_id,
                                                                  const char *list_of_names,
                                                                  const data_stat_t *stat )
{
    TRACE_BEGIN();
    assert( stat != NULL );
    assert( list_of_names != NULL );
    assert( content_id != NULL );
    data_stat_trace( stat );

    /* update type id: */
    (*this_).type_id = type_id;
    gui_simple_message_to_user_private_set_icon_image( this_, type_id );

    /* update content text: */
    utf8stringbuf_clear( (*this_).private_temp_str );
    if ( content_id == GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED )
    {
        TSLOG_EVENT_STR( "GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED", list_of_names );
        utf8stringbuf_append_str( (*this_).private_temp_str, "Files exported, format: " );
        utf8stringbuf_append_str( (*this_).private_temp_str, list_of_names );
        utf8stringbuf_append_str( (*this_).private_temp_str, "\n" );
        gui_simple_message_to_user_private_append_stat( this_, stat, true, (*this_).private_temp_str );
        if ( 0 != data_stat_get_series_count ( stat, DATA_STAT_SERIES_WARNING ) )
        {
            utf8stringbuf_append_str( (*this_).private_temp_str, "\nFor details see comments in exported files." );
        }
    }
    else
    {
        TSLOG_ERROR("unexptected content_id");
        assert(false);
    }
    gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

    /* show: */
    gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}

void gui_simple_message_to_user_private_append_stat ( gui_simple_message_to_user_t *this_,
                                                      const data_stat_t *stat,
                                                      bool alt_labels,
                                                      utf8stringbuf_t out_buf
                                                    )
{
    TRACE_BEGIN();
    assert( stat != NULL );

    bool first_series = true;
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        if ( 0 != data_stat_get_series_count ( stat, series_idx ) )
        {
            if ( first_series )
            {
                first_series = false;
            }
            else
            {
                utf8stringbuf_append_str( out_buf, "\n" );
            }
            const char *const series_name
                = alt_labels
                ? gui_simple_message_to_user_private_series_name4other[series_idx]
                : gui_simple_message_to_user_private_series_name4change[series_idx];
            utf8stringbuf_append_str( out_buf, series_name );
            utf8stringbuf_append_str( out_buf, ": " );

            bool first_table = true;
            for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLES_MAX; tables_idx ++ )
            {
                if ( DATA_TABLE_VOID != tables_idx )
                {
                    uint_fast32_t cnt = data_stat_get_count ( stat, tables_idx, series_idx );
                    if ( 0 != cnt )
                    {
                        if ( first_table )
                        {
                            first_table = false;
                        }
                        else
                        {
                            utf8stringbuf_append_str( out_buf, ", " );
                        }
                        const char *const table_name = gui_simple_message_to_user_private_table_name[tables_idx];
                        utf8stringbuf_append_str( out_buf, table_name );
                        utf8stringbuf_append_str( out_buf, ":" );
                        utf8stringbuf_append_int( out_buf, cnt );
                    }
                }
            }
        }
    }
    if ( 0 == data_stat_get_total_count ( stat ) )
    {
        utf8stringbuf_append_str( out_buf, "0" );
    }
    TRACE_INFO( utf8stringbuf_get_string( out_buf ) );

    TRACE_END();
}

void gui_simple_message_to_user_hide ( gui_simple_message_to_user_t *this_ )
{
    TRACE_BEGIN();

    (*this_).type_id = GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE;
    gtk_widget_hide( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_hide( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}


/*
Copyright 2016-2022 Andreas Warnke

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
