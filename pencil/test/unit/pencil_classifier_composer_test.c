/* File: pencil_classifier_composer_test.c; Copyright and License: see below */

#include "pencil_classifier_composer_test.h"
#include "pencil_classifier_composer.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_expand_inner_space(void);
static void test_set_envelope_box(void);
static void test_set_envelope_box_too_small(void);

test_suite_t pencil_classifier_composer_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_classifier_composer_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_expand_inner_space", &test_expand_inner_space );
    test_suite_add_test_case( &result, "test_set_envelope_box", &test_set_envelope_box );
    test_suite_add_test_case( &result, "test_set_envelope_box_too_small", &test_set_envelope_box_too_small );
    return result;
}

static cairo_surface_t *surface;
static cairo_t *cr;
static geometry_rectangle_t diagram_bounds;
static PangoLayout *font_layout;

static layout_visible_classifier_t layout_vis_classifier;
static data_visible_classifier_t data_vis_classifier;

static pencil_size_t pencil_size;

static draw_classifier_icon_t draw_classifier_icon;

static void set_up(void)
{
    /* init a pango font layout */
    {
        geometry_rectangle_init( &diagram_bounds, 0.0, 0.0, 640.0, 480.0 );
        surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32,
                                              (uint32_t) geometry_rectangle_get_width( &diagram_bounds ),
                                              (uint32_t) geometry_rectangle_get_height( &diagram_bounds )
                                            );
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( surface ) );
        cr = cairo_create (surface);
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( cr ) );
        font_layout = pango_cairo_create_layout (cr);
    }

    pencil_size_init( &pencil_size,
                      geometry_rectangle_get_width( &diagram_bounds ),
                      geometry_rectangle_get_height( &diagram_bounds )
                    );

    draw_classifier_icon_init( &draw_classifier_icon );

    /* init a layout visible classifier */
    {
        data_classifier_t data_classifier;
        data_diagramelement_t data_diagele;

        data_diagramelement_init( &data_diagele,
                                  17,  /* id */
                                  32,  /* diagram_id */
                                  99,  /* classifier_id */
                                  DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE,
                                  DATA_ROW_ID_VOID
                                );
        data_error_t err = data_classifier_init( &data_classifier,
                                                 99,  /* id */
                                                 DATA_CLASSIFIER_TYPE_CLASS,  /* main_type */
                                                 "stereotype",
                                                 "classifier name",
                                                 "classifier description",
                                                 10000,  /* x_order */
                                                 11000,  /* y_order */
                                                 12000  /* list_order */
                                               );
        TEST_ENVIRONMENT_ASSERT_EQUAL_INT( DATA_ERROR_NONE, err );
        data_visible_classifier_init( &data_vis_classifier, &data_classifier, &data_diagele );
        layout_visible_classifier_init( &layout_vis_classifier, &data_vis_classifier );

        data_diagramelement_destroy( &data_diagele );
        data_classifier_destroy( &data_classifier );
    }
}

static void tear_down(void)
{
    /* destroy the layout visible classifier */
    {
        layout_visible_classifier_destroy( &layout_vis_classifier );
        data_visible_classifier_destroy( &data_vis_classifier );
    }

    draw_classifier_icon_destroy( &draw_classifier_icon );

    pencil_size_destroy( &pencil_size );

    /* destroy the pango font layout */
    {
        g_object_unref (font_layout);
        cairo_destroy (cr);
        cairo_surface_finish ( surface );
        cairo_surface_destroy ( surface );
        geometry_rectangle_destroy( &diagram_bounds );
    }
}

static void test_expand_inner_space(void)
{
    pencil_classifier_composer_t classifier_composer;
    pencil_classifier_composer_init( &classifier_composer );

    const geometry_rectangle_t inner_space = { .left = 100.0, .top = 90.0, .width = 160.0, .height = 80.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        data_classifier_set_main_type( data_visible_classifier_get_classifier_ptr( &data_vis_classifier ), classifier_type );
        /* printf("  type: %d, %d\n", t_idx, classifier_type); */

        for ( unsigned int show_children = 0; show_children <= 1; show_children ++ )
        {
            /* run composing method */
            pencil_classifier_composer_expand_inner_space( &classifier_composer,
                                                           &inner_space,
                                                           (show_children != 0),
                                                           &pencil_size,
                                                           font_layout,
                                                           &layout_vis_classifier
                                                         );

            /* check that all layouted rectangles are outside inner_space */
            const geometry_rectangle_t *const symbol = layout_visible_classifier_get_symbol_box_const( &layout_vis_classifier );
            if ( draw_classifier_icon_is_fix_sized_symbol( &draw_classifier_icon, classifier_type ) )
            {
                /* no intesects if fix-size-symbol */
                TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &inner_space, symbol ) );
            }
            TEST_ASSERT( ! geometry_rectangle_is_empty( symbol ) );
            const geometry_rectangle_t *const label = layout_visible_classifier_get_label_box_const( &layout_vis_classifier );
            TEST_ASSERT_EQUAL_DOUBLE( 0.0, geometry_rectangle_get_intersect_area( &inner_space, label ) );
            TEST_ASSERT( ! geometry_rectangle_is_empty( label ) );
            const geometry_rectangle_t *const space = layout_visible_classifier_get_space_const( &layout_vis_classifier );
            TEST_ASSERT_EQUAL_DOUBLE( 100.0, geometry_rectangle_get_left( space ) );
            TEST_ASSERT_EQUAL_DOUBLE( 90.0, geometry_rectangle_get_top( space ) );
            TEST_ASSERT_EQUAL_DOUBLE( 160.0, geometry_rectangle_get_width( space ) );
            TEST_ASSERT_EQUAL_DOUBLE( 80.0, geometry_rectangle_get_height( space ) );
        }
    }

    pencil_classifier_composer_destroy( &classifier_composer );
}

static void test_set_envelope_box(void)
{
    pencil_classifier_composer_t classifier_composer;
    pencil_classifier_composer_init( &classifier_composer );

    const geometry_rectangle_t envelope = { .left = 50.0, .top = 40.0, .width = 210.0, .height = 180.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        data_classifier_set_main_type( data_visible_classifier_get_classifier_ptr( &data_vis_classifier ), classifier_type );
        /* printf("  type: %d, %d\n", t_idx, classifier_type); */

        for ( unsigned int show_children = 0; show_children <= 1; show_children ++ )
        {
            /* run composing method */
            const int err
                = pencil_classifier_composer_set_envelope_box( &classifier_composer,
                                                               &envelope,
                                                               (show_children != 0),
                                                               &pencil_size,
                                                               font_layout,
                                                               &layout_vis_classifier
                                                             );
            TEST_ASSERT_EQUAL_INT( 0, err );

            /* check that all layouted rectangles are within envelope */
            const geometry_rectangle_t *const symbol = layout_visible_classifier_get_symbol_box_const( &layout_vis_classifier );
            TEST_ASSERT( geometry_rectangle_is_containing( &envelope, symbol ) );
            TEST_ASSERT( ! geometry_rectangle_is_empty( symbol ) );
            const geometry_rectangle_t *const label = layout_visible_classifier_get_label_box_const( &layout_vis_classifier );
            TEST_ASSERT( geometry_rectangle_is_containing( &envelope, label ) );
            TEST_ASSERT( ! geometry_rectangle_is_empty( label ) );
            const geometry_rectangle_t *const space = layout_visible_classifier_get_space_const( &layout_vis_classifier );
            TEST_ASSERT( geometry_rectangle_is_containing( &envelope, space ) );
            TEST_ASSERT( ! geometry_rectangle_is_empty( space ) );
        }
    }

    pencil_classifier_composer_destroy( &classifier_composer );
}

static void test_set_envelope_box_too_small(void)
{
    pencil_classifier_composer_t classifier_composer;
    pencil_classifier_composer_init( &classifier_composer );

    const geometry_rectangle_t small_envelope = { .left = 150.0, .top = 140.0, .width = 15.0, .height = 10.0 };

    for ( unsigned int t_idx = 0; t_idx < DATA_CLASSIFIER_TYPE_COUNT; t_idx ++ )
    {
        data_classifier_type_t classifier_type = DATA_CLASSIFIER_TYPE_ARRAY[ t_idx ];
        data_classifier_set_main_type( data_visible_classifier_get_classifier_ptr( &data_vis_classifier ), classifier_type );
        /* printf("  type: %d, %d\n", t_idx, classifier_type); */

        for ( unsigned int show_children = 0; show_children <= 1; show_children ++ )
        {
            /* run composing method */
            const int err
                = pencil_classifier_composer_set_envelope_box( &classifier_composer,
                                                               &small_envelope,
                                                               (show_children != 0),
                                                               &pencil_size,
                                                               font_layout,
                                                               &layout_vis_classifier
                                                             );
            TEST_ASSERT_EQUAL_INT( 1, err );

            /* check that actual_envelope rectangle is bigger and is within small_envelope */
            const geometry_rectangle_t actual_envelope
                = layout_visible_classifier_calc_envelope_box( &layout_vis_classifier );
            TEST_ASSERT( geometry_rectangle_is_containing( &actual_envelope, &small_envelope ) );
            TEST_ASSERT( ! geometry_rectangle_is_containing( &small_envelope, &actual_envelope ) );

            /* check that label and space are withing symbol (unless fix-sized-symbol) */
            if ( ! draw_classifier_icon_is_fix_sized_symbol( &draw_classifier_icon, classifier_type ) )
            {
                const geometry_rectangle_t *const symbol = layout_visible_classifier_get_symbol_box_const( &layout_vis_classifier );
                const geometry_rectangle_t *const label = layout_visible_classifier_get_label_box_const( &layout_vis_classifier );
                const geometry_rectangle_t *const space = layout_visible_classifier_get_space_const( &layout_vis_classifier );
                TEST_ASSERT( geometry_rectangle_is_containing( symbol, label ) );
                TEST_ASSERT( geometry_rectangle_is_containing( symbol, space ) );
            }
        }
    }

    pencil_classifier_composer_destroy( &classifier_composer );
}


/*
 * Copyright 2021-2021 Andreas Warnke
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
