/* File: entity_data_type_test.c; Copyright and License: see below */

#include "entity_data_type_test.h"
#include "entity/data_type.h"
#include "entity/data_diagram_type.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship_type.h"
#include "entity/data_row.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_normal_use( test_fixture_t *test_env );
static test_case_result_t test_error_cases( test_fixture_t *test_env );

test_suite_t entity_data_type_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_type_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_normal_use", &test_normal_use );
    test_suite_add_test_case( &result, "test_error_cases", &test_error_cases );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_normal_use( test_fixture_t *test_env )
{
    data_type_t my_diagram;
    data_type_init_diagram( &my_diagram, DATA_DIAGRAM_TYPE_LIST );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_type_get_context( &my_diagram ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_LIST, data_type_get_type_as_int( &my_diagram ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_LIST, data_type_get_diagram_type( &my_diagram ) );

    data_type_t my_diagram_2;
    data_type_init_diagram( &my_diagram_2, DATA_DIAGRAM_TYPE_LIST );
    TEST_EXPECT_EQUAL_INT( true, data_type_equals( &my_diagram, &my_diagram_2 ) );
    TEST_EXPECT_EQUAL_INT( true, data_type_equals( &my_diagram_2, &my_diagram_2 ) );

    data_type_t my_classifier;
    data_type_init_classifier( &my_classifier, DATA_CLASSIFIER_TYPE_CLASS );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_type_get_context( &my_classifier ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_CLASS, data_type_get_type_as_int( &my_classifier ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_CLASS, data_type_get_classifier_type( &my_classifier ) );

    data_type_t my_feature;
    data_type_init_feature( &my_feature, DATA_FEATURE_TYPE_PROPERTY );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_type_get_context( &my_feature ) );
    TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_PROPERTY, data_type_get_type_as_int( &my_feature ) );
    TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_PROPERTY, data_type_get_feature_type( &my_feature ) );

    assert( (int)DATA_FEATURE_TYPE_PROPERTY == (int)DATA_DIAGRAM_TYPE_LIST );
    TEST_EXPECT_EQUAL_INT( false, data_type_equals( &my_feature, &my_diagram ) );
    data_type_t my_feature_2;
    data_type_init_feature( &my_feature_2, DATA_FEATURE_TYPE_LIFELINE );
    TEST_EXPECT_EQUAL_INT( false, data_type_equals( &my_feature, &my_feature_2 ) );

    data_type_t my_relationship;
    data_type_init_relationship( &my_relationship, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_RELATIONSHIP, data_type_get_context( &my_relationship ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, data_type_get_type_as_int( &my_relationship ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, data_type_get_relationship_type( &my_relationship ) );

    data_type_destroy( &my_diagram );
    data_type_destroy( &my_diagram_2 );
    data_type_destroy( &my_classifier );
    data_type_destroy( &my_feature );
    data_type_destroy( &my_feature_2 );
    data_type_destroy( &my_relationship );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_error_cases( test_fixture_t *test_env )
{
    data_type_t my_void_1;
    data_type_init_void( &my_void_1 );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_type_get_context( &my_void_1 ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_type_get_type_as_int( &my_void_1 ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_VOID, data_type_get_relationship_type( &my_void_1 ) );

    data_type_t my_void_2;
    data_type_init_void( &my_void_2 );
    TEST_EXPECT_EQUAL_INT( false, data_type_equals( &my_void_1, &my_void_2 ) );

    data_type_t my_diagram;
    data_type_init_diagram( &my_diagram, DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_VOID, data_type_get_classifier_type( &my_diagram ) );
    TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_VOID, data_type_get_feature_type( &my_diagram ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_VOID, data_type_get_relationship_type( &my_diagram ) );

    data_type_t my_classifier;
    data_type_init_classifier( &my_classifier, DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_VOID, data_type_get_diagram_type( &my_classifier ) );

    data_type_destroy ( &my_void_1 );
    data_type_destroy ( &my_void_2 );
    data_type_destroy ( &my_diagram );
    data_type_destroy ( &my_classifier );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2025 Andreas Warnke
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
