/* File: io_importer.h; Copyright and License: see below */

#ifndef IO_IMPORTER_H
#define IO_IMPORTER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Imports a set of objects from a file
 */

#include "ctrl_controller.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"

/*!
 *  \brief attributes of the json import object
 */
struct io_importer_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
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
 *  \param json_text null-terminated string in json format, not NULL
 *  \param diagram_id id of the diagram to which to attach the imported data
 *  \param io_stat undefined in case of an error in the return value,
 *                 otherwise statistics on DATA_STAT_SERIES_CREATED,
 *                 DATA_STAT_SERIES_MODIFIED (e.g. in future) and
 *                 DATA_STAT_SERIES_IGNORED (e.g. at import of lifelines
 *                 or if classifier and its features already exist) and
 *                 DATA_STAT_SERIES_ERROR (e.g. if a relation has no source
 *                 or no destination)
 *                 Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_read_pos read position in the stream, in case of an error, this may help finding the cause
 *  \return DATA_ERROR_NONE in case of success, DATA_ERROR_DB_STRUCTURE if diagram_id does not exist, other error code otherwise
 */
data_error_t io_importer_import_buf_to_db( io_importer_t *this_,
                                           const char *json_text,
                                           data_row_id_t diagram_id,
                                           data_stat_t *io_stat,
                                           uint32_t *out_read_pos
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
