/* File: gui_simple_message_to_user.c; Copyright and License: see below */

#include "gui_simple_message_to_user.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include <assert.h>

void gui_simple_message_to_user_init ( gui_simple_message_to_user_t *this_, GtkWidget *text_label, GtkWidget *icon_image, gui_resources_t *res )
{
    TRACE_BEGIN();
    assert ( text_label != NULL );
    assert ( icon_image != NULL );
    assert ( res != NULL );

    (*this_).type_id = GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE;
    (*this_).content_id = GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MESSAGE;
    (*this_).param1_nature = GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID;
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

void gui_simple_message_to_user_show_message_with_int ( gui_simple_message_to_user_t *this_,
                                                        gui_simple_message_type_t type_id,
                                                        gui_simple_message_content_t content_id,
                                                        gui_simple_message_param_nature_t param_nature,
                                                        int int_param )
{
    TRACE_BEGIN();

    char string_of_int_buf[16];
    utf8stringbuf_t string_of_int = UTF8STRINGBUF( string_of_int_buf );
    utf8stringbuf_clear( string_of_int );
    utf8stringbuf_append_int( string_of_int, int_param );

    gui_simple_message_to_user_show_message_with_string( this_, type_id, content_id, param_nature, utf8stringbuf_get_string( string_of_int ));

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_string ( gui_simple_message_to_user_t *this_,
                                                           gui_simple_message_type_t type_id,
                                                           gui_simple_message_content_t content_id,
                                                           gui_simple_message_param_nature_t param_nature,
                                                           const char *string_param )
{
    TRACE_BEGIN();

    if ( GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE == type_id )
    {
        gui_simple_message_to_user_hide( this_ );
    }
    else
    {
        (*this_).type_id = type_id;
        (*this_).content_id = content_id;
        (*this_).param1_nature = param_nature;

        switch ( type_id )
        {
            case GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE:
            {
                assert(0);  /* this cannot happen */
            }
            break;

            case GUI_SIMPLE_MESSAGE_TYPE_INFO:
            {
                gtk_image_set_from_pixbuf ( GTK_IMAGE( (*this_).icon_image ), gui_resources_get_message_info( (*this_).res) );
            }
            break;

            case GUI_SIMPLE_MESSAGE_TYPE_WARNING:
            {
                gtk_image_set_from_pixbuf ( GTK_IMAGE( (*this_).icon_image ), gui_resources_get_message_warn( (*this_).res) );
            }
            break;

            case GUI_SIMPLE_MESSAGE_TYPE_ERROR:
            {
                gtk_image_set_from_pixbuf ( GTK_IMAGE( (*this_).icon_image ), gui_resources_get_message_error( (*this_).res) );
            }
            break;

            case GUI_SIMPLE_MESSAGE_TYPE_ABOUT:
            {
                gtk_image_set_from_pixbuf ( GTK_IMAGE( (*this_).icon_image ), gui_resources_get_crystal_facet_uml( (*this_).res) );
            }
            break;

            default:
            {
                TSLOG_ERROR("unexptected gui_simple_message_type_t");
            }
        }

        utf8stringbuf_clear( (*this_).private_temp_str );
        switch ( content_id )
        {
            case GUI_SIMPLE_MESSAGE_CONTENT_0_ABOUT:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str,
                                          "This is " META_INFO_PROGRAM_NAME_STR " version "
                                        );
                utf8stringbuf_append_str( (*this_).private_temp_str,
                                          META_VERSION_STR
                                        );
                utf8stringbuf_append_str( (*this_).private_temp_str,
                                          "\n"
                                          "Copyright: " META_INFO_COPYRIGHT_STR "\n"
                                          "License: " META_INFO_LICENSE_STR
                                        );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_N_DB_FILE_NOT_OPENED:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_NAME );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Chosen database file could not be opened/created:\n" );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_N_DB_FILE_OPENED_WITH_ERROR:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_NAME );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Chosen database file opened/created with warning:\n" );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_Q_DB_INCONSISTENT:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_QUANTITY );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Current database is inconsistent; errors: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_Q_MAX_WINDOWS_ALREADY_OPEN:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_QUANTITY );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Maximum number of windows already open: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_STRING_TRUNCATED:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Maximum string length exceeded." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_E_NOT_YET_IMPLEMENTED:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_ERROR_EXPL );
                utf8stringbuf_append_str( (*this_).private_temp_str, "This feature is not yet implemented: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_NO_SELECTION:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Nothing selected." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_DELETING_NOT_POSSIBLE:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Some objects could not be deleted: They are still referenced/used." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MORE_UNDO:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "No more actions to be un-done." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_UNDO_NOT_POSSIBLE:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "No more actions can be un-done: undo list at limit." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MORE_REDO:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "No more actions to be re-done." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_NO_INPUT_DATA:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "No input data." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_P_INVALID_INPUT_DATA:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_STREAM_POS );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Invalid input data at position " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_N_FILE_EXPORT_FAILED:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_NAME );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Export failed, destination path: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_N_NAME_NOT_UNIQUE:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_NAME );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Name already in use (use copy and paste to insert the existing object): " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_L_EXPORT_FINISHED:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_LIST_OF_NAMES );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Files exported, format: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_SET_PARTLY_UNSUITABLE:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Operation cannot be performed on all elements in the set." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_DB_FILE_WRITE_ERROR:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Database could not be written to the database file." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_ANCESTOR_IS_NOT_DESCENDANT:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str,
                                          "An ancestor (parent/root) diagram cannot move to a descendant (child) location.\n"
                                          "Instead, try to move a descendant out to an ancestor or sibling location"
                                        );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_DEBUG_MODE:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str,
                                          "This software was compiled in DEBUG mode.\nIt may be slower than the RELEASE version.\n"
                                          "Confidential information may be printed to syslog."
                                        );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_NO_RELATIONSHIPS:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "The current diagram type does not allow to create relationships." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_NO_FEATURES:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "The current diagram type does not allow to create features." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_FEATURELESS_CLASSIFIER:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "The current classifier type does not allow to create features." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_0_IS_ALWAYS_INSTANCE:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID );
                utf8stringbuf_append_str( (*this_).private_temp_str, "The current classifier type does not allow to remove the instance flag." );
            }
            break;

            /*
            case GUI_SIMPLE_MESSAGE_CONTENT_S_CUT_TO_CLIPBOARD_STATS:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_ELEMENT_STATS );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Selection cut to clipboard: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;
            */

            /*
            case GUI_SIMPLE_MESSAGE_CONTENT_S_COPY_TO_CLIPBOARD_STATS:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_ELEMENT_STATS );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Selection copied to clipboard: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;
            */

            case GUI_SIMPLE_MESSAGE_CONTENT_S_PASTE_FROM_CLIPBOARD_STATS:
            {
                assert( (*this_).param1_nature == GUI_SIMPLE_MESSAGE_PARAM_NATURE_ELEMENT_STATS );
                utf8stringbuf_append_str( (*this_).private_temp_str, "Clipboard pasted: " );
                utf8stringbuf_append_str( (*this_).private_temp_str, string_param );
            }
            break;

            default:
            {
                TSLOG_ERROR("unexptected gui_simple_message_content_t");
            }
        }
        gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).private_temp_str ));

        gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
        gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

        TSLOG_EVENT( utf8stringbuf_get_string( (*this_).private_temp_str ) );
    }

    TRACE_END();
}

void gui_simple_message_to_user_hide ( gui_simple_message_to_user_t *this_ )
{
    TRACE_BEGIN();

    (*this_).type_id = GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE;
    (*this_).content_id = GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MESSAGE;
    (*this_).param1_nature = GUI_SIMPLE_MESSAGE_PARAM_NATURE_VOID;
    gtk_widget_hide( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_hide( GTK_WIDGET ( (*this_).icon_image ) );

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
