/* File: data_node_set_test.c; Copyright and License: see below */

#include "data_node_set_test.h"
#include "set/data_node_set.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_vector/test_vector_db.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t no_results( test_fixture_t *test_env );
static test_case_result_t regular_node_set( test_fixture_t *test_env );
static test_case_result_t too_much_input( test_fixture_t *test_env );

test_suite_t data_node_set_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_node_set_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "no_results", &no_results );
    test_suite_add_test_case( &result, "regular_node_set", &regular_node_set );
    test_suite_add_test_case( &result, "too_much_input", &too_much_input );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
    data_node_set_t test_me;  /*!< memory for the object to test */
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
    
    /* load a node set of elements */
    {
        data_node_set_init( &((*fix).test_me) );

        data_classifier_t *const no_classifier = data_node_set_get_classifier_ptr( &((*fix).test_me) );    
        TEST_EXPECT( NULL != no_classifier );    
        TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( no_classifier ) );    
        TEST_EXPECT_EQUAL_INT( false, data_node_set_is_valid(  &((*fix).test_me) ) );    
        const uint32_t no_feature = data_node_set_get_feature_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_feature );
        const uint32_t no_relationship = data_node_set_get_relationship_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_relationship );

        const u8_error_t init_err = data_node_set_load( &((*fix).test_me), 15 /* = classifier_id */, &((*fix).db_reader) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, init_err );
        const uint32_t feature_count = data_node_set_get_feature_count( &((*fix).test_me) );
        TEST_EXPECT_EQUAL_INT( 0, feature_count );

        data_node_set_destroy( &((*fix).test_me) );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t regular_node_set( test_fixture_t *fix )
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
    const data_row_id_t relationship_1_id
        = test_vector_db_create_relationship( &setup_env,
                                              classifier_1_id,
                                              DATA_ROW_ID_VOID,
                                              classifier_2_id,
                                              DATA_ROW_ID_VOID,
                                              DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
                                              "blue-to-green",  /* name */
                                              "stereotype"  
                                            );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != relationship_1_id );
    const data_row_id_t relationship_2_id
        = test_vector_db_create_relationship( &setup_env,
                                              classifier_2_id,
                                              feature_2_id,
                                              classifier_1_id,
                                              feature_1_id,
                                              DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
                                              "GREEN_TO_BLUE",  /* name */
                                              "stereotype"  
                                            );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != relationship_2_id );

    test_vector_db_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

    /* load a node set of elements */
    {
        data_node_set_init( &((*fix).test_me) );

        const u8_error_t init_err = data_node_set_load( &((*fix).test_me), classifier_1_id, &((*fix).db_reader) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, init_err );

        const data_classifier_t *const the_classifier = data_node_set_get_classifier_const( &((*fix).test_me) );    
        TEST_EXPECT( NULL != the_classifier );    
        TEST_EXPECT_EQUAL_INT( true, data_classifier_is_valid( the_classifier ) );    
        TEST_EXPECT_EQUAL_INT( true, data_node_set_is_valid( &((*fix).test_me) ) );

        /* test features (there is only 1) */

        const uint32_t feat_count = data_node_set_get_feature_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 1, feat_count );

        const data_feature_t *const f0_c_by_idx = data_node_set_get_feature_const( &((*fix).test_me), 0 /* index */ );
        const data_feature_t *const f0_c_by_id = data_node_set_get_feature_by_id_const( &((*fix).test_me), feature_1_id );
        TEST_EXPECT_EQUAL_PTR( f0_c_by_idx, f0_c_by_id );
        TEST_EXPECT_EQUAL_INT( feature_1_id, data_feature_get_row_id( f0_c_by_idx ) );

        data_feature_t *const f0_by_idx = data_node_set_get_feature_ptr( &((*fix).test_me), 0 /* index */ );
        data_feature_t *const f0_by_id = data_node_set_get_feature_by_id_ptr( &((*fix).test_me), feature_1_id );
        TEST_EXPECT_EQUAL_PTR( f0_by_idx, f0_by_id );
        TEST_EXPECT_EQUAL_INT( feature_1_id, data_feature_get_row_id( f0_by_idx ) );

        data_feature_t *const f_list = data_node_set_get_feature_list_ptr( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_PTR( f0_by_id, f_list );

        /* test relationships */

        const uint32_t rel_count = data_node_set_get_relationship_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 2, rel_count );

        const data_relationship_t *const rel0_c_by_idx = data_node_set_get_relationship_const( &((*fix).test_me), 0 /* index */ );
        const data_row_id_t rel_0_c_id = data_relationship_get_row_id( rel0_c_by_idx );
        const data_relationship_t *const rel0_c_by_id = data_node_set_get_relationship_by_id_const( &((*fix).test_me), rel_0_c_id );
        TEST_EXPECT_EQUAL_PTR( rel0_c_by_idx, rel0_c_by_id );

        data_relationship_t *const rel0_by_idx = data_node_set_get_relationship_ptr( &((*fix).test_me), 0 /* index */ );
        const data_row_id_t rel_0_id = data_relationship_get_row_id( rel0_c_by_idx );
        data_relationship_t *const rel0_by_id = data_node_set_get_relationship_by_id_ptr( &((*fix).test_me), rel_0_id );
        TEST_EXPECT_EQUAL_PTR( rel0_by_idx, rel0_by_id );

        const data_relationship_t *const rel1_c_by_idx = data_node_set_get_relationship_const( &((*fix).test_me), 1 /* index */ );
        const data_row_id_t rel_1_c_id = data_relationship_get_row_id( rel1_c_by_idx );
        if ( rel_0_c_id == relationship_1_id )
        {
            TEST_EXPECT_EQUAL_INT( rel_1_c_id, relationship_2_id );
        }
        else
        {
            TEST_EXPECT_EQUAL_INT( rel_0_c_id, relationship_2_id );
            TEST_EXPECT_EQUAL_INT( rel_1_c_id, relationship_1_id );
        }

        data_node_set_destroy( &((*fix).test_me) );
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
                                         "stereotype"
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    /* create 1 classifier */
    const data_row_id_t classifier_1_id
        = test_vector_db_create_classifier( &setup_env,
                                            "The-Blue-Stone",  /* name */
                                            DATA_CLASSIFIER_TYPE_COMPONENT,
                                            "stereotype"
                                        );
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_1_id );
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_1_id
                                               );

    /* create many features */
    for ( int_fast32_t f_idx = 0; f_idx <= DATA_NODE_SET_MAX_FEATURES; f_idx ++ )
    {
        const data_row_id_t feature_n_id
            = test_vector_db_create_feature( &setup_env,
                                             classifier_1_id,
                                             "The-Blue-Stone Feature",  /* name */
                                             "stereotype"
                                           );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != feature_n_id );
    }

    /* create many relationships */
    for ( int_fast32_t r_idx = 0; r_idx <= DATA_NODE_SET_MAX_RELATIONSHIPS; r_idx ++ )
    {
        const data_row_id_t relationship_n_id
            = test_vector_db_create_relationship( &setup_env,
                                                  classifier_1_id,
                                                  DATA_ROW_ID_VOID,
                                                  classifier_1_id,
                                                  DATA_ROW_ID_VOID,
                                                  DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
                                                  "blue-to-green",  /* name */
                                                  "stereotype"
                                                );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != relationship_n_id );
    }

    test_vector_db_destroy( &setup_env );
    /* ^--- creating the test vector / input data finished here. */

    /* load a node set of elements */
    {
        data_node_set_init( &((*fix).test_me) );

        const u8_error_t init_err = data_node_set_load( &((*fix).test_me), classifier_1_id, &((*fix).db_reader) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, init_err );

        TEST_EXPECT_EQUAL_INT( true, data_node_set_is_valid( &((*fix).test_me) ) );

        const uint32_t feat_count = data_node_set_get_feature_count( &((*fix).test_me) );
        TEST_EXPECT_EQUAL_INT( DATA_NODE_SET_MAX_FEATURES, feat_count );

        const uint32_t rel_count = data_node_set_get_relationship_count( &((*fix).test_me) );
        TEST_EXPECT_EQUAL_INT( DATA_NODE_SET_MAX_RELATIONSHIPS, rel_count );
    }

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
