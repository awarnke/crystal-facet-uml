/* File: data_profile_part_test.c; Copyright and License: see below */

#include "data_profile_part_test.h"
#include "set/data_profile_part.h"
#include "set/data_visible_set.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_vector/test_vector_db.h"

static void set_up(void);
static void tear_down(void);
static void no_results(void);
static void search_and_filter(void);
static void too_much_input(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief db_writer instance to write to the database.
 *
 *  Typically this should by done via a controller.
 *  But here, this test cases should not use layers above the currently tested one.
 */
static data_database_writer_t db_writer;

test_suite_t data_profile_part_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_profile_part_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "no_results", &no_results );
    test_suite_add_test_case( &result, "search_and_filter", &search_and_filter );
    test_suite_add_test_case( &result, "too_much_input", &too_much_input );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open_in_memory( &database );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );
}

static void tear_down(void)
{
    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static void no_results(void)
{
    test_vector_db_t setup_env;
    test_vector_db_init( &setup_env, &db_writer );

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

    /* create a classifier with stereotype which does not exist */
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
    /* ^^^^ creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_t elements;
        data_visible_set_init( &elements );

        const u8_error_t init_err = data_visible_set_load( &elements, root_diag_id, &db_reader );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == init_err );
        const uint32_t diag_classifier_count
            = data_visible_set_get_visible_classifier_count( &elements );
        TEST_ENVIRONMENT_ASSERT( 2 == diag_classifier_count );

        /* load a profile */
        {
            data_profile_part_t profile;
            data_profile_part_init( &profile );

            const u8_error_t fetch_err = data_profile_part_load( &profile, &elements, &db_reader );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, fetch_err );

            const uint32_t count = data_profile_part_get_stereotype_count( &profile );
            TEST_EXPECT_EQUAL_INT( 0, count );

            data_profile_part_destroy( &profile );
        }

        data_visible_set_destroy(  &elements );
    }
}

static void search_and_filter(void)
{
    test_vector_db_t setup_env;
    test_vector_db_init( &setup_env, &db_writer );

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
    (void) test_vector_db_create_diagramelement( &setup_env,
                                                 root_diag_id,
                                                 classifier_id
                                               );

    test_vector_db_destroy( &setup_env );
    /* ^^^^ creating the test vector / input data finished here. */

    /* load a visible set of elements */
    {
        data_visible_set_t elements;
        data_visible_set_init( &elements );

        const u8_error_t init_err = data_visible_set_load( &elements, root_diag_id, &db_reader );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == init_err );
        const uint32_t diag_classifier_count
            = data_visible_set_get_visible_classifier_count( &elements );
        TEST_ENVIRONMENT_ASSERT( 2 == diag_classifier_count );

        /* load a profile */
        {
            data_profile_part_t profile;
            data_profile_part_init( &profile );

            const u8_error_t fetch_err = data_profile_part_load( &profile, &elements, &db_reader );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, fetch_err );

            const uint32_t count = data_profile_part_get_stereotype_count( &profile );
            TEST_EXPECT_EQUAL_INT( 1, count );

            utf8stringview_t stereotype_name = UTF8STRINGVIEW_STR( "Any-Blue-Item" );
            const data_classifier_t *const classifier
                = data_profile_part_get_stereotype_by_name_const( &profile, stereotype_name );
            TEST_EXPECT_EQUAL_INT( stereotype_id, data_classifier_get_row_id( classifier ) );

            data_profile_part_destroy( &profile );
        }

        data_visible_set_destroy(  &elements );
    }
}

static void too_much_input(void)
{
    TEST_EXPECT_EQUAL_INT( 5, 3+1 );
}


/*
 * Copyright 2023-2023 Andreas Warnke
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
