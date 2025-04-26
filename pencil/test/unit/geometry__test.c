/* File: geometry__test.c; Copyright and License: see below */

#include "geometry__test.h"
#include "geometry/geometry_3dir.h"
#include "geometry/geometry_dimensions.h"
#include "geometry/geometry_grid.h"
#include "geometry/geometry_offset.h"
#include "geometry/geometry_point.h"
#include "geometry/geometry_h_align.h"
#include "geometry/geometry_v_align.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_geometry_3dir( test_fixture_t *fix );
static test_case_result_t test_geometry_dimensions( test_fixture_t *fix );
static test_case_result_t test_geometry_grid( test_fixture_t *fix );
static test_case_result_t test_geometry_offset( test_fixture_t *fix );
static test_case_result_t test_geometry_point( test_fixture_t *fix );
static test_case_result_t test_geometry_h_align( test_fixture_t *fix );
static test_case_result_t test_geometry_v_align( test_fixture_t *fix );

test_suite_t geometry__test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "geometry__test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_geometry_3dir", &test_geometry_3dir );
    test_suite_add_test_case( &result, "test_geometry_dimensions", &test_geometry_dimensions );
    test_suite_add_test_case( &result, "test_geometry_grid", &test_geometry_grid );
    test_suite_add_test_case( &result, "test_geometry_offset", &test_geometry_offset );
    test_suite_add_test_case( &result, "test_geometry_point", &test_geometry_point );
    test_suite_add_test_case( &result, "test_geometry_h_align", &test_geometry_h_align );
    test_suite_add_test_case( &result, "test_geometry_v_align", &test_geometry_v_align );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_geometry_3dir( test_fixture_t *fix )
{
    const geometry_3dir_t my_3dir
        = geometry_3dir_new( GEOMETRY_DIRECTION_CENTER, GEOMETRY_DIRECTION_LEFT, GEOMETRY_DIRECTION_DOWN_LEFT );

    TEST_EXPECT_EQUAL_INT( true, geometry_3dir_equals( &my_3dir, &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_DIRECTION_CENTER, geometry_3dir_get_first( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_DIRECTION_LEFT, geometry_3dir_get_second( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_DIRECTION_DOWN_LEFT, geometry_3dir_get_third( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( false, geometry_3dir_is_first_h( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( false, geometry_3dir_is_first_v( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( true, geometry_3dir_is_second_h( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( false, geometry_3dir_is_second_v( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( false, geometry_3dir_is_third_h( &my_3dir ) );
    TEST_EXPECT_EQUAL_INT( false, geometry_3dir_is_third_v( &my_3dir ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_dimensions( test_fixture_t *fix )
{
    geometry_dimensions_t my_dim1;
    geometry_dimensions_init( &my_dim1, 10.0, 5.0 );

    geometry_dimensions_t my_dim2;
    geometry_dimensions_copy( &my_dim2, &my_dim1 );

    geometry_dimensions_t my_dim3;
    geometry_dimensions_init_empty( &my_dim3 );

    geometry_dimensions_replace( &my_dim3, &my_dim2 );

    geometry_dimensions_reinit_empty( &my_dim1 );

    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_dimensions_get_width( &my_dim1 ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_dimensions_get_height( &my_dim1 ) );
    TEST_EXPECT_EQUAL_FLOAT( 50.0, geometry_dimensions_get_area( &my_dim2 ) );
    TEST_EXPECT_EQUAL_INT( true, geometry_dimensions_is_empty( &my_dim1 ) );
    TEST_EXPECT_EQUAL_INT( false, geometry_dimensions_is_empty( &my_dim3 ) );
    TEST_EXPECT_EQUAL_INT( true, geometry_dimensions_can_contain( &my_dim3, &my_dim1 ) );

    geometry_dimensions_expand( &my_dim3, 0.1, 0.1 );

    TEST_EXPECT_EQUAL_INT( false, geometry_dimensions_can_contain( &my_dim1, &my_dim3 ) );

    /* check that the trace function returns */
    geometry_dimensions_trace( &my_dim3 );

    geometry_dimensions_destroy( &my_dim1 );
    geometry_dimensions_destroy( &my_dim2 );
    geometry_dimensions_destroy( &my_dim3 );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_grid( test_fixture_t *fix )
{
    geometry_grid_t my_grid;
    geometry_grid_init( &my_grid, GEOMETRY_GRID_KIND_0 );
    geometry_grid_reinit( &my_grid, GEOMETRY_GRID_KIND_XY );

    TEST_EXPECT_EQUAL_INT( GEOMETRY_GRID_KIND_XY, geometry_grid_get_kind( &my_grid ) );

    geometry_non_linear_scale_t *x_scale1 = geometry_grid_get_x_scale_ptr( &my_grid );
    const geometry_non_linear_scale_t *x_scale2 = geometry_grid_get_x_scale_const( &my_grid );
    TEST_EXPECT_EQUAL_PTR( x_scale2, x_scale1 );

    geometry_non_linear_scale_t *y_scale1 = geometry_grid_get_y_scale_ptr( &my_grid );
    const geometry_non_linear_scale_t *y_scale2 = geometry_grid_get_y_scale_const( &my_grid );
    TEST_EXPECT_EQUAL_PTR( y_scale2, y_scale1 );

    /* check that the trace function returns */
    geometry_grid_trace( &my_grid );

    geometry_grid_destroy ( &my_grid );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_offset( test_fixture_t *fix )
{
    geometry_offset_t my_offset1;
    geometry_offset_init( &my_offset1, 4.0, 17.0 );

    const geometry_offset_t my_offset2 = geometry_offset_new( 3.0, 13.0 );

    geometry_offset_t my_offset3;
    geometry_offset_copy( &my_offset3, &my_offset2 );

    geometry_offset_replace( &my_offset1, &my_offset3 );

    TEST_EXPECT_EQUAL_FLOAT( 3.0, geometry_offset_get_dx( &my_offset1 ) );
    TEST_EXPECT_EQUAL_FLOAT( 13.0, geometry_offset_get_dy( &my_offset1 ) );

    TEST_EXPECT_EQUAL_INT( true, geometry_offset_equals( &my_offset1, &my_offset3 ) );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_DIRECTION_DOWN_RIGHT, geometry_offset_get_direction( &my_offset1 ) );

    /* check that the trace function returns */
    geometry_offset_trace( &my_offset1 );

    geometry_offset_destroy ( &my_offset1 );
    geometry_offset_destroy ( &my_offset3 );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_point( test_fixture_t *fix )
{
    geometry_point_t my_point1;
    geometry_point_init( &my_point1, 4.0, 17.0 );

    const geometry_point_t my_point2 = geometry_point_new( 3.0, 13.0 );

    geometry_point_t my_point3;
    geometry_point_copy( &my_point3, &my_point2 );

    TEST_EXPECT_EQUAL_FLOAT( 5.0, geometry_point_calc_chess_distance( &my_point1, &my_point2 ) );

    geometry_point_replace( &my_point1, &my_point3 );

    TEST_EXPECT_EQUAL_FLOAT( 3.0, geometry_point_get_x( &my_point1 ) );
    TEST_EXPECT_EQUAL_FLOAT( 13.0, geometry_point_get_y( &my_point1 ) );

    TEST_EXPECT_EQUAL_INT( true, geometry_point_equals( &my_point1, &my_point3 ) );

    /* check that the trace function returns */
    geometry_point_trace( &my_point1 );

    geometry_point_destroy ( &my_point1 );
    geometry_point_destroy ( &my_point3 );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_h_align( test_fixture_t *fix )
{
    const geometry_h_align_t align_left = GEOMETRY_H_ALIGN_LEFT;
    const double left
        = geometry_h_align_get_left( &align_left,
                                     2.0 /* width */,
                                     100.0 /* reference_left */,
                                     10.0 /* reference_width */
                                   );
    TEST_EXPECT_EQUAL_FLOAT( 100.0, left );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_v_align( test_fixture_t *fix )
{
  const geometry_v_align_t align_top = GEOMETRY_V_ALIGN_TOP;
  const double top
        = geometry_v_align_get_top( &align_top,
                                    2.0 /*height*/,
                                    100.0 /* reference_top */,
                                    10.0 /* reference_height */
                                  );
    TEST_EXPECT_EQUAL_FLOAT( 100.0, top );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2025-2025 Andreas Warnke
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
