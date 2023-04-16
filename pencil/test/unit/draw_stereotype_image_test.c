/* File: draw_stereotype_image_test.c; Copyright and License: see below */

#include "draw_stereotype_image_test.h"
#include "draw/draw_stereotype_image.h"
#include "data_classifier_type.h"
#include "test_expect.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_parse_valid_simple_svg_xml( test_fixture_t *test_env );
static test_case_result_t test_parse_valid_svg_xml( test_fixture_t *test_env );
static test_case_result_t test_parse_invalid_svg_xml( test_fixture_t *test_env );

test_suite_t draw_stereotype_image_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "draw_stereotype_image_test_get_suite", &set_up, &tear_down );

    test_suite_add_test_case( &result, "test_parse_valid_simple_svg_xml", &test_parse_valid_simple_svg_xml );
    test_suite_add_test_case( &result, "test_parse_valid_svg_xml", &test_parse_valid_svg_xml );
    test_suite_add_test_case( &result, "test_parse_invalid_svg_xml", &test_parse_invalid_svg_xml );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

#define DRAW_STEREOTYPE_IMAGE_SIMPLE_TESTS_MAX (20)

static test_case_result_t test_parse_valid_simple_svg_xml( test_fixture_t *test_env )
{
    geometry_rectangle_t view_rect;
    u8_error_info_t svg_err_info;
    draw_stereotype_image_t svg_parser;
    draw_stereotype_image_init( &svg_parser );

    const char *const drawing_directives[DRAW_STEREOTYPE_IMAGE_SIMPLE_TESTS_MAX]
    = {
        "<path d=\"m 10,10 -10,-10\"/>",
        "<path d=\"l 10,10\"/>",
        "<path d=\"v 10 h 10\"/>",
        "<path d=\"h 10 v 10 z\"/>",
        "<path d=\"q 5,6 10,10\"/>",
        "<path d=\"t 10,10\"/>",
        "<path d=\"c 4,5 6,7 10,10\"/>",
        "<path d=\"s 6,7 10,10\"/>",
        "<path d=\"z l 10,10\"/>",
        "<path d=\"a 5,5 0 1 0 5,-5\"/>",
        "<path d=\"M 10,10 0,0\"/>",
        "<path d=\"L 10,10\"/>",
        "<path d=\"V 10 H 10\"/>",
        "<path d=\"H 10 V 10 Z\"/>",
        "<path d=\"Q 5,6 10,10\"/>",
        "<path d=\"T 10,10\"/>",
        "<path d=\"C 4,5 6,7 10,10\"/>",
        "<path d=\"S 6,7 10,10\"/>",
        "<path d=\"Z L 10,10\"/>",
        "<path d=\"A 5,5 0 1 0 5,-5\"/>",
    };
    for ( int t_case = 0; t_case < DRAW_STEREOTYPE_IMAGE_SIMPLE_TESTS_MAX; t_case ++ )
    {
        const bool is_arc = (( t_case == 9 )||( t_case == 19 ));
        const u8_error_t svg_err
            = draw_stereotype_image_parse_svg_xml( &svg_parser,
                                                   drawing_directives[t_case],
                                                   &view_rect,
                                                   &svg_err_info
                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, svg_err );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &svg_err_info ) );
        TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_left( &view_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( is_arc ? -5.0 : 0.0, geometry_rectangle_get_top( &view_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_width( &view_rect ) );
        TEST_EXPECT_EQUAL_DOUBLE( 10.0, geometry_rectangle_get_height( &view_rect ) );
    }

    draw_stereotype_image_destroy( &svg_parser );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_parse_valid_svg_xml( test_fixture_t *test_env )
{
    geometry_rectangle_t view_rect;
    u8_error_info_t svg_err_info;
    draw_stereotype_image_t svg_parser;
    draw_stereotype_image_init( &svg_parser );

    const char *const drawing_directives
        = "<path d=\""
        "M 1,1 2,0"
        "l 1,1 -1,1"
        "H 1"
        "v 1"
        "Q 2,2 3,3"
        "t 0,-2"
        "C 3,0.45 3.45,0 4,0"
        "s 1,0.45 1,1"
        "Z"
        "a 1,2 -60 0 1 -1,0"
        "m -1,0 -1,0"
        "L -3,0 -2,-1"
        "h -1 2"
        "V -3 0"
        "q 1,1 0,2 -1,1 0,2"
        "T -1,6 -1 8"
        "c 1,0 1,-1 2,-1 0,-1 -1,-1 -1,-2"
        "S -1, 4 -1 3 2,-2 2,-1"
        "z z"
        "A 1,1 45 0 1 -2 0 1,2 45 1 0 -1 -1"
        "\"/>";
    const u8_error_t svg_err
        = draw_stereotype_image_parse_svg_xml( &svg_parser,
                                               drawing_directives,
                                               &view_rect,
                                               &svg_err_info
                                             );

    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, svg_err );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, u8_error_info_get_error( &svg_err_info ) );
    TEST_EXPECT_EQUAL_DOUBLE( -3.0, geometry_rectangle_get_left( &view_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( -3.0, geometry_rectangle_get_top( &view_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 8.0, geometry_rectangle_get_width( &view_rect ) );
    TEST_EXPECT_EQUAL_DOUBLE( 11.0, geometry_rectangle_get_height( &view_rect ) );

    draw_stereotype_image_destroy( &svg_parser );
    return TEST_CASE_RESULT_OK;
}

#define DRAW_STEREOTYPE_IMAGE_INVALID_TESTS_MAX (8)

static test_case_result_t test_parse_invalid_svg_xml( test_fixture_t *test_env )
{
    geometry_rectangle_t view_rect;
    u8_error_info_t svg_err_info;
    draw_stereotype_image_t svg_parser;
    draw_stereotype_image_init( &svg_parser );

    const char *const drawing_directives[DRAW_STEREOTYPE_IMAGE_INVALID_TESTS_MAX]
    = {
        "path />",
        "<path \nd=\"\"/>",
        "<path \n\nd=\"z\"/>",
        "<path \n\n\nd=\"2\"/>",
        "<path \n\n\nd=\"L 2,2 \n 3\"/>",
        "<path \n\n\nd=\"a 10,20 30 1 0 10,0 \n\n t\"/>",
        "<path \n\n\nd=\"a 10,20 60 0 1 10,0 \n\n m\n m\"/>",
        "<path \n\n\nd=\"a 10,20 115 0 0 10,0 \n\n z \n\n 3\"/>",
    };
    for ( int t_case = 0; t_case < DRAW_STEREOTYPE_IMAGE_INVALID_TESTS_MAX; t_case ++ )
    {
        const bool no_path = ( t_case <= 3 );
        const bool parse_err = ( t_case >= 3 );
        const u8_error_t svg_err
            = draw_stereotype_image_parse_svg_xml( &svg_parser,
                                                   drawing_directives[t_case],
                                                   &view_rect,
                                                   &svg_err_info
                                                 );

        TEST_EXPECT_EQUAL_INT( no_path, u8_error_contains( svg_err, U8_ERROR_NOT_FOUND ) );
        TEST_EXPECT_EQUAL_INT( parse_err, u8_error_contains( svg_err, U8_ERROR_PARSER_STRUCTURE ) );
        if ( parse_err )
        {
            TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, u8_error_info_get_error( &svg_err_info ) );
            TEST_EXPECT_EQUAL_INT( 1+t_case, u8_error_info_get_line( &svg_err_info ) );
        }
    }

    draw_stereotype_image_destroy( &svg_parser );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2023-2023 Andreas Warnke
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
