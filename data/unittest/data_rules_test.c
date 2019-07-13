/* File: data_rules_test.c; Copyright and License: see below */

#include "data_rules_test.h"
#include "set/data_visible_set.h"
#include "data_rules.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_data_rules_filter_scenarios(void);
static void test_data_rules_filter_box_and_list(void);
static void test_data_rules_filter_standard(void);
static void test_data_rules_filter_illegal(void);
static data_visible_set_t* init_test_input_data( data_diagram_type_t diag_type );

test_suite_t data_rules_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_rules_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_data_rules_filter_scenarios", &test_data_rules_filter_scenarios );
    test_suite_add_test_case( &result, "test_data_rules_filter_box_and_list", &test_data_rules_filter_box_and_list );
    test_suite_add_test_case( &result, "test_data_rules_filter_standard", &test_data_rules_filter_standard );
    test_suite_add_test_case( &result, "test_data_rules_filter_illegal", &test_data_rules_filter_illegal );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static const int TEST_DIAG_ID=170;
static const int TEST_CLASSIFIER_ID_OFFSET=1000;
static const int TEST_DIAGELE_ID_OFFSET=2000;
static const int TEST_FEATURE_ID_OFFSET=5000;
static const int TEST_RELATION_ID_OFFSET=10000;
static const int TEST_LIFELINE_COUNT=25;
static const int TEST_CLASSIFIER_REF_MOD=32; /* only the first 64 classifiers are referenced, these multiple times */
    
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
    data_error_t data_err;
    static data_visible_set_t test_input_data;

    data_visible_set_init( &test_input_data );

    /* initialize the test_input_data.diagram */
    {

        data_err = data_diagram_init ( &(test_input_data.diagram),
                                       TEST_DIAG_ID,  /* diagram_id */
                                       70,  /* parent_diagram_id */
                                       diag_type,  /* diagram_type */
                                       "diagram_name",
                                       "diagram_description",
                                       32000  /* list_order */
                                     );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );
    }

    /* initialize the test_input_data.visible_classifiers */
    test_input_data.visible_classifier_count = DATA_VISIBLE_SET_MAX_CLASSIFIERS;
    for ( uint_fast32_t c_idx = 0; c_idx < DATA_VISIBLE_SET_MAX_CLASSIFIERS; c_idx ++ )
    {
        data_visible_classifier_t *current = &(test_input_data.visible_classifiers[c_idx]);

        data_visible_classifier_init_empty ( current );

        data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr ( current );
        data_diagramelement_t *diagele = data_visible_classifier_get_diagramelement_ptr ( current );

        const bool with_feat = ( 0 == ((c_idx/2) & 0x00000001) );
        data_err = data_classifier_init ( classifier,
                                          TEST_CLASSIFIER_ID_OFFSET + c_idx/2,  /* id */
                                          with_feat ? DATA_CLASSIFIER_TYPE_UML_CLASS: DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
                                          "stereotype",
                                          "name",
                                          "description",
                                          1000*c_idx,  /* x_order */
                                          -300*c_idx,  /* y_order */
                                          4000*c_idx  /* list_order */
                                        );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( DATA_VISIBLE_SET_MAX_FEATURES >= TEST_LIFELINE_COUNT );
        TEST_ENVIRONMENT_ASSERT( DATA_VISIBLE_SET_MAX_CLASSIFIERS >= TEST_LIFELINE_COUNT );
        const bool with_lifeline = ( c_idx < TEST_LIFELINE_COUNT );
        data_diagramelement_init ( diagele,
                                   TEST_DIAGELE_ID_OFFSET + c_idx,  /* id */
                                   TEST_DIAG_ID,  /* diagram_id */
                                   TEST_CLASSIFIER_ID_OFFSET + c_idx/2,  /* classifier_id */
                                   DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,  /* display_flags */
                                   with_lifeline ? (TEST_FEATURE_ID_OFFSET+c_idx) : DATA_ID_VOID_ID  /* focused_feature_id */
                                 );

        TEST_ENVIRONMENT_ASSERT( data_visible_classifier_is_valid( current ) );
    }

    TEST_ENVIRONMENT_ASSERT( TEST_CLASSIFIER_REF_MOD > TEST_LIFELINE_COUNT );
    TEST_ENVIRONMENT_ASSERT( TEST_CLASSIFIER_REF_MOD+7 < (DATA_VISIBLE_SET_MAX_CLASSIFIERS/2) );

    /* initialize the test_input_data.features */
    test_input_data.feature_count = DATA_VISIBLE_SET_MAX_FEATURES;
    for ( uint_fast32_t f_idx = 0; f_idx < DATA_VISIBLE_SET_MAX_FEATURES; f_idx ++ )
    {
        data_feature_t *current = &(test_input_data.features[f_idx]);

        const bool lifeline = ( f_idx < TEST_LIFELINE_COUNT );
        data_err = data_feature_init ( current,
                                       TEST_FEATURE_ID_OFFSET + f_idx,  /* feature_id */
                                       lifeline ? DATA_FEATURE_TYPE_LIFELINE : DATA_FEATURE_TYPE_OPERATION,
                                       TEST_CLASSIFIER_ID_OFFSET + ( f_idx % TEST_CLASSIFIER_REF_MOD ),  /* classifier_id */
                                       "feature_key",
                                       "feature_value",
                                       "feature_description",
                                       6000*f_idx  /* list_order */
                                     );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_feature_is_valid( current ) );
    }

    /* initialize the test_input_data.relationships */
    test_input_data.relationship_count = DATA_VISIBLE_SET_MAX_RELATIONSHIPS;
    for ( uint_fast32_t r_idx = 0; r_idx < DATA_VISIBLE_SET_MAX_RELATIONSHIPS; r_idx ++ )
    {
        data_relationship_t *current = &(test_input_data.relationships[r_idx]);

        const bool from_feat = ( 0 == (r_idx & 0x00000001) );
        const bool to_feat = ( 0 == (r_idx & 0x00000002) );
        data_err = data_relationship_init ( current,
                                            TEST_RELATION_ID_OFFSET + r_idx,  /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,  /* relationship_main_type */
                                            TEST_CLASSIFIER_ID_OFFSET + ( r_idx % TEST_CLASSIFIER_REF_MOD ),  /* from_classifier_id */
                                            TEST_CLASSIFIER_ID_OFFSET + ( (r_idx+1) % TEST_CLASSIFIER_REF_MOD ),  /* to_classifier_id */
                                            "relationship_name",
                                            "relationship_description",
                                            1500*r_idx,  /* list_order */
                                            from_feat ? (TEST_FEATURE_ID_OFFSET+r_idx) : DATA_ID_VOID_ID,  /* from_feature_id */
                                            to_feat ? (TEST_FEATURE_ID_OFFSET+(r_idx+1)) : DATA_ID_VOID_ID  /* to_feature_id */
                                          );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_relationship_is_valid( current ) );
    }

    data_visible_set_private_update_containment_cache ( &test_input_data );

    TEST_ENVIRONMENT_ASSERT ( data_visible_set_is_valid ( &test_input_data ) );
    return &test_input_data;
}

static void test_data_rules_filter_scenarios(void)
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM );
    
    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;
    
    /* valid feature(lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+3 );
    TEST_ASSERT( show == true );
    
    /* valid feature(lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+4 );
    TEST_ASSERT( show == true );
    
    /* valid feature(non-lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+2 );
    TEST_ASSERT( show == false );
    
    /* valid feature(non-lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+3 );
    TEST_ASSERT( show == false );
    
    /* valid relationship from feature(lifeline) to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+4 );
    TEST_ASSERT( show == true );

    /* valid relationship from classifier to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+5 );
    TEST_ASSERT( show == true );

    /* valid relationship from feature(lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+6 );
    TEST_ASSERT( show == true );

    /* valid relationship from classifier to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+7 );
    TEST_ASSERT( show == false );

    /* valid relationship from feature(non-lifeline) to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+4 );
    TEST_ASSERT( show == false );

    /* valid relationship from classifier to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+5 );
    TEST_ASSERT( show == false );

    /* valid relationship from feature(non-lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+6 );
    TEST_ASSERT( show == false );

    data_rules_destroy ( &testrules );
}

static void test_data_rules_filter_box_and_list(void)
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_BOX_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;

    /* valid feature(lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+3 );
    TEST_ASSERT( show == false );
    
    /* valid feature(lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+4 );
    TEST_ASSERT( show == false );
    
    /* valid feature(non-lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+2 );
    TEST_ASSERT( show == false );
    
    /* valid feature(non-lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+3 );
    TEST_ASSERT( show == false );
    
    /* valid relationship from feature(lifeline) to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+4 );
    TEST_ASSERT( show == false );

    /* valid relationship from classifier to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+5 );
    TEST_ASSERT( show == false );

    /* valid relationship from feature(lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+6 );
    TEST_ASSERT( show == false );

    /* valid relationship from classifier to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+7 );
    TEST_ASSERT( show == false );

    /* valid relationship from feature(non-lifeline) to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+4 );
    TEST_ASSERT( show == false );

    /* valid relationship from classifier to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+5 );
    TEST_ASSERT( show == false );

    /* valid relationship from feature(non-lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+6 );
    TEST_ASSERT( show == false );

    data_rules_destroy ( &testrules );
}

static void test_data_rules_filter_standard(void)
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    bool show;

    /* valid feature(lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+3 );
    TEST_ASSERT( show == false );
    
    /* valid feature(lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+4 );
    TEST_ASSERT( show == false );
    
    /* valid feature(non-lifeline) at decision-node */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+2 );
    TEST_ASSERT( show == false );
    
    /* valid feature(non-lifeline) at class */
    show = data_rules_diagram_shows_feature ( &testrules, test_input_data, TEST_FEATURE_ID_OFFSET+TEST_LIFELINE_COUNT+3 );
    TEST_ASSERT( show == true );
    
    /* valid relationship from feature(lifeline) to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+4 );
    TEST_ASSERT( show == false );

    /* valid relationship from classifier to feature(lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+5 );
    TEST_ASSERT( show == false );

    /* valid relationship from feature(lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+6 );
    TEST_ASSERT( show == false );

    /* valid relationship from classifier to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+7 );
    TEST_ASSERT( show == true );

    /* valid relationship from feature(non-lifeline) to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+4 );
    TEST_ASSERT( show == true );

    /* valid relationship from classifier to feature(non-lifeline) */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+5 );
    TEST_ASSERT( show == true );

    /* valid relationship from feature(non-lifeline) to classifier */
    show = data_rules_diagram_shows_relationship ( &testrules, test_input_data, TEST_RELATION_ID_OFFSET+TEST_CLASSIFIER_REF_MOD+6 );
    TEST_ASSERT( show == true );

    data_rules_destroy ( &testrules );
}

static void test_data_rules_filter_illegal(void)
{
    const data_visible_set_t *test_input_data;
    test_input_data = init_test_input_data( DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM );

    data_rules_t testrules;
    data_rules_init ( &testrules );

    data_rules_destroy ( &testrules );
}


/*
 * Copyright 2019-2019 Andreas Warnke
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
