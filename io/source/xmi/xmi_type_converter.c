/* File: xmi_type_converter.c; Copyright and License: see below */

#include "xmi/xmi_type_converter.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void xmi_type_converter_init ( xmi_type_converter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void xmi_type_converter_destroy( xmi_type_converter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_, data_classifier_type_t c_type )
{
    TRACE_BEGIN();
    const char* result = "";

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_FEATURE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_STATE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            result = "";
        }
        break;

        /*
        case DATA_CLASSIFIER_TYPE_DYN_PARTITION:
        {
            result = "";
        }
        break;
        */

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            result = "";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            result = "";
        }
        break;

        default:
        {
            TSLOG_ERROR_INT( "switch case statement for data_relationship_type_t incomplete", c_type );
            assert( 0 );
            result = "";
        }
        break;
    }

    TRACE_END_ERR( ('\0'==*result) ? -1 : 0 );
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_feature ( xmi_type_converter_t *this_, data_feature_type_t f_type )
{
    TRACE_BEGIN();
    const char* result = "";

    switch ( f_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        {
            result = "";
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            result = "";
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            result = "";
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            result = "";
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            result = "";
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            result = "";
        }
        break;

        /*
        case DATA_FEATURE_TYPE_TESTCASE:
        {
            result = "";
        }
        break;
        */

        default:
        {
            TSLOG_ERROR_INT( "switch case statement for data_relationship_type_t incomplete", f_type );
            assert( 0 );
            result = "";
        }
        break;
    }

    TRACE_END_ERR( ('\0'==*result) ? -1 : 0 );
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_relationship ( xmi_type_converter_t *this_, data_relationship_type_t r_type )
{
    TRACE_BEGIN();
    const char* result = "";

    switch ( r_type )
    {
        case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REFINE:
        {
            result = "";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            result = "";
        }
        break;

        default:
        {
            TSLOG_ERROR_INT( "switch case statement for data_relationship_type_t incomplete", r_type );
            assert( 0 );
            result = "";
        }
        break;
    }

    TRACE_END_ERR( ('\0'==*result) ? -1 : 0 );
    return result;
}


/*
Copyright 2020-2020 Andreas Warnke

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
