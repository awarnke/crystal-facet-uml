/* File: data_feature_type.c; Copyright and License: see below */

#include "entity/data_feature_type.h"

const data_feature_type_t DATA_FEATURE_TYPE_ARRAY [DATA_FEATURE_TYPE_COUNT]
    = {
        DATA_FEATURE_TYPE_PROPERTY,
        DATA_FEATURE_TYPE_OPERATION,
        DATA_FEATURE_TYPE_PORT,
        DATA_FEATURE_TYPE_LIFELINE,
        DATA_FEATURE_TYPE_PROVIDED_INTERFACE,
        DATA_FEATURE_TYPE_REQUIRED_INTERFACE,
        DATA_FEATURE_TYPE_IN_PORT_PIN,
        DATA_FEATURE_TYPE_OUT_PORT_PIN,
        DATA_FEATURE_TYPE_ENTRY,
        DATA_FEATURE_TYPE_EXIT,
        DATA_FEATURE_TYPE_TAGGED_VALUE
    };


/*
Copyright 2021-2025 Andreas Warnke

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
