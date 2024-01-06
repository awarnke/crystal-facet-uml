/* File: main.h; Copyright and License: see below */

#ifndef MAIN_H
#define MAIN_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Declares the functions to start and stop this program
 */

#include "io_file_format.h"
#include "io_import_mode.h"

/*!
 *  \brief main starts the gui (or a headless feature)
 */
int main (int argc, char **argv);

/*!
 *  \brief compares the command line options to the available export file formats
 *
 *  \return one or more bits of the io_file_format_t enumeration, 0 if no format could be determined.
 */
io_file_format_t main_private_get_selected_format( char *arg_fmt );

/*!
 *  \brief compares the command line options to the available import modes
 *
 *  \return one or more bits of the io_import_mode_t enumeration, IO_IMPORT_MODE_CHECK if no mode could be determined.
 */
io_import_mode_t main_private_get_selected_mode( char *arg_fmt );

#endif  /* MAIN_H */


/*
Copyright 2019-2024 Andreas Warnke

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
