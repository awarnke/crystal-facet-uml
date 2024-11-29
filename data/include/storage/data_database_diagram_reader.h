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
#include "storage/data_classifier_iterator.h"
#include "storage/data_diagram_iterator.h"
#include "storage/data_diagramelement_iterator.h"
#include "storage/data_visible_classifier_iterator.h"
#include "storage/data_database.h"
#include "entity/data_diagram.h"
#include "u8/u8_error.h"
#include "entity/data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "entity/data_row_id.h"
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
    bool statement_diagrams_by_parent_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_diagrams_by_parent_id_null;
    bool statement_diagrams_by_parent_id_null_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_diagrams_by_classifier_id;
    bool statement_diagrams_by_classifier_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_diagram_ids_by_parent_id;
    sqlite3_stmt *statement_diagram_ids_by_parent_id_null;
    sqlite3_stmt *statement_diagram_ids_by_classifier_id;

    sqlite3_stmt *statement_diagramelement_by_id;
    sqlite3_stmt *statement_diagramelement_by_uuid;
    sqlite3_stmt *statement_diagramelements_by_diagram_id;
    bool statement_diagramelements_by_diagram_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_diagramelements_by_classifier_id;
    bool statement_diagramelements_by_classifier_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */

    sqlite3_stmt *statement_visible_classifiers_by_diagram_id;
    bool statement_visible_classifiers_by_diagram_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
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
 *  \param[in,out] io_diagram_iterator iterator over diagrams of selected parent diagram. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagrams_by_parent_id ( data_database_diagram_reader_t *this_,
                                                                    data_row_id_t parent_id,
                                                                    data_diagram_iterator_t *io_diagram_iterator
                                                                  );

/*!
 *  \brief reads all classifier-displaying diagrams from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param[in,out] io_diagram_iterator iterator over diagrams of selected classifier. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagrams_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                        data_row_id_t classifier_id,
                                                                        data_diagram_iterator_t *io_diagram_iterator
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
 *  \param[in,out] io_diagramelement_iterator iterator over diagramelements of selected diagram. The caller is responsible
 *                                            for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagramelements_by_diagram_id ( data_database_diagram_reader_t *this_,
                                                                            data_row_id_t diagram_id,
                                                                            data_diagramelement_iterator_t *io_diagramelement_iterator
                                                                          );

/*!
 *  \brief reads all diagramelements of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the diagram
 *  \param[in,out] io_diagramelement_iterator iterator over diagramelements of selected classifier. The caller is responsible
 *                                            for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_diagramelements_by_classifier_id ( data_database_diagram_reader_t *this_,
                                                                               data_row_id_t classifier_id,
                                                                               data_diagramelement_iterator_t *io_diagramelement_iterator
                                                                             );

/* ================================ VISIBLE_CLASSIFIER ================================ */

/*!
 *  \brief reads all visible classifiers of a diagram from the database.
 *
 *  If a visible classifier is contained multiple times in a diagram, it is returned multiple times in the io_visible_classifier_iterator.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param[in,out] io_visible_classifier_iterator iterator over visible classifiers of selected diagram. The caller is responsible
 *                                                for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_diagram_reader_get_visible_classifiers_by_diagram_id ( data_database_diagram_reader_t *this_,
                                                                                data_row_id_t diagram_id,
                                                                                data_visible_classifier_iterator_t *io_visible_classifier_iterator
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
