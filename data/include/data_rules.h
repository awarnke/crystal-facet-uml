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
 *    For example a relationship must have a valid classifier at each end.
 *  - On the next level, there are must-have rules about which model element is existant in which diagram (implemented here in data_rules).
 *    For example a relationship exists in a diagram only if source and destination classifiers are contained in the diagram.
 *  - On the third level, there are nice-to-have guidelines about what could be hidden but which is possibly not consistent
 *    between versions of crystal_facet_uml, between usage and export/import functions (implemented here in data_rules).
 *    For example a list-diagram-type may hide relationships, an actor-classifier may hide ports.
 *  - Another level is implemented in the pencil package, which decides if to hide elements,
 *    like containment relationships if the containment is visible by inclusion already.
 *  .
 *
 *  Which method implements a guideline and which implements a rule is annotated in the corresponding doxygen comment.
 *  A rule-of-thumb is:
 *  - If a condition makes no sense, a rule states that it must not be.
 *  - If a condition is currently not wanted but could be changed in the future, a guideline proposes to avoid it.
 *  .
 */

#include "set/data_visible_set.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include "data_diagram_type.h"
#include "data_row_id.h"
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
 *  This method implements a combination of guidelines and rules.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_set the data_visible_set_t containing the cached diagram data for pencil
 *  \param feature_id the id of the feature
 *  \return true if the feature shall be visible according to the data_rules
 *          (and the diagram_set contains the feature and the related classifier)
 */
bool data_rules_diagram_shows_feature ( const data_rules_t *this_,
                                        const data_visible_set_t *diagram_set,
                                        data_row_id_t feature_id
                                      );

/*!
 *  \brief determines if the relationship shall be visible in the diagram of the data_visible_set_t
 *
 *  This method implements a combination of guidelines and rules.
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
                                             data_row_id_t relationship_id
                                           );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief determines if the diagram type is scenario-based
 *
 *  This method implements a definition of "scenario"
 *  which corresponds to a uml interaction.
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
 *  \brief determines if the classifier type has unconditional features
 *
 *  This method implements a guideline.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type the classifier type
 *  \return true if the classifier type may have features, false otherwise. false also if classifier_type is invalid.
 */
static inline bool data_rules_classifier_has_uncond_features ( const data_rules_t *this_, data_classifier_type_t classifier_type );

/*!
 *  \brief determines if the feature type is scenario-conditional (applicable only to the 4 interaction diagram types)
 *
 *  This method implements a definition of "scenario".
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type the feature type
 *  \return true if the feature type is DATA_FEATURE_TYPE_LIFELINE.
 */
static inline bool data_rules_feature_is_scenario_cond ( const data_rules_t *this_, data_feature_type_t feature_type );

/*!
 *  \brief determines if the unconditional features shall be visible in the given diagram
 *
 *  This method implements a guideline.
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
 *  This method implements a combination of guidelines and rules.
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
 *          and ( DATA_CLASSIFIER_TYPE_INTERACTION or DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )).
 */
static inline bool data_rules_classifier_has_scenario_semantics ( const data_rules_t *this_,
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
static inline bool data_rules_feature_value_is_stereotype ( const data_rules_t *this_, data_feature_type_t feature_type );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief determines if the relationship type is scenario-conditional
 *
 *  This method implements a definition of "scenario".
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
 *  This method implements a guideline.
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

/*!
 *  \brief determines if the relationships complies to the specification
 *
 *  \param this_ pointer to own object attributes
 *  \param relation_type the type of relationship for which compliance shall be checked
 *  \param from_c_type the type of classifier at source end for which compliance shall be checked
 *  \param from_f_type the type of feature at source end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param to_c_type the type of classifier at target end for which compliance shall be checked
 *  \param to_f_type the type of feature at target end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \return true if relationship type betweeen classifiers is compliant to spec, false otherwise.
 */
/*
static inline bool data_rules_is_relationship_compliant ( const data_rules_t *this_,
                                                          data_relationship_type_t relation_type,
                                                          data_classifier_type_t from_c_type,
                                                          data_feature_type_t from_f_type,
                                                          data_classifier_type_t to_c_type,
                                                          data_feature_type_t to_f_type
                                                        );
*/

#include "data_rules.inl"

#endif  /* DATA_RULES_H */


/*
Copyright 2017-2023 Andreas Warnke

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
