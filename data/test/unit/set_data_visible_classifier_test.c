/* File: set_data_visible_classifier_test.c; Copyright and License: see below */

#include "set_data_visible_classifier_test.h"
#include "set/data_visible_classifier.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_init_copy_get( test_fixture_t *test_env );

test_suite_t set_data_visible_classifier_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "set_data_visible_classifier_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_init_copy_get", &test_init_copy_get );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_init_copy_get( test_fixture_t *test_env )
{
    data_visible_classifier_t my_vis_classifier;
    data_classifier_t classifier;
    data_diagramelement_t diagramelement;

    /* sub test case: init */
    data_classifier_init_empty( &classifier );
    data_diagramelement_init_empty( &diagramelement );
    data_visible_classifier_init( &my_vis_classifier, &classifier, &diagramelement );
    const data_classifier_t *const class_c1 = data_visible_classifier_get_classifier_const( &my_vis_classifier);
    const data_diagramelement_t *const diagele_c1 = data_visible_classifier_get_diagramelement_const( &my_vis_classifier );
    TEST_EXPECT_EQUAL_STRING( data_classifier_get_uuid_const( &classifier ), data_classifier_get_uuid_const( class_c1 ) );
    TEST_EXPECT_EQUAL_STRING( data_diagramelement_get_uuid_const( &diagramelement ), data_diagramelement_get_uuid_const( diagele_c1 ) );

    /* sub test case: copy */
    data_visible_classifier_t my_twin;
    data_visible_classifier_copy( &my_twin, &my_vis_classifier );
    const data_classifier_t *const class_c2 = data_visible_classifier_get_classifier_const( &my_vis_classifier);
    const data_diagramelement_t *const diagele_c2 = data_visible_classifier_get_diagramelement_const( &my_vis_classifier );
    TEST_EXPECT_EQUAL_STRING( data_classifier_get_uuid_const( &classifier ), data_classifier_get_uuid_const( class_c2 ) );
    TEST_EXPECT_EQUAL_STRING( data_diagramelement_get_uuid_const( &diagramelement ), data_diagramelement_get_uuid_const( diagele_c2 ) );
    TEST_EXPECT_EQUAL_INT( false, data_visible_classifier_is_valid( &my_twin ) );

    /* sub test case: init_empty */
    data_visible_classifier_destroy( &my_vis_classifier );
    data_visible_classifier_init_empty( &my_vis_classifier );
    TEST_EXPECT_EQUAL_INT( false, data_visible_classifier_is_valid( &my_vis_classifier ) );

    /* sub test case: get modifiable ptr to members */
    data_classifier_t *const class_mod = data_visible_classifier_get_classifier_ptr( &my_vis_classifier);
    data_diagramelement_t *const diagele_mod = data_visible_classifier_get_diagramelement_ptr( &my_vis_classifier );
    const u8_error_t reinit_result
        = data_classifier_reinit( class_mod,
                                  54,
                                  DATA_CLASSIFIER_TYPE_OBJECT,
                                  "stereotype",
                                  "name",
                                  "description",
                                  16,
                                  9,
                                  47,
                                  "097498ef-e43b-4b79-b26a-df6f23590165"
                                );     
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, reinit_result );
    data_diagramelement_reinit( diagele_mod,
                                12345,
                                103,
                                555,
                                DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT,
                                100,
                                "111498ef-e43b-4b79-b26a-df6f23590165"
                              );
    TEST_EXPECT_EQUAL_INT( true, data_visible_classifier_is_valid( &my_vis_classifier ) );
   
    /* sub test case: replace */
    data_visible_classifier_replace( &my_twin, &my_vis_classifier );
    TEST_EXPECT_EQUAL_INT( true, data_visible_classifier_is_valid( &my_twin ) );

    data_visible_classifier_destroy( &my_twin );
    data_visible_classifier_destroy( &my_vis_classifier );
    data_diagramelement_destroy( &diagramelement );
    data_classifier_destroy( &classifier );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2026 Andreas Warnke
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
