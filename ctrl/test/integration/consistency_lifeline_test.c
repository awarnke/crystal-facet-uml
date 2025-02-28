/* File: consistency_lifeline_test.c; Copyright and License: see below */

#include "consistency_lifeline_test.h"
#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "u8/u8_trace.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t lifeline_to_diagramelement_consistency( test_fixture_t *fix );
static test_case_result_t diagram_to_lifeline_consistency( test_fixture_t *fix );
static test_case_result_t diagramelement_to_lifeline_consistency( test_fixture_t *fix );

test_suite_t consistency_lifeline_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "consistency_lifeline_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "lifeline_to_diagramelement_consistency", &lifeline_to_diagramelement_consistency );
    test_suite_add_test_case( &result, "diagram_to_lifeline_consistency", &diagram_to_lifeline_consistency );
    test_suite_add_test_case( &result, "diagramelement_to_lifeline_consistency", &diagramelement_to_lifeline_consistency );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
    data_database_reader_t db_reader;  /*!< database reader to access the database */
    data_database_writer_t db_writer;  /*!< database writer to access the database */
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
    data_database_writer_init( &((*fix).db_writer), &((*fix).db_reader), &((*fix).database) );
    ctrl_controller_init( &((*fix).controller), &((*fix).database) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    ctrl_controller_destroy( &((*fix).controller) );
    data_database_writer_destroy( &((*fix).db_writer) );
    data_database_reader_destroy( &((*fix).db_reader) );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static test_case_result_t lifeline_to_diagramelement_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    data_row_t root_diag_id;
    {
        data_diagram_t root_diagram;
        data_err = data_diagram_init_new( &root_diagram,
                                          DATA_ROW_VOID, /*=parent_diagram_id*/
                                          DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                          "stereo_t", /* stereotype */
                                          "the_root_diag",
                                          "diagram_description-root",
                                          10555, /*=list_order*/
                                          DATA_DIAGRAM_FLAG_NONE
                                        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        root_diag_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram( diagram_ctrl,
                                                           &root_diagram,
                                                           CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                           &root_diag_id
                                                         );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_VOID != root_diag_id );
        data_diagram_destroy ( &root_diagram );
    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_CLASS */
    data_row_t classifier_id;
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new ( &new_classifier,
                                              DATA_CLASSIFIER_TYPE_CLASS,
                                              "",  /* stereotype */
                                              "my_class",
                                              "",  /* description */
                                              45,
                                              4500,
                                              450000
                                            );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        classifier_id = DATA_ROW_VOID;
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                  &classifier_id
                                                                );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_VOID != classifier_id );
        data_classifier_destroy ( &new_classifier );
    }

    /* create a diagramelement for the classifier */
    data_row_t first_diag_element_id;
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );

        first_diag_element_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &first_diag_element_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_EXPECT( DATA_ROW_VOID != first_diag_element_id );
    }

    /* check that the classifier now has a feature of type DATA_FEATURE_TYPE_LIFELINE */
    data_row_t lifeline_id;
    {
        data_feature_t the_feature;
        data_feature_iterator_t feature_iterator;
        data_err = data_feature_iterator_init_empty( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_database_reader_get_features_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       &feature_iterator
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_feature_iterator_next( &feature_iterator, &the_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &the_feature ) );
        lifeline_id = data_feature_get_row_id( &the_feature );
        TEST_EXPECT( DATA_ROW_VOID != lifeline_id );
        const bool others = data_feature_iterator_has_next( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( false, others );
        data_err |= data_feature_iterator_destroy( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* delete the feature (lifeline) */
    {
        ctrl_err = ctrl_classifier_controller_delete_feature ( classifier_ctrl,
                                                               lifeline_id,
                                                               CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                             );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check that the diagramelement does not reference the deleted lifeline anymore */
    {
        data_diagramelement_t check_diagele2;
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), first_diag_element_id, &check_diagele2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_diagramelement_get_focused_feature_row_id( &check_diagele2 ) );

        data_diagramelement_destroy ( &check_diagele2 );
    }

    /* perform an undo */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_controller_undo ( &((*fix).controller), &stat );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 2, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check that the feature exists again */
    {
        data_feature_t check_feature;
        data_err = data_database_reader_get_feature_by_id ( &((*fix).db_reader), lifeline_id, &check_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        data_feature_destroy( &check_feature );
    }

    /* check that the diagramelement references the feature again */
    {
        data_diagramelement_t check_diagele3;
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), first_diag_element_id, &check_diagele3 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        TEST_EXPECT_EQUAL_INT( lifeline_id, data_diagramelement_get_focused_feature_row_id( &check_diagele3 ) );

        data_diagramelement_destroy ( &check_diagele3 );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t diagram_to_lifeline_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    data_row_t root_diag_id;
    {
        data_diagram_t root_diagram;
        data_err = data_diagram_init( &root_diagram,
                                      DATA_ROW_VOID, /*=diagram_id is ignored*/
                                      DATA_ROW_VOID, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                      "diag:stereo_t", /* stereotype */
                                      "the_root_diag",
                                      "diagram_description-root",
                                      10555, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "4db26042-d7ce-4e2f-b8dc-cc9c9c61f39c"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        root_diag_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &root_diagram,
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                            &root_diag_id
                                                          );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_VOID != root_diag_id );
        data_diagram_destroy ( &root_diagram );
    }

    /* create a second diagram of type DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    data_row_t child_diag_id;
    {
        data_diagram_t child_diagram;
        data_err = data_diagram_init( &child_diagram,
                                      DATA_ROW_VOID, /*=diagram_id is ignored*/
                                      root_diag_id, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                      "diag:stereo_t", /* stereotype */
                                      "the_child_diag",
                                      "diagram_description-child",
                                      20666, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "e89207d2-c7d2-4996-9bfc-bc6e1a9ff6b3"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        child_diag_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &child_diagram,
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                            &child_diag_id
                                                          );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( child_diag_id != DATA_ROW_VOID );
        data_diagram_destroy ( &child_diagram );
    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_CLASS */
    data_row_t classifier_id;
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new( &new_classifier,
                                             DATA_CLASSIFIER_TYPE_CLASS,
                                             "",  /* stereotype */
                                             "my_class",
                                             "",  /* description */
                                             45,
                                             4500,
                                             450000
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        classifier_id = DATA_ROW_VOID;
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                  &classifier_id
                                                                );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_VOID != classifier_id );
        data_classifier_destroy ( &new_classifier );
    }

    /* create one diagramelement for the root diagram */
    data_row_t root_diag_element_id;
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );

        root_diag_element_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &root_diag_element_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_EXPECT( DATA_ROW_VOID != root_diag_element_id );
    }

    /* create one diagramelement for the child diagram */
    data_row_t child_diag_element_id;
    {
        data_diagramelement_t new_diagele2;
        data_diagramelement_init_new ( &new_diagele2,
                                       child_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );

        child_diag_element_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele2,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &child_diag_element_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele2 );
        TEST_EXPECT( DATA_ROW_VOID != child_diag_element_id );
    }

    /* change the type of the child diagram to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_diagram_controller_update_diagram_type( diagram_ctrl,
                                                                child_diag_id,
                                                                DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                                                &stat
                                                              );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 3, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* change the type of the child diagram to DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_diagram_controller_update_diagram_type( diagram_ctrl,
                                                                child_diag_id,
                                                                DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
                                                                &stat
                                                              );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_DELETED ));
        TEST_EXPECT_EQUAL_INT( 3, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* change the type of the child diagram to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    {
        data_stat_t stat;
        data_stat_init(&stat);
        ctrl_err = ctrl_diagram_controller_update_diagram_type( diagram_ctrl,
                                                                child_diag_id,
                                                                DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                                                &stat
                                                              );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_MODIFIED ));
        TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count ( &stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_CREATED ));
        TEST_EXPECT_EQUAL_INT( 3, data_stat_get_total_count ( &stat ));
        data_stat_destroy(&stat);
    }

    /* check that the classifier now has one feature of type DATA_FEATURE_TYPE_LIFELINE */
    data_feature_t the_feature;
    {
        data_feature_iterator_t feature_iterator;
        data_err = data_feature_iterator_init_empty( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_database_reader_get_features_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       &feature_iterator
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_feature_iterator_next( &feature_iterator, &the_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &the_feature ) );
        const bool others = data_feature_iterator_has_next( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( false, others );
        data_err |= data_feature_iterator_destroy( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check that this is referenced */
    {
        data_diagramelement_t check_diagele2;
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), child_diag_element_id, &check_diagele2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        TEST_EXPECT_EQUAL_INT( data_feature_get_row_id( &the_feature ), data_diagramelement_get_focused_feature_row_id( &check_diagele2 ) );

        data_diagramelement_destroy ( &check_diagele2 );
    }

    /* delete the child diagram (but not the classifier) */
    {
        ctrl_err = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl,
                                                                   child_diag_element_id,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );

        ctrl_err = ctrl_diagram_controller_delete_diagram ( diagram_ctrl,
                                                            child_diag_id,
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                          );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check that the feature of type DATA_FEATURE_TYPE_LIFELINE is deleted */
    {
        data_feature_iterator_t feature_iterator;
        data_err = data_feature_iterator_init_empty( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_database_reader_get_features_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       &feature_iterator
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        const bool any = data_feature_iterator_has_next( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( false, any );
        data_err |= data_feature_iterator_destroy( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t diagramelement_to_lifeline_consistency( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t ctrl_err;
    u8_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    ctrl_diagram_controller_t *diagram_ctrl;

    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( &((*fix).controller) );
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( &((*fix).controller) );

    /* create a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM */
    data_row_t root_diag_id;
    {
        data_diagram_t root_diagram;
        data_err = data_diagram_init( &root_diagram,
                                      DATA_ROW_VOID, /*=diagram_id is ignored*/
                                      DATA_ROW_VOID, /*=parent_diagram_id*/
                                      DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
                                      "diagIIstereo_t", /* stereotype */
                                      "the_root_diag",
                                      "diagram_description-root",
                                      10555, /*=list_order*/
                                      DATA_DIAGRAM_FLAG_NONE,
                                      "bad218e0-d633-4a2c-b0bb-d636c28d3496"
                                    );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        root_diag_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &root_diagram,
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                            &root_diag_id
        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_VOID != root_diag_id );
        data_diagram_destroy ( &root_diagram );
    }

    /* create a classifier of type DATA_CLASSIFIER_TYPE_CLASS */
    data_row_t classifier_id;
    {
        data_classifier_t new_classifier;
        data_err = data_classifier_init_new( &new_classifier,
                                             DATA_CLASSIFIER_TYPE_CLASS,
                                             "",  /* stereotype */
                                             "my_class",
                                             "",  /* description */
                                             45,
                                             4500,
                                             450000
                                           );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        classifier_id = DATA_ROW_VOID;
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                  &classifier_id
        );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        TEST_EXPECT( DATA_ROW_VOID != classifier_id );
        data_classifier_destroy ( &new_classifier );
    }

    /* create first diagramelement for the classifier */
    data_row_t first_diag_element_id;
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );

        first_diag_element_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &first_diag_element_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
        TEST_EXPECT( DATA_ROW_VOID != first_diag_element_id );
    }

    /* create second diagramelement for the classifier */
    data_row_t second_diag_element_id;
    {
        data_diagramelement_t new_diagele2;
        data_diagramelement_init_new ( &new_diagele2,
                                       root_diag_id,
                                       classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );

        second_diag_element_id = DATA_ROW_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele2,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   &second_diag_element_id
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
        data_diagramelement_destroy ( &new_diagele2 );
        TEST_EXPECT( DATA_ROW_VOID != second_diag_element_id );
    }

    /* check that the classifier now has two features of type DATA_FEATURE_TYPE_LIFELINE */
    data_feature_t the_feature;
    {
        data_feature_iterator_t feature_iterator;
        data_err = data_feature_iterator_init_empty( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_database_reader_get_features_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       &feature_iterator
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_feature_iterator_next( &feature_iterator, &the_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &the_feature ) );
        data_err = data_feature_iterator_next( &feature_iterator, &the_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &the_feature ) );
        data_err = data_feature_iterator_next( &feature_iterator, &the_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, data_err );
        data_err = data_feature_iterator_destroy( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* delete the first diagramelement (but not the classifier) */
    {
        ctrl_err = ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl,
                                                                   first_diag_element_id,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, ctrl_err );
    }

    /* check that one feature of type DATA_FEATURE_TYPE_LIFELINE is deleted */
    {
        data_feature_iterator_t feature_iterator;
        data_err = data_feature_iterator_init_empty( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_database_reader_get_features_by_classifier_id( &((*fix).db_reader),
                                                                       classifier_id,
                                                                       &feature_iterator
                                                                     );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        data_err = data_feature_iterator_next( &feature_iterator, &the_feature );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
        TEST_EXPECT_EQUAL_INT( DATA_FEATURE_TYPE_LIFELINE, data_feature_get_main_type( &the_feature ) );
        const bool another = data_feature_iterator_has_next( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( false, another );
        data_err |= data_feature_iterator_destroy( &feature_iterator );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    }

    /* check that this is referenced */
    {
        data_diagramelement_t check_diagele2;
        data_err = data_database_reader_get_diagramelement_by_id ( &((*fix).db_reader), second_diag_element_id, &check_diagele2 );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

        TEST_EXPECT_EQUAL_INT( data_feature_get_row_id( &the_feature ), data_diagramelement_get_focused_feature_row_id( &check_diagele2 ) );

        data_diagramelement_destroy ( &check_diagele2 );
    }
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2018-2025 Andreas Warnke
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
