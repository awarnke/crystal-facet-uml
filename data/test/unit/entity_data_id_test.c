/* File: entity_data_id_test.c; Copyright and License: see below */

#include "entity_data_id_test.h"
#include "entity/data_id.h"
#include "utf8stream/utf8stream_writemem.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_init_get( test_fixture_t *test_env );
static test_case_result_t test_equals( test_fixture_t *test_env );
static test_case_result_t test_from_string_ok( test_fixture_t *test_env );
static test_case_result_t test_from_string_err( test_fixture_t *test_env );
static test_case_result_t test_to_string( test_fixture_t *test_env );
static test_case_result_t test_to_stream( test_fixture_t *test_env );

test_suite_t entity_data_id_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_id_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_init_get", &test_init_get );
    test_suite_add_test_case( &result, "test_equals", &test_equals );
    test_suite_add_test_case( &result, "test_from_string_ok", &test_from_string_ok );
    test_suite_add_test_case( &result, "test_from_string_err", &test_from_string_err );
    test_suite_add_test_case( &result, "test_to_string", &test_to_string );
    test_suite_add_test_case( &result, "test_to_stream", &test_to_stream );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_init_get( test_fixture_t *test_env )
{
    data_id_t testee;

    data_id_init( &testee, DATA_TABLE_VOID, 123 );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 123, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &testee ) );

    data_id_destroy( &testee );

    data_id_init_void( &testee );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &testee ) );

    data_id_reinit( &testee, DATA_TABLE_DIAGRAMELEMENT, 15 );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAMELEMENT, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 15, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, data_id_is_valid( &testee ) );

    data_id_t testee2;
    data_id_copy( &testee2, &testee );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAMELEMENT, data_id_get_table( &testee2 ) );
    TEST_EXPECT_EQUAL_INT( 15, data_id_get_row_id( &testee2 ) );
    TEST_EXPECT_EQUAL_INT( true, data_id_is_valid( &testee2 ) );

    data_id_reinit_void( &testee2 );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee2 ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee2 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &testee2 ) );

    data_id_replace( &testee, &testee2 );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &testee ) );

    data_id_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_equals( test_fixture_t *test_env )
{
    data_id_t testee1;
    data_id_t testee2;
    data_id_t testee3;
    data_id_t testee4;
    data_id_t testee5;
    data_id_t testee6 = DATA_ID_VOID;

    data_id_init( &testee1, DATA_TABLE_DIAGRAM, 123 );
    data_id_init( &testee2, DATA_TABLE_DIAGRAMELEMENT, 123 );
    data_id_init( &testee3, DATA_TABLE_DIAGRAMELEMENT, 123 );
    data_id_init( &testee4, DATA_TABLE_DIAGRAMELEMENT, 666 );
    data_id_init_void( &testee5 );

    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &testee1, &testee1 ) );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &testee2, &testee3 ) );

    TEST_EXPECT_EQUAL_INT( false, data_id_equals( &testee1, &testee2 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals( &testee3, &testee4 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals( &testee1, &testee5 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals( &testee5, &testee5 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals( &testee5, &testee6 ) );

    TEST_EXPECT_EQUAL_INT( true, data_id_equals_or_both_void( &testee1, &testee1 ) );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals_or_both_void( &testee2, &testee3 ) );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals_or_both_void( &testee5, &testee5 ) );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals_or_both_void( &testee5, &testee6 ) );

    TEST_EXPECT_EQUAL_INT( false, data_id_equals_or_both_void( &testee1, &testee2 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals_or_both_void( &testee3, &testee4 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals_or_both_void( &testee1, &testee5 ) );

    TEST_EXPECT_EQUAL_INT( true, data_id_equals_id( &testee1, DATA_TABLE_DIAGRAM, 123 ) );

    TEST_EXPECT_EQUAL_INT( false, data_id_equals_id( &testee1, DATA_TABLE_DIAGRAM, 666 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals_id( &testee1, DATA_TABLE_DIAGRAMELEMENT, 123 ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals_id( &testee1, DATA_TABLE_VOID, DATA_ROW_VOID ) );
    TEST_EXPECT_EQUAL_INT( false, data_id_equals_id( &testee5, DATA_TABLE_VOID, DATA_ROW_VOID ) );

    data_id_destroy( &testee1 );
    data_id_destroy( &testee2 );
    data_id_destroy( &testee3 );
    data_id_destroy( &testee4 );
    data_id_destroy( &testee5 );
    data_id_destroy( &testee6 );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_from_string_ok( test_fixture_t *test_env )
{
    data_id_t testee;

    data_id_init_by_string( &testee, "E0123" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAMELEMENT, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 123, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    data_id_init_by_string( &testee, "D9223372036854775807" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 9223372036854775807, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    data_id_init_by_string( &testee, "D-001" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    utf8stringview_t remainder;
    data_id_init_by_stringview( &testee, &UTF8STRINGVIEW_STR( "C0123FG" ), &remainder );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 123, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, utf8stringview_equals_str( &remainder, "FG" ) );
    data_id_destroy( &testee );

    data_id_init_by_stringview( &testee, &UTF8STRINGVIEW_STR( "F0000" ), &remainder );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_FEATURE, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, utf8stringview_equals_str( &remainder, "" ) );
    data_id_destroy( &testee );

    data_id_init_by_stringview( &testee, &UTF8STRINGVIEW_STR( "R9223372036854775807 9223372036854775807" ), &remainder );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_RELATIONSHIP, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( 9223372036854775807, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, utf8stringview_equals_str( &remainder, " 9223372036854775807" ) );
    data_id_destroy( &testee );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_from_string_err( test_fixture_t *test_env )
{
    data_id_t testee;

    data_id_init_by_string( &testee, "" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    data_id_init_by_string( &testee, "D123456789012345678901234567890" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    data_id_init_by_string( &testee, "D-1 123" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    data_id_init_by_string( &testee, "D0012 next" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    data_id_init_by_string( &testee, "A0012" );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    data_id_destroy( &testee );

    utf8stringview_t remainder;
    data_id_init_by_stringview( &testee, &UTF8STRINGVIEW_EMPTY, &remainder );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, utf8stringview_equals_str( &remainder, "" ) );
    data_id_destroy( &testee );

    data_id_init_by_stringview( &testee, &UTF8STRINGVIEW_STR( "D" ), &remainder );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_VOID, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, utf8stringview_equals_str( &remainder, "D" ) );
    data_id_destroy( &testee );

    data_id_init_by_stringview( &testee, &UTF8STRINGVIEW_STR( "D-123" ), &remainder );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &testee ) );
    TEST_EXPECT_EQUAL_INT( -123, data_id_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, utf8stringview_equals_str( &remainder, "" ) );
    data_id_destroy( &testee );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_to_string( test_fixture_t *test_env )
{
    data_id_t testee;
    utf8error_t err;

    char buf[8];
    utf8stringbuf_t strbuf = utf8stringbuf_new( sizeof( buf ), &(buf[0]) );

    data_id_init( &testee, DATA_TABLE_FEATURE, 123 );
    utf8stringbuf_clear( &strbuf );
    err = data_id_to_utf8stringbuf( &testee, strbuf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_INT( true, utf8stringbuf_equals_str( &strbuf, "F0123" ) );

    data_id_reinit( &testee, DATA_TABLE_RELATIONSHIP, DATA_ROW_VOID );
    utf8stringbuf_clear( &strbuf );
    err = data_id_to_utf8stringbuf( &testee, strbuf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_INT( true, utf8stringbuf_equals_str( &strbuf, "R-1" ) );

    data_id_reinit( &testee, DATA_TABLE_CLASSIFIER, 1234567 );
    utf8stringbuf_clear( &strbuf );
    err = data_id_to_utf8stringbuf( &testee, strbuf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_TRUNCATED, err );
    TEST_EXPECT_EQUAL_INT( true, utf8stringbuf_equals_str( &strbuf, "C123456" ) );

    data_id_reinit( &testee, DATA_TABLE_DIAGRAM, 0 );
    utf8stringbuf_clear( &strbuf );
    err = data_id_to_utf8stringbuf( &testee, strbuf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_INT( true, utf8stringbuf_equals_str( &strbuf, "D0000" ) );

    data_id_reinit( &testee, DATA_TABLE_DIAGRAMELEMENT, 40 );
    utf8stringbuf_clear( &strbuf );
    err = data_id_to_utf8stringbuf( &testee, strbuf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_INT( true, utf8stringbuf_equals_str( &strbuf, "E0040" ) );

    data_id_reinit_void( &testee );
    utf8stringbuf_clear( &strbuf );
    err = data_id_to_utf8stringbuf( &testee, strbuf );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_INT( true, utf8stringbuf_equals_str( &strbuf, "void" ) );

    data_id_trace( &testee );
    /* only check that function returns */

    data_id_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_to_stream( test_fixture_t *test_env )
{
    data_id_t testee;
    u8_error_t err;
    u8_error_t u8err;

    char buf[8];
    utf8stream_writemem_t stream;
    utf8stream_writemem_init( &stream, &buf, sizeof(buf) );

    data_id_init( &testee, DATA_TABLE_FEATURE, 123 );
    u8err = utf8stream_writemem_reset( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8err );
    err = data_id_to_utf8_writer( &testee, utf8stream_writemem_get_writer( &stream ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( true, utf8string_equals_str( utf8stream_writemem_get_string( &stream ), "F0123" ) );

    data_id_reinit( &testee, DATA_TABLE_RELATIONSHIP, DATA_ROW_VOID );
    u8err = utf8stream_writemem_reset( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8err );
    err = data_id_to_utf8_writer( &testee, utf8stream_writemem_get_writer( &stream ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( true, utf8string_equals_str( utf8stream_writemem_get_string( &stream ), "R-1" ) );

    data_id_reinit( &testee, DATA_TABLE_CLASSIFIER, 1234567 );
    u8err = utf8stream_writemem_reset( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8err );
    err = data_id_to_utf8_writer( &testee, utf8stream_writemem_get_writer( &stream ) );
    err |= utf8stream_writer_flush( utf8stream_writemem_get_writer( &stream ) ); /* flush now to not get the error later */
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( true, utf8string_equals_str( utf8stream_writemem_get_string( &stream ), "C123456" ) );

    data_id_reinit( &testee, DATA_TABLE_DIAGRAM, 0 );
    u8err = utf8stream_writemem_reset( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8err );
    err = data_id_to_utf8_writer( &testee, utf8stream_writemem_get_writer( &stream ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( true, utf8string_equals_str( utf8stream_writemem_get_string( &stream ), "D0000" ) );

    data_id_reinit( &testee, DATA_TABLE_DIAGRAMELEMENT, 40 );
    u8err = utf8stream_writemem_reset( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8err );
    err = data_id_to_utf8_writer( &testee, utf8stream_writemem_get_writer( &stream ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( true, utf8string_equals_str( utf8stream_writemem_get_string( &stream ), "E0040" ) );

    data_id_reinit_void( &testee );
    u8err = utf8stream_writemem_reset( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8err );
    err = data_id_to_utf8_writer( &testee, utf8stream_writemem_get_writer( &stream ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( true, utf8string_equals_str( utf8stream_writemem_get_string( &stream ), "void" ) );

    data_id_destroy( &testee );

    const u8_error_t err0 = utf8stream_writemem_destroy( &stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err0 );

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
