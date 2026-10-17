/* File: data_diagramelement_flag.c; Copyright and License: see below */

#include "entity/data_diagramelement_flag.h"

const data_enum_def_t DATA_DIAGRAMELEMENT_FLAG_ENUM_ARRAY [ DATA_DIAGRAMELEMENT_FLAG_COUNT ]
    = {
        { .number = DATA_DIAGRAMELEMENT_FLAG_NONE, .name = "" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_EMPHASIS, .name = "Emphasis" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT, .name = "Gray" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_NONE, .name = "Named Instance" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS, .name = "Named Instance, Emphasis" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT, .name = "Named Instance, Gray" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_NONE, .name = "Anonymous Instance" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS, .name = "Anonymous Instance, Emphasis" },
        { .number = DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT, .name = "Anonymous Instance, Gray" },
    };

const char * data_diagramelement_flag_get_name( data_diagramelement_flag_t this_ )
{
    U8_TRACE_BEGIN();
    const char * tag_names = "";

    switch( (uint_fast32_t) this_ )  /* cast from enum to 32bit integer needed */
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
Copyright 2026-2026 Andreas Warnke

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
