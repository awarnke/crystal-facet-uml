/* File: consistency_drop_invisibles_test.c; Copyright and License: see below */

#include "consistency_drop_invisibles_test.h"
#include "ctrl_controller.h"
#include "test_env/test_env_setup_data.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8/u8_trace.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t no_hidden_relationships( test_fixture_t *fix );

test_suite_t consistency_drop_invisibles_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "consistency_drop_invisibles_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "no_hidden_relationships", &no_hidden_relationships );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
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
    data_database_writer_init( &((*fix).db_writer), &((*fix).db_reader), &((*fix).database) );
    ctrl_controller_init( &((*fix).controller), &((*fix).database) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_controller_destroy( &((*fix).controller) );
    data_database_writer_destroy( &((*fix).db_writer) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static test_case_result_t no_hidden_relationships( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    test_env_setup_t test_environ;
    test_env_setup_init( &test_environ, &((*fix).controller) );

    /* create 2 diagrams */
    const data_row_id_t root_diagram = test_env_setup_data_create_diagram( &test_environ, DATA_ROW_ID_VOID, "root diag" );
    const data_row_id_t local_diagram = test_env_setup_data_create_diagram( &test_environ, root_diagram, "local diag" );

    /* create 3 classifiers */
    const data_row_id_t test_classifier = test_env_setup_data_create_classifier( &test_environ, "test classifier" );
    const data_row_id_t omni_classifier = test_env_setup_data_create_classifier( &test_environ, "omni classifier" );
    const data_row_id_t local_classifier = test_env_setup_data_create_classifier( &test_environ, "local classifier" );

    /* create 5 diagramelements */
    const data_row_id_t test_local_diagele
        = test_env_setup_data_create_diagramelement( &test_environ, local_diagram, test_classifier, DATA_ROW_ID_VOID );
    test_env_setup_data_create_diagramelement( &test_environ, root_diagram, test_classifier, DATA_ROW_ID_VOID );
    test_env_setup_data_create_diagramelement( &test_environ, local_diagram, omni_classifier, DATA_ROW_ID_VOID );
    test_env_setup_data_create_diagramelement( &test_environ, root_diagram, omni_classifier, DATA_ROW_ID_VOID );
    test_env_setup_data_create_diagramelement( &test_environ, local_diagram, local_classifier, DATA_ROW_ID_VOID );

    /* create 1 feature */
    const data_row_id_t test_feature = test_env_setup_data_create_feature( &test_environ, test_classifier, "test feature" );

    /* create 2 relationships */
    const data_row_id_t double_rel
        = test_env_setup_data_create_relationship( &test_environ,
                                                   test_classifier, test_feature,
                                                   omni_classifier, DATA_ROW_ID_VOID,
                                                   "double relation" );
    const data_row_id_t local_rel
        = test_env_setup_data_create_relationship( &test_environ,
                                                   test_classifier, test_feature,
                                                   local_classifier, DATA_ROW_ID_VOID,
                                                   "local relation" );
    test_env_setup_destroy( &test_environ );

    /* delete the local diagramelement of the test classifier */
    const u8_error_t c_err
        = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl, test_local_diagele, CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, c_err );

    /* is local rel deleted? */
    data_relationship_t probe;
    const u8_error_t local_err
        = data_database_reader_get_relationship_by_id( &((*fix).db_reader), local_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, local_err );

    /* is double rel existing? */
    const u8_error_t double_err
        = data_database_reader_get_relationship_by_id( &((*fix).db_reader), double_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, double_err );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2018-2024 Andreas Warnke
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
