/* File: geometry_anchor_test.c; Copyright and License: see below */

#include "geometry_anchor_test.h"
#include "geometry/geometry_anchor.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_geometry_anchor_init( test_fixture_t *fix );
static test_case_result_t test_geometry_anchor_align( test_fixture_t *fix );
static test_case_result_t test_geometry_anchor_align_biased( test_fixture_t *fix );


test_suite_t geometry_anchor_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "geometry_anchor_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_geometry_anchor_init", &test_geometry_anchor_init );
    test_suite_add_test_case( &result, "test_geometry_anchor_align", &test_geometry_anchor_align );
    test_suite_add_test_case( &result, "test_geometry_anchor_align_biased", &test_geometry_anchor_align_biased );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_geometry_anchor_init( test_fixture_t *fix )
{
    geometry_anchor_t my_anchor;
    geometry_anchor_init( &my_anchor, 20.0, 1.0, GEOMETRY_H_ALIGN_LEFT, GEOMETRY_V_ALIGN_BOTTOM );

    geometry_anchor_t my_anchor_clone;
    geometry_anchor_copy( &my_anchor_clone, &my_anchor );

    geometry_anchor_replace( &my_anchor, &my_anchor_clone );

    TEST_EXPECT_EQUAL_FLOAT( 20.0, geometry_anchor_get_x( &my_anchor ) );
    TEST_EXPECT_EQUAL_FLOAT( 1.0, geometry_anchor_get_y( &my_anchor ) );

    /* check that the trace function returns */
    geometry_anchor_trace( &my_anchor );

    geometry_anchor_destroy( &my_anchor );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_anchor_align( test_fixture_t *fix )
{
    geometry_anchor_t my_anchor;
    geometry_anchor_init( &my_anchor, 20.0, 1.0, GEOMETRY_H_ALIGN_LEFT, GEOMETRY_V_ALIGN_BOTTOM );

    geometry_dimensions_t my_dim1;
    geometry_dimensions_init( &my_dim1, 10.0, 0.2 );
    geometry_rectangle_t rect1 = geometry_anchor_align_dim( &my_anchor, &my_dim1 );
    TEST_EXPECT_EQUAL_FLOAT( 25.0, geometry_rectangle_get_center_x( &rect1 ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.9, geometry_rectangle_get_center_y( &rect1 ) );
    geometry_rectangle_destroy( &rect1 );
    geometry_dimensions_destroy( &my_dim1 );

    geometry_rectangle_t my_rect;
    geometry_rectangle_init( &my_rect, -1.0, -2.0, 10.0, 0.2 );
    geometry_rectangle_t rect3 = geometry_anchor_align_rect( &my_anchor, &my_rect );
    TEST_EXPECT_EQUAL_FLOAT( 25.0, geometry_rectangle_get_center_x( &rect3 ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.9, geometry_rectangle_get_center_y( &rect3 ) );
    geometry_rectangle_destroy( &rect3 );

    geometry_dimensions_t my_dim2;
    geometry_dimensions_init( &my_dim2, 10.0, 0.2 );
    geometry_rectangle_t my_bounds;
    geometry_rectangle_init( &my_bounds, 19.0, 2.0, 100.0, 0.1 );
    geometry_rectangle_t rect2 = geometry_anchor_align_biased_dim( &my_anchor, &my_dim2, &my_bounds );
    TEST_EXPECT_EQUAL_FLOAT( 25.0, geometry_rectangle_get_center_x( &rect2 ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.9, geometry_rectangle_get_center_y( &rect2 ) );
    geometry_rectangle_destroy( &rect2 );
    geometry_rectangle_destroy( &my_bounds );
    geometry_dimensions_destroy( &my_dim2 );

    geometry_anchor_destroy( &my_anchor );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_anchor_align_biased( test_fixture_t *fix )
{
    geometry_anchor_t my_anchor;
    geometry_anchor_init( &my_anchor, 100.0, 10.0, GEOMETRY_H_ALIGN_CENTER, GEOMETRY_V_ALIGN_CENTER );

    geometry_dimensions_t my_dim2;
    geometry_dimensions_init( &my_dim2, 20.0, 2.0 );

    /* my_dim2 fits into my_bounds */
    geometry_rectangle_t my_bounds;
    geometry_rectangle_init( &my_bounds, 70.0, 2.0, 200.0, 80.0 );
    geometry_rectangle_t rect1 = geometry_anchor_align_biased_dim( &my_anchor, &my_dim2, &my_bounds );
    TEST_EXPECT_EQUAL_FLOAT( 100.0, geometry_rectangle_get_center_x( &rect1 ) );
    TEST_EXPECT_EQUAL_FLOAT( 10.0, geometry_rectangle_get_center_y( &rect1 ) );
    geometry_rectangle_destroy( &rect1 );
    geometry_rectangle_destroy( &my_bounds );

    /* my_dim2 fits into my_bounds if not centered */
    geometry_rectangle_init( &my_bounds, 95.0, 9.5, 200.0, 80.0 );
    geometry_rectangle_t rect2 = geometry_anchor_align_biased_dim( &my_anchor, &my_dim2, &my_bounds );
    TEST_EXPECT_EQUAL_FLOAT( 105.0, geometry_rectangle_get_center_x( &rect2 ) );
    TEST_EXPECT_EQUAL_FLOAT( 10.5, geometry_rectangle_get_center_y( &rect2 ) );
    geometry_rectangle_destroy( &rect2 );
    geometry_rectangle_destroy( &my_bounds );

    /* my_dim2 is outsiude my_bounds */
    geometry_rectangle_init( &my_bounds, 150.0, 15.0, 200.0, 80.0 );
    geometry_rectangle_t rect3 = geometry_anchor_align_biased_dim( &my_anchor, &my_dim2, &my_bounds );
    TEST_EXPECT_EQUAL_FLOAT( 110.0, geometry_rectangle_get_center_x( &rect3 ) );
    TEST_EXPECT_EQUAL_FLOAT( 11.0, geometry_rectangle_get_center_y( &rect3 ) );
    geometry_rectangle_destroy( &rect3 );
    geometry_rectangle_destroy( &my_bounds );

    geometry_dimensions_destroy( &my_dim2 );

    geometry_anchor_destroy( &my_anchor );
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
