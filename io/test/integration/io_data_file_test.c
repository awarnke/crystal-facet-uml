/* File: io_data_file_test.c; Copyright and License: see below */

#include "io_data_file_test.h"
#include "io_data_file.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <stdio.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t create_new_db( test_fixture_t *fix );
static test_case_result_t open_existing_db( test_fixture_t *fix );
static test_case_result_t open_invalid_file( test_fixture_t *fix );

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char DATABASE_FILENAME[] = "unittest_crystal_facet_uml_default.cfuJ";

test_suite_t io_data_file_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "io_data_file_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "create_new_db", &create_new_db );
    test_suite_add_test_case( &result, "open_existing_db", &open_existing_db );
    test_suite_add_test_case( &result, "open_invalid_file", &open_invalid_file );
    return result;
}

struct test_fixture_struct {
    io_data_file_t data_file;  /*!< data_file instance on which the tests are performed */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    io_data_file_init( &((*fix).data_file) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    int stdio_err;
    io_data_file_destroy( &((*fix).data_file) );
    stdio_err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT ( 0 == stdio_err );
}

static test_case_result_t create_new_db( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    bool isopen;

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( false, isopen );

    /* create a new db */
    u8_error_info_t err_info;
    ctrl_err = io_data_file_open_writeable( &((*fix).data_file), DATABASE_FILENAME, &err_info );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &err_info ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, u8_error_info_get_unit( &err_info ) );

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( true, isopen );

    ctrl_err = io_data_file_close ( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t open_existing_db( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    bool isopen;

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( false, isopen );

    /* create a db first */
    u8_error_info_t err_info;
    data_err = io_data_file_open_writeable( &((*fix).data_file), DATABASE_FILENAME, &err_info );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &err_info ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, u8_error_info_get_unit( &err_info ) );

    data_err = io_data_file_close ( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( false, isopen );

    /* open an existing db */
    u8_error_info_init_line( &err_info, U8_ERROR_PARSER_STRUCTURE, 123 /*line*/ );
    ctrl_err = io_data_file_open_writeable( &((*fix).data_file), DATABASE_FILENAME, &err_info );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &err_info ) );

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( true, isopen );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t open_invalid_file( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    int stdio_err;
    bool isopen;

    /* create a file first */
    FILE *nondb;
    nondb = fopen( DATABASE_FILENAME, "w" );
    TEST_EXPECT( NULL != nondb );

    size_t written;
    written = fwrite( DATABASE_FILENAME, 1, sizeof(DATABASE_FILENAME), nondb );
    TEST_EXPECT_EQUAL_INT( sizeof(DATABASE_FILENAME), written );
    TEST_EXPECT_EQUAL_INT( 40, sizeof(DATABASE_FILENAME) );  /* check if sizeof works as expected */

    stdio_err = fclose( nondb );
    TEST_EXPECT_EQUAL_INT( 0, stdio_err );

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( false, isopen );

    /* open an existing non-db file */
    u8_error_info_t err_info;
    ctrl_err = io_data_file_open_writeable( &((*fix).data_file), DATABASE_FILENAME, &err_info );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, u8_error_info_get_error( &err_info ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &err_info ) );
    TEST_EXPECT_EQUAL_INT( 1, u8_error_info_get_line( &err_info ) );

    isopen = io_data_file_is_open( &((*fix).data_file) );
    TEST_EXPECT_EQUAL_INT( false, isopen );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2018-2024 Andreas Warnke
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
