/* File: data_rules.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>

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
            result = DATA_CLASSIFIER_TYPE_CLASS;
        }
        break;

        default:
        {
            TSLOG_ERROR("diagram_type out of range in data_rules_get_default_classifier_type");
            result = DATA_CLASSIFIER_TYPE_COMMENT;
            assert(false);
        }
        break;
    }

    return result;
}

static inline data_relationship_type_t data_rules_get_default_relationship_type ( const data_rules_t *this_, 
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
                result = DATA_RELATIONSHIP_TYPE_UML_INCLUDE;  /* include is preferred over DATA_RELATIONSHIP_TYPE_UML_EXTEND (?) */
            }
            break;

            case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT;  /* containment is a not-so-nice default type - but nothing else makes sense */
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

            case DATA_CLASSIFIER_TYPE_COMMENT:
            {
                result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
            }
            break;

            default:
            {
                TSLOG_ERROR("data_classifier_type_t out of range in data_rules_get_default_relationship_type");
                result = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
                assert(false);
            }
            break;
        }
    }

    return result;
}

static inline data_feature_type_t data_rules_get_default_feature_type ( const data_rules_t *this_, data_classifier_type_t parent_classifier_type )
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
            result = DATA_FEATURE_TYPE_PROPERTY;
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
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        case DATA_CLASSIFIER_TYPE_COMMENT:
        {
            result = DATA_FEATURE_TYPE_PROPERTY;
        }
        break;

        default:
        {
            TSLOG_ERROR("data_classifier_type_t out of range in data_rules_get_default_feature_type");
            result = DATA_FEATURE_TYPE_PROPERTY;
            assert(false);
        }
        break;
    }

    return result;
}

/* ================================ DIAGRAM ================================ */

static inline bool data_rules_diagram_is_scenario ( const data_rules_t *this_, data_diagram_type_t diagram_type )
{
    bool result;
    result = (( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
             || ( diagram_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
             || ( diagram_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
             || ( diagram_type == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ));
    return result;
}

/* ================================ CLASSIFIER ================================ */

/* ================================ FEATURE ================================ */

static inline bool data_rules_classifier_has_uncond_features ( const data_rules_t *this_, data_classifier_type_t classifier_type )
{
    bool result;

    switch ( classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
        case DATA_CLASSIFIER_TYPE_REQUIREMENT: /* for requirements, there is a predefined set of: id, text, ... */
        case DATA_CLASSIFIER_TYPE_INTERFACE:
        case DATA_CLASSIFIER_TYPE_CLASS:
        case DATA_CLASSIFIER_TYPE_OBJECT:
        case DATA_CLASSIFIER_TYPE_USE_CASE:
        case DATA_CLASSIFIER_TYPE_NODE:
        case DATA_CLASSIFIER_TYPE_COMPONENT:
        case DATA_CLASSIFIER_TYPE_PART:
        case DATA_CLASSIFIER_TYPE_PACKAGE:
        case DATA_CLASSIFIER_TYPE_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_STATE:
        {
            result = true;
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTOR:
        case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
        case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:
        case DATA_CLASSIFIER_TYPE_COMMENT:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            result = false;
        }
        break;

        default:
        {
            TSLOG_ERROR("data_classifier_type_t out of range in data_rules_has_features");
            result = false;
            assert(false);
        }
        break;
    }

    return result;
}

static inline bool data_rules_feature_is_scenario_cond ( const data_rules_t *this_, data_feature_type_t feature_type )
{
    bool result;
    result = ( feature_type == DATA_FEATURE_TYPE_LIFELINE );
    return result;
}

static inline bool data_rules_diagram_shows_uncond_features ( const data_rules_t *this_, data_diagram_type_t diagram_type )
{
    bool hide;
    hide = (( diagram_type == DATA_DIAGRAM_TYPE_LIST )
           || ( diagram_type == DATA_DIAGRAM_TYPE_BOX_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ));
    return ( ! hide );
}

static inline bool data_rules_diagram_shows_scenario_features ( const data_rules_t *this_, data_diagram_type_t diagram_type )
{
    bool show;
    show = (( diagram_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ));
    return show;
}

static inline bool data_rules_vis_classifier_has_feature ( const data_rules_t *this_,
                                                           const data_visible_classifier_t *vis_classifier,
                                                           const data_feature_t *feature )
{
    assert( vis_classifier != NULL );
    assert( feature != NULL );
    const bool valid = data_visible_classifier_is_valid ( vis_classifier ) && data_feature_is_valid ( feature );
    const data_classifier_t *clasfy = data_visible_classifier_get_classifier_const ( vis_classifier );
    const data_diagramelement_t *diagele = data_visible_classifier_get_diagramelement_const ( vis_classifier );
    assert( clasfy != NULL );
    assert( diagele != NULL );
    const bool belongs = ( data_feature_get_classifier_id( feature ) == data_classifier_get_id( clasfy ) );
    const bool scenario = data_rules_feature_is_scenario_cond( this_, data_feature_get_main_type( feature ) );
    const bool focused = ( data_diagramelement_get_focused_feature_id( diagele ) == data_feature_get_id( feature ) );
    const bool visible = data_rules_classifier_has_uncond_features( this_, data_classifier_get_main_type( clasfy ) );
    return valid && belongs && (((!scenario)&&visible) || (scenario&&focused));
}

/* ================================ RELATIONSHIP ================================ */

static inline bool data_rules_relationship_is_scenario_cond ( const data_rules_t *this_,
                                                              data_feature_type_t from_feature_type,
                                                              data_feature_type_t to_feature_type
                                                            )
{
    bool result;
    result = ( from_feature_type == DATA_FEATURE_TYPE_LIFELINE )
             || ( to_feature_type == DATA_FEATURE_TYPE_LIFELINE );
    return result;
}

static inline bool data_rules_diagram_shows_uncond_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type )
{
    bool hide;
    hide = (( diagram_type == DATA_DIAGRAM_TYPE_LIST )
           || ( diagram_type == DATA_DIAGRAM_TYPE_BOX_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ));
    return ( ! hide );
}

static inline bool data_rules_diagram_shows_scenario_relationships ( const data_rules_t *this_, data_diagram_type_t diagram_type )
{
    bool show;
    show = (( diagram_type == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
           || ( diagram_type == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ));
    return show;
}

static inline bool data_rules_is_relationship_compliant ( const data_rules_t *this_,
                                                          data_relationship_type_t relation_type,
                                                          data_classifier_type_t from_c_type,
                                                          data_feature_type_t from_f_type,
                                                          data_classifier_type_t to_c_type,
                                                          data_feature_type_t to_f_type )
{
    bool comply;
    comply = true;  /* TODO */
    return comply;
}


/*
Copyright 2017-2020 Andreas Warnke

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
