/* File: ctrl_undo_redo_list_test.c; Copyright and License: see below */

#include "ctrl_undo_redo_list_test.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_multi_step_changer.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "u8/u8_fault_inject.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t undo_redo_classifier( test_fixture_t *fix );
static test_case_result_t undo_redo_list_limits( test_fixture_t *fix );
static test_case_result_t undo_redo_feature_and_relationship( test_fixture_t *fix );
static test_case_result_t undo_redo_update_diagram( test_fixture_t *fix );

test_suite_t ctrl_undo_redo_list_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "ctrl_undo_redo_list_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "undo_redo_classifier", &undo_redo_classifier );
    test_suite_add_test_case( &result, "undo_redo_list_limits", &undo_redo_list_limits );
    test_suite_add_test_case( &result, "undo_redo_feature_and_relationship", &undo_redo_feature_and_relationship );
    test_suite_add_test_case( &result, "undo_redo_update_diagram", &undo_redo_update_diagram );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    ctrl_controller_t controller;  /*!< controller instance on which the tests are performed */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    data_database_init( &((*fix).database) );
    data_database_open_in_memory( &((*fix).database) );
    data_database_reader_init( &((*fix).db_reader), &((*fix).database) );
    ctrl_controller_init( &((*fix).controller), &((*fix).database) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_controller_destroy( &((*fix).controller) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static test_case_result_t undo_redo_classifier( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    data_row_id_t root_diagram_id;
    data_row_id_t classifier_id;
    data_row_id_t diagele_id;
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );

    /* create the root diagram */
    root_diagram_id = DATA_ROW_ID_VOID;
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists( diag_ctrl,
                                                                          DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                          "my_root_diag",
                                                                          &root_diagram_id
                                                                        );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT( DATA_ROW_ID_VOID != root_diagram_id );

    /* create a classifier and a diagramelement */
    classifier_id = DATA_ROW_ID_VOID;
    data_classifier_t new_classifier;
    data_err = data_classifier_init( &new_classifier,
                                     DATA_ROW_ID_VOID,  /* new classifier */
                                     DATA_CLASSIFIER_TYPE_NODE,
                                     "",  /* stereotype */
                                     "my_node",
                                     "",  /* description */
                                     17,
                                     1700,
                                     170000,
                                     "d8df8d54-1916-4150-899e-48bde90c3bbe"
                                   );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    ctrl_err = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                             &new_classifier,
                                                             CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                             &classifier_id
                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    data_classifier_destroy ( &new_classifier );
    TEST_EXPECT( DATA_ROW_ID_VOID != classifier_id );

    diagele_id = DATA_ROW_ID_VOID;
    data_diagramelement_t new_diagele;
    data_err = data_diagramelement_init( &new_diagele,
                                         DATA_ROW_ID_VOID,  /* new diagramelement */
                                         root_diagram_id,
                                         classifier_id,
                                         DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                         DATA_ROW_ID_VOID,
                                         "98e479f0-9112-483e-b64f-251d55a50c13"
                                       );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    ctrl_err = ctrl_diagram_controller_create_diagramelement( diag_ctrl,
                                                              &new_diagele,
                                                              CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                              &diagele_id
                                                            );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    data_diagramelement_destroy ( &new_diagele );
    TEST_EXPECT( DATA_ROW_ID_VOID != diagele_id );

    /* update the classifier */
    ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( classifier_ctrl, classifier_id, "my_stereo" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* undo classifier update */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo classifier and diagramelement creation */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        uint32_t read_vis_classifiers_count;
        data_visible_classifier_t read_vis_classifiers[1];

        data_err = data_database_reader_get_classifiers_by_diagram_id ( &((*fix).db_reader), root_diagram_id, 1, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, read_vis_classifiers_count );
    }

    /* undo root diagram creation */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_diagram_t read_diagram;
        data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), root_diagram_id, &read_diagram );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    /* redo root diagram creation */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_diagram_t read_diagram;
        data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), root_diagram_id, &read_diagram );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( root_diagram_id, data_diagram_get_row_id( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagram_get_parent_row_id( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, data_diagram_get_diagram_type( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_root_diag", data_diagram_get_name_const( &read_diagram ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_diagram_get_description_const( &read_diagram ) ) );
        TEST_EXPECT_EQUAL_INT( 0, data_diagram_get_list_order( &read_diagram ) );
    }

    /* redo classifier and diagramelement creation */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo classifier update */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        uint32_t read_vis_classifiers_count;
        data_visible_classifier_t read_vis_classifiers[1];

        data_err = data_database_reader_get_classifiers_by_diagram_id ( &((*fix).db_reader), root_diagram_id, 1, &read_vis_classifiers, &read_vis_classifiers_count );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 1, read_vis_classifiers_count );

        data_classifier_t *first_classifier;
        first_classifier = data_visible_classifier_get_classifier_ptr( &(read_vis_classifiers[0]) );

        TEST_EXPECT_EQUAL_INT( classifier_id, data_classifier_get_row_id( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_NODE, data_classifier_get_main_type( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_stereo", data_classifier_get_stereotype_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "my_node", data_classifier_get_name_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "", data_classifier_get_description_const( first_classifier ) ) );
        TEST_EXPECT_EQUAL_INT( 17, data_classifier_get_x_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 1700, data_classifier_get_y_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 170000, data_classifier_get_list_order( first_classifier ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "d8df8d54-1916-4150-899e-48bde90c3bbe", data_classifier_get_uuid_const( first_classifier ) ) );

        data_diagramelement_t *first_diagele;
        first_diagele = data_visible_classifier_get_diagramelement_ptr( &(read_vis_classifiers[0]) );

        TEST_EXPECT_EQUAL_INT( root_diagram_id, data_diagramelement_get_diagram_row_id( first_diagele ) );
        TEST_EXPECT_EQUAL_INT( classifier_id, data_diagramelement_get_classifier_row_id( first_diagele ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_EMPHASIS, data_diagramelement_get_display_flags( first_diagele ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_focused_feature_row_id( first_diagele ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "98e479f0-9112-483e-b64f-251d55a50c13", data_diagramelement_get_uuid_const( first_diagele ) ) );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t undo_redo_list_limits( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    data_row_id_t root_diagram_id;
    data_row_id_t child_diag_id;
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );

    /* create the root diagram */
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diag_ctrl,
                                                                           DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
                                                                           "my_root_diag",
                                                                           &root_diagram_id
                                                                         );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* undo root diagram creation */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo at start of list */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo root diagram creation */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo at end of list */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* fill the list by creating classifiers and diagramelements */
    for ( int32_t pos = 0; pos < (CTRL_UNDO_REDO_LIST_MAX_SIZE-1/*first boundary*/-2/*diagram and boundary*/)/2/*list entries per diagram*/; pos ++ )
    {
        /* create a diagram */
        ctrl_err = ctrl_diagram_controller_private_create_child_diagram ( diag_ctrl,
                                                                          root_diagram_id,
                                                                          DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                                          "diagram_name",
                                                                          &child_diag_id
                                                                        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* create one more diagram */
    ctrl_err = ctrl_diagram_controller_private_create_child_diagram ( diag_ctrl,
                                                                      root_diagram_id,
                                                                      DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                                      "diagram_name",
                                                                      &child_diag_id
                                                                    );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* undo everything that is possible */
    for ( int32_t pos = 0; pos < (CTRL_UNDO_REDO_LIST_MAX_SIZE-0/*first boundary is overwritten*/-2/*diagram and boundary*/)/2/*list entries per diagram*/; pos ++ )
    {
        {
            data_stat_t stat;
            data_stat_init(&stat);
            ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
            TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
            TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
            data_stat_destroy(&stat);
        }
    }

    /* undo one more */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo one */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* create a new diagram somewhere in the middle of the list */
    ctrl_err = ctrl_diagram_controller_private_create_child_diagram ( diag_ctrl,
                                                                      root_diagram_id,
                                                                      DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
                                                                      "diagram_name",
                                                                      &child_diag_id
                                                                    );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* redo one but already at end of the list */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo 2 existing and one not existing */
    /* undo one more */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo one more */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo one more */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t undo_redo_feature_and_relationship( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );
    data_feature_t check_f;
    data_relationship_t check_r;

    /* define a feature */
    data_feature_t step1;
    data_err = data_feature_init( &step1,
                                  17, /* feature_id */
                                  DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                  35000, /* classifier_id */
                                  "startup_time", /* feature_key */
                                  "uint64_t", /* feature_value */
                                  "time in nano seconds to start", /* feature_description */
                                  5000000, /* list order */
                                  "8490c942-e425-4764-8212-37d2bfcc7e1e"
                                );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* 1. create the feature in the db */
    data_row_id_t new_feature_id;
    ctrl_err = ctrl_classifier_controller_create_feature ( classifier_ctrl,
                                                           &step1,
                                                           CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                           &new_feature_id
                                                         );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* 2. update the feature in the db */
    ctrl_err = ctrl_classifier_controller_update_feature_main_type ( classifier_ctrl,
                                                                     new_feature_id,
                                                                     DATA_FEATURE_TYPE_OPERATION
                                                                   );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* 3a. update the feature in the db */
    ctrl_err = ctrl_classifier_controller_update_feature_value ( classifier_ctrl,
                                                                 new_feature_id,
                                                                 "(void)->(uint64_t)"
                                                               );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* define a relationship */
    data_relationship_t step3b;
    data_err = data_relationship_init( &step3b,
                                       34, /* relationship_id */
                                       DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                       86000, /* from_classifier_id */
                                       86001, /* to_classifier_id */
                                       "rel:stereo_t", /* stereotype */
                                       "the composition is more", /* name */
                                       "than the sum of its parts", /* description */
                                       -66000, /* list_order */
                                       DATA_ROW_ID_VOID, /* from_feature_id */
                                       150160, /* to_feature_id */
                                       "57d93512-91d6-41eb-860f-0408b79a9aaf"
                                     );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* 3b. create the relationship in the db */
    data_row_id_t new_relationship_id;
    ctrl_err = ctrl_classifier_controller_create_relationship ( classifier_ctrl,
                                                                &step3b,
                                                                CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                &new_relationship_id
                                                              );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* 4. update the relationship in the db */
    ctrl_err = ctrl_classifier_controller_update_relationship_description ( classifier_ctrl,
                                                                            new_relationship_id,
                                                                            "good for modularization"
                                                                          );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* 5. delete the feature and the relationship from the database */
    {
        data_small_set_t small_set;
        data_id_t element_id;
        data_small_set_init( &small_set );
        data_id_init( &element_id, DATA_TABLE_FEATURE, new_feature_id );
        data_err = data_small_set_add_obj ( &small_set, element_id );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_id_reinit( &element_id, DATA_TABLE_RELATIONSHIP, new_relationship_id );
        data_err = data_small_set_add_obj ( &small_set, element_id );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        {
            ctrl_multi_step_changer_t multi_stepper;
            ctrl_multi_step_changer_init( &multi_stepper, &((*fix).controller), &((*fix).db_reader) );
            data_stat_t stat;
            data_stat_init(&stat);

            ctrl_err = ctrl_multi_step_changer_delete_set ( &multi_stepper, &small_set, &stat );

            TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
            TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
            TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
            TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
            data_stat_destroy(&stat);
            ctrl_multi_step_changer_destroy( &multi_stepper );
        }
    }

    /* the undo-list is filled now. test undo */

    /* undo step 5 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_feature_id, data_feature_get_row_id( &check_f ) );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_OPERATION, data_feature_get_main_type( &check_f ) );
        TEST_EXPECT_EQUAL_INT( 35000, data_feature_get_classifier_row_id( &check_f ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "startup_time", data_feature_get_key_const( &check_f ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "(void)->(uint64_t)", data_feature_get_value_const( &check_f ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "time in nano seconds to start", data_feature_get_description_const( &check_f ) ) );
        TEST_EXPECT_EQUAL_INT( 5000000, data_feature_get_list_order( &check_f ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "8490c942-e425-4764-8212-37d2bfcc7e1e", data_feature_get_uuid_const( &check_f ) ) );

        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_relationship_id, data_relationship_get_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, data_relationship_get_main_type( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 86000, data_relationship_get_from_classifier_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 86001, data_relationship_get_to_classifier_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "rel:stereo_t", data_relationship_get_stereotype_const( &check_r ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "the composition is more", data_relationship_get_name_const( &check_r ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "good for modularization", data_relationship_get_description_const( &check_r ) ) );
        TEST_EXPECT_EQUAL_INT( -66000, data_relationship_get_list_order( &check_r ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 150160, data_relationship_get_to_feature_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "57d93512-91d6-41eb-860f-0408b79a9aaf", data_relationship_get_uuid_const( &check_r ) ) );
    }

    /* undo step 4 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "than the sum of its parts", data_relationship_get_description_const( &check_r ) ) );
    }

    /* undo step 3 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "uint64_t", data_feature_get_value_const( &check_f ) ) );

        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    /* undo step 2 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_PROPERTY, data_feature_get_main_type( &check_f ) );
    }

    /* undo step 1 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );

        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    /* undo step 0 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo step 1 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_PROPERTY, data_feature_get_main_type( &check_f ) );
    }

    /* redo step 2 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "uint64_t", data_feature_get_value_const( &check_f ) ) );

        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    /* redo step 3 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "than the sum of its parts", data_relationship_get_description_const( &check_r ) ) );
    }

    /* redo step 4 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_feature_id, data_feature_get_row_id( &check_f ) );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_OPERATION, data_feature_get_main_type( &check_f ) );
        TEST_EXPECT_EQUAL_INT( 35000, data_feature_get_classifier_row_id( &check_f ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "startup_time", data_feature_get_key_const( &check_f ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "(void)->(uint64_t)", data_feature_get_value_const( &check_f ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "time in nano seconds to start", data_feature_get_description_const( &check_f ) ) );
        TEST_EXPECT_EQUAL_INT( 5000000, data_feature_get_list_order( &check_f ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "8490c942-e425-4764-8212-37d2bfcc7e1e", data_feature_get_uuid_const( &check_f ) ) );

        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( new_relationship_id, data_relationship_get_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, data_relationship_get_main_type( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 86000, data_relationship_get_from_classifier_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 86001, data_relationship_get_to_classifier_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "rel:stereo_t", data_relationship_get_stereotype_const( &check_r ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "the composition is more", data_relationship_get_name_const( &check_r ) ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "good for modularization", data_relationship_get_description_const( &check_r ) ) );
        TEST_EXPECT_EQUAL_INT( -66000, data_relationship_get_list_order( &check_r ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_relationship_get_from_feature_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 150160, data_relationship_get_to_feature_row_id( &check_r ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "57d93512-91d6-41eb-860f-0408b79a9aaf", data_relationship_get_uuid_const( &check_r ) ) );
    }

    /* redo step 5 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check what is in the database */
    {
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), new_feature_id, &check_f );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );

        data_err = data_database_reader_get_relationship_by_id ( &((*fix).db_reader), new_relationship_id, &check_r );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_DB_STRUCTURE, data_err );
    }

    /* redo step 6 */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t undo_redo_update_diagram( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_diagram_controller_t *diag_ctrl;
    diag_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );

    /* step 1: create a diagram */
    data_row_id_t root_diagram_id;
    root_diagram_id = DATA_ROW_ID_VOID;
    ctrl_err = ctrl_diagram_controller_create_root_diagram_if_not_exists ( diag_ctrl,
                                                                           DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM,
                                                                           "the_requirements",
                                                                           &root_diagram_id
                                                                         );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT( DATA_ROW_ID_VOID != root_diagram_id );

    /* step 2: create a diagramelement */
    data_row_id_t diagele_id;
    diagele_id = DATA_ROW_ID_VOID;
    data_diagramelement_t new_diagele;
    data_diagramelement_init_new ( &new_diagele,
                                   root_diagram_id,
                                   1034,
                                   DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,
                                   2044
                                 );
    ctrl_err = ctrl_diagram_controller_create_diagramelement ( diag_ctrl,
                                                               &new_diagele,
                                                               CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                               &diagele_id
                                                             );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    data_diagramelement_destroy ( &new_diagele );
    TEST_EXPECT( DATA_ROW_ID_VOID != diagele_id );

    /* step 3a: update the diagram name */
    ctrl_err = ctrl_diagram_controller_update_diagram_name ( diag_ctrl,
                                                             root_diagram_id,
                                                             "MY_NEW_NAME"
                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* step 3b: update the diagram type */
    data_stat_t stat;
    data_stat_init(&stat);
    ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl,
                                                             root_diagram_id,
                                                             DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                                             &stat
                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
    data_stat_destroy(&stat);

    /* step 4a: update the diagramelement display_flags */
    ctrl_err = ctrl_diagram_controller_update_diagramelement_display_flags ( diag_ctrl,
                                                                             diagele_id,
                                                                             DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE,
                                                                             CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                           );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* step 4b: update the diagramelement focused_feature_id */
    ctrl_err = ctrl_diagram_controller_update_diagramelement_focused_feature_id ( diag_ctrl,
                                                                                  diagele_id,
                                                                                  DATA_ROW_ID_VOID,
                                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                                );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

    /* undo step 4b and 4a */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo step 3b */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* undo step 3a */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check the diagram */
    {
        data_diagram_t read_diagram;
        data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), root_diagram_id, &read_diagram );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( root_diagram_id, data_diagram_get_row_id( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagram_get_parent_row_id( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, data_diagram_get_diagram_type( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "the_requirements", data_diagram_get_name_const( &read_diagram ) ) );
    }

    /* check the diagramelement */
    {
        data_diagramelement_t read_diagele;
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), diagele_id, &read_diagele );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( diagele_id, data_diagramelement_get_row_id( &read_diagele ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_EMPHASIS, data_diagramelement_get_display_flags( &read_diagele ) );
        TEST_EXPECT_EQUAL_INT( 2044, data_diagramelement_get_focused_feature_row_id( &read_diagele ) );
    }

    /* redo step 3a */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo step 3b */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* redo step 4a and 4b */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_redo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check the diagram */
    {
        data_diagram_t read_diagram;
        data_err = data_database_reader_get_diagram_by_id ( &((*fix).db_reader), root_diagram_id, &read_diagram );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( root_diagram_id, data_diagram_get_row_id( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagram_get_parent_row_id( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, data_diagram_get_diagram_type( &read_diagram ) );
        TEST_EXPECT_EQUAL_INT( 0, strcmp( "MY_NEW_NAME", data_diagram_get_name_const( &read_diagram ) ) );
    }

    /* check the diagramelement */
    {
        data_diagramelement_t read_diagele;
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), diagele_id, &read_diagele );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( diagele_id, data_diagramelement_get_row_id( &read_diagele ) );
        TEST_EXPECT_EQUAL_INT( DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE, data_diagramelement_get_display_flags( &read_diagele ) );
        TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_diagramelement_get_focused_feature_row_id( &read_diagele ) );
    }
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2016-2024 Andreas Warnke
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
