/* File: test_main.c; Copyright and License: see below */

/* data */
#include "unit/data_change_notifier_test.h"
#include "unit/data_small_set_test.h"
#include "unit/data_uuid_test.h"
#include "unit/data_rules_test.h"
#include "unit/data_database_listener_test.h"
#include "integration/data_database_reader_test.h"
/* ctrl */
#include "unit/ctrl_diagram_controller_test.h"
#include "unit/ctrl_controller_test.h"
#include "unit/ctrl_multi_step_changer_test.h"
#include "unit/ctrl_classifier_controller_test.h"
#include "unit/ctrl_consistency_checker_test.h"
#include "unit/ctrl_undo_redo_list_test.h"
#include "unit/ctrl_diagram_policy_enforcer_test.h"
#include "unit/ctrl_classifier_policy_enforcer_test.h"
/* pencil */
#include "unit/geometry_rectangle_test.h"
#include "unit/geometry_connector_test.h"
#include "unit/geometry_non_linear_scale_test.h"
#include "unit/draw_classifier_contour_test.h"
#include "unit/pencil_layout_data_test.h"
#include "unit/pencil_classifier_composer_test.h"
#include "integration/pencil_layouter_test.h"
#include "integration/pencil_diagram_maker_test.h"
/* gui */
#include "unit/gui_sketch_nav_tree_test.h"
/* io */
#include "unit/txt_writer_test.h"
#include "unit/json_token_reader_test.h"
#include "unit/md_filter_test.h"
#include "unit/io_import_elements_test.h"
#include "integration/io_importer_test.h"
#include "integration/io_export_model_traversal_test.h"
/* u8stream */
#include "unit/universal_array_index_sorter_test.h"
#include "unit/universal_array_list_test.h"
#include "unit/universal_memory_output_stream_test.h"
#include "unit/universal_escaping_output_stream_test.h"
#include "unit/universal_memory_input_stream_test.h"
#include "unit/universal_buffer_input_stream_test.h"
#include "unit/universal_buffer_output_stream_test.h"
/* u8buf */
#include "unit/universal_arena_list_test.h"
#include "unit/universal_memory_arena_test.h"
#include "unit/utf8stringbuf_test.h"
#include "unit/utf8codepoint_test.h"
#include "unit/utf8codepointiterator_test.h"
#include "unit/utf8string_test.h"
/* #include "unit/utf8stringbuf_performance.h" -- performance measurements are not reliable - depend on system config+load */
#include "unit/utf8stringview_test.h"
#include "unit/utf8stringviewiterator_test.h"
#include "utf8stringbuf/utf8string.h"
/* TEST_ENVIRONMENT */
#include "trace/trace.h"
#include "tslog/tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "test_runner.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>

/*!
 *  \brief main runs the unit tests
 */
int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    TRACE_INFO( "--------------------" );
    int exit_code = 0;
    TSLOG_INIT(META_INFO_PROGRAM_ID_STR);

    /* print id, license and copyrights */
    {
        fprintf( stdout, "    + %s %s\n", META_INFO_PROGRAM_ID_STR, META_VERSION_STR );
        fprintf( stdout, "    + Copyright: %s\n", META_INFO_COPYRIGHT_STR );
        fprintf( stdout, "    + License: %s\n", META_INFO_LICENSE_STR );
    }

    bool do_unit_tests = false;
    bool do_integration_tests = false;

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    %s -h for help\n", argv[0] );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -u to run the unit-tests\n", argv[0] );
            fprintf( stdout, "        (test functions of single software units\n" );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -i to run the integration tests\n", argv[0] );
            fprintf( stdout, "        (test interactions between several software parts\n" );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -a to run all tests\n", argv[0] );
        }
        if ( utf8string_equals_str( argv[1], "-u" ) )
        {
            do_unit_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-i" ) )
        {
            do_integration_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-m" ) )  /* compatibility to 1.27.3 and older */
        {
            do_integration_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-a" ) )
        {
            do_unit_tests = true;
            do_integration_tests = true;
        }
    }

    /* initialize the base libraries: gobject, gio, glib, gdk and gtk */
    {
#if ( GTK_MAJOR_VERSION >= 4 )
        const gboolean success = gtk_init_check();
#else
        const gboolean success = gtk_init_check(&argc, &argv);
#endif
        if ( ! success )
        {
            TSLOG_WARNING("gtk could not be initialized.");
        }
    }

    /* start tests */
    test_runner_t runner;
    test_runner_init ( &runner );

    /* unit-tests test single software units and try to cover all control flows */
    if ( do_unit_tests )
    {
        /* data */
        test_runner_run_suite( &runner, data_small_set_test_get_suite() );
        test_runner_run_suite( &runner, data_rules_test_get_suite() );
        test_runner_run_suite( &runner, data_uuid_test_get_suite() );
        test_runner_run_suite( &runner, data_change_notifier_test_get_suite() );
        test_runner_run_suite( &runner, data_database_listener_test_get_suite() );
        /* ctrl */
        /* pencil */
        test_runner_run_suite( &runner, geometry_rectangle_test_get_suite() );
        test_runner_run_suite( &runner, geometry_connector_test_get_suite() );
        test_runner_run_suite( &runner, geometry_non_linear_scale_test_get_suite() );
        test_runner_run_suite( &runner, draw_classifier_contour_test_get_suite() );
        test_runner_run_suite( &runner, pencil_classifier_composer_test_get_suite() );
        test_runner_run_suite( &runner, pencil_layout_data_test_get_suite() );
        /* gui */
        /* io */
        test_runner_run_suite( &runner, txt_writer_test_get_suite() );
        test_runner_run_suite( &runner, json_token_reader_test_get_suite() );
        test_runner_run_suite( &runner, io_import_elements_test_get_suite() );
        /* universal */
        test_runner_run_suite( &runner, universal_array_index_sorter_test_get_suite() );
        test_runner_run_suite( &runner, universal_array_list_test_get_suite() );
        test_runner_run_suite( &runner, universal_memory_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_escaping_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_memory_input_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_buffer_input_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_buffer_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_memory_arena_test_get_suite() );
        test_runner_run_suite( &runner, universal_arena_list_test_get_suite() );
        /* utf8stringbuf */
        test_runner_run_suite( &runner, utf8codepoint_test_get_suite() );
        test_runner_run_suite( &runner, utf8codepointiterator_test_get_suite() );
        test_runner_run_suite( &runner, utf8stringbuf_test_get_suite() );
        test_runner_run_suite( &runner, utf8string_test_get_suite() );
        /* test_runner_run_suite( &runner, utf8stringbuf_performance_get_list() ); -- performance measurements are not reliable - depend on system config+load */
        test_runner_run_suite( &runner, utf8stringview_test_get_suite() );
        test_runner_run_suite( &runner, utf8stringviewiterator_test_get_suite() );
        test_runner_run_suite( &runner, gui_sketch_nav_tree_test_get_suite() );
    }

    /* integration tests test multiple software units and their interactions */
    if ( do_integration_tests )
    {
        /* data */
        test_runner_run_suite( &runner, data_database_reader_test_get_suite() );
        /* ctrl */
        test_runner_run_suite( &runner, ctrl_controller_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_multi_step_changer_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_diagram_controller_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_classifier_controller_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_consistency_checker_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_undo_redo_list_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_diagram_policy_enforcer_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_classifier_policy_enforcer_test_get_suite() );
        /* pencil */
        test_runner_run_suite( &runner, pencil_layouter_test_get_suite() );
        test_runner_run_suite( &runner, pencil_diagram_maker_test_get_suite() );
        /* gui */
        /* io */
        test_runner_run_suite( &runner, io_importer_test_get_suite() );
        test_runner_run_suite( &runner, md_filter_test_get_suite() );
        test_runner_run_suite( &runner, io_export_model_traversal_test_get_suite() );
        /* universal */
        /* utf8stringbuf */
    }

    /* fetch failures */
    test_result_t res = test_get_result( &runner );
    fprintf( stdout,
             "ALL TESTS - RESULT: total %d, failed: %d\n",
             test_result_get_total( &res ),
             test_result_get_failed( &res )
           );
    exit_code = test_result_get_failed( &res );

    TSLOG_DESTROY();
    TRACE_INFO( "--------------------" );
    TRACE_TIMESTAMP();
    TRACE_END_ERR( exit_code );
    return exit_code;
}


/*
 * Copyright 2016-2022 Andreas Warnke
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
