/* File: ctrl_consistency_checker.h; Copyright and License: see below */

#ifndef CTRL_CONSISTENCY_CHECKER_H
#define CTRL_CONSISTENCY_CHECKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Performs consistency checks in the database
 *
 *  - References shall be valid:
 *      - diagramelements.diagram_id,
 *      - diagramelements.classifier_id,
 *      - diagrams.parent_id,
 *      - features.classifier_id,
 *      - relationships.from_classifier_id,
 *      - relationships.to_classifier_id.
 *  - Objects shall be linked:
 *      - classifiers shall be referenced by diagrams
 *  - Circular link structures are forbidden in:
 *      - diagrams.parent_id.
 *  - Names shall be unique:
 *      - classifiers.name (checked by DB-contraint),
 *      - features.key (checked by DB-contraint).
 *  - Enumerations shall be valid constants:
 *      - classifiers.main_type,
 *      - relationships.main_type,
 *      - features.main_type,
 *      - diagrams.diagram_type,
 *      - diagramelements.display_flags.
 */

#include "ctrl_error.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief data attributes needed for the consistency functions
 */
struct ctrl_consistency_checker_struct {
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
};

typedef struct ctrl_consistency_checker_struct ctrl_consistency_checker_t;

/*!
 *  \brief initializes the ctrl_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param db_writer pointer to database writer object that can be used for changing data
 */
void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_, data_database_reader_t *db_reader, data_database_writer_t *db_writer );

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
 *  \return CTRL_ERROR_NONE in case of success,
 *          CTRL_ERROR_NO_DB if database not open/loaded,
 *          CTRL_ERROR_DB_STRUCTURE if database was corrupted
 */
ctrl_error_t ctrl_consistency_checker_repair_database ( ctrl_consistency_checker_t *this_, bool modify_db );

#endif  /* CTRL_CONSISTENCY_CHECKER_H */


/*
Copyright 2016-2016 Andreas Warnke

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
