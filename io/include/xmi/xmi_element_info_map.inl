/* File: xmi_element_info_map.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>
#include <stdlib.h>

static inline const xmi_element_info_t * xmi_element_info_map_static_get_classifier ( const xmi_element_info_map_t *this_,
                                                                                      data_classifier_type_t c_type )
{
    const xmi_element_info_t * result = NULL;

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_BLOCK < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_BLOCK]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_BLOCK );
        }
        break;

        case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_CONSTRAINT_PROPERTY < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CONSTRAINT_PROPERTY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_FEATURE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_REQUIREMENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_REQUIREMENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_ACTOR < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ACTOR]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_ACTOR );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_USE_CASE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_USE_CASE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_USE_CASE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_SYSTEM_BOUNDARY < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_SYSTEM_BOUNDARY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_ACTIVITY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_STATE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_STATE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_STATE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_STATE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DIAGRAM_REFERENCE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DIAGRAM_REFERENCE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_NODE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_COMPONENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_COMPONENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_INTERFACE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_INTERFACE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_INTERFACE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_PACKAGE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_PACKAGE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_PACKAGE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_CLASS < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CLASS]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_CLASS );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_CLASS < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_CLASS]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_CLASS );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_ARTIFACT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_ARTIFACT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_ARTIFACT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_COMMENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_COMMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_UML_COMMENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_INTERRUPTABLE_REGION < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_INTERRUPTABLE_REGION]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_INITIAL_NODE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_INITIAL_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_FINAL_NODE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_FINAL_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_FORK_NODE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_FORK_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_FORK_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_JOIN_NODE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_JOIN_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_DECISION_NODE < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_DECISION_NODE]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_SHALLOW_HISTORY < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_SHALLOW_HISTORY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_DEEP_HISTORY < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_DEEP_HISTORY]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY );
        }
        break;

        /*
        case DATA_CLASSIFIER_TYPE_DYN_PARTITION:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_PARTITION < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_PARTITION]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_PARTITION );
        }
        break;
        */

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_EVENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_EVENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_TIME_EVENT < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_TIME_EVENT]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT );
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            assert ( XMI_ELEMENT_INFO_MAP_INDEX_DYN_SEND_SIGNAL < XMI_ELEMENT_INFO_MAP_INDEX_MAX );
            result = &((*this_)[XMI_ELEMENT_INFO_MAP_INDEX_DYN_SEND_SIGNAL]);
            assert ( (*result).data_type_checksum == (int)DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL );
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

static inline const xmi_element_info_t * xmi_element_info_map_static_get_feature ( const xmi_element_info_map_t *this_,
                                                                                   data_feature_type_t f_type )
{
    return NULL;
}

static inline const xmi_element_info_t * xmi_element_info_map_static_get_relationship ( const xmi_element_info_map_t *this_,
                                                                                        data_relationship_type_t r_type )
{
    return NULL;
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
