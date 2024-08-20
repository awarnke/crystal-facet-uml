/* File: data_database_reader_test.c; Copyright and License: see below */

#include "data_database_reader_test.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <errno.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_search_diagrams( test_fixture_t *fix );
static test_case_result_t test_search_diagramelements( test_fixture_t *fix );
static test_case_result_t test_search_classifiers( test_fixture_t *fix );
static test_case_result_t test_search_features( test_fixture_t *fix );
static test_case_result_t test_search_relationships( test_fixture_t *fix );
static test_case_result_t test_iterate_over_classifiers( test_fixture_t *fix );

test_suite_t data_database_reader_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_database_reader_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_search_diagrams", &test_search_diagrams );
    test_suite_add_test_case( &result, "test_search_diagramelements", &test_search_diagramelements );
    test_suite_add_test_case( &result, "test_search_classifiers", &test_search_classifiers );
    test_suite_add_test_case( &result, "test_search_features", &test_search_features );
    test_suite_add_test_case( &result, "test_search_relationships", &test_search_relationships );
    test_suite_add_test_case( &result, "test_iterate_over_classifiers", &test_iterate_over_classifiers );
    return result;
}

/*!
 *  \brief database filename on which the tests are performed and which is automatically deleted when finished
 */
static const char *const DATABASE_FILENAME = "unittest_crystal_facet_uml_default.cfu1";

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    /* remove old database files first */
    int err;
    err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT ( ( 0 == err )|| (( -1 == err )&&( errno == ENOENT )) );

    /* open a database and initialize a reader and a writer */
    test_fixture_t *fix = &test_fixture;
    data_database_init( &((*fix).database) );
    data_database_open( &((*fix).database), DATABASE_FILENAME );
    data_database_reader_init( &((*fix).db_reader), &((*fix).database) );
    data_database_writer_init( &((*fix).db_writer), &((*fix).db_reader), &((*fix).database) );

    /* initialize some entries */
    u8_error_t data_err;

    /* create a valid root diagram */
    data_diagram_t root_diagram;
    data_err = data_diagram_init( &root_diagram,
                                  6, /*=diagram_id*/
                                  DATA_ROW_ID_VOID, /*=parent_diagram_id*/
                                  DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                  "st_t",  /* stereotype */
                                  "diagram_name-6",
                                  "diagram_description-6",
                                  10444, /*=list_order*/
                                  DATA_DIAGRAM_FLAG_EMPHASIS,
                                  "5aeb80ad-a3a1-4ee1-91a9-2f4d0dbd495a"
                                );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &root_diagram, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    /* create a valid child diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init( &current_diagram,
                                  7, /*=diagram_id*/
                                  6, /*=parent_diagram_id*/
                                  DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                  "st_t",  /* stereotype */
                                  "diagram_name-7",
                                  "diagram_description-7",
                                  10555, /*=list_order*/
                                  DATA_DIAGRAM_FLAG_GRAY_OUT,
                                  "f6d0084a-5d5b-4c26-8c64-782c150feec8"
                                );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagram ( &((*fix).db_writer), &current_diagram, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    /* create two valid classifiers */
    data_classifier_t current_classifier;
    data_err = data_classifier_init( &current_classifier,
                                     12, /*=classifier id*/
                                     DATA_CLASSIFIER_TYPE_INTERFACE,
                                     "stereotype-12",
                                     "name-12",
                                     "description-12",
                                     -34000, /*=x_order*/
                                     -16000, /*=y_order*/
                                     -7000, /*=list_order*/
                                     "1966d7f8-675d-4062-adc3-264672747377"
                                   );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_classifier( &((*fix).db_writer), &current_classifier, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_classifier_t second_classifier;
    data_err = data_classifier_init( &second_classifier,
                                     13, /*=classifier id*/
                                     DATA_CLASSIFIER_TYPE_INTERFACE,
                                     "stereotype-13",
                                     "name-13",
                                     "description-13",
                                     -34000, /*=x_order*/
                                     -48000, /*=y_order*/
                                     -58000, /*=list_order*/
                                     "b9495b71-99c3-406d-88d5-1aa233b09e2d"
                                   );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_classifier( &((*fix).db_writer), &second_classifier, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    /* create four valid diagramelements */
    data_diagramelement_t first_diagramelement;
    data_err = data_diagramelement_init( &first_diagramelement,
                                         130, /*=id*/
                                         6, /*=diagram_id*/
                                         12, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         DATA_ROW_ID_VOID,
                                         "c9dc1401-4b2c-48d2-a85c-efa6194f9aec"
                                       );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &first_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_diagramelement_t duplicate_diagramelement;
    data_err = data_diagramelement_init( &duplicate_diagramelement,
                                         131, /*=id*/
                                         7, /*=diagram_id*/
                                         12, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         8192,
                                         "11e795e5-3666-405a-8c56-26ace2f5a46e"
                                       );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &duplicate_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_diagramelement_t second_diagramelement;
    data_err = data_diagramelement_init( &second_diagramelement,
                                         132, /*=id*/
                                         7, /*=diagram_id*/
                                         12, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         DATA_ROW_ID_VOID,
                                         "ffde926f-efbf-45d5-bd71-ecd932f5143c"
                                       );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &second_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_diagramelement_t third_diagramelement;
    data_err = data_diagramelement_init( &third_diagramelement,
                                         133, /*=id*/
                                         7, /*=diagram_id*/
                                         13, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         DATA_ROW_ID_VOID,
                                         "02088b41-e71d-466d-a413-2551ba3bf10a"
                                       );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &((*fix).db_writer), &third_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    /* define two valid relationships */
    data_relationship_t v_relation;
    data_err = data_relationship_init( &v_relation,
                                       34, /* relationship_id */
                                       12, /* from_classifier_id */
                                       17, /* from_feature_id */
                                       13, /* to_classifier_id */
                                       18, /* to_feature_id */
                                       DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                       "st_t",  /* stereotype */
                                       "the composition is more", /* relationship_name */
                                       "than the sum of its parts", /* relationship_description */
                                       -66000, /* list_order */
                                       "ef90ab9d-6da4-4f3c-b8b8-50d9c955f113"
                                     );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &v_relation, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_relationship_t second_relation;
    data_err = data_relationship_init( &second_relation,
                                       35, /* relationship_id */
                                       13, /* from_classifier_id */
                                       DATA_ROW_ID_VOID, /* from_feature_id */
                                       13, /* to_classifier_id */
                                       DATA_ROW_ID_VOID, /* to_feature_id */
                                       DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, /* relationship_main_type */
                                       "st_t",  /* stereotype */
                                       "the composition is more", /* relationship_name */
                                       "than the sum of its parts", /* relationship_description */
                                       -77000, /* list_order */
                                       "100f8e76-ef34-4735-b7e1-fe8077b91184"
                                     );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_relationship ( &((*fix).db_writer), &second_relation, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    /* create three valid features */
    data_feature_t v_feature;
    data_err = data_feature_init( &v_feature,
                                  17, /* feature_id */
                                  DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                  12, /* classifier_id */
                                  "startup_time", /* feature_key */
                                  "uint64_t", /* feature_value */
                                  "time in nano seconds to start", /* feature_description */
                                  5000000, /* list order */
                                  "5d13a3d4-4f14-4d2f-ae13-137d9082839f"
                                );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &((*fix).db_writer), &v_feature, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_feature_t second_feature;
    data_err = data_feature_init( &second_feature,
                                  18, /* feature_id */
                                  DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                  13, /* classifier_id */
                                  "startup_time", /* feature_key */
                                  "uint64_t", /* feature_value */
                                  "time in nano seconds to start", /* feature_description */
                                  5000000, /* list order */
                                  "17d8377a-cf84-402c-b4d8-0dbfc8f8222e"
                                );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &((*fix).db_writer), &second_feature, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_feature_t thrid_feature;
    data_err = data_feature_init( &thrid_feature,
                                  19, /* feature_id */
                                  DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                  13, /* classifier_id */
                                  "shutdown_time", /* feature_key */
                                  "uint64_t", /* feature_value */
                                  "time in nano seconds to shut down", /* feature_description */
                                  5000000, /* list order */
                                  "56a4cb87-b6d0-4f8f-aab1-ccdca445d323"
                                );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &((*fix).db_writer), &thrid_feature, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == data_err );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );

    /* de-initialize the reader and the writer and close the database */
    data_database_writer_destroy( &((*fix).db_writer) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
    const int stdio_err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT( stdio_err == 0 );
}

static test_case_result_t test_search_diagrams( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_diagram_t diagram_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_diagram_count;

    /* test 1 */
    data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), 7, &(diagram_list[0]) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_diagram_by_uuid ( &((*fix).db_reader),
                                                          "f6d0084a-5d5b-4c26-8c64-782c150feec8",
                                                          &(diagram_list[0])
                                                        );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 7, data_diagram_get_row_id( &(diagram_list[0]) ) );

    /* test 2 */
    data_err = data_database_reader_get_diagrams_by_parent_id ( &((*fix).db_reader),
                                                                6,
                                                                MAX_ARRAY_SIZE,
                                                                &(diagram_list),
                                                                &out_diagram_count
                                                              );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 1, out_diagram_count );

    /* test 3 */
    data_small_set_t out_diagram_ids;
    data_small_set_init( &out_diagram_ids );
    data_err = data_database_reader_get_diagram_ids_by_parent_id ( &((*fix).db_reader),
                                                                   6,
                                                                   &out_diagram_ids
                                                                 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 1, data_small_set_get_count( &out_diagram_ids ) );

    /* test 4 */
    data_err = data_database_reader_get_diagrams_by_classifier_id ( &((*fix).db_reader),
                                                                    12,
                                                                    MAX_ARRAY_SIZE,
                                                                    &(diagram_list),
                                                                    &out_diagram_count
                                                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 2, out_diagram_count );

    /* test 5 */
    data_small_set_t out_showing_diagram_ids;
    data_small_set_init( &out_showing_diagram_ids );
    data_err = data_database_reader_get_diagram_ids_by_classifier_id ( &((*fix).db_reader),
                                                                       12,
                                                                       &out_showing_diagram_ids
                                                                     );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 2, data_small_set_get_count( &out_showing_diagram_ids ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_search_diagramelements( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_diagramelement_t out_diagramelement;

    /* test 1 */
    data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), 130, &out_diagramelement );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_diagramelement_by_uuid ( &((*fix).db_reader),
                                                                 "02088b41-e71d-466d-a413-2551ba3bf10a",
                                                                 &out_diagramelement
                                                               );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 133, data_diagramelement_get_row_id( &out_diagramelement ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_search_classifiers( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_classifier_t out_classifier;
    data_visible_classifier_t visible_classifier_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_classifier_count;

    /* test 1 */
    data_err = data_database_reader_get_classifier_by_id ( &((*fix).db_reader), 13, &out_classifier );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_classifier_by_uuid ( &((*fix).db_reader),
                                                             "b9495b71-99c3-406d-88d5-1aa233b09e2d",
                                                             &out_classifier
                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 13, data_classifier_get_row_id( &out_classifier ) );

    /* test 2 */
    data_err = data_database_reader_get_classifiers_by_diagram_id ( &((*fix).db_reader),
                                                                    7,
                                                                    MAX_ARRAY_SIZE,
                                                                    &(visible_classifier_list),
                                                                    &out_classifier_count
                                                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 3, out_classifier_count );

    /* test 3 */
    data_err = data_database_reader_get_classifier_by_name ( &((*fix).db_reader),
                                                             "name-12",
                                                             &out_classifier
                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 12, data_classifier_get_row_id( &out_classifier ) );

    /* test 4 */
    data_err = data_database_reader_get_classifier_by_name ( &((*fix).db_reader),
                                                             "does not exist",
                                                             &out_classifier
                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NOT_FOUND, data_err );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_search_features( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_feature_t feature_list[4];
    static const int MAX_ARRAY_SIZE = 4;
    uint32_t out_feature_count;

    /* test 1 */
    data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), 19, &(feature_list[0]) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_feature_by_uuid ( &((*fix).db_reader),
                                                          "17d8377a-cf84-402c-b4d8-0dbfc8f8222e",
                                                          &(feature_list[0])
                                                        );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 18, data_feature_get_row_id( &(feature_list[0]) ) );

    /* test 2 */
    data_err = data_database_reader_get_features_by_classifier_id ( &((*fix).db_reader),
                                                                    13, /* classifier_id*/
                                                                    MAX_ARRAY_SIZE,
                                                                    &(feature_list),
                                                                    &out_feature_count
                                                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 2, out_feature_count );

    /* test 3 */
    data_err = data_database_reader_get_features_by_diagram_id ( &((*fix).db_reader),
                                                                 7, /* diagram_id */
                                                                 MAX_ARRAY_SIZE,
                                                                 &(feature_list),
                                                                 &out_feature_count
                                                               );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 3, out_feature_count );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_search_relationships( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_relationship_t relation_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_relationship_count;

    /* test 1 */
    data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), 34, &(relation_list[0]) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_relationship_by_uuid ( &((*fix).db_reader),
                                                               "ef90ab9d-6da4-4f3c-b8b8-50d9c955f113",
                                                               &(relation_list[0])
                                                             );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 34, data_relationship_get_row_id( &(relation_list[0]) ) );

    /* test 2 */
    data_err = data_database_reader_get_relationships_by_classifier_id ( &((*fix).db_reader),
                                                                         13, /*classifier_id*/
                                                                         MAX_ARRAY_SIZE,
                                                                         &(relation_list),
                                                                         &out_relationship_count
                                                                       );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 2, out_relationship_count );

    /* test 3 */
    data_err = data_database_reader_get_relationships_by_diagram_id ( &((*fix).db_reader),
                                                                      7, /* diagram_id */
                                                                      MAX_ARRAY_SIZE,
                                                                      &(relation_list),
                                                                      &out_relationship_count
                                                                    );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 2, out_relationship_count );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_iterate_over_classifiers( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_database_iterator_classifiers_t classifier_iterator;
    data_classifier_t out_classifier;
    bool has_next;

    /* test the iterator, init */
    data_database_iterator_classifiers_init_empty( &classifier_iterator );
    data_err = data_database_reader_get_all_classifiers_iterator ( &((*fix).db_reader), true, &classifier_iterator );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* test the iterator, step 1 */
    has_next = data_database_iterator_classifiers_has_next( &classifier_iterator );
    TEST_EXPECT( has_next );
    data_err = data_database_iterator_classifiers_next( &classifier_iterator, &out_classifier );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 12, data_classifier_get_row_id( &out_classifier ) );

    /* test the iterator, step 2 */
    has_next = data_database_iterator_classifiers_has_next( &classifier_iterator );
    TEST_EXPECT( has_next );
    data_err = data_database_iterator_classifiers_next( &classifier_iterator, &out_classifier );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( 13, data_classifier_get_row_id( &out_classifier ) );

    /* test the iterator, step 3 */
    has_next = data_database_iterator_classifiers_has_next( &classifier_iterator );
    TEST_EXPECT( ! has_next );
    data_err = data_database_iterator_classifiers_next( &classifier_iterator, &out_classifier );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, data_err );

    /* test the iterator, destroy */
    data_err = data_database_iterator_classifiers_destroy( &classifier_iterator );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2024 Andreas Warnke
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
