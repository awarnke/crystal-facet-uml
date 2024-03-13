/* File: ctrl_diagram_controller_test.c; Copyright and License: see below */

#include "ctrl_diagram_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_diagram_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <stdio.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t create_read_modify_read( test_fixture_t *fix );
static test_case_result_t create_diagramelements_and_delete( test_fixture_t *fix );

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
    const static data_row_id_t PARENT_ID = 17;
    data_row_id_t diagram_id;
    uint32_t read_diagrams_count;
    data_diagram_t read_diagrams[2];
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );

    /* create a record */
    {
        diagram_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_private_create_child_diagram ( diag_ctrl, PARENT_ID, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, "diagram_name", &diagram_id );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_ID_VOID != diagram_id );
    }

    /* read this record */
    {
        data_diagram_init_empty( &(read_diagrams[0]) );
        data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), diagram_id, &(read_diagrams[0]) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row_id( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_row_id( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_const( &(read_diagrams[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_const( &(read_diagrams[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );
    }

    /* modify this record */
    {
        ctrl_err = ctrl_diagram_controller_update_diagram_description ( diag_ctrl, diagram_id, "'new' diagram\ndescription" );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_update_diagram_name ( diag_ctrl, diagram_id, "\"new\" diagram name" );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_diagram_controller_update_diagram_type( diag_ctrl,
                                                                diagram_id,
                                                                DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
                                                                &stat
                                                              );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);

        ctrl_err = ctrl_diagram_controller_update_diagram_list_order ( diag_ctrl, diagram_id, -4321 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* search several records, result array too small */
    {
        data_err = data_database_reader_get_diagrams_by_parent_id ( &((*fix).db_reader), PARENT_ID, 0, &read_diagrams, &read_diagrams_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, data_err );
        TEST_EXPECT_EQUAL_INT( 0, read_diagrams_count );
        /* check that old data is not overwritten: */
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row_id( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_row_id( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_const( &(read_diagrams[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_const( &(read_diagrams[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );
    }

    /* search several records, result array sufficient */
    {
        data_err = data_database_reader_get_diagrams_by_parent_id ( &((*fix).db_reader), PARENT_ID, 2, &read_diagrams, &read_diagrams_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, read_diagrams_count );
        /* check that new data is available */
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row_id( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_row_id( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "\"new\" diagram name", data_diagram_get_name_const( &(read_diagrams[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "'new' diagram\ndescription", data_diagram_get_description_const( &(read_diagrams[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( -4321, data_diagram_get_list_order( &(read_diagrams[0]) ) );
    }

    /* search several record ids to a result set */
    {
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        data_err = data_database_reader_get_diagram_ids_by_parent_id ( &((*fix).db_reader), PARENT_ID, &the_set );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, data_small_set_get_count( &the_set ) );
        /* check that new data is available */
        data_id_t the_set_entry = data_small_set_get_id( &the_set, 0 );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_id_get_row_id( &the_set_entry ) );
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
    data_row_id_t diagram_id;
    data_row_id_t classifier_id;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    uint32_t read_vis_classifiers_count;
    data_visible_classifier_t read_vis_classifiers[2];
    data_diagramelement_t *diag_element_ptr;
    data_row_id_t diag_element_id;
    data_diagramelement_t diag_element;
    uint32_t out_diagram_count;
    data_diagram_t out_diagram[2];

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
                                                                   &diag_element_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_EXPECT( DATA_ROW_ID_VOID != diag_element_id );
    }

    /* get the id of the diagramelement */
    {
        data_err = data_database_reader_get_classifiers_by_diagram_id( &((*fix).db_reader),
                                                                       diagram_id,
                                                                       2,
                                                                       &read_vis_classifiers,
                                                                       &read_vis_classifiers_count
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, read_vis_classifiers_count );
        diag_element_ptr = data_visible_classifier_get_diagramelement_ptr( &(read_vis_classifiers[0]) );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_row_id( diag_element_ptr ) );
        TEST_EXPECT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_row_id( diag_element_ptr ) );
        diag_element_id = data_diagramelement_get_row_id( diag_element_ptr );
    }

    /* get the data_diagramelement_t by id */
    {
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), diag_element_id, &diag_element );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_row_id( &diag_element ) );
        TEST_EXPECT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_row_id( &diag_element ) );
        TEST_EXPECT_EQUAL_INT( diag_element_id, data_diagramelement_get_row_id( &diag_element ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_focused_feature_row_id( &diag_element ) );
    }

    /* get all diagrams by classifier id */
    {
        data_err = data_database_reader_get_diagrams_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       2,
                                                                       &out_diagram,
                                                                       &out_diagram_count
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, out_diagram_count );
        TEST_EXPECT_EQUAL_INT( diagram_id, data_diagram_get_row_id( &(out_diagram[0]) ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagram_get_parent_row_id( &(out_diagram[0]) ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, data_diagram_get_diagram_type( &(out_diagram[0]) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "root_diagram", data_diagram_get_name_const( &(out_diagram[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_const( &(out_diagram[0]) ) ) );
        TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &(out_diagram[0]) ) );
    }

    /* delete the diagramelement */
    {
        ctrl_err = ctrl_diagram_controller_delete_diagramelement( diagram_ctrl,
                                                                  diag_element_id,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* get the deleted data_diagramelement_t */
    {
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), diag_element_id, &diag_element );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    /* get all diagrams by classifier id */
    {
        data_err = data_database_reader_get_diagrams_by_classifier_id ( &((*fix).db_reader), classifier_id, 2, &out_diagram, &out_diagram_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, out_diagram_count );
    }
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2016-2024 Andreas Warnke
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
