/* File: data_database_test.c; Copyright and License: see below */

#include "data_database_test.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <errno.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_open_readonly( test_fixture_t *fix );

test_suite_t data_database_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_database",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_open_readonly", &test_open_readonly );
    return result;
}

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char *const DATABASE_FILENAME = "unittest_crystal_facet_uml_default.cfu1";

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    /* remove old database files first */
    int err;
    err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT( ( 0 == err ) || (( -1 == err )&&( errno == ENOENT )) );

    /* fix provides storage memory - nothing is initialized */
    test_fixture_t *fix = &test_fixture;
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
  assert( fix != NULL );
  /* fix is no de-initialized */

  /* remove old database files finally */
  const int stdio_err = remove( DATABASE_FILENAME );
  TEST_ENVIRONMENT_ASSERT( ( 0 == stdio_err ) || (( -1 == stdio_err )&&( errno == ENOENT )) );
}

static test_case_result_t test_open_readonly( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_classifier_iterator_t classifier_iterator;
    data_classifier_t out_classifier;
    bool has_next;

    /* create an empty, exiting database file first */
    data_database_init( &((*fix).database) );
    data_err = data_database_open( &((*fix).database), DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
    data_err = data_database_close( &((*fix).database) );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
    data_database_destroy( &((*fix).database) );

    /* open a database */
    data_database_init( &((*fix).database) );
    data_err = data_database_open_read_only( &((*fix).database), DATABASE_FILENAME );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );

    /* init reader and writer */
    data_err = data_database_reader_init( &((*fix).db_reader), &((*fix).database) );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );
    data_database_writer_init( &((*fix).db_writer), &((*fix).db_reader), &((*fix).database) );

    /* test the iterator, init */
    data_classifier_iterator_init_empty( &classifier_iterator );
    data_err = data_database_reader_get_all_classifiers ( &((*fix).db_reader), true, &classifier_iterator );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );

    /* test the iterator, step on empty set */
    has_next = data_classifier_iterator_has_next( &classifier_iterator );
    TEST_EXPECT( ! has_next );
    data_err = data_classifier_iterator_next( &classifier_iterator, &out_classifier );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_INVALID_REQUEST, data_err, u8_error_get_name );

    /* test the iterator, destroy */
    data_err = data_classifier_iterator_destroy( &classifier_iterator );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_NONE, data_err, u8_error_get_name );

    /* test the writer on a read-only db */
    data_diagram_t empty;
    data_diagram_init_empty( &empty );
    data_err = data_database_writer_create_diagram( &((*fix).db_writer), &empty, NULL /*out_new_id*/ );
    data_diagram_destroy( &empty );
    TEST_EXPECT_EQUAL_ENUM( U8_ERROR_READ_ONLY_DB, data_err, u8_error_get_name );

    /* destroy reader and writer */
    data_database_writer_destroy( &((*fix).db_writer) );
    data_database_reader_destroy( &((*fix).db_reader) );

    /* close the database */
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2026-2026 Andreas Warnke
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
