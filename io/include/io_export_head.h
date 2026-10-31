/* File: io_export_head.h; Copyright and License: see below */

#ifndef IO_EXPORT_HEAD_H
#define IO_EXPORT_HEAD_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Triggers writing a set of header values.
 *
 *  Only sets can be exported, only in json format, only to memory buffers.
 */

#include "io_export_set_traversal.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_error.h"

/*!
 *  \brief attributes of the io_export_head_t object
 *
 *  This is a light version in the sense of memory consumption,
 *  optimized for exporting data sets to the clipboard only.
 *
 *  Lifecycle: An exporter may perform multiple export operations.
 *  It may be initialized at program start and live till program exit.
 */
struct io_export_head_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the output sink */

    data_head_t temp_head;  /*!< own buffer for private use as data cache */
};

typedef struct io_export_head_struct io_export_head_t;

/*!
 *  \brief initializes the io_export_head_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader
 *  \param out_element_writer pointer to an external io_element_writer_t which is the output sink
 */
static inline void io_export_head_init ( io_export_head_t *this_,
                                         data_database_reader_t *db_reader,
                                         io_element_writer_t *out_element_writer
                                       );

/*!
 *  \brief destroys the io_export_head_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void io_export_head_destroy ( io_export_head_t *this_ );

/*!
 *  \brief copies a set of objects to a string buffer, can be forwarded e.g. to the clipboard
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, -1 if output buffer exceeded
 */
static inline u8_error_t io_export_head_iterate_head_values( io_export_head_t *this_ );

/*!
 *  \brief gets the major version of the json file format
 *
 *  This is needed to detemine the output format when saving the file
 *
 *  \param this_ pointer to own object attributes
 *  \return major version, 0 if unknown
 */
static inline unsigned int io_export_head_get_major_version( io_export_head_t *this_ );

#include "io_export_head.inl"

#endif  /* IO_EXPORT_HEAD_H */


/*
Copyright 2026-2026 Andreas Warnke

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
