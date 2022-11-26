/* File: io_import_elements_test.c; Copyright and License: see below */

#include "io_import_elements_test.h"
#include "io_import_elements.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "u8stream/universal_null_output_stream.h"
#include "u8/u8_trace.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_reject_duplicates(void);

/*!
 *  \brief helper function cerates a root diagram and sets the element importer to paste mode
 */
static data_row_id_t set_mode_paste_to_root_diag();

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

/*!
 *  \brief statistics
 */
static data_stat_t stats;

/*!
 *  \brief io_import_elements to be tested
 */
static io_import_elements_t elements_importer;

/*!
 *  \brief ignore report of io_import_elements_t
 */
static universal_null_output_stream_t null_out;

/*!
 *  \brief ignore report of io_import_elements_t
 */
static universal_utf8_writer_t out_writer;

test_suite_t io_import_elements_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "io_import_elements_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_reject_duplicates", &test_reject_duplicates );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );

    data_stat_init( &stats );

    universal_null_output_stream_init( &null_out );
    universal_utf8_writer_init( &out_writer, universal_null_output_stream_get_output_stream( &null_out ) );
    io_import_elements_init( &elements_importer, &db_reader, &controller, &stats, &out_writer );
}

static void tear_down(void)
{
    io_import_elements_destroy( &elements_importer );
    universal_utf8_writer_destroy( &out_writer );
    universal_null_output_stream_destroy( &null_out );

    data_stat_destroy( &stats );

    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static data_row_id_t set_mode_paste_to_root_diag()
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a diagram */
    data_row_id_t root_diag_id;

    data_diagram_t root_diagram;
    data_err = data_diagram_init( &root_diagram,
                                  DATA_ROW_ID_VOID,  /* diagram_id */
                                  DATA_ROW_ID_VOID,  /* parent_diagram_id */
                                  DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                  "Th& <root> d\"agram",
                                  "diagram_description-root",
                                  10555 ,  /* list_order */
                                  DATA_DIAGRAM_FLAG_NONE,
                                  "93055a07-ed69-40c8-ba80-469e94242de3"
                                );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    root_diag_id = DATA_ROW_ID_VOID;
    ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                        &root_diagram,
                                                        CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                        &root_diag_id
                                                      );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == ctrl_err );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );
    data_diagram_destroy ( &root_diagram );

    io_import_elements_destroy( &elements_importer );
    io_import_elements_init_for_paste( &elements_importer, root_diag_id, &db_reader, &controller, &stats, &out_writer );

    return root_diag_id;
}

static void test_reject_duplicates(void)
{
    data_row_id_t root_diag_id = set_mode_paste_to_root_diag();
    TEST_ENVIRONMENT_ASSERT( 1 == root_diag_id );
    u8_error_t err;

    data_classifier_t my_classifier;
    const u8_error_t data_err_1
        = data_classifier_init( &my_classifier,
                                36,  /* id */
                                DATA_CLASSIFIER_TYPE_COMMENT,
                                "my_stereotype1, my_stereotype2",
                                "my name",
                                "description\ndescription",
                                -800,  /* x_order */
                                -600,  /* y_order */
                                100,  /* list_order */
                                "2dac33c1-0fdb-4a5f-80b2-3789f935a700"
                              );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_1 );

    err = io_import_elements_sync_classifier( &elements_importer, &my_classifier );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_table_count( &stats, DATA_TABLE_CLASSIFIER ) );

    /* duplicate id, name and uuid */
    err = io_import_elements_sync_classifier( &elements_importer, &my_classifier );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_IGNORED ) );
    TEST_ASSERT_EQUAL_INT( 2, data_stat_get_table_count( &stats, DATA_TABLE_CLASSIFIER ) );

    /* duplicate name */
    data_classifier_t cloned_classifier;
    const u8_error_t data_err_2
        = data_classifier_init( &cloned_classifier,
                                55,  /* id */
                                DATA_CLASSIFIER_TYPE_COMMENT,
                                "my_stereotype1, my_stereotype2",
                                "my name",
                                "description\ndescription",
                                -800,  /* x_order */
                                -600,  /* y_order */
                                100,  /* list_order */
                                "87950246-9fe2-4acf-ba83-1eb9b813b6d8"
                              );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_2 );

    err = io_import_elements_sync_classifier( &elements_importer, &cloned_classifier );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING ) );
    TEST_ASSERT_EQUAL_INT( 2, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 4, data_stat_get_table_count( &stats, DATA_TABLE_CLASSIFIER ) );

    /* duplicate id and name */
    const u8_error_t d_err_3
        = data_classifier_set_name( &cloned_classifier, "my name" );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == d_err_3 );
    const u8_error_t d_err_0
        = data_classifier_set_uuid( &cloned_classifier, "85855097-07ee-42ae-ac5c-1376c7f21730" );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == d_err_0 );

    err = io_import_elements_sync_classifier( &elements_importer, &cloned_classifier );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_ASSERT_EQUAL_INT( 2, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING ) );
    TEST_ASSERT_EQUAL_INT( 3, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 6, data_stat_get_table_count( &stats, DATA_TABLE_CLASSIFIER ) );

    /* duplicate id and uuid */
    const u8_error_t d_err_1
        = data_classifier_set_name( &cloned_classifier, "other name" );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == d_err_1 );
    const u8_error_t d_err_2
        = data_classifier_set_uuid( &cloned_classifier, "2dac33c1-0fdb-4a5f-80b2-3789f935a700" );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == d_err_2 );

    err = io_import_elements_sync_classifier( &elements_importer, &cloned_classifier );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_ASSERT_EQUAL_INT( 2, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_IGNORED ) );
    TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stats, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_MODIFIED ) );
    TEST_ASSERT_EQUAL_INT( 7, data_stat_get_table_count( &stats, DATA_TABLE_CLASSIFIER ) );
}


/*
 * Copyright 2021-2022 Andreas Warnke
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
