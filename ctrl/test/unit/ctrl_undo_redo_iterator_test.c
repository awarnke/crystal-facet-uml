/* File: ctrl_undo_redo_iterator_test.c; Copyright and License: see below */

#include "ctrl_undo_redo_iterator_test.h"
#include "ctrl_undo_redo_iterator.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testIterateForward( test_fixture_t *fix );
static test_case_result_t testIterateReverse( test_fixture_t *fix );
static test_case_result_t testEmpty( test_fixture_t *fix );

test_suite_t ctrl_undo_redo_iterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "ctrl_undo_redo_iterator",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testIterateForward", &testIterateForward );
    test_suite_add_test_case( &result, "testIterateReverse", &testIterateReverse );
    test_suite_add_test_case( &result, "testEmpty", &testEmpty );
    return result;
}

#define CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE (3)
struct test_fixture_struct {
    const ctrl_undo_redo_entry_t ring_buf[CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE];
};
typedef struct test_fixture_struct test_fixture_t;
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
}

static test_case_result_t testIterateForward( test_fixture_t *fix )
{
    ctrl_undo_redo_iterator_t iter;
    ctrl_undo_redo_iterator_init( &iter,
                                  &((*fix).ring_buf),
                                  CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE,
                                  true, /* iterate_upwards */
                                  1, /* current */
                                  CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE /* length */
                                );

    bool has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    const ctrl_undo_redo_entry_t *next = ctrl_undo_redo_iterator_next( &iter );
    TEST_EXPECT_EQUAL_PTR( &((*fix).ring_buf[1]), next );

    has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = ctrl_undo_redo_iterator_next( &iter );
    TEST_EXPECT_EQUAL_PTR( &((*fix).ring_buf[2]), next );

    has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = ctrl_undo_redo_iterator_next( &iter );
    TEST_EXPECT_EQUAL_PTR( &((*fix).ring_buf[0]), next );

    has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    ctrl_undo_redo_iterator_destroy( &iter );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIterateReverse( test_fixture_t *fix )
{
    ctrl_undo_redo_iterator_t iter;
    ctrl_undo_redo_iterator_init( &iter,
                                  &((*fix).ring_buf),
                                  CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE,
                                  false, /* iterate_upwards */
                                  1, /* current */
                                  CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE /* length */
                                );

    const ctrl_undo_redo_entry_t *next = ctrl_undo_redo_iterator_next( &iter );
    TEST_EXPECT_EQUAL_PTR( &((*fix).ring_buf[1]), next );

    bool has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = ctrl_undo_redo_iterator_next( &iter );
    TEST_EXPECT_EQUAL_PTR( &((*fix).ring_buf[0]), next );

    next = ctrl_undo_redo_iterator_next( &iter );
    TEST_EXPECT_EQUAL_PTR( &((*fix).ring_buf[2]), next );

    has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    ctrl_undo_redo_iterator_reinit( &iter,
                                    &((*fix).ring_buf),
                                    CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE,
                                    false, /* iterate_upwards */
                                    1, /* current */
                                    CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE /* length */
                                  );

    has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    ctrl_undo_redo_iterator_destroy( &iter );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmpty( test_fixture_t *fix )
{
    ctrl_undo_redo_iterator_t iter;
    ctrl_undo_redo_iterator_init_empty( &iter );

    bool has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUF_FALSE, has_next );

    ctrl_undo_redo_iterator_destroy( &iter );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2024 Andreas Warnke
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
