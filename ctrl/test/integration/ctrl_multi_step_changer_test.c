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
#include "test_expect.h"
#include "test_environment_assert.h"

static void set_up(void);
static void tear_down(void);
static void delete_set_successfully(void);
static void delete_set_not_possible(void);

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

test_suite_t ctrl_multi_step_changer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "ctrl_multi_step_changer_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "delete_set_successfully", &delete_set_successfully );
    test_suite_add_test_case( &result, "delete_set_not_possible", &delete_set_not_possible );
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

static void delete_set_not_possible(void)
{
    test_env_setup_t test_env;
    test_env_setup_init( &test_env, &controller );

    /* create 2 diagrams */
    const data_row_id_t root_diagram = test_env_setup_data_create_diagram( &test_env, DATA_ROW_ID_VOID, "root diag" );
    test_env_setup_data_create_diagram( &test_env, root_diagram, "test diag" );

    /* create 1 classifiers */
    const data_row_id_t test_classifier = test_env_setup_data_create_classifier( &test_env, "test classifier" );

    /* create 1 diagramelement */
    test_env_setup_data_create_diagramelement( &test_env, root_diagram, test_classifier, DATA_ROW_ID_VOID );

    test_env_setup_destroy( &test_env );

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
        ctrl_multi_step_changer_init( &multi_stepper, &controller, &db_reader );
        data_stat_t stat;
        data_stat_init(&stat);

        const u8_error_t ctrl_err
            = ctrl_multi_step_changer_delete_set ( &multi_stepper, &small_set, &stat );

        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE | U8_ERROR_OBJECT_STILL_REFERENCED, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR ));
        TEST_EXPECT_EQUAL_INT( 5, data_stat_get_total_count ( &stat ));

        data_stat_destroy(&stat);
        ctrl_multi_step_changer_destroy( &multi_stepper );
    }
}

static void delete_set_successfully(void)
{
    test_env_setup_t test_env;
    test_env_setup_init( &test_env, &controller );

    /* create 2 diagrams */
    const data_row_id_t root_diagram = test_env_setup_data_create_diagram( &test_env, DATA_ROW_ID_VOID, "root diag" );
    const data_row_id_t test_diagram = test_env_setup_data_create_diagram( &test_env, root_diagram, "test diag" );

    /* create 3 classifiers */
    const data_row_id_t test_classifier = test_env_setup_data_create_classifier( &test_env, "test classifier" );
    const data_row_id_t omni_classifier = test_env_setup_data_create_classifier( &test_env, "omni classifier" );
    const data_row_id_t orphaned_classifier = test_env_setup_data_create_classifier( &test_env, "orphaned classifier" );

    /* create 2 diagramelements */
    test_env_setup_data_create_diagramelement( &test_env, root_diagram, omni_classifier, DATA_ROW_ID_VOID );
    const data_row_id_t test_diagele
        = test_env_setup_data_create_diagramelement( &test_env, test_diagram, test_classifier, DATA_ROW_ID_VOID );

    /* create 2 features */
    const data_row_id_t test_feature = test_env_setup_data_create_feature( &test_env, test_classifier, "test feature" );
    const data_row_id_t omni_feature = test_env_setup_data_create_feature( &test_env, omni_classifier, "omni feature" );

    /* create 2 relationships */
    const data_row_id_t test_rel
        = test_env_setup_data_create_relationship( &test_env,
                                                   omni_classifier, DATA_ROW_ID_VOID,
                                                   omni_classifier, omni_feature,
                                                   "test relation" );
    test_env_setup_data_create_relationship( &test_env,
                                             test_classifier, test_feature,
                                             omni_classifier, DATA_ROW_ID_VOID,
                                             "collateral relation" );

    test_env_setup_destroy( &test_env );

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
        ctrl_multi_step_changer_init( &multi_stepper, &controller, &db_reader );
        data_stat_t stat;
        data_stat_init(&stat);

        const u8_error_t ctrl_err
            = ctrl_multi_step_changer_delete_set ( &multi_stepper, &small_set, &stat );

        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED ));
        /* The diagram type is DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM - therefore 1 lifeline is additionally deleted: */
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 8, data_stat_get_total_count ( &stat ));

        data_stat_destroy(&stat);
        ctrl_multi_step_changer_destroy( &multi_stepper );
    }

    /* check the set was really deleted in the database */
    {
        data_feature_t check2;
        const u8_error_t data_err2
            = data_database_reader_get_feature_by_id ( &db_reader, test_feature, &check2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err2 );
    }
    {
        data_diagramelement_t check3;
        const u8_error_t data_err3
            = data_database_reader_get_diagramelement_by_id ( &db_reader, test_diagele, &check3 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err3 );
    }
}


/*
 * Copyright 2016-2023 Andreas Warnke
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