/* File: io_export_model_traversal_test.c; Copyright and License: see below */

#include "io_export_model_traversal_test.h"
#include "io_export_model_traversal.h"
#include "xmi/xmi_element_writer.h"
#include "stream/universal_memory_output_stream.h"
#include "set/data_stat.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "test_assert.h"
#include <glib.h>
        
static void set_up(void);
static void tear_down(void);
static void iterate_types_on_mini_model(void);

static data_row_id_t create_mini_model();  /* helper function to initialize the database */
static int check_if_xml_valid();  /* helper function to validate xml */

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
 *  \brief stream to write to a memory buffer
 */
static universal_memory_output_stream_t mem_output_stream;

/*!
 *  \brief mem-buffer to be written by mem_output_stream
 */
static char mem_buffer[8192];

test_suite_t io_export_model_traversal_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "io_export_model_traversal_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "iterate_types_on_mini_model", &iterate_types_on_mini_model );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open_in_memory( &database );
    data_database_reader_init( &db_reader, &database );
    ctrl_controller_init( &controller, &database );
    
    universal_memory_output_stream_init( &mem_output_stream,
                                         &mem_buffer,
                                         sizeof(mem_buffer)
                                       );
    universal_memory_output_stream_open( &mem_output_stream, "dummy_path" );
}

static void tear_down(void)
{
    universal_memory_output_stream_close( &mem_output_stream );
    universal_memory_output_stream_destroy( &mem_output_stream );
    
    ctrl_controller_destroy( &controller );
    data_database_reader_destroy( &db_reader );
    data_database_close( &database );
    data_database_destroy( &database );
}

static data_row_id_t create_mini_model()
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    data_row_id_t root_diag_id;

    data_diagram_t root_diagram;
    data_err = data_diagram_init ( &root_diagram,
                                   DATA_ROW_ID_VOID /*=diagram_id is ignored*/,
                                   DATA_ROW_ID_VOID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                   "the_root_diag",
                                   "diagram_description-root",
                                   10555 /*=list_order*/
                                 );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    root_diag_id = DATA_ROW_ID_VOID;
    ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                        &root_diagram,
                                                        false, /* add_to_latest_undo_set */
                                                        &root_diag_id
                                                      );
    TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );
    data_diagram_destroy ( &root_diagram );

    return root_diag_id;
}

static GMarkupParser DEV_NULL_PARSER = {
  .start_element=NULL,
  .end_element=NULL,
  .text=NULL,
  .passthrough=NULL,
  .error=NULL
};

int check_if_xml_valid()
{
    bool success;
    GMarkupParseContext *my_ctx = g_markup_parse_context_new ( &DEV_NULL_PARSER, 0, NULL, NULL );
    TEST_ENVIRONMENT_ASSERT( NULL != my_ctx );
    success = g_markup_parse_context_parse ( my_ctx, &(mem_buffer[0]), strlen(mem_buffer), NULL );
    success &= g_markup_parse_context_end_parse ( my_ctx, NULL );
    g_markup_parse_context_free( my_ctx );
    return ( success ? 0 : -1 );
}

static void iterate_types_on_mini_model(void)
{
    data_row_id_t root_diag_id = create_mini_model();

    data_stat_t stat;
    data_stat_init(&stat);
    {
        data_visible_set_t temp_input_data;  /*!< buffer to cache the diagram data */
        io_export_model_traversal_t temp_model_traversal;  /*!< own instance of a model_traversal for text export */
        xmi_element_writer_t temp_xmi_writer;  /*!< memory for a temporary xmi writer */
        
        universal_output_stream_t* output = universal_memory_output_stream_get_output_stream( &mem_output_stream );
        
        {
            xmi_element_writer_init( &temp_xmi_writer, &db_reader, &stat, output );
            /* init the model_traversal */
            io_export_model_traversal_init( &temp_model_traversal,
                                            &db_reader,
                                            &temp_input_data,
                                            &stat,
                                            &temp_xmi_writer
                                          );
            /* write the document */
            int export_err = 0;
            export_err |= xmi_element_writer_write_header( &temp_xmi_writer, "document file name" );
            export_err |= xmi_element_writer_start_main( &temp_xmi_writer, "document file name" );
            xmi_element_writer_set_mode( &temp_xmi_writer, IO_WRITER_PASS_BASE );
            export_err |= io_export_model_traversal_walk_model_nodes( &temp_model_traversal );
            export_err |= xmi_element_writer_end_main( &temp_xmi_writer );
            xmi_element_writer_set_mode( &temp_xmi_writer, IO_WRITER_PASS_PROFILE );
            export_err |= io_export_model_traversal_walk_model_nodes( &temp_model_traversal );
            export_err |= xmi_element_writer_write_footer( &temp_xmi_writer );

            TEST_ASSERT_EQUAL_INT( 0, export_err );
            
            io_export_model_traversal_destroy( &temp_model_traversal );
            xmi_element_writer_destroy( &temp_xmi_writer );
        }

        
    }
    //static const char TERM2[4]="qQe\n";
    //universal_memory_output_stream_write( &mem_output_stream, &TERM2, sizeof(TERM2) );
    static const char TERM='\00';
    int write_err = universal_memory_output_stream_write( &mem_output_stream, &TERM, sizeof(TERM) );
    TEST_ENVIRONMENT_ASSERT( 0 == write_err );
    universal_memory_output_stream_flush( &mem_output_stream );
    fprintf( stdout, "%s", &(mem_buffer[0]) );
    
    TEST_ASSERT_EQUAL_INT( 0, check_if_xml_valid() );
    
    data_stat_trace(&stat);
    data_stat_destroy(&stat);
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
