/* File: gui_clipboard.h; Copyright and License: see below */

#ifndef GUI_CLIPBOARD_H
#define GUI_CLIPBOARD_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Serializes and deserializes a set of objects to/from the clipboard
 */

#include "gui_marked_set.h"
#include "gui_simple_message_to_user.h"
#include "json/json_export_from_database.h"
#include "json/json_import_to_database.h"
#include "ctrl_controller.h"
#include "data_rules.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the serdes object
 */
struct gui_clipboard_struct {
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */

    json_export_from_database_t exporter;  /*!< own instance of a json exporter */
    json_import_to_database_t importer;  /*!< own instance of a json importer */

    int64_t destination_diagram_id;  /*!< id of the diagram to which the deserialized objects shal  be added */
    GtkClipboard *the_clipboard;  /*!< pointer to external GtkClipboard */
    utf8stringbuf_t clipboard_stringbuf;  /*!< stringbuffer to read and write to/from the clipboard */
    char private_clipboard_buffer[128*1024];  /*!< stringbuffer to read and write to/from the clipboard */
};

typedef struct gui_clipboard_struct gui_clipboard_t;

/*!
 *  \brief initializes the gui_clipboard_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param clipboard pointer to the main/primary GtkClipboard
 *  \param message_to_user pointer to an object that can show a message to the user
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 */
void gui_clipboard_init ( gui_clipboard_t *this_,
                          GtkClipboard *clipboard,
                          gui_simple_message_to_user_t *message_to_user,
                          data_database_reader_t *db_reader,
                          ctrl_controller_t *controller
                        );

/*!
 *  \brief destroys the gui_clipboard_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_clipboard_destroy ( gui_clipboard_t *this_ );

/*!
 *  \brief gets the clipboard contents and calls
 *
 *  \param this_ pointer to own object attributes
 *  \param destination_diagram_id diagram to which the deserialized objects shall be added
 */
void gui_clipboard_request_clipboard_text( gui_clipboard_t *this_, int64_t destination_diagram_id );

/*!
 *  \brief callback that informs that the text from the clipboard is now available
 *
 *  \param clipboard pointer to GtkClipboard
 *  \param this_ pointer to the clipboard text or NULL if there is no text
 *  \param data this_ pointer to own object attributes
 */
void gui_clipboard_clipboard_text_received_callback ( GtkClipboard *clipboard, const gchar *text, gpointer data );

/*!
 *  \brief copies a set of objects to the clipboard
 *
 *  \param this_ pointer to own object attributes
 *  \param set_to_be_copied ids of the objects to be exported
 */
void gui_clipboard_copy_set_to_clipboard( gui_clipboard_t *this_, data_small_set_t *set_to_be_copied );

/*!
 *  \brief copies the clipboard contents to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param json_text null-terminated string in json format, not NULL
 */
void gui_clipboard_private_copy_clipboard_to_db( gui_clipboard_t *this_, const char *json_text );

#endif  /* GUI_CLIPBOARD_H */


/*
Copyright 2016-2019 Andreas Warnke

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
