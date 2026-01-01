/* File: entity_data_diagram_test.c; Copyright and License: see below */

#include "entity_data_diagram_test.h"
#include "entity/data_diagram.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_initialize( test_fixture_t *test_env );
static test_case_result_t test_set_get( test_fixture_t *test_env );

test_suite_t entity_data_diagram_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_diagram_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_initialize", &test_initialize );
    test_suite_add_test_case( &result, "test_set_get", &test_set_get );
    return result;
}

/* initialize a long string by single characters to avoid the maximum string length ‘4095’ ISO C99 compilers are required to support */
#define TOO_LONG_20 't', 'o', 'o', ' ', 'l', 'o', 'n', 'g', ' ', 't', 'e', 'x', 't', ' ', '.', '.', '.', ' ', ' ', ' ',
#define TOO_LONG_100 TOO_LONG_20 TOO_LONG_20 TOO_LONG_20 TOO_LONG_20 TOO_LONG_20
#define TOO_LONG_800 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100
static const char static_too_long[8801]
    = { TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800
      TOO_LONG_800 TOO_LONG_800 '\0' };

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
    data_diagram_t testee;

    /* sub test case 0 */
    data_diagram_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagram_get_row_id( &testee ) );
    const char* uuid_0 = data_diagram_get_uuid_const( &testee );
    TEST_EXPECT( uuid_0 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_0 ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagram_get_parent_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_NONE, data_diagram_get_display_flags( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_LIST, data_diagram_get_diagram_type( &testee ) );

    /* sub test case 1 */
    const u8_error_t result_1
        = data_diagram_init_new( &testee,
                                 3,
                                 DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM,
                                 "stereo",
                                 "name",
                                 "description",
                                 24,
                                 DATA_DIAGRAM_FLAG_GRAY_OUT
                               );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_1 );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagram_get_row_id( &testee ) );
    const char* uuid_1 = data_diagram_get_uuid_const( &testee );
    TEST_EXPECT( uuid_1 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_1 ) );
    TEST_EXPECT_EQUAL_INT( 3, data_diagram_get_parent_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "stereo", data_diagram_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "name", data_diagram_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_diagram_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 24, data_diagram_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_GRAY_OUT, data_diagram_get_display_flags( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, data_diagram_get_diagram_type( &testee ) );

    /* sub test case 2 */
    const u8_error_t result_2
        = data_diagram_init_new( &testee,
                                 1001,
                                 DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM,
                                 (*test_env).too_long,
                                 (*test_env).too_long,
                                 (*test_env).too_long,
                                 47,
                                 DATA_DIAGRAM_FLAG_EMPHASIS
                               );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_2 );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagram_get_row_id( &testee ) );
    const char* uuid_2 = data_diagram_get_uuid_const( &testee );
    TEST_EXPECT( uuid_2 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_2 ) );
    TEST_EXPECT_EQUAL_INT( 1001, data_diagram_get_parent_row_id( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_stereotype_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_name_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_description_const( &testee ), "too long text" ) );
    TEST_EXPECT_EQUAL_INT( 47, data_diagram_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_EMPHASIS, data_diagram_get_display_flags( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM, data_diagram_get_diagram_type( &testee ) );

    /* sub test case 3 */
    data_diagram_reinit_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( &testee ) );
    const char* uuid_3 = data_diagram_get_uuid_const( &testee );
    TEST_EXPECT( uuid_3 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_3 ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagram_get_parent_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_NONE, data_diagram_get_display_flags( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_LIST, data_diagram_get_diagram_type( &testee ) );

    /* sub test case 4 */
    const u8_error_t result_4
        = data_diagram_init( &testee,
                             1234,
                             990,
                             DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM,
                             "stereo",
                             "value",
                             "description",
                             24,
                             DATA_DIAGRAM_FLAG_GRAY_OUT,
                             "1ff2be8d-c46a-4777-8017-e073a41cc680"
                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_INT( true, data_diagram_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1234, data_diagram_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( 990, data_diagram_get_parent_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "1ff2be8d-c46a-4777-8017-e073a41cc680", data_diagram_get_uuid_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "stereo", data_diagram_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "value", data_diagram_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_diagram_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 24, data_diagram_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_GRAY_OUT, data_diagram_get_display_flags( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM, data_diagram_get_diagram_type( &testee ) );

    /* sub test case 5 */
    const u8_error_t result_5
        = data_diagram_init( &testee,
                             12345,
                             991,
                             DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                             (*test_env).too_long,
                             (*test_env).too_long,
                             (*test_env).too_long,
                             47,
                             DATA_DIAGRAM_FLAG_EMPHASIS,
                             "097498ef-e43b-4b79-b26a-df6f23590165"
                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT_EQUAL_INT( true, data_diagram_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 12345, data_diagram_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "097498ef-e43b-4b79-b26a-df6f23590165", data_diagram_get_uuid_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 991, data_diagram_get_parent_row_id( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_stereotype_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_name_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_description_const( &testee ), "too long text" ) );
    TEST_EXPECT_EQUAL_INT( 47, data_diagram_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_EMPHASIS, data_diagram_get_display_flags( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, data_diagram_get_diagram_type( &testee ) );

    /* sub test case 6 */
    data_diagram_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( &testee ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_diagram_t testee;
    data_diagram_t testee_copy;

    /* sub test case 0 */
    data_diagram_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( &testee ) );

    /* sub test case 1 */
    data_diagram_trace( &testee );
    /* function call is possible, function returns */

    /* sub test case 2 */
    data_diagram_set_row_id( &testee, 478 );
    const data_row_t row_id = data_diagram_get_row_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, row_id );
    TEST_EXPECT_EQUAL_INT( true, data_diagram_is_valid( &testee ) );
    const data_id_t data_id = data_diagram_get_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, data_id_get_row_id( &data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &data_id ) );

    /* sub test case 3 */
    const data_diagram_type_t m_type_0 = data_diagram_get_diagram_type( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagram_type_is_interaction( m_type_0 ) );
    data_diagram_set_diagram_type( &testee, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM );
    const data_diagram_type_t m_type_1 = data_diagram_get_diagram_type( &testee );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, m_type_1 );
    TEST_EXPECT_EQUAL_INT( true, data_diagram_type_is_interaction( m_type_1 ) );

    /* sub test case 4 */
    TEST_EXPECT_EQUAL_INT( false, data_diagram_has_stereotype( &testee ) );
    const u8_error_t result_4 = data_diagram_set_stereotype( &testee, "2ch" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_diagram_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, data_diagram_has_stereotype( &testee ) );

    /* sub test case 5, work on copy, do not modify original */
    data_diagram_copy( &testee_copy, &testee );
    const u8_error_t result_5 = data_diagram_set_stereotype( &testee_copy, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_stereotype_const( &testee_copy ), "too long text" ) );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_diagram_get_stereotype_const( &testee ) );

    /* sub test case 6, work on copy, do not modify original */
    data_diagram_replace( &testee_copy, &testee );
    const u8_error_t result_6 = data_diagram_set_name( &testee_copy, "Amplifier" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_6 );
    TEST_EXPECT_EQUAL_STRING( "Amplifier", data_diagram_get_name_const( &testee_copy ) );
    TEST_EXPECT_EQUAL_STRING( "", data_diagram_get_name_const( &testee ) );

    /* sub test case 7 */
    const u8_error_t result_7 = data_diagram_set_name( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_7 );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_name_const( &testee ), "too long text" ) );

    /* sub test case 8 */
    const u8_error_t result_8 = data_diagram_set_description( &testee, "The " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_8 );
    TEST_EXPECT_EQUAL_STRING( "The ", data_diagram_get_description_const( &testee ) );

    /* sub test case 9 */
    const u8_error_t result_9 = data_diagram_append_description( &testee, "amplifier " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_9 );
    TEST_EXPECT_EQUAL_STRING( "The amplifier ", data_diagram_get_description_const( &testee ) );

    /* sub test case 10 */
    const u8_error_t result_10 = data_diagram_append_description( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_10 );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_description_const( &testee ), "The amplifier too long" ) );

    /* sub test case 11 */
    const u8_error_t result_11 = data_diagram_set_description( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_11 );
    TEST_EXPECT( utf8string_starts_with_str( data_diagram_get_description_const( &testee ), "too long text" ) );

    /* sub test case 12 */
    data_diagram_set_parent_row_id( &testee, 1006 );
    TEST_EXPECT_EQUAL_INT( 1006, data_diagram_get_parent_row_id( &testee ) );
    const data_id_t p_data_id = data_diagram_get_parent_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 1006, data_id_get_row_id( &p_data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &p_data_id ) );

    /* sub test case 13 */
    data_diagram_set_display_flags( &testee, DATA_DIAGRAM_FLAG_EMPHASIS );
    TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_FLAG_EMPHASIS, data_diagram_get_display_flags( &testee ) );

    /* sub test case 14 */
    data_diagram_set_list_order( &testee, 3 );
    TEST_EXPECT_EQUAL_INT( 3, data_diagram_get_list_order( &testee ) );

    /* sub test case 15 */
    u8_error_t result_15 = data_diagram_set_uuid( &testee, "wrong" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, result_15 );

    /* sub test case 16 */
    u8_error_t result_16 = data_diagram_set_uuid( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_16 );

    /* sub test case 17 */
    u8_error_t result_17 = data_diagram_set_uuid( &testee, "1652f338-5011-4775-9b56-8c08caaa2663" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_17 );
    TEST_EXPECT_EQUAL_STRING( "1652f338-5011-4775-9b56-8c08caaa2663", data_diagram_get_uuid_const( &testee ) );

    data_diagram_destroy( &testee );
    data_diagram_destroy( &testee_copy );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2026 Andreas Warnke
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
