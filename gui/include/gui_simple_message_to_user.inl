/* File: gui_simple_message_to_user.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline gui_simple_message_type_t gui_simple_message_to_user_get_type_id( gui_simple_message_to_user_t *this_ )
{
    return (*this_).type_id;
}

static inline void gui_simple_message_to_user_private_set_icon_image ( gui_simple_message_to_user_t *this_, gui_simple_message_type_t type_id )
{
    switch ( type_id )
    {
        case GUI_SIMPLE_MESSAGE_TYPE_NO_MESSAGE:
        {
            assert(false);  /* should not happen */
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
            U8_LOG_ERROR("unexptected gui_simple_message_type_t");
            assert(false);  /* should not happen */
        }
    }
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
