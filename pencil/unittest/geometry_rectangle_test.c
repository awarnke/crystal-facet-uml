/* File: geometry_rectangle_test.c; Copyright and License: see below */

#include "geometry_rectangle_test.h"
#include "util/geometry/geometry_rectangle.h"

static void set_up(void);
static void tear_down(void);
static void test_contain(void);
static void test_intersect(void);

TestRef geometry_rectangle_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_contain",test_contain),
        new_TestFixture("test_intersect",test_intersect),
    };
    EMB_UNIT_TESTCALLER(result,"geometry_rectangle_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
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
    TEST_ASSERT( 0.0001 > geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( false, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* part intersect */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 15.0, 15.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT( 25.0001 > geometry_rectangle_get_area( &intersect_rect ) );
    TEST_ASSERT( 24.9999 < geometry_rectangle_get_area( &intersect_rect ) );

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
    TEST_ASSERT( 100.0001 > geometry_rectangle_get_area( &intersect_rect ) );
    TEST_ASSERT( 99.9999 < geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_ASSERT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );
}


/*
 * Copyright 2017-2019 Andreas Warnke
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
