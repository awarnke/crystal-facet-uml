/* File: gui_window_manager.h; Copyright and License: see below */

#ifndef GUI_WINDOW_MANAGER_H
#define GUI_WINDOW_MANAGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Initializes main windows, quits the program when last window closed, shows the about window
 */

#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "storage/data_database_reader.h"
#include "gui_resources.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the window manager
 */
struct gui_window_manager_struct {
    data_database_reader_t db_reader;
    gui_resources_t gui_resources;
};

typedef struct gui_window_manager_struct gui_window_manager_t;

/*!
 *  \brief starts main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_window_manager_init( gui_window_manager_t *this_, ctrl_controller_t *controller, data_database_t *database );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_window_manager_destroy( gui_window_manager_t *this_ );

#endif  /* GUI_WINDOW_MANAGER_H */


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
