/* File: data_classifier_type.c; Copyright and License: see below */

#include "entity/data_classifier_type.h"
#include <stddef.h>
#include <assert.h>

const data_classifier_type_t DATA_CLASSIFIER_TYPE_ARRAY [DATA_CLASSIFIER_TYPE_COUNT]
    = {
        DATA_CLASSIFIER_TYPE_BLOCK,
        DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK,
        /* DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE, */
        DATA_CLASSIFIER_TYPE_REQUIREMENT,
        DATA_CLASSIFIER_TYPE_ACTOR,
        DATA_CLASSIFIER_TYPE_USE_CASE,
        DATA_CLASSIFIER_TYPE_SUBSYSTEM,
        DATA_CLASSIFIER_TYPE_ACTIVITY,
        DATA_CLASSIFIER_TYPE_STATE,
        DATA_CLASSIFIER_TYPE_INTERACTION_USE,
        DATA_CLASSIFIER_TYPE_NODE,
        DATA_CLASSIFIER_TYPE_COMPONENT,
        DATA_CLASSIFIER_TYPE_PART,
        DATA_CLASSIFIER_TYPE_INTERFACE,
        DATA_CLASSIFIER_TYPE_PACKAGE,
        DATA_CLASSIFIER_TYPE_CLASS,
        DATA_CLASSIFIER_TYPE_OBJECT,
        DATA_CLASSIFIER_TYPE_ARTIFACT,
        DATA_CLASSIFIER_TYPE_COMMENT,
        DATA_CLASSIFIER_TYPE_IMAGE,
        DATA_CLASSIFIER_TYPE_STEREOTYPE,
        DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION,
        DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE,
        DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE,
        DATA_CLASSIFIER_TYPE_DYN_FORK_NODE,
        DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE,
        DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
        DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY,
        DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY,
        DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT,
        DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT,
        DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL,
        DATA_CLASSIFIER_TYPE_INTERACTION
    };

static const char *const DATA_CLASSIFIER_TYPE_NAME_ARRAY [DATA_CLASSIFIER_TYPE_HASH_MOD]
    = {
        /* 0x0 .. 0x9 */
        [0xa] = "",
        [0xb] = "",
        [0xc] = "",
        [0xd] = "",
        [0xe] = "",
        [ DATA_CLASSIFIER_TYPE_BLOCK % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Block",
        /* 0x10 .. 0x14 */
        [ DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Constraint Block",
        [0x16] = "",
        [0x17] = "",
        [ DATA_CLASSIFIER_TYPE_REQUIREMENT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Requirement",
        [ DATA_CLASSIFIER_TYPE_ACTOR % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Actor",
        [ DATA_CLASSIFIER_TYPE_USE_CASE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Use Case",
        [ DATA_CLASSIFIER_TYPE_SUBSYSTEM % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Subsystem",
        [0x1c] = "",
        [0x1d] = "",
        [0x1e] = "",
        [0x1f] = "",
        [0x20] = "",
        [0x21] = "",
        [0x22] = "",
        [ DATA_CLASSIFIER_TYPE_ACTIVITY % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Activity",
        [ DATA_CLASSIFIER_TYPE_STATE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "State",
        [ DATA_CLASSIFIER_TYPE_INTERACTION_USE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Interaction Use",
        [0x26] = "",
        [0x27] = "",
        /* 0x28 is DATA_CLASSIFIER_TYPE_INTERACTION */
        [0x29] = "",
        [0x2a] = "",
        [0x2b] = "",
        [0x2c] = "",
        [ DATA_CLASSIFIER_TYPE_NODE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Node",
        [ DATA_CLASSIFIER_TYPE_COMPONENT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Component",
        [ DATA_CLASSIFIER_TYPE_PART % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Part",
        [ DATA_CLASSIFIER_TYPE_INTERFACE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Interface",
        [ DATA_CLASSIFIER_TYPE_PACKAGE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Package",
        [ DATA_CLASSIFIER_TYPE_CLASS % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Class",
        [ DATA_CLASSIFIER_TYPE_OBJECT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Object",
        [ DATA_CLASSIFIER_TYPE_ARTIFACT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Artifact",
        [0x35] = "",
        [0x36] = "",
        [ DATA_CLASSIFIER_TYPE_COMMENT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Comment",
        [0x38] = "",
        [0x39] = "",
        [0x3a] = "",
        [0x3b] = "",
        [0x3c] = "",
        [0x3d] = "",
        [0x3e] = "",
        [ DATA_CLASSIFIER_TYPE_IMAGE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Image",
        [ DATA_CLASSIFIER_TYPE_STEREOTYPE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Stereotype",
        [0x41] = "",
        [0x42] = "",
        [0x43] = "",
        [0x44] = "",
        [0x45] = "",
        [0x46] = "",
        [0x47] = "",
        [0x48] = "",
        [0x49] = "",
        [0x4a] = "",
        /* 0x0 .. 0x9 */
        [ DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Interruptable Region",
        [ DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Initial Node",
        [ DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Final Node",
        [0x3] = "",
        [ DATA_CLASSIFIER_TYPE_DYN_FORK_NODE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Fork",
        [ DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Join",
        [0x6] = "",
        [ DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Decision",
        [ DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Shallow History",
        [ DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Deep History",
        /* 0x10 .. 0x14 */
        [0x10] = "",
        [ DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Accept Event",
        [ DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Time Event",
        [0x13] = "",
        [ DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Send Signal",
        /* 0x28 is DATA_CLASSIFIER_TYPE_INTERACTION */
        [ DATA_CLASSIFIER_TYPE_INTERACTION % DATA_CLASSIFIER_TYPE_HASH_MOD ] = "Interaction",
    };

const char * data_classifier_type_get_name( data_classifier_type_t this_ )
{
    const char * result
        = ( this_ == DATA_CLASSIFIER_TYPE_VOID ) ? "" : DATA_CLASSIFIER_TYPE_NAME_ARRAY[ this_ % DATA_CLASSIFIER_TYPE_HASH_MOD ];
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
