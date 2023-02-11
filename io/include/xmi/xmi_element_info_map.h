/* File: xmi_element_info_map.h; Copyright and License: see below */

#ifndef XMI_ELEMENT_INFO_MAP_H
#define XMI_ELEMENT_INFO_MAP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Lists static data on xmi type names, type hierarchies and property names
 */

#include "xmi/xmi_spec.h"
#include "xmi/xmi_element_info.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include <stdbool.h>

/*!
 *  \brief defines the array indices of uml elements
 */
enum xmi_element_info_map_index_enum {
    /* ================================ CLASSIFIER ================================ */
    XMI_ELEMENT_INFO_MAP_INDEX_BLOCK,
    XMI_ELEMENT_INFO_MAP_INDEX_CONSTRAINT_BLOCK,
    XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT,
    XMI_ELEMENT_INFO_MAP_INDEX_ACTOR,
    XMI_ELEMENT_INFO_MAP_INDEX_USE_CASE,
    XMI_ELEMENT_INFO_MAP_INDEX_SUBSYSTEM,
    XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY,
    XMI_ELEMENT_INFO_MAP_INDEX_STRUCTURED_ACTIVITY_NODE,  /*!< Nested activities */
    XMI_ELEMENT_INFO_MAP_INDEX_STATEMACHINE,  /*!< The outermost state is a statemachine */
    XMI_ELEMENT_INFO_MAP_INDEX_STATE,
    XMI_ELEMENT_INFO_MAP_INDEX_INTERACTION_USE,  /*!< equals diagram reference */
    XMI_ELEMENT_INFO_MAP_INDEX_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT,
    XMI_ELEMENT_INFO_MAP_INDEX_INTERFACE,
    XMI_ELEMENT_INFO_MAP_INDEX_PACKAGE,
    XMI_ELEMENT_INFO_MAP_INDEX_CLASS,
    XMI_ELEMENT_INFO_MAP_INDEX_ARTIFACT,
    XMI_ELEMENT_INFO_MAP_INDEX_COMMENT,
    XMI_ELEMENT_INFO_MAP_INDEX_STEREOTYPE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_INTERRUPTABLE_REGION,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_INITIAL_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_INITIAL_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_FINAL_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_FINAL_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_FORK_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_JOIN_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_DECISION_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_DECISION_NODE,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_SHALLOW_HISTORY,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_DEEP_HISTORY,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_PARTITION,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_EVENT,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_TIME_EVENT,
    XMI_ELEMENT_INFO_MAP_INDEX_DYN_SEND_SIGNAL,
    XMI_ELEMENT_INFO_MAP_INDEX_INTERACTION,

    /* ================================ FEATURE ================================ */
    XMI_ELEMENT_INFO_MAP_INDEX_PROPERTY,
    XMI_ELEMENT_INFO_MAP_INDEX_OPERATION,
    XMI_ELEMENT_INFO_MAP_INDEX_PORT,
    XMI_ELEMENT_INFO_MAP_INDEX_INPUT_PIN,
    XMI_ELEMENT_INFO_MAP_INDEX_OUTPUT_PIN,
    XMI_ELEMENT_INFO_MAP_INDEX_STATE_ENTRY,
    XMI_ELEMENT_INFO_MAP_INDEX_STATE_EXIT,
    XMI_ELEMENT_INFO_MAP_INDEX_LIFELINE,
    XMI_ELEMENT_INFO_MAP_INDEX_PROVIDED_INTERFACE,
    XMI_ELEMENT_INFO_MAP_INDEX_REQUIRED_INTERFACE,
    XMI_ELEMENT_INFO_MAP_INDEX_TAGGED_VALUE,

    /* ================================ RELATIONSHIP ================================ */
    XMI_ELEMENT_INFO_MAP_INDEX_DEPENDENCY,
    XMI_ELEMENT_INFO_MAP_INDEX_ASSOCIATION,
    XMI_ELEMENT_INFO_MAP_INDEX_AGGREGATION,
    XMI_ELEMENT_INFO_MAP_INDEX_COMPOSITION,
    XMI_ELEMENT_INFO_MAP_INDEX_GENERALIZATION,
    XMI_ELEMENT_INFO_MAP_INDEX_REALIZATION,
    XMI_ELEMENT_INFO_MAP_INDEX_ASYNC_CALL,
    XMI_ELEMENT_INFO_MAP_INDEX_SYNC_CALL,
    XMI_ELEMENT_INFO_MAP_INDEX_RETURN_CALL,
    XMI_ELEMENT_INFO_MAP_INDEX_COMMUNICATION_PATH,
    XMI_ELEMENT_INFO_MAP_INDEX_CONTROL_FLOW,
    XMI_ELEMENT_INFO_MAP_INDEX_OBJECT_FLOW,
    XMI_ELEMENT_INFO_MAP_INDEX_TRANSITION,
    XMI_ELEMENT_INFO_MAP_INDEX_DEPLOY,
    XMI_ELEMENT_INFO_MAP_INDEX_MANIFEST,
    XMI_ELEMENT_INFO_MAP_INDEX_EXTEND,
    XMI_ELEMENT_INFO_MAP_INDEX_INCLUDE,
    XMI_ELEMENT_INFO_MAP_INDEX_CONTAINMENT,
    XMI_ELEMENT_INFO_MAP_INDEX_REFINE,
    XMI_ELEMENT_INFO_MAP_INDEX_TRACE,
};

typedef enum xmi_element_info_map_index_enum xmi_element_info_map_index_t;

#define XMI_ELEMENT_INFO_MAP_INDEX_MAX (66)

typedef xmi_element_info_t xmi_element_info_map_t[XMI_ELEMENT_INFO_MAP_INDEX_MAX];

extern const xmi_element_info_map_t xmi_element_info_map_standard;

/*!
 *  \brief returns a pointer to the xmi_element_info_t struct identified by classifier_type.
 *
 *  \param this_ pointer to own object attributes; use e.g. the global &xmi_element_info_map_standard
 *  \param parent_type for some classifiers, the result depends on the used context, therefore the parent type is needed to select the xmi_element_info_t
 *  \param classifier_type selects the xmi_element_info_t
 *  \param[out] out_element_info pointer to the selected xmi_element_info_t (or similar xmi_element_info_t in case of unexpected classifier_type)
 *  \return 0 in case of success, -1 if no valid mapping found
 */
static inline int xmi_element_info_map_get_classifier ( const xmi_element_info_map_t *this_,
                                                        data_classifier_type_t parent_type,
                                                        data_classifier_type_t classifier_type,
                                                        const xmi_element_info_t **out_element_info
                                                      );

/*!
 *  \brief returns a pointer to the xmi_element_info_t struct identified by feat_type.
 *
 *  \param this_ pointer to own object attributes; use e.g. the global &xmi_element_info_map_standard
 *  \param parent_type data_classifier_type_t of the parent of which the feature type shall be determined
 *  \param feature_type selects the xmi_element_info_t
 *  \param[out] out_element_info pointer to the selected xmi_element_info_t (or similar xmi_element_info_t in case of unexpected classifier_type)
 *  \return 0 in case of success, -1 if no valid mapping found
 */
static inline int xmi_element_info_map_get_feature ( const xmi_element_info_map_t *this_,
                                                     data_classifier_type_t parent_type,
                                                     data_feature_type_t feature_type,
                                                     const xmi_element_info_t **out_element_info
                                                   );

/*!
 *  \brief returns a pointer to the xmi_element_info_t struct identified by rel_type.
 *
 *  \param this_ pointer to own object attributes; use e.g. the global &xmi_element_info_map_standard
 *  \param statemachine_context for some relationships, the result depends on the used context, e.g. transition in statemachine or control flow in activities
 *  \param rel_type selects the xmi_element_info_t
 *  \param[out] out_element_info pointer to the selected xmi_element_info_t (or similar xmi_element_info_t in case of unexpected classifier_type)
 *  \return 0 in case of success, -1 if no valid mapping found
 */
static inline int xmi_element_info_map_get_relationship ( const xmi_element_info_map_t *this_,
                                                          bool statemachine_context,
                                                          data_relationship_type_t rel_type,
                                                          const xmi_element_info_t **out_element_info
                                                        );

#include "xmi_element_info_map.inl"

#endif  /* XMI_ELEMENT_INFO_MAP_H */


/*
Copyright 2020-2023 Andreas Warnke

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
