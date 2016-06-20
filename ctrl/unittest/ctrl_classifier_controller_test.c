/* File: ctrl_classifier_controller_test.c; Copyright and License: see below */

#include "ctrl_classifier_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "data_visible_classifier.h"
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

TestRef ctrl_classifier_controller_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("create_read_modify_read",create_read_modify_read),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_classifier_controller_test",set_up,tear_down,fixtures);

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
    const static int64_t DIAGRAM_ID = 19;
    int64_t classifier_id;
    data_classifier_t read_classifier;
    ctrl_classifier_controller_t *classifier_ctrl;
    uint32_t read_vis_classifiers_count;
    data_visible_classifier_t read_vis_classifiers[2];
    data_classifier_t *first_classifier;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* create a record */

    classifier_id = -1;
    ctrl_err = ctrl_classifier_controller_create_classifier_in_diagram ( classifier_ctrl, DIAGRAM_ID, DATA_CLASSIFIER_TYPE_UML_COMPONENT, "my_component", &classifier_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( -1 != classifier_id );

    /* read this record */

    data_classifier_init_empty( &read_classifier );
    data_err = data_database_reader_get_classifier_by_id ( &db_reader, classifier_id, &read_classifier );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( classifier_id, data_classifier_get_id( &read_classifier ) );
    TEST_ASSERT_EQUAL_INT( DATA_CLASSIFIER_TYPE_UML_COMPONENT, data_classifier_get_main_type( &read_classifier ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_classifier_get_stereotype_ptr( &read_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_component", data_classifier_get_name_ptr( &read_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_classifier_get_description_ptr( &read_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, data_classifier_get_x_order( &read_classifier ) );
    TEST_ASSERT_EQUAL_INT( 0, data_classifier_get_y_order( &read_classifier ) );

    /* modify this record */
    ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( classifier_ctrl, classifier_id, "my_new_stereotype" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_description ( classifier_ctrl, classifier_id, "my_new_classifier_description" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_name ( classifier_ctrl, classifier_id, "my_node" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_main_type ( classifier_ctrl, classifier_id, DATA_CLASSIFIER_TYPE_UML_NODE );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* search several records, result array too small */

    data_visible_classifier_init_empty( &(read_vis_classifiers[0]) );
    data_visible_classifier_init_empty( &(read_vis_classifiers[1]) );
    first_classifier = data_visible_classifier_get_classifier_ptr( &(read_vis_classifiers[0]) );
    data_classifier_init ( first_classifier, 0x1234, DATA_CLASSIFIER_TYPE_UML_COMPONENT, "stereo", "my_name", "descr", 1000, 400 );

    data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, DIAGRAM_ID, 0, &read_vis_classifiers, &read_vis_classifiers_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_ARRAY_BUFFER_EXCEEDED, data_err );
    TEST_ASSERT_EQUAL_INT( 0, read_vis_classifiers_count );
    /* check that old data is not overwritten: */
    TEST_ASSERT_EQUAL_INT( 0x1234, data_classifier_get_id( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( DATA_CLASSIFIER_TYPE_UML_COMPONENT, data_classifier_get_main_type( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "stereo", data_classifier_get_stereotype_ptr( first_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_name", data_classifier_get_name_ptr( first_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "descr", data_classifier_get_description_ptr( first_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 1000, data_classifier_get_x_order( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( 400, data_classifier_get_y_order( first_classifier ) );

    /* search several records, result array sufficient */

    data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, DIAGRAM_ID, 2, &read_vis_classifiers, &read_vis_classifiers_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, read_vis_classifiers_count );
    /* check that new data is available */
    TEST_ASSERT_EQUAL_INT( classifier_id, data_classifier_get_id( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( DATA_CLASSIFIER_TYPE_UML_NODE, data_classifier_get_main_type( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_new_stereotype", data_classifier_get_stereotype_ptr( first_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_node", data_classifier_get_name_ptr( first_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_new_classifier_description", data_classifier_get_description_ptr( first_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, data_classifier_get_x_order( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( 0, data_classifier_get_y_order( first_classifier ) );
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
