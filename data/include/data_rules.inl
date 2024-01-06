/* File: data_rules.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void data_rules_init ( data_rules_t *this_ )
{
    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */
}

static inline void data_rules_destroy ( data_rules_t *this_ )
{
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
        case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
        case DATA_CLASSIFIER_TYPE_COMPONENT:
        case DATA_CLASSIFIER_TYPE_PART:
        case DATA_CLASSIFIER_TYPE_PACKAGE:
        case DATA_CLASSIFIER_TYPE_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        case DATA_CLASSIFIER_TYPE_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_STATE:
        case DATA_CLASSIFIER_TYPE_STEREOTYPE:
        {
            result = true;
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTOR:
        case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:
        case DATA_CLASSIFIER_TYPE_COMMENT:
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
        case DATA_CLASSIFIER_TYPE_IMAGE:
        {
            result = false;
        }
        break;

        default:
        {
            U8_LOG_ANOMALY("data_classifier_type_t out of range in data_rules_has_features");
            result = true;
            /* this case can happen if a model file of a new cfu version is opened with an older version of cfu */
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
    const bool belongs = ( data_feature_get_classifier_row_id( feature ) == data_classifier_get_row_id( clasfy ) );
    const bool scenario = data_rules_feature_is_scenario_cond( this_, data_feature_get_main_type( feature ) );
    const bool focused = ( data_diagramelement_get_focused_feature_row_id( diagele ) == data_feature_get_row_id( feature ) );
    const bool visible = data_rules_classifier_has_uncond_features( this_, data_classifier_get_main_type( clasfy ) );
    return valid && belongs && (((!scenario)&&visible) || (scenario&&focused));
}

static inline bool data_rules_classifier_has_scenario_semantics ( const data_rules_t *this_,
                                                                  data_diagram_type_t diagram_type,
                                                                  data_classifier_type_t classifier_type )
{
    const bool lifeline_has_no_semantics
        = ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_COMMENT )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_IMAGE )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_STEREOTYPE )
        || (( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
        && (( classifier_type == DATA_CLASSIFIER_TYPE_INTERACTION )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )));
    return ( ! lifeline_has_no_semantics);
}

static inline bool data_rules_feature_value_is_stereotype ( const data_rules_t *this_, data_feature_type_t feature_type )
{
    bool has_value;
    has_value = (( feature_type == DATA_FEATURE_TYPE_PROPERTY )
           || ( feature_type == DATA_FEATURE_TYPE_OPERATION )
           || ( feature_type == DATA_FEATURE_TYPE_TAGGED_VALUE ));
    return ( ! has_value );
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

/*
static inline bool data_rules_is_relationship_compliant ( const data_rules_t *this_,
                                                          data_relationship_type_t relation_type,
                                                          data_classifier_type_t from_c_type,
                                                          data_feature_type_t from_f_type,
                                                          data_classifier_type_t to_c_type,
                                                          data_feature_type_t to_f_type )
{
    bool comply;
    comply = true;
    TODO
    return comply;
}
*/


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
