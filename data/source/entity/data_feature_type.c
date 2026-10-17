/* File: data_feature_type.c; Copyright and License: see below */

#include "entity/data_feature_type.h"
#include <stddef.h>
#include <assert.h>

const data_enum_def_t DATA_FEATURE_TYPE_ENUM_ARRAY [ DATA_FEATURE_TYPE_COUNT ]
    = {
        { .number = DATA_FEATURE_TYPE_PROPERTY, .name = "Property" },
        { .number = DATA_FEATURE_TYPE_OPERATION, .name = "Operation" },
        { .number = DATA_FEATURE_TYPE_PORT, .name = "Port" },
        { .number = DATA_FEATURE_TYPE_LIFELINE, .name = "Lifeline" },
        { .number = DATA_FEATURE_TYPE_PROVIDED_INTERFACE, .name = "Provided Interface" },
        { .number = DATA_FEATURE_TYPE_REQUIRED_INTERFACE, .name = "Required Interface" },
        { .number = DATA_FEATURE_TYPE_IN_PORT_PIN, .name = "Input Port" },
        { .number = DATA_FEATURE_TYPE_OUT_PORT_PIN, .name = "Output Port" },
        { .number = DATA_FEATURE_TYPE_ENTRY, .name = "Entry" },
        { .number = DATA_FEATURE_TYPE_EXIT, .name = "Exit" },
        { .number = DATA_FEATURE_TYPE_TAGGED_VALUE, .name = "Tagged Value" },
    };

static const char *const DATA_FEATURE_TYPE_NAME_ARRAY [DATA_FEATURE_TYPE_HASH_MOD]
    = {
        [ DATA_FEATURE_TYPE_PROPERTY ] = "Property",
        [ DATA_FEATURE_TYPE_OPERATION ] = "Operation",
        [ DATA_FEATURE_TYPE_PORT ] = "Port",
        [ DATA_FEATURE_TYPE_LIFELINE ] = "Lifeline",
        [ DATA_FEATURE_TYPE_PROVIDED_INTERFACE ] = "Provided Interface",
        [ DATA_FEATURE_TYPE_REQUIRED_INTERFACE ] = "Required Interface",
        [ DATA_FEATURE_TYPE_IN_PORT_PIN ] = "Input Port",
        [ DATA_FEATURE_TYPE_OUT_PORT_PIN ] = "Output Port",
        [ DATA_FEATURE_TYPE_ENTRY ] = "Entry",
        [ DATA_FEATURE_TYPE_EXIT ] = "Exit",
        [ DATA_FEATURE_TYPE_TAGGED_VALUE ] = "Tagged Value",
        [11] = "",
        [12] = "",
        [13] = "",
        [14] = "",
        [15] = "",
    };

const char * data_feature_type_get_name( data_feature_type_t this_ )
{
    const char * result
        = ( this_ == DATA_FEATURE_TYPE_VOID ) ? "" : DATA_FEATURE_TYPE_NAME_ARRAY[ this_ % DATA_FEATURE_TYPE_HASH_MOD ];
    assert( result != NULL );
    return ( result == NULL ) ? "" : result;
}


/*
Copyright 2021-2026 Andreas Warnke

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
