/* File: gui_main.h; Copyright and License: see below */

#ifndef GUI_MAIN_H
#define GUI_MAIN_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Starts one gui instance
 */

#include "storage/data_database.h"
#include "ctrl_controller.h"
#include <gtk/gtk.h>

/*!
 *  \brief initializes the gui widgets
 *
 *  \param argc number of command line arguments to this program
 *  \param argv list of command line arguments to this program
 *  \param controller pointer to a controller object which can modify the database
 *  \param database pointer to a database object
 */
void gui_main ( int argc, char *argv[], ctrl_controller_t *controller, data_database_t *database );

#endif  /* GUI_MAIN_H */


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
