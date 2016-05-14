/* File: gui_simple_message_to_user.h; Copyright and License: see below */

#ifndef GUI_SIMPLE_MESSAGE_TO_USER_H
#define GUI_SIMPLE_MESSAGE_TO_USER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Shows an icon and a message to the user
 */

#include "gui_simple_message_type.h"
#include "gui_simple_message_content.h"
#include "gui_resources.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief constants for max string sizes
 */
enum gui_simple_message_to_user_max_enum {
    GUI_SIMPLE_MESSAGE_TO_USER_MAX_CONTENT_SIZE = 128,
};

/*!
 *  \brief attributes of the simple_message_to_user widget-set
 */
struct gui_simple_message_to_user_struct {
    GtkWidget *text_label;  /*!< pointer to external GtkWidget */
    GtkWidget *icon_image;  /*!< pointer to external GtkWidget */
    gui_resources_t *res;  /*!< pointer to external gui_resources_t */
    utf8stringbuf_t content;
    char private_content_buffer[GUI_SIMPLE_MESSAGE_TO_USER_MAX_CONTENT_SIZE];
};

typedef struct gui_simple_message_to_user_struct gui_simple_message_to_user_t;

/*!
 *  \brief initializes the gui_simple_message_to_user_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_simple_message_to_user_init ( gui_simple_message_to_user_t *this_, GtkWidget *text_label, GtkWidget *icon_image, gui_resources_t *res );

/*!
 *  \brief destroys the gui_simple_message_to_user_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_simple_message_to_user_destroy ( gui_simple_message_to_user_t *this_ );

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 */
void gui_simple_message_to_user_show_message_with_string ( gui_simple_message_to_user_t *this_,
                                                           gui_simple_message_type_t type_id,
                                                           gui_simple_message_content_t content_id,
                                                           const char *var_string
                                                         );

/*!
 *  \brief hides whatever is currently shown
 *
 *  \param this_ pointer to own object attributes
 */
void gui_simple_message_to_user_hide ( gui_simple_message_to_user_t *this_ );

#endif  /* GUI_SIMPLE_MESSAGE_TO_USER_H */


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
