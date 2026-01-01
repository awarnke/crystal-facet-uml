/* File: geometry_non_linear_scale_test.c; Copyright and License: see below */

#include "geometry_non_linear_scale_test.h"
#include "geometry/geometry_non_linear_scale.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_init( test_fixture_t *fix );
static test_case_result_t test_get_grid( test_fixture_t *fix );
static test_case_result_t test_scale_conversion( test_fixture_t *fix );
static test_case_result_t test_buffer_exceeded( test_fixture_t *fix );

test_suite_t geometry_non_linear_scale_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "geometry_non_linear_scale_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_init", &test_init );
    test_suite_add_test_case( &result, "test_get_grid", &test_get_grid );
    test_suite_add_test_case( &result, "test_scale_conversion", &test_scale_conversion );
    test_suite_add_test_case( &result, "test_buffer_exceeded", &test_buffer_exceeded );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_init( test_fixture_t *fix )
{
    geometry_non_linear_scale_t my_scale;

    geometry_non_linear_scale_init ( &my_scale, 0.0, 1000.0 );
    const double one_quarter = geometry_non_linear_scale_get_location( &my_scale, INT32_MIN/2 );
    TEST_EXPECT_EQUAL_FLOAT( 250.0, one_quarter );

    geometry_non_linear_scale_reinit ( &my_scale, -250.0, 0.0 );
    const double low_side = geometry_non_linear_scale_get_location( &my_scale, INT32_MIN );
    TEST_EXPECT_EQUAL_FLOAT( -250.0, low_side );

    geometry_non_linear_scale_destroy ( &my_scale );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_get_grid( test_fixture_t *fix )
{
    geometry_non_linear_scale_t my_scale;

    geometry_non_linear_scale_init ( &my_scale, -250.0, 0.0 );
    const u8_error_t err = geometry_non_linear_scale_add_order( &my_scale, 512 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    const double one_quarter = geometry_non_linear_scale_get_location( &my_scale, INT32_MIN );
    TEST_EXPECT_EQUAL_FLOAT( -250.0, one_quarter );

    const uint32_t num = geometry_non_linear_scale_get_grid_intervals( &my_scale );
    TEST_EXPECT_EQUAL_INT( 2, num );

    const double dist = geometry_non_linear_scale_get_grid_distances( &my_scale );
    TEST_EXPECT_EQUAL_FLOAT( 125.0, dist );

    const bool on_grid_500 = geometry_non_linear_scale_is_order_on_grid( &my_scale, 500 );
    TEST_EXPECT_EQUAL_INT( false, on_grid_500 );

    const bool on_grid_512 = geometry_non_linear_scale_is_order_on_grid( &my_scale, 512 );
    TEST_EXPECT_EQUAL_INT( true, on_grid_512 );

    const bool on_grid_min = geometry_non_linear_scale_is_order_on_grid( &my_scale, INT32_MIN );
    TEST_EXPECT_EQUAL_INT( false, on_grid_min );

    const bool on_grid_max = geometry_non_linear_scale_is_order_on_grid( &my_scale, INT32_MAX );
    TEST_EXPECT_EQUAL_INT( false, on_grid_max );

    geometry_non_linear_scale_destroy ( &my_scale );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_scale_conversion( test_fixture_t *fix )
{
    geometry_non_linear_scale_t my_scale;
    double location;
    int32_t order;

    /* init and trace */

    geometry_non_linear_scale_init ( &my_scale, -3.14, 3.14 );

    geometry_non_linear_scale_add_order( &my_scale, -100 );

    geometry_non_linear_scale_add_order( &my_scale, 200 );

    geometry_non_linear_scale_add_order( &my_scale, 200 );

    const uint32_t num = geometry_non_linear_scale_get_grid_intervals( &my_scale );
    TEST_EXPECT_EQUAL_INT( 3, num );

    geometry_non_linear_scale_trace ( &my_scale );

    /* test get_location */

    location = geometry_non_linear_scale_get_location( &my_scale, +200 );
    TEST_EXPECT( ( +1.0 < location ) && ( location < +1.1 ) );

    location = geometry_non_linear_scale_get_location( &my_scale, +100 );
    TEST_EXPECT( ( +0.3 < location ) && ( location < +0.4 ) );

    /* test get_order with snap */

    order = geometry_non_linear_scale_get_order ( &my_scale, -1.0, 0.1 );
    TEST_EXPECT_EQUAL_INT( -100, order );

    /* test get_order without snap */

    order = geometry_non_linear_scale_get_order ( &my_scale, -3.1, 0.1 );
    TEST_EXPECT( ( INT32_MIN < order ) && ( order < -1000000000 ) );

    order = geometry_non_linear_scale_get_order ( &my_scale, 3.1, 0.1 );
    TEST_EXPECT( ( 1000000000 < order ) && ( order < INT32_MAX ) );

    order = geometry_non_linear_scale_get_order ( &my_scale, -0.9, 0.1 );
    TEST_EXPECT( ( -100 < order ) && ( order < -70 ) );

    /* test get_order out of bounds */

    order = geometry_non_linear_scale_get_order ( &my_scale, -3.2, 0.1 );
    TEST_EXPECT_EQUAL_INT( INT32_MIN, order );

    order = geometry_non_linear_scale_get_order ( &my_scale, 3.2, 0.1 );
    TEST_EXPECT_EQUAL_INT( INT32_MAX, order );

    geometry_non_linear_scale_destroy ( &my_scale );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_buffer_exceeded( test_fixture_t *fix )
{
    geometry_non_linear_scale_t my_scale;
    geometry_non_linear_scale_init ( &my_scale, 0.0, 1000.0 * ( GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS - 1 ) );

    for ( int32_t step = 0; step < ( GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS - 2 ); step ++ )
    {
        const u8_error_t err0 = geometry_non_linear_scale_add_order( &my_scale, step + 1 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err0 );
    }
    const u8_error_t err1 = geometry_non_linear_scale_add_order( &my_scale, GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS - 1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, err1 );

    const bool on_grid = geometry_non_linear_scale_is_order_on_grid( &my_scale, GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS - 2 );
    TEST_EXPECT_EQUAL_INT( true, on_grid );
    const bool not_on_grid = geometry_non_linear_scale_is_order_on_grid( &my_scale, GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS - 1 );
    TEST_EXPECT_EQUAL_INT( false, not_on_grid );

    geometry_non_linear_scale_destroy ( &my_scale );
    return TEST_CASE_RESULT_OK;
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
