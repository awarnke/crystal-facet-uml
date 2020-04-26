/* File: md_filter_test.c; Copyright and License: see below */

#include "md_filter_test.h"
#include "md/md_filter.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void run_filter(void);

static int64_t create_root_diag();  /* helper function */

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char* DATABASE_FILENAME = "unittest_crystal_facet_uml_io.cfu1";

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

test_suite_t md_filter_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "md_filter_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "run_filter", &run_filter );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int err;
    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT ( 0 == err );
}

static int64_t create_root_diag()
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a diagram */
    int64_t root_diag_id;

    data_diagram_t root_diagram;
    data_err = data_diagram_init ( &root_diagram,
                                   DATA_ID_VOID_ID /*=diagram_id is ignored*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                   "the_root_diag",
                                   "diagram_description-root",
                                   10555 /*=list_order*/
                                 );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    root_diag_id = DATA_ID_VOID_ID;
    ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                        &root_diagram,
                                                        false, /* add_to_latest_undo_set */
                                                        &root_diag_id
                                                      );
    TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
    TEST_ENVIRONMENT_ASSERT( DATA_ID_VOID_ID != root_diag_id );
    data_diagram_destroy ( &root_diagram );

    return root_diag_id;
}


static void run_filter(void)
{
    int64_t root_diag_id = create_root_diag();

    /*
    json_import_to_database_t importer;
    json_import_to_database_init ( &importer, &db_reader, &controller );

    data_error_t data_err;
    io_stat_t total;
    io_stat_t dropped;
    uint32_t read_pos;
    static const char *json_text_p = "{\"data\":[{\"unknown-type\":{}}]}";
    data_err = json_import_to_database_import_buf_to_db( &importer,
                                                         json_text_p,
                                                         root_diag_id,
                                                         &total,
                                                         &dropped,
                                                         &read_pos
                                                       );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_PARSER_STRUCTURE, data_err );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &total ), 0 );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &dropped ), 0 );
    TEST_ASSERT_EQUAL_INT( read_pos, 9 );

    static const char *json_text_l = "{\"data\":[{\"diagram\":nullnul}]}";
    data_err = json_import_to_database_import_buf_to_db( &importer,
                                                         json_text_l,
                                                         root_diag_id,
                                                         &total,
                                                         &dropped,
                                                         &read_pos
                                                       );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_PARSER_STRUCTURE, data_err );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &total ), 0 );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &dropped ), 0 );
    TEST_ASSERT_EQUAL_INT( read_pos, 20 );

    json_import_to_database_destroy ( &importer );
    */
}


/*
 * Copyright 2020-2020 Andreas Warnke
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
