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
#if 0
static test_case_result_t modify_visible_set( test_fixture_t *test_env );
#endif
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
#if 0
    test_suite_add_test_case( &result, "modify_visible_set", &modify_visible_set );
#endif
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

static test_case_result_t modify_visible_set( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* v--- creating the test vector */
    /*      test vector is already empty */
    /* ^--- creating the test vector / input data finished here. */

    /* modify a visible set of elements */
    {
        data_visible_set_init( &((*fix).test_me) );
        TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( data_visible_set_get_diagram_const( &((*fix).test_me) ) ) );    
        TEST_EXPECT_EQUAL_INT( false, data_visible_set_is_valid(  &((*fix).test_me) ) );    

        /* modify diagram */
        {
            data_diagram_t diag;
            const u8_error_t diag_err 
                = data_diagram_init( &diag,
                                     12,  /* diagram_id */
                                     DATA_ROW_ID_VOID,  /* parent_diagram_id */
                                     DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
                                     "stereotype",
                                     "name",
                                     "description",
                                     54321,  /* list_order */
                                     DATA_DIAGRAM_FLAG_EMPHASIS,
                                     "865e27db-a64a-411c-89b3-83593c87c2f3"
                                   );
            TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, diag_err );

            const u8_error_t modify_err
                = data_visible_set_replace_diagram( &((*fix).test_me), &diag );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, modify_err );
            TEST_EXPECT_EQUAL_INT( false, data_diagram_is_valid( data_visible_set_get_diagram_const( &((*fix).test_me) ) ) );    
            TEST_EXPECT_EQUAL_INT( false, data_visible_set_is_valid(  &((*fix).test_me) ) );    

            data_diagram_destroy( &diag );
        }

        /* append classifiers */
        for ( uint_fast16_t index = 0; index < ( DATA_VISIBLE_SET_MAX_CLASSIFIERS + 1 ) ; index ++ )
        {
            data_visible_classifier_t vis_classfy;
            data_visible_classifier_init_empty( &vis_classfy );
            data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr( &vis_classfy );
            data_diagramelement_t *diagramelement = data_visible_classifier_get_diagramelement_ptr( &vis_classfy );

            const u8_error_t clas_err 
                = data_classifier_init( classifier,
                                        1000+index,  /* classifier_id */
                                        DATA_CLASSIFIER_TYPE_SUBSYSTEM,
                                        "stereotype",
                                        "name",
                                        "description",
                                        123,  /* x_order */
                                        124,  /* y_order */
                                        125,  /* list_order */
                                        "1c5784f7-72ba-45ff-a25a-1068eccef177"
                                      );
            TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, clas_err );

            const u8_error_t diagele_err 
                = data_diagramelement_init( diagramelement,
                                            index+1,  /* = id */
                                            12,  /* diagram_id */
                                            1000+index,  /* classifier_id */
                                            DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE|DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT,
                                            index+10001,  /* = focused_feature_id */
                                            "958aff39-f92e-47d1-9c35-b10705e7f020"
                                          );
            TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, diagele_err );

            const u8_error_t append_err = data_visible_set_append_classifier( &((*fix).test_me), &vis_classfy );
            if ( index < DATA_VISIBLE_SET_MAX_CLASSIFIERS )
            {
                TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, append_err );
                const data_visible_classifier_t *readback_vc = data_visible_set_get_visible_classifier_const( &((*fix).test_me), index );
                const data_classifier_t *readback_c = data_visible_classifier_get_classifier_const( readback_vc );
                TEST_EXPECT_EQUAL_INT( 1000+index, data_classifier_get_row_id( readback_c ) );
            }
            else
            {
                TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, append_err );
                TEST_EXPECT_EQUAL_INT( DATA_VISIBLE_SET_MAX_CLASSIFIERS, data_visible_set_get_visible_classifier_count( &((*fix).test_me) ) );
            }

            data_visible_classifier_destroy ( &vis_classfy );
        }

        /* append features */
        for ( uint_fast16_t index = 0; index < ( DATA_VISIBLE_SET_MAX_FEATURES + 1 ) ; index ++ )
        {
            data_feature_t feat;
            const u8_error_t feat_err 
                = data_feature_init( &feat,
                                     index+10000,  /* = feature_id */
                                     DATA_FEATURE_TYPE_TAGGED_VALUE,
                                     1000,  /* = classifier_id */
                                     "key",
                                     "value",
                                     "description",
                                     12000, /* list_order */
                                     "1a38c5e9-7b17-427d-920c-3c55e1b35fc3"
                                   );
            TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, feat_err );

            const u8_error_t append_err = data_visible_set_append_feature( &((*fix).test_me), &feat );
            if ( index < DATA_VISIBLE_SET_MAX_FEATURES )
            {
                TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, append_err );
                const data_feature_t *readback = data_visible_set_get_feature_const( &((*fix).test_me), index );
                TEST_EXPECT_EQUAL_INT( 10000+index, data_feature_get_row_id( readback ) );
            }
            else
            {
                TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, append_err );
                TEST_EXPECT_EQUAL_INT( DATA_VISIBLE_SET_MAX_FEATURES, data_visible_set_get_feature_count( &((*fix).test_me) ) );
            }

            data_feature_destroy ( &feat );
        }

        /* append relationships */
        for ( uint_fast16_t index = 0; index < ( DATA_VISIBLE_SET_MAX_RELATIONSHIPS + 1 ) ; index ++ )
        {
            data_relationship_t rel;
            const u8_error_t rel_err 
                = data_relationship_init( &rel,
                                          index+40000,  /* =  relationship_id */
                                          1000,  /* from_classifier_row_id */
                                          10000, /* from_feature_row_id */
                                          1001,  /* to_classifier_row_id */
                                          DATA_ROW_ID_VOID, /* to_feature_row_id */
                                          DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
                                          "stereotype", 
                                          "name",
                                          "description",
                                          123,  /* list_order */
                                          "bf1b7de9-555a-4b30-a566-586c5f4a6bd6"
                                        );
            TEST_ENVIRONMENT_ASSERT_EQUAL_INT( U8_ERROR_NONE, rel_err );

            const u8_error_t append_err = data_visible_set_append_relationship(  &((*fix).test_me), &rel );
            if ( index < DATA_VISIBLE_SET_MAX_FEATURES )
            {
                TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, append_err );
                const data_relationship_t *readback = data_visible_set_get_relationship_const( &((*fix).test_me), index );
                TEST_EXPECT_EQUAL_INT( 40000+index, data_relationship_get_row_id( readback ) );
            }
            else
            {
                TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, append_err );
                TEST_EXPECT_EQUAL_INT( DATA_VISIBLE_SET_MAX_RELATIONSHIPS, data_visible_set_get_relationship_count( &((*fix).test_me) ) );
            }

            data_relationship_destroy ( &rel );
        }

        data_visible_set_invalidate( &((*fix).test_me) );

        data_diagram_t *const no_diag = data_visible_set_get_diagram_ptr( &((*fix).test_me) );    
        TEST_EXPECT( NULL != no_diag );    
        TEST_EXPECT_EQUAL_INT( false, data_visible_set_is_valid(  &((*fix).test_me) ) );    
        const uint32_t no_classifier = data_visible_set_get_visible_classifier_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_classifier );
        const uint32_t no_feature = data_visible_set_get_feature_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_feature );
        const uint32_t no_relationship = data_visible_set_get_relationship_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_relationship );

        data_visible_set_destroy( &((*fix).test_me) );
    }

    return TEST_CASE_RESULT_ERR;
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


        const uint32_t no_classifier = data_visible_set_get_visible_classifier_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_classifier );

        const data_visible_classifier_t *const vis_clas_1 = data_visible_set_get_visible_classifier_const( &((*fix).test_me), 0 /* index */ );

        data_visible_classifier_t *const vis_clas_1_mod = data_visible_set_get_visible_classifier_ptr( &((*fix).test_me), 0 /* index */ );

        const data_visible_classifier_t *const vis_clas_2 = data_visible_set_get_visible_classifier_by_id_const( &((*fix).test_me), 0 /* diagramelement_id */ );

        data_visible_classifier_t *const vis_clas_2_mod = data_visible_set_get_visible_classifier_by_id_ptr( &((*fix).test_me), 0 /* diagramelement_id */ );

        const data_classifier_t *const clas = data_visible_set_get_classifier_by_id_const( &((*fix).test_me), 1000 /* row_id */ );

        data_classifier_t *const clas_mod = data_visible_set_get_classifier_by_id_ptr( &((*fix).test_me), 1000 /* row_id */ );

        int32_t clas_idx_1 = data_visible_set_get_classifier_index( &((*fix).test_me), 100 /* row_id */ );

        uint32_t clas_idx_2 = data_visible_set_get_classifier_index_from_pointer( &((*fix).test_me), vis_clas_1 /* vis_classifier_ptr */ );


        const uint32_t no_feature = data_visible_set_get_feature_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_feature );


        const data_feature_t *const feat_1 = data_visible_set_get_feature_const( &((*fix).test_me), 0 /* index */ );

        data_feature_t *const feat_1_mod = data_visible_set_get_feature_ptr( &((*fix).test_me), 0 /* index */ );

        const data_feature_t *const feat_2 = data_visible_set_get_feature_by_id_const( &((*fix).test_me), 1000 /* row_id */ );

        data_feature_t *const feat_2_mod = data_visible_set_get_feature_by_id_ptr( &((*fix).test_me), 1000 /* row_id */ );

        data_feature_t *const feat_list = data_visible_set_get_feature_list_ptr( &((*fix).test_me) );


        const uint32_t no_relationship = data_visible_set_get_relationship_count( &((*fix).test_me) ); 
        TEST_EXPECT_EQUAL_INT( 0, no_relationship );


        const data_relationship_t *const rel_1 = data_visible_set_get_relationship_const( &((*fix).test_me), 0 /* index */ );

        data_relationship_t *const rel_1_mod = data_visible_set_get_relationship_ptr( &((*fix).test_me), 0 /* index */ );

        const data_relationship_t *const rel_2 = data_visible_set_get_relationship_by_id_const( &((*fix).test_me), 1000 /* row_id */ );

        data_relationship_t *const rel_2_mod = data_visible_set_get_relationship_by_id_ptr( &((*fix).test_me), 1000 /* row_id */ );

        const bool is_anc = data_visible_set_is_ancestor_by_index( &((*fix).test_me), 0 /* ancestor_index */, 0 /* descendant_index */ );

        const uint32_t anc = data_visible_set_count_ancestors_of_index( &((*fix).test_me), 0 /* classifier_index */ );

        const uint32_t desc = data_visible_set_count_descendants_of_index( &((*fix).test_me), 0 /* classifier_index */ );



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
