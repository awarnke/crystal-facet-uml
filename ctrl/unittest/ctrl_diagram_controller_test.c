/* File: ctrl_diagram_controller_test.c; Copyright and License: see below */

#include "ctrl_diagram_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_diagram_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void create_read_modify_read(void);

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
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

TestRef ctrl_diagram_controller_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("create_read_modify_read",create_read_modify_read),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_diagram_controller_test",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int err;
    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
}

static void create_read_modify_read(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    const static int64_t PARENT_ID = 17;
    int64_t diagram_id;
    uint32_t read_diagrams_count;
    data_diagram_t read_diagrams[2];
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a record */

    diagram_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_diagram_controller_create_child_diagram ( diag_ctrl, PARENT_ID, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, "diagram_name", &diagram_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != diagram_id );

    /* read this record */

    data_diagram_init_empty( &(read_diagrams[0]) );
    data_err = data_database_reader_get_diagram_by_id ( &db_reader, diagram_id, &(read_diagrams[0]) );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_id( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_ptr( &(read_diagrams[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &(read_diagrams[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );

    /* modify this record */

    ctrl_err = ctrl_diagram_controller_update_diagram_description ( diag_ctrl, diagram_id, "'new' diagram\ndescription" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_diagram_controller_update_diagram_name ( diag_ctrl, diagram_id, "\"new\" diagram name" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl, diagram_id, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* search several records, result array too small */

    data_err = data_database_reader_get_diagrams_by_parent_id ( &db_reader, PARENT_ID, 0, &read_diagrams, &read_diagrams_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_ARRAY_BUFFER_EXCEEDED, data_err );
    TEST_ASSERT_EQUAL_INT( 0, read_diagrams_count );
    /* check that old data is not overwritten: */
    TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_id( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_ptr( &(read_diagrams[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &(read_diagrams[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );

    /* search several records, result array sufficient */

    data_err = data_database_reader_get_diagrams_by_parent_id ( &db_reader, PARENT_ID, 2, &read_diagrams, &read_diagrams_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, read_diagrams_count );
    /* check that new data is available */
    TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_id( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "\"new\" diagram name", data_diagram_get_name_ptr( &(read_diagrams[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "'new' diagram\ndescription", data_diagram_get_description_ptr( &(read_diagrams[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );
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
