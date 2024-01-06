/* File: pencil_classifier_composer_test.c; Copyright and License: see below */

#include "pencil_classifier_composer_test.h"
#include "pencil_classifier_composer.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_expand_space( test_fixture_t *fix );
static test_case_result_t test_set_envelope_box( test_fixture_t *fix );
static test_case_result_t test_set_envelope_box_too_small( test_fixture_t *fix );

test_suite_t pencil_classifier_composer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_classifier_composer_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_expand_space", &test_expand_space );
    test_suite_add_test_case( &result, "test_set_envelope_box", &test_set_envelope_box );
    test_suite_add_test_case( &result, "test_set_envelope_box_too_small", &test_set_envelope_box_too_small );
    return result;
}

struct test_fixture_struct {
    layout_visible_classifier_t layout_vis_classifier;
    data_visible_classifier_t data_vis_classifier;
    data_profile_part_t profile;
    pencil_size_t pencil_size;
    draw_classifier_contour_t draw_classifier_contour;
    cairo_surface_t *surface;
    cairo_t *cr;
    geometry_rectangle_t diagram_bounds;
    PangoLayout *font_layout;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    /* init a pango font layout */
    {
        geometry_rectangle_init( &(*fix).diagram_bounds, 0.0, 0.0, 640.0, 480.0 );
        (*fix).surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32,
                                                     (uint32_t) geometry_rectangle_get_width( &(*fix).diagram_bounds ),
                                                     (uint32_t) geometry_rectangle_get_height( &(*fix).diagram_bounds )
                                                   );
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( (*fix).surface ) );
        (*fix).cr = cairo_create ( (*fix).surface );
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( (*fix).cr ) );
        (*fix).font_layout = pango_cairo_create_layout ( (*fix).cr );
    }

    data_profile_part_init( &(*fix).profile );

    pencil_size_init( &(*fix).pencil_size,
                      geometry_rectangle_get_width( &(*fix).diagram_bounds ),
                      geometry_rectangle_get_height( &(*fix).diagram_bounds )
                    );

    draw_classifier_contour_init( &(*fix).draw_classifier_contour );

    /* init a layout visible classifier */
    {
        data_classifier_t data_classifier;
        data_diagramelement_t data_diagele;

        const u8_error_t err1
            = data_diagramelement_init( &data_diagele,
                                        17,  /* id */
                                        32,  /* diagram_id */
                                        99,  /* classifier_id */
                                        DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE,
                                        DATA_ROW_ID_VOID,
                                        "1ded6d32-cdea-43d8-931c-9459065f8944"
                                      );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, err1 );
        const u8_error_t err2
            = data_classifier_init( &data_classifier,
                                    99,  /* id */
                                    DATA_CLASSIFIER_TYPE_CLASS,  /* main_type */
                                    "stereotype",
                                    "classifier name",
                                    "classifier\ndescription",
                                    10000,  /* x_order */
                                    11000,  /* y_order */
                                    12000,  /* list_order */
                                    "94ad5563-3040-4f27-8e8b-d51ffc5ad4c8"
                                  );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, err2 );
        data_visible_classifier_init( &(*fix).data_vis_classifier, &data_classifier, &data_diagele );
        layout_visible_classifier_init( &(*fix).layout_vis_classifier, &(*fix).data_vis_classifier );

        data_diagramelement_destroy( &data_diagele );
        data_classifier_destroy( &data_classifier );
    }
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    /* destroy the layout visible classifier */
    {
        layout_visible_classifier_destroy( &(*fix).layout_vis_classifier );
        data_visible_classifier_destroy( &(*fix).data_vis_classifier );
    }

    draw_classifier_contour_destroy( &(*fix).draw_classifier_contour );

    pencil_size_destroy( &(*fix).pencil_size );

    data_profile_part_destroy( &(*fix).profile );

    /* destroy the pango font layout */
    {
        g_object_unref ( (*fix).font_layout );
        cairo_destroy ( (*fix).cr );
        cairo_surface_finish ( (*fix).surface );
        cairo_surface_destroy ( (*fix).surface );
        geometry_rectangle_destroy( &(*fix).diagram_bounds );
    }
}

static test_case_result_t test_expand_space( test_fixture_t *fix )
{
    assert( fix != NULL );
    pencil_classifier_composer_t classifier_composer;
    pencil_classifier_composer_init( &classifier_composer );

    const geometry_rectangle_t in_space = { .left = 100.0, .top = 90.0, .width = 420.0, .height = 104.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        data_classifier_set_main_type( data_visible_classifier_get_classifier_ptr( &(*fix).data_vis_classifier ),
                                       classifier_type
                                     );
        /* printf("  type: %d, %d\n", t_idx, classifier_type); */

        for ( unsigned int show_children = 0; show_children <= 1; show_children ++ )
        {
            /* run composing method */
            const int err
                = pencil_classifier_composer_expand_space( &classifier_composer,
                                                           &in_space,
                                                           (show_children != 0),
                                                           &(*fix).profile,
                                                           &(*fix).pencil_size,
                                                           (*fix).font_layout,
                                                           &(*fix).layout_vis_classifier
                                                         );
            TEST_EXPECT_EQUAL_INT( 0, err );

            /* check that all layouted rectangles are outside space */
            const geometry_rectangle_t *const symbol
                = layout_visible_classifier_get_symbol_box_const( &(*fix).layout_vis_classifier );
            const bool has_contour
                = draw_classifier_contour_has_contour( &(*fix).draw_classifier_contour, classifier_type );
            if ( ! has_contour )
            {
                /* no intesects if fix-size-symbol */
                TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &in_space, symbol ) );
            }
            TEST_EXPECT( ! geometry_rectangle_is_empty( symbol ) );
            const geometry_rectangle_t *const label
                = layout_visible_classifier_get_label_box_const( &(*fix).layout_vis_classifier );
            TEST_EXPECT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &in_space, label ) );
            TEST_EXPECT( ! geometry_rectangle_is_empty( label ) );
            const geometry_rectangle_t *const space
                = layout_visible_classifier_get_space_const( &(*fix).layout_vis_classifier );
            TEST_EXPECT_EQUAL_DOUBLE( 100.0, geometry_rectangle_get_left( space ) );
            TEST_EXPECT_EQUAL_DOUBLE( 90.0, geometry_rectangle_get_top( space ) );
            TEST_EXPECT_EQUAL_DOUBLE( 420.0, geometry_rectangle_get_width( space ) );
            TEST_EXPECT_EQUAL_DOUBLE( 104.0, geometry_rectangle_get_height( space ) );
        }
    }

    pencil_classifier_composer_destroy( &classifier_composer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_envelope_box( test_fixture_t *fix )
{
    assert( fix != NULL );
    pencil_classifier_composer_t classifier_composer;
    pencil_classifier_composer_init( &classifier_composer );

    const geometry_rectangle_t envelope = { .left = 50.0, .top = 40.0, .width = 460.0, .height = 180.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        data_classifier_set_main_type( data_visible_classifier_get_classifier_ptr( &(*fix).data_vis_classifier ),
                                       classifier_type
                                     );
        /* printf("  type: %d, %d\n", t_idx, classifier_type); */

        for ( unsigned int show_children = 0; show_children <= 1; show_children ++ )
        {
            /* run composing method */
            const int err
                = pencil_classifier_composer_set_envelope_box( &classifier_composer,
                                                               &envelope,
                                                               (show_children != 0),
                                                               &(*fix).profile,
                                                               &(*fix).pencil_size,
                                                               (*fix).font_layout,
                                                               &(*fix).layout_vis_classifier
                                                             );
            TEST_EXPECT_EQUAL_INT( 0, err );

            /* check that all layouted rectangles are within envelope */
            const geometry_rectangle_t *const symbol
                = layout_visible_classifier_get_symbol_box_const( &(*fix).layout_vis_classifier );
            TEST_EXPECT( geometry_rectangle_is_containing( &envelope, symbol ) );
            TEST_EXPECT( ! geometry_rectangle_is_empty( symbol ) );
            const geometry_rectangle_t *const label
                = layout_visible_classifier_get_label_box_const( &(*fix).layout_vis_classifier );
            TEST_EXPECT( geometry_rectangle_is_containing( &envelope, label ) );
            TEST_EXPECT( ! geometry_rectangle_is_empty( label ) );
            const geometry_rectangle_t *const space
                = layout_visible_classifier_get_space_const( &(*fix).layout_vis_classifier );
            TEST_EXPECT( geometry_rectangle_is_containing( &envelope, space ) );
            TEST_EXPECT( ! geometry_rectangle_is_empty( space ) );
        }
    }

    pencil_classifier_composer_destroy( &classifier_composer );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_envelope_box_too_small( test_fixture_t *fix )
{
    assert( fix != NULL );
    pencil_classifier_composer_t classifier_composer;
    pencil_classifier_composer_init( &classifier_composer );

    const geometry_rectangle_t small_envelope = { .left = 150.0, .top = 140.0, .width = 15.0, .height = 10.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        data_classifier_set_main_type( data_visible_classifier_get_classifier_ptr( &(*fix).data_vis_classifier ),
                                       classifier_type
                                     );
        /* printf("  type: %d, %d\n", t_idx, classifier_type); */

        for ( unsigned int show_children = 0; show_children <= 1; show_children ++ )
        {
            /* run composing method */
            const int err
                = pencil_classifier_composer_set_envelope_box( &classifier_composer,
                                                               &small_envelope,
                                                               (show_children != 0),
                                                               &(*fix).profile,
                                                               &(*fix).pencil_size,
                                                               (*fix).font_layout,
                                                               &(*fix).layout_vis_classifier
                                                             );
            TEST_EXPECT_EQUAL_INT( 1, err );

            /* check that actual_envelope rectangle is bigger and is within small_envelope */
            const geometry_rectangle_t actual_envelope
                = layout_visible_classifier_get_envelope_box( &(*fix).layout_vis_classifier );
            geometry_rectangle_trace( &small_envelope );
            geometry_rectangle_trace( &actual_envelope );
            TEST_EXPECT( geometry_rectangle_is_containing( &actual_envelope, &small_envelope ) );
            TEST_EXPECT( ! geometry_rectangle_is_containing( &small_envelope, &actual_envelope ) );

            /* check that label and space are withing symbol (unless fix-sized-symbol) */
            const bool has_contour
                = draw_classifier_contour_has_contour( &(*fix).draw_classifier_contour, classifier_type );
            if ( has_contour )
            {
                const geometry_rectangle_t *const symbol
                    = layout_visible_classifier_get_symbol_box_const( &(*fix).layout_vis_classifier );
                const geometry_rectangle_t *const label
                    = layout_visible_classifier_get_label_box_const( &(*fix).layout_vis_classifier );
                const geometry_rectangle_t *const space
                    = layout_visible_classifier_get_space_const( &(*fix).layout_vis_classifier );
                TEST_EXPECT( geometry_rectangle_is_containing( symbol, label ) );
                TEST_EXPECT( geometry_rectangle_is_containing( symbol, space ) );
            }
        }
    }

    pencil_classifier_composer_destroy( &classifier_composer );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2021-2024 Andreas Warnke
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
