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
static test_case_result_t test_iterate_forward( test_fixture_t *fix );
static test_case_result_t test_iterate_reverse( test_fixture_t *fix );
static test_case_result_t test_empty( test_fixture_t *fix );
static test_case_result_t test_statistics( test_fixture_t *fix );

test_suite_t ctrl_undo_redo_iterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "ctrl_undo_redo_iterator",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_iterate_forward", &test_iterate_forward );
    test_suite_add_test_case( &result, "test_iterate_reverse", &test_iterate_reverse );
    test_suite_add_test_case( &result, "test_empty", &test_empty );
    test_suite_add_test_case( &result, "test_statistics", &test_statistics );
    return result;
}

#define CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE (3)
struct test_fixture_struct {
    const ctrl_undo_redo_entry_t ring_buf[CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE];
};
typedef struct test_fixture_struct test_fixture_t;
static test_fixture_t test_fixture = { .ring_buf = {
    [0]={.action_type=CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM},
    [1]={.action_type=CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT},
    [2]={.action_type=CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER}
}};

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
}

static test_case_result_t test_iterate_forward( test_fixture_t *fix )
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

static test_case_result_t test_iterate_reverse( test_fixture_t *fix )
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

static test_case_result_t test_empty( test_fixture_t *fix )
{
    ctrl_undo_redo_iterator_t iter;
    ctrl_undo_redo_iterator_init_empty( &iter );

    bool has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUF_FALSE, has_next );

    ctrl_undo_redo_iterator_destroy( &iter );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_statistics( test_fixture_t *fix )
{
    ctrl_undo_redo_iterator_t iter;
    ctrl_undo_redo_iterator_init( &iter,
                                  &((*fix).ring_buf),
                                  CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE,
                                  false, /* iterate_upwards */
                                  0, /* current */
                                  CTRL_UNDO_REDO_ITERATOR_TEST_BUF_SIZE /* length */
                                );

    data_stat_t stat;
    data_stat_init( &stat );

    ctrl_undo_redo_iterator_collect_statistics( &iter, true /*categorize as undo*/, &stat );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ) );
    TEST_EXPECT_EQUAL_INT( 3, data_stat_get_total_count( &stat ) );

    bool has_next = ctrl_undo_redo_iterator_has_next( &iter );
    TEST_EXPECT_EQUAL_INT( UTF8STRINGBUF_FALSE, has_next );

    data_stat_destroy( &stat );

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
