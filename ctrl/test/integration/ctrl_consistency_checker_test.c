/* File: ctrl_consistency_checker_test.c; Copyright and License: see below */

#include "ctrl_consistency_checker_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8stream/universal_stream_output_stream.h"
#include "u8stream/universal_null_output_stream.h"
#include "utf8stream/utf8stream_writer.h"
#include "u8/u8_trace.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t diagram_two_roots_consistency( test_fixture_t *fix );
static test_case_result_t diagram_missing_parent_consistency( test_fixture_t *fix );
static test_case_result_t diagram_circular_referenced_diagrams_consistency( test_fixture_t *fix );
static test_case_result_t diagram_nonreferencing_diagramelements_consistency( test_fixture_t *fix );
static test_case_result_t diagram_illreferencing_diagramelements_consistency( test_fixture_t *fix );
static test_case_result_t repair_unreferenced_classifiers( test_fixture_t *fix );
static test_case_result_t repair_unreferenced_classifiers_2( test_fixture_t *fix );
static test_case_result_t repair_invalid_feature_parent( test_fixture_t *fix );
static test_case_result_t repair_invalid_relationship( test_fixture_t *fix );
static test_case_result_t repair_ill_feature_relationship( test_fixture_t *fix );

test_suite_t ctrl_consistency_checker_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "ctrl_classifier_consistency_checker_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "diagram_two_roots_consistency", &diagram_two_roots_consistency );
    test_suite_add_test_case( &result, "diagram_missing_parent_consistency", &diagram_missing_parent_consistency );
    test_suite_add_test_case( &result, "diagram_circular_referenced_diagrams_consistency", &diagram_circular_referenced_diagrams_consistency );
    test_suite_add_test_case( &result, "diagram_nonreferencing_diagramelements_consistency", &diagram_nonreferencing_diagramelements_consistency );
    test_suite_add_test_case( &result, "diagram_illreferencing_diagramelements_consistency", &diagram_illreferencing_diagramelements_consistency );
    test_suite_add_test_case( &result, "repair_unreferenced_classifiers", &repair_unreferenced_classifiers );
    test_suite_add_test_case( &result, "repair_unreferenced_classifiers_2", &repair_unreferenced_classifiers_2 );
    test_suite_add_test_case( &result, "repair_invalid_feature_parent", &repair_invalid_feature_parent );
    test_suite_add_test_case( &result, "repair_invalid_relationship", &repair_invalid_relationship );
    test_suite_add_test_case( &result, "repair_ill_feature_relationship", &repair_ill_feature_relationship );
    return result;
}

/*!
 *  \brief database consistency is checked, errors are not fixed
 */
static const bool TEST_ONLY = false;

/*!
 *  \brief database consistency is checked and errors are fixed
 */
static const bool FIX_ERRORS = true;

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
    ctrl_controller_t controller;  /*!< controller instance on which the tests are performed */
#ifndef NDEBUG
    universal_stream_output_stream_t out_stream;  /*!< output stream for report writer in DEBUG MODE */
#else
    universal_null_output_stream_t out_stream;  /*!< output stream for report writer in RELEASE MODE */
#endif
    utf8stream_writer_t out_report;  /*!< output report writer */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    data_database_init( &((*fix).database) );
    data_database_open_in_memory( &((*fix).database) );
    data_database_reader_init( &((*fix).db_reader), &((*fix).database) );
    data_database_writer_init( &((*fix).db_writer), &((*fix).db_reader), &((*fix).database) );
    ctrl_controller_init( &((*fix).controller), &((*fix).database) );

    /* create three basic elements that neary every tests needs: a consistent, minimal database */

    /* create a root diagram */
    {
        u8_error_t data_err_d;
        data_diagram_t current_diagram;

        data_err_d = data_diagram_init ( &current_diagram,
                                         6 /*=diagram_id*/,
                                         DATA_ROW_VOID /*=parent_diagram_id*/,
                                         DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                         "diag_stereo_t", /* stereotype */
                                         "diagram_name-6",
                                         "diagram_description-6",
                                         10444 /*=list_order*/,
                                         DATA_DIAGRAM_FLAG_NONE,
                                         "4ad5e0d6-2fd6-4863-b1b3-beec480bd670"
                                       );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_d );

        data_err_d = data_database_writer_create_diagram ( &((*fix).db_writer), &current_diagram, NULL /*=out_new_id*/ );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_d );
    }

    /* create a valid classifier */
    {
        u8_error_t data_err_c;
        data_classifier_t current_classifier;

        data_err_c = data_classifier_init ( &current_classifier,
                                            12 /*=classifier id*/,
                                            DATA_CLASSIFIER_TYPE_INTERFACE,
                                            "stereotype-12",
                                            "name-12",
                                            "description-12",
                                            -34000 /*=x_order*/,
                                            -16000 /*=y_order*/,
                                            -7000 /*=list_order*/,
                                            "a9f873b3-9f92-480b-afd9-5be080c96783"
                                          );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_c );

        data_err_c = data_database_writer_create_classifier( &((*fix).db_writer), &current_classifier, NULL /*=out_new_id*/ );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_c );
    }

    /* create valid diagramelement */
    {
        u8_error_t data_err_de;
        data_diagramelement_t current_diagramelement;

        data_err_de = data_diagramelement_init ( &current_diagramelement,
                                                 13 /*=id*/,
                                                 6 /*=diagram_id*/,
                                                 12 /*=classifier_id*/,
                                                 DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                                 DATA_ROW_VOID,
                                                 "db14f3a4-ce9c-43ea-8787-50094377fa5f"
                                               );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_de );

        data_err_de = data_database_writer_create_diagramelement( &((*fix).db_writer), &current_diagramelement, NULL /*=out_new_id*/ );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err_de );
    }

    /* create the report writer */
#ifndef NDEBUG
    universal_stream_output_stream_init( &((*fix).out_stream), stdout );
    universal_output_stream_t *const out_base = universal_stream_output_stream_get_output_stream( &((*fix).out_stream) );
#else
    universal_null_output_stream_init( &((*fix).out_stream) );
    universal_output_stream_t *const out_base = universal_null_output_stream_get_output_stream( &((*fix).out_stream) );
#endif
    utf8stream_writer_init( &((*fix).out_report), out_base );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    utf8stream_writer_destroy( &((*fix).out_report) );
#ifndef NDEBUG
    universal_stream_output_stream_destroy( &((*fix).out_stream) );
#else
    universal_null_output_stream_destroy( &((*fix).out_stream) );
#endif
    ctrl_controller_destroy( &((*fix).controller) );
    data_database_writer_destroy( &((*fix).db_writer) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static test_case_result_t diagram_two_roots_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create second root diagram */
    {
        data_diagram_t current_diagram;
        data_err = data_diagram_init( &current_diagram,
                                      2, /*=diagram_id*/
                                      DATA_ROW_VOID, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                      "stereo_t", /* stereotype */
                                      "diagram_name",
                                      "diagram_description",
                                      10222, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "89b87e06-2a0f-43bc-97cd-a0395b3d40be"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &current_diagram, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* two roots, not fixed */
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* two roots */
    TEST_EXPECT_EQUAL_INT( 1, fixed_errors );

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );  /* ok */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t diagram_missing_parent_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create 1 diagram */
    {
        data_diagram_t second_diagram;
        data_err = data_diagram_init( &second_diagram,
                                      2, /*=diagram_id*/
                                      0, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                      "stereo_t", /* stereotype */
                                      "diagram_name",
                                      "diagram_description",
                                      10222, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "725903d8-3001-4e3c-8208-2937c75def3b"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &second_diagram, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create another diagram */
    {
        data_diagram_t third_diagram;
        data_err = data_diagram_init( &third_diagram,
                                      4, /*=diagram_id*/
                                      17, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                      "3stereo_t", /* stereotype */
                                      "diagram_name-4",
                                      "diagram_description-4",
                                      10333, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "2955514a-5083-4cfc-a0bc-5cc965f0fb0e"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &third_diagram, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 without parent */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 without parent */
    TEST_EXPECT_EQUAL_INT( 2, fixed_errors );

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t diagram_circular_referenced_diagrams_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up(), but not needed for this test case */
    /* a diagramelement (id=13) is created by set_up(), but not needed for this test case */

    /* create 2nd diagram */
    {
        data_diagram_t second_diagram;
        data_err = data_diagram_init( &second_diagram,
                                      2, /*=diagram_id*/
                                      4 ,/*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                      "2stereo_t", /* stereotype */
                                      "diagram_name-2",
                                      "diagram_description-2",
                                      10222, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "af9f23c6-f92c-4d30-bf43-901606bbfd1a"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &second_diagram, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create another diagram */
    {
        data_diagram_t third_diagram;
        data_err = data_diagram_init( &third_diagram,
                                      4, /*=diagram_id*/
                                      2, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                      "3stereo_t", /* stereotype */
                                      "diagram_name-4",
                                      "diagram_description-4",
                                      10333, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "48190baf-4748-4573-a963-16bd84954c06"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &third_diagram, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 2, found_errors );  /* id-2+id-4 referencing each other as parent */
    TEST_EXPECT_EQUAL_INT( 2, fixed_errors );

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t diagram_nonreferencing_diagramelements_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create diagramelement without classifier */
    {
        data_diagramelement_t second_diagramelement;
        data_err = data_diagramelement_init( &second_diagramelement,
                                             15 /*=id*/,
                                             6 /*=diagram_id*/,
                                             18 /*=classifier_id*/,
                                             DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                             DATA_ROW_VOID,
                                             "038a9a09-a9c1-46a2-83b2-57152e84511c"
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &second_diagramelement, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create diagramelement without diagram */
    {
        data_diagramelement_t third_diagramelement;
        data_err = data_diagramelement_init( &third_diagramelement,
                                             17 /*=id*/,
                                             2 /*=diagram_id*/,
                                             12 /*=classifier_id*/,
                                             DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                             DATA_ROW_VOID,
                                             "ffc3a760-820f-4d97-9e73-69d50ca23a22"
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &third_diagramelement, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create diagramelement without diagram and without classifier */
    {
        data_diagramelement_t fourth_diagramelement;
        data_err = data_diagramelement_init( &fourth_diagramelement,
                                             19 /*=id*/,
                                             2 /*=diagram_id*/,
                                             18 /*=classifier_id*/,
                                             DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                             DATA_ROW_VOID,
                                             "a9caa99a-6f38-488d-aea4-5669835c6b1e"
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &fourth_diagramelement, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /* id-15,17,19 */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /* id-15,17,19 */
    TEST_EXPECT_EQUAL_INT( 3, fixed_errors );

    /* check the diagrams */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t diagram_illreferencing_diagramelements_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
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
                                       5000000 /* list order */,
                                       "3c7d1e4b-c067-4d1f-a143-f048507e5aba"
                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &((*fix).db_writer), &v_feature, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create 2nd diagramelement with non-existing focused feature */
    {
        data_diagramelement_t second_diagramelement;
        data_err = data_diagramelement_init ( &second_diagramelement,
                                   14 /*=id*/,
                                   6 /*=diagram_id*/,
                                   12 /*=classifier_id*/,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   18 /*=focused_feature_id*/,
                                   "ea4f0c24-85fc-4418-a513-ba21d0d6a60f"
                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &second_diagramelement, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create a second valid classifier */
    {
        data_classifier_t current_classifier;
        data_err = data_classifier_init ( &current_classifier,
                                          11 /*=classifier id*/,
                                          DATA_CLASSIFIER_TYPE_CLASS,
                                          "stereotype-11",
                                          "name-11",
                                          "description-11",
                                          -340 /*=x_order*/,
                                          -160 /*=y_order*/,
                                          -7000 /*=list_order*/,
                                          "1695c1e8-f978-4b20-9a5f-10d45b904608"
                                        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &((*fix).db_writer), &current_classifier, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create 3rd diagramelement with existing focused feature but wrong, inconsistent classifier */
    {
        data_diagramelement_t third_diagramelement;
        data_err = data_diagramelement_init( &third_diagramelement,
                                             15 /*=id*/,
                                             6 /*=diagram_id*/,
                                             11 /*=classifier_id*/,
                                             DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                             17 /*=focused_feature_id*/,
                                             "2200e7cf-edae-4da9-9c4c-dd6db50339fa"
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &third_diagramelement, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the diagramelements */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 2, found_errors );  /* 2nd and 3rd diagramelements have non-healthy references */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the diagramelements */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 2, found_errors );  /* 2nd and 3rd diagramelements have non-healthy references */
    TEST_EXPECT_EQUAL_INT( 2, fixed_errors );

    /* check the diagramelements */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t repair_unreferenced_classifiers( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
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
                                          DATA_CLASSIFIER_TYPE_INTERFACE,
                                          "stereotype-13",
                                          "name-13",
                                          "description-13",
                                          -35050 /*=x_order*/,
                                          -17070 /*=y_order*/,
                                          -7000 /*=list_order*/,
                                          "59de3f30-d8d6-4223-b864-29d7dcda9529"
                                        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &((*fix).db_writer), &current_classifier, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* id-13 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* id-13 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 1, fixed_errors );

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t repair_unreferenced_classifiers_2( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
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
                                          DATA_CLASSIFIER_TYPE_INTERFACE,
                                          "stereotype-c6",
                                          "name-c6",
                                          "description-c6",
                                          -4200 /*=x_order*/,
                                          -6200 /*=y_order*/,
                                          -7000 /*=list_order*/,
                                          "b131d8ba-aa6b-4574-b72d-ca14034dde38"
                                        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &((*fix).db_writer), &second_classifier, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
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
                                       5000000 /* list order */,
                                       "f3db90fb-871c-4e57-b8a5-987a248e90e5"
                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &((*fix).db_writer), &v_feature, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define a valid relationship */
    {
        data_relationship_t v_relation;
        data_err = data_relationship_init( &v_relation,
                                           34, /* relationship_id */
                                           6, /* from_classifier_id */
                                           DATA_ROW_VOID, /* from_feature_id */
                                           6, /* to_classifier_id */
                                           DATA_ROW_VOID, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "rel_stereo_t", /* stereotype */
                                           "the composition is more", /* relationship_name */
                                           "than the sum of its parts", /* relationship_description */
                                           -66000, /* list_order */
                                           "8ea7faa0-d01e-4862-903b-f2ab3e3ba40f"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &v_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* id-6 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /* id-6 is unreferenced, after deleting, also the relationship and the feature are unreferenced. */
    TEST_EXPECT_EQUAL_INT( 3, fixed_errors );

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t repair_invalid_feature_parent( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create a valid feature */
    {
        data_feature_t v_feature;
        data_err = data_feature_init( &v_feature,
                                      17, /* feature_id */
                                      DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                      12, /* classifier_id */
                                      "startup_time", /* feature_key */
                                      "uint64_t", /* feature_value */
                                      "time in nano seconds to start", /* feature_description */
                                      5000000, /* list order */
                                      "1653515a-d06e-4c65-a2b3-375343471573"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &((*fix).db_writer), &v_feature, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
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
                                       5000000, /* list order */
                                       "051a1c7a-f445-454d-bf3a-e5e22523a9bc"
                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &((*fix).db_writer), &i_feature, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* id-19 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 1, found_errors );  /* id-19 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 1, fixed_errors );

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t repair_invalid_relationship( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* define a valid relationship */
    {
        data_relationship_t v_relation;
        data_err = data_relationship_init( &v_relation,
                                           34, /* relationship_id */
                                           12, /* from_classifier_id */
                                           DATA_ROW_VOID, /* from_feature_id */
                                           12, /* to_classifier_id */
                                           DATA_ROW_VOID, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "3stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           -66000, /* list_order */
                                           "04ee369e-f3c9-4ead-9377-280917050bc4"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &v_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define first invalid relationship */
    {
        data_relationship_t i1_relation;
        data_err = data_relationship_init( &i1_relation,
                                           35, /* relationship_id */
                                           12, /* from_classifier_id */
                                           DATA_ROW_VOID, /* from_feature_id */
                                           12121212, /* to_classifier_id */
                                           DATA_ROW_VOID, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "4stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           -66000, /* list_order */
                                           "43da4f41-dede-48f2-bd38-29694d2cb332"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &i1_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define second invalid relationship */
    {
        data_relationship_t i2_relation;
        data_err = data_relationship_init( &i2_relation,
                                           36, /* relationship_id */
                                           12121212, /* from_classifier_id */
                                           DATA_ROW_VOID, /* from_feature_id */
                                           12, /* to_classifier_id */
                                           DATA_ROW_VOID, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "5stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           -66000, /* list_order */
                                           "2851e7db-19cc-484c-9cb9-f76562ac3ca5"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &i2_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define third invalid relationship */
    {
        data_relationship_t i3_relation;
        data_err = data_relationship_init( &i3_relation,
                                           37, /* relationship_id */
                                           12121212, /* from_classifier_id */
                                           DATA_ROW_VOID, /* from_feature_id */
                                           12121212, /* to_classifier_id */
                                           DATA_ROW_VOID, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "6stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           -66000, /* list_order */
                                           "40db1fee-8342-4c12-8d2d-60b937c87628"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &i3_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /* id-35,36,37 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /* id-35,36,37 is unreferenced */
    TEST_EXPECT_EQUAL_INT( 3, fixed_errors );

    /* check the database */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t repair_ill_feature_relationship( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    uint32_t found_errors;
    uint32_t fixed_errors;

    /* the root diagram (id=6) is created by set_up() */
    /* a classifier (id=12) is created by set_up() */
    /* a diagramelement (id=13) is created by set_up() */

    /* create a second classifier */
    {
        data_classifier_t second_classifier;
        data_err = data_classifier_init( &second_classifier,
                                         11, /*=classifier id*/
                                         DATA_CLASSIFIER_TYPE_PACKAGE,
                                         "stereotype-11",
                                         "name-11",
                                         "description-11",
                                         -4200, /*=x_order*/
                                         -6200, /*=y_order*/
                                         -7000, /*=list_order*/
                                         "2287cc58-dcf9-4e77-95f2-ce8b2e847faf"
                                       );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_classifier( &((*fix).db_writer), &second_classifier, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create a first valid feature */
    {
        data_feature_t first_feature;
        data_err = data_feature_init( &first_feature,
                                      18, /* feature_id */
                                      DATA_FEATURE_TYPE_LIFELINE, /* feature_main_type */
                                      12, /* classifier_id */
                                      "shutdown scenario", /* feature_key */
                                      "", /* feature_value */
                                      "timeline of shutdown", /* feature_description */
                                      500, /* list order */
                                      "7d07b607-4d30-42db-9d22-0190441bd9f5"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &((*fix).db_writer), &first_feature, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create a second valid feature */
    {
        data_feature_t second_feature;
        data_err = data_feature_init( &second_feature,
                                      17, /* feature_id */
                                      DATA_FEATURE_TYPE_PORT, /* feature_main_type */
                                      11, /* classifier_id */
                                      "Port 8080", /* feature_key */
                                      "ip_port", /* feature_value */
                                      "port for control commands", /* feature_description */
                                      1500, /* list order */
                                      "5bb9bc75-4b22-47c4-877d-9a6c28a5235b"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_feature ( &((*fix).db_writer), &second_feature, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* create a diagramelement for the second classifier */
    {
        data_diagramelement_t second_diagramelement;
        data_err = data_diagramelement_init( &second_diagramelement,
                                             14 /*=id*/,
                                             6 /*=diagram_id*/,
                                             11 /*=classifier_id*/,
                                             DATA_DIAGRAMELEMENT_FLAG_NONE,
                                             17, /*=focused_feature_id*/
                                             "31479db9-080e-48dc-b5cd-664006deb406"
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &second_diagramelement, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define a valid relationship */
    {
        data_relationship_t first_relation;
        data_err = data_relationship_init( &first_relation,
                                           34, /* relationship_id */
                                           11, /* from_classifier_id */
                                           17, /* from_feature_id */
                                           12, /* to_classifier_id */
                                           18, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "3stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           1600, /* list_order */
                                           "e87416fc-c792-4d9d-b267-914517343318"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &first_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define a relationship where source feature does not exist*/
    {
        data_relationship_t second_relation;
        data_err = data_relationship_init( &second_relation,
                                           35, /* relationship_id */
                                           11, /* from_classifier_id */
                                           170, /* from_feature_id */
                                           12, /* to_classifier_id */
                                           18, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "2stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           1700, /* list_order */
                                           "213f7bae-c694-420b-9afe-6b6bfa32e638"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &second_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define a relationship where destination feature has illegal classifier */
    {
        data_relationship_t third_relation;
        data_err = data_relationship_init( &third_relation,
                                           36, /* relationship_id */
                                           11, /* from_classifier_id */
                                           17, /* from_feature_id */
                                           12, /* to_classifier_id */
                                           17, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "1stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           1800, /* list_order */
                                           "d2d6858e-b670-46b7-8a6e-296b83f87c71"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &third_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* define a relationship where source has illegal classifier and destination feature non-existant */
    {
        data_relationship_t first_relation;
        data_err = data_relationship_init( &first_relation,
                                           37, /* relationship_id */
                                           11, /* from_classifier_id */
                                           18, /* from_feature_id */
                                           12, /* to_classifier_id */
                                           180, /* to_feature_id */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           "0stereo_t", /* stereotype */
                                           "the composition is more", /* name */
                                           "than the sum of its parts", /* description */
                                           1900, /* list_order */
                                           "a7ad15b5-5815-46e8-a069-7c8fc49b6f6b"
                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &first_relation, NULL /*=out_new_id*/ );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check the relationships */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, ctrl_err );  /* there are errors not yet fixed */
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /*! illegal: relationships 35, 36 and 37 */
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );

    /* fix the relationships */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), FIX_ERRORS, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 3, found_errors );  /*! illegal: relationships 35, 36 and 37 */
    TEST_EXPECT_EQUAL_INT( 3, fixed_errors );

    /* check the relationships */
    ctrl_err = ctrl_controller_repair_database ( &((*fix).controller), TEST_ONLY, &found_errors, &fixed_errors, &((*fix).out_report) );
    utf8stream_writer_flush( &((*fix).out_report) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 0, found_errors );
    TEST_EXPECT_EQUAL_INT( 0, fixed_errors );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2016-2026 Andreas Warnke
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
