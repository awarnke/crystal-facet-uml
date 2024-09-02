/* File: set_data_search_result_test.c; Copyright and License: see below */

#include "set_data_search_result_test.h"
#include "set/data_search_result.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_data_search_result_classifier( test_fixture_t *test_env );
static test_case_result_t test_data_search_result_feature( test_fixture_t *test_env );
static test_case_result_t test_data_search_result_relationship( test_fixture_t *test_env );
static test_case_result_t test_data_search_result_diagram( test_fixture_t *test_env );

test_suite_t set_data_search_result_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "set_data_search_result_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_data_search_result_classifier", &test_data_search_result_classifier );
    test_suite_add_test_case( &result, "test_data_search_result_feature", &test_data_search_result_feature );
    test_suite_add_test_case( &result, "test_data_search_result_relationship", &test_data_search_result_relationship );
    test_suite_add_test_case( &result, "test_data_search_result_diagram", &test_data_search_result_diagram );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_data_search_result_classifier( test_fixture_t *test_env )
{
    data_search_result_t search_result;

    data_search_result_init_classifier( &search_result, 
                                        (data_row_id_t) 130,  /* match_id */
                                        DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE,
                                        "Power on",
                                        (data_row_id_t) 7  /* diagram_id */
                                      );
    data_id_t match_id = data_search_result_get_match_id( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( &match_id ) );
    TEST_EXPECT_EQUAL_INT( 130, data_id_get_row_id( &match_id ) );
    data_type_t match_type = data_search_result_get_match_type( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_type_get_context( &match_type ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE, data_type_get_type_as_int( &match_type ) );
    data_id_t diag_id = data_search_result_get_diagram_id( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &diag_id ) );
    TEST_EXPECT_EQUAL_INT( 7, data_id_get_row_id( &diag_id ) );
    data_id_t src_id = data_search_result_get_src_classifier_id( &search_result );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &src_id ) );
    data_id_t dst_id = data_search_result_get_dst_classifier_id( &search_result );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &dst_id ) );

    data_search_result_trace( &search_result );

    data_search_result_destroy( &search_result );



    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_search_result_feature( test_fixture_t *test_env )
{
    data_search_result_t search_result;

    data_search_result_init_feature( &search_result, 
                                     (data_row_id_t) 230,  /* match_id */
                                     DATA_FEATURE_TYPE_IN_PORT_PIN,
                                     "DATA_SEARCH_RESULT_MAX_NAME_SIZE 4567890 234567890",
                                     (data_row_id_t) 130,  /* classifier_id */
                                     (data_row_id_t) 7  /* diagram_id */
                                   );

    data_id_t * match_id = data_search_result_get_match_id_ptr( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_id_get_table( match_id ) );
    TEST_EXPECT_EQUAL_INT( 230, data_id_get_row_id( match_id ) );
    data_type_t match_type = data_search_result_get_match_type( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_type_get_context( &match_type ) );
    TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_IN_PORT_PIN, data_type_get_type_as_int( &match_type ) );
    data_id_t * diag_id = data_search_result_get_diagram_id_ptr( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table(diag_id ) );
    TEST_EXPECT_EQUAL_INT( 7, data_id_get_row_id( diag_id ) );
    data_id_t * src_id = data_search_result_get_src_classifier_id_ptr( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( src_id ) );
    TEST_EXPECT_EQUAL_INT( 130, data_id_get_row_id( src_id ) );
    data_id_t * dst_id = data_search_result_get_dst_classifier_id_ptr( &search_result );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( dst_id ) );

    data_search_result_t duplicate;
    data_search_result_copy( &duplicate, &search_result );
    const char *const name = data_search_result_get_match_name_const( &search_result );
    TEST_EXPECT_EQUAL_STRING( "DATA_SEARCH_RESULT_MAX_NAME_SIZE 4567890 234567", name );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_search_result_relationship( test_fixture_t *test_env )
{
    data_search_result_t search_result;

    data_search_result_init_relationship( &search_result, 
                                          (data_row_id_t) 444,  /* match_id */
                                          DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
                                          "aggregates self as part",
                                          (data_row_id_t) 100,  /* from_classifier_id */
                                          (data_row_id_t) 101,  /* to_classifier_id */
                                          (data_row_id_t) 4  /* diagram_id */
                                        );

    const data_id_t *const match_id = data_search_result_get_match_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_RELATIONSHIP, data_id_get_table( match_id ) );
    TEST_EXPECT_EQUAL_INT( 444, data_id_get_row_id( match_id ) );
    data_type_t match_type = data_search_result_get_match_type( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_RELATIONSHIP, data_type_get_context( &match_type ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, data_type_get_type_as_int( &match_type ) );
    const data_id_t *const diag_id = data_search_result_get_diagram_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table(diag_id ) );
    TEST_EXPECT_EQUAL_INT( 4, data_id_get_row_id( diag_id ) );
    const data_id_t *const src_id = data_search_result_get_src_classifier_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( src_id ) );
    TEST_EXPECT_EQUAL_INT( 100, data_id_get_row_id( src_id ) );
    const data_id_t *const dst_id = data_search_result_get_dst_classifier_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( dst_id ) );
    TEST_EXPECT_EQUAL_INT( 101, data_id_get_row_id( dst_id ) );

    data_search_result_destroy( &search_result );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_search_result_diagram( test_fixture_t *test_env )
{
    data_search_result_t search_result;

    data_search_result_init_diagram( &search_result, 
                                     (data_row_id_t) 31,  /* match_id */
                                     DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM,
                                     "power off condition"
                                   );

    const data_id_t *const match_id = data_search_result_get_match_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( match_id ) );
    TEST_EXPECT_EQUAL_INT( 31, data_id_get_row_id( match_id ) );
    data_type_t match_type = data_search_result_get_match_type( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_type_get_context( &match_type ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM, data_type_get_type_as_int( &match_type ) );
    const data_id_t *const diag_id = data_search_result_get_diagram_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( diag_id ) );
    TEST_EXPECT_EQUAL_INT( 31, data_id_get_row_id( diag_id ) );
    const data_id_t *const src_id = data_search_result_get_src_classifier_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( src_id ) );
    const data_id_t *const dst_id = data_search_result_get_dst_classifier_id_const( &search_result );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( dst_id ) );

    data_search_result_destroy( &search_result );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2024 Andreas Warnke
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
