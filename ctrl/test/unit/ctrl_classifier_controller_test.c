/* File: ctrl_classifier_controller_test.c; Copyright and License: see below */

#include "ctrl_classifier_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "set/data_visible_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static void set_up(void);
static void tear_down(void);
static void classifier_create_read_modify_read(void);
static void features_CRURDR(void);
static void relationship_CRURDR(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

test_suite_t ctrl_classifier_controller_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "ctrl_classifier_controller_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "classifier_create_read_modify_read", &classifier_create_read_modify_read );
    test_suite_add_test_case( &result, "features_CRURDR", &features_CRURDR );
    test_suite_add_test_case( &result, "relationship_CRURDR", &relationship_CRURDR );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static void classifier_create_read_modify_read(void)
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    data_row_id_t diagram_id;
    data_row_id_t classifier_id;
    data_row_id_t diagele_id;
    data_classifier_t read_classifier;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;
    uint32_t read_vis_classifiers_count;
    data_visible_classifier_t read_vis_classifiers[2];
    data_classifier_t *first_classifier;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr ( &controller );

    /* create a record */
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new ( &new_classifier,
                                              DATA_CLASSIFIER_TYPE_COMPONENT,
                                              "",  /* stereotype */
                                              "my_component",
                                              "",  /* description */
                                              45,
                                              4500,
                                              450000
                                            );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                  &classifier_id
                                                                );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_classifier_destroy ( &new_classifier );
        TEST_EXPECT( DATA_ROW_ID_VOID != classifier_id );
    }

    /* create the root diagram */
    {
        diagram_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diagram_ctrl,
                                                                               DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                               "root_diagram",
                                                                               &diagram_id
                                                                             );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_ID_VOID != diagram_id );
    }

    /* create a diagramelement */
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       diagram_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_ID_VOID
                                     );
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &diagele_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_EXPECT( DATA_ROW_ID_VOID != diagele_id );
    }

    /* read this record */
    {
        data_classifier_init_empty( &read_classifier );
        data_err = data_database_reader_get_classifier_by_id ( &db_reader, classifier_id, &read_classifier );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( classifier_id, data_classifier_get_row_id( &read_classifier ) );
        TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_COMPONENT, data_classifier_get_main_type( &read_classifier ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_classifier_get_stereotype_const( &read_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_component", data_classifier_get_name_const( &read_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_classifier_get_description_const( &read_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 45, data_classifier_get_x_order( &read_classifier ) );
        TEST_EXPECT_EQUAL_INT( 4500, data_classifier_get_y_order( &read_classifier ) );
        TEST_EXPECT_EQUAL_INT( 450000, data_classifier_get_list_order( &read_classifier ) );
    }

    /* modify this record */
    {
        ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( classifier_ctrl, classifier_id, "my_new_stereotype" );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_classifier_description ( classifier_ctrl, classifier_id, "my_new_classifier_description" );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_classifier_name ( classifier_ctrl, classifier_id, "my_node" );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_classifier_main_type ( classifier_ctrl, classifier_id, DATA_CLASSIFIER_TYPE_NODE );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_classifier_x_order ( classifier_ctrl, classifier_id, 6789 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_classifier_y_order ( classifier_ctrl, classifier_id, 789 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* search several records, result array too small */
    {
        data_visible_classifier_init_empty( &(read_vis_classifiers[0]) );
        data_visible_classifier_init_empty( &(read_vis_classifiers[1]) );
        first_classifier = data_visible_classifier_get_classifier_ptr( &(read_vis_classifiers[0]) );
        data_uuid_t uuid;
        data_uuid_init_new( &uuid );

        data_classifier_reinit( first_classifier,
                                0x1234,
                                DATA_CLASSIFIER_TYPE_COMPONENT,
                                "stereo",
                                "my_name",
                                "descr",
                                1000,
                                400,
                                398,
                                data_uuid_get_string( &uuid )
                              );

        data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, diagram_id, 0, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, data_err );
        TEST_EXPECT_EQUAL_INT( 0, read_vis_classifiers_count );
        /* check that old data is not overwritten: */
        TEST_EXPECT_EQUAL_INT( 0x1234, data_classifier_get_row_id( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_COMPONENT, data_classifier_get_main_type( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "stereo", data_classifier_get_stereotype_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_name", data_classifier_get_name_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "descr", data_classifier_get_description_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 1000, data_classifier_get_x_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 400, data_classifier_get_y_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 398, data_classifier_get_list_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( data_uuid_get_string( &uuid ), data_classifier_get_uuid_const( first_classifier ) ) );

        data_uuid_destroy( &uuid );
    }

    /* search several records, result array sufficient */
    {
        data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, diagram_id, 2, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, read_vis_classifiers_count );
        /* check that new data is available */
        TEST_EXPECT_EQUAL_INT( classifier_id, data_classifier_get_row_id( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_NODE, data_classifier_get_main_type( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_new_stereotype", data_classifier_get_stereotype_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_node", data_classifier_get_name_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_new_classifier_description", data_classifier_get_description_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 6789, data_classifier_get_x_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 789, data_classifier_get_y_order( first_classifier ) );
    }
}

static void features_CRURDR(void)
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    data_uuid_t uuid;
    data_uuid_init_new( &uuid );

    /* create a feature */
    data_row_id_t new_feature_id;
    {
        data_feature_t probe;
        data_err = data_feature_init ( &probe,
                                       17, /* feature_id */
                                       DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                       35000, /* classifier_id */
                                       "startup_time", /* feature_key */
                                       "uint64_t", /* feature_value */
                                       "time in nano seconds to start", /* feature_description */
                                       5000000, /* list order */
                                       data_uuid_get_string( &uuid )
                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        ctrl_err = ctrl_classifier_controller_create_feature ( classifier_ctrl,
                                                               &probe,
                                                               CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                               &new_feature_id
                                                             );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check what was written to the database */
    {
        data_feature_t check;
        data_err = data_database_reader_get_feature_by_id ( &db_reader, new_feature_id, &check );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_feature_id, data_feature_get_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_PROPERTY, data_feature_get_main_type( &check ) );
        TEST_EXPECT_EQUAL_INT( 35000, data_feature_get_classifier_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "startup_time", data_feature_get_key_const( &check ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "uint64_t", data_feature_get_value_const( &check ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "time in nano seconds to start", data_feature_get_description_const( &check ) ) );
        TEST_EXPECT_EQUAL_INT( 5000000, data_feature_get_list_order( &check ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( data_uuid_get_string( &uuid ), data_feature_get_uuid_const( &check ) ) );
    }

    /* modify the feature */
    {
        ctrl_err = ctrl_classifier_controller_update_feature_main_type ( classifier_ctrl,
                                                                         new_feature_id,
                                                                         DATA_FEATURE_TYPE_OPERATION
                                                                       );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_feature_key ( classifier_ctrl,
                                                                   new_feature_id,
                                                                   "get_startup_time()"
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_feature_value ( classifier_ctrl,
                                                                     new_feature_id,
                                                                     "(void)->(uint64_t)"
                                                                   );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_feature_description ( classifier_ctrl,
                                                                           new_feature_id,
                                                                           "gets the startup time in nanoseconds"
                                                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_feature_list_order ( classifier_ctrl,
                                                                          new_feature_id,
                                                                          -40
                                                                        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check what was changed in the database */
    {
        data_feature_t check2;
        data_err = data_database_reader_get_feature_by_id ( &db_reader, new_feature_id, &check2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_feature_id, data_feature_get_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_OPERATION, data_feature_get_main_type( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 35000, data_feature_get_classifier_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "get_startup_time()", data_feature_get_key_const( &check2 ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "(void)->(uint64_t)", data_feature_get_value_const( &check2 ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "gets the startup time in nanoseconds", data_feature_get_description_const( &check2 ) ) );
        TEST_EXPECT_EQUAL_INT( -40, data_feature_get_list_order( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( data_uuid_get_string( &uuid ), data_feature_get_uuid_const( &check2 ) ) );
    }

    /* delete the feature from the database */
    {
        data_err = ctrl_classifier_controller_delete_feature ( classifier_ctrl,
                                                               new_feature_id,
                                                               CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                             );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check what was deleted in the database */
    {
        data_feature_t check3;
        data_err = data_database_reader_get_feature_by_id ( &db_reader, new_feature_id, &check3 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    data_uuid_destroy( &uuid );
}

static void relationship_CRURDR(void)
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    data_uuid_t uuid;
    data_uuid_init_new( &uuid );

    /* create a relationship */
    data_row_id_t new_relationship_id;
    {
        data_relationship_t probe;
        data_err = data_relationship_init ( &probe,
                                            34, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            86000, /* from_classifier_id */
                                            86001, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            -66000, /* list_order */
                                            DATA_ROW_ID_VOID, /* from_feature_id */
                                            100666, /* to_feature_id */
                                            data_uuid_get_string( &uuid )
                                          );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        ctrl_err = ctrl_classifier_controller_create_relationship ( classifier_ctrl,
                                                                    &probe,
                                                                    CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                    &new_relationship_id
                                                                  );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check what was written to the database */
    {
        data_relationship_t check;
        data_err = data_database_reader_get_relationship_by_id ( &db_reader, new_relationship_id, &check );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_relationship_id, data_relationship_get_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, data_relationship_get_main_type( &check ) );
        TEST_EXPECT_EQUAL_INT( 86000, data_relationship_get_from_classifier_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( 86001, data_relationship_get_to_classifier_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "the composition is more", data_relationship_get_name_const( &check ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "than the sum of its parts", data_relationship_get_description_const( &check ) ) );
        TEST_EXPECT_EQUAL_INT( -66000, data_relationship_get_list_order( &check ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( 100666, data_relationship_get_to_feature_row_id( &check ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( data_uuid_get_string( &uuid ), data_relationship_get_uuid_const( &check ) ) );
    }

    /* modify the relationship */
    {
        ctrl_err = ctrl_classifier_controller_update_relationship_main_type ( classifier_ctrl,
                                                                              new_relationship_id,
                                                                              DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL
                                                                            );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_relationship_name ( classifier_ctrl,
                                                                         new_relationship_id,
                                                                         "async message"
                                                                       );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_relationship_description ( classifier_ctrl,
                                                                                new_relationship_id,
                                                                                "good for modularization"
                                                                              );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_classifier_controller_update_relationship_list_order ( classifier_ctrl,
                                                                               new_relationship_id,
                                                                               -88000
                                                                             );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check what was changed in the database */
    {
        data_relationship_t check2;
        data_err = data_database_reader_get_relationship_by_id ( &db_reader, new_relationship_id, &check2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_relationship_id, data_relationship_get_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, data_relationship_get_main_type( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 86000, data_relationship_get_from_classifier_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 86001, data_relationship_get_to_classifier_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "async message", data_relationship_get_name_const( &check2 ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "good for modularization", data_relationship_get_description_const( &check2 ) ) );
        TEST_EXPECT_EQUAL_INT( -88000, data_relationship_get_list_order( &check2 ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 100666, data_relationship_get_to_feature_row_id( &check2 ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( data_uuid_get_string( &uuid ), data_relationship_get_uuid_const( &check2 ) ) );
    }

    /* delete the relationship from the database */
    {
        data_err = ctrl_classifier_controller_delete_relationship( classifier_ctrl,
                                                                   new_relationship_id,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check what was deleted in the database */
    {
        data_relationship_t check3;
        data_err = data_database_reader_get_relationship_by_id ( &db_reader, new_relationship_id, &check3 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    data_uuid_destroy( &uuid );
}


/*
 * Copyright 2016-2022 Andreas Warnke
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
