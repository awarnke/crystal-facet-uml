/* File: data_rules.c; Copyright and License: see below */

#include "data_rules.h"
#include "trace.h"
#include "tslog.h"

/* ================================ VISIBLE_SET ================================ */

bool data_rules_diagram_shows_feature ( const data_rules_t *this_, const data_visible_set_t *diagram_set, int64_t feature_id )
{
    TRACE_BEGIN();
    assert( NULL != diagram_set );
    bool result = false;

    /* search objects */
    const data_diagram_t *diag_ptr = data_visible_set_get_diagram_const( diagram_set );
    assert( diag_ptr != NULL );
    assert( data_diagram_is_valid( diag_ptr ) );
    const data_diagram_type_t diagram_type = data_diagram_get_diagram_type ( diag_ptr );

    const data_feature_t *feat_ptr = data_visible_set_get_feature_by_id_const ( diagram_set, feature_id );
    assert( feat_ptr != NULL );
    assert( data_feature_is_valid( feat_ptr ) );
    const int64_t classifier_id = data_feature_get_classifier_id( feat_ptr );
    const data_feature_type_t feature_type = data_feature_get_main_type ( feat_ptr );

    const data_classifier_t *classifier_ptr = data_visible_set_get_classifier_by_id_const ( diagram_set, classifier_id );
    if ( classifier_ptr != NULL )
    {
        assert( data_classifier_is_valid( classifier_ptr ) );
        const data_classifier_type_t classifier_type = data_classifier_get_main_type ( classifier_ptr );
        const bool is_feat_scenario = data_rules_feature_is_scenario_cond ( this_, feature_type );

        if ( is_feat_scenario )
        {
            /* a scenario-typed feature that belongs to a different diagram is always filtered */
            bool is_foreign_scenario = true;
            const uint32_t vc_count = data_visible_set_get_visible_classifier_count ( diagram_set );
            for ( uint32_t vc_idx = 0; vc_idx < vc_count; vc_idx ++ )
            {
                const data_visible_classifier_t *vc_probe = data_visible_set_get_visible_classifier_const ( diagram_set, vc_idx );
                assert ( NULL != vc_probe );
                const data_diagramelement_t *diag_ele = data_visible_classifier_get_diagramelement_const ( vc_probe );
                assert ( NULL != diag_ele );
                const int64_t diag_ele_feat_id = data_diagramelement_get_focused_feature_id( diag_ele );
                if ( feature_id == diag_ele_feat_id )
                {
                    is_foreign_scenario = false;
                    break;
                }
            }

            /* evaluate filter */
            const bool ok_by_diagram = data_rules_diagram_shows_scenario_features ( this_, diagram_type );
            const bool ok_by_scenario = ! is_foreign_scenario;
            result = ok_by_diagram && ok_by_scenario;
        }
        else
        {
            /* evaluate filter */
            const bool ok_by_classifier = data_rules_classifier_has_features ( this_, classifier_type );
            const bool ok_by_diagram = data_rules_diagram_shows_uncond_features ( this_, diagram_type );
            result = ok_by_classifier && ok_by_diagram;
        }
    }
    else
    {
        /* maybe the data_visible_set_t was full and the classifier could not be stored? */
        TSLOG_ANOMALY_INT( "data_visible_set_t contains a feature but not the related classifier.", feature_id );
    }

    TRACE_END();
    return result;
}

bool data_rules_diagram_shows_relationship ( const data_rules_t *this_, const data_visible_set_t *diagram_set, int64_t relationship_id )
{
    TRACE_BEGIN();
    assert( NULL != diagram_set );
    bool result = false;

    /* search objects */
    const data_diagram_t *diag_ptr = data_visible_set_get_diagram_const( diagram_set );
    assert( diag_ptr != NULL );
    assert( data_diagram_is_valid( diag_ptr ) );
    const data_diagram_type_t diagram_type = data_diagram_get_diagram_type ( diag_ptr );

    const data_relationship_t *relation_ptr = data_visible_set_get_relationship_by_id_const ( diagram_set, relationship_id );
    assert( relation_ptr != NULL );
    assert( data_relationship_is_valid( relation_ptr ) );
    const int64_t from_classifier_id = data_relationship_get_from_classifier_id( relation_ptr );
    const int64_t from_feat_id = data_relationship_get_from_feature_id( relation_ptr );
    const int64_t to_classifier_id = data_relationship_get_to_classifier_id( relation_ptr );
    const int64_t to_feat_id = data_relationship_get_to_feature_id( relation_ptr );

    const data_classifier_t *from_classifier_or_null = data_visible_set_get_classifier_by_id_const( diagram_set, from_classifier_id );
    const data_classifier_t *to_classifier_or_null = data_visible_set_get_classifier_by_id_const( diagram_set, to_classifier_id );
    if (( from_classifier_or_null != NULL )&&( to_classifier_or_null != NULL ))
    {
        const data_feature_t *from_feat_or_null = data_visible_set_get_feature_by_id_const( diagram_set, from_feat_id );
        const data_feature_t *to_feat_or_null = data_visible_set_get_feature_by_id_const( diagram_set, to_feat_id );
        const data_feature_type_t from_feature_type = (NULL==from_feat_or_null)
                                                      ? DATA_FEATURE_TYPE_VOID
                                                      : data_feature_get_main_type( from_feat_or_null );
        const data_feature_type_t to_feature_type = (NULL==to_feat_or_null)
                                                    ? DATA_FEATURE_TYPE_VOID
                                                    : data_feature_get_main_type( to_feat_or_null );

        /* evaluate filter */
        const bool is_scenario = data_rules_relationship_is_scenario_cond( this_, from_feature_type, to_feature_type);
        const bool ok_by_diagram = is_scenario
                                   ? data_rules_diagram_shows_scenario_relationships ( this_, diagram_type )
                                   : data_rules_diagram_shows_uncond_relationships ( this_, diagram_type );
        result = ok_by_diagram;
    }
    else
    {
        if (( from_classifier_or_null == NULL )&&( to_classifier_or_null == NULL ))
        {
            /* maybe the data_visible_set_t was full and the classifier could not be stored? */
            TSLOG_ANOMALY_INT( "data_visible_set_t contains a relationship but no related classifier.", relationship_id );
        }
        else
        {
            /* it is normal behavior that only one end of the relation is contained in the data_visible_set_t */
        }
    }

    TRACE_END();
    return result;
}


/*
Copyright 2019-2019 Andreas Warnke

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
