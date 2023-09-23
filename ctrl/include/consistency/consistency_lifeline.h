/* File: consistency_lifeline.h; Copyright and License: see below */

#ifndef CONSISTENCY_LIFELINE_H
#define CONSISTENCY_LIFELINE_H

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
#include "data_rules.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct ctrl_classifier_controller_struct;
struct ctrl_diagram_controller_struct;

/*!
 *  \brief constants of consistency_lifeline_t
 */
enum consistency_lifeline_const_enum {
    CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES = 128,  /*!< maximum number of diagramelements in a diagram */
};

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct consistency_lifeline_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    struct ctrl_classifier_controller_struct *clfy_ctrl;  /*!< pointer to external classifier controller */
    struct ctrl_diagram_controller_struct *diag_ctrl;  /*!< pointer to external diagram controller */
    data_rules_t rules;  /*!< own instance of a rules object */

    data_diagramelement_t private_temp_diagele_buf[CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES];  /*!< be aware of */
                                                                                      /*!< reentrancy by recursion! */
};

typedef struct consistency_lifeline_struct consistency_lifeline_t;

/*!
 *  \brief initializes the consistency_lifeline_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 *  \param diag_ctrl pointer to diagram controller to modify diagramelements
 */
void consistency_lifeline_init ( consistency_lifeline_t *this_,
                                 data_database_reader_t *db_reader,
                                 struct ctrl_classifier_controller_struct *clfy_ctrl,
                                 struct ctrl_diagram_controller_struct *diag_ctrl
                               );

/*!
 *  \brief destroys the consistency_lifeline_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void consistency_lifeline_destroy ( consistency_lifeline_t *this_ );

/*!
 *  \brief executes policies involved in changing the diagram type.
 *
 *  Current rules are:
 *  - when changing a diagram type to a non-interaction/non-scenario type,
 *    that is any type except DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    or to DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
 *    all contained elements shall get rid of any lifeline.
 *
 *  \param this_ pointer to own object attributes
 *  \param updated_diagram data of the updated diagram.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_lifeline_delete_lifelines ( consistency_lifeline_t *this_,
                                                   const data_diagram_t *updated_diagram
                                                 );

/*!
 *  \brief executes policies involved in changing the diagram type.
 *
 *  Current rules are:
 *  - when changing a diagram type to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    or to DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
 *    all contained elements shall get a lifeline
 *    and this lifeline shall be the focused_feature of the diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param updated_diagram data of the updated diagram.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_lifeline_create_lifelines ( consistency_lifeline_t *this_,
                                                   const data_diagram_t *updated_diagram
                                                 );

/*!
 *  \brief executes policies involved in creating a diagramelement.
 *
 *  Current rules are:
 *  - when creating a diagramelement
 *    to a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    or to DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
 *    the new diagramelement shall get a lifeline
 *    and this lifeline shall be the focused_feature of the diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagramelement data of the new diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_lifeline_create_a_lifeline ( consistency_lifeline_t *this_,
                                                    const data_diagramelement_t *new_diagramelement
                                                  );

/*!
 *  \brief creates one lifeline, unconditional
 *
 *  \param this_ pointer to own object attributes
 *  \param the_diagramelement data of the diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_lifeline_private_create_one_lifeline ( consistency_lifeline_t *this_,
                                                              const data_diagramelement_t *the_diagramelement
                                                            );

/*!
 *  \brief executes policies involved in deleting a diagramelement.
 *
 *  Current rules are:
 *  - when deleting a diagramelement,
 *    a possibly referenced focused_feature of type lifeline
 *    is also deleted.
 *  - when deleting above referenced lifeline, all relationships
 *    to and from that lifeline are also deleted.
 *
 *  Rows are deleted in an order that enables an always consistent database structure.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_lifeline_delete_a_lifeline ( consistency_lifeline_t *this_,
                                                    const data_diagramelement_t *deleted_diagramelement
                                                  );

/*!
 *  \brief executes policies involved in deleting a feature.
 *
 *  Current rules are:
 *  - when deleting a feature
 *    all diagramelements that reference the feature shall be modified to not reference the feature anymore.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_feature data of the deleted feature.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_lifeline_unlink_lifeline ( consistency_lifeline_t *this_,
                                                  const data_feature_t *deleted_feature
                                                );

#endif  /* CONSISTENCY_LIFELINE_H */


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
