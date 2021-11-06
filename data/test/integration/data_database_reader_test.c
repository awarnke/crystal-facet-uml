/* File: data_database_reader_test.c; Copyright and License: see below */

#include "data_database_reader_test.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include "test_assert.h"
#include <errno.h>

static void set_up(void);
static void tear_down(void);
static void test_search_diagrams(void);
static void test_search_diagramelements(void);
static void test_search_classifiers(void);
static void test_search_features(void);
static void test_search_relationships(void);
static void test_iterate_over_classifiers(void);

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

test_suite_t data_database_reader_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_database_reader_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_search_diagrams", &test_search_diagrams );
    test_suite_add_test_case( &result, "test_search_diagramelements", &test_search_diagramelements );
    test_suite_add_test_case( &result, "test_search_classifiers", &test_search_classifiers );
    test_suite_add_test_case( &result, "test_search_features", &test_search_features );
    test_suite_add_test_case( &result, "test_search_relationships", &test_search_relationships );
    test_suite_add_test_case( &result, "test_iterate_over_classifiers", &test_iterate_over_classifiers );
    return result;
}

static void set_up(void)
{
    /* remove old database files first */
    int err;
    err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT ( ( 0 == err )|| (( -1 == err )&&( errno == ENOENT )) );

    /* open a database and initialize a reader and a writer */
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    /* initialize some entries */
    data_error_t data_err;

    /* create a valid root diagram */
    data_diagram_t root_diagram;
    data_err = data_diagram_init( &root_diagram,
                                  6, /*=diagram_id*/
                                  DATA_ROW_ID_VOID, /*=parent_diagram_id*/
                                  DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                  "diagram_name-6",
                                  "diagram_description-6",
                                  10444, /*=list_order*/
                                  DATA_DIAGRAM_FLAG_EMPHASIS,
                                  "5aeb80ad-a3a1-4ee1-91a9-2f4d0dbd495a"
                                );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &root_diagram, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    /* create a valid child diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init( &current_diagram,
                                  7, /*=diagram_id*/
                                  6, /*=parent_diagram_id*/
                                  DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                  "diagram_name-7",
                                  "diagram_description-7",
                                  10555, /*=list_order*/
                                  DATA_DIAGRAM_FLAG_GRAY_OUT,
                                  "f6d0084a-5d5b-4c26-8c64-782c150feec8"
                                );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

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
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

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
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_classifier( &db_writer, &second_classifier, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

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
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &db_writer, &first_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_diagramelement_t duplicate_diagramelement;
    data_err = data_diagramelement_init( &duplicate_diagramelement,
                                         131, /*=id*/
                                         7, /*=diagram_id*/
                                         12, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         8192,
                                         "11e795e5-3666-405a-8c56-26ace2f5a46e"
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &db_writer, &duplicate_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_diagramelement_t second_diagramelement;
    data_err = data_diagramelement_init( &second_diagramelement,
                                         132, /*=id*/
                                         7, /*=diagram_id*/
                                         12, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         DATA_ROW_ID_VOID,
                                         "ffde926f-efbf-45d5-bd71-ecd932f5143c"
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &db_writer, &second_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_diagramelement_t third_diagramelement;
    data_err = data_diagramelement_init( &third_diagramelement,
                                         133, /*=id*/
                                         7, /*=diagram_id*/
                                         13, /*=classifier_id*/
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         DATA_ROW_ID_VOID,
                                         "02088b41-e71d-466d-a413-2551ba3bf10a"
                                       );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagramelement( &db_writer, &third_diagramelement, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    /* define two valid relationships */
    data_relationship_t v_relation;
    data_err = data_relationship_init( &v_relation,
                                       34, /* relationship_id */
                                       DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                       12, /* from_classifier_id */
                                       13, /* to_classifier_id */
                                       "the composition is more", /* relationship_name */
                                       "than the sum of its parts", /* relationship_description */
                                       -66000, /* list_order */
                                       17, /* from_feature_id */
                                       18, /* to_feature_id */
                                       "ef90ab9d-6da4-4f3c-b8b8-50d9c955f113"
                                     );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_relationship ( &db_writer, &v_relation, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_relationship_t second_relation;
    data_err = data_relationship_init( &second_relation,
                                       35, /* relationship_id */
                                       DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, /* relationship_main_type */
                                       13, /* from_classifier_id */
                                       13, /* to_classifier_id */
                                       "the composition is more", /* relationship_name */
                                       "than the sum of its parts", /* relationship_description */
                                       -77000, /* list_order */
                                       DATA_ROW_ID_VOID, /* from_feature_id */
                                       DATA_ROW_ID_VOID, /* to_feature_id */
                                       "100f8e76-ef34-4735-b7e1-fe8077b91184"
                                     );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_relationship ( &db_writer, &second_relation, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

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
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &db_writer, &v_feature, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

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
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &db_writer, &second_feature, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

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
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &db_writer, &thrid_feature, NULL /*=out_new_id*/ );
    TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );
}

static void tear_down(void)
{
    int err;

    /* de-initialize the reader and the writer and close the database */
    data_database_writer_destroy( &db_writer );
    data_database_reader_destroy( &db_reader );

    data_database_close( &database );
    data_database_destroy( &database );
    err = remove( DATABASE_FILENAME );
    TEST_ENVIRONMENT_ASSERT( err == 0 );
}

static void test_search_diagrams(void)
{
    data_error_t data_err;
    data_diagram_t diagram_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_diagram_count;

    /* test 1 */
    data_err = data_database_reader_get_diagram_by_id ( &db_reader, 7, &(diagram_list[0]) );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_diagram_by_uuid ( &db_reader,
                                                          "f6d0084a-5d5b-4c26-8c64-782c150feec8",
                                                          &(diagram_list[0])
                                                        );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 7, data_diagram_get_row_id( &(diagram_list[0]) ) );

    /* test 2 */
    data_err = data_database_reader_get_diagrams_by_parent_id ( &db_reader,
                                                                6,
                                                                MAX_ARRAY_SIZE,
                                                                &(diagram_list),
                                                                &out_diagram_count
                                                              );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, out_diagram_count );

    /* test 3 */
    data_small_set_t out_diagram_ids;
    data_small_set_init( &out_diagram_ids );
    data_err = data_database_reader_get_diagram_ids_by_parent_id ( &db_reader,
                                                                   6,
                                                                   &out_diagram_ids
                                                                 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 1, data_small_set_get_count( &out_diagram_ids ) );

    /* test 4 */
    data_err = data_database_reader_get_diagrams_by_classifier_id ( &db_reader,
                                                                    12,
                                                                    MAX_ARRAY_SIZE,
                                                                    &(diagram_list),
                                                                    &out_diagram_count
                                                                  );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_diagram_count );

    /* test 5 */
    data_small_set_t out_showing_diagram_ids;
    data_small_set_init( &out_showing_diagram_ids );
    data_err = data_database_reader_get_diagram_ids_by_classifier_id ( &db_reader,
                                                                       12,
                                                                       &out_showing_diagram_ids
                                                                     );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, data_small_set_get_count( &out_showing_diagram_ids ) );
}

static void test_search_diagramelements(void)
{
    data_error_t data_err;
    data_diagramelement_t out_diagramelement;

    /* test 1 */
    data_err = data_database_reader_get_diagramelement_by_id ( &db_reader, 130, &out_diagramelement );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_diagramelement_by_uuid ( &db_reader,
                                                                 "02088b41-e71d-466d-a413-2551ba3bf10a",
                                                                 &out_diagramelement
                                                               );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 133, data_diagramelement_get_row_id( &out_diagramelement ) );
}

static void test_search_classifiers(void)
{
    data_error_t data_err;
    data_classifier_t out_classifier;
    data_visible_classifier_t visible_classifier_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_classifier_count;

    /* test 1 */
    data_err = data_database_reader_get_classifier_by_id ( &db_reader, 13, &out_classifier );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_classifier_by_uuid ( &db_reader,
                                                             "b9495b71-99c3-406d-88d5-1aa233b09e2d",
                                                             &out_classifier
                                                           );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 13, data_classifier_get_row_id( &out_classifier ) );

    /* test 2 */
    data_err = data_database_reader_get_classifiers_by_diagram_id ( &db_reader,
                                                                    7,
                                                                    MAX_ARRAY_SIZE,
                                                                    &(visible_classifier_list),
                                                                    &out_classifier_count
                                                                  );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 3, out_classifier_count );

    /* test 3 */
    data_err = data_database_reader_get_classifier_by_name ( &db_reader,
                                                             "name-12",
                                                             &out_classifier
                                                           );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 12, data_classifier_get_row_id( &out_classifier ) );

    /* test 4 */
    data_err = data_database_reader_get_classifier_by_name ( &db_reader,
                                                             "does not exist",
                                                             &out_classifier
                                                           );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_DB_STRUCTURE, data_err );
}

static void test_search_features(void)
{
    data_error_t data_err;
    data_feature_t feature_list[4];
    static const int MAX_ARRAY_SIZE = 4;
    uint32_t out_feature_count;

    /* test 1 */
    data_err = data_database_reader_get_feature_by_id ( &db_reader, 19, &(feature_list[0]) );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_feature_by_uuid ( &db_reader,
                                                          "17d8377a-cf84-402c-b4d8-0dbfc8f8222e",
                                                          &(feature_list[0])
                                                        );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 18, data_feature_get_row_id( &(feature_list[0]) ) );

    /* test 2 */
    data_err = data_database_reader_get_features_by_classifier_id ( &db_reader,
                                                                    13, /* classifier_id*/
                                                                    MAX_ARRAY_SIZE,
                                                                    &(feature_list),
                                                                    &out_feature_count
                                                                  );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_feature_count );

    /* test 3 */
    data_err = data_database_reader_get_features_by_diagram_id ( &db_reader,
                                                                 7, /* diagram_id */
                                                                 MAX_ARRAY_SIZE,
                                                                 &(feature_list),
                                                                 &out_feature_count
                                                               );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 3, out_feature_count );
}

static void test_search_relationships(void)
{
    data_error_t data_err;
    data_relationship_t relation_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_relationship_count;

    /* test 1 */
    data_err = data_database_reader_get_relationship_by_id ( &db_reader, 34, &(relation_list[0]) );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* test 1b */
    data_err = data_database_reader_get_relationship_by_uuid ( &db_reader,
                                                               "ef90ab9d-6da4-4f3c-b8b8-50d9c955f113",
                                                               &(relation_list[0])
                                                             );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 34, data_relationship_get_row_id( &(relation_list[0]) ) );

    /* test 2 */
    data_err = data_database_reader_get_relationships_by_classifier_id ( &db_reader,
                                                                         13, /*classifier_id*/
                                                                         MAX_ARRAY_SIZE,
                                                                         &(relation_list),
                                                                         &out_relationship_count
                                                                       );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_relationship_count );

    /* test 3 */
    data_err = data_database_reader_get_relationships_by_diagram_id ( &db_reader,
                                                                      7, /* diagram_id */
                                                                      MAX_ARRAY_SIZE,
                                                                      &(relation_list),
                                                                      &out_relationship_count
                                                                    );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_relationship_count );
}

static void test_iterate_over_classifiers(void)
{
    data_error_t data_err;
    data_database_iterator_classifiers_t classifier_iterator;
    data_classifier_t out_classifier;
    bool has_next;

    /* test the iterator, init */
    data_database_iterator_classifiers_init_empty( &classifier_iterator );
    data_err = data_database_reader_get_all_classifiers_iterator ( &db_reader, &classifier_iterator );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    /* test the iterator, step 1 */
    has_next = data_database_iterator_classifiers_has_next( &classifier_iterator );
    TEST_ASSERT( has_next );
    data_err = data_database_iterator_classifiers_next( &classifier_iterator, &out_classifier );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 12, data_classifier_get_row_id( &out_classifier ) );

    /* test the iterator, step 2 */
    has_next = data_database_iterator_classifiers_has_next( &classifier_iterator );
    TEST_ASSERT( has_next );
    data_err = data_database_iterator_classifiers_next( &classifier_iterator, &out_classifier );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 13, data_classifier_get_row_id( &out_classifier ) );

    /* test the iterator, step 3 */
    has_next = data_database_iterator_classifiers_has_next( &classifier_iterator );
    TEST_ASSERT( ! has_next );
    data_err = data_database_iterator_classifiers_next( &classifier_iterator, &out_classifier );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_INVALID_REQUEST, data_err );

    /* test the iterator, destroy */
    data_err = data_database_iterator_classifiers_destroy( &classifier_iterator );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
}


/*
 * Copyright 2017-2021 Andreas Warnke
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
