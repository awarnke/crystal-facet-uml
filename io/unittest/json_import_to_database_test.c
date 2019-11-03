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
static void insert_invalid_json(void);
static void insert_invalid_parent_diag(void);
static void insert_empty_set(void);
static void insert_new_classifier_to_existing_diagram(void);
static void insert_new_classifier_to_new_diagram(void);
static void insert_existing_classifier_to_existing_diagram(void);
static void insert_existing_classifier_to_new_diagram(void);
static void insert_unconditional_relationships(void);
static void insert_scenario_relationships_to_scenario(void);
static void insert_scenario_relationships_to_non_scenario(void);

static int64_t create_root_diag();

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
    test_suite_add_test_case( &result, "insert_invalid_json", &insert_invalid_json );
    test_suite_add_test_case( &result, "insert_invalid_parent_diag", &insert_invalid_parent_diag );
    test_suite_add_test_case( &result, "insert_empty_set", &insert_empty_set );
    test_suite_add_test_case( &result, "insert_new_classifier_to_existing_diagram", &insert_new_classifier_to_existing_diagram );
    test_suite_add_test_case( &result, "insert_new_classifier_to_new_diagram", &insert_new_classifier_to_new_diagram );
    test_suite_add_test_case( &result, "insert_existing_classifier_to_existing_diagram", &insert_existing_classifier_to_existing_diagram );
    test_suite_add_test_case( &result, "insert_existing_classifier_to_new_diagram", &insert_existing_classifier_to_new_diagram );
    test_suite_add_test_case( &result, "insert_unconditional_relationships", &insert_unconditional_relationships );
    test_suite_add_test_case( &result, "insert_scenario_relationships_to_scenario", &insert_scenario_relationships_to_scenario );
    test_suite_add_test_case( &result, "insert_scenario_relationships_to_non_scenario", &insert_scenario_relationships_to_non_scenario );
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

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
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

static const char *const test_json_1 =
    "{\n"
    "  \"data\": \n"
    "  [\n"
    "    {\n"
    "      \"diagram\": {\n"
    "        \"id\": 3,\n"
    "        \"diagram_type\": 13,\n"
    "        \"name\": \"Sequence 1\",\n"
    "        \"description\": \"\",\n"
    "        \"list_order\": 65536\n"
    "      }\n"
    "    },\n"
    "    {\n"
    "      \"classifier\": {\n"
    "        \"id\": 15,\n"
    "        \"main_type\": 125,\n"
    "        \"stereotype\": \"\",\n"
    "        \"name\": \"\\\"the Serializer\\\"\",\n"
    "        \"description\": \"hello\nhello\",\n"
    "        \"x_order\": -1087446901,\n"
    "        \"y_order\": 359417528,\n"
    "        \"list_order\": 446877143,\n"
    "        \"features\": \n"
    "        [\n"
    "          {\n"
    "            \"id\": 34,\n"
    "            \"main_type\": 3,\n"
    "            \"key\": \"\",\n"
    "            \"value\": \"\",\n"
    "            \"description\": \"\",\n"
    "            \"list_order\": 0\n"
    "          },\n"
    "          {\n"
    "            \"id\": 12,\n"
    "            \"main_type\": 1,\n"
    "            \"key\": \"new_get_state\",\n"
    "            \"value\": \"enum\",\n"
    "            \"description\": \"\",\n"
    "            \"list_order\": 425984\n"
    "          }\n"
    "        ]\n"
    "      }\n"
    "    },\n"
    "    {\n"
    "      \"relationship\": {\n"
    "        \"id\": 25,\n"
    "        \"main_type\": 221,\n"
    "        \"name\": \"ping\",\n"
    "        \"description\": \"\",\n"
    "        \"list_order\": -244244146,\n"
    "        \"from_classifier_id\": 15,\n"
    "        \"from_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"to_classifier_id\": 13,\n"
    "        \"to_classifier_name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"from_feature_id\": 14,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": 15,\n"
    "        \"to_feature_key\": \"\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static void insert_invalid_json(void)
{

}

static void insert_invalid_parent_diag(void)
{
}

static void insert_empty_set(void)
{
    int64_t root_diag_id = create_root_diag();

    json_import_to_database_t importer;
    json_import_to_database_init ( &importer, &db_reader, &controller );

    data_error_t data_err;
    io_stat_t total;
    io_stat_t dropped;
    uint32_t read_pos;
    static const char *json_text = "{\"data\":[]}";
    data_err = json_import_to_database_import_buf_to_db( &importer,
                                                         json_text,
                                                         root_diag_id,
                                                         &total,
                                                         &dropped,
                                                         &read_pos
                                                       );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &total ), 0 );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &dropped ), 0 );
    TEST_ASSERT_EQUAL_INT( read_pos, strlen(json_text) );

    json_import_to_database_destroy ( &importer );
}

static void insert_new_classifier_to_existing_diagram(void)
{
}

static void insert_new_classifier_to_new_diagram(void)
{
    int64_t root_diag_id = create_root_diag();

    json_import_to_database_t importer;
    json_import_to_database_init ( &importer, &db_reader, &controller );

    data_error_t data_err;
    io_stat_t total;
    io_stat_t dropped;
    uint32_t read_pos;
    data_err = json_import_to_database_import_buf_to_db( &importer,
                                                         test_json_1,
                                                         root_diag_id,
                                                         &total,
                                                         &dropped,
                                                         &read_pos
                                                       );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &total ), 5+1 );  /* +1 for a diagramelement */
    TEST_ASSERT_EQUAL_INT( io_stat_get_sum( &dropped ), 2 );  /* 2 for a lifeline (type 3) and for the relationship */
    TEST_ASSERT_EQUAL_INT( read_pos, strlen(test_json_1) );

    json_import_to_database_destroy ( &importer );
}

static void insert_existing_classifier_to_existing_diagram(void)
{
}

static void insert_existing_classifier_to_new_diagram(void)
{
}

static void insert_unconditional_relationships(void)
{
}

static void insert_scenario_relationships_to_scenario(void)
{
}

static void insert_scenario_relationships_to_non_scenario(void)
{
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
