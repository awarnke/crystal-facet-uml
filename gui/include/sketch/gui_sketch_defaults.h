/* File: gui_sketch_defaults.h; Copyright and License: see below */

#ifndef GUI_SKETCH_DEFAULTS_H
#define GUI_SKETCH_DEFAULTS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines default types for for uml/sysml models - depending on their usage context
 */

#include "set/data_visible_set.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship_type.h"
#include "entity/data_diagram_type.h"
#include "entity/data_row.h"
#include <stdbool.h>

/*!
 *  \brief attributes of gui_sketch_defaults_t
 */
struct gui_sketch_defaults_struct {
    int dummy;  /*!< defaults have no attributes */
};

typedef struct gui_sketch_defaults_struct gui_sketch_defaults_t;

/*!
 *  \brief initializes the gui_sketch_defaults_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_defaults_init ( gui_sketch_defaults_t *this_ );

/*!
 *  \brief destroys the gui_sketch_defaults_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_defaults_destroy ( gui_sketch_defaults_t *this_ );

/*!
 *  \brief gets the default classifier type of the given diagram type
 *
 *  This method implements a guideline.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type the diagram type
 *  \return default classifier type in the given diagram_type.
 *          DATA_CLASSIFIER_TYPE_COMMENT if diagram_type is invalid.
 */
static inline data_classifier_type_t gui_sketch_defaults_get_classifier_type ( const gui_sketch_defaults_t *this_,
                                                                               data_diagram_type_t diagram_type
                                                                             );

/*!
 *  \brief gets the default relationship type of the given source(from) classifier type
 *
 *  This method implements a guideline.
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_type the classifier type where the relation starts from
 *  \param from_feature_type the feature type where the relation starts from;
 *                           DATA_FEATURE_TYPE_VOID if relation starts directly at the classifier
 *  \return default relationship type of the given from(source) classifier type.
 *          DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY if from_classifier_type is invalid.
 */
static inline data_relationship_type_t gui_sketch_defaults_get_relationship_type ( const gui_sketch_defaults_t *this_,
                                                                                   data_classifier_type_t from_classifier_type,
                                                                                   data_feature_type_t from_feature_type
                                                                                 );

/*!
 *  \brief gets the default feature type of the given parent classifier type
 *
 *  This method implements a guideline.
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_classifier_type the classifier type for which to create a feature
 *  \return default feature type of the given parent classifier type.
 *          DATA_FEATURE_TYPE_PROPERTY if parent_classifier_type is invalid.
 */
static inline data_feature_type_t gui_sketch_defaults_get_feature_type ( const gui_sketch_defaults_t *this_,
                                                                         data_classifier_type_t parent_classifier_type
                                                                       );

#include "gui_sketch_defaults.inl"

#endif  /* GUI_SKETCH_DEFAULTS_H */


/*
Copyright 2017-2024 Andreas Warnke

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
