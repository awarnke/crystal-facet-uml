/* File: io_export_set_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_SET_TRAVERSAL_H
#define IO_EXPORT_SET_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Serializes a set of objects to the clipboard
 */

#include "io_element_writer.h"
#include "data_feature.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "util/string/utf8stringbuf.h"

/*!
 *  \brief constants for maximum values of io_export_set_traversal_t
 */
enum io_export_set_traversal_max_enum {
    IO_EXPORT_SET_TRAVERSAL_MAX_FEATURES = 64,  /*!< maximum number of features per classifier */
};

/*!
 *  \brief attributes of the export traversal object
 *
 *  Lifecycle: A database traversal shall perform a single export operation only.
 *  It may be initialized before one export operation and be destroyed afterwards.
 */
struct io_export_set_traversal_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the output sink */

    data_feature_t temp_features[IO_EXPORT_SET_TRAVERSAL_MAX_FEATURES];  /*!< temporary memory for feature list */
};

typedef struct io_export_set_traversal_struct io_export_set_traversal_t;

/*!
 *  \brief initializes the io_export_set_traversal_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 *  \param io_export_stat pointer to statistics object where export statistics are collected.
 *                        Errors and warnings during traversal are counted. Success shall be counted by io_element_writer_t.
 *                        Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_element_writer pointer to an external io_element_writer_t which is the output sink
 */
void io_export_set_traversal_init ( io_export_set_traversal_t *this_,
                                    data_database_reader_t *db_reader,
                                    data_stat_t *io_export_stat,
                                    io_element_writer_t *out_element_writer
                                  );

/*!
 *  \brief destroys the io_export_set_traversal_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_set_traversal_destroy ( io_export_set_traversal_t *this_ );

/*!
 *  \brief copies a set of objects to a string buffer, can be forwarded e.g. to the clipboard
 *
 *  features in the set are ignored. Features of classifiers in the set are exported.
 *
 *  \param this_ pointer to own object attributes
 *  \param set_to_be_exported ids of the objects to be exported
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int io_export_set_traversal_export_set( io_export_set_traversal_t *this_,
                                        const data_small_set_t *set_to_be_exported
                                      );

/*!
 *  \brief fetches required data from the db_reader and exports the object(s) via element_writer
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the object to be exported
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int io_export_set_traversal_private_export_diagram( io_export_set_traversal_t *this_, data_id_t id );

/*!
 *  \brief fetches required data from the db_reader and exports the object(s) via element_writer
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the object to be exported
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int io_export_set_traversal_private_export_diagramelement( io_export_set_traversal_t *this_, data_id_t id );

/*!
 *  \brief fetches required data from the db_reader and exports the object(s) via element_writer
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the object to be exported
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int io_export_set_traversal_private_export_classifier( io_export_set_traversal_t *this_, data_id_t id );

/*!
 *  \brief fetches required data from the db_reader and exports the object(s) via element_writer
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the object to be exported
 *  \return 0 in case of success, -1 if output buffer exceeded
 */
int io_export_set_traversal_private_export_relationship( io_export_set_traversal_t *this_, data_id_t id );

#endif  /* IO_EXPORT_SET_TRAVERSAL_H */


/*
Copyright 2016-2021 Andreas Warnke

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
