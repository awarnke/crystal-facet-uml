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
    const bool belongs = ( data_feature_get_classifier_row( feature ) == data_classifier_get_row( clasfy ) );
    const bool scenario = data_rules_feature_is_scenario_cond( this_, data_feature_get_main_type( feature ) );
    const bool focused = ( data_diagramelement_get_focused_feature_row( diagele ) == data_feature_get_row( feature ) );
    return valid && belongs && ( ( ! scenario ) || ( scenario && focused ) );
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
Copyright 2017-2026 Andreas Warnke

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
