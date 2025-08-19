/* File: json_type_name_map.h; Copyright and License: see below */

#ifndef JSON_TYPE_NAME_MAP_H
#define JSON_TYPE_NAME_MAP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief maps type ids to type names (for the model)
 *         and flag bitsets to tag name lists (for the diagrams)
 */

#include "entity/data_diagram.h"
#include "entity/data_diagram_type.h"
#include "entity/data_diagramelement.h"
#include "entity/data_classifier.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship.h"
#include "entity/data_relationship_type.h"
#include "entity/data_diagramelement_flag.h"
#include "entity/data_diagram_flag.h"

/*!
 *  \brief attributes of the json_type_name_map
 */
struct json_type_name_map_struct {
    const char * classifier_types[ DATA_CLASSIFIER_TYPE_COUNT + 1 ]; /* list of type names, NULL terminated. */
    const char * feature_types[ DATA_FEATURE_TYPE_COUNT + 1 ]; /* list of type names, NULL terminated. */
    const char * relationship_types[ DATA_RELATIONSHIP_TYPE_COUNT + 1 ]; /* list of type names, NULL terminated. */
    const char * diagram_types[ DATA_DIAGRAM_TYPE_COUNT + 1 ]; /* list of type names, NULL terminated. */
    const char * diagram_tags[4]; /* list of type names, NULL terminated. */
    const char * diagramelement_tags[10]; /* list of type names, NULL terminated. */
};

typedef struct json_type_name_map_struct json_type_name_map_t;

/*!
 *  \brief initializes the json_type_name_map
 *
 *  \param this_ pointer to own object attributes
 */
void json_type_name_map_init( json_type_name_map_t *this_ );

/*!
 *  \brief destroys the json_type_name_map
 *
 *  \param this_ pointer to own object attributes
 */
void json_type_name_map_destroy( json_type_name_map_t *this_ );

/*!
 *  \brief gets the type name of the data_classifier_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type id for which to select the name
 *  \return type name of classifier_type id; empty string if the id is unknown
 */
const char * json_type_name_map_get_classifier_type ( const json_type_name_map_t *this_,
                                                      data_classifier_type_t classifier_type
                                                    );

/*!
 *  \brief gets the list of all possible classifier type names.
 *
 *  \param this_ pointer to own object attributes
 *  \return list of type names, NULL terminated.
 */
const char *const * json_type_name_map_get_classifier_types_list ( const json_type_name_map_t *this_ );

/*!
 *  \brief gets the type name of the data_feature_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type id for which to select the name
 *  \return type name of feature_type id; empty string if the id is unknown
 */
const char * json_type_name_map_get_feature_type ( const json_type_name_map_t *this_,
                                                   data_feature_type_t feature_type
                                                 );

/*!
 *  \brief gets the list of all possible feature type names.
 *
 *  \param this_ pointer to own object attributes
 *  \return list of type names, NULL terminated.
 */
const char *const * json_type_name_map_get_feature_types_list ( const json_type_name_map_t *this_ );

/*!
 *  \brief gets the type name of the data_relationship_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param rel_type id for which to select the name
 *  \return type name of rel_type id; empty string if the id is unknown
 */
const char * json_type_name_map_get_relationship_type ( const json_type_name_map_t *this_,
                                                        data_relationship_type_t rel_type
                                                      );

/*!
 *  \brief gets the list of all possible relationship type names.
 *
 *  \param this_ pointer to own object attributes
 *  \return list of type names, NULL terminated.
 */
const char *const * json_type_name_map_get_relationship_types_list ( const json_type_name_map_t *this_ );

/*!
 *  \brief gets the type name of the data_diagram_type_t id.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type id for which to select the name
 *  \return type name of diagram_type id; empty string if the id is unknown
 */
const char * json_type_name_map_get_diagram_type ( const json_type_name_map_t *this_,
                                                   data_diagram_type_t diagram_type
                                                 );

/*!
 *  \brief gets the list of all possible diagram type names.
 *
 *  \param this_ pointer to own object attributes
 *  \return list of type names, NULL terminated.
 */
const char *const * json_type_name_map_get_diagram_types_list ( const json_type_name_map_t *this_ );

/*!
 *  \brief gets the tag name of the data_diagram_flag_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_flag flags for which to select the name
 *  \return tag names of diagram_flag id; empty string if the id is unknown
 */
const char * json_type_name_map_get_diagram_tags ( const json_type_name_map_t *this_,
                                                   data_diagram_flag_t diagram_flag
                                                 );

/*!
 *  \brief gets the list of all possible diagram flag names.
 *
 *  \param this_ pointer to own object attributes
 *  \return list of type names, NULL terminated.
 */
const char *const * json_type_name_map_get_diagram_tags_list ( const json_type_name_map_t *this_ );

/*!
 *  \brief gets the tag name of the data_diagramelement_flag_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_flag flags for which to select the name
 *  \return tag names of diagramelement_flag id; empty string if the id is unknown
 */
const char * json_type_name_map_get_diagramelement_tags ( const json_type_name_map_t *this_,
                                                          data_diagramelement_flag_t diagramelement_flag
                                                        );
/*!
 *  \brief gets the list of all possible diagramelement flag names.
 *
 *  \param this_ pointer to own object attributes
 *  \return tag names of diagramelement_flag id; empty string if the id is unknown
 */
const char *const * json_type_name_map_get_diagramelement_tags_list ( const json_type_name_map_t *this_ );

#endif  /* JSON_TYPE_NAME_MAP_H */


/*
Copyright 2022-2025 Andreas Warnke

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
