/* File: data_rules.h; Copyright and License: see below */

#ifndef DATA_RULES_H
#define DATA_RULES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines constraints, invariants and recommendations for uml/sysml models
 */

#include "set/data_visible_set.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include "data_diagram_type.h"
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

/*!
 *  \brief gets the default classifier type of the given diagram type
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return default classifier type in the given diagram_type.
 *          DATA_CLASSIFIER_TYPE_UML_COMMENT if diagram_type is invalid.
 */
static inline data_classifier_type_t data_rules_get_default_classifier_type ( const data_rules_t *this_,
                                                                              data_diagram_type_t diagram_type
                                                                            );

/*!
 *  \brief gets the default relationship type of the given source(from) classifier type
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_type the classifier type where the relation starts from
 *  \return default relationship type of the given from(source) classifier type.
 *          DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY if from_classifier_type is invalid.
 */
static inline data_relationship_type_t data_rules_get_default_relationship_type ( const data_rules_t *this_,
                                                                                  data_classifier_type_t from_classifier_type
                                                                                );

/*!
 *  \brief gets the default feature type of the given parent classifier type
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_classifier_type the classifier type for which to create a feature
 *  \return default feature type of the given parent classifier type.
 *          DATA_FEATURE_TYPE_PROPERTY if parent_classifier_type is invalid.
 */
static inline data_feature_type_t data_rules_get_default_feature_type ( const data_rules_t *this_,
                                                                        data_classifier_type_t parent_classifier_type
                                                                      );

/* ================================ VISIBLE_SET ================================ */

/*!
 *  \brief determines if the feature shall be visible in the diagram of the data_visible_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_set the data_visible_set_t containing the cached diagram data for pencil
 *  \param feature_id the id of the feature
 *  \return true if the feature shall be visible according to the data_rules
 *          (and the diagram_set contains the feature and the related classifier)
 */
bool data_rules_diagram_shows_feature ( const data_rules_t *this_,
                                        const data_visible_set_t *diagram_set,
                                        int64_t feature_id
                                      );

/*!
 *  \brief determines if the relationship shall be visible in the diagram of the data_visible_set_t
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
                                             int64_t relationship_id
                                           );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief determines if the diagram type is scenario-based
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
 *  \param this_ pointer to own object attributes
 *  \param classifier_type the classifier type
 *  \return true if the classifier type may have features, false otherwise. false also if classifier_type is invalid.
 */
static inline bool data_rules_classifier_has_uncond_features ( const data_rules_t *this_, data_classifier_type_t classifier_type );

/*!
 *  \brief determines if the feature type is scenario-conditional
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type the feature type
 *  \return true if the feature type is DATA_FEATURE_TYPE_LIFELINE.
 */
static inline bool data_rules_feature_is_scenario_cond ( const data_rules_t *this_, data_feature_type_t feature_type );

/*!
 *  \brief determines if the unconditional features shall be visible in the given diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if an unconditional feature (non-lifeline) shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_uncond_features ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief determines if the scenario-specific features shall be visible in the given diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if a lifeline shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_scenario_features ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief determines if the visible classifier has the feature
 *
 *  This method does not take into account if the diagram shows this kind of feature
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
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if an unconditional relationship (non-lifeline start+end) shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_uncond_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief determines if the scenario-specific relationships shall be visible in the given diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return true if a scenario-specific relationship (start or end at lifeline) shall be visible, false otherwise.
 */
static inline bool data_rules_diagram_shows_scenario_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type );

#include "data_rules.inl"

#endif  /* DATA_RULES_H */


/*
Copyright 2017-2019 Andreas Warnke

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
