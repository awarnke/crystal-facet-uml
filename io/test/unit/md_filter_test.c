/* File: md_filter_test.c; Copyright and License: see below */

#include "md_filter_test.h"
#include "md/md_filter.h"
#include "xml/xml_writer.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "stream/universal_memory_output_stream.h"
#include "trace.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_md_plain_mixed(void);
static void test_valid_links(void);
static void test_invalid_links(void);

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

/*!
 *  \brief md_filter to be tested
 */
static md_filter_t md_filter;

#define TAG_BREAK "&LN;"
#define TAG_LINK1 "<REF TO=\""
#define TAG_LINK2 "\">"
#define TAG_LINK3 "</REF>"

static char my_out_buffer[200];
static universal_memory_output_stream_t my_out_stream;
static xml_writer_t xml_writer;

test_suite_t md_filter_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "md_filter_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_md_plain_mixed", &test_md_plain_mixed );
    test_suite_add_test_case( &result, "test_valid_links", &test_valid_links );
    test_suite_add_test_case( &result, "test_invalid_links", &test_invalid_links );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );

    memset( &my_out_buffer, '\0', sizeof(my_out_buffer) );
    universal_memory_output_stream_init( &my_out_stream, &my_out_buffer, sizeof(my_out_buffer) );
    xml_writer_init( &xml_writer, universal_memory_output_stream_get_output_stream( &my_out_stream ) );

    md_filter_init( &md_filter, &db_reader, TAG_BREAK, TAG_LINK1, TAG_LINK2, TAG_LINK3, &xml_writer );
}

static void tear_down(void)
{
    md_filter_destroy( &md_filter );

    xml_writer_destroy( &xml_writer );

    universal_memory_output_stream_destroy( &my_out_stream );

    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static data_row_id_t create_root_diag()
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a diagram */
    data_row_id_t root_diag_id;

    data_diagram_t root_diagram;
    data_err = data_diagram_init_new( &root_diagram,
                                      DATA_ROW_ID_VOID /*=parent_diagram_id*/,
                                      DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                      "Th& <root> d\"agram",
                                      "diagram_description-root",
                                      10555 /*=list_order*/
                                    );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    root_diag_id = DATA_ROW_ID_VOID;
    ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                        &root_diagram,
                                                        CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                        &root_diag_id
                                                      );
    TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );
    data_diagram_destroy ( &root_diagram );

    return root_diag_id;
}


static void test_md_plain_mixed(void)
{
    int err;

    /* plain */
    err = xml_writer_write_plain ( &xml_writer, "<!DTD><body>" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* xml enc */
    err = md_filter_transform ( &md_filter, "plain &amp; \"\'<>D3D#name#id" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* md */
    err = md_filter_transform ( &md_filter, "ln 1a\nln 1b\n\nln 2\n- ln 3\n4\n5 ln 5\n\n\nln 7a\n ln 7b\n> ln8\n" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* plain */
    err = xml_writer_write_plain ( &xml_writer, "</body>" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    static const char expected[] = "<!DTD><body>plain &amp;amp; &quot;\'&lt;&gt;D3D#name#id"
        "ln 1a\nln 1b" TAG_BREAK "\nln 2" TAG_BREAK "- ln 3" TAG_BREAK "4" TAG_BREAK "5 ln 5" TAG_BREAK TAG_BREAK "\nln 7a\n ln 7b" TAG_BREAK "&gt; ln8\n"
        "</body>";
    //fprintf( stdout, "%s\n", &(expected[0]) );
    //fprintf( stdout, "%s\n", &(my_out_buffer[0]) );
    //fflush(stdout);
    TEST_ENVIRONMENT_ASSERT( sizeof(my_out_buffer) >= sizeof(expected)-1 );
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, expected, sizeof(expected)-1 ) );
}

static void test_valid_links(void)
{
    data_row_id_t root_diag_id = create_root_diag();
    TEST_ENVIRONMENT_ASSERT( 1 == root_diag_id );  /* otherwise D0001 needs to be adapted (hint: delete old database file) */
    int err;

    /* 2 valid links, 1 valid but half link */
    err = md_filter_transform ( &md_filter, ">D3DD0001#name#idD0001#id#nameD0001" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    static const char expected[] = "&gt;D3D" TAG_LINK1 "D0001" TAG_LINK2 "Th&amp; &lt;root&gt; d&quot;agram" TAG_LINK3 "#id"
        TAG_LINK1 "D0001" TAG_LINK2 "D0001" TAG_LINK3;
    //fprintf( stdout, "%s\n", &(expected[0]) );
    //fprintf( stdout, "%s\n", &(my_out_buffer[0]) );
    //fflush(stdout);
    TEST_ENVIRONMENT_ASSERT( sizeof(my_out_buffer) >= sizeof(expected)-1 );
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, expected, sizeof(expected)-1 ) );
}

static void test_invalid_links(void)
{
    data_row_id_t root_diag_id = create_root_diag();
    TEST_ENVIRONMENT_ASSERT( 1 == root_diag_id );  /* otherwise D0001 needs to be adapted (hint: delete old database file) */
    int err;

    /* 2 invalid links, 1 valid but non-existig link, 1 valid but half link */
    err = md_filter_transform ( &md_filter, ">D3DD001#nameC0001#idD0002#id#nameD0001#nam" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    static const char expected[] = "&gt;D3DD001#nameC0001#idD0002#id#nameD0001#nam";
    //fprintf( stdout, "%s\n", &(expected[0]) );
    //fprintf( stdout, "%s\n", &(my_out_buffer[0]) );
    //fflush(stdout);
    TEST_ENVIRONMENT_ASSERT( sizeof(my_out_buffer) >= sizeof(expected)-1 );
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, expected, sizeof(expected)-1 ) );
}


/*
 * Copyright 2020-2021 Andreas Warnke
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
