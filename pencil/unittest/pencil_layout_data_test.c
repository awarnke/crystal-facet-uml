/* File: pencil_layout_data_test.c; Copyright and License: see below */

#include "pencil_layout_data_test.h"
#include "pencil_layout_data.h"
#include "set/data_visible_set.h"
#include "test_assert.h"

#define PROG_ASSERT(cond) if (!(cond)) {exit(-1);}
/* use TEST_ASSERT_* to check and report the test case result, use PROG_ASSERT() if the testing program could not be executed as expected */

static void set_up(void);
static void tear_down(void);
static void test_empty_model(void);
static void test_normal_model(void);
static void test_too_big_model(void);
static void test_inconsistent_model(void);
static data_visible_set_t* init_fake_input_data( uint_fast32_t classifiers, uint_fast32_t features, uint_fast32_t relationships );

test_suite_t pencil_layout_data_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_layout_data_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_empty_model", &test_empty_model );
    test_suite_add_test_case( &result, "test_normal_model", &test_normal_model );
    test_suite_add_test_case( &result, "test_too_big_model", &test_too_big_model );
    test_suite_add_test_case( &result, "test_inconsistent_model", &test_inconsistent_model );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static data_visible_set_t* init_fake_input_data( uint_fast32_t classifiers, uint_fast32_t features, uint_fast32_t relationships )
{
    data_error_t data_err;
    static data_visible_set_t fake_input_data;

    data_visible_set_init( &fake_input_data );

    /* initialize the fake_input_data.diagram */
    {

        data_err = data_diagram_init ( &(fake_input_data.diagram),
                                       3,  /* diagram_id */
                                       DATA_ID_VOID_ID,  /* parent_diagram_id */
                                       DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM,  /* diagram_type */
                                       "diagram_name",
                                       "diagram_description",
                                       32000  /* list_order */
                                     );
        PROG_ASSERT( data_err == DATA_ERROR_NONE );
    }

    /* initialize the fake_input_data.visible_classifiers */
    fake_input_data.visible_classifier_count = classifiers;
    for ( uint_fast32_t c_idx = 0; c_idx < classifiers; c_idx ++ )
    {
        data_visible_classifier_t *current = &(fake_input_data.visible_classifiers[c_idx]);

        data_visible_classifier_init_empty ( current );

        data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr ( current );
        data_diagramelement_t *diagele = data_visible_classifier_get_diagramelement_ptr ( current );

        data_err = data_classifier_init ( classifier,
                                          c_idx/2,  /* id */
                                          DATA_CLASSIFIER_TYPE_UML_USE_CASE,  /* main_type */
                                          "stereotype",
                                          "name",
                                          "description",
                                          1000*c_idx,  /* x_order */
                                          -300*c_idx,  /* y_order */
                                          4000*c_idx  /* list_order */
                                        );
        PROG_ASSERT( data_err == DATA_ERROR_NONE );

        data_diagramelement_init ( diagele,
                                   c_idx,  /* id */
                                   3,  /* diagram_id */
                                   c_idx/2,  /* classifier_id */
                                   DATA_DIAGRAMELEMENT_FLAG_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,  /* display_flags */
                                   DATA_ID_VOID_ID  /* focused_feature_id */
                                 );

        PROG_ASSERT( data_visible_classifier_is_valid( current ) );
    }

    /* initialize the fake_input_data.features */
    fake_input_data.feature_count = features;
    for ( uint_fast32_t f_idx = 0; f_idx < features; f_idx ++ )
    {
        data_feature_t *current = &(fake_input_data.features[f_idx]);

        data_err = data_feature_init ( current,
                                       f_idx,  /* feature_id */
                                       DATA_FEATURE_TYPE_OPERATION,  /* feature_main_type */
                                       f_idx % (classifiers/4+1),  /* classifier_id */
                                       "feature_key",
                                       "feature_value",
                                       "feature_description",
                                       6000*f_idx  /* list_order */
                                     );
        PROG_ASSERT( data_err == DATA_ERROR_NONE );

        PROG_ASSERT( data_feature_is_valid( current ) );
    }

    /* initialize the fake_input_data.relationships */
    fake_input_data.relationship_count = relationships;
    for ( uint_fast32_t r_idx = 0; r_idx < relationships; r_idx ++ )
    {
        data_relationship_t *current = &(fake_input_data.relationships[r_idx]);

        data_err = data_relationship_init ( current,
                                            r_idx,  /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,  /* relationship_main_type */
                                            r_idx % (classifiers/4+1),  /* from_classifier_id */
                                            r_idx % (classifiers/3+1),  /* to_classifier_id */
                                            "relationship_name",
                                            "relationship_description",
                                            1500*r_idx,  /* list_order */
                                            DATA_ID_VOID_ID,  /* from_feature_id */
                                            DATA_ID_VOID_ID  /* to_feature_id */
                                          );
        PROG_ASSERT( data_err == DATA_ERROR_NONE );

        PROG_ASSERT( data_relationship_is_valid( current ) );
    }

    data_visible_set_private_update_containment_cache ( &fake_input_data );

    PROG_ASSERT ( data_visible_set_is_valid ( &fake_input_data ) );
    return &fake_input_data;
}

static void test_empty_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data(0,0,0);
}

static void test_normal_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data(15,30,20);
}

static void test_too_big_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data( DATA_VISIBLE_SET_MAX_CLASSIFIERS,
                                            DATA_VISIBLE_SET_MAX_FEATURES,
                                            DATA_VISIBLE_SET_MAX_RELATIONSHIPS
                                          );
}

static void test_inconsistent_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data(5,5,5);
}


/*
 * Copyright 2019-2019 Andreas Warnke
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
