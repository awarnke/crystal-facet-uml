/* File: io_exporter_light.h; Copyright and License: see below */

#ifndef IO_EXPORTER_LIGHT_H
#define IO_EXPORTER_LIGHT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Serializes a set of objects to a stringbuffer.
 *
 *  Only sets can be exported, only in json format, only to memory buffers.
 */

#include "io_export_set_traversal.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_error.h"

/*!
 *  \brief attributes of the io_exporter_light_t object
 *
 *  This is a light version in the sense of memory consumption,
 *  optimized for exporting data sets to the clipboard only.
 *
 *  Lifecycle: An exporter may perform multiple export operations.
 *  It may be initialized at program start and live till program exit.
 */
struct io_exporter_light_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */

    io_export_set_traversal_t temp_set_traversal;  /*!< temporary memory for a traversal of a small set */
};

typedef struct io_exporter_light_struct io_exporter_light_t;

/*!
 *  \brief initializes the io_exporter_light_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 */
static inline void io_exporter_light_init ( io_exporter_light_t *this_, data_database_reader_t *db_reader );

/*!
 *  \brief destroys the io_exporter_light_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_exporter_light_destroy ( io_exporter_light_t *this_ );

/*!
 *  \brief copies a set of objects to a string buffer, can be forwarded e.g. to the clipboard
 *
 *  \param this_ pointer to own object attributes
 *  \param set_to_be_exported ids of the objects to be exported
 *  \param io_export_stat pointer to statistics object where export statistics are collected.
 *                        Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_buf string buffer to which to write the json formatted data to
 *  \return U8_ERROR_NONE in case of success, -1 if output buffer exceeded
 */
static inline u8_error_t io_exporter_light_export_set_to_buf( io_exporter_light_t *this_,
                                                              const data_small_set_t *set_to_be_exported,
                                                              data_stat_t *io_export_stat,
                                                              utf8stringbuf_t out_buf
                                                            );

#include "io_exporter_light.inl"

#endif  /* IO_EXPORTER_LIGHT_H */


/*
Copyright 2021-2023 Andreas Warnke

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
