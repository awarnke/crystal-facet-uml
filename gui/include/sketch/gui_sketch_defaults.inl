/* File: gui_sketch_defaults.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void gui_sketch_defaults_init ( gui_sketch_defaults_t *this_ )
{
}

static inline void gui_sketch_defaults_destroy ( gui_sketch_defaults_t *this_ )
{
}

static inline data_classifier_type_t gui_sketch_defaults_get_classifier_type ( const gui_sketch_defaults_t *this_,
                                                                               data_diagram_type_t diagram_type )
{
    data_classifier_type_t result;

    switch ( diagram_type )
    {
        case DATA_DIAGRAM_TYPE_LIST:
        {
            result = DATA_CLASSIFIER_TYPE_OBJECT;
        }
        break;

        case DATA_DIAGRAM_TYPE_BOX_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_PACKAGE;
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM:  /* and */
        case DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_BLOCK;
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_BLOCK;
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_REQUIREMENT;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_USE_CASE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_ACTIVITY;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_STATE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_CLASS;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_CLASS;
        }
        break;

        case DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_STATE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_NODE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_COMPONENT;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_PART;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_PACKAGE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_CLASS;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_STEREOTYPE;
        }
        break;

        default:
        {
            U8_LOG_ANOMALY("diagram_type out of range in gui_sketch_defaults_get_classifier_type");
            result = DATA_CLASSIFIER_TYPE_COMMENT;
            /* this case can happen if a model file of a new cfu version is opened with an older version of cfu */
        }
        break;
    }

    return result;
}

static inline data_relationship_type_t gui_sketch_defaults_get_relationship_type ( const gui_sketch_defaults_t *this_,
                                                                                   data_classifier_type_t from_classifier_type,
                                                                                   data_feature_type_t from_feature_type )
{
    data_relationship_type_t result;

    if ( from_feature_type == DATA_FEATURE_TYPE_LIFELINE )
    {
        result = DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL;
    }
    else
    {
        switch ( from_classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_BLOCK:  /* and */
            case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH;
            }
            break;

            case DATA_CLASSIFIER_TYPE_REQUIREMENT:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_TRACE;
            }
            break;

            case DATA_CLASSIFIER_TYPE_ACTOR:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;
            }
            break;

            case DATA_CLASSIFIER_TYPE_USE_CASE:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_INCLUDE;  /* a simple include is preferred over the more complex DATA_RELATIONSHIP_TYPE_UML_EXTEND. */
            }
            break;

            case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
            {
                /* containment would make sense here but is not intuitive from a useability point of view */
                result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;
            }
            break;

            case DATA_CLASSIFIER_TYPE_ACTIVITY:  /* and */
            case DATA_CLASSIFIER_TYPE_STATE:  /* and */
            case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:  /* and */
            case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW;  /* control is preferred over DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW (?) */
            }
            break;

            case DATA_CLASSIFIER_TYPE_NODE:  /* and */
            case DATA_CLASSIFIER_TYPE_COMPONENT:  /* and */
            case DATA_CLASSIFIER_TYPE_PART:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH;  /* a node/component/part communicates with another node */
            }
            break;

            case DATA_CLASSIFIER_TYPE_ARTIFACT:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_DEPLOY;  /* an artifact is deployed onto something, prefferred over DATA_RELATIONSHIP_TYPE_UML_MANIFEST */
            }
            break;

            case DATA_CLASSIFIER_TYPE_INTERFACE:  /* and */
            case DATA_CLASSIFIER_TYPE_CLASS:  /* and */
            case DATA_CLASSIFIER_TYPE_OBJECT:  /* and */
            {
                result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;
            }
            break;

            case DATA_CLASSIFIER_TYPE_PACKAGE:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;  /* containment is a not-so-nice default type - therefore association is preferred */
            }
            break;

            case DATA_CLASSIFIER_TYPE_COMMENT:  /* and */
            case DATA_CLASSIFIER_TYPE_IMAGE:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
            }
            break;

            case DATA_CLASSIFIER_TYPE_STEREOTYPE:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION;
            }
            break;

            default:
            {
                U8_LOG_ANOMALY("data_classifier_type_t out of range in gui_sketch_defaults_get_relationship_type");
                result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
                /* this case can happen if a model file of a new cfu version is opened with an older version of cfu */
            }
            break;
        }
    }

    return result;
}

static inline data_feature_type_t gui_sketch_defaults_get_feature_type ( const gui_sketch_defaults_t *this_,
                                                                         data_classifier_type_t parent_classifier_type )
{
    data_feature_type_t result;

    switch ( parent_classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:  /* and */
        case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
        {
            result = DATA_FEATURE_TYPE_PORT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            result = DATA_FEATURE_TYPE_TAGGED_VALUE;
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTOR:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_USE_CASE:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
        {
            result = DATA_FEATURE_TYPE_PORT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTIVITY:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            result = DATA_FEATURE_TYPE_OUT_PORT_PIN; /* Out Pin */
        }
        break;

        case DATA_CLASSIFIER_TYPE_STATE:
        {
            result = DATA_FEATURE_TYPE_EXIT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:  /* and */
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            result = DATA_FEATURE_TYPE_OPERATION;
        }
        break;

        case DATA_CLASSIFIER_TYPE_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_COMPONENT:  /* and */
        case DATA_CLASSIFIER_TYPE_PART:
        {
            result = DATA_FEATURE_TYPE_PORT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_ARTIFACT:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_INTERFACE:  /* and */
        case DATA_CLASSIFIER_TYPE_CLASS:  /* and */
        case DATA_CLASSIFIER_TYPE_OBJECT:  /* and */
        {
            result = DATA_FEATURE_TYPE_OPERATION;
        }
        break;

        case DATA_CLASSIFIER_TYPE_PACKAGE:
        {
            result = DATA_FEATURE_TYPE_TAGGED_VALUE;
        }
        break;

        case DATA_CLASSIFIER_TYPE_COMMENT:
        case DATA_CLASSIFIER_TYPE_IMAGE:
        {
            result = DATA_FEATURE_TYPE_TAGGED_VALUE;
        }
        break;

        case DATA_CLASSIFIER_TYPE_STEREOTYPE:
        {
             /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.3.4 */
             /* a property of a stereotype-class is a tagged-value definition */
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        default:
        {
            U8_LOG_ANOMALY("data_classifier_type_t out of range in gui_sketch_defaults_get_feature_type");
            result = DATA_FEATURE_TYPE_TAGGED_VALUE;
            /* this case can happen if a model file of a new cfu version is opened with an older version of cfu */
        }
        break;
    }

    return result;
}


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
