/* File: data_rules.h; Copyright and License: see below */

#ifndef DATA_RULES_H
#define DATA_RULES_H

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
 *    (defined here in data_rules any implemented by ctrl_classifier_trigger and ctrl_diagram_trigger).
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
 *  \brief attributes of a data_rules_t
 */
struct data_rules_struct {
    int dummy;  /*!< rules have no attributes */
};

typedef struct data_rules_struct data_rules_t;

/*!
 *  \brief initializes the data_rules_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_rules_init ( data_rules_t *this_ );

/*!
 *  \brief destroys the data_rules_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_rules_destroy ( data_rules_t *this_ );

/* ================================ VISIBLE_SET ================================ */

/*!
 *  \brief determines if the feature shall be visible in the diagram of the data_visible_set_t
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_set the data_visible_set_t containing the cached diagram data for pencil
 *  \param feature_id the id of the feature, needed in case the feature is a lifeline
 *  \return true if the feature shall be visible according to the data_rules
 *          (and the diagram_set contains the feature and the related classifier)
 */
bool data_rules_diagram_shows_feature ( const data_rules_t *this_,
                                        const data_visible_set_t *diagram_set,
                                        data_row_t feature_id
                                      );

/*!
 *  \brief determines if the relationship shall be visible in the diagram of the data_visible_set_t
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_set the data_visible_set_t containing the cached diagram data for pencil
 *  \param relationship_id the id of the relationship
 *  \return true if the relationship shall be visible according to the data_rules
 *          (and the diagram_set contains the relationship and the related classifiers
 *          and the possibly related features)
 */
bool data_rules_diagram_shows_relationship ( const data_rules_t *this_,
                                             const data_visible_set_t *diagram_set,
                                             data_row_t relationship_id
                                           );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief determines if the diagram type is scenario-based
 *
 *  This method implements a definition of "scenario"
 *  which corresponds to a uml interaction.
 *  This classification is the basis for a rule on visibility.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if the feature type is DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *          or DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM or DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM.
 */
static inline bool data_rules_diagram_is_scenario ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/* ================================ CLASSIFIER ================================ */

/* ================================ FEATURE ================================ */

/*!
 *  \brief determines if the feature type is scenario-conditional (applicable only to the 4 interaction diagram types)
 *
 *  This method implements a definition of "scenario".
 *  This classification is the basis for a rule on visibility.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type the feature type
 *  \return true if the feature type is DATA_FEATURE_TYPE_LIFELINE.
 */
static inline bool data_rules_feature_is_scenario_cond ( const data_rules_t *this_, data_feature_type_t feature_type );

/*!
 *  \brief determines if the unconditional features shall be visible in the given diagram
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if an unconditional feature (non-lifeline) shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_uncond_features ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief determines if the scenario-specific features shall be visible in the given diagram
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if a lifeline shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_scenario_features ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief determines if the visible classifier has the feature
 *
 *  This method does not take into account if the diagram shows this kind of feature.
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param vis_classifier the visible classifier
 *  \param feature the feature
 *  \return true if the feature belongs to the vis_classifier
 *          and in the scenario case (lifeline), the feature is the focused feature
 *          and data_rules_classifier_has_features()
 */
static inline bool data_rules_vis_classifier_has_feature ( const data_rules_t *this_,
                                                           const data_visible_classifier_t *vis_classifier,
                                                           const data_feature_t *feature
                                                         );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief determines if the relationship type is scenario-conditional
 *
 *  This method implements a definition of "scenario".
 *  This classification is the basis for a rule on visibility.
 *
 *  \param this_ pointer to own object attributes
 *  \param from_feature_type the feature type of the source end, DATA_FEATURE_TYPE_VOID if source is not a feature
 *  \param to_feature_type the feature type of the destination end, DATA_FEATURE_TYPE_VOID if destination is not a feature
 *  \return true if the from_feature_type type is DATA_FEATURE_TYPE_LIFELINE and/or the to_feature_type is DATA_FEATURE_TYPE_LIFELINE.
 */
static inline bool data_rules_relationship_is_scenario_cond ( const data_rules_t *this_,
                                                              data_feature_type_t from_feature_type,
                                                              data_feature_type_t to_feature_type
                                                            );

/*!
 *  \brief determines if the unconditional relationships shall be visible in the given diagram
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if an unconditional relationship (non-lifeline start+end) shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_uncond_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief determines if the scenario-specific relationships shall be visible in the given diagram
 *
 *  This method implements a rule.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if a scenario-specific relationship (start or end at lifeline) shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_scenario_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type );

#include "data_rules.inl"

#endif  /* DATA_RULES_H */


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
