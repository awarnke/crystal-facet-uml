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
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    tvec_setup_t test_environ;
    tvec_setup_init( &test_environ, &((*fix).controller) );

    /* create 3 diagrams */
    const data_row_t test_diagram
        = tvec_setup_diagram( &test_environ, DATA_ROW_VOID, "test diag", DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );
    const data_row_t seq_diagram
        = tvec_setup_diagram( &test_environ, test_diagram, "seq diag", DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );
    const data_row_t class_diagram
        = tvec_setup_diagram( &test_environ, test_diagram, "class diag", DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );

    /* create 1 classifier and 3 diagramelements */
    const data_row_t semi_classifier = tvec_setup_classifier( &test_environ, "semi classifier" );
    tvec_setup_diagramelement( &test_environ, test_diagram, semi_classifier );
    const data_row_t semi_life
        = tvec_setup_lifeline( &test_environ, seq_diagram, semi_classifier, NULL /* ignore out_diagele_id */  );
    (void) semi_life;  /* lifeline not needed */

    /* create 1 feature */
    const data_row_t semi_feature = tvec_setup_feature( &test_environ, semi_classifier, "semi feature" );

    /* create 1 classifier and 3 diagramelements */
    const data_row_t omni_classifier = tvec_setup_classifier( &test_environ, "omni classifier" );
    tvec_setup_diagramelement( &test_environ, test_diagram, omni_classifier );
    const data_row_t omni_life
        = tvec_setup_lifeline( &test_environ, seq_diagram, omni_classifier, NULL /* ignore out_diagele_id */  );
    (void) omni_life;  /* lifeline not needed */
    tvec_setup_diagramelement( &test_environ, class_diagram, omni_classifier );

    /* create 1 feature */
    const data_row_t omni_feature = tvec_setup_feature( &test_environ, omni_classifier, "omni feature" );

    tvec_setup_destroy( &test_environ );

    /* test diag / DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM (< change to DATA_DIAGRAM_TYPE_BOX_DIAGRAM ) */
    /*     semi classifier */
    /*         semi feature */
    /*     omni classifier */
    /*         omni feature */
    /* seq diag / DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    /*     semi classifier */
    /*         semi feature */
    /*     omni classifier */
    /*         omni feature */
    /* class diag / DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    /*     omni classifier */
    /*         omni feature */

    /* change the type of the test diagram */
    consistency_stat_t statistics = CONSISTENCY_STAT_ZERO;
    const u8_error_t c_err
        = ctrl_diagram_controller_update_diagram_type( diagram_ctrl,
                                                       test_diagram,
                                                       DATA_DIAGRAM_TYPE_BOX_DIAGRAM,
                                                       &statistics
                                                     );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, c_err, u8_error_get_name );
    TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_features( &statistics ) );
    TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_total_count( &statistics ) );
    consistency_stat_destroy( &statistics );

    /* is semi_feature deleted? */
    data_feature_t probe;
    const u8_error_t semi_err
        = data_database_reader_get_feature_by_id( &((*fix).db_reader), semi_feature, &probe );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_DB_STRUCTURE, semi_err, u8_error_get_name );

    /* is omni_feature deleted? */
    const u8_error_t omni_err
        = data_database_reader_get_feature_by_id( &((*fix).db_reader), omni_feature, &probe );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, omni_err, u8_error_get_name );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t delete_diagramelement( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    tvec_setup_t test_environ;
    tvec_setup_init( &test_environ, &((*fix).controller) );

    /* create 3 diagrams */
    const data_row_t box_diagram
        = tvec_setup_diagram( &test_environ, DATA_ROW_VOID, "box diag", DATA_DIAGRAM_TYPE_BOX_DIAGRAM );
    const data_row_t seq_diagram
        = tvec_setup_diagram( &test_environ, box_diagram, "seq diag", DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );
    const data_row_t class_diagram
        = tvec_setup_diagram( &test_environ, box_diagram, "class diag", DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );

    /* create 1 classifier and 3 diagramelements */
    const data_row_t test_classifier = tvec_setup_classifier( &test_environ, "test classifier" );
    tvec_setup_diagramelement( &test_environ, box_diagram, test_classifier );
    data_row_t test_seq_diagele;
    const data_row_t test_life
        = tvec_setup_lifeline( &test_environ, seq_diagram, test_classifier, &test_seq_diagele );
    (void) test_life;  /* lifeline not needed */
    tvec_setup_diagramelement( &test_environ, class_diagram, test_classifier );

    /* create 1 feature */
    const data_row_t test_feature = tvec_setup_feature( &test_environ, test_classifier, "test feature" );

    /* create 1 classifier and 3 diagramelements */
    const data_row_t omni_classifier = tvec_setup_classifier( &test_environ, "omni classifier" );
    tvec_setup_diagramelement( &test_environ, box_diagram, omni_classifier );
    const data_row_t omni_life
        = tvec_setup_lifeline( &test_environ, seq_diagram, omni_classifier, NULL /* ignore out_diagele_id */ );
    (void) omni_life;  /* lifeline not needed */
    const data_row_t omni_class_diagele
        = tvec_setup_diagramelement( &test_environ, class_diagram, omni_classifier );

    /* create 1 feature */
    const data_row_t omni_feature = tvec_setup_feature( &test_environ, omni_classifier, "omni feature" );

    tvec_setup_destroy( &test_environ );

    /* box diag / DATA_DIAGRAM_TYPE_BOX_DIAGRAM */
    /*     test classifier */
    /*         test feature */
    /*     omni classifier */
    /*         omni feature */
    /* seq diag / DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    /*     test classifier   (< diagramelement will be deleted) */
    /*         test feature */
    /*     omni classifier */
    /*         omni feature */
    /* class diag / DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    /*     test classifier */
    /*         test feature */
    /*     omni classifier (< diagramelement will be deleted) */
    /*         omni feature */

    /* delete the test diagramelement @ seq diag */
    consistency_stat_t statistics = CONSISTENCY_STAT_ZERO;
    const u8_error_t c_err1
        = ctrl_diagram_controller_delete_diagramelement( diagram_ctrl,
                                                         test_seq_diagele,
                                                         CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                         &statistics
                                                       );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, c_err1, u8_error_get_name );
    TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_lifelines( &statistics ) );
    TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_total_count( &statistics ) );

    /* is test_feature deleted? */
    data_feature_t probe;
    const u8_error_t test_err
        = data_database_reader_get_feature_by_id( &((*fix).db_reader), test_feature, &probe );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, test_err, u8_error_get_name );

    /* delete the omni diagramelement @ class diag */
    statistics = CONSISTENCY_STAT_ZERO;
    const u8_error_t c_err2
        = ctrl_diagram_controller_delete_diagramelement( diagram_ctrl,
                                                         omni_class_diagele,
                                                         CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                         &statistics
                                                       );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, c_err2, u8_error_get_name );
    TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_classifiers( &statistics ) );
    TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_features( &statistics ) );  /* the omni_feature is not visible in box or seq diagrams */
    TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_lifelines( &statistics ) );
    TEST_EXPECT_EQUAL_INT( 0, consistency_stat_get_relationships( &statistics ) );
    TEST_EXPECT_EQUAL_INT( -1, consistency_stat_get_total_count( &statistics ) );

    /* is omni_feature deleted? */
    const u8_error_t omni_err
    = data_database_reader_get_feature_by_id( &((*fix).db_reader), omni_feature, &probe );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_DB_STRUCTURE, omni_err, u8_error_get_name );

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
