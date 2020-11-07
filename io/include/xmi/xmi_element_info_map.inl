/* File: xmi_element_info_map.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>
#include <stdlib.h>

static inline const xmi_element_info_t * xmi_element_info_map_get_classifier ( const xmi_element_info_map_t *this_,
                                                                               data_classifier_type_t c_type,
                                                                               bool statemachine_context )
{
    const xmi_element_info_t * result = NULL;

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_BLOCK]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_BLOCK );
        }
        break;

        case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CONSTRAINT_BLOCK]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK );
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_REQUIREMENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ACTOR]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_ACTOR );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_USE_CASE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_USE_CASE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_SYSTEM_BOUNDARY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_ACTIVITY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_STATE:
        {
            if ( statemachine_context )
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_STATE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_STATE );
            }
            else
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_STATEMACHINE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_STATE );
            }
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DIAGRAM_REFERENCE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_COMPONENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_COMPONENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_INTERFACE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_INTERFACE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_PACKAGE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_PACKAGE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CLASS]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_CLASS );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CLASS]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_CLASS );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ARTIFACT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_ARTIFACT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_COMMENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_INTERRUPTABLE_REGION]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        {
            if ( statemachine_context )
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_INITIAL_NODE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE );
            }
            else
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_INITIAL_NODE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE );
            }
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        {
            if ( statemachine_context )
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_FINAL_NODE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE );
            }
            else
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_FINAL_NODE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE );
            }
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_FORK_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_FORK_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_JOIN_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        {
            if ( statemachine_context )
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_DECISION_NODE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE );
            }
            else
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_DECISION_NODE]);
                assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE );
            }
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_SHALLOW_HISTORY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_DEEP_HISTORY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY );
        }
        break;

        /*
        case DATA_CLASSIFIER_TYPE_DYN_PARTITION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_PARTITION]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_PARTITION );
        }
        break;
        */

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_EVENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_TIME_EVENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_SEND_SIGNAL]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL );
        }
        break;

        case DATA_CLASSIFIER_TYPE_INTERACTION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_INTERACTION]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_INTERACTION );
        }
        break;
        
        default:
        {
            TSLOG_ERROR_INT( "switch case statement for data_classifier_type_t incomplete", c_type );
            assert( 0 );
        }
        break;
    }

    return result;
}

static inline const xmi_element_info_t * xmi_element_info_map_get_feature ( const xmi_element_info_map_t *this_,
                                                                            data_feature_type_t f_type )
{
    const xmi_element_info_t * result = NULL;

    switch ( f_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_PROPERTY]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_PROPERTY );
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_OPERATION]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_OPERATION );
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_PORT]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_PORT );
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_LIFELINE]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_LIFELINE );
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_PROVIDED_INTERFACE]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_PROVIDED_INTERFACE );
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_REQUIRED_INTERFACE]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_REQUIRED_INTERFACE );
        }
        break;

        /*
        case DATA_FEATURE_TYPE_TESTCASE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_TESTCAS]);
            assert ( (*result).data_type_checksum == (int)DATA_FEATURE_TYPE_TESTCASE );
        }
        break;
        */

        default:
        {
            TSLOG_ERROR_INT( "switch case statement for data_relationship_type_t incomplete", f_type );
            assert( 0 );
        }
        break;
    }

    return result;
}

static inline const xmi_element_info_t * xmi_element_info_map_get_relationship ( const xmi_element_info_map_t *this_,
                                                                                 data_relationship_type_t r_type,
                                                                                 bool statemachine_context )
{
    const xmi_element_info_t * result = NULL;

    switch ( r_type )
    {
        case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DEPENDENCY]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ASSOCIATION]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_AGGREGATION]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_AGGREGATION );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMPOSITION]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_COMPOSITION );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_GENERALIZATION]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_REALIZATION]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_REALIZATION );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ASYNC_CALL]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_SYNC_CALL]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_RETURN_CALL]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMMUNICATION_PATH]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
        {
            if ( statemachine_context )
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_TRANSITION]);
                assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW );
            }
            else
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CONTROL_FLOW]);
                assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW );
            }
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
        {
            if ( statemachine_context )
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_TRANSITION]);
                assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW ); /*mix on purpose*/
            }
            else
            {
                result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_OBJECT_FLOW]);
                assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW );
            }
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DEPLOY]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_DEPLOY );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_MANIFEST]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_MANIFEST );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_EXTEND]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_EXTEND );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_INCLUDE]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_INCLUDE );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CONTAINMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REFINE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_REFINE]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_REFINE );
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_TRACE]);
            assert ( (*result).data_type_checksum == (int)DATA_RELATIONSHIP_TYPE_UML_TRACE );
        }
        break;

        default:
        {
            TSLOG_ERROR_INT( "switch case statement for data_relationship_type_t incomplete", r_type );
            assert( 0 );
        }
        break;
    }

    return result;
}


/*
Copyright 2020-2020 Andreas Warnke

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
