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
#include "set/data_stat.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_error_info.h"
#include <gtk/gtk.h>
#include <stdbool.h>

/*!
 *  \brief constants for max string sizes
 */
enum gui_simple_message_to_user_max_enum {
    GUI_SIMPLE_MESSAGE_TO_USER_MAX_CONTENT_SIZE = 512,
};

/*!
 *  \brief attributes of the simple_message_to_user widget-set
 */
struct gui_simple_message_to_user_struct {
    gui_simple_message_type_t type_id;  /*!< current visible type */
    GtkWidget *text_label;  /*!< pointer to external GtkWidget */
    GtkWidget *icon_image;  /*!< pointer to external GtkWidget */
    gui_resources_t *res;  /*!< pointer to external gui_resources_t */
    utf8stringbuf_t private_temp_str;
    char private_temp_buf[GUI_SIMPLE_MESSAGE_TO_USER_MAX_CONTENT_SIZE];

#if ( GTK_MAJOR_VERSION >= 4 )
    GdkTexture* icon_info;
    GdkTexture* icon_warning;
    GdkTexture* icon_error;
    GdkTexture* icon_about;
#endif
};

typedef struct gui_simple_message_to_user_struct gui_simple_message_to_user_t;

/*!
 *  \brief initializes the gui_simple_message_to_user_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param text_label pointer to GTK text label widget
 *  \param icon_image pointer to GTK icon widget
 *  \param res pointer to a resource provider
 */
void gui_simple_message_to_user_init ( gui_simple_message_to_user_t *this_,
                                       GtkWidget *text_label,
                                       GtkWidget *icon_image,
                                       gui_resources_t *res
                                     );

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
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 */
void gui_simple_message_to_user_show_message ( gui_simple_message_to_user_t *this_,
                                               gui_simple_message_type_t type_id,
                                               gui_simple_message_content_t content_id
                                             );

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 *  \param quantity a quantity parameter that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_quantity ( gui_simple_message_to_user_t *this_,
                                                             gui_simple_message_type_t type_id,
                                                             const gui_simple_message_content_quantity_t *content_id,
                                                             int quantity
                                                           );

#if 0
/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 *  \param stream_line a line number parameter that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_line ( gui_simple_message_to_user_t *this_,
                                                         gui_simple_message_type_t type_id,
                                                         const gui_simple_message_content_position_t *content_id,
                                                         int stream_line
                                                       );
#endif

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 *  \param name a name parameter that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_name ( gui_simple_message_to_user_t *this_,
                                                         gui_simple_message_type_t type_id,
                                                         const gui_simple_message_content_name_t *content_id,
                                                         const char *name
                                                       );

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 *  \param list_of_names a list_of_names parameter that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_names ( gui_simple_message_to_user_t *this_,
                                                          gui_simple_message_type_t type_id,
                                                          const gui_simple_message_content_names_t *content_id,
                                                          const char *list_of_names
                                                        );

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 *  \param error_message a error_message parameter that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_error ( gui_simple_message_to_user_t *this_,
                                                          gui_simple_message_type_t type_id,
                                                          const gui_simple_message_content_error_t *content_id,
                                                          const char *error_message
                                                        );

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and could be translated within this method.
 *  \param stat statistics on performed actions that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_stat ( gui_simple_message_to_user_t *this_,
                                                         gui_simple_message_type_t type_id,
                                                         const gui_simple_message_content_stat_t *content_id,
                                                         const data_stat_t *stat
                                                       );

/*!
 *  \brief shows a message
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of message to be shown
 *  \param content_id id of the message to be shown. The id is language-independant and is translated within this method.
 *  \param list_of_names a language-independant list of names that is printed with the content-message-string.
 *  \param stat statistics on performed actions that is printed with the content-message-string.
 */
void gui_simple_message_to_user_show_message_with_names_and_stat( gui_simple_message_to_user_t *this_,
                                                                  gui_simple_message_type_t type_id,
                                                                  const gui_simple_message_content_names_stat_t *content_id,
                                                                  const char *list_of_names,
                                                                  const data_stat_t *stat
                                                                );

/*!
 *  \brief shows an u8_error_info_t
 *
 *  \param this_ pointer to own object attributes
 *  \param err_info the u8_error_info_t to be shown
 */
void gui_simple_message_to_user_show_error_info ( gui_simple_message_to_user_t *this_, const u8_error_info_t *err_info );

/*!
 *  \brief hides whatever is currently shown
 *
 *  \param this_ pointer to own object attributes
 */
void gui_simple_message_to_user_hide ( gui_simple_message_to_user_t *this_ );

/*!
 *  \brief gets the type of the currently visible message type id
 *
 *  \param this_ pointer to own object attributes
 *  \return message type
 */
static inline gui_simple_message_type_t gui_simple_message_to_user_get_type_id( gui_simple_message_to_user_t *this_ );

/*!
 *  \brief updates the icon_image according to the message type id
 *
 *  \param this_ pointer to own object attributes
 *  \param type_id type of icon_image to be shown
 */
static inline void gui_simple_message_to_user_private_set_icon_image ( gui_simple_message_to_user_t *this_, gui_simple_message_type_t type_id );

/*!
 *  \brief appends statistics to a string buffer
 *
 *  \param this_ pointer to own object attributes
 *  \param stat statistics on performed actions that is printed with the content-message-string.
 *  \param alt_labels use alternative labels for export.
 *  \param out_buf a stringbuffer which is already initialized, output is appended.
 */
void gui_simple_message_to_user_private_append_stat ( gui_simple_message_to_user_t *this_,
                                                      const data_stat_t *stat,
                                                      bool alt_labels,
                                                      utf8stringbuf_t out_buf
                                                    );

#include "gui_simple_message_to_user.inl"

#endif  /* GUI_SIMPLE_MESSAGE_TO_USER_H */


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
