/* File: consistency_feature_test.c; Copyright and License: see below */

#include "consistency_feature_test.h"
#include "ctrl_controller.h"
#include "tvec/tvec_setup.h"
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
static test_case_result_t change_diagram_type( test_fixture_t *fix );
static test_case_result_t delete_diagramelement( test_fixture_t *fix );

test_suite_t consistency_feature_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "consistency_feature_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "change_diagram_type", &change_diagram_type );
    test_suite_add_test_case( &result, "delete_diagramelement", &delete_diagramelement );
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

static test_case_result_t change_diagram_type( test_fixture_t *fix )
{
    assert( fix != NULL );

    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t delete_diagramelement( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    tvec_setup_t test_environ;
    tvec_setup_init( &test_environ, &((*fix).controller) );

    /* create 2 diagrams */
    const data_row_t root_diagram
        = tvec_setup_diagram( &test_environ, DATA_ROW_VOID, "root diag", DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );
    const data_row_t local_diagram
        = tvec_setup_diagram( &test_environ, root_diagram, "local diag", DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );

    /* create 3 classifiers */
    const data_row_t test_classifier = tvec_setup_classifier( &test_environ, "test classifier" );
    const data_row_t omni_classifier = tvec_setup_classifier( &test_environ, "omni classifier" );
    const data_row_t local_classifier = tvec_setup_classifier( &test_environ, "local classifier" );

    /* create 5 diagramelements */
    const data_row_t test_local_diagele
        = tvec_setup_diagramelement( &test_environ, local_diagram, test_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, root_diagram, test_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, local_diagram, omni_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, root_diagram, omni_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, local_diagram, local_classifier, DATA_ROW_VOID );

    /* create 1 feature */
    const data_row_t test_feature = tvec_setup_feature( &test_environ, test_classifier, "test feature" );

    tvec_setup_destroy( &test_environ );

    /* root diag / DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    /*     test classifier */
    /*         test feature  ----, */
    /*     omni classifier  <----' */
    /* local diag / DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    /*     test classifier   (< will be deleted) */
    /*         test feature  ----,  ----, */
    /*     omni classifier  <----'      | */
    /*     local classifier  <----------' */

    /* delete the local diagramelement of the test classifier */
    const u8_error_t c_err
        = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl, test_local_diagele, CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, c_err );

    /* is test_feature deleted? */
    data_feature_t probe;
    const u8_error_t local_err
        = data_database_reader_get_feature_by_id( &((*fix).db_reader), test_feature, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, local_err );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2018-2026 Andreas Warnke
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
