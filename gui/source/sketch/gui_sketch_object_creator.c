/* File: gui_sketch_object_creator.c; Copyright and License: see below */

#include "sketch/gui_sketch_object_creator.h"
#include "geometry/geometry_rectangle.h"
#include "ctrl_multi_step_changer.h"
#include "data_table.h"
#include "data_id.h"
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
    data_rules_init ( &((*this_).data_rules ) );
    (*this_).message_to_user = message_to_user;

    U8_TRACE_END();
}

void gui_sketch_object_creator_destroy ( gui_sketch_object_creator_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).message_to_user = NULL;
    data_rules_destroy ( &((*this_).data_rules) );
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    U8_TRACE_END();
}

u8_error_t gui_sketch_object_creator_create_classifier ( gui_sketch_object_creator_t *this_,
                                                         data_row_id_t diagram_id,
                                                         int32_t x_order,
                                                         int32_t y_order,
                                                         data_row_id_t *out_diagramelement_id,
                                                         data_row_id_t *out_classifier_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );
    assert ( DATA_ROW_ID_VOID != diagram_id );

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
    type_of_new_classifier = data_rules_get_default_classifier_type( &((*this_).data_rules), diag_type );

    /* propose a name */
    const char *const full_new_name
        = gui_sketch_object_creator_private_propose_classifier_name( this_, type_of_new_classifier );

    /* define classifier */
    const u8_error_t d_err
        = data_classifier_init_new ( &((*this_).private_temp_classifier),
                                     type_of_new_classifier,
                                     "",  /* stereotype */
                                     full_new_name,
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
                                       DATA_ROW_ID_VOID
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
                                                                  data_row_id_t diagram_id,
                                                                  data_row_id_t parent_classifier_id,
                                                                  int32_t x_order,
                                                                  int32_t y_order,
                                                                  data_row_id_t *out_diagramelement_id,
                                                                  data_row_id_t *out_classifier_id,
                                                                  data_row_id_t *out_relationship_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );
    assert ( NULL != out_relationship_id );
    assert ( DATA_ROW_ID_VOID != parent_classifier_id );
    assert ( DATA_ROW_ID_VOID != diagram_id );

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
                                          DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
                                          parent_classifier_id,
                                          *out_classifier_id,
                                          "", /* =stereotype */
                                          "", /* =name */
                                          "", /* =description */
                                          y_order,  /* =list_order */
                                          DATA_ROW_ID_VOID,
                                          DATA_ROW_ID_VOID
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
                                                      data_row_id_t parent_diagram_id,
                                                      int32_t list_order,
                                                      data_row_id_t *out_diagram_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_diagram_id );

    u8_error_t c_result;

    ctrl_diagram_controller_t *diag_control;
    diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

    const char *const new_name
        = gui_sketch_object_creator_private_propose_diagram_name( this_ );

    /* create the diagram */
    const u8_error_t d_err
        = data_diagram_init_new( &((*this_).private_temp_diagram),
                                 parent_diagram_id,
                                 DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM,
                                 "",
                                 new_name,
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
                                                           data_row_id_t from_classifier_id,
                                                           data_row_id_t from_feature_id,
                                                           data_row_id_t to_classifier_id,
                                                           data_row_id_t to_feature_id,
                                                           int32_t list_order,
                                                           data_row_id_t *out_relationship_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_relationship_id );
    assert ( DATA_ROW_ID_VOID != from_classifier_id );
    assert ( DATA_ROW_ID_VOID != to_classifier_id );

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
        if ( from_feature_id != DATA_ROW_ID_VOID )
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
        new_rel_type = data_rules_get_default_relationship_type( &((*this_).data_rules), from_class_type, from_feature_type );
    }

    /* define relationship struct */
    const u8_error_t d_err
        = data_relationship_init_new( &((*this_).private_temp_relationship),
                                      new_rel_type,
                                      from_classifier_id,
                                      to_classifier_id,
                                      "", /* =stereotype */
                                      "", /* =name */
                                      "", /* =description */
                                      list_order,
                                      from_feature_id,
                                      to_feature_id
                                    );
    if ( d_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX("data_relationship_init failed in gui_sketch_object_creator_create_relationship:",d_err);
    }

    /* check preconditions */
    const bool is_scenario = data_rules_diagram_is_scenario ( &((*this_).data_rules), diag_type )
                             && (( from_feature_id != DATA_ROW_ID_VOID )||( to_feature_id != DATA_ROW_ID_VOID ));
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
                                                      data_row_id_t parent_classifier_id,
                                                      int32_t std_list_order,
                                                      int32_t port_list_order,
                                                      data_row_id_t *out_feature_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_feature_id );
    assert ( DATA_ROW_ID_VOID != parent_classifier_id );

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
    new_feature_type = data_rules_get_default_feature_type( &((*this_).data_rules), parent_class_type );

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

    /* find a good default name */
    const char *const full_new_name
        = gui_sketch_object_creator_private_propose_feature_name( this_, new_feature_type );

    /* define feature struct */
    const u8_error_t data_err
        = data_feature_init_new( &((*this_).private_temp_feature),
                                 new_feature_type,
                                 parent_classifier_id,
                                 full_new_name,
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

const char* gui_sketch_object_creator_private_propose_diagram_name( gui_sketch_object_creator_t *this_ )
{
    static int cycle_names = 0;
    static const char *const (NAMES[8])
        = {"New Overview","New Context","New Structure","New Deployment","New Lifecycle","New Errors","New Use Cases","New Sequence"};

    cycle_names ++;

    return NAMES[cycle_names&0x07];
}

const char* gui_sketch_object_creator_private_propose_classifier_name( gui_sketch_object_creator_t *this_, data_classifier_type_t c_type )
{
    const char* result = "";

    static int cycle_names = 0;
    static const char *const (BLOCK_NAMES[8])
        = {"New DRAM","New SoC","New NAND","New NOR","New PMIC","New Watchdog","New CPU","New SRAM"};
    static const char *(REQUIREMENT_NAMES[8])
        = {"New Reaction Time","New Startup Time","New Latency","New Resource Consumption","New Update","New Fault Tolerance","New Availability","New Hardware Abstraction"};
    static const char *const (ACTOR_NAMES[8])
        = {"New Customer","New Database","New Service/Maintenance","New Operator","New Backend/Server","New Authorities","New Hacker/Cheater","New Peer/Client"};
    static const char *const (USECASE_NAMES[8])
        = {"New Get Status","New Perform Transaction","New SW Update","New Pay Order","New Deliver","New Debug","New Prove Eventlog","New Manage Rights"};
    static const char *const (BOUNDARIES_NAMES[8])
        = {"New Controller SW","New Machine","New Backend","New Virtual Machine","New Security Module","New Terminal","New Smart Device","New PC"};
    static const char *const (ACTIVITY_NAMES[8])
        = {"New Startup","New SW Update","New Background Scan","New Sleep","New User Input","New Normal Operation","New Error Reporting","New Idle"};
    static const char *const (STATE_NAMES[8])
        = {"New Off","New Starting","New On","New Debugging","New Wait","New Shutdown","New Send","New Receive"};
    static const char *const (DIAGREF_NAMES[8])
        = {"New Startup","New Data Upload","New Data Download","New Shutdown","New Suspend and Resume","New Background Activities","New Error Notification","New SW Update"};
    static const char *const (COMPONENT_NAMES[8])
        = {"New Persistence","New ErrorLogger","New UserInterface","New PeripheralControl","New Monitor","New ServiceDiscovery","New LifecycleManager","New Controller"};
    static const char *const (ARTIFACT_NAMES[8])
        = {"New Firmware","New Code","New Data","New Settings","New Log","New ErrorReport","New RuleSet","New Inventory"};
    static const char *const (IF_NAMES[8])
        = {"New Authentication_IF","New Log_IF","New TraceChannel_IF","New Update_IF","New DataSync_IF","New DataLink_IF","New AliveSignal_IF","New PowerControl_IF"};
    static const char *const (CLASS_NAMES[8])
        = {"New Serializer","New Deserializer","New CompressionAlg","New Transformer","New Wrapper","New EventLoop","New RingBuffer","New Multiplexer"};
    static const char *const (PACKAGE_NAMES[8])
        = {"New Drivers","New Platform Services","New User Applications","New Utilities","New Debug Tools","New Authentication and Authorization","New Controller","New Maintenance Tools"};
    static const char *const (NOTE_NAMES[8])
        = {"New Note","New Comment","New Hint","New Todo","New Remark","New Question","New Rationale","New Proposal"};
    static const char *const (STEREOTYPE_NAMES[8])
        = {"New transient","New persistent","New 3rd-party","New entity","New database","New decision","New testcase","New copy-of"};

    cycle_names ++;

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
        {
            result = BLOCK_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            result = REQUIREMENT_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTOR:
        {
            result = ACTOR_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_USE_CASE:
        {
            result = USECASE_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
        {
            result = BOUNDARIES_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            result = ACTIVITY_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_STATE:
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            result = STATE_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:
        {
            result = DIAGREF_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_NODE:
        {
            result = BLOCK_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_COMPONENT:  /* and */
        {
            result = COMPONENT_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_PART:
        {
            result = COMPONENT_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_ARTIFACT:
        {
            result = ARTIFACT_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_INTERFACE:  /* and */
        {
            result = IF_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_CLASS:  /* and */
        {
            result = CLASS_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_OBJECT:  /* and */
        {
            result = CLASS_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_PACKAGE:
        {
            result = PACKAGE_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_COMMENT:
        {
            result = NOTE_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_CLASSIFIER_TYPE_STEREOTYPE:
        case DATA_CLASSIFIER_TYPE_IMAGE:
        {
            result = STEREOTYPE_NAMES[cycle_names&0x07];
        }
        break;

        default:
        {
            U8_LOG_ERROR("data_classifier_type_t out of range in gui_sketch_object_creator_private_propose_classifier_name");
            result = "";
        }
        break;
    }

    return result;
}

const char* gui_sketch_object_creator_private_propose_feature_name( gui_sketch_object_creator_t *this_, data_feature_type_t f_type )
{
    const char* result = "";

    static int cycle_names = 0;
    static const char *const (PROPERTY_NAMES[8])
        = {"new_state","new_run_mode","new_error_code","new_color","new_name","new_type","new_size","new_weight"};
    static const char *const (OPERATION_NAMES[8])
        = {"new_start","new_stop","new_pause","new_resume","new_get_state","new_handle_event","new_set_color","new_is_valid"};
    static const char *const (PORT_NAMES[8])
        = {"new_in_a","new_in_b","new_in_c","new_out_a","new_out_b","new_out_c","new_out_error","new_in_reset"};
    static const char *const (IO_PORT_NAMES[8])
        = {"new_order","new_item","new_error","new_report","new_audio_file","new_video_file","new_plan","new_status"};
    static const char *const (IF_NAMES[8])
        = {"New Auth_IF","New Log_IF","New Trace_IF","New Update_IF","New Sync_IF","New Link_IF","New Alive_IF","New Power_IF"};
    static const char *const (ENTRY_NAMES[8])
        = {"new_again","new_first_time","new_error_case","new_std_entry","new_retries_exceeded","new_debug","new_rookie_mode","new_last_try"};
    static const char *const (EXIT_NAMES[8])
        = {"new_abort","new_std_exit","new_precondition_failed","new_warning","new_error","new_retry","new_ok","new_repair_request"};

    cycle_names ++;

    switch ( f_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        case DATA_FEATURE_TYPE_TAGGED_VALUE:
        {
            result = PROPERTY_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            result = OPERATION_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            result = PORT_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            result = "";
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            result = IF_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            result = IF_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_IN_PORT_PIN:
        case DATA_FEATURE_TYPE_OUT_PORT_PIN:
        {
            result = IO_PORT_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_ENTRY:
        {
            result = ENTRY_NAMES[cycle_names&0x07];
        }
        break;

        case DATA_FEATURE_TYPE_EXIT:
        {
            result = EXIT_NAMES[cycle_names&0x07];
        }
        break;

        default:
        {
            U8_LOG_ERROR("data_feature_type_t out of range in gui_sketch_object_creator_private_propose_feature_name");
            result = "";
        }
        break;
    }

    return result;
}


/*
Copyright 2017-2023 Andreas Warnke

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

