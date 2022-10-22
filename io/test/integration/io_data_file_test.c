/* File: io_data_file_test.c; Copyright and License: see below */

#include "io_data_file_test.h"
#include "io_data_file.h"
#include "test_assert.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void create_new_db(void);
static void open_existing_db(void);
static void open_invalid_file(void);

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char DATABASE_FILENAME[] = "unittest_crystal_facet_uml_default.cfu1";

/*!
 *  \brief data_file instance on which the tests are performed
 */
static io_data_file_t data_file;

test_suite_t io_data_file_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "io_data_file_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "create_new_db", &create_new_db );
    test_suite_add_test_case( &result, "open_existing_db", &open_existing_db );
    test_suite_add_test_case( &result, "open_invalid_file", &open_invalid_file );
    return result;
}

static void set_up(void)
{
    io_data_file_init( &data_file );
}

static void tear_down(void)
{
    int stdio_err;
    io_data_file_destroy( &data_file );
    stdio_err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT ( 0 == stdio_err );
}

static void create_new_db(void)
{
    u8_error_t ctrl_err;
    bool isopen;

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* create a new db */
    u8_error_info_t err_info;
    ctrl_err = io_data_file_open_writeable( &data_file, DATABASE_FILENAME, &err_info );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &err_info ) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, u8_error_info_get_unit( &err_info ) );

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( true, isopen );
}

static void open_existing_db(void)
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    bool isopen;

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* create a db first */
    u8_error_info_t err_info;
    data_err = io_data_file_open_writeable( &data_file, DATABASE_FILENAME, &err_info );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &err_info ) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, u8_error_info_get_unit( &err_info ) );

    data_err = io_data_file_close ( &data_file );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* open an existing db */
    u8_error_info_init_line( &err_info, U8_ERROR_PARSER_STRUCTURE, 123 /*line*/ );
    ctrl_err = io_data_file_open_writeable( &data_file, DATABASE_FILENAME, &err_info );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &err_info ) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, u8_error_info_get_unit( &err_info ) );

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( true, isopen );
}

static void open_invalid_file(void)
{
    u8_error_t ctrl_err;
    int stdio_err;
    bool isopen;

    /* create a file first */
    FILE *nondb;
    nondb = fopen( DATABASE_FILENAME, "w" );
    TEST_ASSERT( NULL != nondb );

    size_t written;
    written = fwrite( DATABASE_FILENAME, 1, sizeof(DATABASE_FILENAME), nondb );
    TEST_ASSERT_EQUAL_INT( sizeof(DATABASE_FILENAME), written );
    TEST_ASSERT_EQUAL_INT( 40, sizeof(DATABASE_FILENAME) );  /* check if sizeof works as expected */

    stdio_err = fclose( nondb );
    TEST_ASSERT_EQUAL_INT( 0, stdio_err );

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* open an existing non-db file */
    u8_error_info_t err_info;
    ctrl_err = io_data_file_open_writeable( &data_file, DATABASE_FILENAME, &err_info );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, u8_error_info_get_error( &err_info ) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &err_info ) );
    TEST_ASSERT_EQUAL_INT( 1, u8_error_info_get_position( &err_info ) );

    isopen = io_data_file_is_open( &data_file );
    TEST_ASSERT_EQUAL_INT( false, isopen );
}


/*
 * Copyright 2018-2022 Andreas Warnke
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
