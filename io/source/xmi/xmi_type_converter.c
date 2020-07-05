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

#define XMI_TYPE_CONVERTER_NS_UML "uml:"
#define XMI_TYPE_CONVERTER_NS_SYSML "SysML:"

const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_, data_classifier_type_t c_type )
{
    TRACE_BEGIN();
    const char* result = "";

    switch ( c_type )
    {
        case DATA_CLASSIFIER_TYPE_BLOCK:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Blocks */
            result = XMI_TYPE_CONVERTER_NS_SYSML "Block";
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: ConstraintBlocks */
            result = XMI_TYPE_CONVERTER_NS_SYSML "ConstraintBlock";
        }
        break;

        case DATA_CLASSIFIER_TYPE_FEATURE:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
            result = XMI_TYPE_CONVERTER_NS_SYSML "Requirement";
            /* note: there is no special type in SysML for a feature or composite requirement */
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
            result = XMI_TYPE_CONVERTER_NS_SYSML "Requirement";
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
            result = XMI_TYPE_CONVERTER_NS_UML "Actor";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
            result = XMI_TYPE_CONVERTER_NS_UML "UseCase";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
            result = XMI_TYPE_CONVERTER_NS_UML "Node";
            /* TODO: check type */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "Activity";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_STATE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
            result = XMI_TYPE_CONVERTER_NS_UML "State";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "Activity";
            /* TODO: check type */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
            result = XMI_TYPE_CONVERTER_NS_UML "Node";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Component";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PART:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Component";
            /* TODO: check type */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Interface";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
            result = XMI_TYPE_CONVERTER_NS_UML "Package";
            /* TODO: spec ok but is ignored at import to EA, maybe a packageImport-tag needed */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Class";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Class";
            /* TODO: check type */
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
            result = XMI_TYPE_CONVERTER_NS_UML "Artifact";
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
            result = XMI_TYPE_CONVERTER_NS_UML "Comment";
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "InterruptibleActivityRegion";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "InitialNode";
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            /* result = XMI_TYPE_CONVERTER_NS_UML "FinalNode"; */
            result = XMI_TYPE_CONVERTER_NS_UML "ActivityFinalNode";
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
            /* result = XMI_TYPE_CONVERTER_NS_UML "FinalState"; */
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "ForkNode";
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "JoinNode";
            /* TODO: spec ok but is ignored at import to EA */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "DecisionNode";
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
            /* result = XMI_TYPE_CONVERTER_NS_UML "ConditionalNode"; */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
            result = XMI_TYPE_CONVERTER_NS_UML "Pseudostate";
            /* TODO: PseudostateKind = shallowHistory shall be set */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
            result = XMI_TYPE_CONVERTER_NS_UML "Pseudostate";
            /* TODO: PseudostateKind = deepHistory shall be set */
        }
        break;

        /*
        case DATA_CLASSIFIER_TYPE_DYN_PARTITION:
        {
            / * spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities * /
            result = XMI_TYPE_CONVERTER_NS_UML "ActivityPartition";
        }
        break;
        */

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Reception";
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
            /* result = XMI_TYPE_CONVERTER_NS_UML "AcceptEventAction"; */
            /* TODO: check type */
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
            result = XMI_TYPE_CONVERTER_NS_UML "TimeEvent";
        }
        break;

        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            /* result = XMI_TYPE_CONVERTER_NS_UML "Signal"; */
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
            result = XMI_TYPE_CONVERTER_NS_UML "SignalEvent";
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
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
            result = XMI_TYPE_CONVERTER_NS_UML "Property";
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
            result = XMI_TYPE_CONVERTER_NS_UML "Operation";
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Port";
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Interactions */
            result = XMI_TYPE_CONVERTER_NS_UML "Lifeline";
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Interface";
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Interface";
        }
        break;

        /*
        case DATA_FEATURE_TYPE_TESTCASE:
        {
            / * spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements * /
            result = XMI_TYPE_CONVERTER_NS_SYSML "TestCase";
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

const char* xmi_type_converter_get_xmi_owning_type_of_feature ( xmi_type_converter_t *this_, data_feature_type_t f_type )
{
    TRACE_BEGIN();
    const char* result = "";

    switch ( f_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 9.9.17 */
            result = "ownedAttribute";
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 9.6.2, 11.9.16 */
            result = "ownedOperation";
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 11.8.13.5 */
            result = "ownedPort";
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF TODO lifelines are only loosely coupled with their classifiers */
            result = "ownedBehavior";
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 11.6.2 */
            result = "provided";
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 11.6.2 */
            result = "required";
        }
        break;

        /*
        case DATA_FEATURE_TYPE_TESTCASE:
        {
            / * spec: https://www.omg.org/spec/UML/2.5.1/PDF (v1.6) pkg: Requirements * /
            result = "ownedBehavior";
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
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
            result = XMI_TYPE_CONVERTER_NS_UML "Dependency";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = XMI_TYPE_CONVERTER_NS_UML "Association";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
            result = XMI_TYPE_CONVERTER_NS_UML "AggregationKind";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
            result = XMI_TYPE_CONVERTER_NS_UML "AggregationKind";
            /* TODO: check type */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
            result = XMI_TYPE_CONVERTER_NS_UML "Generalization";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            /* result = XMI_TYPE_CONVERTER_NS_UML "InterfaceRealization"; */
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
            result = XMI_TYPE_CONVERTER_NS_UML "Realization";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Interactions */
            result = XMI_TYPE_CONVERTER_NS_UML "Message";
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
            /* result = XMI_TYPE_CONVERTER_NS_UML "SignalEvent"; */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
            result = XMI_TYPE_CONVERTER_NS_UML "CallEvent";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
            result = XMI_TYPE_CONVERTER_NS_UML "ReplyAction";
            /* TODO: check type */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
            result = XMI_TYPE_CONVERTER_NS_UML "CommunicationPath";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "ControlFlow";
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
            /* result = XMI_TYPE_CONVERTER_NS_UML "SendSignalAction"; */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
            result = XMI_TYPE_CONVERTER_NS_UML "ObjectFlow";
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
            /* result = XMI_TYPE_CONVERTER_NS_UML "SendObjectAction"; */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
            result = XMI_TYPE_CONVERTER_NS_UML "Deployment";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
            result = XMI_TYPE_CONVERTER_NS_UML "Manifestation";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
            result = XMI_TYPE_CONVERTER_NS_UML "Extend";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
            result = XMI_TYPE_CONVERTER_NS_UML "Include";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
            result = XMI_TYPE_CONVERTER_NS_UML "PackageMerge";
            /* TODO: check type */
            /* TODO: spec like but is ignored at import to EA, maybe a packageImport-tag or elementImport-tag is more suitable */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REFINE:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
            result = XMI_TYPE_CONVERTER_NS_SYSML "Refine";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
            result = XMI_TYPE_CONVERTER_NS_SYSML "Trace";
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
