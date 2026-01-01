/* File: ctrl_consistency_checker.h; Copyright and License: see below */

#ifndef CTRL_CONSISTENCY_CHECKER_H
#define CTRL_CONSISTENCY_CHECKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Performs consistency checks in the database and can repair the database
 *
 *  - There shall be 1 root diagram
 *      - diagrams.parent_id == DATA_ROW_VOID.
 *  - References shall be valid:
 *      - diagramelements.diagram_id,
 *      - diagramelements.classifier_id,
 *      - diagrams.parent_id,
 *      - features.classifier_id,
 *      - relationships.from_classifier_id,
 *      - relationships.to_classifier_id.
 *  - References shall be valid or NULL:
 *      - relationships.from_feature_id (and consistent classifier_id between relationship and feature)
 *      - relationships.to_feature_id (and consistent classifier_id between relationship and feature)
 *      - diagramelements.focused_feature_id (and consistent classifier_id between diagramelement and feature)
 *  - Objects shall be visible:
 *      - classifiers shall be referenced by diagrams,
 *      - relationships shall be visible in at least one diagram (only partially checked).
 *  - Circular link structures are forbidden in:
 *      - diagrams.parent_id,
 *      - relationships.to_classifier_id (depending on the relationships.main_type, circles are forbidden. not yet checked).
 *  - Names shall be unique:
 *      - classifiers.name (checked by DB-constraint),
 *      - features.key (unique only within one classifier, not checked, low importance).
 *
 */

#include "u8/u8_error.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_consistency_checker.h"
#include "entity/data_diagram.h"
#include "utf8stream/utf8stream_writer.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief data attributes needed for the consistency functions
 */
struct ctrl_consistency_checker_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    data_database_consistency_checker_t db_checker;  /*!< own instance of a db_checker */

    data_diagram_t temp_diagram;  /*!< buffer for reading one diagram after another */
};

typedef struct ctrl_consistency_checker_struct ctrl_consistency_checker_t;

/*!
 *  \brief initializes the ctrl_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database pointer to database
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param db_writer pointer to database writer object that can be used for changing data
 */
void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_, data_database_t *database, data_database_reader_t *db_reader, data_database_writer_t *db_writer );

/*!
 *  \brief destroys the ctrl_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_consistency_checker_destroy ( ctrl_consistency_checker_t *this_ );

/*!
 *  \brief checks and repairs the database
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param out_err number of errors detected (NULL if not requested)
 *  \param out_fix number of errors fixed (NULL if not requested)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted and is not fully fixed yet
 */
u8_error_t ctrl_consistency_checker_repair_database ( ctrl_consistency_checker_t *this_,
                                                      bool modify_db,
                                                      uint32_t *out_err,
                                                      uint32_t *out_fix,
                                                      utf8stream_writer_t *out_english_report
                                                    );

/*!
 *  \brief checks and repairs the database with regards to providing a single root diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_single_root_diagram ( ctrl_consistency_checker_t *this_,
                                                                         bool modify_db,
                                                                         uint32_t *io_err,
                                                                         uint32_t *io_fix,
                                                                         utf8stream_writer_t *out_english_report
                                                                       );

/*!
 *  \brief checks the database with regards to invalid and circular references to parent diagrams
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_valid_diagram_parents ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stream_writer_t *out_english_report
                                                                         );

/*!
 *  \brief checks and repairs the database with regards to diagramelements having a valid references to diagrams and classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_valid_diagramelements ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stream_writer_t *out_english_report
                                                                         );

/*!
 *  \brief checks and repairs the database with regards to diagramelements having NULL or valid references to focused features
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_valid_diagele_features ( ctrl_consistency_checker_t *this_,
                                                                            bool modify_db,
                                                                            uint32_t *io_err,
                                                                            uint32_t *io_fix,
                                                                            utf8stream_writer_t *out_english_report
                                                                          );

/*!
 *  \brief checks and repairs the database with regards to classifiers being referenced by diagramelements
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_referenced_classifiers ( ctrl_consistency_checker_t *this_,
                                                                            bool modify_db,
                                                                            uint32_t *io_err,
                                                                            uint32_t *io_fix,
                                                                            utf8stream_writer_t *out_english_report
                                                                          );

/*!
 *  \brief checks and repairs the database with regards to features having a valid parent classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_valid_feature_parents ( ctrl_consistency_checker_t *this_,
                                                                           bool modify_db,
                                                                           uint32_t *io_err,
                                                                           uint32_t *io_fix,
                                                                           utf8stream_writer_t *out_english_report
                                                                         );

/*!
 *  \brief checks and repairs the database with regards to relationships referencing valid to and from classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_valid_relationship_classifiers ( ctrl_consistency_checker_t *this_,
                                                                                    bool modify_db,
                                                                                    uint32_t *io_err,
                                                                                    uint32_t *io_fix,
                                                                                    utf8stream_writer_t *out_english_report
                                                                                  );

/*!
 *  \brief checks and repairs the database with regards to relationships referencing NULL or valid to and from features
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param io_err number of errors detected (not NULL)
 *  \param io_fix number of errors fixed (not NULL)
 *  \param out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted
 */
u8_error_t ctrl_consistency_checker_private_ensure_valid_relationship_features ( ctrl_consistency_checker_t *this_,
                                                                                 bool modify_db,
                                                                                 uint32_t *io_err,
                                                                                 uint32_t *io_fix,
                                                                                 utf8stream_writer_t *out_english_report
                                                                               );

#endif  /* CTRL_CONSISTENCY_CHECKER_H */


/*
Copyright 2016-2026 Andreas Warnke

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
