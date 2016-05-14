/* File: gui_simple_message_to_user.c; Copyright and License: see below */

#include "gui_simple_message_to_user.h"
#include "trace.h"
#include "log.h"
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

void gui_simple_message_to_user_show_message_with_string ( gui_simple_message_to_user_t *this_,
                                                           gui_simple_message_type_t type_id,
                                                           gui_simple_message_content_t content_id,
                                                           const char *var_string )
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
                    LOG_ERROR("unexptected gui_simple_message_type_t");
                }
        }

        utf8stringbuf_clear( (*this_).content );
        switch ( content_id )
        {
            case GUI_SIMPLE_MESSAGE_CONTENT_ABOUT:
                {
                    utf8stringbuf_append_str( (*this_).content, "This is crystal_facet_uml version 0.3\nAuthor+Copyright: 2016-2016 Andreas Warnke" );
                }
                break;
            case GUI_SIMPLE_MESSAGE_CONTENT_NOTHING_SELECTED:
                {
                    utf8stringbuf_append_str( (*this_).content, "Nothing selected." );
                }
                break;
            case GUI_SIMPLE_MESSAGE_CONTENT_NOTHING_FOCUSED:
                {
                    utf8stringbuf_append_str( (*this_).content, "Nothing focused." );
                }
                break;
            default:
                {
                    LOG_ERROR("unexptected gui_simple_message_content_t");
                }
        }
        gtk_label_set_text ( GTK_LABEL( (*this_).text_label ), utf8stringbuf_get_string( (*this_).content ));

        gtk_widget_show( GTK_WIDGET ( (*this_).text_label ) );
        gtk_widget_show( GTK_WIDGET ( (*this_).icon_image ) );
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
