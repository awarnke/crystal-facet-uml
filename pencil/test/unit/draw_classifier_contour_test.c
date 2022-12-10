/* File: draw_classifier_contour_test.c; Copyright and License: see below */

#include "draw_classifier_contour_test.h"
#include "draw/draw_classifier_contour.h"
#include "data_classifier_type.h"
#include "test_expect.h"

static void set_up(void);
static void tear_down(void);
static void test_calc_inner_area_and_back(void);
static void test_calc_inner_area_too_small(void);

test_suite_t draw_classifier_contour_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "draw_classifier_contour_test_get_suite", &set_up, &tear_down );

    test_suite_add_test_case( &result, "test_calc_inner_area_and_back", &test_calc_inner_area_and_back );
    test_suite_add_test_case( &result, "test_calc_inner_area_too_small", &test_calc_inner_area_too_small );
    return result;
}

static pencil_size_t pencil_size;

static void set_up(void)
{
    pencil_size_init( &pencil_size, 640.0, 480.0 );
}

static void tear_down(void)
{
    pencil_size_destroy( &pencil_size );
}

static void test_calc_inner_area_and_back(void)
{
    draw_classifier_contour_t contour_calculator;
    draw_classifier_contour_init( &contour_calculator );

    const geometry_rectangle_t outer_bounds_before = { .left = 100.0, .top = 90.0, .width = 110.0, .height = 80.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        /* printf("  type: %d\n", t_idx); */

        const geometry_rectangle_t inner_area
            = draw_classifier_contour_calc_inner_area( &contour_calculator,
                                                       classifier_type,
                                                       &outer_bounds_before,
                                                       &pencil_size
                                                     );

        TEST_EXPECT( geometry_rectangle_is_containing( &outer_bounds_before, &inner_area ) );
        TEST_EXPECT( ! geometry_rectangle_is_empty( &inner_area ) );

        const geometry_rectangle_t outer_bounds_after
            = draw_classifier_contour_calc_outer_bounds( &contour_calculator,
                                                         classifier_type,
                                                         &inner_area,
                                                         &pencil_size
                                                       );

        TEST_EXPECT_EQUAL_DOUBLE( 100.0, geometry_rectangle_get_left( &outer_bounds_after ) );
        TEST_EXPECT_EQUAL_DOUBLE( 90.0, geometry_rectangle_get_top( &outer_bounds_after ) );
        TEST_EXPECT_EQUAL_DOUBLE( 110.0, geometry_rectangle_get_width( &outer_bounds_after ) );
        TEST_EXPECT_EQUAL_DOUBLE( 80.0, geometry_rectangle_get_height( &outer_bounds_after ) );
    }

    draw_classifier_contour_destroy( &contour_calculator );
}

static void test_calc_inner_area_too_small(void)
{
    draw_classifier_contour_t contour_calculator;
    draw_classifier_contour_init( &contour_calculator );

    const double gap = pencil_size_get_standard_object_border( &pencil_size );

    const geometry_rectangle_t outer_bounds = { .left = 0.0, .top = 0.0, .width = gap, .height = gap };

    const data_classifier_type_t classifier_type  = DATA_CLASSIFIER_TYPE_USE_CASE;

    const geometry_rectangle_t inner_area
        = draw_classifier_contour_calc_inner_area( &contour_calculator,
                                                   classifier_type,
                                                   &outer_bounds,
                                                   &pencil_size
                                                 );

    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_width( &inner_area ) );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_height( &inner_area ) );

    draw_classifier_contour_destroy( &contour_calculator );
}


/*
 * Copyright 2021-2022 Andreas Warnke
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
