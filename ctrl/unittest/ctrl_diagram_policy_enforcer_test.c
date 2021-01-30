/* File: ctrl_diagram_policy_enforcer_test.c; Copyright and License: see below */

#include "ctrl_diagram_policy_enforcer_test.h"
#include "ctrl_controller.h"
#include "test_env/test_env_setup_data.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void diagram_to_lifeline_consistency(void);
static void diagramelement_to_lifeline_consistency(void);
static void no_hidden_relationships(void);

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
    test_suite_add_test_case( &result, "no_hidden_relationships", &no_hidden_relationships );
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
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
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
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
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
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
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
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
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
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
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
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_delete_diagram ( diagram_ctrl,
                                                            child_diag_id,
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
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
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
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
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
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
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
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
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
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
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
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

static void no_hidden_relationships(void)
{
    /* TODO */
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );
    ctrl_error_t c_err;
    
    /* create 2 diagrams */
    const data_row_id_t root_diagram = test_env_setup_data_create_diagram( DATA_ROW_ID_VOID, "root diag", &controller );
    const data_row_id_t local_diagram = test_env_setup_data_create_diagram( root_diagram, "local diag", &controller );
    
    /* create 3 classifiers */
    const data_row_id_t test_classifier = test_env_setup_data_create_classifier( "test classifier", &controller );
    const data_row_id_t omni_classifier = test_env_setup_data_create_classifier( "omni classifier", &controller );
    const data_row_id_t local_classifier = test_env_setup_data_create_classifier( "local classifier", &controller );
    
    /* create 5 diagramelements */
    const data_row_id_t test_local_diagele 
        = test_env_setup_data_create_diagramelement( local_diagram, test_classifier, DATA_ROW_ID_VOID, &controller );
    const data_row_id_t test_root_diagele 
        = test_env_setup_data_create_diagramelement( root_diagram, test_classifier, DATA_ROW_ID_VOID, &controller );
    const data_row_id_t omni_local_diagele
        = test_env_setup_data_create_diagramelement( local_diagram, omni_classifier, DATA_ROW_ID_VOID, &controller );
    const data_row_id_t omni_root_diagele
        = test_env_setup_data_create_diagramelement( root_diagram, omni_classifier, DATA_ROW_ID_VOID, &controller );
    const data_row_id_t local_local_diagele
        = test_env_setup_data_create_diagramelement( local_diagram, local_classifier, DATA_ROW_ID_VOID, &controller );

    /* create 1 feature */
    const data_row_id_t test_feature = test_env_setup_data_create_feature( test_classifier, "test feature", &controller );

    /* create 2 relationships */
    const data_row_id_t double_rel 
        = test_env_setup_data_create_relationship( test_classifier, test_feature,
                                                   omni_classifier, DATA_ROW_ID_VOID,
                                                   "double relation", &controller );
    const data_row_id_t local_rel 
        = test_env_setup_data_create_relationship( test_classifier, test_feature,
                                                   local_classifier, DATA_ROW_ID_VOID,
                                                   "local relation", &controller );
    
    
}


/*
 * Copyright 2018-2021 Andreas Warnke
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
