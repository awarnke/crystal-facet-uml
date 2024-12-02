/* File: io_md_writer_test.c; Copyright and License: see below */

#include "io_md_writer_test.h"
#include "format/io_md_writer.h"
#include "format/io_xml_writer.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8stream/universal_memory_output_stream.h"
#include "u8/u8_trace.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_md_plain_mixed( test_fixture_t *fix );
static test_case_result_t test_valid_links( test_fixture_t *fix );
static test_case_result_t test_invalid_links( test_fixture_t *fix );

static data_row_t create_root_diag( ctrl_controller_t *controller );  /* helper function */

test_suite_t io_md_writer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "io_md_writer_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_md_plain_mixed", &test_md_plain_mixed );
    test_suite_add_test_case( &result, "test_valid_links", &test_valid_links );
    test_suite_add_test_case( &result, "test_invalid_links", &test_invalid_links );
    return result;
}

#define TAG_BREAK "&LN;"
#define TAG_LINK1 "<REF TO=\""
#define TAG_LINK2 "\">"
#define TAG_LINK3 "</REF>"

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    ctrl_controller_t controller;  /*!< controller instance on which the tests are performed */
    io_md_writer_t md_writer;  /*!< io_md_writer to be tested */
    char out_buffer[200];
    universal_memory_output_stream_t out_stream;
    io_xml_writer_t xml_writer;
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

    memset( &((*fix).out_buffer), '\0', sizeof( (*fix).out_buffer) );
    universal_memory_output_stream_init( &((*fix).out_stream),
                                         &((*fix).out_buffer),
                                         sizeof( (*fix).out_buffer),
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    io_xml_writer_init( &((*fix).xml_writer), universal_memory_output_stream_get_output_stream( &((*fix).out_stream) ) );

    io_md_writer_init( &((*fix).md_writer), &((*fix).db_reader), TAG_BREAK, TAG_LINK1, TAG_LINK2, TAG_LINK3, &((*fix).xml_writer) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    io_md_writer_destroy( &((*fix).md_writer) );

    io_xml_writer_destroy( &((*fix).xml_writer) );

    universal_memory_output_stream_destroy( &((*fix).out_stream) );

    ctrl_controller_destroy( &((*fix).controller) );

    data_database_reader_destroy( &((*fix).db_reader) );

    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static data_row_t create_root_diag( ctrl_controller_t *controller )
{
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( controller );

    /* create a diagram */
    data_row_t root_diag_id;

    data_diagram_t root_diagram;
    data_err = data_diagram_init_new( &root_diagram,
                                      DATA_ROW_VOID /*=parent_diagram_id*/,
                                      DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                      "stereo_t",  /* stereotype */
                                      "Th& <root> d\"agram",
                                      "diagram_description-root",
                                      10555 ,/*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE
                                    );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    root_diag_id = DATA_ROW_VOID;
    ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                        &root_diagram,
                                                        CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                        &root_diag_id
                                                      );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == ctrl_err );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_VOID != root_diag_id );
    data_diagram_destroy ( &root_diagram );

    return root_diag_id;
    return TEST_CASE_RESULT_OK;
}


static test_case_result_t test_md_plain_mixed( test_fixture_t *fix )
{
    assert( fix != NULL );
    int err;

    /* plain */
    err = io_xml_writer_write_plain ( &((*fix).xml_writer), "<!DTD><body>" );
    TEST_EXPECT_EQUAL_INT( 0, err );

    /* xml enc */
    err = io_md_writer_transform ( &((*fix).md_writer), "plain &amp; \"\'<>D3D#name#id" );
    TEST_EXPECT_EQUAL_INT( 0, err );

    /* md */
    err = io_md_writer_transform ( &((*fix).md_writer), "ln 1a\nln 1b\n\nln 2\n- ln 3\n4\n5 ln 5\n\n\nln 7a\n ln 7b\n> ln8\n" );
    TEST_EXPECT_EQUAL_INT( 0, err );

    /* plain */
    err = io_xml_writer_write_plain ( &((*fix).xml_writer), "</body>" );
    TEST_EXPECT_EQUAL_INT( 0, err );

    static const char expected[] = "<!DTD><body>plain &amp;amp; &quot;\'&lt;&gt;D3D#name#id"
        "ln 1a\nln 1b" TAG_BREAK "\nln 2" TAG_BREAK "- ln 3" TAG_BREAK "4" TAG_BREAK "5 ln 5" TAG_BREAK TAG_BREAK "\nln 7a\n ln 7b" TAG_BREAK "&gt; ln8\n"
        "</body>";
    //fprintf( stdout, "%s\n", &(expected[0]) );
    //fprintf( stdout, "%s\n", &(my_out_buffer[0]) );
    //fflush(stdout);
    TEST_ENVIRONMENT_ASSERT( sizeof( (*fix).out_buffer ) >= sizeof(expected)-1 );
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), expected, sizeof(expected)-1 ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_valid_links( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_row_t root_diag_id = create_root_diag( &((*fix).controller) );
    TEST_ENVIRONMENT_ASSERT( 1 == root_diag_id );  /* otherwise D0001 needs to be adapted (hint: delete old database file) */
    int err;

    /* 2 valid links, 1 valid but half link */
    err = io_md_writer_transform ( &((*fix).md_writer), ">D3DD0001#name#idD0001#id#nameD0001" );
    TEST_EXPECT_EQUAL_INT( 0, err );

    static const char expected[] = "&gt;D3D" TAG_LINK1 "D0001" TAG_LINK2 "Th&amp; &lt;root&gt; d&quot;agram" TAG_LINK3 "#id"
        TAG_LINK1 "D0001" TAG_LINK2 "D0001" TAG_LINK3;
    //fprintf( stdout, "%s\n", &(expected[0]) );
    //fprintf( stdout, "%s\n", &(my_out_buffer[0]) );
    //fflush(stdout);
    TEST_ENVIRONMENT_ASSERT( sizeof( (*fix).out_buffer ) >= sizeof(expected)-1 );
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), expected, sizeof(expected)-1 ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_invalid_links( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_row_t root_diag_id = create_root_diag( &((*fix).controller) );
    TEST_ENVIRONMENT_ASSERT( 1 == root_diag_id );  /* otherwise D0001 needs to be adapted (hint: delete old database file) */
    int err;

    /* 2 invalid links, 1 valid but non-existig link, 1 valid but half link */
    err = io_md_writer_transform ( &((*fix).md_writer), ">D3DD001#nameC0001#idD0002#id#nameD0001#nam" );
    TEST_EXPECT_EQUAL_INT( 0, err );

    static const char expected[] = "&gt;D3DD001#nameC0001#idD0002#id#nameD0001#nam";
    //fprintf( stdout, "%s\n", &(expected[0]) );
    //fprintf( stdout, "%s\n", &(my_out_buffer[0]) );
    //fflush(stdout);
    TEST_ENVIRONMENT_ASSERT( sizeof( (*fix).out_buffer ) >= sizeof(expected)-1 );
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), expected, sizeof(expected)-1 ) );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2020-2024 Andreas Warnke
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
