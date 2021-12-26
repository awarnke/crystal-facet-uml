/* File: io_importer_test.c; Copyright and License: see below */

#include "io_importer_test.h"
#include "io_importer.h"
#include "set/data_stat.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"

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
    "        \"diagram_type\": 13,\n"
    "        \"name\": \"Sequence 1\",\n"
    "        \"description\": [ \"\" ],\n"
    "        \"list_order\": 65536,\n"
    "        \"display_flags\": 0,\n"
    "        \"uuid\": \"3318bd97-bfe4-4788-9ae8-0c57640fadae\",\n"
    "        \"diagramelements\": \n"
    "        [\n"
    "          {\n"
    "            \"id\": 89,\n"
    "            \"classifier_id\": 15,\n"
    "            \"focused_feature_id\": -1,\n"
    "            \"display_flags\": 0,\n"
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
    "        \"name\": \"ping\",\n"
    "        \"description\": [ \"\", \"\" ],\n"
    "        \"list_order\": -244244146,\n"
    "        \"from_classifier_id\": 15,\n"
    "        \"from_classifier_name\": \"\\\"the Serializer\\\"\",\n"
    "        \"to_classifier_id\": 13,\n"
    "        \"to_classifier_name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"from_feature_id\": 34,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": 33,\n"
    "        \"to_feature_key\": \"\",\n"
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
    "        \"from_feature_id\": 12,\n"
    "        \"from_feature_key\": \"new_get_state\",\n"
    "        \"to_feature_id\": 11,\n"
    "        \"to_feature_key\": \"new_get_mode\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static void insert_invalid_json(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_line;
    static const char *json_text_p = "{\"head\":{},\"views\":[{\n\"unknown-type\"\n:{}}]}";
    data_err = io_importer_import_clipboard( &importer,
                                             json_text_p,
                                             root_diag_id,
                                             &stat,
                                             &read_line
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, data_err );
    TEST_ASSERT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_ASSERT_EQUAL_INT( read_line, 2 );
    /* error happens at char 24 according to the log */
    /* but this happens in json_element_reader_get_type_of_next_element which does not advance the read pos */

    static const char *json_text_l = "{\"head\":{},\"views\":[{\"diagram\":\nnullnul\n}]}";
    data_err = io_importer_import_clipboard( &importer,
                                             json_text_l,
                                             root_diag_id,
                                             &stat,
                                             &read_line
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, data_err );
    TEST_ASSERT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_ASSERT_EQUAL_INT( read_line, 2 );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
}

static void insert_invalid_parent_diag(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_line;
    data_err = io_importer_import_clipboard( &importer,
                                             test_json_no_diag,
                                             root_diag_id+1, /* does not exist */
                                             &stat,
                                             &read_line
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_FOCUS_EMPTY, data_err );
    TEST_ASSERT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_ASSERT_EQUAL_INT( read_line, 20 );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
}

static void insert_empty_set(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_line;
    static const char *json_text = "{\"head\":{},\"views\":[],\"nodes\":[],\"edges\":[]}\n";
    data_err = io_importer_import_clipboard( &importer,
                                             json_text,
                                             root_diag_id,
                                             &stat,
                                             &read_line
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( data_stat_get_total_count( &stat ), 0 );
    TEST_ASSERT_EQUAL_INT( read_line, 2 );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
}

static void insert_new_classifier_to_existing_diagram(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_pos;
    data_err = io_importer_import_clipboard( &importer,
                                             test_json_no_diag,
                                             root_diag_id,
                                             &stat,
                                             &read_pos
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_DIAGRAM ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
     /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_IGNORED ) );
    /* DATA_TABLE_RELATIONSHIP: no names of auto-generated lifelines are mentioned, therefore only unconditional relationships */
    TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
    /* DATA_TABLE_RELATIONSHIP: source does not exist */
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ) );
    TEST_ASSERT_EQUAL_INT( 5, data_stat_get_total_count( &stat ) );
    TEST_ASSERT_EQUAL_INT( 64, read_pos );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
}

static void insert_new_classifier_to_new_diagram(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_pos;
    data_err = io_importer_import_clipboard( &importer,
                                             test_json_own_diagram,
                                             root_diag_id,
                                             &stat,
                                             &read_pos
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_IGNORED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
    /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_IGNORED ) );
    /* DATA_TABLE_RELATIONSHIP: no names of auto-generated lifelines are mentioned, therefore only unconditional relationships */
    TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
    /* DATA_TABLE_RELATIONSHIP: destination does not exist */
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ) );
    TEST_ASSERT_EQUAL_INT( 7, data_stat_get_total_count( &stat ) );
    TEST_ASSERT_EQUAL_INT( 87, read_pos );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
}

static void insert_existing_classifier_to_existing_diagram(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 5, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_existing_diagram */
        TEST_ASSERT_EQUAL_INT( 64, read_pos );
        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_DIAGRAM ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
        /* DATA_TABLE_FEATURE a feature of an already existing classifier is dropped */
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_IGNORED ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_WARNING ) );
        /* DATA_TABLE_RELATIONSHIP: no names of auto-generated lifelines are mentioned, therefore only unconditional relationships */
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_RELATIONSHIP: source does not exist */
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ) );
        TEST_ASSERT_EQUAL_INT( 6, data_stat_get_total_count( &stat ) );
        TEST_ASSERT_EQUAL_INT( 64, read_pos );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
}

static void insert_existing_classifier_to_new_diagram(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_own_diagram,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 7, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_new_diagram */
        TEST_ASSERT_EQUAL_INT( 87, read_pos );
        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_own_diagram,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_IGNORED ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_IGNORED ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
        /* DATA_TABLE_FEATURE a feature of an already existing classifier is dropped */
        TEST_ASSERT_EQUAL_INT( 2, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_IGNORED ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_WARNING ) );
        /* DATA_TABLE_RELATIONSHIP: no names of auto-generated lifelines are mentioned, therefore only unconditional relationships */
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_RELATIONSHIP: destination does not exist */
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ) );
        TEST_ASSERT_EQUAL_INT( 6, data_stat_get_total_count( &stat ) );
        TEST_ASSERT_EQUAL_INT( 87, read_pos );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
}

static void insert_unconditional_relationships(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_own_diagram,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 6, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_new_diagram */
        TEST_ASSERT_EQUAL_INT( 87, read_pos );
        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_DIAGRAM ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ) );
        /* DATA_TABLE_FEATURE: lifeline (type 3) is dropped */
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_IGNORED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 5, data_stat_get_total_count( &stat ) );
        TEST_ASSERT_EQUAL_INT( 64, read_pos );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
}

static const char *const test_scenario_relationship =
    "{\n"
    "  \"head\":\n"
    "  {\n"
    "  },\n"
    "  \"views\": \n"
    "  [\n"
    "    {\n"
    "      \"diagram\": {\n"
    "        \"id\": 3,\n"
    "        \"diagram_type\": 13,\n"
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
    "        \"from_feature_id\": 34,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": 33,\n"
    "        \"to_feature_key\": \"\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static void insert_scenario_relationships_to_scenario(void)
{
    data_row_id_t root_diag_id = create_root_diag();

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_pos;
    data_err = io_importer_import_clipboard( &importer,
                                             test_scenario_relationship,
                                             root_diag_id,
                                             &stat,
                                             &read_pos
                                           );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
    /* type 13 == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM; is scenario */
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 2, data_stat_get_count( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 2, data_stat_get_count( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_FEATURE ) );
    TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
    TEST_ASSERT_EQUAL_INT( 6, data_stat_get_total_count( &stat ) );
    TEST_ASSERT_EQUAL_INT( 71, read_pos );

    data_stat_destroy(&stat);
    io_importer_destroy ( &importer );
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
    "        \"from_classifier_id\": 13,\n"
    "        \"from_classifier_name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"to_classifier_id\": 13,\n"
    "        \"to_classifier_name\": \"New||RingBuffer{⅞[\\]}\",\n"
    "        \"from_feature_id\": 33,\n"
    "        \"from_feature_key\": \"\",\n"
    "        \"to_feature_id\": 33,\n"
    "        \"to_feature_key\": \"\"\n"
    "      }\n"
    "    }\n"
    "  ]\n"
    "}\n";

static void insert_scenario_relationships_to_non_scenario(void)
{
    data_row_id_t root_diag_id = create_root_diag();  /* root doag type is DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM, no scenario */

    io_importer_t importer;
    io_importer_init ( &importer, &db_reader, &controller );

    u8_error_t data_err;
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_no_diag,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 5, data_stat_get_total_count( &stat ) );  /* as in test case insert_new_classifier_to_existing_diagram */
        TEST_ASSERT_EQUAL_INT( 64, read_pos );
        data_stat_destroy(&stat);
    }
    {
        data_stat_t stat;
        data_stat_init(&stat);
        uint32_t read_pos;
        data_err = io_importer_import_clipboard( &importer,
                                                 test_json_scenario_self_relation,
                                                 root_diag_id,
                                                 &stat,
                                                 &read_pos
                                               );
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, data_err );
        /* type 13 == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM; is scenario */
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_DIAGRAM ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_DIAGRAMELEMENT ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_CLASSIFIER ) );
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_table_count( &stat, DATA_TABLE_FEATURE ) );
        /* DATA_TABLE_RELATIONSHIP: source+dst classifier have no lifeline, no evidence for scenario */
        TEST_ASSERT_EQUAL_INT( 0, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ) );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_total_count( &stat ) );
        TEST_ASSERT_EQUAL_INT( 32, read_pos );

        data_stat_destroy(&stat);
    }
    io_importer_destroy ( &importer );
}


/*
 * Copyright 2019-2021 Andreas Warnke
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
