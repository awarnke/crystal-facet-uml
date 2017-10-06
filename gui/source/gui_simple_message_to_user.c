/* File: gui_simple_message_to_user.c; Copyright and License: see below */

#include "gui_simple_message_to_user.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include <assert.h>

void gui_simple_message_to_user_init ( gui_simple_message_to_user_t *this_, GtkWidget *text_label, GtkWidget *icon_image, gui_resources_t *res )
{
    TRACE_BEGIN();
    assert ( text_label != NULL );
    assert ( icon_image != NULL );
    assert ( res != NULL );

    (*this_).text_label = text_label;
    (*this_).icon_image = icon_image;
    (*this_).res = res;

    (*this_).content = utf8stringbuf_init( sizeof((*this_).private_content_buffer), (*this_).private_content_buffer );
    utf8stringbuf_clear( (*this_).content );

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
                                                        int int_param )
{
    TRACE_BEGIN();

    char string_of_int_buf[16];
    utf8stringbuf_t string_of_int = UTF8STRINGBUF( string_of_int_buf );
    utf8stringbuf_clear( string_of_int );
    utf8stringbuf_append_int( string_of_int, int_param );

    gui_simple_message_to_user_show_message_with_string( this_, type_id, content_id, utf8stringbuf_get_string( string_of_int ));

    TRACE_END();
}

void gui_simple_message_to_user_show_message_with_string ( gui_simple_message_to_user_t *this_,
                                                           gui_simple_message_type_t type_id,
                                                           gui_simple_message_content_t content_id,
                                                           const char *string_param )
{
    TRACE_BEGIN();

    if ( GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE == type_id )
    {
        gui_simple_message_to_user_hide( this_ );
    }
    else
    {

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

        utf8stringbuf_clear( (*this_).content );
        switch ( content_id )
        {
            case GUI_SIMPLE_MESSAGE_CONTENT_ABOUT:
            {
                utf8stringbuf_append_str( (*this_).content,
                                          "This is " META_INFO_PROGRAM_NAME_STR " version " META_INFO_VERSION_STR "\n"
                                          "License: Apache 2.0\n"
                                          "Author+Copyright: 2016-2017 Andreas Warnke" );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED:
            {
                utf8stringbuf_append_str( (*this_).content, "Chosen database file could not be opened/created:\n" );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR:
            {
                utf8stringbuf_append_str( (*this_).content, "Chosen database file opened/created with warning:\n" );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN:
            {
                utf8stringbuf_append_str( (*this_).content, "Maximum number of windows already open: " );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED:
            {
                utf8stringbuf_append_str( (*this_).content, "Maximum string length exceeded." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED:
            {
                utf8stringbuf_append_str( (*this_).content, "This feature is not yet implemented: " );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION:
            {
                utf8stringbuf_append_str( (*this_).content, "Nothing selected." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE:
            {
                utf8stringbuf_append_str( (*this_).content, "Some objects could not be deleted: They are still referenced/used." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_UNDO:
            {
                utf8stringbuf_append_str( (*this_).content, "No more actions to be un-done." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_UNDO_NOT_POSSIBLE:
            {
                utf8stringbuf_append_str( (*this_).content, "No more actions can be un-done: undo list at limit." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_REDO:
            {
                utf8stringbuf_append_str( (*this_).content, "No more actions to be re-done." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA:
            {
                utf8stringbuf_append_str( (*this_).content, "No input data." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA:
            {
                utf8stringbuf_append_str( (*this_).content, "Invalid input data at character position " );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED:
            {
                utf8stringbuf_append_str( (*this_).content, "Export failed, type: " );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE:
            {
                utf8stringbuf_append_str( (*this_).content, "Name already in use (use copy and paste to insert the existing object): " );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED:
            {
                utf8stringbuf_append_str( (*this_).content, "Files exported, type: " );
                utf8stringbuf_append_str( (*this_).content, string_param );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_SET_PARTLY_UNSUITABLE:
            {
                utf8stringbuf_append_str( (*this_).content, "Operation cannot be performed on all elements in the set." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_CLICK_IN_CHILDREN_AREA:
            {
                utf8stringbuf_append_str( (*this_).content, "Click in the children area to create a diagram." );
            }
            break;

            case GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_WRITE_ERROR:
            {
                utf8stringbuf_append_str( (*this_).content, "Database could not be written to the database file." );
            }
            break;

            default:
            {
                TSLOG_ERROR("unexptected gui_simple_message_content_t");
            }
        }
        gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).content ));

        gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
        gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );

        TSLOG_EVENT( utf8stringbuf_get_string( (*this_).content ) );
    }

    TRACE_END();
}

void gui_simple_message_to_user_hide ( gui_simple_message_to_user_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_hide( GTK_WIDGET ( (*this_).text_label ) );
    gtk_widget_hide( GTK_WIDGET ( (*this_).icon_image ) );

    TRACE_END();
}


/*
Copyright 2016-2017 Andreas Warnke

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
