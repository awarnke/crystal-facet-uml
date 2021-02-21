/* File: pencil_layouter_test.c; Copyright and License: see below */

#include "pencil_layouter_test.h"
#include "pencil_layouter.h"
#include "trace.h"
#include "test_assert.h"
        
static void set_up(void);
static void tear_down(void);
static void iterate_mini_models(void);

test_suite_t pencil_layouter_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_layouter_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "iterate_mini_models", &iterate_mini_models );
    return result;
}

static data_visible_set_t data_set;
static pencil_layouter_t layouter;
static cairo_surface_t *surface;
static cairo_t *cr;
geometry_rectangle_t diagram_bounds;
PangoLayout *font_layout;

static void set_up(void)
{
    data_visible_set_init( &data_set );
    pencil_layouter_init( &layouter, &data_set );
    geometry_rectangle_init( &diagram_bounds, 0.0, 0.0, 320.0, 240.0 );
    surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, 
                                          (uint32_t) geometry_rectangle_get_width( &diagram_bounds ),
                                          (uint32_t) geometry_rectangle_get_height( &diagram_bounds )
                                        );
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( surface ) );
    cr = cairo_create (surface);
    TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( cr ) );
    font_layout = pango_cairo_create_layout (cr);
}

static void tear_down(void)
{
    g_object_unref (font_layout);
    cairo_destroy (cr);
    cairo_surface_finish ( surface );
    cairo_surface_destroy ( surface );
    geometry_rectangle_destroy( &diagram_bounds );
    pencil_layouter_destroy( &layouter );
    data_visible_set_destroy( &data_set );
}
    
static void iterate_mini_models(void)
{
    /* setup */
    
    /* perform test */
    pencil_layouter_prepare ( &layouter );
    pencil_layouter_define_grid ( &layouter, diagram_bounds );
    pencil_layouter_layout_elements ( &layouter, font_layout );
    
    /* check result */
    
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
