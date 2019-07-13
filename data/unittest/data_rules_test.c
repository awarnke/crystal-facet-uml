/* File: data_rules_test.c; Copyright and License: see below */

#include "data_rules_test.h"
#include "set/data_small_set.h"
#include "test_assert.h"

#define PROG_ASSERT(cond) if (!(cond)) {exit(-1);}
/* use TEST_ASSERT_* to check and report the test case result, use PROG_ASSERT() if the testing program could not be executed as expected */

static void set_up(void);
static void tear_down(void);
static void test_data_rules_filter_scenarios(void);
static void test_data_rules_filter_box_and_list(void);
static void test_data_rules_filter_standard(void);

test_suite_t data_rules_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_rules_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_data_rules_filter_scenarios", &test_data_rules_filter_scenarios );
    test_suite_add_test_case( &result, "test_data_rules_filter_box_and_list", &test_data_rules_filter_box_and_list );
    test_suite_add_test_case( &result, "test_data_rules_filter_standard", &test_data_rules_filter_standard );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_data_rules_filter_scenarios(void)
{
    data_small_set_t my_set;
    data_error_t d_err;

    data_small_set_init ( &my_set );


    data_small_set_destroy ( &my_set );
}

static void test_data_rules_filter_box_and_list(void)
{
    data_small_set_t my_set;
    data_error_t d_err;

    data_small_set_init ( &my_set );


    data_small_set_destroy ( &my_set );
}

static void test_data_rules_filter_standard(void)
{
    data_small_set_t my_set;
    data_error_t d_err;

    data_small_set_init ( &my_set );


    data_small_set_destroy ( &my_set );
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
