/* File: unittest_main.c; Copyright and License: see below */

#include "data_change_notifier_test.h"
#include "data_small_set_test.h"
#include "data_database_listener_test.h"
#include "data_database_reader_test.h"
#include "ctrl_diagram_controller_test.h"
#include "ctrl_controller_test.h"
#include "ctrl_classifier_controller_test.h"
#include "ctrl_consistency_checker_test.h"
#include "ctrl_undo_redo_list_test.h"
#include "ctrl_diagram_policy_enforcer_test.h"
#include "ctrl_classifier_policy_enforcer_test.h"
#include "geometry_rectangle_test.h"
#include "geometry_connector_test.h"
#include "geometry_non_linear_scale_test.h"
#include "io_format_writer_test.h"
#include "json_tokenizer_test.h"
#include "pencil_layout_data_test.h"
#include "universal_array_index_sorter_test.h"
#include "utf8stringbuf_test.h"
#include "utf8codepoint_test.h"
#include "utf8string_test.h"
#include "utf8stringbuf_performance.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "util/string/utf8string.h"
#include <stdbool.h>
#include <test_runner.h>

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
        fprintf( stdout, "    + %s %s\n", META_INFO_PROGRAM_ID_STR, META_INFO_VERSION_STR );
        fprintf( stdout, "    + Copyright: %s\n", META_INFO_COPYRIGHT_STR );
        fprintf( stdout, "    + License: %s\n", META_INFO_LICENSE_STR );
    }

    bool do_unit_tests = false;
    bool do_module_tests = false;

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    %s -h for help\n", argv[0] );
            fprintf( stdout, "    %s -u to run the unit-tests (on single software units)\n", argv[0] );
            fprintf( stdout, "    %s -m to run the module tests (focusing on interaction between several units)\n", argv[0] );
            fprintf( stdout, "    %s -a to run all tests\n", argv[0] );
        }
        if ( utf8string_equals_str( argv[1], "-u" ) )
        {
            do_unit_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-m" ) )
        {
            do_module_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-a" ) )
        {
            do_unit_tests = true;
            do_module_tests = true;
        }
    }

    /* start tests */
    test_runner_t runner;
    test_runner_init ( &runner );

    /* unit-tests */
    if ( do_unit_tests )
    {
        test_runner_run_suite( &runner, data_small_set_test_get_list() );
        test_runner_run_suite( &runner, data_change_notifier_test_get_list() );
        test_runner_run_suite( &runner, data_database_listener_test_get_list() );
        test_runner_run_suite( &runner, geometry_rectangle_test_get_list() );
        test_runner_run_suite( &runner, geometry_connector_test_get_list() );
        test_runner_run_suite( &runner, geometry_non_linear_scale_test_get_list() );
        test_runner_run_suite( &runner, io_format_writer_test_get_list() );
        test_runner_run_suite( &runner, json_tokenizer_test_get_list() );
        test_runner_run_suite( &runner, pencil_layout_data_test_get_list() );
        test_runner_run_suite( &runner, universal_array_index_sorter_test_get_list() );
        test_runner_run_suite( &runner, utf8codepoint_test_get_list() );
        test_runner_run_suite( &runner, utf8stringbuf_test_get_list() );
        test_runner_run_suite( &runner, utf8string_test_get_list() );
        test_runner_run_suite( &runner, utf8stringbuf_performance_get_list()) ;
    }

    /* module tests which involve multiple software units */
    if ( do_module_tests )
    {
        test_runner_run_suite( &runner, data_database_reader_test_get_list() );
        test_runner_run_suite( &runner, ctrl_controller_test_get_list() );
        test_runner_run_suite( &runner, ctrl_diagram_controller_test_get_list() );
        test_runner_run_suite( &runner, ctrl_classifier_controller_test_get_list() );
        test_runner_run_suite( &runner, ctrl_consistency_checker_test_get_list() );
        test_runner_run_suite( &runner, ctrl_undo_redo_list_test_get_list() );
        test_runner_run_suite( &runner, ctrl_diagram_policy_enforcer_test_get_list() );
        test_runner_run_suite( &runner, ctrl_classifier_policy_enforcer_test_get_list() );
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
 * Copyright 2016-2019 Andreas Warnke
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
