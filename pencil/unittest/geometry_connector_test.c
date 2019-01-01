/* File: geometry_connector_test.c; Copyright and License: see below */

#include "geometry_connector_test.h"
#include "util/geometry/geometry_connector.h"
#include "util/geometry/geometry_rectangle.h"

static void set_up(void);
static void tear_down(void);
static void test_base_methods(void);
static void test_rectangle_related_methods(void);
static void test_connector_intersects(void);

TestRef geometry_connector_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_base_methods",test_base_methods),
        new_TestFixture("test_rectangle_related_methods",test_rectangle_related_methods),
        new_TestFixture("test_connector_intersects",test_connector_intersects),
    };
    EMB_UNIT_TESTCALLER(result,"geometry_connector_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_base_methods(void)
{
    geometry_connector_t my_connector;
    geometry_connector_init_empty( &my_connector );

    /* init 3 segments line */
    geometry_connector_reinit_vertical ( &my_connector,
                                         10.0 /*source_end_x*/,
                                         10.0 /*source_end_y*/,
                                         30.0 /*destination_end_x*/,
                                         30.0 /*destination_end_y*/,
                                         20.0 /*main_line_x*/
                                       );
    TEST_ASSERT( 40.0001 > geometry_connector_get_length( &my_connector ) );
    TEST_ASSERT( 39.9999 < geometry_connector_get_length( &my_connector ) );
    TEST_ASSERT_EQUAL_INT( true, geometry_connector_is_close( &my_connector, 21.0, 9.0, 1.5 /* max_distance */ ) );
    TEST_ASSERT_EQUAL_INT( false, geometry_connector_is_close( &my_connector, 21.0, 9.0, 0.5 /* max_distance */ ) );

    /* init 1 segment line */
    geometry_connector_reinit_vertical ( &my_connector,
                                         10.0 /*source_end_x*/,
                                         10.0 /*source_end_y*/,
                                         10.0 /*destination_end_x*/,
                                         30.0 /*destination_end_y*/,
                                         10.0 /*main_line_x*/
                                       );
    TEST_ASSERT( 20.0001 > geometry_connector_get_length( &my_connector ) );
    TEST_ASSERT( 19.9999 < geometry_connector_get_length( &my_connector ) );
    TEST_ASSERT_EQUAL_INT( true, geometry_connector_is_close( &my_connector, 9.0, 20.0, 1.5 /* max_distance */ ) );
    TEST_ASSERT_EQUAL_INT( false, geometry_connector_is_close( &my_connector, 10.0, 8.0, 1.5 /* max_distance */ ) );

    geometry_connector_destroy ( &my_connector );
}

static void test_rectangle_related_methods(void)
{
    geometry_connector_t my_connector;
    geometry_rectangle_t overlap;
    geometry_rectangle_t bounds;
    bool intersects;

    /* init 3 segments line, Z-Form */
    geometry_connector_init_vertical ( &my_connector,
                                       10.0 /*source_end_x*/,
                                       10.0 /*source_end_y*/,
                                       30.0 /*destination_end_x*/,
                                       30.0 /*destination_end_y*/,
                                       20.0 /*main_line_x*/
                                     );
    geometry_rectangle_init( &overlap, 19.0, 19.0, 5.0 /*W*/, 5.0 /*H*/ );
    intersects = geometry_connector_is_intersecting_rectangle( &my_connector, &overlap );
    TEST_ASSERT_EQUAL_INT( true, intersects );
    bounds = geometry_connector_get_bounding_rectangle ( &my_connector );
    TEST_ASSERT( 400.0001 > geometry_rectangle_get_area( &bounds ) );
    TEST_ASSERT( 399.9999 < geometry_rectangle_get_area( &bounds ) );

    geometry_rectangle_destroy ( &bounds );
    geometry_rectangle_destroy ( &overlap );

    /* init 3 segments line, U-Form */
    geometry_connector_reinit_horizontal ( &my_connector,
                                           10.0 /*source_end_x*/,
                                           30.0 /*source_end_y*/,
                                           30.0 /*destination_end_x*/,
                                           30.0 /*destination_end_y*/,
                                           10.0 /*main_line_y*/
                                         );
    geometry_rectangle_init( &overlap, 11.0, 11.0, 18.0 /*W*/, 25.0 /*H*/ );
    intersects = geometry_connector_is_intersecting_rectangle( &my_connector, &overlap );
    TEST_ASSERT_EQUAL_INT( false, intersects );
    bounds = geometry_connector_get_bounding_rectangle ( &my_connector );
    TEST_ASSERT( 400.0001 > geometry_rectangle_get_area( &bounds ) );
    TEST_ASSERT( 399.9999 < geometry_rectangle_get_area( &bounds ) );

    geometry_rectangle_destroy ( &bounds );
    geometry_rectangle_destroy ( &overlap );

    /* init 1 segment line */
    geometry_connector_reinit_vertical ( &my_connector,
                                         10.0 /*source_end_x*/,
                                         10.0 /*source_end_y*/,
                                         10.0 /*destination_end_x*/,
                                         30.0 /*destination_end_y*/,
                                         10.0 /*main_line_x*/
                                       );
    geometry_rectangle_init( &overlap, 9.0, 11.0, 10.0 /*W*/, 10.0 /*H*/ );
    intersects = geometry_connector_is_intersecting_rectangle( &my_connector, &overlap );
    TEST_ASSERT_EQUAL_INT( true, intersects );
    bounds = geometry_connector_get_bounding_rectangle ( &my_connector );
    TEST_ASSERT( 0.0001 > geometry_rectangle_get_area( &bounds ) );
    TEST_ASSERT( -0.0001 < geometry_rectangle_get_area( &bounds ) );

    geometry_rectangle_destroy ( &overlap );
    geometry_rectangle_destroy ( &bounds );
    geometry_connector_destroy ( &my_connector );
}

static void test_connector_intersects(void)
{
    geometry_connector_t my_connector_1;
    geometry_connector_t my_connector_2;
    uint32_t intersect_count;

    /* double empty */
    geometry_connector_init_empty( &my_connector_1 );
    geometry_connector_init_empty( &my_connector_2 );

    intersect_count = geometry_connector_count_connector_intersects ( &my_connector_1, &my_connector_2 );
    TEST_ASSERT_EQUAL_INT( 9, intersect_count );

    geometry_connector_destroy ( &my_connector_1 );
    geometry_connector_destroy ( &my_connector_2 );

    /* completely separated */
    geometry_connector_init_vertical ( &my_connector_1,
                                       10.0 /*source_end_x*/,
                                       10.0 /*source_end_y*/,
                                       10.0 /*destination_end_x*/,
                                       30.0 /*destination_end_y*/,
                                       20.0 /*main_line_x*/
                                     );
    geometry_connector_init_vertical ( &my_connector_2,
                                       40.0 /*source_end_x*/,
                                       10.0 /*source_end_y*/,
                                       40.0 /*destination_end_x*/,
                                       30.0 /*destination_end_y*/,
                                       50.0 /*main_line_x*/
                                     );

    intersect_count = geometry_connector_count_connector_intersects ( &my_connector_1, &my_connector_2 );
    TEST_ASSERT_EQUAL_INT( 0, intersect_count );

    geometry_connector_destroy ( &my_connector_1 );
    geometry_connector_destroy ( &my_connector_2 );

    /* close but not overlapping */
    geometry_connector_init_horizontal ( &my_connector_1,
                                         10.0 /*source_end_x*/,
                                         30.0 /*source_end_y*/,
                                         30.0 /*destination_end_x*/,
                                         30.0 /*destination_end_y*/,
                                         10.0 /*main_line_y*/
                                       );
    geometry_connector_init_horizontal ( &my_connector_2,
                                         15.0 /*source_end_x*/,
                                         30.0 /*source_end_y*/,
                                         25.0 /*destination_end_x*/,
                                         30.0 /*destination_end_y*/,
                                         11.0 /*main_line_y*/
                                       );

    intersect_count = geometry_connector_count_connector_intersects ( &my_connector_1, &my_connector_2 );
    TEST_ASSERT_EQUAL_INT( 0, intersect_count );

    geometry_connector_destroy ( &my_connector_1 );
    geometry_connector_destroy ( &my_connector_2 );

    /* 2x touching but not overlapping */
    geometry_connector_init_horizontal ( &my_connector_1,
                                         10.0 /*source_end_x*/,
                                         10.0 /*source_end_y*/,
                                         30.0 /*destination_end_x*/,
                                         10.0 /*destination_end_y*/,
                                         20.0 /*main_line_y*/
                                       );
    geometry_connector_init_horizontal ( &my_connector_2,
                                         15.0 /*source_end_x*/,
                                         20.0 /*source_end_y*/,
                                         25.0 /*destination_end_x*/,
                                         20.0 /*destination_end_y*/,
                                         10.0 /*main_line_y*/
                                       );

    intersect_count = geometry_connector_count_connector_intersects ( &my_connector_1, &my_connector_2 );
    TEST_ASSERT_EQUAL_INT( 2, intersect_count );

    geometry_connector_destroy ( &my_connector_1 );
    geometry_connector_destroy ( &my_connector_2 );

    /* 2 segments identical, 1 empty */
    geometry_connector_init_horizontal ( &my_connector_1,
                                         10.0 /*source_end_x*/,
                                         10.0 /*source_end_y*/,
                                         30.0 /*destination_end_x*/,
                                         20.0 /*destination_end_y*/,
                                         20.0 /*main_line_y*/
                                       );
    geometry_connector_replace ( &my_connector_2, &my_connector_1 );

    intersect_count = geometry_connector_count_connector_intersects ( &my_connector_1, &my_connector_2 );
    TEST_ASSERT_EQUAL_INT( 7, intersect_count );

    geometry_connector_destroy ( &my_connector_1 );
    geometry_connector_destroy ( &my_connector_2 );

    /* three real intersects */
    geometry_connector_init_horizontal ( &my_connector_1,
                                         10.0 /*source_end_x*/,
                                         10.0 /*source_end_y*/,
                                         30.0 /*destination_end_x*/,
                                         12.0 /*destination_end_y*/,
                                         20.0 /*main_line_y*/
                                       );
    geometry_connector_init_vertical ( &my_connector_2,
                                       5.0 /*source_end_x*/,
                                       11.0 /*source_end_y*/,
                                       5.0 /*destination_end_x*/,
                                       19.0 /*destination_end_y*/,
                                       40.0 /*main_line_x*/
                                     );

    intersect_count = geometry_connector_count_connector_intersects ( &my_connector_1, &my_connector_2 );
    TEST_ASSERT_EQUAL_INT( 3, intersect_count );

    geometry_connector_destroy ( &my_connector_1 );
    geometry_connector_destroy ( &my_connector_2 );
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
