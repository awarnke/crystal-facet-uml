/* File: gui_sketch_nav_tree_test.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_nav_tree.h"
#include "util/shape/shape_int_rectangle.h"
#include "gui_resources.h"
#include "test_assert.h"
#include "test_suite.h"

static void set_up(void);
static void tear_down(void);
static void test_get_object_at_pos_on_no_diagram(void); /* extreme case */
static void test_get_object_at_pos_on_single_diagram(void); /* no parent, no siblings, no children */
static void test_get_object_at_pos_on_1parent_1child_diagram(void); /* standard case, but no siblings */
static void test_get_object_at_pos_on_2parent_2siblings_diagram(void); /* standard case, but no children */

test_suite_t gui_sketch_nav_tree_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "gui_sketch_nav_tree_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_no_diagram", &test_get_object_at_pos_on_no_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_single_diagram", &test_get_object_at_pos_on_single_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_1parent_1child_diagram", &test_get_object_at_pos_on_1parent_1child_diagram );
    test_suite_add_test_case( &result, "test_get_object_at_pos_on_2parent_2siblings_diagram", &test_get_object_at_pos_on_2parent_2siblings_diagram );
    return result;
}

gui_resources_t res;
shape_int_rectangle_t bounds;
static const int LEFT = 100;
static const int TOP = 200;
static const int WIDTH = 50;
static const int HEIGHT = 150;
static const int FIRST_LINE = 1;  /* this is a nav tree property: the first line is empty */

static void set_up(void)
{
    gui_resources_init ( &res );
    shape_int_rectangle_init( &bounds, LEFT, TOP, WIDTH, HEIGHT );
}

static void tear_down(void)
{
    shape_int_rectangle_destroy( &bounds );
    gui_resources_destroy ( &res );
}

static void test_get_object_at_pos_on_no_diagram(void)
{
    /* init the testee */
    gui_sketch_nav_tree_t testee;
    gui_sketch_nav_tree_init( &testee, &res );
    gui_sketch_nav_tree_set_bounds( &testee, bounds );
    gui_sketch_nav_tree_private_do_layout( &testee );

    /* test button type */
    gui_sketch_action_t action_id;
    const static int button_dy = FIRST_LINE*GUI_SKETCH_NAV_TREE_LINE_HEIGHT;
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT, TOP+button_dy, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM );
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT+1, TOP+button_dy-1, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT-1, TOP+button_dy, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT+WIDTH, TOP+button_dy, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT+1, TOP+button_dy+GUI_SKETCH_NAV_TREE_LINE_HEIGHT, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    /* test object id */
    data_id_t selected_id;
    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, LEFT, TOP+GUI_SKETCH_NAV_TREE_LINE_HEIGHT, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;
    
    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, LEFT, TOP+GUI_SKETCH_NAV_TREE_LINE_HEIGHT, &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( g_err, GUI_ERROR_OUT_OF_BOUNDS );

    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, LEFT, TOP+(2*GUI_SKETCH_NAV_TREE_LINE_HEIGHT), &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( g_err, GUI_ERROR_OUT_OF_BOUNDS );
    
#if 0    
    GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM = 2,
    GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM = 3,

    GUI_ERROR_NONE = 0,  /*!< 0: success */
    GUI_ERROR_OUT_OF_BOUNDS = 1,  /*!< 1: input parameters are out of bounds */
    GUI_ERROR_UNKNOWN_OBJECT = 2,  /*!< 2: referenced object is not in current diagram */


#endif
    
    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}

static void test_get_object_at_pos_on_single_diagram(void)
{
    /* init the testee */
    gui_sketch_nav_tree_t testee;
    gui_sketch_nav_tree_init( &testee, &res );
    gui_sketch_nav_tree_set_bounds( &testee, bounds );
    gui_sketch_nav_tree_private_do_layout( &testee );

    /* test button type */
    gui_sketch_action_t action_id;
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT, TOP, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM );
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT-1, TOP, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT+1, TOP+GUI_SKETCH_NAV_TREE_LINE_HEIGHT, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    /* test object id */
    data_id_t selected_id;
    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, LEFT, TOP, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;
    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, LEFT, TOP, &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( g_err, GUI_ERROR_NONE );
    TEST_ASSERT( ! data_id_is_valid(&parent_id) );
    TEST_ASSERT_EQUAL_INT( list_order, 0 );
    
#if 0    
    GUI_SKETCH_ACTION_NEW_CHILD_DIAGRAM = 2,
    GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM = 3,

    GUI_ERROR_NONE = 0,  /*!< 0: success */
    GUI_ERROR_OUT_OF_BOUNDS = 1,  /*!< 1: input parameters are out of bounds */
    GUI_ERROR_UNKNOWN_OBJECT = 2,  /*!< 2: referenced object is not in current diagram */


#endif
    
    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}

static void test_get_object_at_pos_on_1parent_1child_diagram(void)
{
    /* init the testee */
    gui_sketch_nav_tree_t testee;
    gui_sketch_nav_tree_init( &testee, &res );
    gui_sketch_nav_tree_set_bounds( &testee, bounds );
    gui_sketch_nav_tree_private_do_layout( &testee );

    /* test button type */
    gui_sketch_action_t action_id;
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT, TOP, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM );
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT-1, TOP, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT+1, TOP+GUI_SKETCH_NAV_TREE_LINE_HEIGHT, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    /* test object id */
    data_id_t selected_id;
    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, LEFT, TOP, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;
    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, LEFT, TOP, &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( g_err, GUI_ERROR_NONE );
    TEST_ASSERT( ! data_id_is_valid(&parent_id) );
    TEST_ASSERT_EQUAL_INT( list_order, 0 );
    
    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}

static void test_get_object_at_pos_on_2parent_2siblings_diagram(void)
{
    /* init the testee */
    gui_sketch_nav_tree_t testee;
    gui_sketch_nav_tree_init( &testee, &res );
    gui_sketch_nav_tree_set_bounds( &testee, bounds );
    gui_sketch_nav_tree_private_do_layout( &testee );

    /* test button type */
    gui_sketch_action_t action_id;
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT, TOP, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NEW_ROOT_DIAGRAM );
    
    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT-1, TOP, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    gui_sketch_nav_tree_get_button_at_pos ( &testee, LEFT+1, TOP+GUI_SKETCH_NAV_TREE_LINE_HEIGHT, &action_id );
    TEST_ASSERT_EQUAL_INT( action_id, GUI_SKETCH_ACTION_NONE );

    /* test object id */
    data_id_t selected_id;
    gui_sketch_nav_tree_get_object_id_at_pos ( &testee, LEFT, TOP, &selected_id );
    TEST_ASSERT( ! data_id_is_valid(&selected_id) );

    /* test gap info */
    gui_error_t g_err;
    data_id_t parent_id;
    int32_t list_order;
    shape_int_rectangle_t gap_line;
    g_err = gui_sketch_nav_tree_get_gap_info_at_pos ( &testee, LEFT, TOP, &parent_id, &list_order, &gap_line );
    TEST_ASSERT_EQUAL_INT( g_err, GUI_ERROR_NONE );
    TEST_ASSERT( ! data_id_is_valid(&parent_id) );
    TEST_ASSERT_EQUAL_INT( list_order, 0 );
    
    /* destroy the testee */
    gui_sketch_nav_tree_destroy( &testee );
}


/*
 * Copyright 2019-2019 Andreas Warnke
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
