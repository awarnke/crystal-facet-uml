/* File: data_database_reader_test.c; Copyright and License: see below */

#include "data_database_reader_test.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_writer.h"
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_search_features(void);
static void test_search_relationships(void);

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

TestRef data_database_reader_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_search_features",test_search_features),
        new_TestFixture("test_search_relationships",test_search_relationships),
    };
    EMB_UNIT_TESTCALLER(result,"data_small_set_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
    /* open a database and initialize a reader and a writer */
    data_database_init( &database );
    data_database_open( &database, DATABASE_FILENAME );

    data_database_reader_init( &db_reader, &database );
    data_database_writer_init( &db_writer, &db_reader, &database );

    /* initialize some entries */
    data_error_t data_err;

    /* create a valid root diagram */
    data_diagram_t root_diagram;
    data_err = data_diagram_init ( &root_diagram,
                                   6 /*=diagram_id*/,
                                   DATA_ID_VOID_ID /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-6",
                                   "diagram_description-6",
                                   10444 /*=list_order*/
    );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &root_diagram, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    /* create a valid child diagram */
    data_diagram_t current_diagram;
    data_err = data_diagram_init ( &current_diagram,
                                   7 /*=diagram_id*/,
                                   6 /*=parent_diagram_id*/,
                                   DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
                                   "diagram_name-7",
                                   "diagram_description-7",
                                   10555 /*=list_order*/
    );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_diagram ( &db_writer, &current_diagram, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    /* create two valid classifiers */
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
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_classifier( &db_writer, &current_classifier, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_classifier_t second_classifier;
    data_err = data_classifier_init ( &second_classifier,
                                      13 /*=classifier id*/,
                                      DATA_CLASSIFIER_TYPE_UML_INTERFACE,
                                      "stereotype-13",
                                      "name-13",
                                      "description-13",
                                      -34000 /*=x_order*/,
                                      -48000 /*=y_order*/
    );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_classifier( &db_writer, &second_classifier, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    /* create four valid diagramelements */
    data_diagramelement_t first_diagramelement;
    data_diagramelement_init ( &first_diagramelement,
                               130 /*=id*/,
                               6 /*=diagram_id*/,
                               12 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &first_diagramelement, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_diagramelement_t duplicate_diagramelement;
    data_diagramelement_init ( &duplicate_diagramelement,
                               131 /*=id*/,
                               7 /*=diagram_id*/,
                               12 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &duplicate_diagramelement, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_diagramelement_t second_diagramelement;
    data_diagramelement_init ( &second_diagramelement,
                               132 /*=id*/,
                               7 /*=diagram_id*/,
                               12 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &second_diagramelement, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_diagramelement_t third_diagramelement;
    data_diagramelement_init ( &third_diagramelement,
                               133 /*=id*/,
                               7 /*=diagram_id*/,
                               13 /*=classifier_id*/,
                               DATA_DIAGRAMELEMENT_FLAG_EMPHASIS
    );

    data_err = data_database_writer_create_diagramelement( &db_writer, &third_diagramelement, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    /* define two valid relationships */
    data_relationship_t v_relation;
    data_err = data_relationship_init ( &v_relation,
                                        34, /* relationship_id */
                                        DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                        12, /* from_classifier_id */
                                        13, /* to_classifier_id */
                                        "the composition is more", /* relationship_name */
                                        "than the sum of its parts", /* relationship_description */
                                        -66000 /* list_order */ );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_relationship ( &db_writer, &v_relation, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_relationship_t second_relation;
    data_err = data_relationship_init ( &second_relation,
                                        35, /* relationship_id */
                                        DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                        13, /* from_classifier_id */
                                        13, /* to_classifier_id */
                                        "the composition is more", /* relationship_name */
                                        "than the sum of its parts", /* relationship_description */
                                        -77000 /* list_order */ );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_relationship ( &db_writer, &second_relation, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    /* create three valid features */
    data_feature_t v_feature;
    data_err = data_feature_init ( &v_feature,
                                   17, /* feature_id */
                                   DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                   12, /* classifier_id */
                                   "startup_time", /* feature_key */
                                   "uint64_t", /* feature_value */
                                   "time in nano seconds to start", /* feature_description */
                                   5000000 /* list order */ );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &db_writer, &v_feature, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_feature_t second_feature;
    data_err = data_feature_init ( &second_feature,
                                   18, /* feature_id */
                                   DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                   13, /* classifier_id */
                                   "startup_time", /* feature_key */
                                   "uint64_t", /* feature_value */
                                   "time in nano seconds to start", /* feature_description */
                                   5000000 /* list order */ );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &db_writer, &second_feature, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );

    data_feature_t thrid_feature;
    data_err = data_feature_init ( &thrid_feature,
                                   19, /* feature_id */
                                   DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                   13, /* classifier_id */
                                   "shutdown_time", /* feature_key */
                                   "uint64_t", /* feature_value */
                                   "time in nano seconds to shut down", /* feature_description */
                                   5000000 /* list order */ );
    assert( DATA_ERROR_NONE == data_err );

    data_err = data_database_writer_create_feature ( &db_writer, &thrid_feature, NULL /*=out_new_id*/ );
    assert( DATA_ERROR_NONE == data_err );
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
    assert( err == 0 );
}

static void test_search_features(void)
{
    data_error_t data_err;
    data_feature_t feature_list[4];
    static const int MAX_ARRAY_SIZE = 4;
    uint32_t out_feature_count;

    data_err = data_database_reader_get_feature_by_id ( &db_reader, 19, &(feature_list[0]) );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_reader_get_features_by_classifier_id ( &db_reader,
                                                                    13, /* classifier_id*/
                                                                    MAX_ARRAY_SIZE,
                                                                    &(feature_list),
                                                                    &out_feature_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_feature_count );

    data_err = data_database_reader_get_features_by_diagram_id ( &db_reader,
                                                                 7, /* diagram_id */
                                                                 MAX_ARRAY_SIZE,
                                                                 &(feature_list),
                                                                 &out_feature_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 3, out_feature_count );
}

static void test_search_relationships(void)
{
    data_error_t data_err;
    data_relationship_t relation_list[3];
    static const int MAX_ARRAY_SIZE = 3;
    uint32_t out_relationship_count;

    data_err = data_database_reader_get_relationship_by_id ( &db_reader, 34, &(relation_list[0]) );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );

    data_err = data_database_reader_get_relationships_by_classifier_id ( &db_reader,
                                                                         13, /*classifier_id*/
                                                                         MAX_ARRAY_SIZE,
                                                                         &(relation_list),
                                                                         &out_relationship_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_relationship_count );

    data_err = data_database_reader_get_relationships_by_diagram_id ( &db_reader,
                                                                      7, /* diagram_id */
                                                                      MAX_ARRAY_SIZE,
                                                                      &(relation_list),
                                                                      &out_relationship_count );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, data_err );
    TEST_ASSERT_EQUAL_INT( 2, out_relationship_count );
}


/*
 * Copyright 2017-2017 Andreas Warnke
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
