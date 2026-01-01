/* File: data_relationship_type.c; Copyright and License: see below */

#include "entity/data_relationship_type.h"
#include <stddef.h>
#include <assert.h>

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

static const char *const DATA_RELATIONSHIP_TYPE_NAME_ARRAY [DATA_RELATIONSHIP_TYPE_HASH_MOD]
    = {
        /* 0x0 is DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY */
        [ DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Dependency",
        /* 0x1b .. 0x1f */
        [0x1b] = "",
        [ DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Association",
        [ DATA_RELATIONSHIP_TYPE_UML_AGGREGATION % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Aggregation",
        [ DATA_RELATIONSHIP_TYPE_UML_COMPOSITION % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Composition",
        [0x1f] = "",
        /* 0x26 .. 0x29 */
        [ DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Generalization",
        [ DATA_RELATIONSHIP_TYPE_UML_REALIZATION % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Realization",
        [0x28] = "",
        [0x29] = "",
        /* 0x04 .. 0x0c */
        [0x4] = "",
        [ DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Asynchronous Call",
        [ DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Synchronous Call",
        [ DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Return",
        [0x8] = "",
        [0x9] = "",
        [0xa] = "",
        [0xb] = "",
        [0xc] = "",
        /* 0x18 .. 0x1a */
        [0x18] = "",
        [ DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Control Flow",
        [ DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Object Flow",
        /* 0x20 .. 0x25 */
        [0x20] = "",
        [0x21] = "",
        [0x22] = "",
        [ DATA_RELATIONSHIP_TYPE_UML_DEPLOY % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Deployment",
        [ DATA_RELATIONSHIP_TYPE_UML_MANIFEST % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Manifestation",
        [0x25] = "",
        /* 0x01 .. 0x03 */
        [0x1] = "",
        [ DATA_RELATIONSHIP_TYPE_UML_EXTEND % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Extension",
        [ DATA_RELATIONSHIP_TYPE_UML_INCLUDE % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Inclusion",
        /* 0x2a is DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT */
        [ DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Containment",
        /* 0x0d .. 0x17 */
        [ DATA_RELATIONSHIP_TYPE_UML_REFINE % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Refinement",
        [ DATA_RELATIONSHIP_TYPE_UML_TRACE % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Trace",
        [ DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH % DATA_RELATIONSHIP_TYPE_HASH_MOD ] = "Communication Path",
        [0x10] = "",
        [0x11] = "",
        [0x12] = "",
        [0x13] = "",
        [0x14] = "",
        [0x15] = "",
        [0x16] = "",
        [0x17] = "",
    };

const char * data_relationship_type_get_name( data_relationship_type_t this_ )
{
    const char * result
        = ( this_ == DATA_RELATIONSHIP_TYPE_VOID ) ? "" : DATA_RELATIONSHIP_TYPE_NAME_ARRAY[ this_ % DATA_RELATIONSHIP_TYPE_HASH_MOD ];
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
