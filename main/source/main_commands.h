/* File: main_commands.h; Copyright and License: see below */

#ifndef MAIN_COMMANDS_H
#define MAIN_COMMANDS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Declares the main commands to be executed by this program
 */

#include "io_file_format.h"
#include "io_import_mode.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "universal_utf8_writer.h"
#include "u8/u8_error.h"
#include <stdbool.h>

/*!
 *  \brief attributes of the main_commands object
 */
struct main_commands_struct {
    data_database_t temp_database;  /*!< a database struct, is only temporarily initialized (and then destroyed again) */
    ctrl_controller_t temp_controller;  /*!< a controller struct, is only temporarily initialized (and then destroyed again) */
};

typedef struct main_commands_struct main_commands_t;

/*!
 *  \brief initializes the main_commands_t struct
 *
 *  \param start_gui true if a graphical window shall be started by main_commands
 *  \param argc number of command line arguments
 *  \param argv array of command line arguments
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t main_commands_init ( main_commands_t *this_, bool start_gui, int argc, char *argv[] );

/*!
 *  \brief destroys the main_commands_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void main_commands_destroy ( main_commands_t *this_ );

/*!
 *  \brief upgrades the database to the latest format
 *
 *  \param this_ pointer to own object attributes
 *  \param database_path pathname of the database
 *  \param out_english_report universal_utf8_writer_t where to write a non-translated report to
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t main_commands_upgrade ( main_commands_t *this_, const char *database_path, universal_utf8_writer_t *out_english_report );

/*!
 *  \brief repairs or checks the database
 *
 *  \param this_ pointer to own object attributes
 *  \param database_path pathname of the database
 *  \param check_only true if the database shall not be modified
 *  \param out_english_report universal_utf8_writer_t where to write a non-translated report to
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t main_commands_repair ( main_commands_t *this_,
                           const char *database_path,
                           bool check_only,
                           universal_utf8_writer_t *out_english_report
                         );

/*!
 *  \brief starts the graphical user interface
 *
 *  \param this_ pointer to own object attributes
 *  \param database_path pathname of the database, may be NULL if no preselected database file
 *  \param out_english_report universal_utf8_writer_t where to write a non-translated report to
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t main_commands_start_gui ( main_commands_t *this_, const char *database_path, universal_utf8_writer_t *out_english_report );

/*!
 *  \brief exports the database in the selected data format to the export_directory
 *
 *  \param this_ pointer to own object attributes
 *  \param database_path pathname of the database
 *  \param export_format format to export
 *  \param export_directory pathname of the directory where to write exported files to
 *  \param out_english_report universal_utf8_writer_t where to write a non-translated report to
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t main_commands_export ( main_commands_t *this_,
                                  const char *database_path,
                                  io_file_format_t export_format,
                                  const char *export_directory,
                                  universal_utf8_writer_t *out_english_report
                               );

/*!
 *  \brief imports the database in the selected data format to the export_directory
 *
 *  \param this_ pointer to own object attributes
 *  \param database_path pathname of the database
 *  \param import_mode import mode, e.g. check-only or update-overwrite
 *  \param import_file_path pathname of the file which to import
 *  \param out_english_report universal_utf8_writer_t where to write a non-translated report to
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t main_commands_import ( main_commands_t *this_,
                                  const char *database_path,
                                  io_import_mode_t import_mode,
                                  const char *import_file_path,
                                  universal_utf8_writer_t *out_english_report
                                );

#endif  /* MAIN_COMMANDS_H */


/*
Copyright 2021-2021 Andreas Warnke

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
