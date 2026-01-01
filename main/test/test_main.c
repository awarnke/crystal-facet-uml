/* File: test_main.c; Copyright and License: see below */

/* data */
#include "unit/entity_data_diagram_test.h"
#include "unit/entity_data_diagramelement_test.h"
#include "unit/entity_data_classifier_test.h"
#include "unit/entity_data_feature_test.h"
#include "unit/entity_data_relationship_test.h"
#include "unit/entity_data_head_test.h"
#include "unit/entity_data_id_test.h"
#include "unit/entity_data_type_test.h"
#include "unit/entity_data_uuid_test.h"
#include "unit/set_data_full_id_list_test.h"
#include "unit/set_data_full_id_test.h"
#include "unit/set_data_search_result_list_test.h"
#include "unit/set_data_search_result_test.h"
#include "unit/set_data_small_set_test.h"
#include "unit/set_data_stat_test.h"
#include "unit/set_data_visible_classifier_test.h"
#include "unit/data_change_notifier_test.h"
#include "unit/data_rules_test.h"
#include "unit/data_database_listener_test.h"
#include "unit/data_database_head_test.h"
#include "integration/data_database_reader_test.h"
#include "integration/data_database_text_search_test.h"
#include "integration/data_profile_part_test.h"
#include "integration/data_visible_set_test.h"
/* ctrl */
#include "integration/ctrl_diagram_controller_test.h"
#include "integration/ctrl_multi_step_changer_test.h"
#include "integration/ctrl_classifier_controller_test.h"
#include "integration/ctrl_consistency_checker_test.h"
#include "integration/ctrl_undo_redo_list_test.h"
#include "integration/consistency_lifeline_test.h"
#include "integration/consistency_drop_invisibles_test.h"
#include "unit/ctrl_undo_redo_iterator_test.h"
/* pencil */
#include "unit/geometry__test.h"
#include "unit/geometry_anchor_test.h"
#include "unit/geometry_rectangle_test.h"
#include "unit/geometry_connector_test.h"
#include "unit/geometry_non_linear_scale_test.h"
#include "unit/layout_visible_set_test.h"
#include "unit/draw_classifier_contour_test.h"
#include "unit/draw_stereotype_icon_test.h"
#include "unit/pencil_classifier_composer_test.h"
#include "integration/pencil_layouter_test.h"
#include "integration/pencil_diagram_maker_test.h"
/* gui */
#include "unit/gui_sketch_nav_tree_test.h"
/* io */
#include "unit/io_txt_writer_test.h"
#include "unit/json_token_reader_test.h"
#include "unit/io_md_writer_test.h"
#include "unit/io_import_elements_test.h"
#include "integration/io_data_file_test.h"
#include "integration/io_importer_test.h"
#include "integration/io_export_model_traversal_test.h"
/* u8stream */
#include "unit/u8__test.h"
#include "unit/universal_array_index_sorter_test.h"
#include "unit/universal_array_list_test.h"
#include "unit/universal_memory_output_stream_test.h"
#include "unit/universal_escaping_output_stream_test.h"
#include "unit/universal_memory_input_stream_test.h"
#include "unit/universal_buffer_input_stream_test.h"
#include "unit/universal_buffer_output_stream_test.h"
#include "unit/universal_arena_list_test.h"
#include "unit/universal_memory_arena_test.h"
#include "unit/utf8stringbuf_test.h"
#include "unit/utf8codepoint_test.h"
#include "unit/utf8codepointiterator_test.h"
#include "unit/utf8string_test.h"
#include "unit/utf8stringbuf_performance.h"
#include "unit/utf8stringview_test.h"
#include "unit/utf8stringviewiterator_test.h"
#include "unit/utf8stringviewtokenizer_test.h"
#include "unit/utf8stringlines_test.h"
#include "unit/utf8stream_writer_test.h"
#include "integration/u8dir_file_test.h"
#include "integration/universal_file_input_stream_test.h"
#include "integration/universal_file_output_stream_test.h"
#include "integration/universal_stream_output_stream_test.h"
/* TEST_ENVIRONMENT */
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "test_runner.h"
#if __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#include <gtk/gtk.h>
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
#include <stdbool.h>
#include <stdio.h>

/*!
 *  \brief main runs the unit tests
 */
int main (int argc, char *argv[]) {
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    U8_TRACE_INFO( "--------------------" );
    int exit_code = 0;
    U8_LOG_INIT(META_INFO_PROGRAM_ID_STR);

    /* print id, license and copyrights */
    {
        fprintf( stdout, "    + %s %s\n", META_INFO_PROGRAM_ID_STR, META_VERSION_STR );
        fprintf( stdout, "    + Copyright: %s\n", META_INFO_COPYRIGHT_STR );
        fprintf( stdout, "    + License: %s\n", META_INFO_LICENSE_STR );
    }

    test_category_t mask = 0;
    test_category_t pattern = -1;

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    %s -h for help\n", argv[0] );
            /* G_ENABLE_DIAGNOSTIC=1 prints more information on misusing the gtk libraries */
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -a to run all continuous tests\n", argv[0] );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -R to run all continuous and release tests\n", argv[0] );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -@ to run only non-regular quest tests\n", argv[0] );

            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -U to run regular unit tests\n", argv[0] );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -I to run regular unit and integration tests\n", argv[0] );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -Q to run regular qualification tests\n", argv[0] );
            fprintf( stdout, "    G_ENABLE_DIAGNOSTIC=1 %s -C to run regular coverage tests\n", argv[0] );
        }
        if ( utf8string_equals_str( argv[1], "-a" ) )
        {
            mask = TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_QUEST;
            pattern = TEST_CATEGORY_CONTINUOUS;
        }
        if ( utf8string_equals_str( argv[1], "-R" ) )
        {
            mask = TEST_CATEGORY_QUEST;
            pattern = 0x0;
        }
        if ( utf8string_equals_str( argv[1], "-@" ) )
        {
            mask = TEST_CATEGORY_QUEST;
            pattern = TEST_CATEGORY_QUEST;
        }
        if ( utf8string_equals_str( argv[1], "-U" ) )
        {
            mask = TEST_CATEGORY_UNIT | TEST_CATEGORY_QUEST;
            pattern = TEST_CATEGORY_UNIT;
        }
        if ( utf8string_equals_str( argv[1], "-I" ) )
        {
            mask = TEST_CATEGORY_QUALIFICATION | TEST_CATEGORY_QUEST;
            pattern = 0x0;
        }
        if ( utf8string_equals_str( argv[1], "-Q" ) )
        {
            mask = TEST_CATEGORY_QUALIFICATION | TEST_CATEGORY_QUEST;
            pattern = TEST_CATEGORY_QUALIFICATION;
        }
        if ( utf8string_equals_str( argv[1], "-C" ) )
        {
            mask = TEST_CATEGORY_COVERAGE | TEST_CATEGORY_QUEST;
            pattern = TEST_CATEGORY_COVERAGE;
        }
    }

    /* initialize the base libraries: gobject, gio, glib, gdk and gtk */
    {
        const gboolean success = gtk_init_check();
        if ( ! success )
        {
            U8_LOG_WARNING("gtk could not be initialized.");
        }
    }

    /* start tests */
    test_runner_t runner;
    test_runner_init( &runner );
    test_runner_set_filter( &runner, mask, pattern );

    /* run all test cases, filtering is performed by the test runner (unless pattern contains masked bits) */
    if ( ( pattern & ( ~ mask ) ) == 0x0 )
    {
        /* u8 */
        test_runner_run_suite( &runner, u8__test_get_suite() );
        test_runner_run_suite( &runner, u8dir_file_test_get_suite() );
        test_runner_run_suite( &runner, universal_array_index_sorter_test_get_suite() );
        test_runner_run_suite( &runner, universal_array_list_test_get_suite() );
        test_runner_run_suite( &runner, universal_memory_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_escaping_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_memory_input_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_buffer_input_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_buffer_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_memory_arena_test_get_suite() );
        test_runner_run_suite( &runner, universal_arena_list_test_get_suite() );
        test_runner_run_suite( &runner, universal_file_input_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_file_output_stream_test_get_suite() );
        test_runner_run_suite( &runner, universal_stream_output_stream_test_get_suite() );
        /* u8/utf8stringbuf */
        test_runner_run_suite( &runner, utf8codepoint_test_get_suite() );
        test_runner_run_suite( &runner, utf8codepointiterator_test_get_suite() );
        test_runner_run_suite( &runner, utf8stringbuf_test_get_suite() );
        test_runner_run_suite( &runner, utf8string_test_get_suite() );
#ifndef NDEBUG
        /* test_runner_run_suite( &runner, utf8stringbuf_performance_get_list() ); -- performance measurements are not reliable - depend on system config+load */
#endif
        test_runner_run_suite( &runner, utf8stringview_test_get_suite() );
        test_runner_run_suite( &runner, utf8stringviewiterator_test_get_suite() );
        test_runner_run_suite( &runner, utf8stringviewtokenizer_test_get_suite() );
        test_runner_run_suite( &runner, utf8stringlines_test_get_suite() );
        test_runner_run_suite( &runner, utf8stream_writer_test_get_suite() );

        /* data */
        test_runner_run_suite( &runner, entity_data_diagram_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_diagramelement_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_classifier_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_feature_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_relationship_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_head_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_id_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_type_test_get_suite() );
        test_runner_run_suite( &runner, entity_data_uuid_test_get_suite() );
        test_runner_run_suite( &runner, set_data_full_id_list_test_get_suite() );
        test_runner_run_suite( &runner, set_data_full_id_test_get_suite() );
        test_runner_run_suite( &runner, set_data_search_result_list_test_get_suite() );
        test_runner_run_suite( &runner, set_data_search_result_test_get_suite() );
        test_runner_run_suite( &runner, set_data_small_set_test_get_suite() );
        test_runner_run_suite( &runner, set_data_stat_test_get_suite() );
        test_runner_run_suite( &runner, set_data_visible_classifier_test_get_suite() );
        test_runner_run_suite( &runner, data_rules_test_get_suite() );
        test_runner_run_suite( &runner, data_change_notifier_test_get_suite() );
        test_runner_run_suite( &runner, data_database_listener_test_get_suite() );
        test_runner_run_suite( &runner, data_database_head_test_get_suite() );

        test_runner_run_suite( &runner, data_database_reader_test_get_suite() );
        test_runner_run_suite( &runner, data_database_text_search_test_get_suite() );
        test_runner_run_suite( &runner, data_profile_part_test_get_suite() );
        test_runner_run_suite( &runner, data_visible_set_test_get_suite() );

        /* ctrl */
        test_runner_run_suite( &runner, ctrl_undo_redo_iterator_test_get_suite() );

        test_runner_run_suite( &runner, ctrl_multi_step_changer_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_diagram_controller_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_classifier_controller_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_consistency_checker_test_get_suite() );
        test_runner_run_suite( &runner, ctrl_undo_redo_list_test_get_suite() );
        test_runner_run_suite( &runner, consistency_lifeline_test_get_suite() );
        test_runner_run_suite( &runner, consistency_drop_invisibles_test_get_suite() );

        /* pencil */
        test_runner_run_suite( &runner, geometry__test_get_suite() );
        test_runner_run_suite( &runner, geometry_anchor_test_get_suite() );
        test_runner_run_suite( &runner, geometry_rectangle_test_get_suite() );
        test_runner_run_suite( &runner, geometry_connector_test_get_suite() );
        test_runner_run_suite( &runner, geometry_non_linear_scale_test_get_suite() );
        test_runner_run_suite( &runner, layout_visible_set_test_get_suite() );
        test_runner_run_suite( &runner, draw_classifier_contour_test_get_suite() );
        test_runner_run_suite( &runner, draw_stereotype_icon_test_get_suite() );
        test_runner_run_suite( &runner, pencil_classifier_composer_test_get_suite() );

        test_runner_run_suite( &runner, pencil_layouter_test_get_suite() );
        test_runner_run_suite( &runner, pencil_diagram_maker_test_get_suite() );

        /* io */
        test_runner_run_suite( &runner, io_txt_writer_test_get_suite() );
        test_runner_run_suite( &runner, json_token_reader_test_get_suite() );
        test_runner_run_suite( &runner, io_md_writer_test_get_suite() );
        test_runner_run_suite( &runner, io_import_elements_test_get_suite() );

        test_runner_run_suite( &runner, io_data_file_test_get_suite() );
        test_runner_run_suite( &runner, io_importer_test_get_suite() );
        test_runner_run_suite( &runner, io_export_model_traversal_test_get_suite() );

        /* gui */
        test_runner_run_suite( &runner, gui_sketch_nav_tree_test_get_suite() );
    }

    /* fetch failures */
    test_result_t res = test_get_result( &runner );
    fprintf( stdout,
             "ALL TESTS - RESULT: skipped: %d, passed %d, failed: %d\n",
             test_result_get_skipped( &res ),
             test_result_get_passed( &res ),
             test_result_get_failed( &res )
           );
    exit_code = test_result_get_failed( &res );

    U8_LOG_STATS();
    U8_LOG_DESTROY();
    U8_TRACE_INFO( "--------------------" );
    U8_TRACE_TIMESTAMP();
    U8_TRACE_END_ERR( exit_code );
    return exit_code;
}


/*
 * Copyright 2016-2026 Andreas Warnke
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
