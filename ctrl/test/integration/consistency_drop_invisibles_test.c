/* File: consistency_drop_invisibles_test.c; Copyright and License: see below */

#include "consistency_drop_invisibles_test.h"
#include "ctrl_controller.h"
#include "test_env/test_env_setup_data.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8/u8_trace.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t no_hidden_relationships( test_fixture_t *test_env );

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

test_suite_t consistency_drop_invisibles_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "consistency_drop_invisibles_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "no_hidden_relationships", &no_hidden_relationships );
    return result;
}

static test_fixture_t * set_up()
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    ctrl_controller_init( &controller, &database );
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
    ctrl_controller_destroy( &controller );

    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static test_case_result_t no_hidden_relationships( test_fixture_t *test_env )
{
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    test_env_setup_t test_environ;
    test_env_setup_init( &test_environ, &controller );

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
        = data_database_reader_get_relationship_by_id( &db_reader, local_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, local_err );

    /* is double rel existing? */
    const u8_error_t double_err
        = data_database_reader_get_relationship_by_id( &db_reader, double_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, double_err );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2018-2023 Andreas Warnke
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
