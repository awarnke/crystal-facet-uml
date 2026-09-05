/* File: geometry_compartments_test.c; Copyright and License: see below */

#include "geometry_compartments_test.h"
#include "geometry/geometry_compartments.h"
#include "geometry/geometry_compartment_type.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_geometry_compartment_type_init( test_fixture_t *fix );
static test_case_result_t test_geometry_compartments_init( test_fixture_t *fix );
static test_case_result_t test_geometry_compartments_add_feature( test_fixture_t *fix );


test_suite_t geometry_compartments_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "geometry_compartments",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_geometry_compartment_type_init", &test_geometry_compartment_type_init );
    test_suite_add_test_case( &result, "test_geometry_compartments_init", &test_geometry_compartments_init );
    test_suite_add_test_case( &result, "test_geometry_compartments_add_feature", &test_geometry_compartments_add_feature );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_geometry_compartment_type_init( test_fixture_t *fix )
{
    /* test the GEOMETRY_COMPARTMENT_TYPE_VOID */

    const geometry_compartment_type_t type1 = geometry_compartment_type_new( DATA_FEATURE_TYPE_VOID, 0 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_VOID, type1 );

    const geometry_compartment_type_t type2 = geometry_compartment_type_new( DATA_FEATURE_TYPE_LIFELINE, 0 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_VOID, type2 );

    /* test the GEOMETRY_COMPARTMENT_TYPE_PROPERTIES, OPERATIONS, TAGGED_VALUES */

    const geometry_compartment_type_t type3 = geometry_compartment_type_new( DATA_FEATURE_TYPE_PROPERTY, 0 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_PROPERTIES, type3 );

    const geometry_compartment_type_t type4 = geometry_compartment_type_new( DATA_FEATURE_TYPE_OPERATION, 0 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_OPERATIONS, type4 );

    const geometry_compartment_type_t type5 = geometry_compartment_type_new( DATA_FEATURE_TYPE_TAGGED_VALUE, 0 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_TAGGED_VALUES, type5 );

    /* test the GEOMETRY_COMPARTMENT_TYPE_PORT_... */

    const geometry_compartment_type_t type6a = geometry_compartment_type_new( DATA_FEATURE_TYPE_PORT, 1000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_PORT_ON_LEFT, type6a );

    const geometry_compartment_type_t type6b = geometry_compartment_type_new( DATA_FEATURE_TYPE_IN_PORT_PIN, -2000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_PORT_ON_RIGHT, type6b );

    const geometry_compartment_type_t type6c = geometry_compartment_type_new( DATA_FEATURE_TYPE_OUT_PORT_PIN, -1000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_PORT_ON_TOP, type6c );

    const geometry_compartment_type_t type6d = geometry_compartment_type_new( DATA_FEATURE_TYPE_ENTRY, 2000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_PORT_ON_BOTTOM, type6d );

    const geometry_compartment_type_t type6e = geometry_compartment_type_new( DATA_FEATURE_TYPE_EXIT, 2000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_PORT_ON_BOTTOM, type6e );

    /* test the GEOMETRY_COMPARTMENT_TYPE_IF_... */

    const geometry_compartment_type_t type7a = geometry_compartment_type_new( DATA_FEATURE_TYPE_PROVIDED_INTERFACE, 1000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT, type7a );

    const geometry_compartment_type_t type7b = geometry_compartment_type_new( DATA_FEATURE_TYPE_REQUIRED_INTERFACE, -2000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_IF_ON_RIGHT, type7b );

    const geometry_compartment_type_t type7c = geometry_compartment_type_new( DATA_FEATURE_TYPE_REQUIRED_INTERFACE, -1000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_IF_ON_TOP, type7c );

    const geometry_compartment_type_t type7d = geometry_compartment_type_new( DATA_FEATURE_TYPE_REQUIRED_INTERFACE, 2000000000 );
    TEST_EXPECT_EQUAL_INT( GEOMETRY_COMPARTMENT_TYPE_IF_ON_BOTTOM, type7d );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_compartments_init( test_fixture_t *fix )
{
    const double standard_object_border = 2.0;
    const double preferred_object_distance = 10.0;
    const geometry_dimensions_t feature_dim = geometry_dimensions_new( 80.0, 60.0 );

    /* test the standard constructors */

    geometry_compartments_t my_original;
    geometry_compartments_t my_copy;

    geometry_compartments_init( &my_original, standard_object_border, standard_object_border );
    geometry_compartments_reinit( &my_original, standard_object_border, preferred_object_distance );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT, &feature_dim );
    const double if0_h = geometry_compartments_get_if_height_on_left( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 70.0, if0_h );

    geometry_compartments_copy( &my_copy, &my_original );

    geometry_compartments_add_feature( &my_copy, GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT, &feature_dim );
    const double if1_h = geometry_compartments_get_if_height_on_left( &my_copy );
    TEST_EXPECT_EQUAL_FLOAT( 140.0, if1_h );

    geometry_compartments_destroy( &my_original );
    geometry_compartments_move( &my_original, &my_copy );
    geometry_compartments_replace( &my_original, &my_copy );
    geometry_compartments_replacemove( &my_original, &my_copy );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT, &feature_dim );
    const double if2_h = geometry_compartments_get_if_height_on_left( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 210.0, if2_h );

    geometry_compartments_destroy( &my_original );

    /* test the placement new constructors */

    const geometry_compartments_t my_const = geometry_compartments_new_empty();

    geometry_compartments_trace( &my_const );

    const geometry_dimensions_t outer_dim = geometry_dimensions_new( 100.0, 40.0 );

    const geometry_compartments_t my_const_2 = geometry_compartments_new( &feature_dim, &outer_dim );
    const double any_height = geometry_compartments_get_outer_height( &my_const_2 );
    TEST_EXPECT_EQUAL_FLOAT( 40.0, any_height );
    const double any_width = geometry_compartments_get_outer_width( &my_const_2 );
    TEST_EXPECT_EQUAL_FLOAT( 100.0, any_width );
    const geometry_dimensions_t *const inner_features = geometry_compartments_get_feature_compartments( &my_const_2 );
    TEST_EXPECT_EQUAL_FLOAT( 80.0, geometry_dimensions_get_width( inner_features ) );
    TEST_EXPECT_EQUAL_FLOAT( 60.0, geometry_dimensions_get_height( inner_features ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_geometry_compartments_add_feature( test_fixture_t *fix )
{
    const double standard_object_border = 2.0;
    const double preferred_object_distance = 10.0;
    const geometry_dimensions_t feature_dim = geometry_dimensions_new( 80.0, 60.0 );

    geometry_compartments_t my_original;
    geometry_compartments_init( &my_original, standard_object_border, preferred_object_distance );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_TAGGED_VALUES, &feature_dim );

    const geometry_dimensions_t *const inner_features
        = geometry_compartments_get_feature_compartments( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 84.0, geometry_dimensions_get_width( inner_features ) );
    TEST_EXPECT_EQUAL_FLOAT( 72.0, geometry_dimensions_get_height( inner_features ) );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_PORT_ON_LEFT, &feature_dim );
    const double port_left = geometry_compartments_get_port_height_on_left( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 70.0, port_left );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_PORT_ON_RIGHT, &feature_dim );
    const double port_right = geometry_compartments_get_port_height_on_right( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 70.0, port_right );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_PORT_ON_TOP, &feature_dim );
    const double port_top = geometry_compartments_get_port_width_on_top( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 90.0, port_top );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_PORT_ON_BOTTOM, &feature_dim );
    const double port_bottom = geometry_compartments_get_port_width_on_bottom( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 90.0, port_bottom );

    const geometry_dimensions_t iface_dim = geometry_dimensions_new( 75.0, 55.0 );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT, &iface_dim );
    const double iface_left = geometry_compartments_get_if_height_on_left( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 65.0, iface_left );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_IF_ON_RIGHT, &iface_dim );
    const double iface_right = geometry_compartments_get_if_height_on_right( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 65.0, iface_right );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_IF_ON_TOP, &iface_dim );
    const double iface_top = geometry_compartments_get_if_width_on_top( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 85.0, iface_top );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_IF_ON_BOTTOM, &iface_dim );
    const double iface_bottom = geometry_compartments_get_if_width_on_bottom( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 85.0, iface_bottom );

    const double any_height = geometry_compartments_get_outer_height( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 70.0, any_height );

    const double any_width = geometry_compartments_get_outer_width( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 90.0, any_width );

    /* test the other inner features on uninitialized objects: */

    geometry_compartments_reinit( &my_original, standard_object_border, preferred_object_distance );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_PROPERTIES, &feature_dim );
    const geometry_dimensions_t *const inner_features_2 = geometry_compartments_get_feature_compartments( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 84.0, geometry_dimensions_get_width( inner_features_2 ) );
    TEST_EXPECT_EQUAL_FLOAT( 72.0, geometry_dimensions_get_height( inner_features_2 ) );

    geometry_compartments_reinit( &my_original, standard_object_border, preferred_object_distance );

    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_OPERATIONS, &feature_dim );
    geometry_compartments_add_feature( &my_original, GEOMETRY_COMPARTMENT_TYPE_VOID, &feature_dim );
    const geometry_dimensions_t *const inner_features_3 = geometry_compartments_get_feature_compartments( &my_original );
    TEST_EXPECT_EQUAL_FLOAT( 84.0, geometry_dimensions_get_width( inner_features_3 ) );
    TEST_EXPECT_EQUAL_FLOAT( 72.0, geometry_dimensions_get_height( inner_features_3 ) );

    geometry_compartments_destroy( &my_original );
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
