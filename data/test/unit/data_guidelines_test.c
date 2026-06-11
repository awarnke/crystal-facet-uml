/* File: data_guidelines_test.c; Copyright and License: see below */

#include "data_guidelines_test.h"
#include "data_guidelines.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_data_guidelines_classifier_is_scenario( test_fixture_t *test_env );
static test_case_result_t test_data_guidelines_feature_value( test_fixture_t *test_env );

test_suite_t data_guidelines_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_guidelines_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_data_guidelines_classifier_is_scenario", &test_data_guidelines_classifier_is_scenario );
    test_suite_add_test_case( &result, "test_data_guidelines_feature_value", &test_data_guidelines_feature_value );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_data_guidelines_classifier_is_scenario( test_fixture_t *test_env )
{
    data_guidelines_t testguideline;
    data_guidelines_init ( &testguideline );

    const bool result_1
        = data_guidelines_classifier_has_scenario_semantics( &testguideline,
                                                             DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                                             DATA_CLASSIFIER_TYPE_INTERACTION_USE
                                                           );
    TEST_EXPECT( result_1 == false );

    const bool result_2
        = data_guidelines_classifier_has_scenario_semantics( &testguideline,
                                                             DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                                             DATA_CLASSIFIER_TYPE_ACTIVITY
                                                           );
    TEST_EXPECT( result_2 == true );

    data_guidelines_destroy ( &testguideline );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_data_guidelines_feature_value( test_fixture_t *test_env )
{
    data_guidelines_t testguideline;
    data_guidelines_init ( &testguideline );

    const bool result_1
        = data_guidelines_feature_value_is_stereotype( &testguideline, DATA_FEATURE_TYPE_TAGGED_VALUE );

    TEST_EXPECT( result_1 == false );

    const bool result_2
        = data_guidelines_feature_value_is_stereotype( &testguideline, DATA_FEATURE_TYPE_IN_PORT_PIN );

    TEST_EXPECT( result_2 == true );

    data_guidelines_destroy ( &testguideline );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2026-2026 Andreas Warnke
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
