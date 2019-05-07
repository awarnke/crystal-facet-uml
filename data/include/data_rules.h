/* File: data_rules.h; Copyright and License: see below */

#ifndef DATA_RULES_H
#define DATA_RULES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines constraints, invariants and recommendations for uml/sysml models
 */

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
 *  \return default classifier type in the given diagram_type. DATA_CLASSIFIER_TYPE_UML_COMMENT if diagram_type is invalid.
 */
static inline data_classifier_type_t data_rules_get_default_classifier_type ( const data_rules_t *this_, data_diagram_type_t diagram_type );

/*!
 *  \brief gets the default relationship type of the given source(from) classifier type
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_type the classifier type where the relation starts from
 *  \return default relationship type of the given from(source) classifier type. DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY if from_classifier_type is invalid.
 */
static inline data_relationship_type_t data_rules_get_default_relationship_type ( const data_rules_t *this_, data_classifier_type_t from_classifier_type );

/*!
 *  \brief gets the default feature type of the given parent classifier type
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_classifier_type the classifier type for which to create a feature
 *  \return default feature type of the given parent classifier type. DATA_FEATURE_TYPE_PROPERTY if parent_classifier_type is invalid.
 */
static inline data_feature_type_t data_rules_get_default_feature_type ( const data_rules_t *this_, data_classifier_type_t parent_classifier_type );

/*!
 *  \brief determines if the classifier type has features
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type the classifier type
 *  \return true if the classifier type may have features, false otherwise. false also if classifier_type is invalid.
 */
static inline bool data_rules_has_features ( const data_rules_t *this_, data_classifier_type_t classifier_type );

/*!
 *  \brief determines if the classifier type is always an instance
 *
 *  This is true for part and object
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type the classifier type
 *  \return true if the classifier type cannot be an abstract concept but is always an instance.
 */
static inline bool data_rules_is_always_instance ( const data_rules_t *this_, data_classifier_type_t classifier_type );

/* static inline bool data_rules_diagram_shows_features ( const data_rules_t *this_, data_diagram_type_t diagram_type ); */
/* static inline bool data_rules_diagram_shows_classifier_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type ); */
/* static inline bool data_rules_diagram_shows_feature_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type ); */

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
