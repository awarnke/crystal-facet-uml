/* File: data_profile_part_test.c; Copyright and License: see below */

#include "data_profile_part_test.h"
#include "set/data_profile_part.h"
#include "set/data_visible_set.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
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
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

/*!
 *  \brief row id of the root diagram
 */
static data_row_id_t diagram_id;

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

    ctrl_controller_init( &controller, &database );
    ctrl_diagram_controller_t *const diagram_ctrl
        = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create the root diagram */
    {
        diagram_id = DATA_ROW_ID_VOID;
        const u8_error_t ctrl_err
            = ctrl_diagram_controller_create_root_diagram_if_not_exists( diagram_ctrl,
                                                                         DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                         "root_diagram",
                                                                         &diagram_id
                                                                       );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == ctrl_err );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != diagram_id );
    }
}

static void tear_down(void)
{
    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
}

static void no_results(void)
{
    ctrl_classifier_controller_t *const classifier_ctrl
        = ctrl_controller_get_classifier_control_ptr( &controller );
    ctrl_diagram_controller_t *const diagram_ctrl
        = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a classifier */
    data_row_id_t classifier_id = DATA_ROW_ID_VOID;
    {
        static data_classifier_t new_classifier;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        const u8_error_t data_err = data_classifier_init( &new_classifier,
                                                          DATA_ROW_ID_VOID /* classifier_id is ignored */,
                                                          DATA_CLASSIFIER_TYPE_COMPONENT,
                                                          "The-Blue-Stone",  /* stereotype */
                                                          "The-Blue-Stone",  /* name */
                                                          "my_descr",  /* description */
                                                          45,
                                                          4500,
                                                          450000,
                                                          "7958d381-1859-49fc-b6c5-49fbc2bfebe8"
                                                        );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        const u8_error_t ctrl_err = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                                  &new_classifier,
                                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                                  &classifier_id
                                                                                );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == ctrl_err );
        data_classifier_destroy ( &new_classifier );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_id );
    }

    /* create a diagramelement for the classifier */
    data_row_id_t first_diag_element_id = DATA_ROW_ID_VOID;
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new( &new_diagele,
                                      diagram_id,
                                      classifier_id,
                                      DATA_DIAGRAMELEMENT_FLAG_NONE,
                                      DATA_ROW_ID_VOID
                                    );

        first_diag_element_id = DATA_ROW_ID_VOID;
        const u8_error_t ctrl_err
            = ctrl_diagram_controller_create_diagramelement( diagram_ctrl,
                                                             &new_diagele,
                                                             CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                             &first_diag_element_id
                                                           );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != first_diag_element_id );
    }

    /* load a visible set of elements */
    {
        data_visible_set_t elements;
        data_visible_set_init( &elements );

        const u8_error_t init_err = data_visible_set_load( &elements, diagram_id, &db_reader );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == init_err );

        /* load a profile */
        {
            data_profile_part_t profile;
            data_profile_part_init( &profile );

            const u8_error_t fetch_err = data_profile_part_load( &profile, &elements, &db_reader );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, fetch_err );

            const uint32_t count = data_profile_part_get_stereotype_count( &profile );
            TEST_EXPECT_EQUAL_INT( 1, count );

            utf8stringview_t stereotype_name = UTF8STRINGVIEW_STR( "The-Blue-Stone" );
            const data_classifier_t *const classifier
                = data_profile_part_get_stereotype_by_name_const( &profile, stereotype_name );
            TEST_EXPECT_EQUAL_STRING( "my_descr", data_classifier_get_description_const( classifier ) );

            data_profile_part_destroy( &profile );
        }

        data_visible_set_destroy(  &elements );
    }
}

static void search_and_filter(void)
{
    ctrl_classifier_controller_t *const classifier_ctrl
        = ctrl_controller_get_classifier_control_ptr( &controller );
    ctrl_diagram_controller_t *const diagram_ctrl
        = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a classifier */
    {
        static data_classifier_t new_classifier;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        const u8_error_t data_err = data_classifier_init( &new_classifier,
                                                          DATA_ROW_ID_VOID /* classifier_id is ignored */,
                                                          DATA_CLASSIFIER_TYPE_STEREOTYPE,
                                                          "",  /* stereotype */
                                                          "Any-Blue-Item",
                                                          "",  /* description */
                                                          45,
                                                          4500,
                                                          450000,
                                                          "7958d381-1859-49fc-b6c5-49fbc2bfebe8"
                                                        );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
        data_row_id_t classifier_id;
        const u8_error_t ctrl_err = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                                  &new_classifier,
                                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                                  &classifier_id
                                                                                );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == ctrl_err );
        data_classifier_destroy ( &new_classifier );
        TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_id );
    }

    TEST_EXPECT_EQUAL_INT( 5, 4+1 );
}

static void too_much_input(void)
{
    TEST_EXPECT_EQUAL_INT( 5, 4+1 );
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
