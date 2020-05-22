/* File: ctrl_diagram_controller_test.c; Copyright and License: see below */

#include "ctrl_diagram_controller_test.h"
#include "ctrl_controller.h"
#include "ctrl_diagram_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"
#include "data_diagram_type.h"
#include "test_assert.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void create_read_modify_read(void);
static void create_diagramelements_and_delete(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char* DATABASE_FILENAME = "unittest_crystal_facet_uml_default.cfu1";

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

test_suite_t ctrl_diagram_controller_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "ctrl_diagram_controller_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "create_read_modify_read", &create_read_modify_read );
    test_suite_add_test_case( &result, "create_diagramelements_and_delete", &create_diagramelements_and_delete );
    return result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int err;
    ctrl_controller_destroy( &controller );

    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT ( 0 == err );
}

static void create_read_modify_read(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    const static int64_t PARENT_ID = 17;
    int64_t diagram_id;
    uint32_t read_diagrams_count;
    data_diagram_t read_diagrams[2];
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );

    /* create a record */
    {
        diagram_id = DATA_ID_VOID_ID;
        ctrl_err = ctrl_diagram_controller_private_create_child_diagram ( diag_ctrl, PARENT_ID, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, "diagram_name", &diagram_id );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ID_VOID_ID != diagram_id );
    }

    /* read this record */
    {
        data_diagram_init_empty( &(read_diagrams[0]) );
        data_err = data_database_reader_get_diagram_by_id ( &db_reader, diagram_id, &(read_diagrams[0]) );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_id( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_ptr( &(read_diagrams[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &(read_diagrams[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );
    }

    /* modify this record */
    {
        ctrl_err = ctrl_diagram_controller_update_diagram_description ( diag_ctrl, diagram_id, "'new' diagram\ndescription" );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_update_diagram_name ( diag_ctrl, diagram_id, "\"new\" diagram name" );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl, diagram_id, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_update_diagram_list_order ( diag_ctrl, diagram_id, -4321 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    }

    /* search several records, result array too small */
    {
        data_err = data_database_reader_get_diagrams_by_parent_id ( &db_reader, PARENT_ID, 0, &read_diagrams, &read_diagrams_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_ARRAY_BUFFER_EXCEEDED, data_err );
        TEST_ASSERT_EQUAL_INT( 0, read_diagrams_count );
        /* check that old data is not overwritten: */
        TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_id( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "diagram_name", data_diagram_get_name_ptr( &(read_diagrams[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &(read_diagrams[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(read_diagrams[0]) ) );
    }

    /* search several records, result array sufficient */
    {
        data_err = data_database_reader_get_diagrams_by_parent_id ( &db_reader, PARENT_ID, 2, &read_diagrams, &read_diagrams_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, read_diagrams_count );
        /* check that new data is available */
        TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( PARENT_ID, data_diagram_get_parent_id( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, data_diagram_get_diagram_type( &(read_diagrams[0]) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "\"new\" diagram name", data_diagram_get_name_ptr( &(read_diagrams[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "'new' diagram\ndescription", data_diagram_get_description_ptr( &(read_diagrams[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( -4321, data_diagram_get_list_order( &(read_diagrams[0]) ) );
    }

    /* search several record ids to a result set */
    {
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        data_err = data_database_reader_get_diagram_ids_by_parent_id ( &db_reader, PARENT_ID, &the_set );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, data_small_set_get_count( &the_set ) );
        /* check that new data is available */
        data_id_t the_set_entry = data_small_set_get_id( &the_set, 0 );
        TEST_ASSERT_EQUAL_INT( diagram_id, data_id_get_row_id( &the_set_entry ) );
        TEST_ASSERT_EQUAL_INT( DATA_TABLE_DIAGRAM, data_id_get_table( &the_set_entry ) );
        data_small_set_destroy( &the_set );
    }
}

static void create_diagramelements_and_delete(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;
    int64_t diagram_id;
    int64_t classifier_id;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &controller );
    uint32_t read_vis_classifiers_count;
    data_visible_classifier_t read_vis_classifiers[2];
    data_diagramelement_t *diag_element_ptr;
    int64_t diag_element_id;
    data_diagramelement_t diag_element;
    uint32_t out_diagram_count;
    data_diagram_t out_diagram[2];
    data_small_set_t small_set;
    data_id_t element_id;

    /* create the root diagram */
    {
        diagram_id = DATA_ID_VOID_ID;
        ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diagram_ctrl,
                                                                               DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                               "root_diagram",
                                                                               &diagram_id
                                                                             );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT( DATA_ID_VOID_ID != diagram_id );
    }

    /* create a classifier */
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new ( &new_classifier,
                                              DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                              "",  /* stereotype */
                                              "my_if",
                                              "",  /* description */
                                              88,
                                              8800,
                                              880000
                                            );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  false,  /* add_to_latest_undo_set */
                                                                  &classifier_id
                                                                );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        data_classifier_destroy ( &new_classifier );
        TEST_ASSERT( DATA_ID_VOID_ID != classifier_id );
    }

    /* create a diagramelement */
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       diagram_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ID_VOID_ID
                                     );
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   true,  /* add_to_latest_undo_set */
                                                                   &diag_element_id
                                                                 );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_ASSERT( DATA_ID_VOID_ID != diag_element_id );
    }

    /* get the id of the diagramelement */
    {
        data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader, diagram_id, 2, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, read_vis_classifiers_count );
        diag_element_ptr = data_visible_classifier_get_diagramelement_ptr( &(read_vis_classifiers[0]) );
        TEST_ASSERT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_id( diag_element_ptr ) );
        TEST_ASSERT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_id( diag_element_ptr ) );
        diag_element_id = data_diagramelement_get_id( diag_element_ptr );
    }

    /* get the data_diagramelement_t by id */
    {
        data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, diag_element_id, &diag_element );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( diagram_id, data_diagramelement_get_diagram_id( &diag_element ) );
        TEST_ASSERT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_id( &diag_element ) );
        TEST_ASSERT_EQUAL_INT( diag_element_id, data_diagramelement_get_id( &diag_element ) );
        TEST_ASSERT_EQUAL_INT( DATA_ID_VOID_ID, data_diagramelement_get_focused_feature_id( &diag_element ) );
    }

    /* get all diagrams by classifier id */
    {
        data_err = data_database_reader_get_diagrams_by_classifier_id ( &db_reader, classifier_id, 2, &out_diagram, &out_diagram_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 1, out_diagram_count );
        TEST_ASSERT_EQUAL_INT( diagram_id, data_diagram_get_id( &(out_diagram[0]) ) );
        TEST_ASSERT_EQUAL_INT( DATA_ID_VOID_ID, data_diagram_get_parent_id( &(out_diagram[0]) ) );
        TEST_ASSERT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, data_diagram_get_diagram_type( &(out_diagram[0]) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "root_diagram", data_diagram_get_name_ptr( &(out_diagram[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_ptr( &(out_diagram[0]) ) ) );
        TEST_ASSERT_EQUAL_INT( 0, data_diagram_get_list_order( &(out_diagram[0]) ) );
    }

    /* delete the diagramelement */
    {
        data_small_set_init( &small_set );
        data_id_init( &element_id, DATA_TABLE_DIAGRAMELEMENT, diag_element_id );
        data_err = data_small_set_add_obj ( &small_set, element_id );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_delete_set ( &controller, small_set, &stat );
        TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_DELETED ));
        TEST_ASSERT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED ));
        TEST_ASSERT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* get the deleted data_diagramelement_t */
    {
        data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, diag_element_id, &diag_element );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_DB_STRUCTURE, data_err );
    }

    /* get all diagrams by classifier id */
    {
        data_err = data_database_reader_get_diagrams_by_classifier_id ( &db_reader, classifier_id, 2, &out_diagram, &out_diagram_count );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
        TEST_ASSERT_EQUAL_INT( 0, out_diagram_count );
    }
}


/*
 * Copyright 2016-2020 Andreas Warnke
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
