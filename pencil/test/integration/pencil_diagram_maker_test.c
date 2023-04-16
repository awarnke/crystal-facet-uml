/* File: pencil_diagram_maker_test.c; Copyright and License: see below */

#include "pencil_diagram_maker_test.h"
#include "pencil_layouter.h"
#include "pencil_diagram_maker.h"
#include "geometry/geometry_rectangle.h"
#include "set/data_visible_set.h"
#include "set/data_profile_part.h"
#include "test_data/test_data_setup.h"
#include "u8/u8_trace.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t render_good_cases( test_fixture_t *test_env );
#ifndef NDEBUG
static test_case_result_t render_challenging_cases( test_fixture_t *test_env );
static test_case_result_t render_edge_cases( test_fixture_t *test_env );
#endif

#ifndef NDEBUG
#define PENCIL_DIAGRAM_MAKER_TEST_EXPORT_SAMPLES
#else
#endif

test_suite_t pencil_diagram_maker_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_diagram_maker_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "render_good_cases", &render_good_cases );
#ifdef PENCIL_DIAGRAM_MAKER_TEST_EXPORT_SAMPLES
    /* no need to test more drawing if the diagrams are not generated to files */
    test_suite_add_test_case( &result, "render_challenging_cases", &render_challenging_cases );
    test_suite_add_test_case( &result, "render_edge_cases", &render_edge_cases );
#endif
    return result;
}

static data_visible_set_t data_set;
static data_profile_part_t profile;
static pencil_diagram_maker_t painter;
static cairo_surface_t *surface;
static cairo_t *cr;
static geometry_rectangle_t diagram_bounds;

static test_fixture_t * set_up()
{
    data_profile_part_init( &profile );
    data_visible_set_init( &data_set );
    pencil_diagram_maker_init( &painter, &data_set, &profile );
    geometry_rectangle_init( &diagram_bounds, 0.0, 0.0, 640.0, 480.0 );
    surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32,
                                          (uint32_t) geometry_rectangle_get_width( &diagram_bounds ),
                                          (uint32_t) geometry_rectangle_get_height( &diagram_bounds )
                                        );
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( surface ) );
    cr = cairo_create (surface);
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( cr ) );
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
    cairo_destroy (cr);
    cairo_surface_finish ( surface );
    cairo_surface_destroy ( surface );
    geometry_rectangle_destroy( &diagram_bounds );
    pencil_diagram_maker_destroy( &painter );
    data_visible_set_destroy( &data_set );
    data_profile_part_destroy( &profile );
}

static void draw_background()
{
    U8_TRACE_BEGIN();
    /* draw paper */
    cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
    cairo_rectangle ( cr,
                      geometry_rectangle_get_left( &diagram_bounds ),
                      geometry_rectangle_get_top( &diagram_bounds ),
                      geometry_rectangle_get_width( &diagram_bounds ),
                      geometry_rectangle_get_height( &diagram_bounds )
                    );
    cairo_fill (cr);
    U8_TRACE_END();
}

#ifdef PENCIL_DIAGRAM_MAKER_TEST_EXPORT_SAMPLES
static void render_to_file( const test_data_setup_t *ts_case_setup, data_stat_t *render_stats )
{
    U8_TRACE_BEGIN();
    /* create filename */
    char filename_buf[48]="";
    utf8stringbuf_t filename = UTF8STRINGBUF(filename_buf);
    utf8stringbuf_append_str( filename, "test_pencil" );

    const unsigned int variant = test_data_setup_get_variant( ts_case_setup );
    switch ( test_data_setup_get_mode( ts_case_setup ) )
    {
        default:
        case TEST_DATA_SETUP_MODE_GOOD_CASES:
        {
            utf8stringbuf_append_str( filename, "_good" );
        }
        break;

        case TEST_DATA_SETUP_MODE_CHALLENGING_CASES:
        {
            utf8stringbuf_append_str( filename, "_cllg" );
        }
        break;

        case TEST_DATA_SETUP_MODE_EDGE_CASES:
        {
            utf8stringbuf_append_str( filename, "_edge" );
        }
        break;
    }
    utf8stringbuf_append_str( filename, "_" );
    utf8stringbuf_append_int( filename, data_stat_get_series_count( render_stats, DATA_STAT_SERIES_ERROR ) );
    utf8stringbuf_append_str( filename, "e_" );
    utf8stringbuf_append_int( filename, data_stat_get_series_count( render_stats, DATA_STAT_SERIES_WARNING ) );
    utf8stringbuf_append_str( filename, "w_" );
    utf8stringbuf_append_int( filename, data_stat_get_series_count( render_stats, DATA_STAT_SERIES_EXPORTED ) );
    utf8stringbuf_append_str( filename, "ok_" );
    utf8stringbuf_append_int( filename, variant );
    utf8stringbuf_append_str( filename, ".png" );
    U8_TRACE_INFO_STR("filename:", utf8stringbuf_get_string( filename ));

    /* finish surface */
    const cairo_status_t png_result
        = cairo_surface_write_to_png ( surface, utf8stringbuf_get_string( filename ) );
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == png_result );
    U8_TRACE_END();
}
#endif

static test_case_result_t render_good_cases( test_fixture_t *test_env )
{
    test_data_setup_t ts_setup;
    test_data_setup_init( &ts_setup, TEST_DATA_SETUP_MODE_GOOD_CASES );
    for ( ; test_data_setup_is_valid_variant( &ts_setup ); test_data_setup_next_variant( &ts_setup ) )
    {
        /* setup */
        test_data_setup_get_variant_data( &ts_setup, &data_set );
        draw_background();

        /* perform test: draw diagram */
        data_id_t void_id;
        data_id_init_void( &void_id );
        data_small_set_t void_set;
        data_small_set_init( &void_set );
        data_stat_t layout_stats;
        data_stat_init( &layout_stats );
        pencil_diagram_maker_define_grid( &painter, diagram_bounds );
        pencil_diagram_maker_layout_elements( &painter, NULL, cr );
        pencil_diagram_maker_show_overlaps( &painter, &layout_stats, cr );
        pencil_diagram_maker_draw ( &painter,
                                    void_id,
                                    void_id,
                                    &void_set,
                                    cr
                                  );

        /* check result */
        /* TODO, manual check for now */
#ifdef PENCIL_DIAGRAM_MAKER_TEST_EXPORT_SAMPLES
        render_to_file( &ts_setup, &layout_stats );
#endif
        data_stat_destroy( &layout_stats );
    }
    test_data_setup_destroy( &ts_setup );
    return TEST_CASE_RESULT_OK;
}

#ifndef NDEBUG
static test_case_result_t render_challenging_cases( test_fixture_t *test_env )
{
    test_data_setup_t ts_setup;
    test_data_setup_init( &ts_setup, TEST_DATA_SETUP_MODE_CHALLENGING_CASES );
    for ( ; test_data_setup_is_valid_variant( &ts_setup ); test_data_setup_next_variant( &ts_setup ) )
    {
        /* setup */
        test_data_setup_get_variant_data( &ts_setup, &data_set );
        draw_background();

        /* perform test: draw diagram */
        data_id_t void_id;
        data_id_init_void( &void_id );
        data_small_set_t void_set;
        data_small_set_init( &void_set );
        data_stat_t layout_stats;
        data_stat_init( &layout_stats );
        pencil_diagram_maker_define_grid( &painter, diagram_bounds );
        pencil_diagram_maker_layout_elements( &painter, NULL, cr );
        pencil_diagram_maker_show_overlaps( &painter, &layout_stats, cr );
        pencil_diagram_maker_draw ( &painter,
                                    void_id,
                                    void_id,
                                    &void_set,
                                    cr
                                  );

        /* check result */
        /* TODO, manual check for now */
#ifdef PENCIL_DIAGRAM_MAKER_TEST_EXPORT_SAMPLES
        render_to_file( &ts_setup, &layout_stats );
#endif
        data_stat_destroy( &layout_stats );
    }
    test_data_setup_destroy( &ts_setup );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t render_edge_cases( test_fixture_t *test_env )
{
    test_data_setup_t ts_setup;
    test_data_setup_init( &ts_setup, TEST_DATA_SETUP_MODE_EDGE_CASES );
    for ( ; test_data_setup_is_valid_variant( &ts_setup ); test_data_setup_next_variant( &ts_setup ) )
    {
        /* setup */
        test_data_setup_get_variant_data( &ts_setup, &data_set );
        draw_background();

        /* perform test: draw diagram */
        data_id_t void_id;
        data_id_init_void( &void_id );
        data_small_set_t void_set;
        data_small_set_init( &void_set );
        data_stat_t layout_stats;
        data_stat_init( &layout_stats );
        pencil_diagram_maker_define_grid( &painter, diagram_bounds );
        pencil_diagram_maker_layout_elements( &painter, NULL, cr );
        pencil_diagram_maker_show_overlaps( &painter, &layout_stats, cr );
        pencil_diagram_maker_draw ( &painter,
                                    void_id,
                                    void_id,
                                    &void_set,
                                    cr
                                  );

        /* check result */
        /* TODO, manual check for now */
#ifdef PENCIL_DIAGRAM_MAKER_TEST_EXPORT_SAMPLES
        render_to_file( &ts_setup, &layout_stats );
#endif
        data_stat_destroy( &layout_stats );
    }
    test_data_setup_destroy( &ts_setup );
    return TEST_CASE_RESULT_OK;
}
#endif


/*
 * Copyright 2021-2023 Andreas Warnke
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
