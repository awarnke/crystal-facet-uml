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

    int64_t new_diag_id;
    c_result = ctrl_diagram_controller_create_child_diagram ( diag_control,
                                                              parent_diagram_id,
                                                              DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM,
                                                              utf8stringbuf_get_string(new_name),
                                                              out_diagram_id );

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
                                                        int32_t list_order,
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

    /* create feature */
    int64_t new_feature_id;
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
    static char *(NAMES[8]) = {"Overview","Context","Structure","Deployment","Lifecycle","Errors","Use Cases","Sequence"};

    cycle_names ++;
    utf8stringbuf_copy_str( out_name, NAMES[cycle_names&0x07] );
}

void gui_sketch_object_creator_private_propose_classifier_name( gui_sketch_object_creator_t *this_,
                                                                data_classifier_type_t c_type,
                                                                utf8stringbuf_t out_name )
{
    static int cycle_names = 0;
    static char *(BLOCK_NAMES[8]) = {"DRAM","SoC","NAND","NOR","PMIC","Watchdog","CPU","SRAM"};
    static char *(FEATURE_NAMES[8]) = {"Usability","Reliability","Function","Security","Performance","Compatibility","Maintainability","Portability"};
    static char *(REQUIREMENT_NAMES[8]) = {"reaction time","startup time","latency","resource consumption","update","fault tolerance","availability","hardware abstraction"};
    static char *(ACTOR_NAMES[8]) = {"Customer","Database","Service/Maintenance","Operator","Backend/Server","Authorities","Hacker/Cheater","Peer/Client"};
    static char *(USECASE_NAMES[8]) = {"Get Status","Perform Transaction","SW Update","Pay Order","Deliver","Debug","Prove Eventlog","Manage Rights"};
    static char *(BOUNDARIES_NAMES[8]) = {"Controller SW","Machine","Backend","Virtual Machine","Security Module","Terminal","Smart Device","PC"};
    static char *(ACTIVITY_NAMES[8]) = {"Startup","SW Update","Background Scan","Sleep","User Input","Normal Operation","Error Reporting","Idle"};
    static char *(STATE_NAMES[8]) = {"off","starting","on","debugging","wait","shutdown","send","receive"};
    static char *(DIAGREF_NAMES[8]) = {"Startup","Data Upload","Data Download","Shutdown","Suspend and Resume","Background Activities","Error Notification","SW Update"};
    static char *(COMPONENT_NAMES[8]) = {"Persistence","ErrorLogger","UserInterface","PeripheralControl","Monitor","ServiceDiscovery","LifecycleManager","Controller"};
    static char *(ARTIFACT_NAMES[8]) = {"Firmware","Code","Data","Settings","Log","ErrorReport","RuleSet","Inventory"};
    static char *(IF_NAMES[8]) = {"Authentication_IF","Log_IF","TraceChannel_IF","Update_IF","DataSync_IF","DataLink_IF","AliveSignal_IF","PowerControl_IF"};
    static char *(CLASS_NAMES[8]) = {"Serializer","Deserializer","CompressionAlg","Transformer","Wrapper","EventLoop","RingBuffer","Multiplexer"};
    static char *(PACKAGE_NAMES[8]) = {"Drivers","Platform Services","User Applications","Utilities","Debug Tools","Authentication and Authorization","Controller","Maintenance Tools"};
    static char *(NOTE_NAMES[8]) = {"Note","Comment","Hint","Todo","Remark","Question","Rationale","Proposal"};

    cycle_names ++;

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
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
        {
            utf8stringbuf_copy_str( out_name, ACTIVITY_NAMES[cycle_names&0x07] );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_STATE:
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
    static char *(PROPERTY_NAMES[8]) = {"state","run_mode","error_code","color","name","type","size","weight"};
    static char *(PROPERTY_TYPES[8]) = {"uint32_t","enum","struct","uint8_t[4]","char[48]","","size_t","double"};
    static char *(OPERATION_NAMES[8]) = {"start","stop","pause","resume","get_state","handle_event","set_color","is_valid"};
    static char *(OPERATION_TYPES[8]) = {"uint32_t()(void)","uint32_t(*)(enum)","","","enum","","uint32_t(*)(uint8_t[4])","bool"};
    static char *(PORT_NAMES[8]) = {"in_a","in_b","in_c","out_a","out_b","out_c","out_error","in_reset"};
    static char *(PORT_TYPES[8]) = {"","signal","uint16_t","IP-socket","signal","","","bool"};

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
Copyright 2017-2018 Andreas Warnke

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

