/* File: data_database_diagram_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_DIAGRAM_READER_H
#define DATA_DATABASE_DIAGRAM_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Searches and reads diagram-related records from the database
 */

#include "storage/data_database_listener.h"
#include "storage/data_database_listener_signal.h"
#include "storage/data_database_iterator_classifiers.h"
#include "storage/data_database.h"
#include "data_diagram.h"
#include "u8/u8_error.h"
#include "data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_row_id.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_diagram_reader_struct {
    data_database_t *database;  /*!< pointer to external database */

    sqlite3_stmt *statement_diagram_by_id;
    sqlite3_stmt *statement_diagram_by_uuid;
    sqlite3_stmt *statement_diagrams_by_parent_id;
    sqlite3_stmt *statement_diagrams_by_parent_id_null;
    sqlite3_stmt *statement_diagrams_by_classifier_id;
    sqlite3_stmt *statement_diagram_ids_by_parent_id;
    sqlite3_stmt *statement_diagram_ids_by_parent_id_null;
    sqlite3_stmt *statement_diagram_ids_by_classifier_id;
    sqlite3_stmt *statement_diagramelement_by_id;
    sqlite3_stmt *statement_diagramelement_by_uuid;
    sqlite3_stmt *statement_diagramelements_by_diagram_id;
    sqlite3_stmt *statement_diagramelements_by_classifier_id;
};

typedef struct data_database_diagram_reader_struct data_database_diagram_reader_t;

/*!
 *  \brief initializes the data_database_diagram_reader_t struct
 *
 *  This object shall be initialized only after opening the database.
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_diagram_reader_init ( data_database_diagram_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_diagram_reader_t struct
 *
 *  This object shall be destroyed before closing the database.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_diagram_reader_destroy ( data_database_diagram_reader_t *this_ );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagram to be read from the database
 *  \param[out] out_diagram the diagram read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagram_by_id ( data_database_diagram_reader_t *this_,
                                                            data_row_id_t id,
                                                            data_diagram_t *out_diagram
                                                          );

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the diagram to be read from the database
 *  \param[out] out_diagram the diagram read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagram_by_uuid ( data_database_diagram_reader_t *this_,
                                                              const char *uuid,
                                                              data_diagram_t *out_diagram
                                                            );

/*!
 *  \brief reads all child-diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram, DATA_ROW_ID_VOID to get all root diagrams
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagram array of diagrams read from the database (in case of success)
 *  \param[out] out_diagram_count number of diagram records stored in out_diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open,
 *          U8_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
u8_error_t data_database_diagram_reader_get_diagrams_by_parent_id ( data_database_diagram_reader_t *this_,
                                                                    data_row_id_t parent_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_diagram_t (*out_diagram)[],
                                                                    uint32_t *out_diagram_count
                                                                  );

/*!
 *  \brief reads all classifier-displaying diagrams from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagram array of diagrams read from the database (in case of success or exceeded buffer)
 *  \param[out] out_diagram_count number of diagram records stored in out_diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open,
 *          U8_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
u8_error_t data_database_diagram_reader_get_diagrams_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                        data_row_id_t classifier_id,
                                                                        uint32_t max_out_array_size,
                                                                        data_diagram_t (*out_diagram)[],
                                                                        uint32_t *out_diagram_count
                                                                      );

/*!
 *  \brief reads all child-diagram ids from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram, DATA_ROW_ID_VOID to get all root diagram ids
 *  \param[out] out_diagram_ids set of diagram ids read from the database (in case of success). The provided set shall be initialized.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagram_ids_by_parent_id ( data_database_diagram_reader_t *this_,
                                                                       data_row_id_t parent_id,
                                                                       data_small_set_t *out_diagram_ids
                                                                     );

/*!
 *  \brief reads all classifier-displaying diagrams ids from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param[out] out_diagram_ids set of diagram ids read from the database (in case of success). The provided set shall be initialized.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagram_ids_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                           data_row_id_t classifier_id,
                                                                           data_small_set_t *out_diagram_ids
                                                                         );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief reads a diagramelement from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagramelement to be read from the database
 *  \param[out] out_diagramelement the diagramelement read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagramelement_by_id ( data_database_diagram_reader_t *this_,
                                                                   data_row_id_t id,
                                                                   data_diagramelement_t *out_diagramelement
                                                                 );

/*!
 *  \brief reads a diagramelement from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the diagramelement to be read from the database
 *  \param[out] out_diagramelement the diagramelement read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagramelement_by_uuid ( data_database_diagram_reader_t *this_,
                                                                     const char *uuid,
                                                                     data_diagramelement_t *out_diagramelement
                                                                   );

/*!
 *  \brief reads all diagramelements of a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagramelement array of diagramelements read from the database (in case of success)
 *  \param[out] out_diagramelement_count number of diagramelement records stored in out_diagramelement
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open,
 *          U8_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
u8_error_t data_database_diagram_reader_get_diagramelements_by_diagram_id ( data_database_diagram_reader_t *this_,
                                                                            data_row_id_t diagram_id,
                                                                            uint32_t max_out_array_size,
                                                                            data_diagramelement_t (*out_diagramelement)[],
                                                                            uint32_t *out_diagramelement_count
                                                                          );

/*!
 *  \brief reads all diagramelements of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagramelement array of diagramelements read from the database (in case of success)
 *  \param[out] out_diagramelement_count number of diagramelement records stored in out_diagramelement
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open,
 *          U8_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
u8_error_t data_database_diagram_reader_get_diagramelements_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                               data_row_id_t classifier_id,
                                                                               uint32_t max_out_array_size,
                                                                               data_diagramelement_t (*out_diagramelement)[],
                                                                               uint32_t *out_diagramelement_count
                                                                             );

/* ================================ private ================================ */

/*!
 *  \brief initializes the data_database_diagram_reader_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_diagram_reader_private_open ( data_database_diagram_reader_t *this_ );

/*!
 *  \brief closes the data_database_diagram_reader_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_diagram_reader_private_close ( data_database_diagram_reader_t *this_ );

/*!
 *  \brief creates a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_statement statement as string to be prepared
 *  \param string_size size of string_statement in bytes, including the terminating zero
 *  \param[out] out_statement_ptr address of a pointer. The pointer is modifies as to point to a statement object.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_diagram_reader_private_prepare_statement ( data_database_diagram_reader_t *this_,
                                                                                  const char *string_statement,
                                                                                  int string_size,
                                                                                  sqlite3_stmt **out_statement_ptr
                                                                                );

/*!
 *  \brief finalizes a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_diagram_reader_private_finalize_statement ( data_database_diagram_reader_t *this_,
                                                                                   sqlite3_stmt *statement_ptr
                                                                                 );

/*!
 *  \brief resets a statement only, without binding.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_diagram_reader_private_bind_void_to_statement ( data_database_diagram_reader_t *this_,
                                                                                       sqlite3_stmt *statement_ptr
                                                                                     );

/*!
 *  \brief binds a single integer to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id integer to bind to the prepared statement. DATA_ROW_ID_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_diagram_reader_private_bind_id_to_statement ( data_database_diagram_reader_t *this_,
                                                                                     sqlite3_stmt *statement_ptr,
                                                                                     data_row_id_t id
                                                                                   );

/*!
 *  \brief binds a single string to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text char sequence to bind to the prepared statement.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_diagram_reader_private_bind_text_to_statement ( data_database_diagram_reader_t *this_,
                                                                                       sqlite3_stmt *statement_ptr,
                                                                                       const char *text
                                                                                     );

#include "storage/data_database_diagram_reader.inl"

#endif  /* DATA_DATABASE_DIAGRAM_READER_H */


/*
Copyright 2016-2024 Andreas Warnke

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
