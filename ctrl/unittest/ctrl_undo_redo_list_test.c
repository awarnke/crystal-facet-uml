/* File: ctrl_undo_redo_list_test.c; Copyright and License: see below */

#include "ctrl_undo_redo_list_test.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"

static void set_up(void);
static void tear_down(void);
static void undo_redo_classifier(void);
static void undo_redo_list_limits(void);

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

TestRef ctrl_undo_redo_list_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("undo_redo_classifier",undo_redo_classifier),
        new_TestFixture("undo_redo_list_limits",undo_redo_list_limits),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_undo_redo_list_test",set_up,tear_down,fixtures);

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

static void undo_redo_classifier(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    int64_t root_diagram_id;
    int64_t classifier_id;
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* create the root diagram */
    root_diagram_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diag_ctrl,
                                                                           DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM,
                                                                           "my_root_diag",
                                                                           &root_diagram_id
                                                                         );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != root_diagram_id );

    /* create a classifier and a diagramelement */
    classifier_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_classifier_controller_create_classifier_in_diagram ( classifier_ctrl,
                                                                         root_diagram_id,
                                                                         DATA_CLASSIFIER_TYPE_UML_NODE,
                                                                         "my_node",
                                                                         17,
                                                                         1700,
                                                                         &classifier_id
                                                                       );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != classifier_id );

    /* update the classifier */
    ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( classifier_ctrl, classifier_id, "my_stereo" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* undo classifier update */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* undo classifier and diagramelement creation */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    {
        uint32_t read_vis_classifiers_count;
        data_visible_classifier_t read_vis_classifiers[1];

        data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, root_diagram_id, 1, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 0, read_vis_classifiers_count );
    }

    /* undo root diagram creation */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    {
        data_diagram_t read_diagram;
        data_err = data_database_reader_get_diagram_by_id ( &db_reader, root_diagram_id, &read_diagram );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_DB_STRUCTURE, data_err );
    }

    /* redo root diagram creation */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    {
        data_diagram_t read_diagram;
        data_err = data_database_reader_get_diagram_by_id ( &db_reader, root_diagram_id, &read_diagram );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( root_diagram_id, data_diagram_get_id( &read_diagram ) );
        TEST_ASSERT_EQUAL_INT( DATA_ID_VOID_ID, data_diagram_get_parent_id( &read_diagram ) );
        TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, data_diagram_get_diagram_type( &read_diagram ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_root_diag", data_diagram_get_name_ptr( &read_diagram ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &read_diagram ) ) );
        TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &read_diagram ) );
    }

    /* redo classifier and diagramelement creation */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* redo classifier update */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    {
        uint32_t read_vis_classifiers_count;
        data_visible_classifier_t read_vis_classifiers[1];
        data_classifier_t *first_classifier;
        first_classifier = data_visible_classifier_get_classifier_ptr( &(read_vis_classifiers[0]) );

        data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, root_diagram_id, 1, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, read_vis_classifiers_count );
        TEST_ASSERT_EQUAL_INT( classifier_id, data_classifier_get_id( first_classifier ) );
        TEST_ASSERT_EQUAL_INT( DATA_CLASSIFIER_TYPE_UML_NODE, data_classifier_get_main_type( first_classifier ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_stereo", data_classifier_get_stereotype_ptr( first_classifier ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_node", data_classifier_get_name_ptr( first_classifier ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_classifier_get_description_ptr( first_classifier ) ) );
        TEST_ASSERT_EQUAL_INT( 17, data_classifier_get_x_order( first_classifier ) );
        TEST_ASSERT_EQUAL_INT( 1700, data_classifier_get_y_order( first_classifier ) );
    }
}

static void undo_redo_list_limits(void)
{
    /* tests not yet implemented */

    ctrl_error_t ctrl_err;
    int64_t root_diagram_id;
    int64_t child_diag_id;
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create the root diagram */
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diag_ctrl,
                                                                           DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM,
                                                                           "my_root_diag",
                                                                           &root_diagram_id
    );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* undo root diagram creation */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* undo at start of list */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_INVALID_REQUEST, ctrl_err );

    /* redo root diagram creation */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* redo at end of list */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_INVALID_REQUEST, ctrl_err );

    /* fill the list by creating classifiers and diagramelements */
    for ( int32_t pos = 0; pos < (CTRL_UNDO_REDO_LIST_MAX_SIZE-1/*first boundary*/-2/*diagram and boundary*/)/2/*list entries per diagram*/; pos ++ )
    {
        /* create a diagram */
        ctrl_err = ctrl_diagram_controller_create_child_diagram ( diag_ctrl,
                                                                  root_diagram_id,
                                                                  DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                                  "diagram_name",
                                                                  &child_diag_id );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* create one more classifier and diagramelement */
    ctrl_err = ctrl_diagram_controller_create_child_diagram ( diag_ctrl,
                                                              root_diagram_id,
                                                              DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                              "diagram_name",
                                                              &child_diag_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* undo everything that is possible */
    for ( int32_t pos = 0; pos < (CTRL_UNDO_REDO_LIST_MAX_SIZE-0/*first boundary is overwritten*/-2/*diagram and boundary*/)/2/*list entries per diagram*/; pos ++ )
    {
        ctrl_err = ctrl_controller_undo ( &controller );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* undo one more */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_ARRAY_BUFFER_EXCEEDED, ctrl_err );

    /* redo one */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* create a new diagram somewhere in the middle of the list */
    ctrl_err = ctrl_diagram_controller_create_child_diagram ( diag_ctrl,
                                                              root_diagram_id,
                                                              DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                              "diagram_name",
                                                              &child_diag_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* redo one but already at end of the list */
    ctrl_err = ctrl_controller_redo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_INVALID_REQUEST, ctrl_err );

    /* undo 2 existing and one not existing */
    /* undo one more */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    /* undo one more */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    /* undo one more */
    ctrl_err = ctrl_controller_undo ( &controller );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_ARRAY_BUFFER_EXCEEDED, ctrl_err );
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
