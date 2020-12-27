/* File: ctrl_diagram_policy_enforcer_test.c; Copyright and License: see below */

#include "ctrl_diagram_policy_enforcer_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void diagram_to_lifeline_consistency(void);
static void diagramelement_to_lifeline_consistency(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

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

test_suite_t ctrl_diagram_policy_enforcer_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "ctrl_diagram_policy_enforcer_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "diagram_to_lifeline_consistency", &diagram_to_lifeline_consistency );
    test_suite_add_test_case( &result, "diagramelement_to_lifeline_consistency", &diagramelement_to_lifeline_consistency );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    ctrl_controller_destroy( &controller );

    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static void diagram_to_lifeline_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    data_row_id_t root_diag_id;
    {
        data_diagram_t root_diagram;
        data_err = data_diagram_init ( &root_diagram,
                                       DATA_ROW_ID_VOID /*=diagram_id is ignored*/,
                                       DATA_ROW_ID_VOID /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                       "the_root_diag",
                                       "diagram_description-root",
                                       10555 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        root_diag_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &root_diagram,
                                                            false, /* add_to_latest_undo_set */
                                                            &root_diag_id
                                                          );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ROW_ID_VOID != root_diag_id );
        data_diagram_destroy ( &root_diagram );
    }

    /* create a second diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    data_row_id_t child_diag_id;
    {
        data_diagram_t child_diagram;
        data_err = data_diagram_init ( &child_diagram,
                                       DATA_ROW_ID_VOID /*=diagram_id is ignored*/,
                                       root_diag_id /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                       "the_child_diag",
                                       "diagram_description-child",
                                       20666 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        child_diag_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &child_diagram,
                                                            true, /* add_to_latest_undo_set */
                                                            &child_diag_id
                                                          );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( child_diag_id != DATA_ROW_ID_VOID );
        data_diagram_destroy ( &child_diagram );
    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_CLASS */
    data_row_id_t classifier_id;
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new ( &new_classifier,
                                              DATA_CLASSIFIER_TYPE_CLASS,
                                              "",  /* stereotype */
                                              "my_class",
                                              "",  /* description */
                                              45,
                                              4500,
                                              450000
                                            );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        classifier_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  true,  /* add_to_latest_undo_set */
                                                                  &classifier_id
                                                                );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ROW_ID_VOID != classifier_id );
        data_classifier_destroy ( &new_classifier );
    }

    /* create one diagramelement for the root diagram */
    data_row_id_t root_diag_element_id;
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_ID_VOID
                                     );

        root_diag_element_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   true,  /* add_to_latest_undo_set */
                                                                   &root_diag_element_id
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_ASSERT( DATA_ROW_ID_VOID != root_diag_element_id );
    }

    /* create one diagramelement for the child diagram */
    data_row_id_t child_diag_element_id;
    {
        data_diagramelement_t new_diagele2;
        data_diagramelement_init_new ( &new_diagele2,
                                       child_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_ID_VOID
                                     );

        child_diag_element_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele2,
                                                                   true,  /* add_to_latest_undo_set */
                                                                   &child_diag_element_id
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele2 );
        TEST_ASSERT( DATA_ROW_ID_VOID != child_diag_element_id );
    }

    /* change the type of the child diagram to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    {
        ctrl_err = ctrl_diagram_controller_update_diagram_type ( diagram_ctrl, child_diag_id, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* change the type of the child diagram to DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    {
        ctrl_err = ctrl_diagram_controller_update_diagram_type ( diagram_ctrl, child_diag_id, DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* change the type of the child diagram to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    {
        ctrl_err = ctrl_diagram_controller_update_diagram_type ( diagram_ctrl, child_diag_id, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* check that the classifier now has one feature of type DATA_FEATURE_TYPE_LIFELINE */
    static const uint32_t max_featues_size=2;
    data_feature_t features[2];
    uint32_t feature_count;
    {
        data_err = data_database_reader_get_features_by_classifier_id ( &db_reader,
                                                                        classifier_id,
                                                                        max_featues_size,
                                                                        &features,
                                                                        &feature_count
                                                                      );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, feature_count );
        TEST_ASSERT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &(features[0]) ) );
    }

    /* check that this is referenced */
    {
        data_diagramelement_t check_diagele2;
        data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, child_diag_element_id, &check_diagele2 );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        TEST_ASSERT_EQUAL_INT( data_feature_get_row_id( &(features[0])), data_diagramelement_get_focused_feature_row_id( &check_diagele2 ) );

        data_diagramelement_destroy ( &check_diagele2 );
    }

    /* delete the child diagram (but not the classifier) */
    {
        ctrl_err = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl,
                                                                   child_diag_element_id,
                                                                   false /* add_to_latest_undo_set */
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_delete_diagram ( diagram_ctrl,
                                                            child_diag_id,
                                                            true /* add_to_latest_undo_set */
                                                          );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* check that the feature of type DATA_FEATURE_TYPE_LIFELINE is deleted */
    {
        data_err = data_database_reader_get_features_by_classifier_id ( &db_reader,
                                                                        classifier_id,
                                                                        max_featues_size,
                                                                        &features,
                                                                        &feature_count
                                                                      );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 0, feature_count );
    }
}

static void diagramelement_to_lifeline_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    data_row_id_t root_diag_id;
    {
        data_diagram_t root_diagram;
        data_err = data_diagram_init ( &root_diagram,
                                       DATA_ROW_ID_VOID /*=diagram_id is ignored*/,
                                       DATA_ROW_ID_VOID /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                       "the_root_diag",
                                       "diagram_description-root",
                                       10555 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        root_diag_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &root_diagram,
                                                            false, /* add_to_latest_undo_set */
                                                            &root_diag_id
        );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ROW_ID_VOID != root_diag_id );
        data_diagram_destroy ( &root_diagram );
    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_CLASS */
    data_row_id_t classifier_id;
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new ( &new_classifier,
                                              DATA_CLASSIFIER_TYPE_CLASS,
                                              "",  /* stereotype */
                                              "my_class",
                                              "",  /* description */
                                              45,
                                              4500,
                                              450000
                                            );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        classifier_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  false,  /* add_to_latest_undo_set */
                                                                  &classifier_id
        );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ROW_ID_VOID != classifier_id );
        data_classifier_destroy ( &new_classifier );
    }

    /* create first diagramelement for the classifier */
    data_row_id_t first_diag_element_id;
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_ID_VOID
                                     );

        first_diag_element_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   true,  /* add_to_latest_undo_set */
                                                                   &first_diag_element_id
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_ASSERT( DATA_ROW_ID_VOID != first_diag_element_id );
    }

    /* create second diagramelement for the classifier */
    data_row_id_t second_diag_element_id;
    {
        data_diagramelement_t new_diagele2;
        data_diagramelement_init_new ( &new_diagele2,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_ID_VOID
                                     );

        second_diag_element_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele2,
                                                                   true,  /* add_to_latest_undo_set */
                                                                   &second_diag_element_id
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele2 );
        TEST_ASSERT( DATA_ROW_ID_VOID != second_diag_element_id );
    }

    /* check that the classifier now has two features of type DATA_FEATURE_TYPE_LIFELINE */
    static const uint32_t max_featues_size=3;
    data_feature_t features[3];
    uint32_t feature_count;
    {
        data_err = data_database_reader_get_features_by_classifier_id ( &db_reader,
                                                                        classifier_id,
                                                                        max_featues_size,
                                                                        &features,
                                                                        &feature_count
                                                                      );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 2, feature_count );
        TEST_ASSERT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &(features[0]) ) );
        TEST_ASSERT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &(features[1]) ) );
    }

    /* delete the first diagramelement (but not the classifier) */
    {
        ctrl_err = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl,
                                                                   first_diag_element_id,
                                                                   false /* add_to_latest_undo_set */
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* check that one feature of type DATA_FEATURE_TYPE_LIFELINE is deleted */
    {
        data_err = data_database_reader_get_features_by_classifier_id ( &db_reader,
                                                                        classifier_id,
                                                                        max_featues_size,
                                                                        &features,
                                                                        &feature_count
                                                                      );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, feature_count );
        TEST_ASSERT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &(features[0]) ) );
    }

    /* check that this is referenced */
    {
        data_diagramelement_t check_diagele2;
        data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, second_diag_element_id, &check_diagele2 );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        TEST_ASSERT_EQUAL_INT( data_feature_get_row_id( &(features[0])), data_diagramelement_get_focused_feature_row_id( &check_diagele2 ) );

        data_diagramelement_destroy ( &check_diagele2 );
    }
}


/*
 * Copyright 2018-2020 Andreas Warnke
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
