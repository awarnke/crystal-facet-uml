/* File: gui_file_export_dialog.h; Copyright and License: see below */

#ifndef GUI_FILE_EXPORT_DIALOG_H
#define GUI_FILE_EXPORT_DIALOG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Displays a file export dialog and reacts on user events
 */

#include "gui_simple_message_to_user.h"
#include "io_exporter.h"
#include "storage/data_database.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file export dialog
 */
struct gui_file_export_dialog_struct {
    data_database_t *database;  /*!< pointer to external data_database_t */

    io_exporter_t file_exporter;  /*!<  own instance of io_exporter_t */

    GtkWindow *parent_window;  /*!< pointer to parent window, needed for modal dialogs */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external gui_simple_message_to_user_t */

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    GtkWidget *export_file_chooser;  /*!< pointer to instance of a file chooser for export */
#else
    GtkFileDialog *export_file_dialog;  /*!< pointer to instance of a file dialog for export */
#endif

    GtkWidget *format_docbook;  /*!< pointer to checkbox for export-format docbook */
    GtkWidget *format_pdf;  /*!< pointer to checkbox for export-format pdf */
    GtkWidget *format_png;  /*!< pointer to checkbox for export-format png */
    GtkWidget *format_ps;  /*!< pointer to checkbox for export-format ps */
    GtkWidget *format_svg;  /*!< pointer to checkbox for export-format svg */
    GtkWidget *format_txt;  /*!< pointer to checkbox for export-format txt */
    GtkWidget *format_html;  /*!< pointer to checkbox for export-format html */
    GtkWidget *format_xmi2;  /*!< pointer to checkbox for export-format xmi version 2.x */
    GtkWidget *format_json;  /*!< pointer to checkbox for export-format json */

    GtkWidget *diagram_set_label;  /*!< the label for filetype-sets */
    GtkWidget *document_label;  /*!< the label for document-types */
    GtkButton *export_button;  /*!< the export button that triggers the directory chooser */
    GtkWidget *options_layout;  /*!< the grid layout container for the export options */
};

typedef struct gui_file_export_dialog_struct gui_file_export_dialog_t;

/*!
 *  \brief initializes the file export dialog
 *
 *  \param this_ pointer to own object attributes
 *  \param database pointer to external data_database_t
 *  \param db_reader pointer to a database reader object
 *  \param parent_window pointer to the gtk parent window, to which this modal dialog belongs
 *  \param message_to_user pointer to the message_to_user object to use
 */
void gui_file_export_dialog_init( gui_file_export_dialog_t *this_,
                                  data_database_t *database,
                                  data_database_reader_t *db_reader,
                                  GtkWindow *parent_window,
                                  gui_simple_message_to_user_t *message_to_user
                                );

/*!
 *  \brief destroys the file export dialog
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_export_dialog_destroy( gui_file_export_dialog_t *this_ );

/*!
 *  \brief returns the file options container showing the type selection.
 *
 *  This container can be displayed in the main window.
 *
 *  \param this_ pointer to own object attributes
 */
GtkWidget *gui_file_export_dialog_get_options( gui_file_export_dialog_t *this_ );

/*!
 *  \brief callback that informs that the export-to-folder button was pressed
 */
void gui_file_export_dialog_export_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief shows the file export dialog
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_export_dialog_show( gui_file_export_dialog_t *this_ );

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))

/*!
 *  \brief callback function of the old GtkDialog
 */
void gui_file_export_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data );

#else  /* GTK >= 4.10 */

/*!
 *  \brief callback function of the 4.10 GtkFileDialog
 */
void gui_file_export_dialog_async_ready_callback( GObject* source_object,
                                                  GAsyncResult* res,
                                                  gpointer user_data
                                                );

#endif  /* GTK ? 4.10 */

#endif  /* GUI_FILE_EXPORT_DIALOG_H */


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
