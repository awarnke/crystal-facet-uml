/* File: ctrl_consistency_checker_test.c; Copyright and License: see below */

#include "ctrl_consistency_checker_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include <stdio.h>

static void set_up(void);
static void tear_down(void);
static void diagram_two_roots_consistency(void);
static void diagram_missing_parent_consistency(void);
static void diagram_circular_referenced_diagrams_consistency(void);
static void diagram_nonreferencing_diagramelements_consistency(void);
static void diagram_illreferencing_diagramelements_consistency(void);
static void repair_unreferenced_classifiers(void);
static void repair_unreferenced_classifiers_2(void);
static void repair_invalid_feature_parent(void);
static void repair_invalid_relationship(void);
static void repair_ill_feature_relationship(void);

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
        new_TestFixture("diagram_illreferencing_diagramelements_consistency",diagram_illreferencing_diagramelements_consistency),
        new_TestFixture("repair_unreferenced_classifiers",repair_unreferenced_classifiers),
        new_TestFixture("repair_unreferenced_classifiers_2",repair_unreferenced_classifiers_2),
        new_TestFixture("repair_invalid_feature_parent",repair_invalid_feature_parent),
        new_TestFixture("repair_invalid_relationship",repair_invalid_relationship),
        new_TestFixture("repair_ill_feature_relationship",repair_ill_feature_relationship),
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

    /* crete three basic elements that neary every tests needs: a consistent, minimal database */

    /* create a root diagram */
    {
        data_error_t data_err_d;
        data_diagram_t current_diagram;

        data_err_d = data_diagram_init ( &current_diagram,
                                         6 /*=diagram_id*/,
                                         DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                         DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                         "diagram_name-6",
                                         "diagram_description-6",
                                         10444 /*=list_order*/
                                       );
        assert( DATA_ERROR_NONE == data_err_d );

        data_err_d = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
        assert( DATA_ERROR_NONE == data_err_d );
    }

    /* create a valid classifier */
    {
        data_error_t data_err_c;
        data_classifier_t current_classifier;

        data_err_c = data_classifier_init ( &current_classifier,
                                            12 /*=classifier id*/,
                                            DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                            "stereotype-12",
                                            "name-12",
                                            "description-12",
                                            -34000 /*=x_order*/,
                                            -16000 /*=y_order*/,
                                            -7000 /*=list_order*/
                                          );
        assert( DATA_ERROR_NONE == data_err_c );

        data_err_c = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
        assert( DATA_ERROR_NONE == data_err_c );
    }

    /* create valid diagramelement */
    {
        data_error_t data_err_de;
        data_diagramelement_t current_diagramelement;

        data_diagramelement_init ( &current_diagramelement,
                                   13 /*=id*/,
                                   6 /*=diagram_id*/,
                                   12 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   DATA_ID_VOID_ID
                                 );

        data_err_de = data_database_writer_create_diagramelement( &db_writer, &current_diagramelement, NULL /*=out_new_id*/ );
        assert( DATA_ERROR_NONE == data_err_de );
    }
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

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create second root diagram */
    {
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
    }

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_ASSERT_EQUAL_INT( 1, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
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

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create 1 diagram */
    {
        data_diagram_t second_diagram;
        data_err = data_diagram_init ( &second_diagram,
                                       2 /*=diagram_id*/,
                                       0 /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                       "diagram_name",
                                       "diagram_description",
                                       10222 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &db_writer, &second_diagram, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create another diagram */
    {
        data_diagram_t third_diagram;
        data_err = data_diagram_init ( &third_diagram,
                                       4 /*=diagram_id*/,
                                       17 /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                       "diagram_name-4",
                                       "diagram_description-4",
                                       10333 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &db_writer, &third_diagram, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 without parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 without parent */
    TEST_ASSERT_EQUAL_INT( 2, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
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

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create 2nd diagram */
    {
        data_diagram_t second_diagram;
        data_err = data_diagram_init ( &second_diagram,
                                       2 /*=diagram_id*/,
                                       4 /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                       "diagram_name-2",
                                       "diagram_description-2",
                                       10222 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &db_writer, &second_diagram, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create another diagram */
    {
        data_diagram_t third_diagram;
        data_err = data_diagram_init ( &third_diagram,
                                       4 /*=diagram_id*/,
                                       2 /*=parent_diagram_id*/,
                                       DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                       "diagram_name-4",
                                       "diagram_description-4",
                                       10333 /*=list_order*/
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &db_writer, &third_diagram, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );  /* cannot fix */

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
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

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create diagramelement without classifier */
    {
        data_diagramelement_t second_diagramelement;
        data_diagramelement_init ( &second_diagramelement,
                                   15 /*=id*/,
                                   6 /*=diagram_id*/,
                                   18 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   DATA_ID_VOID_ID
                                 );

        data_err = data_database_writer_create_diagramelement( &db_writer, &second_diagramelement, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create diagramelement without diagram */
    {
        data_diagramelement_t third_diagramelement;
        data_diagramelement_init ( &third_diagramelement,
                                   17 /*=id*/,
                                   2 /*=diagram_id*/,
                                   12 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   DATA_ID_VOID_ID
                                 );

        data_err = data_database_writer_create_diagramelement( &db_writer, &third_diagramelement, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create diagramelement without diagram and without classifier */
    {
        data_diagramelement_t fourth_diagramelement;
        data_diagramelement_init ( &fourth_diagramelement,
                                   19 /*=id*/,
                                   2 /*=diagram_id*/,
                                   18 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   DATA_ID_VOID_ID
                                 );

        data_err = data_database_writer_create_diagramelement( &db_writer, &fourth_diagramelement, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-15,17,19 */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-15,17,19 */
    TEST_ASSERT_EQUAL_INT( 3, fixed_errors );

    /* check the diagrams */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void diagram_illreferencing_diagramelements_consistency(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create a valid feature for classifier-12 */
    {
        data_feature_t v_feature;
        data_err = data_feature_init ( &v_feature,
                                       17, /* feature_id */
                                       DATA_FEATURE_TYPE_LIFELINE, /* feature_main_type */
                                       12, /* classifier_id */
                                       "startup_time", /* feature_key */
                                       "uint64_t", /* feature_value */
                                       "time in nano seconds to start", /* feature_description */
                                       5000000 /* list order */
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &db_writer, &v_feature, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create 2nd diagramelement with non-existing focused feature */
    {
        data_diagramelement_t second_diagramelement;
        data_diagramelement_init ( &second_diagramelement,
                                   14 /*=id*/,
                                   6 /*=diagram_id*/,
                                   12 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   18 /*=focused_feature_id*/
                                 );

        data_err = data_database_writer_create_diagramelement( &db_writer, &second_diagramelement, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create a second valid classifier */
    {
        data_classifier_t current_classifier;
        data_err = data_classifier_init ( &current_classifier,
                                          11 /*=classifier id*/,
                                          DATA_CLASSIFIER_TYPE_UML_CLASS,
                                          "stereotype-11",
                                          "name-11",
                                          "description-11",
                                          -340 /*=x_order*/,
                                          -160 /*=y_order*/,
                                          -7000 /*=list_order*/
                                        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create 3rd diagramelement with existing focused feature but wrong, inconsistent classifier */
    {
        data_diagramelement_t third_diagramelement;
        data_diagramelement_init ( &third_diagramelement,
                                   15 /*=id*/,
                                   6 /*=diagram_id*/,
                                   11 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   17 /*=focused_feature_id*/
                                 );

        data_err = data_database_writer_create_diagramelement( &db_writer, &third_diagramelement, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the diagramelements */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* 2nd and 3rd diagramelements have non-healthy references */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagramelements */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 2, found_errors );  /* 2nd and 3rd diagramelements have non-healthy references */
    TEST_ASSERT_EQUAL_INT( 2, fixed_errors );

    /* check the diagramelements */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void repair_unreferenced_classifiers(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create 1 unreferenced classifier */
    {
        data_classifier_t current_classifier;
        data_err = data_classifier_init ( &current_classifier,
                                          13 /*=classifier id*/,
                                          DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                          "stereotype-13",
                                          "name-13",
                                          "description-13",
                                          -35050 /*=x_order*/,
                                          -17070 /*=y_order*/,
                                          -7000 /*=list_order*/
                                        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-13 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-13 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 1, fixed_errors );

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void repair_unreferenced_classifiers_2(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create 1 unreferenced classifier */
    {
        data_classifier_t second_classifier;
        data_err = data_classifier_init ( &second_classifier,
                                          6 /*=classifier id*/,
                                          DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                          "stereotype-c6",
                                          "name-c6",
                                          "description-c6",
                                          -4200 /*=x_order*/,
                                          -6200 /*=y_order*/,
                                          -7000 /*=list_order*/
                                        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &db_writer, &second_classifier, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create a valid feature */
    {
        data_feature_t v_feature;
        data_err = data_feature_init ( &v_feature,
                                       17, /* feature_id */
                                       DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                       6, /* classifier_id */
                                       "startup_time", /* feature_key */
                                       "uint64_t", /* feature_value */
                                       "time in nano seconds to start", /* feature_description */
                                       5000000 /* list order */
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &db_writer, &v_feature, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define a valid relationship */
    {
        data_relationship_t v_relation;
        data_err = data_relationship_init ( &v_relation,
                                            34, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            6, /* from_classifier_id */
                                            6, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            -66000, /* list_order */
                                            DATA_ID_VOID_ID, /* from_feature_id */
                                            DATA_ID_VOID_ID /* to_feature_id */
                                          );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &v_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-6 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-6 is unreferenced, after deleting, also the relationship and the feature are unreferenced. */
    TEST_ASSERT_EQUAL_INT( 3, fixed_errors );

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void repair_invalid_feature_parent(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create a valid feature */
    {
        data_feature_t v_feature;
        data_err = data_feature_init ( &v_feature,
                                       17, /* feature_id */
                                       DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                       12, /* classifier_id */
                                       "startup_time", /* feature_key */
                                       "uint64_t", /* feature_value */
                                       "time in nano seconds to start", /* feature_description */
                                       5000000 /* list order */
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &db_writer, &v_feature, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create an invalid feature */
    {
        data_feature_t i_feature;
        data_err = data_feature_init ( &i_feature,
                                       19, /* feature_id */
                                       DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                       12121212, /* classifier_id */
                                       "startup_time", /* feature_key */
                                       "uint64_t", /* feature_value */
                                       "time in nano seconds to start", /* feature_description */
                                       5000000 /* list order */
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &db_writer, &i_feature, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-19 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 1, found_errors );  /* id-19 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 1, fixed_errors );

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void repair_invalid_relationship(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* define a valid relationship */
    {
        data_relationship_t v_relation;
        data_err = data_relationship_init ( &v_relation,
                                            34, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            12, /* from_classifier_id */
                                            12, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            -66000, /* list_order */
                                            DATA_ID_VOID_ID, /* from_feature_id */
                                            DATA_ID_VOID_ID /* to_feature_id */
                                          );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &v_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define first invalid relationship */
    {
        data_relationship_t i1_relation;
        data_err = data_relationship_init ( &i1_relation,
                                            35, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            12, /* from_classifier_id */
                                            12121212, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            -66000, /* list_order */
                                            DATA_ID_VOID_ID, /* from_feature_id */
                                            DATA_ID_VOID_ID /* to_feature_id */
                                          );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &i1_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define second invalid relationship */
    {
        data_relationship_t i2_relation;
        data_err = data_relationship_init ( &i2_relation,
                                            36, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            12121212, /* from_classifier_id */
                                            12, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            -66000, /* list_order */
                                            DATA_ID_VOID_ID, /* from_feature_id */
                                            DATA_ID_VOID_ID /* to_feature_id */
                                          );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &i2_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define third invalid relationship */
    {
        data_relationship_t i3_relation;
        data_err = data_relationship_init ( &i3_relation,
                                            37, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            12121212, /* from_classifier_id */
                                            12121212, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            -66000, /* list_order */
                                            DATA_ID_VOID_ID, /* from_feature_id */
                                            DATA_ID_VOID_ID /* to_feature_id */
                                          );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &i3_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-35,36,37 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /* id-35,36,37 is unreferenced */
    TEST_ASSERT_EQUAL_INT( 3, fixed_errors );

    /* check the database */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}

static void repair_ill_feature_relationship(void)
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    char out_report_buf[1024] = "";
    utf8stringbuf_t out_report = UTF8STRINGBUF( out_report_buf );
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create a second classifier */
    {
        data_classifier_t second_classifier;
        data_err = data_classifier_init ( &second_classifier,
                                          11 /*=classifier id*/,
                                          DATA_CLASSIFIER_TYPE_UML_PACKAGE,
                                          "stereotype-11",
                                          "name-11",
                                          "description-11",
                                          -4200 /*=x_order*/,
                                          -6200 /*=y_order*/,
                                          -7000 /*=list_order*/
                                        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &db_writer, &second_classifier, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create a first valid feature */
    {
        data_feature_t first_feature;
        data_err = data_feature_init ( &first_feature,
                                       18, /* feature_id */
                                       DATA_FEATURE_TYPE_LIFELINE, /* feature_main_type */
                                       12, /* classifier_id */
                                       "shutdown scenario", /* feature_key */
                                       "", /* feature_value */
                                       "timeline of shutdown", /* feature_description */
                                       500 /* list order */
        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &db_writer, &first_feature, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create a second valid feature */
    {
        data_feature_t second_feature;
        data_err = data_feature_init ( &second_feature,
                                       17, /* feature_id */
                                       DATA_FEATURE_TYPE_PORT, /* feature_main_type */
                                       11, /* classifier_id */
                                       "Port 8080", /* feature_key */
                                       "ip_port", /* feature_value */
                                       "port for control commands", /* feature_description */
                                       1500 /* list order */
                                     );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &db_writer, &second_feature, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* create a diagramelement for the second classifier */
    {
        data_diagramelement_t second_diagramelement;
        data_diagramelement_init ( &second_diagramelement,
                                   14 /*=id*/,
                                   6 /*=diagram_id*/,
                                   11 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_NONE,
                                   17 /*=focused_feature_id*/
                                 );

        data_err = data_database_writer_create_diagramelement( &db_writer, &second_diagramelement, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define a valid relationship */
    {
        data_relationship_t first_relation;
        data_err = data_relationship_init ( &first_relation,
                                            34, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            11, /* from_classifier_id */
                                            12, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            1600, /* list_order */
                                            17, /* from_feature_id */
                                            18 /* to_feature_id */
                                          );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &first_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define a relationship where source feature does not exist*/
    {
        data_relationship_t second_relation;
        data_err = data_relationship_init ( &second_relation,
                                            35, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            11, /* from_classifier_id */
                                            12, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            1700, /* list_order */
                                            170, /* from_feature_id */
                                            18 /* to_feature_id */
        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &second_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define a relationship where destination feature has illegal classifier */
    {
        data_relationship_t third_relation;
        data_err = data_relationship_init ( &third_relation,
                                            36, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            11, /* from_classifier_id */
                                            12, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            1800, /* list_order */
                                            17, /* from_feature_id */
                                            17 /* to_feature_id */
        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &third_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* define a relationship where source has illegal classifier and destination feature non-existant */
    {
        data_relationship_t first_relation;
        data_err = data_relationship_init ( &first_relation,
                                            37, /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                            11, /* from_classifier_id */
                                            12, /* to_classifier_id */
                                            "the composition is more", /* relationship_name */
                                            "than the sum of its parts", /* relationship_description */
                                            1900, /* list_order */
                                            18, /* from_feature_id */
                                            180 /* to_feature_id */
        );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &db_writer, &first_relation, NULL /*=out_new_id*/ );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    }

    /* check the relationships */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /*! illegal: relationships 35, 36 and 37 */
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );

    /* fix the relationships */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, FIX_ERRORS, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 3, found_errors );  /*! illegal: relationships 35, 36 and 37 */
    TEST_ASSERT_EQUAL_INT( 3, fixed_errors );

    /* check the relationships */
    utf8stringbuf_clear( out_report );
    ctrl_err = ctrl_controller_repair_database ( &controller, TEST_ONLY, &found_errors, &fixed_errors, out_report );
    TRACE_INFO_STR( "out_report:", utf8stringbuf_get_string( out_report ) );
    TEST_ASSERT_EQUAL_INT( CTRL_ERROR_NONE, ctrl_err );
    TEST_ASSERT_EQUAL_INT( 0, found_errors );
    TEST_ASSERT_EQUAL_INT( 0, fixed_errors );
}


/*
 * Copyright 2016-2018 Andreas Warnke
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
