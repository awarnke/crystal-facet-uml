/* File: data_relationship_type.c; Copyright and License: see below */

#include "data_relationship_type.h"

const data_relationship_type_t DATA_RELATIONSHIP_TYPE_ARRAY [DATA_RELATIONSHIP_TYPE_COUNT]
    = {
        DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
        DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
        DATA_RELATIONSHIP_TYPE_UML_AGGREGATION,
        DATA_RELATIONSHIP_TYPE_UML_COMPOSITION,
        DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION,
        DATA_RELATIONSHIP_TYPE_UML_REALIZATION,
        DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL,
        DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL,
        DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL,
        DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH,
        DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW,
        DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW,
        DATA_RELATIONSHIP_TYPE_UML_DEPLOY,
        DATA_RELATIONSHIP_TYPE_UML_MANIFEST,
        DATA_RELATIONSHIP_TYPE_UML_EXTEND,
        DATA_RELATIONSHIP_TYPE_UML_INCLUDE,
        DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
        DATA_RELATIONSHIP_TYPE_UML_REFINE,
        DATA_RELATIONSHIP_TYPE_UML_TRACE
    };


/*
Copyright 2021-2022 Andreas Warnke

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
