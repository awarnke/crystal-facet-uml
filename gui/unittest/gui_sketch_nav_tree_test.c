/* File: gui_sketch_nav_tree_test.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_nav_tree.h"
#include "gui_resources.h"
#include "test_assert.h"
#include "test_suite.h"

static void set_up(void);
static void tear_down(void);
static void test_get_object_at_pos(void);

test_suite_t gui_sketch_nav_tree_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "gui_sketch_nav_tree_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_get_object_at_pos", &test_get_object_at_pos );
    return result;
}

gui_resources_t res;

static void set_up(void)
{
    gui_resources_init ( &res );
}

static void tear_down(void)
{
    gui_resources_destroy ( &res );
}

static void test_get_object_at_pos(void)
{
    gui_sketch_nav_tree_t testee;
    
    gui_sketch_nav_tree_init( &testee, &res );
    
    TEST_ASSERT_EQUAL_INT( 0, 0 );

#if 0    
    static inline void gui_sketch_nav_tree_get_button_at_pos ( gui_sketch_nav_tree_t *this_,
                                                           int32_t x,
                                                           int32_t y,
                                                           gui_sketch_action_t *out_action_id
                                                         );

void gui_sketch_nav_tree_get_object_id_at_pos ( gui_sketch_nav_tree_t *this_,
                                                int32_t x,
                                                int32_t y,
                                                data_id_t *out_selected_id
                                              );

gui_error_t gui_sketch_nav_tree_get_gap_info_at_pos ( gui_sketch_nav_tree_t *this_,
                                                      int32_t x,
                                                      int32_t y,
                                                      data_id_t *out_parent_id,
                                                      int32_t *out_list_order,
                                                      shape_int_rectangle_t *out_gap_line
                                                    );

#endif
    
    
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
