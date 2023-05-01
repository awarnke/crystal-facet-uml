/* File: pencil_rules.h; Copyright and License: see below */

#ifndef PENCIL_RULES_H
#define PENCIL_RULES_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines filters for uml elements to display.
 *
 *  Via this class, one can check if
 *  - a lifeline is visible or if it implicitly proxies a classifier,
 *  - a relationship is visible or if it is implicitly shown by being contained in an outer classifier.
 */

#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include "data_diagram_type.h"
#include <stdbool.h>

/*!
 *  \brief attributes of a pencil_rules_t
 *
 *  \note This class is stateless.
 *        It may either be instantiated once and used many times or be instantiated per use.
 */
struct pencil_rules_struct {
    int dummy;  /*!< rules have no attributes */
};

typedef struct pencil_rules_struct pencil_rules_t;

/*!
 *  \brief initializes the pencil_rules_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_rules_init ( pencil_rules_t *this_ );

/*!
 *  \brief destroys the pencil_rules_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void pencil_rules_destroy ( pencil_rules_t *this_ );

/*!
 *  \brief determines if the feature is a lifeline and if the lifeline in the current diagram is only an implicit proxy.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type the feature type
 *  \param classifier_type the classifier type
 *  \param diagram_type the diagram type
 *  \return false if the feature type is not DATA_FEATURE_TYPE_LIFELINE
 *          or the diagram type is not DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *          and not DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM
 *          or the classifier_type is DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE
 *          or DATA_CLASSIFIER_TYPE_REQUIREMENT or DATA_CLASSIFIER_TYPE_COMMENT
 *          or DATA_CLASSIFIER_TYPE_INTERACTION or DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE
 */
static inline bool pencil_rules_feature_is_implicit_proxy ( const pencil_rules_t *this_,
                                                            data_feature_type_t feature_type,
                                                            data_classifier_type_t classifier_type,
                                                            data_diagram_type_t diagram_type
                                                          );

#include "filter/pencil_rules.inl"

#endif  /* PENCIL_RULES_H */


/*
Copyright 2023-2023 Andreas Warnke

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
