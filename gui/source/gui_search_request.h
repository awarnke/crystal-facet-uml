/* File: gui_search_request.h; Copyright and License: see below */

#ifndef GUI_SEARCH_REQUEST_H
#define GUI_SEARCH_REQUEST_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Shows and hides a label, a query-entry and a search-button
 */

#include "gui_tool.h"
#include "gui_search_runner.h"
#include "gui_marked_set.h"
#include "entity/data_id.h"
#include "storage/data_change_message.h"
#include "gui_gtk.h"

/*!
 *  \brief attributes of the search request widget-set
 */
struct gui_search_request_struct {
    GtkWidget *search_label;  /*!< pointer to external GtkWidget */
    GtkWidget *search_entry;  /*!< pointer to external GtkWidget */
    GtkWidget *search_button;  /*!< pointer to external GtkWidget */
    gui_marked_set_t *marked_set;  /*!< pointer to external marked_set */
    gui_search_runner_t *search_runner;  /*!< pointer to external search runner */
};

typedef struct gui_search_request_struct gui_search_request_t;

/*!
 *  \brief initializes the gui_search_request_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param search_label pointer to GTK label widget
 *  \param search_entry pointer to GTK text entry widget
 *  \param search_button pointer to GTK search button widget
 *  \param marked_set pointer to marked_set which knows the currently selected item
 *  \param search_runner pointer to search runner
 */
void gui_search_request_init ( gui_search_request_t *this_,
                               GtkWidget *search_label,
                               GtkWidget *search_entry,
                               GtkWidget *search_button,
                               gui_marked_set_t *marked_set,
                               gui_search_runner_t *search_runner
                             );

/*!
 *  \brief destroys the gui_search_request_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_search_request_destroy ( gui_search_request_t *this_ );

/*!
 *  \brief shows the search request fields
 *
 *  \param this_ pointer to own object attributes
 */
void gui_search_request_show ( gui_search_request_t *this_ );

/*!
 *  \brief hides the search request fields
 *
 *  \param this_ pointer to own object attributes
 */
void gui_search_request_hide ( gui_search_request_t *this_ );

/* ================================ TOOL CHANGED OR BUTTON CALLBACKS ================================ */

/*!
 *  \brief callback that informs that the chosen tool changed
 *
 *  \param widget widget that triggered the callback
 *  \param tool the new, selected tool
 *  \param data pointer to own object attributes
 */
void gui_search_request_tool_changed_callback( GtkWidget *widget, gui_tool_t tool, gpointer data );

/*!
 *  \brief callback that informs that the search button was pressed
 *         or that enter was pressed in the text entry widget
 *
 *  \param trigger_widget widget that triggered the callback, either the search button or the search text entry field
 *  \param data pointer to own object attributes
 */
void gui_search_request_search_start_callback( GtkWidget* trigger_widget, gpointer data );

/*!
 *  \brief callback that informs that the id search button (in the attributes editor pane) was triggered
 *  \param widget the search id button that triggered the callback
 *  \param user_data pointer to own object attributes
 */
void gui_search_request_id_search_callback ( GtkWidget *widget, gpointer user_data );

/* ================================ MODEL CHANGED CALLBACKS ================================ */

/*!
 *  \brief callback that informs that the data of an object changed
 *
 *  \param widget proxy-widget that proxies the data module as source of the callback trigger
 *  \param msg pointer to a record explaining what data is changed
 *  \param user_data pointer to own object attributes
 */
void gui_search_request_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data );

#endif  /* GUI_SEARCH_REQUEST_H */


/*
Copyright 2019-2026 Andreas Warnke

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
