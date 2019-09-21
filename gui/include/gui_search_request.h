/* File: gui_search_request.h; Copyright and License: see below */

#ifndef GUI_SEARCH_REQUEST_H
#define GUI_SEARCH_REQUEST_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Shows and hides a label, a query-entry and a search-button
 */

#include <gtk/gtk.h>

/*!
 *  \brief attributes of the simple_message_to_user widget-set
 */
struct gui_search_request_struct {
    GtkWidget *search_label;  /*!< pointer to external GtkWidget */
    GtkWidget *search_entry;  /*!< pointer to external GtkWidget */
    GtkWidget *search_button;  /*!< pointer to external GtkWidget */
};

typedef struct gui_search_request_struct gui_search_request_t;

/*!
 *  \brief initializes the gui_search_request_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param search_label pointer to GTK label widget
 *  \param search_entry pointer to GTK text entry widget
 *  \param search_button pointer to GTK search button widget
 */
void gui_search_request_init ( gui_search_request_t *this_, GtkWidget *search_label, GtkWidget *search_entry, GtkWidget *search_button );

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

#endif  /* GUI_SEARCH_REQUEST_H */


/*
Copyright 2019-2019 Andreas Warnke

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
