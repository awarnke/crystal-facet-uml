/* File: data_guidelines.h; Copyright and License: see below */

#ifndef DATA_GUIDELINES_H
#define DATA_GUIDELINES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines constraints, invariants and recommendations for uml/sysml models
 *
 *  There are several levels of constraints:
 *  - On the deepest level, the database structure itself, there exist must-have rules, checked by the data_database_consistency_checker.
 *    These rules ensure the consistency of the entities and relationships in the database.
 *    For example a relationship must have a valid classifier at each end.
 *  - On the next level, there are must-have rules about which model element is existant in which diagram
 *    (defined here in data_guidelines any implemented by ctrl_classifier_trigger and ctrl_diagram_trigger).
 *    These rules ensure that all model elements are visible in at least one diagram.
 *    For example a relationship exists in a diagram only if source and destination classifiers are contained in the diagram.
 *    For example a list-diagram-type hides relationships and features.
 *  - On the third level, there are nice-to-have guidelines implemented in the pencil package, which decides how to show elements,
 *    like containment relationships if the containment is visible by inclusion already
 *    or like hiding lifelines if the classifier is a comment or ineraction-use.
 *  .
 */

#include "set/data_visible_set.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship_type.h"
#include "entity/data_diagram_type.h"
#include "entity/data_row.h"
#include <stdbool.h>

/*!
 *  \brief attributes of a data_guidelines_t
 */
struct data_guidelines_struct {
    int dummy;  /*!< rules have no attributes */
};

typedef struct data_guidelines_struct data_guidelines_t;

/*!
 *  \brief initializes the data_guidelines_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_guidelines_init ( data_guidelines_t *this_ );

/*!
 *  \brief destroys the data_guidelines_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_guidelines_destroy ( data_guidelines_t *this_ );

/* ================================ DIAGRAM ================================ */

/* ================================ CLASSIFIER ================================ */

/* ================================ FEATURE ================================ */

/*!
 *  \brief determines if lifelines have semantics
 *
 *  In a scenario-typed diagram, every visible classifier has a lifeline.
 *  This is necessary because the classifier type can be changed (which shall not invalidate relationships).
 *  But a lifeline does not make sense for comments, requirements and (in sequences) also diagram-references.
 *
 *  This method implements a guideline.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \param classifier_type the classifier type
 *  \return true if the classifier type is DATA_CLASSIFIER_TYPE_REQUIREMENT
 *          or DATA_CLASSIFIER_TYPE_COMMENT
 *          or DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE
 *          or ( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *          and ( DATA_CLASSIFIER_TYPE_INTERACTION or DATA_CLASSIFIER_TYPE_INTERACTION_USE )).
 */
static inline bool data_guidelines_classifier_has_scenario_semantics ( const data_guidelines_t *this_,
                                                                  data_diagram_type_t diagram_type,
                                                                  data_classifier_type_t classifier_type
                                                                );

/*!
 *  \brief determines if the feature value is a stereotype
 *
 *  Properties, Operations and Tagges Values do not have stereotypes, the value is the value.
 *  Other types do not have a value, the value stores the stereotype instead.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type the feature type
 *  \return true if the feature type is not DATA_FEATURE_TYPE_PROPERTY, DATA_FEATURE_TYPE_OPERATION,
 *          DATA_FEATURE_TYPE_TAGGED_VALUE.
 */
static inline bool data_guidelines_feature_value_is_stereotype ( const data_guidelines_t *this_, data_feature_type_t feature_type );

/* ================================ RELATIONSHIP ================================ */

#include "data_guidelines.inl"

#endif  /* DATA_GUIDELINES_H */


/*
Copyright 2017-2026 Andreas Warnke

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
