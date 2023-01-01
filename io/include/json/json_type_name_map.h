/* File: json_type_name_map.h; Copyright and License: see below */

#ifndef JSON_TYPE_NAME_MAP_H
#define JSON_TYPE_NAME_MAP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief maps type ids to type names (for the model)
 *         and flag bitsets to tag name lists (for the diagrams)
 */

#include "data_diagram.h"
#include "data_diagramelement.h"
#include "data_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_diagramelement_flag.h"
#include "data_diagram_flag.h"

/*!
 *  \brief attributes of the json_type_name_map
 */
struct json_type_name_map_struct {
    int dummy;  /*!< a dummy value */
};

typedef struct json_type_name_map_struct json_type_name_map_t;

/*!
 *  \brief initializes the json_type_name_map
 *
 *  \param this_ pointer to own object attributes
 */
static inline void json_type_name_map_init( json_type_name_map_t *this_ );

/*!
 *  \brief destroys the json_type_name_map
 *
 *  \param this_ pointer to own object attributes
 */
static inline void json_type_name_map_destroy( json_type_name_map_t *this_ );

/*!
 *  \brief gets the type name of the data_classifier_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type for some classifiers, the result depends on the used context,
 *                     therefore the parent type is needed to select the xmi_element_info_t
 *  \param classifier_type id for which to select the name
 *  \return type name of classifier_type id; empty string if the id is unknown
 */
static inline const char * json_type_name_map_get_classifier_type ( const json_type_name_map_t *this_,
                                                                    data_classifier_type_t parent_type,
                                                                    data_classifier_type_t classifier_type
                                                                  );

/*!
 *  \brief gets the type name of the data_feature_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the feature type shall be determined
 *  \param feature_type id for which to select the name
 *  \return type name of feature_type id; empty string if the id is unknown
 */
static inline const char * json_type_name_map_get_feature_type ( const json_type_name_map_t *this_,
                                                                 data_classifier_type_t parent_type,
                                                                 data_feature_type_t feature_type
                                                               );

/*!
 *  \brief gets the type name of the data_relationship_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param statemachine_context for some relationships, the result depends on the used context,
 *                              e.g. transition in statemachine or control flow in activities
 *  \param rel_type id for which to select the name
 *  \return type name of rel_type id; empty string if the id is unknown
 */
static inline const char * json_type_name_map_get_relationship_type ( const json_type_name_map_t *this_,
                                                                      bool statemachine_context,
                                                                      data_relationship_type_t rel_type
                                                                    );

/*!
 *  \brief gets the type name of the data_diagram_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type id for which to select the name
 *  \return type name of diagram_type id; empty string if the id is unknown
 */
static inline const char * json_type_name_map_get_diagram_type ( const json_type_name_map_t *this_,
                                                                 data_diagram_type_t diagram_type
                                                               );

/*!
 *  \brief gets the tag name of the data_diagram_flag_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_flag flags for which to select the name
 *  \return tag names of diagram_flag id; empty string if the id is unknown
 */
static inline const char * json_type_name_map_get_diagram_tags ( const json_type_name_map_t *this_,
                                                                 data_diagram_flag_t diagram_flag
                                                               );

/*!
 *  \brief gets the tag name of the data_diagramelement_flag_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_flag flags for which to select the name
 *  \return tag names of diagramelement_flag id; empty string if the id is unknown
 */
static inline const char * json_type_name_map_get_diagramelement_tags ( const json_type_name_map_t *this_,
                                                                        data_diagramelement_flag_t diagramelement_flag
                                                                      );

#include "json_type_name_map.inl"

#endif  /* JSON_TYPE_NAME_MAP_H */


/*
Copyright 2022-2023 Andreas Warnke

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
