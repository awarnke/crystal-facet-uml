/* File: json_import_to_database_test.c; Copyright and License: see below */

#include "json_import_to_database_test.h"
#include "json/json_import_to_database.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void insert_new_classifier_to_existing_diagram(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char* DATABASE_FILENAME = "unittest_crystal_facet_uml_json.cfu1";

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

test_suite_t json_import_to_database_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "json_import_to_database_test_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "insert_new_classifier_to_existing_diagram", &insert_new_classifier_to_existing_diagram );
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

static void insert_new_classifier_to_existing_diagram(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );

    /* database is empty */

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    int64_t root_diag_id;
    {
        data_diagram_t root_diagram;
        data_err = data_diagram_init ( &root_diagram,
                                       DATA_ID_VOID_ID /*=diagram_id is ignored*/,
                                       DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                       "the_root_diag",
                                       "diagram_description-root",
                                       10555 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        root_diag_id = DATA_ID_VOID_ID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &root_diagram,
                                                            false, /* add_to_latest_undo_set */
                                                            &root_diag_id
                                                          );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ID_VOID_ID != root_diag_id );
        data_diagram_destroy ( &root_diagram );
    }

    json_import_to_database_t importer;
    json_import_to_database_init ( &importer, &db_reader, &controller );

    data_error_t d_err;
    uint32_t read_pos;
    static const char *json_text = "{\"set\":[]}";
    d_err = json_import_to_database_import_buf_to_db( &importer,
                                                      json_text,
                                                      root_diag_id,
                                                      &read_pos
                                                    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    json_import_to_database_destroy ( &importer );



}


/*
 * Copyright 2019-2019 Andreas Warnke
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
