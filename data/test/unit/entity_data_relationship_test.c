/* File: entity_data_relationship_test.c; Copyright and License: see below */

#include "entity_data_relationship_test.h"
#include "entity/data_relationship.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_initialize( test_fixture_t *test_env );
static test_case_result_t test_set_get( test_fixture_t *test_env );

test_suite_t entity_data_relationship_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_relationship_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_initialize", &test_initialize );
    test_suite_add_test_case( &result, "test_set_get", &test_set_get );
    return result;
}

#define TOO_LONG_100 "too long text ...   too long text ...   too long text ...   too long text ...   too long text ...   "
#define TOO_LONG_800 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100
static const char static_too_long[4801] = TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800;

struct test_fixture_struct {
    const char *too_long;  /*!< a pointer to a string that is too long */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture.too_long = &(static_too_long[0]);
    return &test_fixture;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_initialize( test_fixture_t *test_env )
{
    data_relationship_t testee;

    /* sub test case 0 */
    data_relationship_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_relationship_is_valid( &testee ) );
    const char* uuid_0 = data_relationship_get_uuid_const( &testee );
    TEST_EXPECT( uuid_0 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_0 ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_relationship_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, data_relationship_get_main_type( &testee ) );

    /* sub test case 1 */
    const u8_error_t result_1
        = data_relationship_init_new( &testee,
                                    1033,
                                    812,
                                    1035,
                                    823,
                                    DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW,
                                    "stereotype",
                                    "name",
                                    "description",
                                    24
                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_1 );
    TEST_EXPECT_EQUAL_INT( false, data_relationship_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_row_id( &testee ) );
    const char* uuid_1 = data_relationship_get_uuid_const( &testee );
    TEST_EXPECT( uuid_1 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_1 ) );
    TEST_EXPECT_EQUAL_INT( 1033, data_relationship_get_from_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 812, data_relationship_get_from_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1035, data_relationship_get_to_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 823, data_relationship_get_to_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "stereotype", data_relationship_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "name", data_relationship_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_relationship_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 24, data_relationship_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, data_relationship_get_main_type( &testee ) );

    /* sub test case 2 */
    const u8_error_t result_2
        = data_relationship_init_new( &testee,
                                    1034,
                                    DATA_ROW_ID_VOID,
                                    1036,
                                    824,
                                    DATA_RELATIONSHIP_TYPE_UML_REALIZATION,
                                    (*test_env).too_long,
                                    (*test_env).too_long,
                                    (*test_env).too_long,
                                    16
                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_2 );
    TEST_EXPECT_EQUAL_INT( false, data_relationship_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_row_id( &testee ) );
    const char* uuid_2 = data_relationship_get_uuid_const( &testee );
    TEST_EXPECT( uuid_2 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_2 ) );
    TEST_EXPECT_EQUAL_INT( 1034, data_relationship_get_from_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1036, data_relationship_get_to_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 824, data_relationship_get_to_feature_row_id( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_name_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_stereotype_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_description_const( &testee ), "too long text" ) );
    TEST_EXPECT_EQUAL_INT( 16, data_relationship_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_REALIZATION, data_relationship_get_main_type( &testee ) );

    /* sub test case 3 */
    data_relationship_reinit_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_relationship_is_valid( &testee ) );
    const char* uuid_3 = data_relationship_get_uuid_const( &testee );
    TEST_EXPECT( uuid_3 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_3 ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_relationship_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, data_relationship_get_main_type( &testee ) );

    /* sub test case 4 */
    const u8_error_t result_4
        = data_relationship_init( &testee,
                                  1234,
                                  1033,
                                  813,
                                  1035,
                                  DATA_ROW_ID_VOID,
                                  DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
                                  "stereotype",
                                  "name",
                                  "description",
                                  24,
                                  "1ff2be8d-c46a-4777-8017-e073a41cc680"
                                );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_INT( true, data_relationship_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1234, data_relationship_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1033, data_relationship_get_from_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 813, data_relationship_get_from_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1035, data_relationship_get_to_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_to_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "1ff2be8d-c46a-4777-8017-e073a41cc680", data_relationship_get_uuid_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "stereotype", data_relationship_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "name", data_relationship_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_relationship_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 24, data_relationship_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, data_relationship_get_main_type( &testee ) );

    /* sub test case 5 */
    const u8_error_t result_5
        = data_relationship_init( &testee,
                                  54,
                                  1034,
                                  DATA_ROW_ID_VOID,
                                  1036,
                                  824,
                                  DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
                                  (*test_env).too_long,
                                  (*test_env).too_long,
                                  (*test_env).too_long,
                                  47,
                                  "097498ef-e43b-4b79-b26a-df6f23590165"
                                );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT_EQUAL_INT( true, data_relationship_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 54, data_relationship_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1034, data_relationship_get_from_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1036, data_relationship_get_to_classifier_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 824, data_relationship_get_to_feature_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "097498ef-e43b-4b79-b26a-df6f23590165", data_relationship_get_uuid_const( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_name_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_stereotype_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_description_const( &testee ), "too long text" ) );
    TEST_EXPECT_EQUAL_INT( 47, data_relationship_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, data_relationship_get_main_type( &testee ) );

    /* sub test case 6 */
    data_relationship_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_relationship_is_valid( &testee ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_relationship_t testee;
    data_relationship_t testee_copy;

    /* sub test case 0 */
    data_relationship_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_relationship_is_valid( &testee ) );

    /* sub test case 1 */
    data_relationship_trace( &testee );
    /* function call is possible, function returns */

    /* sub test case 2 */
    data_relationship_set_row_id( &testee, 478 );
    const data_row_id_t row_id = data_relationship_get_row_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, row_id );
    TEST_EXPECT_EQUAL_INT( true, data_relationship_is_valid( &testee ) );
    const data_id_t data_id = data_relationship_get_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, data_id_get_row_id( &data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_RELATIONSHIP, data_id_get_table( &data_id ) );

    /* sub test case 3 */
    data_relationship_set_main_type( &testee, DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW );
    const data_relationship_type_t m_type = data_relationship_get_main_type( &testee );
    TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, m_type );

    /* sub test case 4 */
    TEST_EXPECT_EQUAL_INT( false, data_relationship_has_stereotype( &testee ) );
    const u8_error_t result_4 = data_relationship_set_stereotype( &testee, "2ch" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_relationship_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, data_relationship_has_stereotype( &testee ) );

    /* sub test case 5, work on copy, do not modify original */
    data_relationship_copy( &testee_copy, &testee );
    const u8_error_t result_5 = data_relationship_set_stereotype( &testee_copy, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_stereotype_const( &testee_copy ), "too long text" ) );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_relationship_get_stereotype_const( &testee ) );

    /* sub test case 6, work on copy, do not modify original */
    data_relationship_replace( &testee_copy, &testee );
    const u8_error_t result_6 = data_relationship_set_name( &testee_copy, "Amplifier" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_6 );
    TEST_EXPECT_EQUAL_STRING( "Amplifier", data_relationship_get_name_const( &testee_copy ) );
    TEST_EXPECT_EQUAL_STRING( "", data_relationship_get_name_const( &testee ) );

    /* sub test case 7 */
    const u8_error_t result_7 = data_relationship_set_name( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_7 );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_name_const( &testee ), "too long text" ) );

    /* sub test case 8 */
    const u8_error_t result_8 = data_relationship_set_description( &testee, "The " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_8 );
    TEST_EXPECT_EQUAL_STRING( "The ", data_relationship_get_description_const( &testee ) );

    /* sub test case 9 */
    const u8_error_t result_9 = data_relationship_append_description( &testee, "amplifier " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_9 );
    TEST_EXPECT_EQUAL_STRING( "The amplifier ", data_relationship_get_description_const( &testee ) );

    /* sub test case 10 */
    const u8_error_t result_10 = data_relationship_append_description( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_10 );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_description_const( &testee ), "The amplifier too long" ) );

    /* sub test case 11 */
    const u8_error_t result_11 = data_relationship_set_description( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_11 );
    TEST_EXPECT( utf8string_starts_with_str( data_relationship_get_description_const( &testee ), "too long text" ) );

    /* sub test case 12 */
    data_relationship_set_from_classifier_row_id( &testee, 1600 );
    TEST_EXPECT_EQUAL_INT( 1600, data_relationship_get_from_classifier_row_id( &testee ) );
    const data_id_t from_c_data_id = data_relationship_get_from_classifier_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 1600, data_id_get_row_id( &from_c_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( &from_c_data_id ) );
    data_relationship_set_from_feature_row_id( &testee, DATA_ROW_ID_VOID );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &testee ) );
    const data_id_t from_f_data_id = data_relationship_get_from_feature_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_id_get_row_id( &from_f_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_id_get_table( &from_f_data_id ) );

    /* sub test case 13 */
    data_relationship_set_to_classifier_row_id( &testee, 1602 );
    TEST_EXPECT_EQUAL_INT( 1602, data_relationship_get_to_classifier_row_id( &testee ) );
    const data_id_t to_c_data_id = data_relationship_get_to_classifier_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 1602, data_id_get_row_id( &to_c_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( &to_c_data_id ) );
    data_relationship_set_to_feature_row_id( &testee, 1222 );
    TEST_EXPECT_EQUAL_INT( 1222, data_relationship_get_to_feature_row_id( &testee ) );
    const data_id_t to_f_data_id = data_relationship_get_to_feature_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 1222, data_id_get_row_id( &to_f_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_id_get_table( &to_f_data_id ) );

    /* sub test case 14 */
    data_relationship_set_list_order( &testee, 3 );
    TEST_EXPECT_EQUAL_INT( 3, data_relationship_get_list_order( &testee ) );

    /* sub test case 15 */
    u8_error_t result_15 = data_relationship_set_uuid( &testee, "wrong" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, result_15 );

    /* sub test case 16 */
    u8_error_t result_16 = data_relationship_set_uuid( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_16 );

    /* sub test case 17 */
    u8_error_t result_17 = data_relationship_set_uuid( &testee, "1652f338-5011-4775-9b56-8c08caaa2663" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_17 );
    TEST_EXPECT_EQUAL_STRING( "1652f338-5011-4775-9b56-8c08caaa2663", data_relationship_get_uuid_const( &testee ) );

    data_relationship_destroy( &testee );
    data_relationship_destroy( &testee_copy );

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
