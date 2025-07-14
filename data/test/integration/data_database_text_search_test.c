/* File: data_database_text_search_test.c; Copyright and License: see below */

#include "data_database_text_search_test.h"
#include "storage/data_database_text_search.h"
#include "storage/data_database.h"
#include "storage/data_search_result_iterator.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include "tvec/tvec_add.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t no_results( test_fixture_t *fix );
static test_case_result_t search_no_filter( test_fixture_t *fix );

test_suite_t data_database_text_search_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_database_text_search_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "no_results", &no_results );
    test_suite_add_test_case( &result, "search_no_filter", &search_no_filter );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
    data_visible_set_t loaded_elements;  /*!< database loader to fetch all elements visible in a diagram */
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

    /* v--- creating the test vector */
    tvec_add_t setup_env;
    tvec_add_init( &setup_env, &((*fix).db_writer) );

    /* create the root diagram */
    const data_row_t root_diag_id
    = tvec_add_diagram( &setup_env, DATA_ROW_VOID, "root AAA&_\\\xef\xbc\x8eZZZ", "Any-Blue-Item" );

    /* create a child diagram */
    const data_row_t child_diag_id
    = tvec_add_diagram( &setup_env, root_diag_id, "child A\xef\xbc\x8eZ", "Any-Green-Item" );
    (void) child_diag_id;  /* unused variable */

    /* create a classifier and a feature */
    const data_row_t classifier_id
    = tvec_add_classifier( &setup_env,
                           "The-Blue-Stone",  /* name */
                           DATA_CLASSIFIER_TYPE_COMPONENT,
                           "Any-Blue-Item &_\\\xef\xbc\x8e"  /* stereotype */
    );
    const data_row_t feature_id
    = tvec_add_feature( &setup_env,
                        classifier_id,
                        "The-Blue-Stone Feature",  /* name */
                        "AAA&_\\\xef\xbc\x8eZZZ"
    );
    (void) tvec_add_diagramelement( &setup_env, root_diag_id, classifier_id );

    /* create 1 relationship */
    const data_row_t relation_id
    = tvec_add_relationship( &setup_env,
                             classifier_id,
                             feature_id,
                             classifier_id,
                             DATA_ROW_VOID,
                             DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
                             "feature-to-classifier",  /* name */
                             "AAA&_\\\xef\xbc\x8eZZZ"
    );
    (void) relation_id;  /* unused variable */

    tvec_add_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

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

static test_case_result_t no_results( test_fixture_t *fix )
{
    assert( fix != NULL );

    data_database_text_search_t txt_src;
    const u8_error_t text_init_err = data_database_text_search_init( &txt_src, &((*fix).database) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, text_init_err );

    data_search_result_iterator_t data_search_result_iterator;
    const u8_error_t init_err = data_search_result_iterator_init_empty( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, init_err );

    const u8_error_t src_err
    = data_database_text_search_get_objects_by_text_fragment( &txt_src,
                                                              "\\\xef\xbc\x8e&_",
                                                              &data_search_result_iterator
    );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, src_err );

    const bool next_0 = data_search_result_iterator_has_next( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( false, next_0 );

    const u8_error_t destr_err = data_search_result_iterator_destroy( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, destr_err );

    const u8_error_t text_destr_err = data_database_text_search_destroy( &txt_src );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, text_destr_err );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t search_no_filter( test_fixture_t *fix )
{
    assert( fix != NULL );

    data_database_text_search_t txt_src;
    const u8_error_t text_init_err = data_database_text_search_init( &txt_src, &((*fix).database) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, text_init_err );

    data_search_result_iterator_t data_search_result_iterator;
    const u8_error_t init_err = data_search_result_iterator_init_empty( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, init_err );

    const u8_error_t src_err
        = data_database_text_search_get_objects_by_text_fragment( &txt_src,
                                                                  "&_\\\xef\xbc\x8e",
                                                                  &data_search_result_iterator
                                                                );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, src_err );

    const bool next_0 = data_search_result_iterator_has_next( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( true, next_0 );

    data_search_result_t current_search_result;
    const u8_error_t load_err_0
        = data_search_result_iterator_next( &data_search_result_iterator, &current_search_result );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, load_err_0 );
    const int eq_0 = utf8string_equals_str( "root AAA&_\\\xef\xbc\x8eZZZ",
                                            data_search_result_get_match_name_const( &current_search_result )
                                          );
    TEST_EXPECT_EQUAL_INT( 1, eq_0 );

    const bool next_1 = data_search_result_iterator_has_next( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( true, next_1 );

    const u8_error_t load_err_1
        = data_search_result_iterator_next( &data_search_result_iterator, &current_search_result );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, load_err_1 );
    const int eq_1 = utf8string_equals_str( "The-Blue-Stone",
                                            data_search_result_get_match_name_const( &current_search_result )
                                          );
    TEST_EXPECT_EQUAL_INT( 1, eq_1 );

    const bool next_2 = data_search_result_iterator_has_next( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( true, next_2 );

    const u8_error_t load_err_2
        = data_search_result_iterator_next( &data_search_result_iterator, &current_search_result );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, load_err_2 );
    const int eq_2 = utf8string_equals_str( "The-Blue-Stone Feature",
                                            data_search_result_get_match_name_const( &current_search_result )
                                          );
    TEST_EXPECT_EQUAL_INT( 1, eq_2 );

    const bool next_3 = data_search_result_iterator_has_next( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( true, next_3 );

    const u8_error_t load_err_3
        = data_search_result_iterator_next( &data_search_result_iterator, &current_search_result );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, load_err_3 );
    const int eq_3 = utf8string_equals_str( "feature-to-classifier",
                                            data_search_result_get_match_name_const( &current_search_result )
                                          );
    TEST_EXPECT_EQUAL_INT( 1, eq_3 );

    const bool next_4 = data_search_result_iterator_has_next( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( false, next_4 );

    const u8_error_t destr_err = data_search_result_iterator_destroy( &data_search_result_iterator );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, destr_err );

    const u8_error_t text_destr_err = data_database_text_search_destroy( &txt_src );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, text_destr_err );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2025-2025 Andreas Warnke
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
