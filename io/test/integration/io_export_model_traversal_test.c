/* File: io_export_model_traversal_test.c; Copyright and License: see below */

#include "io_export_model_traversal_test.h"
#include "io_export_model_traversal.h"
#include "test_env/test_env_setup_data.h"
#include "test_result_check/test_result_check_xml.h"
#include "xmi/xmi_element_writer.h"
#include "u8stream/universal_memory_output_stream.h"
#include "set/data_stat.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8/u8_trace.h"
#include "test_expect.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t iterate_types_on_mini_model( test_fixture_t *test_env );

/*!
 *  \brief helper function to initialize the database
 */
static void create_mini_model( data_row_id_t * out_root_diagram,
                               data_row_id_t * out_from_classifier_parent,
                               data_row_id_t * out_from_classifier,
                               data_row_id_t * out_from_feature,
                               data_row_id_t * out_to_classifier_parent,
                               data_row_id_t * out_to_classifier,
                               data_row_id_t * out_to_feature,
                               data_row_id_t * out_relation_clas_clas,
                               data_row_id_t * out_relation_clas_feat,
                               data_row_id_t * out_relation_feat_clas,
                               data_row_id_t * out_relation_feat_feat
                             );

/*!
 *  \brief database instance on which the tests are performed
 */
static data_database_t database;

/*!
 *  \brief database reader to access the database
 */
static data_database_reader_t db_reader;

/*!
 *  \brief controller instance on which the tests are performed
 */
static ctrl_controller_t controller;

/*!
 *  \brief stream to write to a memory buffer
 */
static universal_memory_output_stream_t mem_output_stream;

/*!
 *  \brief mem-buffer to be written by mem_output_stream
 */
static char mem_buffer[16384];

test_suite_t io_export_model_traversal_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "io_export_model_traversal_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "iterate_types_on_mini_model", &iterate_types_on_mini_model );
    return result;
}

static test_fixture_t * set_up()
{
    data_database_init( &database );
    data_database_open_in_memory( &database );
    data_database_reader_init( &db_reader, &database );
    ctrl_controller_init( &controller, &database );

    universal_memory_output_stream_init( &mem_output_stream,
                                         &mem_buffer,
                                         sizeof(mem_buffer)
                                       );
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
    universal_memory_output_stream_destroy( &mem_output_stream );

    ctrl_controller_destroy( &controller );
    data_database_reader_destroy( &db_reader );
    data_database_close( &database );
    data_database_destroy( &database );
}

static void create_mini_model( data_row_id_t * out_root_diagram,
                               data_row_id_t * out_from_classifier_parent,
                               data_row_id_t * out_from_classifier,
                               data_row_id_t * out_from_feature,
                               data_row_id_t * out_to_classifier_parent,
                               data_row_id_t * out_to_classifier,
                               data_row_id_t * out_to_feature,
                               data_row_id_t * out_relation_clas_clas,
                               data_row_id_t * out_relation_clas_feat,
                               data_row_id_t * out_relation_feat_clas,
                               data_row_id_t * out_relation_feat_feat )
{
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &controller );
    u8_error_t c_err;
    test_env_setup_t test_env;
    test_env_setup_init( &test_env, &controller );

    *out_root_diagram = test_env_setup_data_create_diagram( &test_env, DATA_ROW_ID_VOID, "root diag" );

    *out_from_classifier_parent = test_env_setup_data_create_classifier( &test_env, "from parent" );
    test_env_setup_data_create_diagramelement( &test_env, *out_root_diagram, *out_from_classifier_parent, DATA_ROW_ID_VOID );

    *out_from_classifier = test_env_setup_data_create_classifier( &test_env, "from classifier" );
    *out_from_feature = test_env_setup_data_create_feature( &test_env, *out_from_classifier, "from feature" );
    test_env_setup_data_create_diagramelement( &test_env, *out_root_diagram, *out_from_classifier, *out_from_feature );

    *out_to_classifier_parent = test_env_setup_data_create_classifier( &test_env, "to parent" );
    test_env_setup_data_create_diagramelement( &test_env, *out_root_diagram, *out_to_classifier_parent, DATA_ROW_ID_VOID );

    *out_to_classifier = test_env_setup_data_create_classifier( &test_env, "to classifier" );
    *out_to_feature = test_env_setup_data_create_feature( &test_env, *out_to_classifier, "to feature" );
    test_env_setup_data_create_diagramelement( &test_env, *out_root_diagram, *out_to_classifier, *out_to_feature );

    /* from child has parent */
    {
        data_row_id_t from_parent_rel_id = test_env_setup_data_create_relationship( &test_env,
                                                                                    *out_from_classifier_parent,
                                                                                    DATA_ROW_ID_VOID,
                                                                                    *out_from_classifier,
                                                                                    DATA_ROW_ID_VOID,
                                                                                    "from child rel"
                                                                                  );
        c_err = ctrl_classifier_controller_update_relationship_main_type ( classifier_ctrl,
                                                                           from_parent_rel_id,
                                                                           DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT
                                                                         );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == c_err );
    }

    /* to child has parent */
    {
        data_row_id_t to_parent_rel_id = test_env_setup_data_create_relationship( &test_env,
                                                                                  *out_to_classifier_parent,
                                                                                  DATA_ROW_ID_VOID,
                                                                                  *out_to_classifier,
                                                                                  DATA_ROW_ID_VOID,
                                                                                  "to child rel"
                                                                                );
        c_err = ctrl_classifier_controller_update_relationship_main_type ( classifier_ctrl,
                                                                           to_parent_rel_id,
                                                                           DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT
                                                                         );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == c_err );
    }

    *out_relation_clas_clas = test_env_setup_data_create_relationship( &test_env,
                                                                       *out_from_classifier_parent,
                                                                       DATA_ROW_ID_VOID,
                                                                       *out_to_classifier,
                                                                       DATA_ROW_ID_VOID,
                                                                       "from classifier to classifier"
                                                                     );
    *out_relation_clas_feat = test_env_setup_data_create_relationship( &test_env,
                                                                       *out_from_classifier_parent,
                                                                       DATA_ROW_ID_VOID,
                                                                       *out_to_classifier,
                                                                       *out_to_feature,
                                                                       "from classifier to feature"
                                                                     );
    *out_relation_feat_clas = test_env_setup_data_create_relationship( &test_env,
                                                                       *out_from_classifier_parent,
                                                                       *out_from_feature,
                                                                       *out_to_classifier,
                                                                       DATA_ROW_ID_VOID,
                                                                       "from feature to classifier"
                                                                     );
    *out_relation_feat_feat = test_env_setup_data_create_relationship( &test_env,
                                                                       *out_from_classifier_parent,
                                                                       *out_from_feature,
                                                                       *out_to_classifier,
                                                                       *out_to_feature,
                                                                       "from feature to feature"
                                                                     );

    test_env_setup_destroy( &test_env );
}

enum io_export_model_traversal_test_future_enum {
    IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE = 470063,  /*!< downwards compatibility check */
};

static const data_classifier_type_t classifier_types[]
={
    DATA_CLASSIFIER_TYPE_BLOCK,
    DATA_CLASSIFIER_TYPE_SUBSYSTEM, /* equivilent to DATA_CLASSIFIER_TYPE_COMPONENT */
    DATA_CLASSIFIER_TYPE_ACTIVITY,
    DATA_CLASSIFIER_TYPE_STATE,
    DATA_CLASSIFIER_TYPE_NODE,
    DATA_CLASSIFIER_TYPE_CLASS, /* equivilent to DATA_CLASSIFIER_TYPE_OBJECT */
    DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION,
    DATA_CLASSIFIER_TYPE_BLOCK,
    DATA_CLASSIFIER_TYPE_PACKAGE,
    DATA_CLASSIFIER_TYPE_STEREOTYPE,
    DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK,
    DATA_CLASSIFIER_TYPE_REQUIREMENT,
    DATA_CLASSIFIER_TYPE_USE_CASE,
    DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE,
    DATA_CLASSIFIER_TYPE_INTERFACE,
    DATA_CLASSIFIER_TYPE_ARTIFACT,
    DATA_CLASSIFIER_TYPE_COMMENT,
    DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE, /* equivilent to DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE, DATA_CLASSIFIER_TYPE_DYN_FORK_NODE */
                                         /* DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE */
    DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
    DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY, /* equivilent to DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY */
    DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT, /* equivilent to DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT, DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL */
    DATA_CLASSIFIER_TYPE_INTERACTION,  /* is fake id only */
    DATA_CLASSIFIER_TYPE_PART,
    DATA_CLASSIFIER_TYPE_ACTOR,
    /* unexpected */
    (data_classifier_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE,  /* downwards compatibility check, equivilent to DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE */
};

static const data_feature_type_t feature_types[]
={
    DATA_FEATURE_TYPE_PROPERTY,
    DATA_FEATURE_TYPE_OPERATION,
    DATA_FEATURE_TYPE_PORT,
    DATA_FEATURE_TYPE_LIFELINE,
    DATA_FEATURE_TYPE_PROVIDED_INTERFACE,
    DATA_FEATURE_TYPE_REQUIRED_INTERFACE,
    DATA_FEATURE_TYPE_IN_PORT_PIN,
    DATA_FEATURE_TYPE_OUT_PORT_PIN,
    DATA_FEATURE_TYPE_ENTRY,
    DATA_FEATURE_TYPE_EXIT,
    DATA_FEATURE_TYPE_TAGGED_VALUE,
    /* unexpected */
    (data_feature_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE,  /* downwards compatibility check, equivilent to DATA_FEATURE_TYPE_VOID */
};

static const data_relationship_type_t relationship_types[]
={
    DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
    DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
    DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, /* equivilent to DATA_RELATIONSHIP_TYPE_UML_COMPOSITION */
    DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION,
    DATA_RELATIONSHIP_TYPE_UML_REALIZATION,
    DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, /* equivilent to DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL, DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL */
    DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH,
    DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW,
    DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW,
    DATA_RELATIONSHIP_TYPE_UML_DEPLOY, /* equivilent to DATA_RELATIONSHIP_TYPE_UML_MANIFEST */
    DATA_RELATIONSHIP_TYPE_UML_EXTEND, /* equivilent to DATA_RELATIONSHIP_TYPE_UML_INCLUDE */
    DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
    DATA_RELATIONSHIP_TYPE_UML_TRACE, /* equivilent to DATA_RELATIONSHIP_TYPE_UML_REFINE */
    /* unexpected */
    (data_relationship_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE,  /* downwards compatibility check */
};

static test_case_result_t iterate_types_on_mini_model( test_fixture_t *test_env )
{
    /* fill database with mini model */
    data_row_id_t from_classifier_parent;
    data_row_id_t from_classifier;
    data_row_id_t from_feature;
    data_row_id_t to_classifier_parent;
    data_row_id_t to_classifier;
    data_row_id_t to_feature;
    data_row_id_t relation_clas_clas;
    data_row_id_t relation_clas_feat;
    data_row_id_t relation_feat_clas;
    data_row_id_t relation_feat_feat;
    data_row_id_t root_diag_id;
    create_mini_model( &root_diag_id,
                       &from_classifier_parent,
                       &from_classifier,
                       &from_feature,
                       &to_classifier_parent,
                       &to_classifier,
                       &to_feature,
                       &relation_clas_clas,
                       &relation_clas_feat,
                       &relation_feat_clas,
                       &relation_feat_feat
                     );

    unsigned int clas_cnt = sizeof(classifier_types)/sizeof(classifier_types[0]);
    unsigned int feat_cnt = sizeof(feature_types)/sizeof(feature_types[0]);
    unsigned int rel_cnt = sizeof(relationship_types)/sizeof(relationship_types[0]);
    for ( unsigned int rel1_idx = 0; rel1_idx < rel_cnt; rel1_idx ++ )
    {
        for ( unsigned int clas1_idx = 0; clas1_idx < clas_cnt; clas1_idx ++ )
        {
            for ( unsigned int feat1_idx = 0; feat1_idx < feat_cnt; feat1_idx ++ )
            {
                for ( unsigned int variation_idx = 0; variation_idx < 2; variation_idx ++ )
                {
                    U8_LOG_ANOMALY_INT("variation_idx",variation_idx);
                    /* update types in database */
                    ctrl_classifier_controller_t *const c_ctrl
                        = ctrl_controller_get_classifier_control_ptr( &controller );

                    /* determine if all 4 relationships between from_classifier and to_classifier shall have the same type */
                    const unsigned int rel2_idx
                        = (variation_idx==0)
                        ? rel1_idx  /* same relationship type for all relationships */
                        : ( rel1_idx + clas1_idx + feat1_idx + variation_idx ) % rel_cnt;  /* high variation otherwise */
                    /* determine if all 4 classifiers shall have the same type */
                    const unsigned int clas2_idx
                        = (variation_idx==0)
                        ? clas1_idx  /* same classifier type for all classifiers */
                        : ( rel1_idx + clas1_idx + feat1_idx + variation_idx ) % clas_cnt;  /* high variation otherwise */
                    /* determine if both features at from_classifier and to_classifier shall have the same type */
                    const unsigned int feat2_idx
                        = (variation_idx==0)
                        ? feat1_idx  /* same feature type for all features */
                        : ( rel1_idx + clas1_idx + feat1_idx + variation_idx ) % feat_cnt;  /* high variation otherwise */

                    u8_error_t c_err = U8_ERROR_NONE;
                    c_err |= ctrl_classifier_controller_update_relationship_main_type ( c_ctrl, relation_clas_clas, relationship_types[rel1_idx] );
                    c_err |= ctrl_classifier_controller_update_relationship_main_type ( c_ctrl, relation_clas_feat, relationship_types[rel2_idx] );
                    c_err |= ctrl_classifier_controller_update_relationship_main_type ( c_ctrl, relation_feat_clas, relationship_types[rel1_idx] );
                    c_err |= ctrl_classifier_controller_update_relationship_main_type ( c_ctrl, relation_feat_feat, relationship_types[rel1_idx] );
                    c_err |= ctrl_classifier_controller_update_classifier_main_type ( c_ctrl, from_classifier_parent, classifier_types[clas1_idx] );
                    c_err |= ctrl_classifier_controller_update_classifier_main_type ( c_ctrl, from_classifier, classifier_types[clas1_idx] );
                    c_err |= ctrl_classifier_controller_update_classifier_main_type ( c_ctrl, to_classifier_parent, classifier_types[clas1_idx] );
                    c_err |= ctrl_classifier_controller_update_classifier_main_type ( c_ctrl, to_classifier, classifier_types[clas2_idx] );
                    c_err |= ctrl_classifier_controller_update_feature_main_type ( c_ctrl, from_feature, feature_types[feat1_idx] );
                    c_err |= ctrl_classifier_controller_update_feature_main_type ( c_ctrl, to_feature, feature_types[feat2_idx] );
                    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == c_err );

                    data_stat_t stat;
                    data_stat_init( &stat );
                    {
                        /* static variables are ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
                        static data_visible_set_t temp_input_data;  /*!< buffer to cache the diagram data */
                        static io_export_model_traversal_t temp_model_traversal;  /*!< own instance of a model_traversal for text export */
                        xmi_element_writer_t temp_xmi_writer;  /*!< memory for a temporary xmi writer */

                        universal_memory_output_stream_reset( &mem_output_stream );
                        universal_output_stream_t* output = universal_memory_output_stream_get_output_stream( &mem_output_stream );

                        {
                            xmi_element_writer_init( &temp_xmi_writer, &stat, output );
                            /* init the model_traversal */
                            io_export_model_traversal_init( &temp_model_traversal,
                                                            &db_reader,
                                                            &temp_input_data,
                                                            &stat,
                                                            xmi_element_writer_get_element_writer( &temp_xmi_writer )
                                                        );
                            /* write the document */
                            int export_err = 0;
                            export_err |= xmi_element_writer_write_header( &temp_xmi_writer, "document file name" );
                            export_err |= xmi_element_writer_start_main( &temp_xmi_writer, "document file name" );
                            xmi_element_writer_set_mode( &temp_xmi_writer, XMI_WRITER_PASS_BASE );
                            export_err |= io_export_model_traversal_walk_model_nodes( &temp_model_traversal );
                            export_err |= xmi_element_writer_end_main( &temp_xmi_writer );
                            xmi_element_writer_set_mode( &temp_xmi_writer, XMI_WRITER_PASS_PROFILE );
                            export_err |= io_export_model_traversal_walk_model_nodes( &temp_model_traversal );
                            export_err |= xmi_element_writer_write_footer( &temp_xmi_writer );

                            TEST_EXPECT_EQUAL_INT( 0, export_err );

                            io_export_model_traversal_destroy( &temp_model_traversal );
                            xmi_element_writer_destroy( &temp_xmi_writer );
                        }
                    }
                    //static const char TERM2[4]="qQe\n";
                    //universal_memory_output_stream_write( &mem_output_stream, &TERM2, sizeof(TERM2) );
                    static const char TERM='\00';
                    int write_err = universal_memory_output_stream_write( &mem_output_stream, &TERM, sizeof(TERM) );
                    TEST_ENVIRONMENT_ASSERT( 0 == write_err );
                    universal_memory_output_stream_flush( &mem_output_stream );
#ifndef NDEBUG
                    fprintf( stdout, "\n%s\n", &(mem_buffer[0]) );
#endif

                    const int xml_is_error = test_result_check_xml_validate_xml( &(mem_buffer[0]) );
                    TEST_EXPECT_EQUAL_INT( 0, xml_is_error );

                    data_stat_trace( &stat );
                    const int expected_rel_errors
                        = (( relationship_types[rel1_idx] == (data_relationship_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE ) ? 3 : 0 )
                        + (( relationship_types[rel2_idx] == (data_relationship_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE ) ? 1 : 0 );
                    const int expected_clas_errors
                        = (( classifier_types[clas1_idx] == (data_classifier_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE ) ? 3 : 0 )
                        + (( classifier_types[clas2_idx] == (data_classifier_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE ) ? 1 : 0 );
                    const int expected_feat_errors
                        = (( feature_types[feat1_idx] == (data_feature_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE ) ? 1 : 0 )
                        + (( feature_types[feat2_idx] == (data_feature_type_t) IO_EXPORT_MODEL_TRAVERSAL_TEST_FUTURE_TYPE ) ? 1 : 0 );
                    TEST_EXPECT_EQUAL_INT( expected_clas_errors, data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR ) );
                    TEST_EXPECT_EQUAL_INT( expected_feat_errors, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_ERROR ) );
                    TEST_EXPECT_EQUAL_INT( expected_rel_errors, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR ) );
                    TEST_EXPECT_EQUAL_INT( expected_clas_errors + expected_feat_errors + expected_rel_errors,
                                           data_stat_get_series_count( &stat, DATA_STAT_SERIES_ERROR )
                                         );
                    TEST_EXPECT_EQUAL_INT( 5 - expected_clas_errors,
                                           data_stat_get_count( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_EXPORTED )
                                         );
                    //TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_EXPORTED ) );
                    //TEST_EXPECT_EQUAL_INT( 4, data_stat_get_count( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED ) );
                    //TEST_EXPECT_EQUAL_INT( 10, data_stat_get_series_count( &stat, DATA_STAT_SERIES_EXPORTED ) );
                    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_series_count( &stat, DATA_STAT_SERIES_MODIFIED ) );
                    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_series_count( &stat, DATA_STAT_SERIES_DELETED ) );
                    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_series_count( &stat, DATA_STAT_SERIES_IGNORED ) );
                    //TEST_EXPECT_EQUAL_INT( 0, data_stat_get_series_count( &stat, DATA_STAT_SERIES_WARNING ) );
                    data_stat_destroy( &stat );
                }
            }
        }
    }
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2020-2023 Andreas Warnk e
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
