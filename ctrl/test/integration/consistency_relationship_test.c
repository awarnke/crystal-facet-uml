/* File: consistency_relationship_test.c; Copyright and License: see below */

#include "consistency_relationship_test.h"
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
static test_case_result_t consistency_check_error( test_fixture_t *fix );

test_suite_t consistency_relationship_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "consistency_relationship_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "change_diagram_type", &change_diagram_type );
    test_suite_add_test_case( &result, "delete_diagramelement", &delete_diagramelement );
    test_suite_add_test_case( &result, "consistency_check_error", &consistency_check_error );
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
    tvec_setup_diagramelement( &test_environ, test_diagram, semi_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, seq_diagram, semi_classifier, DATA_ROW_VOID );

    /* create 1 classifier and 3 diagramelements */
    const data_row_t omni_classifier = tvec_setup_classifier( &test_environ, "omni classifier" );
    tvec_setup_diagramelement( &test_environ, test_diagram, omni_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, seq_diagram, omni_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, class_diagram, omni_classifier, DATA_ROW_VOID );

    /* create 1 feature */
    const data_row_t omni_feature = tvec_setup_feature( &test_environ, omni_classifier, "omni feature" );

    /* create 2 relationship */
    const data_row_t semi_rel = tvec_setup_relationship( &test_environ,
                                                         semi_classifier, DATA_ROW_VOID,
                                                         omni_classifier, DATA_ROW_VOID,
                                                         "semi to omni relation"
                                                       );
    const data_row_t omni_rel = tvec_setup_relationship( &test_environ,
                                                         omni_classifier, omni_feature,
                                                         omni_classifier, DATA_ROW_VOID,
                                                         "omni to omni relation"
                                                       );

    tvec_setup_destroy( &test_environ );

    /* test diag / DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM (< change to DATA_DIAGRAM_TYPE_BOX_DIAGRAM ) */
    /*     semi classifier  ----------, */
    /*     omni classifier  <----, <--' */
    /*         omni feature  ----' */
    /* seq diag / DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    /*     semi classifier  ----------, */
    /*     omni classifier  <----, <--' */
    /*         omni feature  ----' */
    /* class diag / DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    /*     omni classifier  <----, */
    /*         omni feature  ----' */

    /* change the type of the test diagram */
    data_stat_t statistics;
    data_stat_init( &statistics );
    const u8_error_t c_err
    = ctrl_diagram_controller_update_diagram_type( diagram_ctrl,
                                                   test_diagram,
                                                   DATA_DIAGRAM_TYPE_BOX_DIAGRAM,
                                                   &statistics
    );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, c_err );

    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_series_count( &statistics, DATA_STAT_SERIES_MODIFIED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_series_count( &statistics, DATA_STAT_SERIES_DELETED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_table_count( &statistics, DATA_STAT_TABLE_DIAGRAM ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_table_count( &statistics, DATA_STAT_TABLE_RELATIONSHIP ) );
    TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count( &statistics ) );
    data_stat_destroy( &statistics );

    /* is semi_relationship deleted? */
    data_relationship_t probe;
    const u8_error_t rel_err1
        = data_database_reader_get_relationship_by_id( &((*fix).db_reader), semi_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, rel_err1 );

    /* is omni_relationship deleted? */
    const u8_error_t rel_err2
        = data_database_reader_get_relationship_by_id( &((*fix).db_reader), omni_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, rel_err2 );

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
    tvec_setup_diagramelement( &test_environ, box_diagram, test_classifier, DATA_ROW_VOID );
    const data_row_t test_seq_diagele
        = tvec_setup_diagramelement( &test_environ, seq_diagram, test_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, class_diagram, test_classifier, DATA_ROW_VOID );

    /* create 1 feature */
    const data_row_t test_feature = tvec_setup_feature( &test_environ, test_classifier, "test feature" );

    /* create 1 classifier and 3 diagramelements */
    const data_row_t omni_classifier = tvec_setup_classifier( &test_environ, "omni classifier" );
    tvec_setup_diagramelement( &test_environ, box_diagram, omni_classifier, DATA_ROW_VOID );
    tvec_setup_diagramelement( &test_environ, seq_diagram, omni_classifier, DATA_ROW_VOID );
    const data_row_t omni_class_diagele
        = tvec_setup_diagramelement( &test_environ, class_diagram, omni_classifier, DATA_ROW_VOID );

    /* create 1 relationship */
    const data_row_t a_rel = tvec_setup_relationship( &test_environ,
                                                      test_classifier, test_feature,
                                                      omni_classifier, DATA_ROW_VOID,
                                                      "a relation"
                                                    );

    tvec_setup_destroy( &test_environ );

    /* box diag / DATA_DIAGRAM_TYPE_BOX_DIAGRAM */
    /*     test classifier */
    /*         test feature  ----, */
    /*     omni classifier  <----' */
    /* seq diag / DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    /*     test classifier   (< diagramelement will be deleted) */
    /*         test feature  ----, */
    /*     omni classifier  <----' */
    /* class diag / DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    /*     test classifier */
    /*         test feature  ----, */
    /*     omni classifier  <----' (< diagramelement will be deleted) */

    /* delete the test diagramelement @ seq diag */
    const u8_error_t c_err1
        = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl, test_seq_diagele, CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, c_err1 );

    /* is the rel deleted? */
    data_relationship_t probe;
    const u8_error_t rel_err1
        = data_database_reader_get_relationship_by_id( &((*fix).db_reader), a_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, rel_err1 );

    /* delete the omni diagramelement @ class diag */
    const u8_error_t c_err2
        = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl, omni_class_diagele, CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, c_err2 );

    /* is the rel deleted? */
    const u8_error_t rel_err2
        = data_database_reader_get_relationship_by_id( &((*fix).db_reader), a_rel, &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, rel_err2 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t consistency_check_error( test_fixture_t *fix )
{
    assert( fix != NULL );

    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    tvec_setup_t test_environ;
    tvec_setup_init( &test_environ, &((*fix).controller) );

    /* create 1 diagram */
    const data_row_t box_diagram
        = tvec_setup_diagram( &test_environ, DATA_ROW_VOID, "box diag", DATA_DIAGRAM_TYPE_BOX_DIAGRAM );

    /* create 1 classifier and 1 diagramelement */
    const data_row_t test_classifier = tvec_setup_classifier( &test_environ, "test classifier" );
    const data_row_t test_at_box
        = tvec_setup_diagramelement( &test_environ, box_diagram, test_classifier, DATA_ROW_VOID );

    /* create 1 relationship */
    tvec_setup_relationship( &test_environ,
                             test_classifier, DATA_ROW_VOID,
                             test_classifier, DATA_ROW_VOID,
                             "a relation"
                           );

    tvec_setup_destroy( &test_environ );

    /* box diag / DATA_DIAGRAM_TYPE_BOX_DIAGRAM */
    /*     test classifier <----o */

    /* damage the database structure */
    {
        u8_error_t result = U8_ERROR_NONE;
        const char *sql_cmd = "DROP TABLE relationships;";
        result |= data_database_transaction_begin ( &((*fix).database) );
        result |= data_database_in_transaction_execute( &((*fix).database), sql_cmd );
        result |= data_database_transaction_commit ( &((*fix).database) );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == result );
    }

    /* delete the test diagramelement @ box diag */
    const u8_error_t c_err
        = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl, test_at_box, CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_DB, c_err );

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
