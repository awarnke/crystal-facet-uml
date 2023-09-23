/* File: consistency_drop_invisibles.h; Copyright and License: see below */

#ifndef CONSISTENCY_DROP_INVISIBLES_H
#define CONSISTENCY_DROP_INVISIBLES_H

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
#include "set/data_node_set.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct ctrl_classifier_controller_struct;
struct ctrl_diagram_controller_struct;

/*!
 *  \brief constants of consistency_drop_invisibles_t
 */
enum consistency_drop_invisibles_const_enum {
    CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_DIAGELES = 128,  /*!< maximum number of diagramelements of a classifier */
    CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_RELATIONS = DATA_NODE_SET_MAX_RELATIONSHIPS,  /*!< maximum number of relationships attached to a classifier */
};

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct consistency_drop_invisibles_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    struct ctrl_classifier_controller_struct *clfy_ctrl;  /*!< pointer to external classifier controller */
    struct ctrl_diagram_controller_struct *diag_ctrl;  /*!< pointer to external diagram controller */

    data_diagramelement_t private_temp_diagele_buf[CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_DIAGELES];/*!< be aware */
                                                                                     /*!< of reentrancy by recursion! */
    data_relationship_t private_temp_rel_buf[CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_RELATIONS];  /*!< be aware of */
                                                                                        /*!< reentrancy by recursion! */
};

typedef struct consistency_drop_invisibles_struct consistency_drop_invisibles_t;

/*!
 *  \brief initializes the consistency_drop_invisibles_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 *  \param diag_ctrl pointer to diagram controller to modify diagramelements
 */
void consistency_drop_invisibles_init ( consistency_drop_invisibles_t *this_,
                                        data_database_reader_t *db_reader,
                                        struct ctrl_classifier_controller_struct *clfy_ctrl,
                                        struct ctrl_diagram_controller_struct *diag_ctrl
                                      );

/*!
 *  \brief destroys the consistency_drop_invisibles_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void consistency_drop_invisibles_destroy ( consistency_drop_invisibles_t *this_ );

/* ================================ NO ABANDONED CLASSIFIERS ================================ */

/*!
 *  \brief executes policies involved in deleting a diagramelement.
 *
 *  Current rules are:
 *  - after deleting a diagramelement,
 *    delete the classifier (if now unreferenced)
 *
 *  Rows are deleted in an order that enables an always consistent database structure.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_drop_invisibles_delete_unreferenced_classifier ( consistency_drop_invisibles_t *this_,
                                                                        const data_diagramelement_t *deleted_diagramelement
                                                                      );

/* ================================ NO INVISIBLE RELATIONSHIPS ================================ */

/*!
 *  \brief executes policies involved in deleting a diagramelement.
 *
 *  Current rules are:
 *  - after deleting a diagramelement,
 *    delete relationships where from classifier and to classifier have no diagram in common
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_drop_invisibles_delete_invisible_relationships ( consistency_drop_invisibles_t *this_,
                                                                        const data_diagramelement_t *deleted_diagramelement
                                                                      );

/*!
 *  \brief checks if a relationship is visible in a diagram.
 *
 *  \param this_ pointer to own object attributes
 *  \param relation relationship to be checked for visibility.
 *  \param[out] out_result true if the relatinoship end classifiers are visible in the same diagram
 *                         (except diagram-type-specific filtering).
 *  \return error id in case of an error, e.g. U8_ERROR_ARRAY_BUFFER_EXCEEDED; U8_ERROR_NONE in case of success
 */
u8_error_t consistency_drop_invisibles_private_has_relationship_a_diagram ( consistency_drop_invisibles_t *this_,
                                                                             const data_relationship_t *relation,
                                                                             bool *out_result
                                                                           );

#endif  /* CONSISTENCY_DROP_INVISIBLES_H */


/*
Copyright 2018-2023 Andreas Warnke

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
