/* File: io_txt_icon.inl; Copyright and License: see below */

#include <assert.h>
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

#define IO_TXT_ICON_TWO_MIDDLE_DOTS "\xC2\xB7\xC2\xB7"

static inline void io_txt_icon_init( io_txt_icon_t *this_ )
{
}

static inline void io_txt_icon_destroy( io_txt_icon_t *this_ )
{
}

static inline const char* io_txt_icon_get_relationship ( io_txt_icon_t *this_, data_relationship_type_t type )
{
    const char* result;

    switch (type)
    {
        case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
        {
            result = "-->";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
        {
            result = "o--";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
        {
            result = "*--";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
        {
            result = "--|>";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS "|>";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
        {
            result = "-->";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
        {
            result = "-->>";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">>";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
        {
            result = "--";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
        {
            result = "-->";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
        {
            result = "#-->";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
        {
            result = "+--";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REFINE:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
        }
        break;

        default:
        {
            result = IO_TXT_ICON_TWO_MIDDLE_DOTS ">";
            U8_LOG_ERROR("error: unknown data_relationship_type_t.");
            assert(false);
        }
        break;
    }
    return ( result );
}


/*
Copyright 2023-2025 Andreas Warnke

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
