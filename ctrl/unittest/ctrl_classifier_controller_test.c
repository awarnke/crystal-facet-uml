/* File: ctrl_classifier_controller_test.c; Copyright and License: see below */

#include "ctrl_classifier_controller_test.h"
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
static void classifier_create_read_modify_read(void);
static void create_diagramelements_and_delete(void);
static void features_CRURDR(void);
static void relationship_CRURDR(void);

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
        new_TestFixture("classifier_create_read_modify_read",classifier_create_read_modify_read),
        new_TestFixture("create_diagramelements_and_delete",create_diagramelements_and_delete),
        new_TestFixture("features_CRURDR",features_CRURDR),
        new_TestFixture("relationship_CRURDR",relationship_CRURDR),
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

static void classifier_create_read_modify_read(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    const static int64_t DIAGRAM_ID = 19;
    int64_t classifier_id;
    int64_t diagele_id;
    data_classifier_t read_classifier;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;
    uint32_t read_vis_classifiers_count;
    data_visible_classifier_t read_vis_classifiers[2];
    data_classifier_t *first_classifier;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr ( &controller );

    /* create a record */
    data_classifier_t new_classifier;
    data_err = data_classifier_init_new ( &new_classifier,
                                          DATA_CLASSIFIER_TYPE_UML_COMPONENT,
                                          "",  /* stereotype */
                                          "my_component",
                                          "",  /* description */
                                          45,
                                          4500
                                        );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                              &new_classifier,
                                                              false,  /* add_to_latest_undo_set */
                                                              &classifier_id
                                                            );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    data_classifier_destroy ( &new_classifier );
    TEST_ASSERT( DATA_ID_VOID_ID != classifier_id );

    data_diagramelement_t new_diagele;
    data_diagramelement_init_new ( &new_diagele,
                                   DIAGRAM_ID,
                                   classifier_id,
                                   DATA_DIAGRAMELEMENT_FLAG_NONE
                                 );
    ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                               &new_diagele,
                                                               true,  /* add_to_latest_undo_set */
                                                               &diagele_id
                                                             );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    data_diagramelement_destroy ( &new_diagele );
    TEST_ASSERT( DATA_ID_VOID_ID != diagele_id );

    /* read this record */
    data_classifier_init_empty( &read_classifier );
    data_err = data_database_reader_get_classifier_by_id ( &db_reader, classifier_id, &read_classifier );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( classifier_id, data_classifier_get_id( &read_classifier ) );
    TEST_ASSERT_EQUAL_INT( DATA_CLASSIFIER_TYPE_UML_COMPONENT, data_classifier_get_main_type( &read_classifier ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_classifier_get_stereotype_ptr( &read_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "my_component", data_classifier_get_name_ptr( &read_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_classifier_get_description_ptr( &read_classifier ) ) );
    TEST_ASSERT_EQUAL_INT( 45, data_classifier_get_x_order( &read_classifier ) );
    TEST_ASSERT_EQUAL_INT( 4500, data_classifier_get_y_order( &read_classifier ) );

    /* modify this record */
    ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( classifier_ctrl, classifier_id, "my_new_stereotype" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_description ( classifier_ctrl, classifier_id, "my_new_classifier_description" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_name ( classifier_ctrl, classifier_id, "my_node" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_main_type ( classifier_ctrl, classifier_id, DATA_CLASSIFIER_TYPE_UML_NODE );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_x_order ( classifier_ctrl, classifier_id, 6789 );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_classifier_y_order ( classifier_ctrl, classifier_id, 789 );
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
    TEST_ASSERT_EQUAL_INT( 6789, data_classifier_get_x_order( first_classifier ) );
    TEST_ASSERT_EQUAL_INT( 789, data_classifier_get_y_order( first_classifier ) );
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
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diagram_ctrl,
                                                                           DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                           "root_diagram",
                                                                           &diagram_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT( DATA_ID_VOID_ID != diagram_id );

    /* create a classifier */
    data_classifier_t new_classifier;
    data_err = data_classifier_init_new ( &new_classifier,
                                          DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                          "",  /* stereotype */
                                          "my_if",
                                          "",  /* description */
                                          88,
                                          8800
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                              &new_classifier,
                                                              false,  /* add_to_latest_undo_set */
                                                              &classifier_id
    );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    data_classifier_destroy ( &new_classifier );
    TEST_ASSERT( DATA_ID_VOID_ID != classifier_id );

    data_diagramelement_t new_diagele;
    data_diagramelement_init_new ( &new_diagele,
                                   diagram_id,
                                   classifier_id,
                                   DATA_DIAGRAMELEMENT_FLAG_NONE
    );
    ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                               &new_diagele,
                                                               true,  /* add_to_latest_undo_set */
                                                               &diag_element_id
    );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    data_diagramelement_destroy ( &new_diagele );
    TEST_ASSERT( DATA_ID_VOID_ID != diag_element_id );

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
    TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, data_diagram_get_diagram_type( &(out_diagram[0]) ) );
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

static void features_CRURDR(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* define a feature */
    data_feature_t probe;
    data_err = data_feature_init ( &probe,
                                   17, /* feature_id */
                                   DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                   35000, /* classifier_id */
                                   "startup_time", /* feature_key */
                                   "uint64_t", /* feature_value */
                                   "time in nano seconds to start", /* feature_description */
                                   5000000 /* list order */ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create the feature in the db */
    int64_t new_feature_id;
    ctrl_err = ctrl_classifier_controller_create_feature ( classifier_ctrl,
                                                           &probe,
                                                           false, /* add_to_latest_undo_set */
                                                           &new_feature_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* check what was written to the database */
    data_feature_t check;
    data_err = data_database_reader_get_feature_by_id ( &db_reader, new_feature_id, &check );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( new_feature_id, data_feature_get_id( &check ) );
    TEST_ASSERT_EQUAL_INT( DATA_FEATURE_TYPE_PROPERTY, data_feature_get_main_type( &check ) );
    TEST_ASSERT_EQUAL_INT( 35000, data_feature_get_classifier_id( &check ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "startup_time", data_feature_get_key_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "uint64_t", data_feature_get_value_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "time in nano seconds to start", data_feature_get_description_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 5000000, data_feature_get_list_order( &check ) );

    ctrl_err = ctrl_classifier_controller_update_feature_main_type ( classifier_ctrl,
                                                                     new_feature_id,
                                                                     DATA_FEATURE_TYPE_OPERATION );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_feature_key ( classifier_ctrl,
                                                               new_feature_id,
                                                               "get_startup_time()" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_feature_value ( classifier_ctrl,
                                                                 new_feature_id,
                                                                 "(void)->(uint64_t)" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_feature_description ( classifier_ctrl,
                                                                       new_feature_id,
                                                                       "gets the startup time in nanoseconds" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_feature_list_order ( classifier_ctrl,
                                                                      new_feature_id,
                                                                      -40 );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* check what was changed in the database */
    data_err = data_database_reader_get_feature_by_id ( &db_reader, new_feature_id, &check );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( new_feature_id, data_feature_get_id( &check ) );
    TEST_ASSERT_EQUAL_INT( DATA_FEATURE_TYPE_OPERATION, data_feature_get_main_type( &check ) );
    TEST_ASSERT_EQUAL_INT( 35000, data_feature_get_classifier_id( &check ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "get_startup_time()", data_feature_get_key_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "(void)->(uint64_t)", data_feature_get_value_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "gets the startup time in nanoseconds", data_feature_get_description_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( -40, data_feature_get_list_order( &check ) );

    /* delete the feature from the database */
    data_small_set_t small_set;
    data_id_t element_id;
    data_small_set_init( &small_set );
    data_id_init( &element_id, DATA_TABLE_FEATURE, new_feature_id );
    data_err = data_small_set_add_obj ( &small_set, element_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    ctrl_err = ctrl_classifier_controller_delete_set ( classifier_ctrl, small_set );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* check what was deleted in the database */
    data_err = data_database_reader_get_feature_by_id ( &db_reader, new_feature_id, &check );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_DB_STRUCTURE, data_err );
}

static void relationship_CRURDR(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* define a relationship */
    data_relationship_t probe;
    data_err = data_relationship_init ( &probe,
                                        34, /* relationship_id */
                                        DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                        86000, /* from_classifier_id */
                                        86001, /* to_classifier_id */
                                        "the composition is more", /* relationship_name */
                                        "than the sum of its parts", /* relationship_description */
                                        -66000 /* list_order */ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create the relationship in the db */
    int64_t new_relationship_id;
    ctrl_err = ctrl_classifier_controller_create_relationship ( classifier_ctrl,
                                                                &probe,
                                                                false, /* add_to_latest_undo_set */
                                                                &new_relationship_id );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* check what was written to the database */
    data_relationship_t check;
    data_err = data_database_reader_get_relationship_by_id ( &db_reader, new_relationship_id, &check );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( new_relationship_id, data_relationship_get_id( &check ) );
    TEST_ASSERT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, data_relationship_get_main_type( &check ) );
    TEST_ASSERT_EQUAL_INT( 86000, data_relationship_get_from_classifier_id( &check ) );
    TEST_ASSERT_EQUAL_INT( 86001, data_relationship_get_to_classifier_id( &check ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "the composition is more", data_relationship_get_name_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "than the sum of its parts", data_relationship_get_description_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( -66000, data_relationship_get_list_order( &check ) );

    ctrl_err = ctrl_classifier_controller_update_relationship_main_type ( classifier_ctrl,
                                                                          new_relationship_id,
                                                                          DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_relationship_name ( classifier_ctrl,
                                                                     new_relationship_id,
                                                                     "async message" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_relationship_description ( classifier_ctrl,
                                                                            new_relationship_id,
                                                                            "good for modularization" );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    ctrl_err = ctrl_classifier_controller_update_relationship_list_order ( classifier_ctrl,
                                                                           new_relationship_id,
                                                                           -88000 );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* check what was changed in the database */
    data_err = data_database_reader_get_relationship_by_id ( &db_reader, new_relationship_id, &check );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( new_relationship_id, data_relationship_get_id( &check ) );
    TEST_ASSERT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, data_relationship_get_main_type( &check ) );
    TEST_ASSERT_EQUAL_INT( 86000, data_relationship_get_from_classifier_id( &check ) );
    TEST_ASSERT_EQUAL_INT( 86001, data_relationship_get_to_classifier_id( &check ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "async message", data_relationship_get_name_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "good for modularization", data_relationship_get_description_ptr( &check ) ) );
    TEST_ASSERT_EQUAL_INT( -88000, data_relationship_get_list_order( &check ) );

    /* delete the relationship from the database */
    data_small_set_t small_set;
    data_id_t element_id;
    data_small_set_init( &small_set );
    data_id_init( &element_id, DATA_TABLE_RELATIONSHIP, new_relationship_id );
    data_err = data_small_set_add_obj ( &small_set, element_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    ctrl_err = ctrl_classifier_controller_delete_set ( classifier_ctrl, small_set );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

    /* check what was deleted in the database */
    data_err = data_database_reader_get_relationship_by_id ( &db_reader, new_relationship_id, &check );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_DB_STRUCTURE, data_err );
}


/*
 * Copyright 2016-2017 Andreas Warnke
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
