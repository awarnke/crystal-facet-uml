/* File: gui_window_manager.h; Copyright and License: see below */

#ifndef GUI_WINDOW_MANAGER_H
#define GUI_WINDOW_MANAGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Initializes main windows, quits the program when last window closed, shows the about window
 */

#include "gui_main_window.h"
#include "gui_simple_message_to_user.h"
#include "gui_resources.h"
#include "util/observer/observer.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "storage/data_database_reader.h"
#include <gtk/gtk.h>
#include <stdbool.h>

/*!
 *  \brief constants of gui_sketch_area_t
 */
enum gui_window_manager_max_enum {
    GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS = 3,  /*!< maximum number of simultaneoulsy open main windows */
};

/*!
 *  \brief attributes of the window manager
 */
struct gui_window_manager_struct {
    ctrl_controller_t *controller;  /*!< pointer to external ctrl_controller_t */
    data_database_t *database;  /*!< pointer to external data_database_t */
    data_database_reader_t db_reader;  /*!< own instance of data_database_reader_t */
    gui_resources_t gui_resources;  /*!< own instance of gui_resources_t */
    GtkApplication *gtk_app;  /*!< the gtk application object */
    observer_t window_close_observer;  /*!< own instance of observer_t to observe window close events */
    observer_t window_open_observer;  /*!< own instance of observer_t to observe window open events */

    gui_main_window_t main_window[GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS];
    bool main_window_active[GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS];  /*!< window data is big - we do not move or copy this. we just keep track of which window is active and which is unused. */
};

typedef struct gui_window_manager_struct gui_window_manager_t;

/*!
 *  \brief starts window manager
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param database pointer to a database object
 *  \param gtk_app the gtk application object
 */
void gui_window_manager_init( gui_window_manager_t *this_,
                              ctrl_controller_t *controller,
                              data_database_t *database,
                              GtkApplication *gtk_app
                            );

/*!
 *  \brief destroys the window manager
 *
 *  \param this_ pointer to own object attributes
 */
void gui_window_manager_destroy( gui_window_manager_t *this_ );

/*!
 *  \brief starts a main window
 *
 *  \param this_ pointer to own object attributes
 *  \return address of new main window
 */
gui_main_window_t *gui_window_manager_open_main_window( gui_window_manager_t *this_ );

/*!
 *  \brief starts a main window, provides the function signature required by observer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param message_to_user provide a way to send a message to the user
 */
void gui_window_manager_open_main_window2( gui_window_manager_t *this_, gui_simple_message_to_user_t *message_to_user );

/*!
 *  \brief stops a main window
 *
 *  \param this_ pointer to own object attributes
 *  \param main_window address of main window to be closed
 */
void gui_window_manager_close_main_window( gui_window_manager_t *this_, gui_main_window_t *main_window );

#endif  /* GUI_WINDOW_MANAGER_H */


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
