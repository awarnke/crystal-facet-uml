/* File: geometry_non_linear_scale_test.c; Copyright and License: see below */

#include "geometry_non_linear_scale_test.h"
#include "util/geometry/geometry_non_linear_scale.h"

static void set_up(void);
static void tear_down(void);
static void test_scale_conversion(void);

TestRef geometry_non_linear_scale_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_scale_conversion",test_scale_conversion),
    };
    EMB_UNIT_TESTCALLER(result,"geometry_non_linear_scale_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_scale_conversion(void)
{
    geometry_non_linear_scale_t my_scale;
    double location;
    int32_t order;

    /* init and trace */

    geometry_non_linear_scale_init ( &my_scale, -3.14, 3.14 );

    geometry_non_linear_scale_add_order( &my_scale, -100 );

    geometry_non_linear_scale_add_order( &my_scale, 200 );

    geometry_non_linear_scale_add_order( &my_scale, 200 );

    geometry_non_linear_scale_trace ( &my_scale );

    /* test get_location */

    location = geometry_non_linear_scale_get_location( &my_scale, +200 );

    TEST_ASSERT( ( +1.0 < location ) && ( location < +1.1 ) );

    location = geometry_non_linear_scale_get_location( &my_scale, +100 );

    TEST_ASSERT( ( +0.3 < location ) && ( location < +0.4 ) );

    /* test get_order */

    order = geometry_non_linear_scale_get_order ( &my_scale, -3.1, 2.0 );

    TEST_ASSERT( order < -1000000000 );

    order = geometry_non_linear_scale_get_order ( &my_scale, -1.0, 2.0 );

    TEST_ASSERT( ( -100 < order ) && ( order < -80 ) );

    geometry_non_linear_scale_destroy ( &my_scale );
}


/*
 * Copyright 2016-2016 Andreas Warnke
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
