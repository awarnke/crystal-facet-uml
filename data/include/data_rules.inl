/* File: data_rules.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"

static inline void data_rules_init ( data_rules_t *this_ )
{
}

static inline void data_rules_destroy ( data_rules_t *this_ )
{
}

static inline data_classifier_type_t data_rules_get_default_classifier_type ( const data_rules_t *this_, data_diagram_type_t diagram_type )
{
    data_classifier_type_t result;

    switch ( diagram_type )
    {
        case DATA_DIAGRAM_TYPE_LIST:
        {
            result = DATA_CLASSIFIER_TYPE_UML_OBJECT;
        }
        break;

        case DATA_DIAGRAM_TYPE_BOX_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_PACKAGE;
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
            result = DATA_CLASSIFIER_TYPE_UML_USE_CASE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_ACTIVITY;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_STATE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_CLASS;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_CLASS;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_STATE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_NODE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_COMPONENT;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_PART;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_PACKAGE;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_CLASS;
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM:
        {
            result = DATA_CLASSIFIER_TYPE_UML_CLASS;
        }
        break;

        default:
        {
            TSLOG_ERROR("diagram_type out of range in data_rules_get_default_classifier_type");
            result = DATA_CLASSIFIER_TYPE_UML_COMMENT;
        }
        break;
    }

    return result;
}

static inline data_relationship_type_t data_rules_get_default_relationship_type ( const data_rules_t *this_, data_classifier_type_t from_classifier_type )
{
    data_relationship_type_t result;

    switch ( from_classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH;
        }
        break;

        case DATA_CLASSIFIER_TYPE_FEATURE:  /* and */
        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_INCLUDE;  /* include is preferred over DATA_RELATIONSHIP_TYPE_UML_EXTEND (?) */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT;  /* containment is a not-so-nice default type - but nothing else makes sense */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_STATE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW;  /* control is preferred over DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW (?) */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH;  /* a node/component/part communicates with another node */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_DEPLOY;  /* an artifact is deployed onto something, prefferred over DATA_RELATIONSHIP_TYPE_UML_MANIFEST */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_CLASS:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_OBJECT:  /* and */
        {
            result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION;  /* containment is a not-so-nice default type - therefore association is preferred */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
        }
        break;

        default:
        {
            TSLOG_ERROR("data_classifier_type_t out of range in data_rules_get_default_relationship_type");
            result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
        }
        break;
    }

    return result;
}

static inline data_feature_type_t data_rules_get_default_feature_type ( const data_rules_t *this_, data_classifier_type_t parent_classifier_type )
{
    data_feature_type_t result;

    switch ( parent_classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        {
            result = DATA_FEATURE_TYPE_PORT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_FEATURE:  /* and */
        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            result = DATA_FEATURE_TYPE_PORT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_STATE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            result = DATA_FEATURE_TYPE_OPERATION;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            result = DATA_FEATURE_TYPE_PORT;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_CLASS:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_OBJECT:  /* and */
        {
            result = DATA_FEATURE_TYPE_OPERATION;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        default:
        {
            TSLOG_ERROR("data_classifier_type_t out of range in data_rules_get_default_feature_type");
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;
    }

    return result;
}

static inline bool data_rules_has_features ( const data_rules_t *this_, data_classifier_type_t classifier_type )
{
    bool result;

    switch ( classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_FEATURE: /* a feature is an abstract requirement or requirement-group */
        case DATA_CLASSIFIER_TYPE_REQUIREMENT: /* for requirements, there is a predefined set of: id, text, ... */
        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        {
            result = true;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_UML_STATE:
        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        case DATA_CLASSIFIER_TYPE_UML_NODE:
        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        case DATA_CLASSIFIER_TYPE_UML_PART:
        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            result = false;
        }
        break;

        default:
        {
            TSLOG_ERROR("data_classifier_type_t out of range in data_rules_has_features");
            result = false;
        }
        break;
    }

    return result;
}


/*
Copyright 2017-2018 Andreas Warnke

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
