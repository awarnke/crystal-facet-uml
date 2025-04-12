/* File: gui_file_use_db_dialog.h; Copyright and License: see below */

#ifndef GUI_FILE_USE_DB_DIALOG_H
#define GUI_FILE_USE_DB_DIALOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a use database file dialog and reacts on user events
 *
 *  This file dialog can either create a new data file or open an existing data file.
 */

#include "gui_simple_message_to_user.h"
#include "gui_file_action.h"
#include "io_data_file.h"
#include "gui_gtk.h"

/*!
 *  \brief attributes of the use database file dialog
 */
struct gui_file_use_db_dialog_struct {
    gui_file_action_t *file_action;  /*!<  pointer to instance of gui_file_action_t */

    GtkWindow *parent_window;  /*!< pointer to parent window, needed for modal dialogs */
#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    GtkWidget *new_file_chooser;  /*!< pointer to instance of a file chooser for new */
    GtkWidget *open_file_chooser;  /*!< pointer to instance of a file chooser for open */
#else
    GtkFileDialog *new_file_dialog;  /*!< pointer to instance of a file dialog for new */
    GtkFileDialog *open_file_dialog;  /*!< pointer to instance of a file dialog for open */
#endif
};

typedef struct gui_file_use_db_dialog_struct gui_file_use_db_dialog_t;

/*!
 *  \brief initializes the gui_file_use_db_dialog
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_window pointer to the gtk parent window, to which this modal dialog belongs
 *  \param file_action in case a file was selected for new/open, file_action is the executor of that action
 */
void gui_file_use_db_dialog_init( gui_file_use_db_dialog_t *this_,
                                  GtkWindow *parent_window,
                                  gui_file_action_t *file_action
                                );

/*!
 *  \brief destroys the gui_file_use_db_dialog
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_use_db_dialog_destroy( gui_file_use_db_dialog_t *this_ );

/*!
 *  \brief shows the gui_file_use_db_dialog
 *
 *  \param this_ pointer to own object attributes
 *  \param open_existing true if only existing files shall be selectable
 */
void gui_file_use_db_dialog_show( gui_file_use_db_dialog_t *this_, bool open_existing );

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))

/*!
 *  \brief callback function of a GtkDialog
 */
void gui_file_use_db_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data );

#else  /* GTK >= 4.10 */

/*!
 *  \brief callback function of the 4.10 GtkFileDialog
 */
void gui_file_use_db_dialog_async_ready_callback_on_new( GObject* source_object,
                                                         GAsyncResult* res,
                                                         gpointer user_data
                                                       );

/*!
 *  \brief callback function of the 4.10 GtkFileDialog
 */
void gui_file_use_db_dialog_async_ready_callback_on_open( GObject* source_object,
                                                          GAsyncResult* res,
                                                          gpointer user_data
                                                        );

#endif  /* GTK < 4.10 */

#endif  /* GUI_FILE_USE_DB_DIALOG_H */


/*
Copyright 2019-2025 Andreas Warnke

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
