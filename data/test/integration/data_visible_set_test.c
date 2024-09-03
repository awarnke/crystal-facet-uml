/* File: data_visible_set_test.c; Copyright and License: see below */

#include "data_visible_set_test.h"
#include "set/data_visible_set.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_small_set_add_and_remove( test_fixture_t *test_env );
static test_case_result_t test_small_set_full( test_fixture_t *test_env );
static test_case_result_t test_small_set_clear( test_fixture_t *test_env );

test_suite_t data_visible_set_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_visible_set_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_small_set_add_and_remove", &test_small_set_add_and_remove );
    test_suite_add_test_case( &result, "test_small_set_full", &test_small_set_full );
    test_suite_add_test_case( &result, "test_small_set_clear", &test_small_set_clear );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
    data_visible_set_t test_me;  /*!< memory for the object to test */
};
typedef struct test_fixture_struct test_fixture_t;  /* big data structure */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    data_database_init( &((*fix).database) );
    data_database_open_in_memory( &((*fix).database) );
    data_database_reader_init( &((*fix).db_reader), &((*fix).database) );
    data_database_writer_init( &((*fix).db_writer), &((*fix).db_reader), &((*fix).database) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_database_writer_destroy( &((*fix).db_writer) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static test_case_result_t test_small_set_add_and_remove( test_fixture_t *test_env )
{
    assert( fix != NULL );
    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t test_small_set_full( test_fixture_t *test_env )
{
    assert( fix != NULL );
    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t test_small_set_clear( test_fixture_t *test_env )
{
    assert( fix != NULL );
    return TEST_CASE_RESULT_ERR;
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
