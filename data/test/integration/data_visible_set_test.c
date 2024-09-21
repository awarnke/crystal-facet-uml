/* File: data_visible_set_test.c; Copyright and License: see below */

#include "data_visible_set_test.h"
#include "set/data_visible_set.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_vector/test_vector_db.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t no_results( test_fixture_t *test_env );
static test_case_result_t regular_visible_set( test_fixture_t *test_env );
static test_case_result_t too_much_input( test_fixture_t *test_env );

test_suite_t data_visible_set_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_visible_set_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "no_results", &no_results );
    test_suite_add_test_case( &result, "regular_visible_set", &regular_visible_set );
    test_suite_add_test_case( &result, "too_much_input", &too_much_input );
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

static test_case_result_t no_results( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* v--- creating the test vector */
    /*      test vector is already empty */
    /* ^--- creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_init( &((*fix).test_me) );

        data_diagram_t *const no_diag = data_visible_set_get_diagram_ptr( &((*fix).test_me) );    
        TEST_EXPECT( NULL != no_diag );    
        TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( no_diag ) );    
        TEST_EXPECT_EQUAL_INT( false, data_visible_set_is_valid(  &((*fix).test_me) ) );    

        data_visible_set_reinit( &((*fix).test_me) );

        const uint32_t no_classifier = data_visible_set_get_visible_classifier_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_classifier );
        const uint32_t no_feature = data_visible_set_get_feature_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_feature );
        const uint32_t no_relationship = data_visible_set_get_relationship_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_relationship );

        const u8_error_t init_err = data_visible_set_load( &((*fix).test_me), 13 /* = diagram_id */, &((*fix).db_reader) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, init_err );
        const uint32_t diag_classifier_count = data_visible_set_get_visible_classifier_count( &((*fix).test_me) );
        TEST_EXPECT_EQUAL_INT( 0, diag_classifier_count );

        data_visible_set_destroy( &((*fix).test_me) );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t regular_visible_set( test_fixture_t *fix )
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
                                         "stereotype" 
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    /* create a classifier */
    const data_row_id_t classifier_1_id
        = test_vector_db_create_classifier( &setup_env,
                                            "The-Blue-Stone",  /* name */
                                            DATA_CLASSIFIER_TYPE_COMPONENT,
                                            "stereotype"  
                                          );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_1_id );
    const data_row_id_t feature_1_id
        = test_vector_db_create_feature( &setup_env,
                                         classifier_1_id,
                                         "The-Blue-Stone Feature",  /* name */
                                         "stereotype"  
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != feature_1_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_1_id
                                               );

    /* create another classifier */
    const data_row_id_t classifier_2_id
        = test_vector_db_create_classifier( &setup_env,
                                            "The-Green-Stone",  /* name */
                                            DATA_CLASSIFIER_TYPE_COMPONENT,
                                            "stereotype"  
                                          );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_2_id );
    const data_row_id_t feature_2_id
        = test_vector_db_create_feature( &setup_env,
                                         classifier_2_id,
                                         "The-Green-Stone Feature",  /* name */
                                         "stereotype"  
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != feature_2_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_2_id
                                               );

    /* create 2 relationships */
    const data_row_id_t rel_1_id
        = test_vector_db_create_relationship( &setup_env,
                                              classifier_1_id,
                                              DATA_ROW_ID_VOID,
                                              classifier_2_id,
                                              DATA_ROW_ID_VOID,
                                              "blue-to-green",  /* name */
                                              "stereotype"  
                                            );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != rel_1_id );
    const data_row_id_t rel_2_id
        = test_vector_db_create_relationship( &setup_env,
                                              classifier_2_id,
                                              feature_2_id,
                                              classifier_1_id,
                                              feature_1_id,
                                              "GREEN_TO_BLUE",  /* name */
                                              "stereotype"  
                                            );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != rel_2_id );

    test_vector_db_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_init( &((*fix).test_me) );

        const u8_error_t init_err = data_visible_set_load( &((*fix).test_me), root_diag_id, &((*fix).db_reader) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, init_err );



        const data_diagram_t *const no_diag = data_visible_set_get_diagram_const( &((*fix).test_me) );    
        TEST_EXPECT( NULL != no_diag );    
        TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( no_diag ) );    
        TEST_EXPECT_EQUAL_INT( false, data_visible_set_is_valid(  &((*fix).test_me) ) );    

        data_visible_set_reinit( &((*fix).test_me) );

        const uint32_t no_classifier = data_visible_set_get_visible_classifier_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_classifier );
        const uint32_t no_feature = data_visible_set_get_feature_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_feature );
        const uint32_t no_relationship = data_visible_set_get_relationship_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_relationship );

        const uint32_t diag_classifier_count = data_visible_set_get_visible_classifier_count( &((*fix).test_me) );
        TEST_EXPECT_EQUAL_INT( 0, diag_classifier_count );

        data_visible_set_destroy( &((*fix).test_me) );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t too_much_input( test_fixture_t *fix )
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
