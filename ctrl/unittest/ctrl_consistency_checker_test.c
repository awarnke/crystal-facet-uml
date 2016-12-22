/* File: ctrl_consistency_checker_test.c; Copyright and License: see below */

#include "ctrl_consistency_checker_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void diagram_two_roots_consistency(void);
static void diagram_missing_parent_consistency(void);
static void diagram_nonreferencing_diagramelements_consistency(void);
static void diagram_unreferenced_classifiers_consistency(void);
static void diagram_circular_referenced_diagrams_consistency(void);

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char* DATABASE_FILENAME = "unittest_crystal_facet_uml_default.cfu.sqlite3";

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief database writer to access the database
 */
static data_database_writer_t db_writer;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

/*!
 *  \brief database consistency is checked, errors are not fixed
 */
static const bool TEST_ONLY = false;

/*!
 *  \brief database consistency is checked and errors are fixed
 */
static const bool FIX_ERRORS = true;

TestRef ctrl_consistency_checker_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("diagram_two_roots_consistency",diagram_two_roots_consistency),
        new_TestFixture("diagram_missing_parent_consistency",diagram_missing_parent_consistency),
        new_TestFixture("diagram_circular_referenced_diagrams_consistency",diagram_circular_referenced_diagrams_consistency),
        new_TestFixture("diagram_nonreferencing_diagramelements_consistency",diagram_nonreferencing_diagramelements_consistency),
        new_TestFixture("diagram_unreferenced_classifiers_consistency",diagram_unreferenced_classifiers_consistency),
    };
    EMB_UNIT_TESTCALLER(result,"ctrl_consistency_checker_test",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    ctrl_controller_init( &controller, &database );
}

static void tear_down(void)
{
    int err;
    ctrl_controller_destroy( &controller );

    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
}

static void diagram_two_roots_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create first root diagrams */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   2 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name",
                                   "diagram_description",
                                   10222 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create second root diagrams */
    data_err = data_diagram_init ( &current_diagram,
                                   4 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name_2",
                                   "diagram_description_2",
                                   10333 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 1, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );  /* ok */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void diagram_missing_parent_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create 1 diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   2 /*=diagram_id*/,
                                   0 /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name",
                                   "diagram_description",
                                   10222 /*=list_order*/
                                 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create another diagram */
    data_err = data_diagram_init ( &current_diagram,
                                   4 /*=diagram_id*/,
                                   17 /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-4",
                                   "diagram_description-4",
                                   10333 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create a root diagram */
    data_err = data_diagram_init ( &current_diagram,
                                   6 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-6",
                                   "diagram_description-6",
                                   10444 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 without parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 without parent */
    TEST_ASSERT_EQUAL_INT( 2, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void diagram_circular_referenced_diagrams_consistency( void )
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create 1 diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   2 /*=diagram_id*/,
                                   4 /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-2",
                                   "diagram_description-2",
                                   10222 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create another diagram */
    data_err = data_diagram_init ( &current_diagram,
                                   4 /*=diagram_id*/,
                                   2 /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-4",
                                   "diagram_description-4",
                                   10333 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create a root diagram */
    data_err = data_diagram_init ( &current_diagram,
                                   6 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-6",
                                   "diagram_description-6",
                                   10444 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );  /* cannot fix */

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void diagram_nonreferencing_diagramelements_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create a root diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   6 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-6",
                                   "diagram_description-6",
                                   10444 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create 1 classifier */
    data_classifier_t current_classifier;
    data_err = data_classifier_init ( &current_classifier,
                                      12 /*=classifier id*/,
                                      DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                      "stereotype-12",
                                      "name-12",
                                      "description-12",
                                      -34000 /*=x_order*/,
                                      -16000 /*=y_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create valid diagramelement */
    data_diagramelement_t current_diagramelement;
    data_diagramelement_init ( &current_diagramelement,
                               13 /*=id*/,
                               6 /*=diagram_id*/,
                               12 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &current_diagramelement, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create diagramelement without classifier */
    data_diagramelement_init ( &current_diagramelement,
                               15 /*=id*/,
                               6 /*=diagram_id*/,
                               18 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &current_diagramelement, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create diagramelement without diagram */
    data_diagramelement_init ( &current_diagramelement,
                               17 /*=id*/,
                               2 /*=diagram_id*/,
                               12 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &current_diagramelement, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create diagramelement without diagram and without classifier */
    data_diagramelement_init ( &current_diagramelement,
                               19 /*=id*/,
                               2 /*=diagram_id*/,
                               18 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &current_diagramelement, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-15,17,19*/
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-15,17,19*/
    TEST_ASSERT_EQUAL_INT( 3, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void diagram_unreferenced_classifiers_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* create a root diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   6 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-6",
                                   "diagram_description-6",
                                   10444 /*=list_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* create 1 unreferenced classifier */
    data_classifier_t current_classifier;
    data_err = data_classifier_init ( &current_classifier,
                                      12 /*=classifier id*/,
                                      DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                      "stereotype-12",
                                      "name-12",
                                      "description-12",
                                      -34000 /*=x_order*/,
                                      -16000 /*=y_order*/
    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-12 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-12 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 1, fixed_errors );

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    fprintf( stdout, "%s", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}


/*
 * Copyright 2016-2016 Andreas Warnke
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
