/* File: gui_sketch_object_creator.c; Copyright and License: see below */

#include "sketch/gui_sketch_object_creator.h"
#include "geometry/geometry_rectangle.h"
#include "ctrl_multi_step_changer.h"
#include "entity/data_table.h"
#include "entity/data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdint.h>
#include <stdbool.h>

void gui_sketch_object_creator_init ( gui_sketch_object_creator_t *this_,
                                      ctrl_controller_t *controller,
                                      data_database_reader_t *db_reader,
                                      gui_simple_message_to_user_t *message_to_user )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    gui_sketch_defaults_init ( &((*this_).defaults ) );
    data_rules_init ( &((*this_).data_rules ) );
    (*this_).message_to_user = message_to_user;

    U8_TRACE_END();
}

void gui_sketch_object_creator_destroy ( gui_sketch_object_creator_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).message_to_user = NULL;
    data_rules_destroy ( &((*this_).data_rules) );
    gui_sketch_defaults_destroy ( &((*this_).defaults) );
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    U8_TRACE_END();
}

u8_error_t gui_sketch_object_creator_create_classifier ( gui_sketch_object_creator_t *this_,
                                                         data_row_t diagram_id,
                                                         int32_t x_order,
                                                         int32_t y_order,
                                                         data_row_t *out_diagramelement_id,
                                                         data_row_t *out_classifier_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );
    assert ( DATA_ROW_VOID != diagram_id );

    u8_error_t c_result;

    /* get type of diagram */
    data_diagram_type_t diag_type = DATA_DIAGRAM_TYPE_LIST;
    {
        const u8_error_t diag_err
            = data_database_reader_get_diagram_by_id( (*this_).db_reader,
                                                      diagram_id,
                                                      &((*this_).private_temp_diagram)
                                                    );
        if ( U8_ERROR_NONE == diag_err )
        {
            diag_type = data_diagram_get_diagram_type( &((*this_).private_temp_diagram) );
            data_diagram_destroy ( &((*this_).private_temp_diagram) );
        }
        else
        {
            U8_LOG_ERROR_INT( "gui_sketch_object_creator_create_classifier cannot find diagram:", diagram_id );
        }
    }

    /* determine type of new classifier */
    data_classifier_type_t type_of_new_classifier;
    type_of_new_classifier = gui_sketch_defaults_get_classifier_type( &((*this_).defaults), diag_type );

    /* define classifier */
    const u8_error_t d_err
        = data_classifier_init_new ( &((*this_).private_temp_classifier),
                                     type_of_new_classifier,
                                     "",  /* stereotype */
                                     "New Element",  /* name */
                                     "",  /* description */
                                     x_order,
                                     y_order,
                                     y_order  /* y_order used also as list_order */
                                   );
    if ( d_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX("data_classifier_init_new failed in gui_sketch_object_creator_create_classifier:",d_err);
    }

    /* create classifier, adapt name if necessary: */
    {
        ctrl_multi_step_changer_t multi_stepper;
        ctrl_multi_step_changer_init( &multi_stepper, (*this_).controller, (*this_).db_reader );
        u8_error_t out_info;
        c_result = ctrl_multi_step_changer_create_classifier( &multi_stepper,
                                                              &((*this_).private_temp_classifier),
                                                              &out_info
                                                            );
        *out_classifier_id = data_classifier_get_row_id( &((*this_).private_temp_classifier) );
        ctrl_multi_step_changer_destroy( &multi_stepper );
    }

    if ( U8_ERROR_NONE == c_result )
    {
        /* get diagram controller */
        ctrl_diagram_controller_t *const diagram_control = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       diagram_id,
                                       *out_classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ROW_VOID
                                     );

        c_result = ctrl_diagram_controller_create_diagramelement ( diagram_control,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   out_diagramelement_id
                                                                 );

        /* cleanup */
        data_diagramelement_destroy ( &new_diagele );
    }
    else if ( U8_ERROR_READ_ONLY_DB == c_result )
    {
        /* notify read-only warning to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                               );
    }

    /* cleanup */
    data_classifier_destroy ( &((*this_).private_temp_classifier) );

    U8_TRACE_END_ERR( c_result );
    return c_result;
}

u8_error_t gui_sketch_object_creator_create_classifier_as_child ( gui_sketch_object_creator_t *this_,
                                                                  data_row_t diagram_id,
                                                                  data_row_t parent_classifier_id,
                                                                  int32_t x_order,
                                                                  int32_t y_order,
                                                                  data_row_t *out_diagramelement_id,
                                                                  data_row_t *out_classifier_id,
                                                                  data_row_t *out_relationship_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );
    assert ( NULL != out_relationship_id );
    assert ( DATA_ROW_VOID != parent_classifier_id );
    assert ( DATA_ROW_VOID != diagram_id );

    u8_error_t c_result;

    c_result = gui_sketch_object_creator_create_classifier( this_,
                                                            diagram_id,
                                                            x_order,
                                                            y_order,
                                                            out_diagramelement_id,
                                                            out_classifier_id
                                                          );

    if ( U8_ERROR_NONE == c_result )
    {
        /* get classifier controller */
        ctrl_classifier_controller_t *classifier_control;
        classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

        /* define relationship */
        const u8_error_t d_err
            = data_relationship_init_new( &((*this_).private_temp_relationship),
                                          parent_classifier_id,
                                          DATA_ROW_VOID,
                                          *out_classifier_id,
                                          DATA_ROW_VOID,
                                          DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
                                          "", /* =stereotype */
                                          "", /* =name */
                                          "", /* =description */
                                          y_order  /* =list_order */
                                        );
        if ( d_err != U8_ERROR_NONE )
        {
            U8_LOG_ERROR_HEX("data_relationship_init failed in gui_sketch_object_creator_create_classifier_as_child:",d_err);
        }

        /* create relationship */
        c_result = ctrl_classifier_controller_create_relationship( classifier_control,
                                                                   &((*this_).private_temp_relationship),
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                   out_relationship_id
                                                                 );

        /* cleanup */
        data_relationship_destroy( &((*this_).private_temp_relationship) );
    }

    U8_TRACE_END_ERR( c_result );
    return c_result;
}

u8_error_t gui_sketch_object_creator_create_diagram ( gui_sketch_object_creator_t *this_,
                                                      data_row_t parent_diagram_id,
                                                      int32_t list_order,
                                                      data_row_t *out_diagram_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_diagram_id );

    u8_error_t c_result;

    ctrl_diagram_controller_t *diag_control;
    diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

    /* create the diagram */
    const u8_error_t d_err
        = data_diagram_init_new( &((*this_).private_temp_diagram),
                                 parent_diagram_id,
                                 DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM,
                                 "",
                                 "New Diagram",
                                 "",
                                 list_order,
                                 DATA_DIAGRAM_FLAG_NONE
                               );
    if ( d_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX("data_diagram_init_new failed in gui_sketch_object_creator_create_diagram:",d_err);
    }

    c_result = ctrl_diagram_controller_create_diagram( diag_control,
                                                       &((*this_).private_temp_diagram),
                                                       CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                       out_diagram_id
                                                     );
    if ( U8_ERROR_READ_ONLY_DB == c_result )
    {
        /* notify read-only warning to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                               );
    }

    /* cleanup */
    if ( d_err == U8_ERROR_NONE )
    {
        data_diagram_destroy ( &((*this_).private_temp_diagram) );
    }

    U8_TRACE_END_ERR( c_result );
    return c_result;
}

u8_error_t gui_sketch_object_creator_create_relationship ( gui_sketch_object_creator_t *this_,
                                                           data_diagram_type_t diag_type,
                                                           data_row_t from_classifier_id,
                                                           data_row_t from_feature_id,
                                                           data_row_t to_classifier_id,
                                                           data_row_t to_feature_id,
                                                           int32_t list_order,
                                                           data_row_t *out_relationship_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_relationship_id );
    assert ( DATA_ROW_VOID != from_classifier_id );
    assert ( DATA_ROW_VOID != to_classifier_id );

    u8_error_t c_result;

    /* get classifier controller */
    ctrl_classifier_controller_t *classifier_control;
    classifier_control = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

    /* propose a type for the relationship */
    data_relationship_type_t new_rel_type = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;
    {
        /* get type of from_classifier */
        data_classifier_type_t from_class_type = DATA_CLASSIFIER_TYPE_CLASS;
        {
            const u8_error_t clsfy_err
                = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                             from_classifier_id,
                                                             &((*this_).private_temp_classifier)
                                                           );
            if ( U8_ERROR_NONE == clsfy_err )
            {
                from_class_type = data_classifier_get_main_type( &((*this_).private_temp_classifier) );
                data_classifier_destroy ( &((*this_).private_temp_classifier) );
            }
            else
            {
                U8_LOG_ERROR_INT( "gui_sketch_object_creator_create_relationship cannot find classifier:", from_classifier_id );
            }
        }

        /* get type of from_feature */
        data_feature_type_t from_feature_type = DATA_FEATURE_TYPE_VOID;
        if ( from_feature_id != DATA_ROW_VOID )
        {
            const u8_error_t feat_err
                = data_database_reader_get_feature_by_id( (*this_).db_reader,
                                                          from_feature_id,
                                                          &((*this_).private_temp_feature)
                                                        );
            if ( U8_ERROR_NONE == feat_err )
            {
                from_feature_type = data_feature_get_main_type( &((*this_).private_temp_feature) );
                data_feature_destroy ( &((*this_).private_temp_feature) );
            }
            else
            {
                U8_LOG_ERROR_INT( "gui_sketch_object_creator_create_relationship cannot find feature:", from_feature_id );
            }
        }
        new_rel_type = gui_sketch_defaults_get_relationship_type( &((*this_).defaults), from_class_type, from_feature_type );
    }

    /* define relationship struct */
    const u8_error_t d_err
        = data_relationship_init_new( &((*this_).private_temp_relationship),
                                      from_classifier_id,
                                      from_feature_id,
                                      to_classifier_id,
                                      to_feature_id,
                                      new_rel_type,
                                      "", /* =stereotype */
                                      "", /* =name */
                                      "", /* =description */
                                      list_order
                                    );
    if ( d_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX("data_relationship_init failed in gui_sketch_object_creator_create_relationship:",d_err);
    }

    /* check preconditions */
    const bool is_scenario = data_rules_diagram_is_scenario ( &((*this_).data_rules), diag_type )
                             && (( from_feature_id != DATA_ROW_VOID )||( to_feature_id != DATA_ROW_VOID ));
    const bool diagram_ok = is_scenario
                            ? data_rules_diagram_shows_scenario_relationships ( &((*this_).data_rules), diag_type )
                            : data_rules_diagram_shows_uncond_relationships ( &((*this_).data_rules), diag_type );

    if ( diagram_ok ) {
        /* create relationship */
        c_result = ctrl_classifier_controller_create_relationship( classifier_control,
                                                                   &((*this_).private_temp_relationship),
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                   out_relationship_id
                                                                 );
        if ( U8_ERROR_READ_ONLY_DB == c_result )
        {
            /* notify read-only warning to user */
            gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                     GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                     GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                   );
        }
    }
    else
    {
        /* notify error to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_RELATIONSHIPS
                                               );
        c_result = U8_ERROR_DIAGRAM_HIDES_RELATIONSHIPS;
    }

    /* cleanup */
    data_relationship_destroy( &((*this_).private_temp_relationship) );

    U8_TRACE_END_ERR( c_result );
    return c_result;
}

u8_error_t gui_sketch_object_creator_create_feature ( gui_sketch_object_creator_t *this_,
                                                      data_diagram_type_t diag_type,
                                                      data_row_t parent_classifier_id,
                                                      int32_t std_list_order,
                                                      int32_t port_list_order,
                                                      data_row_t *out_feature_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_feature_id );
    assert ( DATA_ROW_VOID != parent_classifier_id );

    u8_error_t c_result;

    /* get classifier controller */
    ctrl_classifier_controller_t *classifier_control;
    classifier_control = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

    /* get type of parent classifier */
    data_classifier_type_t parent_class_type = DATA_CLASSIFIER_TYPE_CLASS;
    {
        const u8_error_t clsfy_err
            = data_database_reader_get_classifier_by_id( (*this_).db_reader,
                                                         parent_classifier_id,
                                                         &((*this_).private_temp_classifier)
                                                       );
        if ( U8_ERROR_NONE == clsfy_err )
        {
            parent_class_type = data_classifier_get_main_type( &((*this_).private_temp_classifier) );
            data_classifier_destroy( &((*this_).private_temp_classifier) );
        }
        else
        {
            U8_LOG_ERROR_INT( "gui_sketch_object_creator_create_feature cannot find classifier:", parent_classifier_id );
        }
    }

    /* propose a type for the feature */
    data_feature_type_t new_feature_type;
    new_feature_type = gui_sketch_defaults_get_feature_type( &((*this_).defaults), parent_class_type );

    /* select the right list_order */
    int32_t list_order;
    if ( ( DATA_FEATURE_TYPE_PROVIDED_INTERFACE == new_feature_type )
        || ( DATA_FEATURE_TYPE_REQUIRED_INTERFACE == new_feature_type )
        || ( DATA_FEATURE_TYPE_PORT == new_feature_type )
        || ( DATA_FEATURE_TYPE_IN_PORT_PIN == new_feature_type )
        || ( DATA_FEATURE_TYPE_OUT_PORT_PIN == new_feature_type )
        || ( DATA_FEATURE_TYPE_ENTRY == new_feature_type )
        || ( DATA_FEATURE_TYPE_EXIT == new_feature_type ) )
    {
        list_order = port_list_order;
    }
    else  /* DATA_FEATURE_TYPE_PROPERTY or DATA_FEATURE_TYPE_OPERATION or DATA_FEATURE_TYPE_TAGGED_VALUE */
    {
        list_order = std_list_order;
    }

    /* define feature struct */
    const u8_error_t data_err
        = data_feature_init_new( &((*this_).private_temp_feature),
                                 new_feature_type,
                                 parent_classifier_id,
                                 "New Feature",  /* name */
                                 "",  /* type/value */
                                 "",
                                 list_order
                               );
    if ( data_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX("data_feature_init failed in gui_sketch_object_creator_create_feature:",data_err);
    }

    /* check preconditions */
    const bool classifier_ok = data_rules_classifier_has_uncond_features ( &((*this_).data_rules), parent_class_type );
    const bool is_scenario = data_rules_feature_is_scenario_cond ( &((*this_).data_rules), new_feature_type );
    assert ( ! is_scenario );  /* lifelines should not be created by this function */
    const bool diagram_ok = is_scenario
                            ? data_rules_diagram_shows_scenario_features ( &((*this_).data_rules), diag_type )
                            : data_rules_diagram_shows_uncond_features ( &((*this_).data_rules), diag_type );

    if ( diagram_ok && classifier_ok )
    {
        /* create feature */
        c_result = ctrl_classifier_controller_create_feature( classifier_control,
                                                              &((*this_).private_temp_feature),
                                                              CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                              out_feature_id
                                                            );
        if ( U8_ERROR_READ_ONLY_DB == c_result )
        {
            /* notify read-only warning to user */
            gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                     GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                     GUI_SIMPLE_MESSAGE_CONTENT_DB_IS_READ_ONLY
                                                   );
        }
    }
    else if ( ! classifier_ok )
    {
        /* notify error to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_FEATURELESS_CLASSIFIER
                                               );
        c_result = U8_ERROR_CLASSIFIER_REFUSES_FEATURE;
    }
    else
    {
        /* notify error to user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_FEATURES
                                               );
        c_result = U8_ERROR_DIAGRAM_HIDES_FEATURES;
    }

    /* cleanup */
    data_feature_destroy( &((*this_).private_temp_feature) );

    U8_TRACE_END_ERR( c_result );
    return c_result;
}


/*
Copyright 2017-2025 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
