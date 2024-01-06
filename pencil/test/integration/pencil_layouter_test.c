/* File: pencil_layouter_test.c; Copyright and License: see below */

#include "pencil_layouter_test.h"
#include "pencil_layouter.h"
#include "pencil_diagram_maker.h"
#include "test_data/test_data_setup.h"
#include "u8/u8_trace.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <inttypes.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t layout_good_cases( test_fixture_t *fix );
static test_case_result_t layout_challenging_cases( test_fixture_t *fix );
#ifndef NDEBUG
static test_case_result_t layout_edge_cases( test_fixture_t *fix );
#endif

/*
Note that the test results/statistics stronly depend on the installed fonts.
*/

test_suite_t pencil_layouter_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_layouter_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "layout_good_cases", &layout_good_cases );
    test_suite_add_test_case( &result, "layout_challenging_cases", &layout_challenging_cases );
#ifndef NDEBUG
    test_suite_add_test_case( &result, "layout_edge_cases", &layout_edge_cases );
#endif
    return result;
}

struct test_fixture_struct {
    data_visible_set_t data_set;
    data_profile_part_t profile;
    pencil_layouter_t layouter;
    cairo_surface_t *surface;
    cairo_t *cr;
    geometry_rectangle_t diagram_bounds;
    PangoLayout *font_layout;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    data_visible_set_init( &((*fix).data_set) );
    data_profile_part_init( &((*fix).profile) );
    pencil_layouter_init( &((*fix).layouter), &((*fix).data_set), &((*fix).profile) );
    geometry_rectangle_init( &((*fix).diagram_bounds), 0.0, 0.0, 640.0, 480.0 );
    (*fix).surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32,
                                                 (uint32_t) geometry_rectangle_get_width( &((*fix).diagram_bounds) ),
                                                 (uint32_t) geometry_rectangle_get_height( &((*fix).diagram_bounds) )
                                               );
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( (*fix).surface ) );
    (*fix).cr = cairo_create( (*fix).surface );
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( (*fix).cr ) );
    (*fix).font_layout = pango_cairo_create_layout( (*fix).cr );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    g_object_unref( (*fix).font_layout );
    cairo_destroy( (*fix).cr );
    cairo_surface_finish( (*fix).surface );
    cairo_surface_destroy( (*fix).surface );
    geometry_rectangle_destroy( &((*fix).diagram_bounds) );
    pencil_layouter_destroy( &((*fix).layouter) );
    data_profile_part_destroy( &((*fix).profile) );
    data_visible_set_destroy( &((*fix).data_set) );
}

static test_case_result_t layout_good_cases( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_stat_t total_stats;
    data_stat_init( &total_stats );

    test_data_setup_t ts_setup;
    test_data_setup_init( &ts_setup, TEST_DATA_SETUP_MODE_GOOD_CASES );
    for ( ; test_data_setup_is_valid_variant( &ts_setup ); test_data_setup_next_variant( &ts_setup ) )
    {
        /* setup */
        test_data_setup_get_variant_data( &ts_setup, &((*fix).data_set) );
        data_stat_t layout_stats;
        data_stat_init( &layout_stats );

        /* perform test */
        pencil_layouter_prepare ( &((*fix).layouter) );
        pencil_layouter_define_grid ( &((*fix).layouter), (*fix).diagram_bounds, (*fix).font_layout );
        pencil_layouter_layout_elements ( &((*fix).layouter), (*fix).font_layout );
        pencil_layout_data_get_statistics( pencil_layouter_get_layout_data_const( &((*fix).layouter) ), &layout_stats );

        /* check result */
        const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const( &((*fix).layouter) );
        TEST_EXPECT( NULL != layout_data );
        /*
        const uint32_t class_cnt = pencil_layout_data_get_visible_classifier_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, class_cnt );
        const uint32_t feat_cnt = pencil_layout_data_get_feature_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, feat_cnt );
        const uint32_t rel_cnt = pencil_layout_data_get_relationship_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, rel_cnt );
        */

#ifndef NDEBUG
        const unsigned int variant = test_data_setup_get_variant( &ts_setup );
        fprintf( stdout,
             "    variant=%i, #Diag=%" PRIuFAST32 ", total=%" PRIuFAST32 " |  ERR=%" PRIuFAST32
             ", W/C=%" PRIuFAST32 ", W/F=%" PRIuFAST32 ", W/L=%" PRIuFAST32 ", W/R=%" PRIuFAST32 "\n",
             variant,
             data_stat_get_count( &layout_stats, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED ),
             data_stat_get_series_count( &layout_stats, DATA_STAT_SERIES_EXPORTED ),
             data_stat_get_series_count( &layout_stats, DATA_STAT_SERIES_ERROR ),
             data_stat_get_count( &layout_stats, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &layout_stats, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &layout_stats, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &layout_stats, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING )
           );
#endif

        data_stat_add( &total_stats, &layout_stats );
        data_stat_destroy( &layout_stats );
    }
    test_data_setup_destroy( &ts_setup );

    fprintf( stdout,
             "    #Diag=%" PRIuFAST32 ", total=%" PRIuFAST32 " |  ERR=%" PRIuFAST32
             ", W/C=%" PRIuFAST32 ", W/F=%" PRIuFAST32 ", W/L=%" PRIuFAST32 ", W/R=%" PRIuFAST32 "\n",
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED ),
             data_stat_get_series_count( &total_stats, DATA_STAT_SERIES_EXPORTED ),
             data_stat_get_series_count( &total_stats, DATA_STAT_SERIES_ERROR ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING )
           );
    data_stat_destroy( &total_stats );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t layout_challenging_cases( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_stat_t total_stats;
    data_stat_init( &total_stats );

    test_data_setup_t ts_setup;
    test_data_setup_init( &ts_setup, TEST_DATA_SETUP_MODE_CHALLENGING_CASES );
    for ( ; test_data_setup_is_valid_variant( &ts_setup ); test_data_setup_next_variant( &ts_setup ) )
    {
        /* setup */
        test_data_setup_get_variant_data( &ts_setup, &((*fix).data_set) );
        data_stat_t layout_stats;
        data_stat_init( &layout_stats );

        /* perform test */
        pencil_layouter_prepare ( &((*fix).layouter) );
        pencil_layouter_define_grid ( &((*fix).layouter), (*fix).diagram_bounds, (*fix).font_layout );
        pencil_layouter_layout_elements ( &((*fix).layouter), (*fix).font_layout );
        pencil_layout_data_get_statistics( pencil_layouter_get_layout_data_const( &((*fix).layouter) ), &layout_stats );

        /* check result */
        const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const( &((*fix).layouter) );
        TEST_EXPECT( NULL != layout_data );
        /*
        const uint32_t class_cnt = pencil_layout_data_get_visible_classifier_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, class_cnt );
        const uint32_t feat_cnt = pencil_layout_data_get_feature_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, feat_cnt );
        const uint32_t rel_cnt = pencil_layout_data_get_relationship_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, rel_cnt );
        */
        data_stat_add( &total_stats, &layout_stats );
        data_stat_destroy( &layout_stats );
    }
    test_data_setup_destroy( &ts_setup );
    fprintf( stdout,
             "    #Diag=%" PRIuFAST32 ", total=%" PRIuFAST32 " |  ERR=%" PRIuFAST32
             ", W/C=%" PRIuFAST32 ", W/F=%" PRIuFAST32 ", W/L=%" PRIuFAST32 ", W/R=%" PRIuFAST32 "\n",
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED ),
             data_stat_get_series_count( &total_stats, DATA_STAT_SERIES_EXPORTED ),
             data_stat_get_series_count( &total_stats, DATA_STAT_SERIES_ERROR ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_WARNING ),
             data_stat_get_count( &total_stats, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING )
           );
    data_stat_destroy( &total_stats );
    return TEST_CASE_RESULT_OK;
}

#ifndef NDEBUG
static test_case_result_t layout_edge_cases( test_fixture_t *fix )
{
    assert( fix != NULL );
    test_data_setup_t ts_setup;
    test_data_setup_init( &ts_setup, TEST_DATA_SETUP_MODE_EDGE_CASES );
    for ( ; test_data_setup_is_valid_variant( &ts_setup ); test_data_setup_next_variant( &ts_setup ) )
    {
        /* setup */
        test_data_setup_get_variant_data( &ts_setup, &((*fix).data_set) );

        /* perform test */
        pencil_layouter_prepare ( &((*fix).layouter) );
        pencil_layouter_define_grid ( &((*fix).layouter), (*fix).diagram_bounds, (*fix).font_layout );
        pencil_layouter_layout_elements ( &((*fix).layouter), (*fix).font_layout );

        /* check result */
        const pencil_layout_data_t *const layout_data = pencil_layouter_get_layout_data_const( &((*fix).layouter) );
        TEST_EXPECT( NULL != layout_data );
        /*
        const uint32_t class_cnt = pencil_layout_data_get_visible_classifier_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, class_cnt );
        const uint32_t feat_cnt = pencil_layout_data_get_feature_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, feat_cnt );
        const uint32_t rel_cnt = pencil_layout_data_get_relationship_count( layout_data );
        TEST_EXPECT_EQUAL_INT( 0, rel_cnt );
        */
    }
    test_data_setup_destroy( &ts_setup );
    return TEST_CASE_RESULT_OK;
}
#endif


/*
 * Copyright 2021-2024 Andreas Warnke
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
