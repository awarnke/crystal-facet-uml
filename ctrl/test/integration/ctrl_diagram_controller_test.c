/* File: ctrl_diagram_controller_test.c; Copyright and License: see below */

#include "ctrl_diagram_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_diagram_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "entity/data_diagram.h"
#include "entity/data_diagram_type.h"
#include "tvec/tvec_setup.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <stdio.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t create_read_modify_read( test_fixture_t *fix );
static test_case_result_t create_diagramelements_and_delete( test_fixture_t *fix );
static test_case_result_t modify_type( test_fixture_t *fix );

test_suite_t ctrl_diagram_controller_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "ctrl_diagram_controller_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "create_read_modify_read", &create_read_modify_read );
    test_suite_add_test_case( &result, "create_diagramelements_and_delete", &create_diagramelements_and_delete );
    test_suite_add_test_case( &result, "modify_type", &modify_type );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    ctrl_controller_t controller;  /*!< controller instance on which the tests are performed */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    data_database_init( &((*fix).database) );
    data_database_open_in_memory( &((*fix).database) );
    data_database_reader_init( &((*fix).db_reader), &((*fix).database) );
    ctrl_controller_init( &((*fix).controller), &((*fix).database) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_controller_destroy( &((*fix).controller) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static test_case_result_t create_read_modify_read( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    const static data_row_t PARENT_ID = 17;
    data_row_t diagram_id;
    data_diagram_t read_diagram;
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );

    /* create a record */
    {
        diagram_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_private_create_child_diagram ( diag_ctrl, PARENT_ID, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, "diagram_name", &diagram_id );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
        TEST_EXPECT( DATA_ROW_VOID != diagram_id );
    }

    /* read this record */
    {
        data_diagram_init_empty( &(read_diagram) );
        data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), diagram_id, &(read_diagram) );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row( &(read_diagram) ) );
        TEST_EXPECT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_row( &(read_diagram) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagram) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_const( &(read_diagram) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_const( &(read_diagram) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagram) ) );
    }

    /* modify this record */
    {
        ctrl_err = ctrl_diagram_controller_update_diagram_description ( diag_ctrl, diagram_id, "'new' diagram\ndescription" );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );

        ctrl_err = ctrl_diagram_controller_update_diagram_name ( diag_ctrl, diagram_id, "\"new\" diagram name" );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );

        consistency_stat_t stat;
        consistency_stat_init( &stat );
        ctrl_err = ctrl_diagram_controller_update_diagram_type( diag_ctrl,
                                                                diagram_id,
                                                                DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
                                                                &stat
                                                              );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_total_count( &stat ) );
        consistency_stat_destroy(&stat);

        ctrl_err = ctrl_diagram_controller_update_diagram_list_order ( diag_ctrl, diagram_id, -4321 );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
    }

    /* search several records */
    {
        data_diagram_iterator_t diagram_iterator;
        data_diagram_iterator_init_empty( &diagram_iterator );
        data_err = data_database_reader_get_diagrams_by_parent_id ( &((*fix).db_reader), PARENT_ID, &diagram_iterator );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( true, data_diagram_iterator_has_next( &diagram_iterator ) );
        /* check that new data is available */
        data_err = data_diagram_iterator_next( &diagram_iterator, &(read_diagram) );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row( &(read_diagram) ) );
        TEST_EXPECT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_row( &(read_diagram) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagram) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "\"new\" diagram name", data_diagram_get_name_const( &(read_diagram) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "'new' diagram\ndescription", data_diagram_get_description_const( &(read_diagram) ) ) );
        TEST_EXPECT_EQUAL_INT( -4321, data_diagram_get_list_order( &(read_diagram) ) );
        /* check for end */
        TEST_EXPECT_EQUAL_INT( false, data_diagram_iterator_has_next( &diagram_iterator ) );
        data_err = data_diagram_iterator_destroy( &diagram_iterator );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
    }

    /* search several record ids to a result set */
    {
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        data_err = data_database_reader_get_diagram_ids_by_parent_id ( &((*fix).db_reader), PARENT_ID, &the_set );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( 1, data_small_set_get_count( &the_set ) );
        /* check that new data is available */
        data_id_t the_set_entry = data_small_set_get_id( &the_set, 0 );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_id_get_row( &the_set_entry ) );
        TEST_EXPECT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &the_set_entry ) );
        data_small_set_destroy( &the_set );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t create_diagramelements_and_delete( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;
    data_row_t diagram_id;
    data_row_t classifier_id;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    data_visible_classifier_t read_vis_classifier;
    data_diagramelement_t *diag_element_ptr;
    data_row_t diag_element_id;
    data_diagramelement_t diag_element;
    data_diagram_t out_diagram;

    /* create the root diagram */
    {
        diagram_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diagram_ctrl,
                                                                               DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                               "root_diagram",
                                                                               &diagram_id
                                                                             );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
        TEST_EXPECT( DATA_ROW_VOID != diagram_id );
    }

    /* create a classifier */
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new ( &new_classifier,
                                              DATA_CLASSIFIER_TYPE_INTERFACE,
                                              "",  /* stereotype */
                                              "my_if",
                                              "",  /* description */
                                              88,
                                              8800,
                                              880000
                                            );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                  &classifier_id
                                                                );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
        data_classifier_destroy ( &new_classifier );
        TEST_EXPECT( DATA_ROW_VOID != classifier_id );
    }

    /* create a diagramelement */
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       diagram_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );
        data_id_t created_lifeline;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &diag_element_id,
                                                                   &created_lifeline
                                                                 );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( false, data_id_is_valid( &created_lifeline ) );
        data_diagramelement_destroy ( &new_diagele );
        TEST_EXPECT( DATA_ROW_VOID != diag_element_id );
    }

    /* get the id of the diagramelement */
    {
        data_visible_classifier_iterator_t visible_classifier_iterator;
        data_visible_classifier_iterator_init_empty( &visible_classifier_iterator );
        data_err = data_database_reader_get_visible_classifiers_by_diagram_id( &((*fix).db_reader),
                                                                               diagram_id,
                                                                               &visible_classifier_iterator
                                                                             );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( true, data_visible_classifier_iterator_has_next( &visible_classifier_iterator ) );

        data_err = data_visible_classifier_iterator_next( &visible_classifier_iterator, &read_vis_classifier );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        diag_element_ptr = data_visible_classifier_get_diagramelement_ptr( &read_vis_classifier );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_row( diag_element_ptr ) );
        TEST_EXPECT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_row( diag_element_ptr ) );
        diag_element_id = data_diagramelement_get_row( diag_element_ptr );

        TEST_EXPECT_EQUAL_INT( false, data_visible_classifier_iterator_has_next( &visible_classifier_iterator ) );
        data_err = data_visible_classifier_iterator_destroy( &visible_classifier_iterator );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
    }

    /* get the data_diagramelement_t by id */
    {
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), diag_element_id, &diag_element );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_row( &diag_element ) );
        TEST_EXPECT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_row( &diag_element ) );
        TEST_EXPECT_EQUAL_INT( diag_element_id, data_diagramelement_get_row( &diag_element ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagramelement_get_focused_feature_row( &diag_element ) );
    }

    /* get all diagrams by classifier id */
    {
        data_diagram_iterator_t diagram_iterator;
        data_diagram_iterator_init_empty( &diagram_iterator );

        data_err = data_database_reader_get_diagrams_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       &diagram_iterator
                                                                     );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( true, data_diagram_iterator_has_next( &diagram_iterator ) );

        data_err = data_diagram_iterator_next( &diagram_iterator, &(out_diagram) );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row( &(out_diagram) ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagram_get_parent_row( &(out_diagram) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, data_diagram_get_diagram_type( &(out_diagram) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "root_diagram", data_diagram_get_name_const( &(out_diagram) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_const( &(out_diagram) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &(out_diagram) ) );

        TEST_EXPECT_EQUAL_INT( false, data_diagram_iterator_has_next( &diagram_iterator ) );
        data_err = data_diagram_iterator_destroy( &diagram_iterator );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
    }

    /* delete the diagramelement */
    {
        consistency_stat_t stat;
        consistency_stat_init( &stat );
        ctrl_err = ctrl_diagram_controller_delete_diagramelement( diagram_ctrl,
                                                                  diag_element_id,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                  &stat
                                                                );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, ctrl_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_classifiers( &stat ) );
        TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_total_count( &stat ) );
        consistency_stat_destroy( &stat );
    }

    /* get the deleted data_diagramelement_t */
    {
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), diag_element_id, &diag_element );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_DB_STRUCTURE, data_err, u8_error_get_name );
    }

    /* get all diagrams by classifier id */
    {
        data_diagram_iterator_t diagram_iterator;
        data_diagram_iterator_init_empty( &diagram_iterator );

        data_err = data_database_reader_get_diagrams_by_classifier_id ( &((*fix).db_reader), classifier_id, &diagram_iterator );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( false, data_diagram_iterator_has_next( &diagram_iterator ) );

        data_err = data_diagram_iterator_destroy( &diagram_iterator );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t modify_type( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* setup a model to test */
    tvec_setup_t test_environ;
    tvec_setup_init( &test_environ, &((*fix).controller) );

    /* create 2 diagrams */
    const data_row_t root_diagram
        = tvec_setup_diagram( &test_environ, DATA_ROW_VOID, "root diag", DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM );
    const data_row_t test_diagram
        = tvec_setup_diagram( &test_environ, root_diagram, "test diag", DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM );

    /* create 2 classifiers */
    const data_row_t omni_classifier = tvec_setup_classifier( &test_environ, "omni classifier" );
    const data_row_t test_classifier = tvec_setup_classifier( &test_environ, "test classifier" );

    /* create 3 diagramelements */
    tvec_setup_diagramelement( &test_environ, root_diagram, omni_classifier );
    tvec_setup_diagramelement( &test_environ, test_diagram, omni_classifier );
    tvec_setup_diagramelement( &test_environ, test_diagram, test_classifier );

    /* create 2 features */
    tvec_setup_feature( &test_environ, omni_classifier, "omni@feature" );
    const data_row_t test_feature
        = tvec_setup_feature( &test_environ, test_classifier, "test@feature" );

    /* create 2 relationships which will get deleted for different reasons */
    tvec_setup_relationship( &test_environ,
                             omni_classifier, omni_classifier,
                             test_classifier, test_feature,
                             "test 1 relation"
                           );
    tvec_setup_relationship( &test_environ,
                             test_classifier, DATA_ROW_VOID,
                             test_classifier, DATA_ROW_VOID,
                             "test 2 relation"
                           );

    tvec_setup_destroy( &test_environ );

    /* do some type changes of the diagram */
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );

    {
        consistency_stat_t stat;
        consistency_stat_init( &stat );
        const u8_error_t err1
            = ctrl_diagram_controller_update_diagram_type( diag_ctrl,
                                                           test_diagram,
                                                           DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                                           &stat
                                                         );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, err1, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_classifiers( &stat ) );
        TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_features( &stat ) );
        TEST_EXPECT_EQUAL_INT( -2, consistency_stat_get_relationships( &stat ) );
        TEST_EXPECT_EQUAL_INT( 2, consistency_stat_get_lifelines( &stat ) );

        consistency_stat_destroy( &stat );
    }

    {
        consistency_stat_t stat;
        consistency_stat_init( &stat );
        const u8_error_t err1
        = ctrl_diagram_controller_update_diagram_type( diag_ctrl,
                                                       test_diagram,
                                                       DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                       &stat
        );
        TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, err1, u8_error_get_name );
        TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_classifiers( &stat ) );
        TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_features( &stat ) );
        TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_relationships( &stat ) );
        TEST_EXPECT_EQUAL_INT( -2, consistency_stat_get_lifelines( &stat ) );

        consistency_stat_destroy( &stat );
    }

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2016-2026 Andreas Warnke
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
