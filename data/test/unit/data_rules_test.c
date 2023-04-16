/* File: data_rules_test.c; Copyright and License: see below */

#include "data_rules_test.h"
#include "set/data_visible_set.h"
#include "data_rules.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_data_rules_filter_scenarios( test_fixture_t *test_env );
static test_case_result_t test_data_rules_filter_box_and_list( test_fixture_t *test_env );
static test_case_result_t test_data_rules_filter_standard( test_fixture_t *test_env );
static test_case_result_t test_data_rules_filter_incomplete( test_fixture_t *test_env );
static data_visible_set_t* init_test_input_data( data_diagram_type_t diag_type );

test_suite_t data_rules_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_rules_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_data_rules_filter_scenarios", &test_data_rules_filter_scenarios );
    test_suite_add_test_case( &result, "test_data_rules_filter_box_and_list", &test_data_rules_filter_box_and_list );
    test_suite_add_test_case( &result, "test_data_rules_filter_standard", &test_data_rules_filter_standard );
    test_suite_add_test_case( &result, "test_data_rules_filter_incomplete", &test_data_rules_filter_incomplete );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static const data_row_id_t TEST_DIAG_ID=170;
static const data_row_id_t TEST_CLASSIFIER_ID_OFFSET=1000;
static const data_row_id_t TEST_DIAGELE_ID_OFFSET=2000;
static const data_row_id_t TEST_FEATURE_ID_OFFSET=5000;
static const data_row_id_t TEST_RELATION_ID_OFFSET=10000;
static const unsigned int TEST_LIFELINE_COUNT=25;
static const unsigned int TEST_LIFELINE_REFS=20;
static const unsigned int TEST_CLASSIFIER_REF_MOD=32; /* only the first 32 classifiers are referenced, these multiple times */
static const data_row_id_t TEST_CLASSIFIER_ID_GAP=9;  /* classifiers have consecutive IDs - except at this position */
static const data_row_id_t TEST_FEATURE_ID_GAP=12;  /* features have consecutive IDs - except at this position */

/* create
 *                                  1 data_diagram_t            with ID TEST_DIAG_ID
 *   DATA_VISIBLE_SET_MAX_CLASSIFIERS data_visible_classifier_t
 * DATA_VISIBLE_SET_MAX_CLASSIFIERS/2 data_classifier_t         starting at ID TEST_CLASSIFIER_ID_OFFSET
 *   DATA_VISIBLE_SET_MAX_CLASSIFIERS data_diagramelement_t     starting at ID TEST_DIAGELE_ID_OFFSET
 *      DATA_VISIBLE_SET_MAX_FEATURES data_feature_t            starting at ID TEST_FEATURE_ID_OFFSET
 * DATA_VISIBLE_SET_MAX_RELATIONSHIPS data_relatoinship_t       starting at ID TEST_RELATION_ID_OFFSET
 */
static data_visible_set_t* init_test_input_data( data_diagram_type_t diag_type )
{
    u8_error_t data_err;
    static data_visible_set_t test_input_data;

    data_visible_set_init( &test_input_data );

    /* initialize the test_input_data.diagram */
    {

        data_err = data_diagram_init ( &(test_input_data.diagram),
                                       TEST_DIAG_ID,  /* diagram_id */
                                       70,  /* parent_diagram_id */
                                       diag_type,  /* diagram_type */
                                       "st_t",  /* stereotype */
                                       "diagram_name",
                                       "diagram_description",
                                       32000,  /* list_order */
                                       DATA_DIAGRAM_FLAG_EMPHASIS,
                                       "fa8800f9-58d8-4d2a-8f09-66a54109b9f4"
                                     );
        TEST_ENVIRONMENT_ASSERT( data_err == U8_ERROR_NONE );
    }

    /* initialize the test_input_data.visible_classifiers */
    test_input_data.visible_classifier_count = DATA_VISIBLE_SET_MAX_CLASSIFIERS;
    for ( uint_fast32_t vc_idx = 0; vc_idx < DATA_VISIBLE_SET_MAX_CLASSIFIERS; vc_idx ++ )
    {
        data_visible_classifier_t *current = &(test_input_data.visible_classifiers[vc_idx]);

        data_visible_classifier_init_empty ( current );

        data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr ( current );
        data_diagramelement_t *diagele = data_visible_classifier_get_diagramelement_ptr ( current );

        data_row_id_t classifier_id = TEST_CLASSIFIER_ID_OFFSET + (vc_idx/2);
        if ( (vc_idx/2) == TEST_CLASSIFIER_ID_GAP ) { classifier_id = classifier_id+1; }
        const bool with_feat = ( 0 == (classifier_id & 0x00000001) );
        data_err = data_classifier_init( classifier,
                                         classifier_id,
                                         with_feat ? DATA_CLASSIFIER_TYPE_CLASS: DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
                                         "stereotype",
                                         "name",
                                         "description",
                                         1000*classifier_id,  /* x_order */
                                         -300*classifier_id,  /* y_order */
                                         4000*classifier_id,  /* list_order */
                                         "0bc0667f-2009-4c12-a30b-975627b19889"
                                       );
        TEST_ENVIRONMENT_ASSERT( data_err == U8_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( DATA_VISIBLE_SET_MAX_FEATURES >= TEST_LIFELINE_COUNT );
        TEST_ENVIRONMENT_ASSERT( TEST_LIFELINE_REFS < TEST_LIFELINE_COUNT );  /* not all lifelines shall be visible */
        TEST_ENVIRONMENT_ASSERT( DATA_VISIBLE_SET_MAX_CLASSIFIERS >= TEST_LIFELINE_COUNT );
        const bool with_lifeline = ( vc_idx < TEST_LIFELINE_REFS );
        data_err = data_diagramelement_init( diagele,
                                             TEST_DIAGELE_ID_OFFSET + vc_idx,  /* id */
                                             TEST_DIAG_ID,
                                             classifier_id,
                                             DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                             with_lifeline ? (TEST_FEATURE_ID_OFFSET+vc_idx) : DATA_ROW_ID_VOID,  /* focused_feature_id */
                                             "6bfe2cbc-498b-4f96-93dd-6293e8ffe443"
                                           );
        TEST_ENVIRONMENT_ASSERT( data_err == U8_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_visible_classifier_is_valid( current ) );
    }

    TEST_ENVIRONMENT_ASSERT( TEST_CLASSIFIER_REF_MOD > TEST_LIFELINE_COUNT );
    TEST_ENVIRONMENT_ASSERT( TEST_CLASSIFIER_REF_MOD+7 < (DATA_VISIBLE_SET_MAX_CLASSIFIERS/2) );

    /* initialize the test_input_data.features */
    test_input_data.feature_count = DATA_VISIBLE_SET_MAX_FEATURES;
    for ( uint_fast32_t f_idx = 0; f_idx < DATA_VISIBLE_SET_MAX_FEATURES; f_idx ++ )
    {
        data_feature_t *current = &(test_input_data.features[f_idx]);

        data_row_id_t feature_id = TEST_FEATURE_ID_OFFSET + f_idx;
        if ( f_idx == TEST_FEATURE_ID_GAP ) { feature_id = feature_id+1; }
        const bool lifeline = ( f_idx < TEST_LIFELINE_COUNT );
        data_err = data_feature_init( current,
                                      feature_id,
                                      lifeline ? DATA_FEATURE_TYPE_LIFELINE : DATA_FEATURE_TYPE_OPERATION,
                                      TEST_CLASSIFIER_ID_OFFSET + ( f_idx % TEST_CLASSIFIER_REF_MOD ),  /* classifier_id */
                                      "feature_key",
                                      "feature_value",
                                      "feature_description",
                                      6000*f_idx,  /* list_order */
                                      "79464edf-be17-4497-98ec-175913eff4e5"
                                    );
        TEST_ENVIRONMENT_ASSERT( data_err == U8_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_feature_is_valid( current ) );
    }

    /* initialize the test_input_data.relationships */
    test_input_data.relationship_count = DATA_VISIBLE_SET_MAX_RELATIONSHIPS;
    for ( uint_fast32_t r_idx = 0; r_idx < DATA_VISIBLE_SET_MAX_RELATIONSHIPS; r_idx ++ )
    {
        data_relationship_t *current = &(test_input_data.relationships[r_idx]);

        const bool from_feat = ( 0 == (r_idx & 0x00000001) )||( r_idx == TEST_FEATURE_ID_GAP );
        const bool to_feat = ( 0 == (r_idx & 0x00000002) )||( (r_idx+1) == TEST_FEATURE_ID_GAP );
        data_err = data_relationship_init( current,
                                           TEST_RELATION_ID_OFFSET + r_idx,  /* relationship_id */
                                           DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
                                           TEST_CLASSIFIER_ID_OFFSET + ( r_idx % TEST_CLASSIFIER_REF_MOD ),  /* from_classifier_id */
                                           TEST_CLASSIFIER_ID_OFFSET + ( (r_idx+1) % TEST_CLASSIFIER_REF_MOD ),  /* to_classifier_id */
                                           "st_t",  /* stereotype */
                                           "relationship_name",
                                           "relationship_description",
                                           1500*r_idx,  /* list_order */
                                           from_feat ? (TEST_FEATURE_ID_OFFSET+r_idx) : DATA_ROW_ID_VOID,  /* from_feature_id */
                                           to_feat ? (TEST_FEATURE_ID_OFFSET+(r_idx+1)) : DATA_ROW_ID_VOID,  /* to_feature_id */
                                           "dc1dc264-e50b-4140-bfb7-591977e21a37"
                                         );
        TEST_ENVIRONMENT_ASSERT( data_err == U8_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_relationship_is_valid( current ) );
    }

    data_visible_set_private_update_containment_cache ( &test_input_data );

    TEST_ENVIRONMENT_ASSERT ( data_visible_set_is_valid ( &test_input_data ) );
    return &test_input_data;
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_rules_filter_scenarios( test_fixture_t *test_env )
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;

    /* valid feature(lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+3 );
    TEST_EXPECT( show == true );

    /* valid feature(lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+4 );
    TEST_EXPECT( show == true );

    /* valid feature(lifeline) which is not referenced */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_REFS );
    TEST_EXPECT( show == false );

    /* valid feature(non-lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+2 );
    TEST_EXPECT( show == false );

    /* valid feature(non-lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+3 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(lifeline) to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+4 );
    TEST_EXPECT( show == true );

    /* valid relationship from classifier to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+5 );
    TEST_EXPECT( show == true );

    /* valid relationship from feature(lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+6 );
    TEST_EXPECT( show == true );

    /* valid relationship from classifier to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+7 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(non-lifeline) to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+4 );
    TEST_EXPECT( show == false );

    /* valid relationship from classifier to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+5 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(non-lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+6 );
    TEST_EXPECT( show == false );

    data_rules_destroy ( &testrules );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_rules_filter_box_and_list( test_fixture_t *test_env )
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_BOX_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;

    /* valid feature(lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+3 );
    TEST_EXPECT( show == false );

    /* valid feature(lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+4 );
    TEST_EXPECT( show == false );

    /* valid feature(lifeline) which is not referenced */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_REFS );
    TEST_EXPECT( show == false );

    /* valid feature(non-lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+2 );
    TEST_EXPECT( show == false );

    /* valid feature(non-lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+3 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(lifeline) to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+4 );
    TEST_EXPECT( show == false );

    /* valid relationship from classifier to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+5 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+6 );
    TEST_EXPECT( show == false );

    /* valid relationship from classifier to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+7 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(non-lifeline) to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+4 );
    TEST_EXPECT( show == false );

    /* valid relationship from classifier to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+5 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(non-lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+6 );
    TEST_EXPECT( show == false );

    data_rules_destroy ( &testrules );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_rules_filter_standard( test_fixture_t *test_env )
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;

    /* valid feature(lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+3 );
    TEST_EXPECT( show == false );

    /* valid feature(lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+4 );
    TEST_EXPECT( show == false );

    /* valid feature(lifeline) which is not referenced */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_REFS );
    TEST_EXPECT( show == false );

    /* valid feature(non-lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+2 );
    TEST_EXPECT( show == false );

    /* valid feature(non-lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+3 );
    TEST_EXPECT( show == true );

    /* valid relationship from feature(lifeline) to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+4 );
    TEST_EXPECT( show == false );

    /* valid relationship from classifier to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+5 );
    TEST_EXPECT( show == false );

    /* valid relationship from feature(lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+6 );
    TEST_EXPECT( show == false );

    /* valid relationship from classifier to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+7 );
    TEST_EXPECT( show == true );

    /* valid relationship from feature(non-lifeline) to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+4 );
    TEST_EXPECT( show == true );

    /* valid relationship from classifier to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+5 );
    TEST_EXPECT( show == true );

    /* valid relationship from feature(non-lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+6 );
    TEST_EXPECT( show == true );

    data_rules_destroy ( &testrules );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_rules_filter_incomplete( test_fixture_t *test_env )
{
    data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;

    /* feature referencing unknown parent */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_CLASSIFIER_ID_GAP );
    TEST_EXPECT( show == false );

    /* relationship referencing unknown from classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_ID_GAP );
    TEST_EXPECT( show == false );

    /* relationship referencing unknown to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_ID_GAP+1 );
    TEST_EXPECT( show == false );

    /* relationship referencing unknown from feature */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_FEATURE_ID_GAP );
    TEST_EXPECT( show == false );

    /* relationship referencing unknown to feature */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_FEATURE_ID_GAP+1 );
    TEST_EXPECT( show == false );

    data_rules_destroy ( &testrules );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2019-2023 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
