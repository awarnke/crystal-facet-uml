/* File: ctrl_consistency_checker_test.c; Copyright and License: see below */

#include "ctrl_consistency_checker_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void diagram_two_roots_consistency(void);
static void diagram_missing_parent_consistency(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char* DATABASE_FILENAME = "unittest_crystal_facet_uml_default.cfu.sqlite3";

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief database writer to access the database
 */
static data_database_writer_t db_writer;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

TestRef ctrl_consistency_checker_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("diagram_two_roots_consistency",diagram_two_roots_consistency),
        new_TestFixture("diagram_missing_parent_consistency",diagram_missing_parent_consistency),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_consistency_checker_test",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int err;
    ctrl_controller_destroy( &controller );

    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
}

static void diagram_two_roots_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[512] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create first root diagrams */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   2 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name",
                                   "diagram_description",
                                   10222 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create second root diagrams */
    data_err = data_diagram_init ( &current_diagram,
                                   4 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name_2",
                                   "diagram_description_2",
                                   10333 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, false /*modify_db*/, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, true /*modify_db*/, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 1, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, false /*modify_db*/, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}


static void diagram_missing_parent_consistency(void)
{
    /* tests not yet implemented */
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[512] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create some diagrams */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   2 /*=diagram_id*/,
                                   5 /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name",
                                   "diagram_description",
                                   10222 /*=list_order*/
                                 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, false /*modify_db*/, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* no root, id-2 without parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}


/*
 * Copyright 2016-2016 Andreas Warnke
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
