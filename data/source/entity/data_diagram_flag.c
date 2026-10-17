/* File: data_diagram_flag.c; Copyright and License: see below */

#include "entity/data_diagram_flag.h"

const data_enum_def_t DATA_DIAGRAM_FLAG_ENUM_ARRAY [ DATA_DIAGRAM_FLAG_COUNT ]
    = {
        { .number = DATA_DIAGRAM_FLAG_NONE, .name = "" },
        { .number = DATA_DIAGRAM_FLAG_EMPHASIS, .name = "Emphasis" },
        { .number = DATA_DIAGRAM_FLAG_GRAY_OUT, .name = "Gray" },
    };

const char * data_diagram_flag_get_name( data_diagram_flag_t this_ )
{
    U8_TRACE_BEGIN();
    const char * tag_names = "";

    switch( this_ )
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
