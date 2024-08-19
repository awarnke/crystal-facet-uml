/* File: entity_data_diagramelement_test.c; Copyright and License: see below */

#include "entity_data_diagramelement_test.h"
#include "entity/data_diagramelement.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_initialize( test_fixture_t *test_env );
static test_case_result_t test_set_get( test_fixture_t *test_env );

test_suite_t entity_data_diagramelement_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_diagramelement_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_initialize", &test_initialize );
    test_suite_add_test_case( &result, "test_set_get", &test_set_get );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_initialize( test_fixture_t *test_env )
{
    data_diagramelement_t testee;

    /* sub test case 0 */
    data_diagramelement_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagramelement_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_row_id( &testee ) );
    const char* uuid_0 = data_diagramelement_get_uuid_const( &testee );
    TEST_EXPECT( uuid_0 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_0 ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_diagram_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_focused_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_NONE, data_diagramelement_get_display_flags( &testee ) );

    /* sub test case 1 */
    data_diagramelement_init_new( &testee,
                                  103,
                                  555,
                                  DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT,
                                  100
                                );
    TEST_EXPECT_EQUAL_INT( false, data_diagramelement_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_row_id( &testee ) );
    const char* uuid_1 = data_diagramelement_get_uuid_const( &testee );
    TEST_EXPECT( uuid_1 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_1 ) );
    TEST_EXPECT_EQUAL_INT( 103, data_diagramelement_get_diagram_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 555, data_diagramelement_get_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 100, data_diagramelement_get_focused_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT, data_diagramelement_get_display_flags( &testee ) );

    /* sub test case 2 */
    /* n/a */

    /* sub test case 3 */
    /* n/a */

    /* sub test case 4 */
    data_diagramelement_init( &testee,
                              1234,
                              107,
                              557,
                              DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                              DATA_ROW_ID_VOID,
                              "1ff2be8d-c46a-4777-8017-e073a41cc680"
                            );
    TEST_EXPECT_EQUAL_INT( true, data_diagramelement_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1234, data_diagramelement_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "1ff2be8d-c46a-4777-8017-e073a41cc680", data_diagramelement_get_uuid_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 107, data_diagramelement_get_diagram_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 557, data_diagramelement_get_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_focused_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_EMPHASIS, data_diagramelement_get_display_flags( &testee ) );

    /* sub test case 5 */
    data_diagramelement_reinit( &testee,
                                12345,
                                103,
                                555,
                                DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT,
                                100,
                                "097498ef-e43b-4b79-b26a-df6f23590165"
                              );
    TEST_EXPECT_EQUAL_INT( true, data_diagramelement_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 12345, data_diagramelement_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "097498ef-e43b-4b79-b26a-df6f23590165", data_diagramelement_get_uuid_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 103, data_diagramelement_get_diagram_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 555, data_diagramelement_get_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 100, data_diagramelement_get_focused_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT, data_diagramelement_get_display_flags( &testee ) );

    /* sub test case 6 */
    data_diagramelement_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagramelement_is_valid( &testee ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_diagramelement_t testee;
    data_diagramelement_t testee_copy;

    /* sub test case 0 */
    data_diagramelement_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagramelement_is_valid( &testee ) );

    /* sub test case 1 */
    data_diagramelement_trace( &testee );
    /* function call is possible, function returns */

    /* sub test case 2 */
    data_diagramelement_set_row_id( &testee, 478 );
    const data_row_id_t row_id = data_diagramelement_get_row_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, row_id );
    TEST_EXPECT_EQUAL_INT( true, data_diagramelement_is_valid( &testee ) );
    const data_id_t data_id = data_diagramelement_get_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, data_id_get_row_id( &data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAMELEMENT, data_id_get_table( &data_id ) );

    /* sub test case 3 */
    /* n/a */

    /* sub test case 4 */
    /* n/a */

    /* sub test case 5 */
    /* n/a */

    /* sub test case 6 */
    /* n/a */

    /* sub test case 7 */
    /* n/a */

    /* sub test case 8 */
    /* n/a */

    /* sub test case 9 */
    /* n/a */

    /* sub test case 10 */
    /* n/a */

    /* sub test case 11, work on copy, do not modify original */
    data_diagramelement_copy( &testee_copy, &testee );
    data_diagramelement_set_diagram_row_id( &testee_copy, 1006 );
    TEST_EXPECT_EQUAL_INT( 1006, data_diagramelement_get_diagram_row_id( &testee_copy ) );
    const data_id_t d_data_id = data_diagramelement_get_diagram_data_id( &testee_copy );
    TEST_EXPECT_EQUAL_INT( 1006, data_id_get_row_id( &d_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &d_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_diagram_row_id( &testee ) );

    /* sub test case 12, work on copy, do not modify original */
    data_diagramelement_replace( &testee_copy, &testee );
    data_diagramelement_set_classifier_row_id( &testee_copy, 1011 );
    TEST_EXPECT_EQUAL_INT( 1011, data_diagramelement_get_classifier_row_id( &testee_copy ) );
    const data_id_t c_data_id = data_diagramelement_get_classifier_data_id( &testee_copy );
    TEST_EXPECT_EQUAL_INT( 1011, data_id_get_row_id( &c_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( &c_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_classifier_row_id( &testee ) );

    /* sub test case 13 */
    data_diagramelement_set_display_flags( &testee, DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT, data_diagramelement_get_display_flags( &testee ) );

    /* sub test case 14 */
    data_diagramelement_set_focused_feature_row_id( &testee, 1999 );
    TEST_EXPECT_EQUAL_INT( 1999, data_diagramelement_get_focused_feature_row_id( &testee ) );
    const data_id_t f_data_id = data_diagramelement_get_focused_feature_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 1999, data_id_get_row_id( &f_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_id_get_table( &f_data_id ) );

    /* sub test case 15 */
    u8_error_t result_15 = data_diagramelement_set_uuid( &testee, "wrong" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, result_15 );

    /* sub test case 16 */
    u8_error_t result_16 = data_diagramelement_set_uuid( &testee, "too long text ...   too long text ...   " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_16 );

    /* sub test case 17 */
    u8_error_t result_17 = data_diagramelement_set_uuid( &testee, "1652f338-5011-4775-9b56-8c08caaa2663" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_17 );
    TEST_EXPECT_EQUAL_STRING( "1652f338-5011-4775-9b56-8c08caaa2663", data_diagramelement_get_uuid_const( &testee ) );

    data_diagramelement_destroy( &testee );
    data_diagramelement_destroy( &testee_copy );

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
