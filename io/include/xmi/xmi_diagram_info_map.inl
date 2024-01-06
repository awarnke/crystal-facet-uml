/* File: xmi_diagram_info_map.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <stdlib.h>

extern const xmi_diagram_info_t xmi_diagram_info_map_unknown_type;

static inline u8_error_t xmi_diagram_info_map_get_diagram ( const xmi_diagram_info_map_t *this_,
                                                            data_diagram_type_t diagram_type,
                                                            const xmi_diagram_info_t **out_diagram_info )
{
    assert( NULL != out_diagram_info );
    const xmi_diagram_info_t * result = NULL;

    switch ( diagram_type )
    {
        case DATA_DIAGRAM_TYPE_LIST:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_LIST]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_LIST );
        }
        break;

        case DATA_DIAGRAM_TYPE_BOX_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_BOX]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_BOX_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_BLOCK_DEFINITION]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_INTERNAL_BLOCK]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_PARAMETRIC]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_REQUIREMENTS]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_USE_CASE]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_ACTIVITY]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_STATE_MACHINE]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_SEQUENCE]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_COMMUNICATION]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_TIMING]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_INTERACTION_OVERVIEW]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_DEPLOYMENT]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_COMPONENT]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_COMPOSITE_STRUCTURE]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_PACKAGE]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_CLASS]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM );
        }
        break;

        case DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM:
        {
            result = &((*this_)[XMI_DIAGRAM_INFO_MAP_INDEX_PROFILE]);
            assert ( (*result).data_type_checksum == (int)DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM );
        }
        break;

        default:
        {
            U8_LOG_ANOMALY_INT( "switch case statement for data_diagram_type_t incomplete", diagram_type );
            /* this is a possible error case that can happen when */
            /* a database created with a newer version of the program is opened with this version */
            result = NULL;
        }
        break;
    }

    *out_diagram_info = (result==NULL) ? &xmi_diagram_info_map_unknown_type : result;
    return (result==NULL) ? U8_ERROR_NOT_FOUND : U8_ERROR_NONE;
}


/*
Copyright 2022-2024 Andreas Warnke

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
