/* File: json_type_name_map.inl; Copyright and License: see below */

#include "json/json_type_name_map.h"
#include "u8/u8_trace.h"
#include <stdint.h>
#include <assert.h>

void json_type_name_map_init( json_type_name_map_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}

void json_type_name_map_destroy( json_type_name_map_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}

const char * json_type_name_map_get_diagram_tags( const json_type_name_map_t *this_,
                                                  data_diagram_flag_t diagram_flag )
{
    U8_TRACE_BEGIN();
    const char * tag_names = "";

    switch( diagram_flag )
    {
        case DATA_DIAGRAM_FLAG_EMPHASIS:
        {
            tag_names = "Emphasis";
        }
        break;

        case DATA_DIAGRAM_FLAG_GRAY_OUT:
        {
            tag_names = "Gray";
        }
        break;

        default:
        {
            tag_names = "";
        }
        break;
    }

    U8_TRACE_END();
    return tag_names;
}

const char * json_type_name_map_get_diagramelement_tags( const json_type_name_map_t *this_,
                                                         data_diagramelement_flag_t diagramelement_flag )
{
    U8_TRACE_BEGIN();
    const char * tag_names = "";

    switch( (int_fast32_t) diagramelement_flag )
    {
        case DATA_DIAGRAMELEMENT_FLAG_EMPHASIS:
        {
            tag_names = "Emphasis";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT:
        {
            tag_names = "Gray";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE:
        {
            tag_names = "Named Instance";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS:
        {
            tag_names = "Named Instance, Emphasis";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT:
        {
            tag_names = "Named Instance, Gray";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE:
        {
            tag_names = "Anonymous Instance";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS:
        {
            tag_names = "Anonymous Instance, Emphasis";
        }
        break;

        case DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT:
        {
            tag_names = "Anonymous Instance, Gray";
        }
        break;

        default:
        {
            tag_names = "";
        }
        break;
    }

    U8_TRACE_END();
    return tag_names;
}


/*
Copyright 2022-2026 Andreas Warnke

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
