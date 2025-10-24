/* File: geometry_rectangle_test.c; Copyright and License: see below */

#include "geometry_rectangle_test.h"
#include "geometry/geometry_rectangle.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_copy( test_fixture_t *fix );
static test_case_result_t test_set( test_fixture_t *fix );
static test_case_result_t test_contain( test_fixture_t *fix );
static test_case_result_t test_calc_chess_distance( test_fixture_t *fix );
static test_case_result_t test_intersect( test_fixture_t *fix );
static test_case_result_t test_bounds( test_fixture_t *fix );
static test_case_result_t test_difference_max_basic( test_fixture_t *fix );
static test_case_result_t test_difference_max_all_4_candidates( test_fixture_t *fix );
static test_case_result_t test_difference_max_3_U_candidates( test_fixture_t *fix );
static test_case_result_t test_difference_max_2_L_corner_candidates( test_fixture_t *fix );
static test_case_result_t test_difference_max_2_stripe_candidates( test_fixture_t *fix );
static test_case_result_t test_difference_max_only_1_solution( test_fixture_t *fix );
static test_case_result_t test_enlarge( test_fixture_t *fix );
static test_case_result_t test_expand_4d( test_fixture_t *fix );
static test_case_result_t test_embrace( test_fixture_t *fix );
static test_case_result_t test_difference_at_pivot( test_fixture_t *fix );
static test_case_result_t test_difference_at_pivot_corner_cases( test_fixture_t *fix );


test_suite_t geometry_rectangle_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "geometry_rectangle_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_copy", &test_copy );
    test_suite_add_test_case( &result, "test_set", &test_set );
    test_suite_add_test_case( &result, "test_contain", &test_contain );
    test_suite_add_test_case( &result, "test_calc_chess_distance", &test_calc_chess_distance );
    test_suite_add_test_case( &result, "test_intersect", &test_intersect );
    test_suite_add_test_case( &result, "test_bounds", &test_bounds );
    test_suite_add_test_case( &result, "test_difference_max_basic", &test_difference_max_basic );
    test_suite_add_test_case( &result, "test_difference_max_all_4_candidates", &test_difference_max_all_4_candidates );
    test_suite_add_test_case( &result, "test_difference_max_3_U_candidates", &test_difference_max_3_U_candidates );
    test_suite_add_test_case( &result, "test_difference_max_2_L_corner_candidates", &test_difference_max_2_L_corner_candidates );
    test_suite_add_test_case( &result, "test_difference_max_2_stripe_candidates", &test_difference_max_2_stripe_candidates );
    test_suite_add_test_case( &result, "test_difference_max_only_1_solution", &test_difference_max_only_1_solution );
    test_suite_add_test_case( &result, "test_enlarge", &test_enlarge );
    test_suite_add_test_case( &result, "test_expand_4d", &test_expand_4d );
    test_suite_add_test_case( &result, "test_embrace", &test_embrace );
    test_suite_add_test_case( &result, "test_difference_at_pivot", &test_difference_at_pivot );
    test_suite_add_test_case( &result, "test_difference_at_pivot_corner_cases", &test_difference_at_pivot_corner_cases );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_copy( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t rect_c;

    geometry_rectangle_init( &rect_a, 1.0, 2.0, 3.0 /*width*/, 4.0 /*height*/ );
    TEST_EXPECT_EQUAL_FLOAT( 1.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 4.0, geometry_rectangle_get_right( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 6.0, geometry_rectangle_get_bottom( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 3.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 4.0, geometry_rectangle_get_height( &rect_a ) );

    geometry_rectangle_init_empty( &rect_b );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_left( &rect_b ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_top( &rect_b ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_width( &rect_b ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_height( &rect_b ) );

    geometry_rectangle_reinit( &rect_a, -1.0, -1.0, 2.0 /*width*/, 2.0 /*height*/ );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_height( &rect_a ) );

    geometry_rectangle_destroy( &rect_b );
    geometry_rectangle_copy( &rect_b, &rect_a );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_left( &rect_b ) );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_top( &rect_b ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_width( &rect_b ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_height( &rect_b ) );

    geometry_rectangle_reinit_empty( &rect_a );
    geometry_rectangle_move( &rect_c, &rect_a );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_left( &rect_c ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_top( &rect_c ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_width( &rect_c ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, geometry_rectangle_get_height( &rect_c ) );
    /* rect_a is destroyed now */

    geometry_rectangle_replace( &rect_c, &rect_b );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_left( &rect_c ) );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_top( &rect_c ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_width( &rect_c ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_height( &rect_c ) );

    geometry_rectangle_init_empty( &rect_a );
    geometry_rectangle_replacemove( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( -1.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 2.0, geometry_rectangle_get_height( &rect_a ) );
    /* rect_b is destroyed now */

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_c );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;

    geometry_rectangle_init_empty( &rect_a );
    geometry_rectangle_set_left( &rect_a, -38.5 );
    geometry_rectangle_set_top( &rect_a, -12.625 );
    geometry_rectangle_set_width( &rect_a, 3.5 );
    geometry_rectangle_set_height( &rect_a, 0.375 );
    TEST_EXPECT_EQUAL_FLOAT( -38.5, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( -12.625, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 3.5, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.375, geometry_rectangle_get_height( &rect_a ) );

    geometry_dimensions_t dim = geometry_rectangle_get_dimensions( &rect_a );
    TEST_EXPECT_EQUAL_FLOAT( 3.5, geometry_dimensions_get_width( &dim ) );
    TEST_EXPECT_EQUAL_FLOAT( 0.375, geometry_dimensions_get_height( &dim ) );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_contain( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    bool contains;

    /* no contain */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 11.0, 11.0, 11.0 /*width*/, 11.0 /*height*/ );

    contains = geometry_rectangle_is_containing( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( false, contains );

    contains = geometry_rectangle_is_containing( &rect_b, &rect_a );
    TEST_EXPECT_EQUAL_INT( false, contains );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );

    /* contain */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 10.0, 11.0, 9.0 /*width*/, 9.0 /*height*/ );

    contains = geometry_rectangle_is_containing( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( true, contains );

    contains = geometry_rectangle_is_containing( &rect_b, &rect_a );
    TEST_EXPECT_EQUAL_INT( false, contains );

    contains = geometry_rectangle_contains( &rect_a, 9.0, 9.0 );
    TEST_EXPECT_EQUAL_INT( false, contains );

    contains = geometry_rectangle_contains( &rect_a, 19.0, 19.0 );
    TEST_EXPECT_EQUAL_INT( true, contains );

    /* test point */
    geometry_point_t center = geometry_rectangle_get_center( &rect_a );
    contains = geometry_rectangle_contains_point( &rect_a, &center );
    TEST_EXPECT_EQUAL_INT( true, contains );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_calc_chess_distance( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;

    geometry_rectangle_init( &rect_a, 5.0, 5.0, 1.0 /*width*/, 2.0 /*height*/ );

    const double to_left = geometry_rectangle_calc_chess_distance( &rect_a, 3.0 /*x*/, 6.0 /*y*/ );
    TEST_EXPECT_EQUAL_DOUBLE( 2.0, to_left );
    const double to_top_right = geometry_rectangle_calc_chess_distance( &rect_a, 7.0 /*x*/, 3.0 /*y*/ );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, to_top_right );
    const double to_bottom = geometry_rectangle_calc_chess_distance( &rect_a, 5.5 /*x*/, 8.0 /*y*/ );
    TEST_EXPECT_EQUAL_DOUBLE( 1.0, to_bottom );
    const double within = geometry_rectangle_calc_chess_distance( &rect_a, 5.5 /*x*/, 6.0 /*y*/ );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, within );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_intersect( test_fixture_t *fix )
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
    TEST_EXPECT_EQUAL_INT( -1, err );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( false, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* touch */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 20.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( false, intersects );

    intersects = geometry_rectangle_is_contiguous( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* part intersect */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 15.0, 15.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_DOUBLE( 25.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );

    /* full contained */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 20.0 /*width*/, 20.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 15.0, 15.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_intersect( &intersect_rect, &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_DOUBLE( 100.0, geometry_rectangle_get_area( &intersect_rect ) );

    intersects = geometry_rectangle_is_intersecting( &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( true, intersects );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &intersect_rect );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_bounds( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t bounds_rect;
    int err;

    /* part intersect */
    geometry_rectangle_init ( &rect_a, 10.0, 10.0, 10.0 /*width*/, 10.0 /*height*/ );
    geometry_rectangle_init ( &rect_b, 18.0, 12.0, 10.0 /*width*/, 10.0 /*height*/ );
    err = geometry_rectangle_init_by_bounds( &bounds_rect, &rect_a, &rect_b );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_left( &bounds_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 28.0, geometry_rectangle_get_right( &bounds_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_top( &bounds_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 22.0, geometry_rectangle_get_bottom( &bounds_rect ) );

    /* clean up */
    geometry_rectangle_destroy ( &rect_a );
    geometry_rectangle_destroy ( &rect_b );
    geometry_rectangle_destroy ( &bounds_rect );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_max_basic( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /* no intersect */
    {
        geometry_rectangle_init ( &rect_a, 9.0, 10.0, 1.0 /*width*/, 90.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, 11.0, 20.0, 10.0 /*width*/, 10.0 /*height*/ );
        geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
        TEST_EXPECT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_left( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_top( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 1.0, geometry_rectangle_get_width( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 90.0, geometry_rectangle_get_height( &diff_rect ) );

        /* clean up */
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }

    /* part intersect */
    {
        geometry_rectangle_init ( &rect_a, 9.0, 10.0, 1.0 /*width*/, 90.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, 8.0, 11.0, 1.5 /*width*/, 1.0 /*height*/ );
        geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
        TEST_EXPECT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_left( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 12.0, geometry_rectangle_get_top( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 1.0, geometry_rectangle_get_width( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 88.0, geometry_rectangle_get_height( &diff_rect ) );

        /* clean up */
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }

    /* full contained */
    {
        geometry_rectangle_init ( &rect_a, 9.0, 10.0, 1.0 /*width*/, 90.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, 8.0, 9.0, 3.0 /*width*/, 92.0 /*height*/ );
        geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
        TEST_EXPECT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_left( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_top( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_width( &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_height( &diff_rect ) );

        /* clean up */
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_max_all_4_candidates( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    for ( int left_space = 0; left_space < 4; left_space ++ )
    {
        for ( int top_space = 0; top_space < 4; top_space ++ )
        {
            if ( top_space == left_space )
            {
                /* not a test case, only different widths/heights are tested */
            }
            else
            {
                for ( int right_space = 0; right_space < 4; right_space ++ )
                {
                    if (( right_space == left_space )||( right_space == top_space ))
                    {
                        /* not a test case */
                    }
                    else
                    {
                        for ( int bottom_space = 0; bottom_space < 4; bottom_space ++ )
                        {
                            if (( bottom_space == left_space )||( bottom_space == top_space )||( bottom_space == right_space ))
                            {
                                /* not a test case */
                            }
                            else
                            {
                                geometry_rectangle_init( &rect_a, 1.0, 1.0, 8.0 /*width*/, 8.0 /*height*/ );
                                geometry_rectangle_init( &rect_b,
                                                         1.5 + left_space,
                                                         1.5 + top_space,
                                                         7.0 - left_space - right_space,
                                                         7.0 - top_space - bottom_space );
                                geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
                                TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
                                TEST_EXPECT_EQUAL_DOUBLE( 28.0, geometry_rectangle_get_area( &diff_rect ) );
                                geometry_rectangle_destroy( &rect_a );
                                geometry_rectangle_destroy( &rect_b );
                                geometry_rectangle_destroy( &diff_rect );
                            }
                        }
                    }
                }
            }
        }
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_max_3_U_candidates( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    for ( int left_space = 0; left_space < 4; left_space ++ )
    {
        for ( int top_space = 0; top_space < 4; top_space ++ )
        {
            if ( top_space == left_space )
            {
                /* not a test case, only different widths/heights are tested */
            }
            else
            {
                for ( int right_space = 0; right_space < 4; right_space ++ )
                {
                    if (( right_space == left_space )||( right_space == top_space ))
                    {
                        /* not a test case */
                    }
                    else
                    {
                        for ( int bottom_space = 0; bottom_space < 4; bottom_space ++ )
                        {
                            if (( bottom_space == left_space )||( bottom_space == top_space )||( bottom_space == right_space ))
                            {
                                /* not a test case */
                            }
                            else
                            {
                                geometry_rectangle_init( &rect_a, 1.0, 1.0, 8.0 /*width*/, 8.0 /*height*/ );
                                geometry_rectangle_init( &rect_b,
                                                         0.5 + left_space,
                                                         0.5 + top_space,
                                                         9.0 - left_space - right_space,
                                                         9.0 - top_space - bottom_space );
                                geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
                                TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
                                TEST_EXPECT_EQUAL_DOUBLE( 20.0, geometry_rectangle_get_area( &diff_rect ) );
                                geometry_rectangle_destroy( &rect_a );
                                geometry_rectangle_destroy( &rect_b );
                                geometry_rectangle_destroy( &diff_rect );
                            }
                        }
                    }
                }
            }
        }
    }

    return TEST_CASE_RESULT_OK;
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

static test_case_result_t test_difference_max_2_L_corner_candidates( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /* these 8 test cases test the L-shaped result sets where all 8 combinations of rotations and width/height ratios are covered */
    /*                 le.to.    ri.to.    ri.bo.    le.bo.  */
    double in_x[8] = { 2.0, 1.0, 5.0, 6.0, 6.0, 5.0, 2.0, 1.0 };
    double in_y[8] = { 1.0, 2.0, 1.0, 2.0, 5.0, 6.0, 6.0, 5.0 };
    for ( int case_idx = 0; case_idx < 8; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 3.0, 3.0, 4.0 /*width*/, 4.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], 3.0, 3.0 );
        geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
        TEST_EXPECT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 12.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_max_2_stripe_candidates( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /* these 4 test cases test the stripe-shaped result sets where all 4 combinations of rotations and width/height ratios are covered */
    /*                 vertical  horizontal */
    double in_x[4] = { 4.0, 5.0, 1.0, 1.0 };
    double in_y[4] = { 1.0, 1.0, 4.0, 5.0 };
    double in_w[4] = { 1.0, 1.0, 8.0, 8.0 };
    double in_h[4] = { 8.0, 8.0, 1.0, 1.0 };
    for ( int case_idx = 0; case_idx < 4; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 3.0, 3.0, 4.0 /*width*/, 4.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], in_w[case_idx], in_h[case_idx] );
        geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
        TEST_EXPECT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 8.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_max_only_1_solution( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;
    geometry_rectangle_t rect_b;
    geometry_rectangle_t diff_rect;

    /* these 4 test cases test result sets where exactly 1 solution exists */
    /*                right top left bottom */
    double in_x[4] = { 1.0, 3.0, 5.0, 3.0 };
    double in_y[4] = { 3.0, 5.0, 3.0, 1.0 };
    for ( int case_idx = 0; case_idx < 4; case_idx ++ )
    {
        geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
        geometry_rectangle_init ( &rect_b, in_x[case_idx], in_y[case_idx], 4.0 /*width*/, 4.0 /*height*/ );
        geometry_rectangle_init_by_difference_max( &diff_rect, &rect_a, &rect_b );
        TEST_EXPECT( geometry_rectangle_is_containing( &rect_a, &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &rect_b, &diff_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_area( &diff_rect ) );
        geometry_rectangle_destroy ( &rect_a );
        geometry_rectangle_destroy ( &rect_b );
        geometry_rectangle_destroy ( &diff_rect );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_enlarge( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;

    /* good case */
    geometry_rectangle_init ( &rect_a, 3.0, 4.0, 1.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_enlarge ( &rect_a, 10.0, 20.0 );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 11.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 22.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );

    /* negative-size case */
    geometry_rectangle_init ( &rect_a, 3.0, 4.0, 1.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_enlarge ( &rect_a, -10.0, -20.0 );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_expand_4d( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;

    /* good case */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_expand_4dir ( &rect_a, 1.0, 2.0 );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 6.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );

    /* negative-size case */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_expand_4dir ( &rect_a, -3.0, -4.0 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_embrace( test_fixture_t *fix )
{
    geometry_rectangle_t rect_a;

    /* already contained */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_embrace ( &rect_a, 5.0, 5.0 );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );

    /* top left expand */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_embrace ( &rect_a, -3.0, -4.0 );
    TEST_EXPECT_EQUAL_DOUBLE( -3.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( -4.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 9.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );

    /* bottom right expand */
    geometry_rectangle_init ( &rect_a, 4.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    geometry_rectangle_embrace ( &rect_a, 7.0, 8.0 );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_left( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_top( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_width( &rect_a ) );
    TEST_EXPECT_EQUAL_DOUBLE( 4.0, geometry_rectangle_get_height( &rect_a ) );
    geometry_rectangle_destroy ( &rect_a );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_at_pivot( test_fixture_t *fix )
{
    geometry_rectangle_t rect_moon;
    geometry_rectangle_t rect_shadow;
    geometry_rectangle_t diff_rect;

    geometry_rectangle_init ( &rect_moon, 2.0, 2.0, 6.0 /*width*/, 7.0 /*height*/ );
    geometry_rectangle_init ( &rect_shadow, 3.0, 4.0, 2.0 /*width*/, 1.0 /*height*/ );
    /*    2 3 4 5 6 7 8 */
    /* 2  + - - - - - + */
    /* 3  I           I */
    /* 4  I + - +     I */
    /* 5  I + - +     I */
    /* 6  I           I */
    /* 7  I           I */
    /* 8  I           I */
    /* 9  + - - - - - + */

    /* pivot at left top */
    geometry_point_t sun_1 = { .x = 1.0, .y = 1.0 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_1 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at left */
    geometry_point_t sun_1b = { .x = 2.5, .y = 4.5 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_1b );
    TEST_EXPECT_EQUAL_DOUBLE( 2.5, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.5, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at left bottom */
    geometry_point_t sun_2 = { .x = 2.5, .y = 8.5 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_2 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at bottom */
    geometry_point_t sun_2b = { .x = 4.0, .y = 6.0 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_2b );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at right bottom */
    geometry_point_t sun_3 = { .x = 5.0, .y = 5.0 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_3 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at right */
    geometry_point_t sun_3b = { .x = 5.5, .y = 4.5 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_3b );
    TEST_EXPECT_EQUAL_DOUBLE( 6.5, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.5, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at right top */
    geometry_point_t sun_4 = { .x = 7.0, .y = 3.0 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_4 );
    TEST_EXPECT_EQUAL_DOUBLE( 6.5, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.5, geometry_rectangle_get_center_y( &diff_rect ) );

    /* pivot at top */
    geometry_point_t sun_4b = { .x = 4.0, .y = 3.0 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_4b );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_center_y( &diff_rect ) );

    geometry_rectangle_destroy ( &rect_moon );
    geometry_rectangle_destroy ( &rect_shadow );
    geometry_rectangle_destroy ( &diff_rect );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_difference_at_pivot_corner_cases( test_fixture_t *fix )
{
    geometry_rectangle_t rect_moon;
    geometry_rectangle_t rect_shadow;
    geometry_rectangle_t diff_rect;

    /* cases: no overlap */
    geometry_rectangle_init ( &rect_moon, 2.0, 2.0, 6.0 /*width*/, 7.0 /*height*/ );
    geometry_rectangle_init ( &rect_shadow, 9.0, 4.0, 2.0 /*width*/, 2.0 /*height*/ );
    /*    2 3 4 5 6 7 8 9      */
    /* 2  + - - - - - +        */
    /* 3  I           I        */
    /* 4  I           I  + - + */
    /* 5  I           I  I   I */
    /* 6  I           I  + - + */
    /* 7  I           I        */
    /* 8  I           I        */
    /* 9  + - - - - - +        */

    /* case: no overlap, pivot at left top */
    const geometry_point_t sun_1 = geometry_point_new( 1.0, 1.0 );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_1 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.5, geometry_rectangle_get_center_y( &diff_rect ) );

    /* cases: shadow not contained */
    geometry_rectangle_reinit ( &rect_shadow, 1.0, 4.0, 4.0 /*width*/, 5.0 /*height*/ );
    /*    1 2 3 4 5 6 7 8 */
    /* 2    + - - - - - + */
    /* 3    I           I */
    /* 4  + - - - +     I */
    /* 5  +       I     I */
    /* 6  +       I     I */
    /* 7  +       I     I */
    /* 8  +       I     I */
    /* 9  + - - - + - - + */

    /* case: shadow not contained, pivot at left */
    geometry_point_t sun_5 = { .x = 2.0, .y = 5.0 };
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_5 );
    TEST_EXPECT_EQUAL_DOUBLE( 2.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.5, geometry_rectangle_get_center_y( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_area( &diff_rect ) );

    /* cases: sun and moon at center --> select area closest to pivot(sun) and larger width/height */
    geometry_rectangle_reinit ( &rect_moon, 2.0, 2.0, 6.0 /*width*/, 6.0 /*height*/ );
    const geometry_point_t sun_0 = { .x = 5.0, .y = 5.0 };
    /*    2 3 4 5 6 7 8 */
    /* 2  + - - - - - + */
    /* 3  I           I */
    /* 4  I   + - +   I */
    /* 5  I   I   I   I */
    /* 6  I   + - +   I */
    /* 7  I           I */
    /* 8  + - - - - - + */

    /* case: sun and moon at center, left is closest difference-area and larger width/height */
    geometry_rectangle_reinit ( &rect_shadow, 4.0, 3.9, 2.1 /*width*/, 2.3 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_y( &diff_rect ) );

    geometry_rectangle_reinit ( &rect_shadow, 4.0, 3.8, 2.1 /*width*/, 2.3 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* case: sun and moon at center, top is closest difference-area and larger width/height */
    geometry_rectangle_reinit ( &rect_shadow, 3.9, 4.0, 2.3 /*width*/, 2.1 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_center_y( &diff_rect ) );

    geometry_rectangle_reinit ( &rect_shadow, 3.8, 4.0, 2.3 /*width*/, 2.1 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 3.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* case: sun and moon at center, right is closest difference-area and larger width/height */
    geometry_rectangle_reinit ( &rect_shadow, 3.9, 3.9, 2.1 /*width*/, 2.3 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_y( &diff_rect ) );

    geometry_rectangle_reinit ( &rect_shadow, 3.9, 3.8, 2.1 /*width*/, 2.3 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_y( &diff_rect ) );

    /* case: sun and moon at center, bottom is closest difference-area and larger width/height */
    geometry_rectangle_reinit ( &rect_shadow, 3.9, 3.9, 2.3 /*width*/, 2.1 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_y( &diff_rect ) );

    geometry_rectangle_reinit ( &rect_shadow, 3.8, 3.9, 2.3 /*width*/, 2.1 /*height*/ );
    geometry_rectangle_init_by_difference_at_pivot( &diff_rect, &rect_moon, &rect_shadow, &sun_0 );
    TEST_EXPECT_EQUAL_DOUBLE( 5.0, geometry_rectangle_get_center_x( &diff_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 7.0, geometry_rectangle_get_center_y( &diff_rect ) );

    geometry_rectangle_destroy ( &rect_moon );
    geometry_rectangle_destroy ( &rect_shadow );
    geometry_rectangle_destroy ( &diff_rect );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2025 Andreas Warnke
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
