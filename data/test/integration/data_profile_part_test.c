/* File: data_profile_part_test.c; Copyright and License: see below */

#include "data_profile_part_test.h"
#include "set/data_profile_part.h"
#include "set/data_visible_set.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include "test_vector/test_vector_db.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t no_results( test_fixture_t *fix );
static test_case_result_t search_and_filter( test_fixture_t *fix );
static test_case_result_t too_much_input( test_fixture_t *fix );

test_suite_t data_profile_part_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_profile_part_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "no_results", &no_results );
    test_suite_add_test_case( &result, "search_and_filter", &search_and_filter );
    test_suite_add_test_case( &result, "too_much_input", &too_much_input );
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

    /* v--- creating the test vector */
    test_vector_db_t setup_env;
    test_vector_db_init( &setup_env, &((*fix).db_writer) );

    /* create the root diagram */
    const data_row_id_t root_diag_id
        = test_vector_db_create_diagram( &setup_env,
                                         DATA_ROW_ID_VOID,  /* parent_diagram_id */
                                         "root_diagram",  /* name */
                                         "Any-Blue-Item"  /* stereotype */
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    /* create a classifier with stereotype which name exists but is no stereotype */
    const data_row_id_t classifier_1_id
        = test_vector_db_create_classifier( &setup_env,
                                            "The-Blue-Stone",  /* name */
                                            DATA_CLASSIFIER_TYPE_COMPONENT,
                                            "The-Blue-Stone"  /* stereotype */
                                          );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_1_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_1_id
                                               );

    /* create a classifier with stereotype which does not exist: "ThE" is not "The" */
    const data_row_id_t classifier_2_id
        = test_vector_db_create_classifier( &setup_env,
                                            "non_existing_stereotype",  /* name */
                                            DATA_CLASSIFIER_TYPE_COMPONENT,
                                            "ThE-Blue-Stone"  /* stereotype */
                                          );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_2_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_2_id
                                               );

    test_vector_db_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_init( &((*fix).loaded_elements) );

        const u8_error_t init_err = data_visible_set_load( &((*fix).loaded_elements), root_diag_id, &((*fix).db_reader) );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == init_err );
        const uint32_t diag_classifier_count
            = data_visible_set_get_visible_classifier_count( &((*fix).loaded_elements) );
        TEST_ENVIRONMENT_ASSERT( 2 == diag_classifier_count );

        /* load a profile */
        {
            data_profile_part_t profile;
            data_profile_part_init( &profile );

            const u8_error_t fetch_err = data_profile_part_load( &profile, &((*fix).loaded_elements), &((*fix).db_reader) );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, fetch_err );

            const uint32_t count = data_profile_part_get_stereotype_count( &profile );
            TEST_EXPECT_EQUAL_INT( 0, count );

            data_profile_part_destroy( &profile );
        }

        data_visible_set_destroy( &((*fix).loaded_elements) );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t search_and_filter( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* v--- creating the test vector */
    test_vector_db_t setup_env;
    test_vector_db_init( &setup_env, &((*fix).db_writer) );

    /* create the root diagram */
    const data_row_id_t root_diag_id
        = test_vector_db_create_diagram( &setup_env,
                                         DATA_ROW_ID_VOID,  /* parent_diagram_id */
                                         "root_diagram",  /* name */
                                         "Any-Blue-Item"  /* stereotype */
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    /* create a stereotype (which references itself as stereotype) */
    const data_row_id_t stereotype_id
        = test_vector_db_create_classifier( &setup_env,
                                            "Any-Blue-Item",  /* name */
                                            DATA_CLASSIFIER_TYPE_STEREOTYPE,
                                            "Any-Blue-Item"  /* stereotype */
                                          );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != stereotype_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 stereotype_id
                                               );

    /* create a classifier */
    const data_row_id_t classifier_id
        = test_vector_db_create_classifier( &setup_env,
                                            "The-Blue-Stone",  /* name */
                                            DATA_CLASSIFIER_TYPE_COMPONENT,
                                            "Any-Blue-Item"  /* stereotype */
                                          );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_id );
    const data_row_id_t feature_id
        = test_vector_db_create_feature( &setup_env,
                                         classifier_id,
                                         "The-Blue-Stone Feature",  /* name */
                                         "stereotype-2"  
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != feature_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_id
                                               );

    /* create 1 relationship */
    const data_row_id_t relation_id
        = test_vector_db_create_relationship( &setup_env,
                                              classifier_id,
                                              feature_id,
                                              classifier_id,
                                              DATA_ROW_ID_VOID,
                                              DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
                                              "feature-to-classifier",  /* name */
                                              "stereotype-3"  
                                            );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != relation_id );

    test_vector_db_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_init( &((*fix).loaded_elements) );

        const u8_error_t init_err = data_visible_set_load( &((*fix).loaded_elements), root_diag_id, &((*fix).db_reader) );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == init_err );
        const uint32_t diag_classifier_count
            = data_visible_set_get_visible_classifier_count( &((*fix).loaded_elements) );
        TEST_ENVIRONMENT_ASSERT( 2 == diag_classifier_count );

        /* load a profile */
        {
            data_profile_part_t profile;
            data_profile_part_init( &profile );

            const u8_error_t fetch_err = data_profile_part_load( &profile, &((*fix).loaded_elements), &((*fix).db_reader) );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, fetch_err );

            const uint32_t count = data_profile_part_get_stereotype_count( &profile );
            TEST_EXPECT_EQUAL_INT( 1, count );

            const data_classifier_t *const blueitem = data_profile_part_get_stereotype_const( &profile, 0 /* index */ );
            TEST_EXPECT_EQUAL_STRING( "Any-Blue-Item", data_classifier_get_name_const( blueitem ) );
            const data_classifier_t *const nullitem = data_profile_part_get_stereotype_const( &profile, 1 /* index */ );
            TEST_EXPECT_EQUAL_PTR( NULL, nullitem );

            const utf8stringview_t stereotype_name = UTF8STRINGVIEW_STR( "Any-Blue-Item" );
            const data_classifier_t *const classifier
                = data_profile_part_get_stereotype_by_name_const( &profile, &stereotype_name );
            TEST_EXPECT_EQUAL_INT( stereotype_id, data_classifier_get_row_id( classifier ) );

            /* check that trace can be called and that it finishes */
            data_profile_part_trace( &profile );

            data_profile_part_destroy( &profile );
        }

        data_visible_set_destroy( &((*fix).loaded_elements) );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t too_much_input( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* v--- creating the test vector */
    test_vector_db_t setup_env;
    test_vector_db_init( &setup_env, &((*fix).db_writer) );

    /* create the root diagram */
    const data_row_id_t root_diag_id
        = test_vector_db_create_diagram( &setup_env,
                                         DATA_ROW_ID_VOID,  /* parent_diagram_id */
                                         "root_diagram",  /* name */
                                         "Any-Blue-Item"  /* stereotype */
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    const uint_fast16_t test_count = DATA_PROFILE_PART_MAX_STEREOTYPES + 1;
    for ( uint_fast16_t index = 0; index < test_count; index ++ )
    {
        /* create a stereotype (which references itself as stereotype) */
        char name_buf[16];
        utf8stringbuf_t name = UTF8STRINGBUF( name_buf );
        utf8stringbuf_copy_str( name, "Kind-" );
        utf8stringbuf_append_int( name, index );
        const data_row_id_t stereotype_id
            = test_vector_db_create_classifier( &setup_env,
                                                utf8stringbuf_get_string( name ),  /* name */
                                                DATA_CLASSIFIER_TYPE_STEREOTYPE,
                                                utf8stringbuf_get_string( name )  /* stereotype */
                                              );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != stereotype_id );
        (void) test_vector_db_create_diagramelement( &setup_env,
                                                     root_diag_id,
                                                     stereotype_id
                                                   );
    }

    test_vector_db_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_init( &((*fix).loaded_elements) );

        const u8_error_t init_err = data_visible_set_load( &((*fix).loaded_elements), root_diag_id, &((*fix).db_reader) );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == init_err );
        const uint32_t diag_classifier_count
            = data_visible_set_get_visible_classifier_count( &((*fix).loaded_elements) );
        TEST_ENVIRONMENT_ASSERT( test_count == diag_classifier_count );

        /* load a profile */
        {
            data_profile_part_t profile;
            data_profile_part_init( &profile );

            const u8_error_t fetch_err = data_profile_part_load( &profile, &((*fix).loaded_elements), &((*fix).db_reader) );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, fetch_err );

            const uint32_t count = data_profile_part_get_stereotype_count( &profile );
            TEST_EXPECT_EQUAL_INT( DATA_PROFILE_PART_MAX_STEREOTYPES, count );

            const utf8stringview_t stereotype_name = UTF8STRINGVIEW_STR( "Kind-0" );
            const data_classifier_t *const classifier
                = data_profile_part_get_stereotype_by_name_const( &profile, &stereotype_name );
            TEST_EXPECT_EQUAL_STRING( "Kind-0", data_classifier_get_stereotype_const( classifier ) );

            data_profile_part_destroy( &profile );
        }

        data_visible_set_destroy( &((*fix).loaded_elements) );
    }
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2023-2024 Andreas Warnke
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
