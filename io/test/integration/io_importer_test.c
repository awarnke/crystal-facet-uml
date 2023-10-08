/* File: io_importer_test.c; Copyright and License: see below */

#include "io_importer_test.h"
#include "io_importer.h"
#include "set/data_stat.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "utf8stream/utf8stream_writer.h"
#include "u8stream/universal_memory_input_stream.h"
#include "u8stream/universal_memory_output_stream.h"
#include "u8/u8_trace.h"
#include "test_expect.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t insert_invalid_json( test_fixture_t *test_env );
static test_case_result_t insert_invalid_parent_diag( test_fixture_t *test_env );
static test_case_result_t insert_empty_set( test_fixture_t *test_env );
static test_case_result_t insert_new_classifier_to_existing_diagram( test_fixture_t *test_env );
static test_case_result_t insert_new_classifier_to_new_diagram( test_fixture_t *test_env );
static test_case_result_t insert_existing_feature_to_other_classifier( test_fixture_t *test_env );
static test_case_result_t insert_existing_classifier_to_new_diagram( test_fixture_t *test_env );
static test_case_result_t insert_unconditional_relationships( test_fixture_t *test_env );
static test_case_result_t insert_relationships_to_non_scenario( test_fixture_t *test_env );
static test_case_result_t insert_scenario_relationships_to_scenario( test_fixture_t *test_env );

static data_row_id_t create_root_diag();  /* helper function */

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

test_suite_t io_importer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "io_importer_test_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "insert_invalid_json", &insert_invalid_json );
    test_suite_add_test_case( &result, "insert_invalid_parent_diag", &insert_invalid_parent_diag );
    test_suite_add_test_case( &result, "insert_empty_set", &insert_empty_set );
    test_suite_add_test_case( &result, "insert_new_classifier_to_existing_diagram", &insert_new_classifier_to_existing_diagram );
    test_suite_add_test_case( &result, "insert_new_classifier_to_new_diagram", &insert_new_classifier_to_new_diagram );
    test_suite_add_test_case( &result, "insert_existing_feature_to_other_classifier", &insert_existing_feature_to_other_classifier );
    test_suite_add_test_case( &result, "insert_existing_classifier_to_new_diagram", &insert_existing_classifier_to_new_diagram );
    test_suite_add_test_case( &result, "insert_unconditional_relationships", &insert_unconditional_relationships );
    test_suite_add_test_case( &result, "insert_relationships_to_non_scenario", &insert_relationships_to_non_scenario );
    test_suite_add_test_case( &result, "insert_scenario_relationships_to_scenario", &insert_scenario_relationships_to_scenario );
    return result;
}

static test_fixture_t * set_up()
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static data_row_id_t create_root_diag()
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    data_row_id_t root_diag_id;

    data_diagram_t root_diagram;
    data_err = data_diagram_init( &root_diagram,
                                  DATA_ROW_ID_VOID, /*=diagram_id is ignored*/
                                  DATA_ROW_ID_VOID, /*=parent_diagram_id*/
                                  DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                  "stereo_t",  /* stereotype */
                                  "the_root_diag",
                                  "diagram_description-root",
                                  10555, /*=list_order*/
                                  DATA_DIAGRAM_FLAG_NONE,
                                  "8a086be4-e05d-4299-a56a-10c5b9037835"
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

    return root_diag_id;
    return TEST_CASE_RESULT_OK;
}

static const char *const test_json_own_diagram =
    "{\n"
    "  \"head\":\n"
    "  {\n"
    "  },\n"
    "  \"views\": \n"
    "  [\n"
    "    {\n"
    "      \"diagram\": {\n"
    "        \"id\": 3,\n"
    "        \"parent_name\": \"none\",\n"
    "        \"diagram_type\": 13,\n"
    "        \"type\": \"DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM\",\n"
    "        \"name\": \"Sequence 1\",\n"
    "        \"description\": [ \"\" ],\n"
    "        \"list_order\": 65536,\n"
    "        \"display_flags\": 0,\n"
    "        \"display_tags\": \"\",\n"
    "        \"uuid\": \"3318bd97-bfe4-4788-9ae8-0c57640fadae\",\n"
    "        \"diagramelements\": \n"
    "        [\n"
    "          {\n"
    "            \"id\": 89,\n"
    "            \"classifier_id\": 15,\n"
    "            \"classifier_name\": \"\",\n"
    "            \"focused_feature_id\": -1,\n"
    "            \"focused_feature_name\": \"\",\n"
    "            \"display_flags\": 0,\n"
    "            \"display_tags\": \"\",\n"
    "            \"node\": \"0967dfbf-7df0-458e-addc-85ef7be06067\",\n"
    "            \"uuid\": \"7fa23aed-0e92-4f00-9fb4-9b97930d58f2\"\n"
    "          }\n"
    "        ]\n"
    "      }\n"
    "    }\n"
    "  ],\n"
    "  \"nodes\": \n"
    "  [\n"
    "    {\n"
    "      \"classifier\": {\n"
    "        \"id\": 15,\n"
    "        \"main_type\": 125,\n"
    "        \"type\": \"DATA_CLASSIFIER_TYPE_CLASS\",\n"
    "        \"stereotype\": \"\",\n"
    "        \"name\": \"\\\"the Serializer\\\"\",\n"
    "        \"description\": [ \"hello\\nhello\" ],\n"
    "        \"x_order\": -1087446901,\n"
    "        \"y_order\": 359417528,\n"
    "        \"list_order\": 446877143,\n"
    "        \"uuid\": \"0967dfbf-7df0-458e-addc-85ef7be06067\",\n"
    "        \"features\": \n"
    "        [\n"
    "          {\n"
    "            \"id\": 34,\n"
    "            \"main_type\": 3,\n"
    "            \"type\": \"DATA_FEATURE_TYPE_LIFELINE\",\n"
    "            \"key\": \"\",\n"
    "            \"value\": \"\",\n"
    "            \"description\": [ ],\n"
    "            \"list_order\": 0,\n"
    "            \"uuid\": \"67c2bc6d-ddc7-458a-80f1-be353c197381\"\n"
    "          },\n"
    "          {\n"
    "            \"id\": 12,\n"
    "            \"main_type\": 1,\n"
    "            \"key\": \"new_get_state\",\n"
    "            \"value\": \"enum\",\n"
    "            \"description\": [ \"\" ],\n"
    "            \"list_order\": 425984\n"
    "          }\n"
    "        ]\n"
    "      }\n"
    "    }\n"
    "  ],\n"
    "  \"edges\": \n"
    "  [\n"
    "    {\n"
    "      \"relationship\": {\n"
    "        \"id\": 25,\n"
    "        \"main_type\": 221,\n"
    "        \"type\": \"DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL\",\n"
    "        \"name\": \"ping\",\n"
    "        \"description\": [ \"\", \"\" ],\n"
    "        \"list_order\": -244244146,\n"
    "        \"from_classifier_id\": 15,\n"
    "        \"from_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"to_classifier_id\": 15,\n"
    "        \"to_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"from_feature_id\": -1,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": -1,\n"
    "        \"to_feature_key\": \"\",\n"
    "        \"from_node\": \"0967dfbf-7df0-458e-addc-85ef7be06067\",\n"
    "        \"to_node\": \"0967dfbf-7df0-458e-addc-85ef7be06067\",\n"
    "        \"uuid\": \"429fc16a-50d3-4541-b5a1-077381b55f02\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static const char *const test_json_no_diag =
    "{\n"
    "  \"head\":\n"
    "  {\n"
    "  },\n"
    "  \"views\": \n"
    "  [\n"
    "  ],\n"
    "  \"nodes\": \n"
    "  [\n"
    "    {\n"
    "      \"classifier\": {\n"
    "        \"id\": 13,\n"
    "        \"main_type\": 125,\n"
    "        \"stereotype\": \"\",\n"
    "        \"name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"description\": [ \"world\\n\", \"world\" ],\n"
    "        \"x_order\": -1087446901,\n"
    "        \"y_order\": 359417528,\n"
    "        \"list_order\": 446877143,\n"
    "        \"features\": \n"
    "        [\n"
    "          {\n"
    "            \"id\": 33,\n"
    "            \"main_type\": 3,\n"
    "            \"key\": \"\",\n"
    "            \"value\": \"\",\n"
    "            \"description\": [ \"i am a lifeline\" ],\n"
    "            \"list_order\": 0\n"
    "          },\n"
    "          {\n"
    "            \"id\": 11,\n"
    "            \"main_type\": 1,\n"
    "            \"key\": \"new_get_mode\",\n"
    "            \"value\": \"enum\",\n"
    "            \"description\": [ ],\n"
    "            \"list_order\": 425984,\n"
    "            \"uuid\": \"67c2bc6d-ddc7-458a-80f1-be353c197381\"\n"
    "          }\n"
    "        ]\n"
    "      }\n"
    "    }\n"
    "  ],\n"
    "  \"edges\": \n"
    "  [\n"
    "    {\n"
    "      \"relationship\": {\n"
    "        \"id\": 25,\n"
    "        \"main_type\": 221,\n"
    "        \"name\": \"ping\",\n"
    "        \"description\": [ \"LINE-1\\nLINE-2\" ],\n"
    "        \"list_order\": -244244146,\n"
    "        \"from_classifier_id\": 15,\n"
    "        \"from_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"to_classifier_id\": 13,\n"
    "        \"to_classifier_name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"from_feature_id\": 11,\n"
    "        \"from_feature_key\": \"new_get_mode\",\n"
    "        \"to_feature_id\": 11,\n"
    "        \"to_feature_key\": \"new_get_mode\",\n"
    "        \"from_node\": \"0967dfbf-7df0-458e-addc-85ef7be06067\",\n"
    "        \"to_node\": \"67c2bc6d-ddc7-458a-80f1-be353c197381\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static test_case_result_t insert_invalid_json( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    u8_error_info_t read_line;
    static const char *json_text_p = "{\"head\":{},\"views\":[{\n\"unknown-type\"\n:{}}]}";
    data_err = io_importer_import_clipboard( &importer,
                                             json_text_p,
                                             root_diag_id,
                                             &stat,
                                             &read_line
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, data_err );
    TEST_EXPECT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, u8_error_info_get_error( &read_line ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_line ) );
    TEST_EXPECT_EQUAL_INT( 2, u8_error_info_get_line( &read_line ) );

    /* error happens at char 24 according to the log */
    /* but this happens in json_element_reader_get_type_of_next_element which does not advance the read pos */

    static const char *json_text_l = "{\"head\":{},\"views\":[{\"diagram\":\nnullnul\n}]}";
    data_err = io_importer_import_clipboard( &importer,
                                             json_text_l,
                                             root_diag_id,
                                             &stat,
                                             &read_line
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, data_err );
    TEST_EXPECT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, u8_error_info_get_error( &read_line ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_line ) );
    TEST_EXPECT_EQUAL_INT( 2, u8_error_info_get_line( &read_line ) );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_invalid_parent_diag( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    u8_error_info_t read_line;
    data_err = io_importer_import_clipboard( &importer,
                                             test_json_no_diag,
                                             root_diag_id+1, /* does not exist */
                                             &stat,
                                             &read_line
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_FOCUS_EMPTY, data_err );
    TEST_EXPECT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_FOCUS_EMPTY, u8_error_info_get_error( &read_line ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_line ) );
    TEST_EXPECT_EQUAL_INT( 20, u8_error_info_get_line( &read_line ) );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_empty_set( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    u8_error_info_t read_line;
    static const char *json_text = "{\"head\":{},\"views\":[],\"nodes\":[],\"edges\":[]}\n";
    data_err = io_importer_import_clipboard( &importer,
                                             json_text,
                                             root_diag_id,
                                             &stat,
                                             &read_line
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_line ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_line ) );
    TEST_EXPECT_EQUAL_INT( 2, u8_error_info_get_line( &read_line ) );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_new_classifier_to_existing_diagram( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    u8_error_info_t read_pos;
    data_err = io_importer_import_clipboard( &importer,
                                             test_json_no_diag,
                                             root_diag_id,
                                             &stat,
                                             &read_pos
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, data_err );  /* source of relationship does not exist */
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_DIAGRAM ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
     /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped, because mode is PASTE to clipboard */
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_IGNORED ) );
    TEST_EXPECT_EQUAL_INT( 4, data_stat_get_total_count( &stat ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, u8_error_info_get_error( &read_pos ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
    TEST_EXPECT_EQUAL_INT( 62, u8_error_info_get_line( &read_pos ) );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_new_classifier_to_new_diagram( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    u8_error_info_t read_pos;
    data_err = io_importer_import_clipboard( &importer,
                                             test_json_own_diagram,
                                             root_diag_id,
                                             &stat,
                                             &read_pos
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_IGNORED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
    /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_IGNORED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 7, data_stat_get_total_count( &stat ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
    TEST_EXPECT_EQUAL_INT( 99, u8_error_info_get_line( &read_pos ) );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_existing_feature_to_other_classifier( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, data_err );  /* source of relationship does not exist */
        TEST_EXPECT_EQUAL_INT( 4, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_existing_diagram */
        TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 62, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, data_err );  /* source of relationship does not exist */
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_DIAGRAM ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
        /* duplicate name for classifier */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING ) );
        /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_IGNORED ) );
        /* DATA_TABLE_FEATURE an already existing feature of an other classifier is dropped */
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_IGNORED ) );
        /* DATA_TABLE_RELATIONSHIP: no names of auto-generated lifelines are mentioned, therefore only unconditional relationships */
        TEST_EXPECT_EQUAL_INT( 5, data_stat_get_total_count( &stat ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 62, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_existing_classifier_to_new_diagram( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_own_diagram,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 7, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_new_diagram */
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 99, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_own_diagram,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_IGNORED ) );
        /* a diagramelement is created when pasting an existing classifier */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_IGNORED ) );
        /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_IGNORED ) );
        /* DATA_TABLE_FEATURE a new feature to an already existing classifier is created */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_RELATIONSHIP: duplicate */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_IGNORED ) );
        /*data_stat_trace(&stat);*/
        TEST_EXPECT_EQUAL_INT( 7, data_stat_get_total_count( &stat ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 99, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t insert_unconditional_relationships( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_own_diagram,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 7, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_new_diagram */
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 99, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_DIAGRAM ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_IGNORED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 5, data_stat_get_total_count( &stat ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 66, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static const char *const test_non_scenario_relationship =
    "{\n"
    "  \"head\":\n"
    "  {\n"
    "  },\n"
    "  \"views\": \n"
    "  [\n"
    "    {\n"
    "      \"diagram\": {\n"
    "        \"id\": 3,\n"
    "        \"diagram_type\": 34,\n"
    "        \"name\": \"Sequence 1\",\n"
    "        \"description\": [ \"LINE-1\\n\", \"LINE-2\" ],\n"
    "        \"list_order\": 65536\n"
    "      }\n"
    "    }\n"
    "  ],\n"
    "  \"nodes\": \n"
    "  [\n"
    "    {\n"
    "      \"classifier\": {\n"
    "        \"id\": 15,\n"
    "        \"main_type\": 125,\n"
    "        \"stereotype\": \"\",\n"
    "        \"name\": \"\\\"the Serializer\\\"\",\n"
    "        \"description\": [ \"hello\\nhello\" ],\n"
    "        \"x_order\": -1087446901,\n"
    "        \"y_order\": 359417528,\n"
    "        \"list_order\": 446877143,\n"
    "        \"uuid\": \"4967b532-f884-445e-bb95-28540caa8d83\",\n"
    "        \"features\": \n"
    "        [\n"
    "        ]\n"
    "      }\n"
    "    },\n"
    "    {\n"
    "      \"classifier\": {\n"
    "        \"id\": 13,\n"
    "        \"main_type\": 125,\n"
    "        \"stereotype\": \"\",\n"
    "        \"name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"description\": [ \"world\\nworld\" ],\n"
    "        \"x_order\": -1087446901,\n"
    "        \"y_order\": 359417528,\n"
    "        \"list_order\": 446877143,\n"
    "        \"uuid\": \"64458a4c-4d4b-4f17-a929-c002da1596c1\",\n"
    "        \"features\": \n"
    "        [\n"
    "        ]\n"
    "      }\n"
    "    }\n"
    "  ],\n"
    "  \"edges\": \n"
    "  [\n"
    "    {\n"
    "      \"relationship\": {\n"
    "        \"id\": 25,\n"
    "        \"main_type\": 221,\n"
    "        \"name\": \"ping\",\n"
    "        \"description\": [ ],\n"
    "        \"list_order\": -244244146,\n"
    "        \"from_classifier_id\": 15,\n"
    "        \"from_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"to_classifier_id\": 13,\n"
    "        \"to_classifier_name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"from_feature_id\": -1,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": -1,\n"
    "        \"to_feature_key\": \"\",\n"
    "        \"from_node\": \"4967b532-f884-445e-bb95-28540caa8d83\",\n"
    "        \"to_node\": \"64458a4c-4d4b-4f17-a929-c002da1596c1\",\n"
    "        \"uuid\": \"5f3f8899-713d-4e16-971b-d570480daffb\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static test_case_result_t insert_relationships_to_non_scenario( test_fixture_t *test_env )
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    u8_error_info_t read_pos;
    data_err = io_importer_import_clipboard( &importer,
                                             test_non_scenario_relationship,
                                             root_diag_id,
                                             &stat,
                                             &read_pos
                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    /* type 13 == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM; is scenario */
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_FEATURE ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 6, data_stat_get_total_count( &stat ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
    TEST_EXPECT_EQUAL_INT( 76, u8_error_info_get_line( &read_pos ) );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}

static const char *const test_json_scenario_self_relation =
    "{\n"
    "  \"head\":\n"
    "  {\n"
    "  },\n"
    "  \"views\": \n"
    "  [\n"
    "  ],\n"
    "  \"nodes\": \n"
    "  [\n"
    "  ],\n"
    "  \"edges\": \n"
    "  [\n"
    "    {\n"
    "      \"relationship\": {\n"
    "        \"id\": 25,\n"
    "        \"main_type\": 221,\n"
    "        \"name\": \"ping\",\n"
    "        \"description\": [ \"LINE-1\\n\", \"LINE-2\\n\" ],\n"
    "        \"list_order\": 44244146,\n"
    "        \"from_classifier_id\": 15,\n"
    "        \"from_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"to_classifier_id\": 15,\n"
    "        \"to_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"from_feature_id\": 34,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": 34,\n"
    "        \"to_feature_key\": \"\",\n"
    "        \"from_node\": \"67c2bc6d-ddc7-458a-80f1-be353c197381\",\n"
    "        \"to_node\": \"67c2bc6d-ddc7-458a-80f1-be353c197381\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static test_case_result_t insert_scenario_relationships_to_scenario( test_fixture_t *test_env )
{
    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);

        char report_buffer[32];
        universal_memory_output_stream_t report_stream;
        universal_memory_output_stream_init( &report_stream, &report_buffer, sizeof(report_buffer) );
        utf8stream_writer_t report;
        utf8stream_writer_init( &report, universal_memory_output_stream_get_output_stream( &report_stream ) );

        universal_memory_input_stream_t mem_json;
        universal_memory_input_stream_init( &mem_json, test_json_own_diagram, utf8string_get_length( test_json_own_diagram ) );

        u8_error_info_t read_pos;
        data_err = io_importer_import_stream( &importer,
                                              IO_IMPORT_MODE_CREATE | IO_IMPORT_MODE_LINK,
                                              universal_memory_input_stream_get_input_stream( &mem_json ),
                                              &stat,
                                              &read_pos,
                                              &report
                                            );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 6, data_stat_get_total_count( &stat ) );  /* 1 less than in insert_new_classifier_to_new_diagram */
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 99, u8_error_info_get_line( &read_pos ) );

        universal_memory_input_stream_destroy( &mem_json );
        utf8stream_writer_destroy( &report );
        universal_memory_output_stream_destroy( &report_stream );
        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        u8_error_info_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_scenario_self_relation,
                                                 (data_row_id_t)3,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        /* type 13 == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM; is scenario */
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_DIAGRAM ) );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT ) );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_CLASSIFIER ) );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_STAT_TABLE_FEATURE ) );
        /* DATA_TABLE_RELATIONSHIP: source+dst classifier are a lifeline */
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count( &stat ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, u8_error_info_get_unit( &read_pos ) );
        TEST_EXPECT_EQUAL_INT( 34, u8_error_info_get_line( &read_pos ) );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2019-2023 Andreas Warnke
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
