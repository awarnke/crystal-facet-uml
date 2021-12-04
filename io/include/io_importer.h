/* File: io_importer.h; Copyright and License: see below */

#ifndef IO_IMPORTER_H
#define IO_IMPORTER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Imports a set of objects from a file
 */

#include "json/json_importer.h"
#include "io_import_elements.h"
#include "io_file_format.h"
#include "ctrl_controller.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "universal_utf8_writer.h"

/*!
 *  \brief attributes of the import object
 *
 *  Lifecycle: An importer may perform multiple import operations.
 *  It may be initialized at program start and live till program exit.
 */
struct io_importer_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */

    json_importer_t temp_json_importer;  /*!< own instance of a json stream importer */
    io_import_elements_t temp_elements_importer;  /*!< own instance of a db-element sync to database */
};

typedef struct io_importer_struct io_importer_t;

/*!
 *  \brief initializes the io_importer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 */
void io_importer_init ( io_importer_t *this_,
                        data_database_reader_t *db_reader,
                        ctrl_controller_t *controller
                      );

/*!
 *  \brief destroys the io_importer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void io_importer_destroy ( io_importer_t *this_ );

/*!
 *  \brief copies the clipboard contents to the focused diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param json_text 0-terminated string in json format, not NULL
 *  \param diagram_id id of the diagram to which to attach the imported data
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_CREATED,
 *                 DATA_STAT_SERIES_MODIFIED (e.g. in future) and
 *                 DATA_STAT_SERIES_IGNORED (e.g. at import of lifelines
 *                 or if classifier and its features already exist) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if a relation has no source
 *                 or no destination)
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_read_line read position in the stream, in case of an error, this may help finding the cause
 *  \return DATA_ERROR_NONE in case of success, DATA_ERROR_DB_STRUCTURE if diagram_id does not exist, other error code otherwise
 */
data_error_t io_importer_import_memory_buffer( io_importer_t *this_,
                                               const char *json_text,
                                               data_row_id_t diagram_id,
                                               data_stat_t *io_stat,
                                               uint32_t *out_read_line
                                             );


/*!
 *  \brief imports the file contents to the database
 *
 *  \param this_ pointer to own object attributes
 *  \param import_format file format, currently only IO_FILE_FORMAT_JSON is supported
 *  \param import_file_path null-terminated file path, not NULL
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_CREATED,
 *                 DATA_STAT_SERIES_MODIFIED (e.g. in future) and
 *                 DATA_STAT_SERIES_IGNORED (e.g. at import of lifelines
 *                 or if classifier and its features already exist) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if a relation has no source
 *                 or no destination)
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_english_report universal_utf8_writer_t where to write a non-translated report to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_INVALID_REQUEST if file cannot be opened,
 *          DATA_ERROR_AT_FILE_READ in case of reading errors after open,
 *          other error code otherwise
 */
data_error_t io_importer_import_file( io_importer_t *this_,
                                      io_file_format_t import_format,
                                      const char *import_file_path,
                                      data_stat_t *io_stat,
                                      universal_utf8_writer_t *out_english_report
                                    );


#endif  /* IO_IMPORTER_H */


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
