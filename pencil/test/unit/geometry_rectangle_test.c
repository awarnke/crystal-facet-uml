/* File: geometry_rectangle_test.c; Copyright and License: see below */

#include "geometry_rectangle_test.h"
#include "util/geometry/geometry_rectangle.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_contain(void);
static void test_intersect(void);
static void test_bounds(void);
static void test_difference_basic(void);
static void test_difference_4_candidates(void);
static void test_difference_3_candidates(void);
static void test_difference_2_corner_candidates(void);
static void test_difference_2_stripe_candidates(void);
static void test_difference_1_candidate(void);
static void test_expand_4d(void);

test_suite_t geometry_rectangle_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "geometry_rectangle_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_contain", &test_contain );
    test_suite_add_test_case( &result, "test_intersect", &test_intersect );
    test_suite_add_test_case( &result, "test_bounds", &test_bounds );
    test_suite_add_test_case( &result, "test_difference_basic", &test_difference_basic );
    test_suite_add_test_case( &result, "test_difference_4_candidates", &test_difference_4_candidates );
    test_suite_add_test_case( &result, "test_difference_3_candidates", &test_difference_3_candidates );
    test_suite_add_test_case( &result, "test_difference_2_corner_candidates", &test_difference_2_corner_candidates );
    test_suite_add_test_case( &result, "test_difference_2_stripe_candidates", &test_difference_2_stripe_candidates );
    test_suite_add_test_case( &result, "test_difference_1_candidate", &test_difference_1_candidate );
    test_suite_add_test_case( &result, "test_expand_4d", &test_expand_4d );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_contain(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    bool contains;

    /* no contain */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 11.0, 11.0, 11.0 /*width*/, 11.0 /*height*/ );

    contains = geometry_rectangle_is_containing( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( false, contains );

    contains = geometry_rectangle_is_containing( &rect_b, &rect_a );
    TEST_ASSERT_EQUAL_INT( false, contains );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );

    /* contain */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 10.0, 11.0, 9.0 /*width*/, 9.0 /*height*/ );

    contains = geometry_rectangle_is_containing( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( true, contains );

    contains = geometry_rectangle_is_containing( &rect_b, &rect_a );
    TEST_ASSERT_EQUAL_INT( false, contains );

    contains = geometry_rectangle_contains( &rect_a, 9.0, 9.0 );
    TEST_ASSERT_EQUAL_INT( false, contains );

    contains = geometry_rectangle_contains( &rect_a, 19.0, 19.0 );
    TEST_ASSERT_EQUAL_INT( true, contains );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
}

static void test_intersect(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t intersect_rect;
    int err;
    bool intersects;

    /* no intersect */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 10.0, 21.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( false, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* touch */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 20.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( false, intersects );

    intersects = geometry_rectangle_is_contiguous( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* part intersect */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 15.0, 15.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_DOUBLE( 25.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* full contained */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 20.0 /*width*/, 20.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 15.0, 15.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_DOUBLE( 100.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );
}

static void test_bounds(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t bounds_rect;
    int err;

    /* part intersect */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 18.0, 12.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_bounds( &bounds_rect, &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_left( &bounds_rect ) );
    TEST_ASSERT_EQUAL_DOUBLE( 28.0, geometry_rectangle_get_right( &bounds_rect ) );
    TEST_ASSERT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_top( &bounds_rect ) );
    TEST_ASSERT_EQUAL_DOUBLE( 22.0, geometry_rectangle_get_bottom( &bounds_rect ) );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &bounds_rect );
}

static void test_difference_basic(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /* no intersect */
    {
        geometry_rectangle_init ( &rect_a, 9.0, 10.0, 1.0 /*width*/, 90.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, 11.0, 20.0, 10.0 /*width*/, 10.0 /*height*/ );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_left( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_top( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 1.0, geometry_rectangle_get_width( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 90.0, geometry_rectangle_get_height( &diff_rect ) );

        /* clean up */
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }

    /* part intersect */
    {
        geometry_rectangle_init ( &rect_a, 9.0, 10.0, 1.0 /*width*/, 90.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, 8.0, 11.0, 1.5 /*width*/, 1.0 /*height*/ );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_left( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 12.0, geometry_rectangle_get_top( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 1.0, geometry_rectangle_get_width( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 88.0, geometry_rectangle_get_height( &diff_rect ) );

        /* clean up */
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }

    /* full contained */
    {
        geometry_rectangle_init ( &rect_a, 9.0, 10.0, 1.0 /*width*/, 90.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, 8.0, 9.0, 3.0 /*width*/, 92.0 /*height*/ );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_left( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_top( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_width( &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_height( &diff_rect ) );

        /* clean up */
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
}

static void test_difference_4_candidates(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /* no intersect */
    double in_x[4] = { 1.0, 2.0, 3.0, 2.0 };
    double in_y[4] = { 2.0, 3.0, 2.0, 1.0 };
    for ( int case_idx = 0; case_idx < 4; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 0.0, 0.0, 5.0 /*width*/, 5.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], 1.0 /*width*/, 1.0 /*height*/ );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        //geometry_rectangle_trace( &rect_a );
        //geometry_rectangle_trace( &rect_b );
        //geometry_rectangle_trace( &diff_rect );
        TEST_ASSERT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 15.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
}

static void test_difference_3_candidates(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /*                  from left      from bottom    from right     from top     */
    double in_x[12] = { 0.0, 0.0, 0.0, 2.0, 4.5, 7.0, 2.0, 7.0, 2.0, 2.0, 4.5, 7.0 };
    double in_y[12] = { 2.0, 4.5, 7.0, 2.0, 7.0, 2.0, 2.0, 4.5, 7.0, 0.0, 0.0, 0.0 };
    double in_w[12] = { 8.0, 3.0, 8.0, 1.0, 1.0, 1.0, 8.0, 3.0, 8.0, 1.0, 1.0, 1.0 };
    double in_h[12] = { 1.0, 1.0, 1.0, 8.0, 3.0, 8.0, 1.0, 1.0, 1.0, 8.0, 3.0, 8.0 };
    for ( int case_idx = 0; case_idx < 12; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 1.0, 1.0, 8.0 /*width*/, 8.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], in_w[case_idx], in_h[case_idx] );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 48.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
}

/*   0  1  2  3  4  5  6  7  8  9  10  */
/*  0  .  .  .  .  .  .  .  .  .  .    */
/*  1  .  .  .  .  .  .  .  .  .  .    */
/*  2  .  .  .  .  .  .  .  .  .  .    */
/*  3  .  .  .  _  _  _  _  .  .  .    */
/*  4  .  .  .  _  _  _  _  .  .  .    */
/*  5  .  .  .  _  _  _  _  .  .  .    */
/*  6  .  .  .  _  _  _  _  .  .  .    */
/*  7  .  .  .  .  .  .  .  .  .  .    */
/*  8  .  .  .  .  .  .  .  .  .  .    */
/*  9  .  .  .  .  .  .  .  .  .  .    */
/* 10                                  */

static void test_difference_2_corner_candidates(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /*                 le.to.    ri.to.    ri.bo.    le.bo.  */
    double in_x[8] = { 2.0, 1.0, 5.0, 6.0, 6.0, 5.0, 6.0, 1.0 };
    double in_y[8] = { 1.0, 2.0, 1.0, 2.0, 5.0, 6.0, 2.0, 5.0 };
    for ( int case_idx = 0; case_idx < 8; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 3.0, 3.0, 4.0 /*width*/, 4.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], 3.0, 3.0 );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 12.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
}

static void test_difference_2_stripe_candidates(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /*                 vertical  horizontal */
    double in_x[4] = { 4.0, 5.0, 1.0, 1.0 };
    double in_y[4] = { 1.0, 1.0, 4.0, 5.0 };
    double in_w[4] = { 1.0, 1.0, 8.0, 8.0 };
    double in_h[4] = { 8.0, 8.0, 1.0, 1.0 };
    for ( int case_idx = 0; case_idx < 4; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 3.0, 3.0, 4.0 /*width*/, 4.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], in_w[case_idx], in_h[case_idx] );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 8.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
}

static void test_difference_1_candidate(void)
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    double in_x[4] = { 1.0, 3.0, 5.0, 3.0 };
    double in_y[4] = { 3.0, 5.0, 3.0, 1.0 };
    for ( int case_idx = 0; case_idx < 4; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], 4.0 /*width*/, 4.0 /*height*/ );
        geometry_rectangle_init_by_difference( &diff_rect, &rect_a, &rect_b );
        TEST_ASSERT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_ASSERT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
}

static void test_expand_4d(void)
{
    geometry_rectangle_t rect_a;

    /* good case */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_expand_4d ( &rect_a, 1.0, 2.0 );
    TEST_ASSERT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_ASSERT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_ASSERT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_ASSERT_EQUAL_DOUBLE( 6.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );

    /* negative-size case */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_expand_4d ( &rect_a, -3.0, -4.0 );
    TEST_ASSERT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_ASSERT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );
}


/*
 * Copyright 2017-2021 Andreas Warnke
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
