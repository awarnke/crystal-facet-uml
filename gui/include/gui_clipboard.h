/* File: gui_clipboard.h; Copyright and License: see below */

#ifndef GUI_CLIPBOARD_H
#define GUI_CLIPBOARD_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Serializes and deserializes a set of objects to/from the clipboard
 */

#include "gui_marked_set.h"
#include "gui_simple_message_to_user.h"
#include "io_exporter_light.h"
#include "io_importer.h"
#include "ctrl_controller.h"
#include "data_rules.h"
#include "utf8stringbuf/utf8stringbuf.h"
#if ( GTK_MAJOR_VERSION >= 4 )
#include <gdk/gdkclipboard.h>
#else
#include <gtk/gtk.h>
typedef GtkClipboard GdkClipboard;
#endif

/*!
 *  \brief attributes of the serdes object
 */
struct gui_clipboard_struct {
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */

    io_exporter_light_t exporter;  /*!< own instance of a json exporter */
    io_importer_t importer;  /*!< own instance of a json importer */

    data_row_id_t destination_diagram_id;  /*!< id of the diagram to which the deserialized objects shal  be added */
    GdkClipboard *the_clipboard;  /*!< pointer to external GdkClipboard/GtkClipboard */
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
                          GdkClipboard *clipboard,
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
 *  \brief requests the clipboard contents.
 *
 *  When the clipboard contents is available, gui_clipboard_clipboard_text_received_callback
 *  is called automatically.
 *
 *  \param this_ pointer to own object attributes
 *  \param destination_diagram_id diagram to which the deserialized objects shall be added
 */
void gui_clipboard_request_clipboard_text( gui_clipboard_t *this_, data_row_id_t destination_diagram_id );

#if ( GTK_MAJOR_VERSION >= 4 )
/*!
 *  \brief callback that informs that the text from the clipboard is now available.
 *
 *  Thi function implicitly assumes that the clipboard text shall be imported and calls
 *  gui_clipboard_private_copy_clipboard_to_db.
 *
 *  \param source_object pointer to the source GObject
 *  \param res pointer to the clipboard text
 *  \param user_data this_ pointer to own object attributes
 */
void gui_clipboard_clipboard_text_received_callback( GObject *source_object,
                                                     GAsyncResult* res,
                                                     gpointer user_data
                                                   );
#else
/*!
 *  \brief callback that informs that the text from the clipboard is now available.
 *
 *  Thi function implicitly assumes that the clipboard text shall be imported and calls
 *  gui_clipboard_private_copy_clipboard_to_db.
 *
 *  \param clipboard pointer to GtkClipboard
 *  \param text pointer to the clipboard text or NULL if there is no text
 *  \param data this_ pointer to own object attributes
 */
void gui_clipboard_clipboard_text_received_callback( GdkClipboard *clipboard,
                                                     const gchar *text,
                                                     gpointer data
                                                   );
#endif

/*!
 *  \brief copies a set of objects to the clipboard
 *
 *  \param this_ pointer to own object attributes
 *  \param set_to_be_copied ids of the objects to be exported
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on copied data.
 *                 Statistics are only added; *io_stat shall be initialized by caller.
 *  \return 0 in case of success, -1 if set_to_be_copied cannot be serialized
 */
int gui_clipboard_copy_set_to_clipboard( gui_clipboard_t *this_,
                                         const data_small_set_t *set_to_be_copied,
                                         data_stat_t *io_stat
                                       );

/*!
 *  \brief copies the clipboard contents to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param json_text null-terminated string in json format, not NULL
 */
void gui_clipboard_private_copy_clipboard_to_db( gui_clipboard_t *this_, const char *json_text );

#endif  /* GUI_CLIPBOARD_H */


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
