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
static void create_diagramelements_and_delete(void);

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
        new_TestFixture("create_diagramelements_and_delete",create_diagramelements_and_delete),
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

    classifier_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_classifier_controller_create_classifier_in_diagram ( classifier_ctrl, DIAGRAM_ID, DATA_CLASSIFIER_TYPE_UML_COMPONENT, "my_component", &classifier_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != classifier_id );

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
    data_classifier_reinit ( first_classifier, 0x1234, DATA_CLASSIFIER_TYPE_UML_COMPONENT, "stereo", "my_name", "descr", 1000, 400 );

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

static void create_diagramelements_and_delete(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;
    int64_t diagram_id;
    int64_t classifier_id;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    uint32_t read_vis_classifiers_count;
    data_visible_classifier_t read_vis_classifiers[2];
    data_diagramelement_t *diag_element_ptr;
    int64_t diag_element_id;
    data_diagramelement_t diag_element;
    uint32_t out_diagram_count;
    data_diagram_t out_diagram[2];
    data_small_set_t small_set;
    data_id_t element_id;

    /* create the root diagram */
    diagram_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diagram_ctrl, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, "root_diagram", &diagram_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != diagram_id );

    /* create a classifier */
    classifier_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_classifier_controller_create_classifier_in_diagram ( classifier_ctrl, diagram_id, DATA_CLASSIFIER_TYPE_UML_INTERFACE, "my_if", &classifier_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != classifier_id );

    /* get the id of the diagramelement */
    data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, diagram_id, 2, &read_vis_classifiers, &read_vis_classifiers_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, read_vis_classifiers_count );
    diag_element_ptr = data_visible_classifier_get_diagramelement_ptr( &(read_vis_classifiers[0]) );
    TEST_ASSERT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_id( diag_element_ptr ) );
    TEST_ASSERT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_id( diag_element_ptr ) );
    diag_element_id = data_diagramelement_get_id( diag_element_ptr );

    /* get the data_diagramelement_t by id */
    data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, diag_element_id, &diag_element );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_id( &diag_element ) );
    TEST_ASSERT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_id( &diag_element ) );
    TEST_ASSERT_EQUAL_INT( diag_element_id, data_diagramelement_get_id( &diag_element ) );

    /* get all diagrams by classifier id */
    data_err = data_database_reader_get_diagrams_by_classifier_id ( &db_reader, classifier_id, 2, &out_diagram, &out_diagram_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, out_diagram_count );
    TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(out_diagram[0]) ) );
    TEST_ASSERT_EQUAL_INT( DATA_ID_VOID_ID, data_diagram_get_parent_id( &(out_diagram[0]) ) );
    TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, data_diagram_get_type( &(out_diagram[0]) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "root_diagram", data_diagram_get_name_ptr( &(out_diagram[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &(out_diagram[0]) ) ) );
    TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(out_diagram[0]) ) );

    /* delete the diagramelement */
    data_small_set_init( &small_set );
    data_id_init( &element_id, DATA_TABLE_DIAGRAMELEMENT, diag_element_id );
    data_err = data_small_set_add_obj ( &small_set, element_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    ctrl_err = ctrl_classifier_controller_delete_set ( classifier_ctrl, small_set );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* get the deleted data_diagramelement_t */
    data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, diag_element_id, &diag_element );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_DB_STRUCTURE, data_err );

    /* get all diagrams by classifier id */
    data_err = data_database_reader_get_diagrams_by_classifier_id ( &db_reader, classifier_id, 2, &out_diagram, &out_diagram_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 0, out_diagram_count );
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
