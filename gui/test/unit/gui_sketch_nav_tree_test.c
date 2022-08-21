/* File: gui_sketch_nav_tree_test.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_nav_tree.h"
#include "shape/shape_int_rectangle.h"
#include "gui_resources.h"
#include "data_diagram.h"
#include "test_assert.h"
#include "test_suite.h"

static void set_up(void);
static void tear_down(void);
static void test_get_object_at_pos_on_no_diagram(void); /* extreme case */
static void test_get_object_at_pos_on_single_diagram(void); /* no parent, no siblings, no children */
static void test_get_object_at_pos_on_1parent_1child_diagram(void); /* standard case, but no siblings */
static void test_get_object_at_pos_on_2parent_2siblings_diagram(void); /* standard case, but no children */

test_suite_t gui_sketch_nav_tree_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "gui_sketch_nav_tree_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_no_diagram", &test_get_object_at_pos_on_no_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_single_diagram", &test_get_object_at_pos_on_single_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_1parent_1child_diagram", &test_get_object_at_pos_on_1parent_1child_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_2parent_2siblings_diagram", &test_get_object_at_pos_on_2parent_2siblings_diagram );
    return result;
}

gui_resources_t res;
shape_int_rectangle_t bounds;
static const int LEFT = 100;
static const int TOP = 57;
static const int WIDTH = 50;
static const int HEIGHT = 400;
data_diagram_t self;
data_diagram_t parent;
data_diagram_t child;
data_diagram_t other_child;

/* cairo and pango objects */
static cairo_surface_t *surface;
static cairo_t *cr;
static PangoLayout *font_layout;

static void set_up(void)
{
    /* init cairo and pango objects */
    {
        surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, 640, 480 );
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_surface_status( surface ) );
        cr = cairo_create (surface);
        TEST_ENVIRONMENT_ASSERT( CAIRO_STATUS_SUCCESS == cairo_status( cr ) );
        font_layout = pango_cairo_create_layout (cr);
    }

    gui_resources_init ( &res );
    shape_int_rectangle_init( &bounds, LEFT, TOP, WIDTH, HEIGHT );
    data_diagram_init_empty( &parent );
    data_diagram_set_row_id( &parent, 1000 );
    data_diagram_init_empty( &self );
    data_diagram_set_row_id( &self, 1001 );
    data_diagram_set_parent_row_id( &self, 1000 );
    data_diagram_init_empty( &child );
    data_diagram_set_row_id( &child, 1002 );
    data_diagram_set_parent_row_id( &child, 1001 );
    data_diagram_init_empty( &other_child );
    data_diagram_set_row_id( &other_child, 1003 );
    data_diagram_set_parent_row_id( &other_child, 1001 );
}

static void tear_down(void)
{
    data_diagram_destroy( &parent );
    data_diagram_destroy( &self );
    data_diagram_destroy( &child );
    data_diagram_destroy( &other_child );
    shape_int_rectangle_destroy( &bounds );
    gui_resources_destroy ( &res );

    /* destroy cairo and pango objects */
    {
        g_object_unref (font_layout);
        cairo_destroy (cr);
        cairo_surface_finish ( surface );
        cairo_surface_destroy ( surface );
    }
}

void get_node_coords( const gui_sketch_nav_tree_t *testee, uint32_t idx, int32_t *out_x, int32_t *out_y )
{
    const uint32_t cnt = gui_sketch_nav_tree_get_node_count( testee );
    TEST_ASSERT( idx < cnt );
    const pos_nav_tree_node_t *pos = gui_sketch_nav_tree_get_node_pos_const( testee, idx );
    const shape_int_rectangle_t *pos_icon = pos_nav_tree_node_get_icon_box_const( pos );
    *out_x = shape_int_rectangle_get_left(pos_icon) + 1;
    *out_y = shape_int_rectangle_get_top(pos_icon) + 1;
    /* printf("x:%d,y:%d\n",*out_x,*out_y); */
}

static void test_get_object_at_pos_on_no_diagram(void)
{
    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &res );
        gui_sketch_nav_tree_set_bounds( &testee, bounds );
        gui_sketch_nav_tree_do_layout( &testee, cr );
    }

    /* get position */
    int32_t x;
    int32_t y;
    get_node_coords( &testee, 0, &x, &y );
    TEST_ASSERT_EQUAL_INT( 1, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x, y, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x, (y-2), &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT-1, y, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    /* test object id */
    data_id_t selected_id;
    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x, y, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x, y, &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_OUT_OF_BOUNDS, g_err );

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}

static void test_get_object_at_pos_on_single_diagram(void)
{
    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &res );
        gui_sketch_nav_tree_set_bounds( &testee, bounds );
        {
            testee.ancestors_count = 1;
            testee.ancestor_diagrams[0] = parent;
            testee.siblings_count = 1;
            testee.sibling_diagrams[0] = parent;
            testee.siblings_self_index = 0;
        }
        gui_sketch_nav_tree_do_layout( &testee, cr );
    }

    /* node positions */
    int32_t x0;
    int32_t y0;
    get_node_coords( &testee, 0, &x0, &y0 );
    int32_t x1;
    int32_t y1;
    get_node_coords( &testee, 1, &x1, &y1 );
    TEST_ASSERT_EQUAL_INT( 2, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x0, y0, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x1, y1, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM, action_id );

    /* test object id */
    data_id_t selected_id;

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x0, y0, &selected_id );
    TEST_ASSERT( data_id_is_valid(&selected_id) );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x1, y1, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x0, (y0-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT( ! data_id_is_valid(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, (y1-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT( data_id_is_valid(&parent_id) );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, y1+(y1-y0), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT( data_id_is_valid(&parent_id) );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );  /* same gap as above */

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}

static void test_get_object_at_pos_on_1parent_1child_diagram(void)
{
    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &res );
        gui_sketch_nav_tree_set_bounds( &testee, bounds );
        {
            testee.ancestors_count = 2;
            testee.ancestor_diagrams[0] = self;
            testee.ancestor_diagrams[1] = parent;
            testee.siblings_count = 1;
            testee.sibling_diagrams[0] = self;
            testee.siblings_self_index = 0;
            testee.children_count = 1;
            testee.child_diagrams[0] = child;
        }
        gui_sketch_nav_tree_do_layout( &testee, cr );
    }

    /* node positions */
    int32_t x0;
    int32_t y0;
    get_node_coords( &testee, 0, &x0, &y0 );
    int32_t x1;
    int32_t y1;
    get_node_coords( &testee, 1, &x1, &y1 );
    int32_t x2;
    int32_t y2;
    get_node_coords( &testee, 2, &x2, &y2 );
    int32_t x3;
    int32_t y3;
    get_node_coords( &testee, 3, &x3, &y3 );
    int32_t x4;
    int32_t y4;
    get_node_coords( &testee, 4, &x4, &y4 );
    TEST_ASSERT_EQUAL_INT( 5, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x0, y0, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x1, y1, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x2, y2, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x3, y3, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x4, y4, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM, action_id );

    /* test object id */
    data_id_t selected_id;

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x0, y0, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x1, y1, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x2, y2, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1002, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x3, y3, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x4, y4, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x0, (y0-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT( ! data_id_is_valid(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, (y1-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x2, (y2-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x3, (y3-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x4, (y4-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x4, y4+(y4-y3), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );  /* same gap as above */

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}

static void test_get_object_at_pos_on_2parent_2siblings_diagram(void)
{
    /* init the testee, because a gui_sketch_nav_tree_t contains some hundred diagrams, each abobe 10kB */
    static gui_sketch_nav_tree_t testee;
    {
        gui_sketch_nav_tree_init( &testee, &res );
        gui_sketch_nav_tree_set_bounds( &testee, bounds );
        {
            testee.ancestors_count = 3;
            testee.ancestor_diagrams[0] = child;
            testee.ancestor_diagrams[1] = self;
            testee.ancestor_diagrams[2] = parent;
            testee.siblings_count = 3;
            testee.sibling_diagrams[0] = other_child;
            testee.sibling_diagrams[1] = child;
            testee.sibling_diagrams[2] = other_child;
            testee.siblings_self_index = 1;
        }
        gui_sketch_nav_tree_do_layout( &testee, cr );
    }

    /* node positions */
    int32_t x0;
    int32_t y0;
    get_node_coords( &testee, 0, &x0, &y0 );
    int32_t x1;
    int32_t y1;
    get_node_coords( &testee, 1, &x1, &y1 );
    int32_t x2;
    int32_t y2;
    get_node_coords( &testee, 2, &x2, &y2 );
    int32_t x3;
    int32_t y3;
    get_node_coords( &testee, 3, &x3, &y3 );
    int32_t x4;
    int32_t y4;
    get_node_coords( &testee, 4, &x4, &y4 );
    int32_t x5;
    int32_t y5;
    get_node_coords( &testee, 5, &x5, &y5 );
    int32_t x6;
    int32_t y6;
    get_node_coords( &testee, 6, &x6, &y6 );
    TEST_ASSERT_EQUAL_INT( 7, gui_sketch_nav_tree_get_node_count( &testee ) );

    /* test button type */
    gui_sketch_action_t action_id;

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x0, y0, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x1, y1, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x2, y2, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x3, y3, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x4, y4, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x5, y5, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NONE, action_id );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, x6, y6, &action_id );
    TEST_ASSERT_EQUAL_INT( GUI_SKETCH_ACTION_NEW_SIBLING_DIAGRAM, action_id );

    /* test object id */
    data_id_t selected_id;

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x0, y0, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x1, y1, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x2, y2, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1003, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x3, y3, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1002, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x4, y4, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x5, y5, &selected_id );
    TEST_ASSERT_EQUAL_INT( 1003, data_id_get_row_id(&selected_id) );

    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, x6, y6, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x0, (y0-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT( ! data_id_is_valid(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x1, (y1-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1000, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x2, (y2-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x3, (y3-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x4, (y4-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1002, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x5, (y5-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x6, (y6-2), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, x6, y6+(y6-y5), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( GUI_ERROR_NONE, g_err );
    TEST_ASSERT_EQUAL_INT( 1001, data_id_get_row_id(&parent_id) );  /* same gap as above */

    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}


/*
 * Copyright 2019-2022 Andreas Warnke
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
