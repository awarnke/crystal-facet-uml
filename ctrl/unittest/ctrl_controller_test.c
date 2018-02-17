/* File: ctrl_controller_test.c; Copyright and License: see below */

#include "ctrl_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "data_visible_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void create_new_db(void);
static void open_existing_db(void);
static void open_invalid_file(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char DATABASE_FILENAME[] = "unittest_crystal_facet_uml_default.cfu1";

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

TestRef ctrl_controller_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("create_new_db",create_new_db),
        new_TestFixture("open_existing_db",open_existing_db),
        new_TestFixture("open_invalid_file",open_invalid_file),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_controller_test",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
    data_database_init( &database );
    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int stdio_err;
    ctrl_controller_destroy( &controller );
    data_database_destroy( &database );
    stdio_err = remove( DATABASE_FILENAME );
}

static void create_new_db(void)
{
    ctrl_error_t ctrl_err;
    bool isopen;

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* create a new db */
    ctrl_err = ctrl_controller_switch_database ( &controller, DATABASE_FILENAME );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( true, isopen );
}

static void open_existing_db(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    bool isopen;

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* create a db first */
    data_err = data_database_open ( &database, DATABASE_FILENAME );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_close ( &database );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* open an existing db */
    ctrl_err = ctrl_controller_switch_database ( &controller, DATABASE_FILENAME );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, data_err );

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( true, isopen );
}

static void open_invalid_file(void)
{
    ctrl_error_t ctrl_err;
    int stdio_err;
    bool isopen;

    /* create a file first */
    FILE *nondb;
    nondb = fopen( DATABASE_FILENAME, "w" );
    TEST_ASSERT( NULL != nondb );

    size_t written;
    written = fwrite ( DATABASE_FILENAME, 1, sizeof(DATABASE_FILENAME), nondb );
    TEST_ASSERT_EQUAL_INT( sizeof(DATABASE_FILENAME), written );
    TEST_ASSERT_EQUAL_INT( 40, sizeof(DATABASE_FILENAME) );  /* check if sizeof works as expected */

    stdio_err = fclose( nondb );
    TEST_ASSERT_EQUAL_INT( 0, stdio_err );

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( false, isopen );

    /* open an existing non-db file */
    ctrl_err = ctrl_controller_switch_database ( &controller, DATABASE_FILENAME );
    /*TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NO_DB & DATA_ERROR_MASK, ctrl_err & DATA_ERROR_MASK );*/
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_AT_DB, ctrl_err );

    isopen = data_database_is_open( &database );
    TEST_ASSERT_EQUAL_INT( false, isopen );
}


/*
 * Copyright 2018-2018 Andreas Warnke
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
