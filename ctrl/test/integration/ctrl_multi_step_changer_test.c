/* File: ctrl_multi_step_changer_test.c; Copyright and License: see below */

#include "ctrl_multi_step_changer_test.h"
#include "ctrl_multi_step_changer.h"
#include "ctrl_controller.h"
#include "test_env/test_env_setup_data.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "set/data_visible_classifier.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t delete_set_successfully( test_fixture_t *fix );
static test_case_result_t delete_set_not_possible( test_fixture_t *fix );

test_suite_t ctrl_multi_step_changer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "ctrl_multi_step_changer_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "delete_set_successfully", &delete_set_successfully );
    test_suite_add_test_case( &result, "delete_set_not_possible", &delete_set_not_possible );
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

static test_case_result_t delete_set_not_possible( test_fixture_t *fix )
{
    assert( fix != NULL );
    test_env_setup_t test_environ;
    test_env_setup_init( &test_environ, &((*fix).controller) );

    /* create 2 diagrams */
    const data_row_id_t root_diagram = test_env_setup_data_create_diagram( &test_environ, DATA_ROW_ID_VOID, "root diag" );
    test_env_setup_data_create_diagram( &test_environ, root_diagram, "test diag" );

    /* create 1 classifiers */
    const data_row_id_t test_classifier = test_env_setup_data_create_classifier( &test_environ, "test classifier" );

    /* create 1 diagramelement */
    test_env_setup_data_create_diagramelement( &test_environ, root_diagram, test_classifier, DATA_ROW_ID_VOID );

    test_env_setup_destroy( &test_environ );

    /* try to delete each type once from the database */
    {
        data_small_set_t small_set;
        u8_error_t add_ok;
        data_small_set_init( &small_set );

        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_CLASSIFIER, test_classifier );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_FEATURE, (data_row_id_t)50000 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_RELATIONSHIP, (data_row_id_t)50000 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_DIAGRAMELEMENT, (data_row_id_t)50000 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_DIAGRAM, root_diagram );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );

        ctrl_multi_step_changer_t multi_stepper;
        ctrl_multi_step_changer_init( &multi_stepper, &((*fix).controller), &((*fix).db_reader) );
        data_stat_t stat;
        data_stat_init(&stat);

        const u8_error_t ctrl_err
            = ctrl_multi_step_changer_delete_set ( &multi_stepper, &small_set, &stat );

        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE | U8_ERROR_OBJECT_STILL_REFERENCED, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 5, data_stat_get_total_count ( &stat ));

        data_stat_destroy(&stat);
        ctrl_multi_step_changer_destroy( &multi_stepper );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t delete_set_successfully( test_fixture_t *fix )
{
    assert( fix != NULL );
    test_env_setup_t test_environ;
    test_env_setup_init( &test_environ, &((*fix).controller) );

    /* create 2 diagrams */
    const data_row_id_t root_diagram = test_env_setup_data_create_diagram( &test_environ, DATA_ROW_ID_VOID, "root diag" );
    const data_row_id_t test_diagram = test_env_setup_data_create_diagram( &test_environ, root_diagram, "test diag" );

    /* create 3 classifiers */
    const data_row_id_t test_classifier = test_env_setup_data_create_classifier( &test_environ, "test classifier" );
    const data_row_id_t omni_classifier = test_env_setup_data_create_classifier( &test_environ, "omni classifier" );
    const data_row_id_t orphaned_classifier = test_env_setup_data_create_classifier( &test_environ, "orphaned classifier" );

    /* create 2 diagramelements */
    test_env_setup_data_create_diagramelement( &test_environ, root_diagram, omni_classifier, DATA_ROW_ID_VOID );
    const data_row_id_t test_diagele
        = test_env_setup_data_create_diagramelement( &test_environ, test_diagram, test_classifier, DATA_ROW_ID_VOID );

    /* create 2 features */
    const data_row_id_t test_feature = test_env_setup_data_create_feature( &test_environ, test_classifier, "test feature" );
    const data_row_id_t omni_feature = test_env_setup_data_create_feature( &test_environ, omni_classifier, "omni feature" );

    /* create 2 relationships */
    const data_row_id_t test_rel
        = test_env_setup_data_create_relationship( &test_environ,
                                                   omni_classifier, DATA_ROW_ID_VOID,
                                                   omni_classifier, omni_feature,
                                                   "test relation" );
    test_env_setup_data_create_relationship( &test_environ,
                                             test_classifier, test_feature,
                                             omni_classifier, DATA_ROW_ID_VOID,
                                             "collateral relation" );

    test_env_setup_destroy( &test_environ );

    /* delete each type once from the database */
    {
        data_small_set_t small_set;
        u8_error_t add_ok;
        data_small_set_init( &small_set );

        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_CLASSIFIER, orphaned_classifier );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_FEATURE, test_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_RELATIONSHIP, test_rel );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_DIAGRAMELEMENT, test_diagele );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );
        add_ok = data_small_set_add_row_id( &small_set, DATA_TABLE_DIAGRAM, test_diagram );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, add_ok );

        ctrl_multi_step_changer_t multi_stepper;
        ctrl_multi_step_changer_init( &multi_stepper, &((*fix).controller), &((*fix).db_reader) );
        data_stat_t stat;
        data_stat_init(&stat);

        const u8_error_t ctrl_err
            = ctrl_multi_step_changer_delete_set ( &multi_stepper, &small_set, &stat );

        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        /* expected deleted classifiers: orphaned_classifier, test_classifier (via test_diagram ) */
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED ));
        /* expected deleted features: test_feature */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
        /* The diagram type is DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM - therefore 1 lifeline is additionally deleted: */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 8, data_stat_get_total_count ( &stat ));

        data_stat_destroy(&stat);
        ctrl_multi_step_changer_destroy( &multi_stepper );
    }

    /* check the set was really deleted in the database */
    {
        data_feature_t check2;
        const u8_error_t data_err2
            = data_database_reader_get_feature_by_id ( &((*fix).db_reader), test_feature, &check2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err2 );
    }
    {
        data_diagramelement_t check3;
        const u8_error_t data_err3
            = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), test_diagele, &check3 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err3 );
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
