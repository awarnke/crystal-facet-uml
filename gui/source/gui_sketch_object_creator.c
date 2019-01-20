/* File: gui_sketch_object_creator.c; Copyright and License: see below */

#include "gui_sketch_object_creator.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_table.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <stdint.h>
#include <stdbool.h>

void gui_sketch_object_creator_init ( gui_sketch_object_creator_t *this_,
                                     ctrl_controller_t *controller,
                                     data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    data_rules_init ( &((*this_).data_rules) );

    TRACE_END();
}

void gui_sketch_object_creator_destroy ( gui_sketch_object_creator_t *this_ )
{
    TRACE_BEGIN();

    data_rules_destroy ( &((*this_).data_rules) );
    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    TRACE_END();
}

ctrl_error_t gui_sketch_object_creator_create_classifier ( gui_sketch_object_creator_t *this_,
                                                           int64_t diagram_id,
                                                           int32_t x_order,
                                                           int32_t y_order,
                                                           int64_t *out_diagramelement_id,
                                                           int64_t *out_classifier_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );
    assert ( DATA_ID_VOID_ID != diagram_id );

    ctrl_error_t c_result;

    /* get classifier controller */
    ctrl_classifier_controller_t *classifier_control;
    classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

    /* get diagram controller */
    ctrl_diagram_controller_t *diagram_control;
    diagram_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

    /* get type of diagram */
    data_diagram_type_t diag_type = DATA_DIAGRAM_TYPE_LIST;
    {
        data_error_t diag_err;
        diag_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader,
                                                            diagram_id,
                                                            &((*this_).private_temp_diagram) );
        if ( DATA_ERROR_NONE == diag_err )
        {
            diag_type = data_diagram_get_diagram_type( &((*this_).private_temp_diagram) );
            data_diagram_destroy ( &((*this_).private_temp_diagram) );
        }
        else
        {
            TSLOG_ERROR_INT( "gui_sketch_object_creator_create_classifier cannot find diagram:", diagram_id );
        }
    }

    /* determine type of new classifier */
    data_classifier_type_t type_of_new_classifier;
    type_of_new_classifier = data_rules_get_default_classifier_type( &((*this_).data_rules), diag_type );

    /* find a good, unused name */
    char newname_buf[DATA_CLASSIFIER_MAX_NAME_SIZE];
    utf8stringbuf_t full_new_name = UTF8STRINGBUF( newname_buf );
    {
        bool name_ok = false;
        static const int MAX_SEARCH_STEP = 64;
        static int my_counter = 0;
        for ( int search_step = 0; ( search_step < MAX_SEARCH_STEP )&&( ! name_ok ); search_step ++ )
        {
            my_counter ++;
            gui_sketch_object_creator_private_propose_classifier_name( this_, type_of_new_classifier, full_new_name );
            utf8stringbuf_append_str( full_new_name, "-" );
            utf8stringbuf_append_int( full_new_name, my_counter+search_step );

            /* check if that name is already in use */
            data_error_t exists_err;
            exists_err = data_database_reader_get_classifier_by_name ( (*this_).db_reader,
                                                                       utf8stringbuf_get_string( full_new_name ),
                                                                       &((*this_).private_temp_classifier) );
            if ( DATA_ERROR_NONE == exists_err )
            {
                data_classifier_destroy ( &((*this_).private_temp_classifier) );
                name_ok = false;  /* name already in use */
            }
            else
            {
                name_ok = true;  /* name unused */
            }
        }
    }

    /* define classifier */
    data_error_t d_err;
    d_err = data_classifier_init_new ( &((*this_).private_temp_classifier),
                                       type_of_new_classifier,
                                       "",  /* stereotype */
                                       utf8stringbuf_get_string( full_new_name ),
                                       "",  /* description */
                                       x_order,
                                       y_order,
                                       y_order  /* y_order used also as list_order */
                                     );
    if ( d_err != DATA_ERROR_NONE )
    {
        TSLOG_ERROR_HEX("data_classifier_init_new failed in gui_sketch_object_creator_create_classifier:",d_err);
    }

    c_result = ctrl_classifier_controller_create_classifier ( classifier_control,
                                                              &((*this_).private_temp_classifier),
                                                              false,  /* add_to_latest_undo_set */
                                                              out_classifier_id
                                                            );

    if ( CTRL_ERROR_NONE == c_result )
    {
        data_diagramelement_t new_diagele;
        data_diagramelement_init_new ( &new_diagele,
                                       diagram_id,
                                       *out_classifier_id,
                                       DATA_DIAGRAMELEMENT_FLAG_NONE,
                                       DATA_ID_VOID_ID
                                     );

        c_result = ctrl_diagram_controller_create_diagramelement ( diagram_control,
                                                                   &new_diagele,
                                                                   true,  /* add_to_latest_undo_set */
                                                                   out_diagramelement_id
                                                                 );

        data_diagramelement_destroy ( &new_diagele );
    }

    data_classifier_destroy ( &((*this_).private_temp_classifier) );

    TRACE_END_ERR( c_result );
    return c_result;
}

ctrl_error_t gui_sketch_object_creator_create_classifier_as_child ( gui_sketch_object_creator_t *this_,
                                                                    int64_t diagram_id,
                                                                    int64_t parent_classifier_id,
                                                                    int32_t x_order,
                                                                    int32_t y_order,
                                                                    int64_t *out_diagramelement_id,
                                                                    int64_t *out_classifier_id,
                                                                    int64_t *out_relationship_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_classifier_id );
    assert ( NULL != out_diagramelement_id );
    assert ( NULL != out_relationship_id );
    assert ( DATA_ID_VOID_ID != parent_classifier_id );
    assert ( DATA_ID_VOID_ID != diagram_id );

    ctrl_error_t c_result;

    c_result = gui_sketch_object_creator_create_classifier ( this_,
                                                             diagram_id,
                                                             x_order,
                                                             y_order,
                                                             out_diagramelement_id,
                                                             out_classifier_id
                                                           );

    if ( CTRL_ERROR_NONE == c_result )
    {
        /* get classifier controller */
        ctrl_classifier_controller_t *classifier_control;
        classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

        /* define relationship */
        data_error_t d_err;
        d_err = data_relationship_init ( &((*this_).private_temp_relationship),
                                         DATA_ID_VOID_ID,
                                         DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
                                         parent_classifier_id,
                                         *out_classifier_id,
                                         "", /* =relationship_name */
                                         "", /* =relationship_description */
                                         y_order,  /* =list_order */
                                         DATA_ID_VOID_ID,
                                         DATA_ID_VOID_ID
                                       );
        if ( d_err != DATA_ERROR_NONE )
        {
            TSLOG_ERROR_HEX("data_relationship_init failed in gui_sketch_object_creator_create_classifier_as_child:",d_err);
        }

        /* create relationship */
        c_result = ctrl_classifier_controller_create_relationship ( classifier_control,
                                                                    &((*this_).private_temp_relationship),
                                                                    true, /*=add_to_latest_undo_set*/
                                                                    out_relationship_id
                                                                  );
        data_relationship_destroy ( &((*this_).private_temp_relationship) );
    }

    TRACE_END_ERR( c_result );
    return c_result;
}

ctrl_error_t gui_sketch_object_creator_create_diagram ( gui_sketch_object_creator_t *this_,
                                                        int64_t parent_diagram_id,
                                                        int32_t list_order,
                                                        int64_t *out_diagram_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_diagram_id );

    ctrl_error_t c_result;

    ctrl_diagram_controller_t *diag_control;
    diag_control = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

    char newname_buf[DATA_DIAGRAM_MAX_NAME_SIZE];
    utf8stringbuf_t new_name = UTF8STRINGBUF( newname_buf );
    gui_sketch_object_creator_private_propose_diagram_name( this_, new_name );

    /* create the diagram */
    data_error_t d_err;
    d_err = data_diagram_init_new ( &((*this_).private_temp_diagram),
                                    parent_diagram_id,
                                    DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM,
                                    utf8stringbuf_get_string(new_name),
                                    "",
                                    list_order
                                  );
    if ( d_err != DATA_ERROR_NONE )
    {
        TSLOG_ERROR_HEX("data_diagram_init_new failed in gui_sketch_object_creator_create_diagram:",d_err);
    }

    c_result = ctrl_diagram_controller_create_diagram ( diag_control,
                                                        &((*this_).private_temp_diagram),
                                                        false,  /* add_to_latest_undo_set */
                                                        out_diagram_id
                                                      );

    if ( d_err == DATA_ERROR_NONE )
    {
        data_diagram_destroy ( &((*this_).private_temp_diagram) );
    }

    TRACE_END_ERR( c_result );
    return c_result;
}

ctrl_error_t gui_sketch_object_creator_create_relationship ( gui_sketch_object_creator_t *this_,
                                                             int64_t from_classifier_id,
                                                             int64_t to_classifier_id,
                                                             int32_t list_order,
                                                             int64_t from_feature_id,
                                                             int64_t to_feature_id,
                                                             int64_t *out_relationship_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_relationship_id );
    assert ( DATA_ID_VOID_ID != from_classifier_id );
    assert ( DATA_ID_VOID_ID != to_classifier_id );

    ctrl_error_t c_result;

    /* get classifier controller */
    ctrl_classifier_controller_t *classifier_control;
    classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

    /* get type of from_classifier */
    data_classifier_type_t from_class_type = DATA_CLASSIFIER_TYPE_UML_CLASS;
    {
        data_error_t clsfy_err;
        clsfy_err = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                from_classifier_id,
                                                                &((*this_).private_temp_classifier) );
        if ( DATA_ERROR_NONE == clsfy_err )
        {
            from_class_type = data_classifier_get_main_type( &((*this_).private_temp_classifier) );
            data_classifier_destroy ( &((*this_).private_temp_classifier) );
        }
        else
        {
            TSLOG_ERROR_INT( "gui_sketch_object_creator_create_relationship cannot find classifier:", from_classifier_id );
        }
    }

    /* propose a type for the relationship */
    data_relationship_type_t new_rel_type;
    new_rel_type = data_rules_get_default_relationship_type ( &((*this_).data_rules), from_class_type );

    /* define relationship */
    data_error_t d_err;
    d_err = data_relationship_init ( &((*this_).private_temp_relationship),
                                     DATA_ID_VOID_ID,
                                     new_rel_type,
                                     from_classifier_id,
                                     to_classifier_id,
                                     "", /* =relationship_name */
                                     "", /* =relationship_description */
                                     list_order,
                                     from_feature_id,
                                     to_feature_id
                                   );
    if ( d_err != DATA_ERROR_NONE )
    {
        TSLOG_ERROR_HEX("data_relationship_init failed in gui_sketch_object_creator_create_relationship:",d_err);
    }

    /* create relationship */
    c_result = ctrl_classifier_controller_create_relationship ( classifier_control,
                                                                &((*this_).private_temp_relationship),
                                                                false, /*=add_to_latest_undo_set*/
                                                                out_relationship_id
                                                              );
    data_relationship_destroy ( &((*this_).private_temp_relationship) );

    TRACE_END_ERR( c_result );
    return c_result;
}

ctrl_error_t gui_sketch_object_creator_create_feature ( gui_sketch_object_creator_t *this_,
                                                        int64_t parent_classifier_id,
                                                        int32_t std_list_order,
                                                        int32_t port_list_order,
                                                        int64_t *out_feature_id )
{
    TRACE_BEGIN();
    assert ( NULL != out_feature_id );
    assert ( DATA_ID_VOID_ID != parent_classifier_id );

    ctrl_error_t c_result;

    /* get classifier controller */
    ctrl_classifier_controller_t *classifier_control;
    classifier_control = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

    /* get type of parent classifier */
    data_classifier_type_t parent_class_type = DATA_CLASSIFIER_TYPE_UML_CLASS;
    {
        data_error_t clsfy_err;
        clsfy_err = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                parent_classifier_id,
                                                                &((*this_).private_temp_classifier) );
        if ( DATA_ERROR_NONE == clsfy_err )
        {
            parent_class_type = data_classifier_get_main_type( &((*this_).private_temp_classifier) );
            data_classifier_destroy ( &((*this_).private_temp_classifier) );
        }
        else
        {
            TSLOG_ERROR_INT( "gui_sketch_object_creator_create_feature cannot find classifier:", parent_classifier_id );
        }
    }

    /* propose a type for the feature */
    data_feature_type_t new_feature_type;
    new_feature_type = data_rules_get_default_feature_type ( &((*this_).data_rules), parent_class_type );

    /* select the right list_order */
    int32_t list_order;
    if ( ( DATA_FEATURE_TYPE_PROVIDED_INTERFACE == new_feature_type )
        || ( DATA_FEATURE_TYPE_REQUIRED_INTERFACE == new_feature_type )
        || ( DATA_FEATURE_TYPE_PORT == new_feature_type ) )
    {
        list_order = port_list_order;
    }
    else  /* DATA_FEATURE_TYPE_PROPERTY or DATA_FEATURE_TYPE_OPERATION */
    {
        list_order = std_list_order;
    }

    /* find a good default name */
    char newname_buf[DATA_CLASSIFIER_MAX_NAME_SIZE];
    utf8stringbuf_t full_new_name = UTF8STRINGBUF( newname_buf );
    char newtype_buf[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE];
    utf8stringbuf_t full_new_type = UTF8STRINGBUF( newtype_buf );
    gui_sketch_object_creator_private_propose_feature_name( this_, new_feature_type, full_new_name, full_new_type );

    /* define feature */
    data_error_t data_err;
    data_err = data_feature_init ( &((*this_).private_temp_feature),
                                   DATA_ID_VOID_ID, /* feature_id */
                                   new_feature_type,
                                   parent_classifier_id,
                                   utf8stringbuf_get_string( full_new_name ),
                                   utf8stringbuf_get_string( full_new_type ),
                                   "",
                                   list_order
                                 );
    if ( data_err != DATA_ERROR_NONE )
    {
        TSLOG_ERROR_HEX("data_feature_init failed in gui_sketch_object_creator_create_feature:",data_err);
    }

    /* create feature */
    c_result = ctrl_classifier_controller_create_feature ( classifier_control,
                                                           &((*this_).private_temp_feature),
                                                           false, /*=add_to_latest_undo_set*/
                                                           out_feature_id
                                                         );
    data_feature_destroy ( &((*this_).private_temp_feature) );

    TRACE_END_ERR( c_result );
    return c_result;
}

void gui_sketch_object_creator_private_propose_diagram_name( gui_sketch_object_creator_t *this_, utf8stringbuf_t out_name )
{
    static int cycle_names = 0;
    static char *(NAMES[8]) = {"New Overview","New Context","New Structure","New Deployment","New Lifecycle","New Errors","New Use Cases","New Sequence"};

    cycle_names ++;
    utf8stringbuf_copy_str( out_name, NAMES[cycle_names&0x07] );
}

void gui_sketch_object_creator_private_propose_classifier_name( gui_sketch_object_creator_t *this_,
                                                                data_classifier_type_t c_type,
                                                                utf8stringbuf_t out_name )
{
    static int cycle_names = 0;
    static char *(BLOCK_NAMES[8]) = {"New DRAM","New SoC","New NAND","New NOR","New PMIC","New Watchdog","New CPU","New SRAM"};
    static char *(FEATURE_NAMES[8]) = {"New Usability","New Reliability","New Function","New Security","New Performance","New Compatibility","New Maintainability","New Portability"};
    static char *(REQUIREMENT_NAMES[8]) = {"New Reaction Time","New Startup Time","New Latency","New Resource Consumption","New Update","New Fault Tolerance","New Availability","New Hardware Abstraction"};
    static char *(ACTOR_NAMES[8]) = {"New Customer","New Database","New Service/Maintenance","New Operator","New Backend/Server","New Authorities","New Hacker/Cheater","New Peer/Client"};
    static char *(USECASE_NAMES[8]) = {"New Get Status","New Perform Transaction","New SW Update","New Pay Order","New Deliver","New Debug","New Prove Eventlog","New Manage Rights"};
    static char *(BOUNDARIES_NAMES[8]) = {"New Controller SW","New Machine","New Backend","New Virtual Machine","New Security Module","New Terminal","New Smart Device","New PC"};
    static char *(ACTIVITY_NAMES[8]) = {"New Startup","New SW Update","New Background Scan","New Sleep","New User Input","New Normal Operation","New Error Reporting","New Idle"};
    static char *(STATE_NAMES[8]) = {"New Off","New Starting","New On","New Debugging","New Wait","New Shutdown","New Send","New Receive"};
    static char *(DIAGREF_NAMES[8]) = {"New Startup","New Data Upload","New Data Download","New Shutdown","New Suspend and Resume","New Background Activities","New Error Notification","New SW Update"};
    static char *(COMPONENT_NAMES[8]) = {"New Persistence","New ErrorLogger","New UserInterface","New PeripheralControl","New Monitor","New ServiceDiscovery","New LifecycleManager","New Controller"};
    static char *(ARTIFACT_NAMES[8]) = {"New Firmware","New Code","New Data","New Settings","New Log","New ErrorReport","New RuleSet","New Inventory"};
    static char *(IF_NAMES[8]) = {"New Authentication_IF","New Log_IF","New TraceChannel_IF","New Update_IF","New DataSync_IF","New DataLink_IF","New AliveSignal_IF","New PowerControl_IF"};
    static char *(CLASS_NAMES[8]) = {"New Serializer","New Deserializer","New CompressionAlg","New Transformer","New Wrapper","New EventLoop","New RingBuffer","New Multiplexer"};
    static char *(PACKAGE_NAMES[8]) = {"New Drivers","New Platform Services","New User Applications","New Utilities","New Debug Tools","New Authentication and Authorization","New Controller","New Maintenance Tools"};
    static char *(NOTE_NAMES[8]) = {"New Note","New Comment","New Hint","New Todo","New Remark","New Question","New Rationale","New Proposal"};

    cycle_names ++;

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
        {
            utf8stringbuf_copy_str( out_name, BLOCK_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_FEATURE:
        {
            utf8stringbuf_copy_str( out_name, FEATURE_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            utf8stringbuf_copy_str( out_name, REQUIREMENT_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            utf8stringbuf_copy_str( out_name, ACTOR_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            utf8stringbuf_copy_str( out_name, USECASE_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            utf8stringbuf_copy_str( out_name, BOUNDARIES_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_INTERRUPT:
        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            utf8stringbuf_copy_str( out_name, ACTIVITY_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_STATE:
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            utf8stringbuf_copy_str( out_name, STATE_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            utf8stringbuf_copy_str( out_name, DIAGREF_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            utf8stringbuf_copy_str( out_name, BLOCK_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:  /* and */
        {
            utf8stringbuf_copy_str( out_name, COMPONENT_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            utf8stringbuf_copy_str( out_name, COMPONENT_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            utf8stringbuf_copy_str( out_name, ARTIFACT_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:  /* and */
        {
            utf8stringbuf_copy_str( out_name, IF_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_CLASS:  /* and */
        {
            utf8stringbuf_copy_str( out_name, CLASS_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_OBJECT:  /* and */
        {
            utf8stringbuf_copy_str( out_name, CLASS_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            utf8stringbuf_copy_str( out_name, PACKAGE_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            utf8stringbuf_copy_str( out_name, NOTE_NAMES[cycle_names&0x07] );
        }
        break;

        default:
        {
            TSLOG_ERROR("data_classifier_type_t out of range in gui_sketch_object_creator_private_propose_classifier_name");
            utf8stringbuf_clear( out_name );
        }
        break;
    }
}

void gui_sketch_object_creator_private_propose_feature_name( gui_sketch_object_creator_t *this_,
                                                             data_feature_type_t f_type,
                                                             utf8stringbuf_t out_name,
                                                             utf8stringbuf_t out_type )
{
    static int cycle_names = 0;
    static char *(PROPERTY_NAMES[8]) = {"new_state","new_run_mode","new_error_code","new_color","new_name","new_type","new_size","new_weight"};
    static char *(PROPERTY_TYPES[8]) = {"uint32_t","enum","struct","uint8_t[4]","char[48]","","size_t","double"};
    static char *(OPERATION_NAMES[8]) = {"new_start","new_stop","new_pause","new_resume","new_get_state","new_handle_event","new_set_color","new_is_valid"};
    static char *(OPERATION_TYPES[8]) = {"uint32_t()(void)","uint32_t(*)(enum)","","","enum","","uint32_t(*)(uint8_t[4])","bool"};
    static char *(PORT_NAMES[8]) = {"new_in_a","new_in_b","new_in_c","new_out_a","new_out_b","new_out_c","new_out_error","new_in_reset"};
    static char *(PORT_TYPES[8]) = {"","signal","uint16_t","IP-socket","signal","","","bool"};
    static char *(IF_NAMES[8]) = {"New Auth_IF","New Log_IF","New Trace_IF","New Update_IF","New Sync_IF","New Link_IF","New Alive_IF","New Power_IF"};

    cycle_names ++;

    switch ( f_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        {
            utf8stringbuf_copy_str( out_name, PROPERTY_NAMES[cycle_names&0x07] );
            utf8stringbuf_copy_str( out_type, PROPERTY_TYPES[cycle_names&0x07] );
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            utf8stringbuf_copy_str( out_name, OPERATION_NAMES[cycle_names&0x07] );
            utf8stringbuf_copy_str( out_type, OPERATION_TYPES[cycle_names&0x07] );
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            utf8stringbuf_copy_str( out_name, PORT_NAMES[cycle_names&0x07] );
            utf8stringbuf_copy_str( out_type, PORT_TYPES[cycle_names&0x07] );
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            utf8stringbuf_clear( out_name );
            utf8stringbuf_clear( out_type );
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            utf8stringbuf_copy_str( out_name, IF_NAMES[cycle_names&0x07] );
            utf8stringbuf_clear( out_type );
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            utf8stringbuf_copy_str( out_name, IF_NAMES[cycle_names&0x07] );
            utf8stringbuf_clear( out_type );
        }
        break;

        default:
        {
            TSLOG_ERROR("data_feature_type_t out of range in gui_sketch_object_creator_private_propose_feature_name");
            utf8stringbuf_clear( out_name );
            utf8stringbuf_clear( out_type );
        }
        break;
    }
}


/*
Copyright 2017-2019 Andreas Warnke

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

