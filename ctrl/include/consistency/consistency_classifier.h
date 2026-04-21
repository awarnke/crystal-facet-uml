/* File: consistency_classifier.h; Copyright and License: see below */

#ifndef CONSISTENCY_CLASSIFIER_H
#define CONSISTENCY_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Enforces application-level conditions.
 *
 *  While application level conditions are more a nice to have feature to not irritate the user
 *  and to support in creating consistent diagrams,
 *  the ctrl_consistency_checker checks that the model in the database is valid,
 *  e.g. relations link to existing objects instead of invalid ids.
 */

#include "u8/u8_error.h"
#include "storage/data_database_reader.h"
/* #include "ctrl_stat.h" */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct ctrl_classifier_controller_struct;
struct ctrl_diagram_controller_struct;

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct consistency_classifier_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    struct ctrl_classifier_controller_struct *clfy_ctrl;  /*!< pointer to external classifier controller */
};

typedef struct consistency_classifier_struct consistency_classifier_t;

/*!
 *  \brief initializes the consistency_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 */
void consistency_classifier_init ( consistency_classifier_t *this_,
                                   data_database_reader_t *db_reader,
                                   struct ctrl_classifier_controller_struct *clfy_ctrl
                                 );

/*!
 *  \brief destroys the consistency_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void consistency_classifier_destroy ( consistency_classifier_t *this_ );

/*!
 *  \brief executes policies on classifiers triggered by deleting a diagramelement.
 *
 *  Current rules are:
 *  - after deleting a diagramelement,
 *    delete the classifier (if now unreferenced)
 *
 *  Rows are deleted in an order that enables an always consistent database structure.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \param[in,out] io_stat Statistics on created and deleted objects.
 *                         *io_stat shall be initialized by caller, statistics are added to initial values.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_classifier_delete_unreferenced_classifier ( consistency_classifier_t *this_,
                                                                   const data_diagramelement_t *deleted_diagramelement /*,*/
                                                                   /* ctrl_stat_t *io_stat */
                                                                 );

#endif  /* CONSISTENCY_CLASSIFIER_H */


/*
Copyright 2018-2026 Andreas Warnke

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
