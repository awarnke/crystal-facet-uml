/* File: xmi_type_converter.c; Copyright and License: see below */

#include "xmi/xmi_type_converter.h"
#include "xmi/xmi_element_info_map.h"
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

/* ================================ CLASSIFIER ================================ */

xmi_spec_t xmi_type_converter_get_xmi_spec_of_classifier ( xmi_type_converter_t *this_, data_classifier_type_t c_type )
{
    TRACE_BEGIN();

    const xmi_element_info_t *e_info
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, c_type );
    assert ( e_info != NULL );
    const xmi_spec_t result
        = (*e_info).specification;

    TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t c_type,
                                                            xmi_spec_t spec )
{
    TRACE_BEGIN();

    const xmi_element_info_t *e_info
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, c_type );
    assert ( e_info != NULL );
    const char* result
        = (( (spec & (XMI_SPEC_SYSML|XMI_SPEC_STANDARD)) != 0 )&&( (*e_info).profile_name != NULL ))
        ? (*e_info).profile_name
        : (*e_info).base_name;
    assert ( result != NULL );

    TRACE_END_ERR( ('\0'==*result) ? -1 : 0 );
    return result;
}

int xmi_type_converter_get_xmi_nesting_property_of_classifier ( xmi_type_converter_t *this_,
                                                                data_classifier_type_t parent_type,
                                                                data_classifier_type_t child_type,
                                                                char const * *out_xmi_name )
{
    TRACE_BEGIN();
    assert( out_xmi_name != NULL );
    const char* result = NULL;

    const xmi_element_info_t *parent_info
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, parent_type );
    assert ( parent_info != NULL );
    const xmi_element_info_t *child_info
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, child_type );
    assert ( child_info != NULL );

    if ( xmi_element_info_is_a_package(parent_info) &&  xmi_element_info_is_a_packageable_element(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.4.5.6 */
        result = "packagedElement";
    }
    else if ( xmi_element_info_is_a_comment(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.2 */
        /* any element can own 0..* comments */
        result = "ownedComment";
    }
    else if ( xmi_element_info_is_a_classifier(parent_info) && (child_type==DATA_CLASSIFIER_TYPE_UML_USE_CASE) )
    {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classifier */
        result = "ownedUseCase";
    }
    else if ( xmi_element_info_is_a_node(parent_info) && xmi_element_info_is_a_node(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 19.5.10.5 */
        result = "nestedNode";
    }
    else if ( xmi_element_info_is_a_activity(parent_info) && xmi_element_info_is_a_activity_node(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.1.5 */
        result = "node";
    }
    else if ( xmi_element_info_is_a_behaviored_classifier(parent_info) && xmi_element_info_is_a_behavior(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.1 */
        result = "ownedBehavior";
    }
    else if ( xmi_element_info_is_a_class(parent_info) && xmi_element_info_is_a_classifier(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.3.6 */
        result = "nestedClassifier";
    }
    else if ( xmi_element_info_is_a_artifact(parent_info) && xmi_element_info_is_a_artifact(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter  */
        result = "nestedArtifact";
    }
    else if ( (parent_type==DATA_CLASSIFIER_TYPE_UML_PACKAGE) )
    {
        /* emergengy: at least the outer model should contain the elements */
        result = "packagedElement";
        TSLOG_WARNING_STR("The xmi export cannot place:",(*child_info).base_name);
    }

    *out_xmi_name = (result==NULL) ? "" : result;
    const int result_err = (result==NULL) ? -1 : 0;
    TRACE_END_ERR( result_err );
    return result_err;
}

/* ================================ FEATURE ================================ */

xmi_spec_t xmi_type_converter_get_xmi_spec_of_feature ( xmi_type_converter_t *this_, data_feature_type_t f_type )
{
    TRACE_BEGIN();
    xmi_spec_t result = XMI_SPEC_UML;  /* all currently known features are defined in the uml specification */

    TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_feature ( xmi_type_converter_t *this_,
                                                         data_feature_type_t f_type,
                                                         xmi_spec_t spec )
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

const char* xmi_type_converter_get_xmi_owning_property_of_feature ( xmi_type_converter_t *this_, data_feature_type_t f_type )
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
            /* TODO look out for ownedReception */
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

/* ================================ RELATIONSHIP ================================ */

xmi_spec_t xmi_type_converter_get_xmi_spec_of_relationship ( xmi_type_converter_t *this_, data_relationship_type_t r_type )
{
    TRACE_BEGIN();
    xmi_spec_t result
        = (( r_type == DATA_RELATIONSHIP_TYPE_UML_REFINE )||( r_type == DATA_RELATIONSHIP_TYPE_UML_TRACE ))
        ? XMI_SPEC_STANDARD
        : XMI_SPEC_UML;

    TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_relationship ( xmi_type_converter_t *this_,
                                                              data_relationship_type_t r_type,
                                                              xmi_spec_t spec  )
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
            result
                = ( (spec & XMI_SPEC_STANDARD) == XMI_SPEC_STANDARD )
                ? XMI_TYPE_CONVERTER_NS_STDPROF "Refine"
                : XMI_TYPE_CONVERTER_NS_UML "Abstraction";  /* base class from which the standard profile is derived, SysML 1.4 */
                /*: XMI_TYPE_CONVERTER_NS_UML "Class";*/  /* base class from which the standard profile is derived */
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
            result
                = ( (spec & XMI_SPEC_STANDARD) == XMI_SPEC_STANDARD )
                ? XMI_TYPE_CONVERTER_NS_STDPROF "Trace"
                : XMI_TYPE_CONVERTER_NS_UML "Abstraction";  /* base class from which the standard profile is derived, SysML 1.4 */
                /*: XMI_TYPE_CONVERTER_NS_UML "Class";*/  /* base class from which the standard profile is derived */
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

const char* xmi_type_converter_private_get_xmi_end_property_of_relationship ( xmi_type_converter_t *this_,
                                                                              data_relationship_type_t r_type,
                                                                              bool from_end )
{
    TRACE_BEGIN();
    const char* result = "";

    /* spec: https://www.omg.org/spec/UML/2.5.1/PDF */
    switch ( r_type )
    {
        case DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY:
        {
            /* Type: UML Dependency, Usage, Deployment, Realization, Manifestation, Abstraction */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Dependency.client" : XMI_TYPE_CONVERTER_NS_UML "Dependency.supplier";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION:
        {
            /* Type: UML Association, CommunicationPath */
            /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
            result = XMI_TYPE_CONVERTER_NS_UML "Association.memberEnd";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_AGGREGATION:
        {
            /* Type: UML AggregationKind */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMPOSITION:
        {
            /* Type: UML AggregationKind */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION:
        {
            /* Type: UML Generalization */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Generalization.specific" : XMI_TYPE_CONVERTER_NS_UML "Generalization.general";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REALIZATION:
        {
            /* Type: UML Dependency, Usage, Deployment, Realization, Manifestation, Abstraction */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Dependency.client" : XMI_TYPE_CONVERTER_NS_UML "Dependency.supplier";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL:
        {
            /* Type: UML Message */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL:
        {
            /* Type: UML CallEvent */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL:
        {
            /* Type: UML ReplyAction */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH:
        {
            /* Type: UML Association, CommunicationPath */
            /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
            result = XMI_TYPE_CONVERTER_NS_UML "Association.memberEnd";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW:
        {
            /* Type: UML ControlFlow */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW:
        {
            /* Type: UML ObjectFlow */
            /* TODO: source and target are generic read-only properties - choose the right ones for this type of relationship */
            result = from_end ? "informationSource" : "informationTarget";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_DEPLOY:
        {
            /* Type: UML Dependency, Usage, Deployment, Realization, Manifestation, Abstraction */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Dependency.client" : XMI_TYPE_CONVERTER_NS_UML "Dependency.supplier";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_MANIFEST:
        {
            /* Type: UML Dependency, Usage, Deployment, Realization, Manifestation, Abstraction */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Dependency.client" : XMI_TYPE_CONVERTER_NS_UML "Dependency.supplier";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_EXTEND:
        {
            /* Type: UML Extend */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Extend.extension" : XMI_TYPE_CONVERTER_NS_UML "Extend.extendedCase";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_INCLUDE:
        {
            /* Type: UML Include */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Include.includingCase" : XMI_TYPE_CONVERTER_NS_UML "Include.addition";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT:
        {
            /* Type: UML PackageMerge */
            /* TODO: PackageMerge may be the wrong type - choose the properties of the right type of relationship */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "PackageMerge.receivingPackage" : XMI_TYPE_CONVERTER_NS_UML "PackageMerge.mergedPackage";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_REFINE:
        {
            /* Type: UML Dependency, Usage, Deployment, Realization, Manifestation, Abstraction */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Dependency.client" : XMI_TYPE_CONVERTER_NS_UML "Dependency.supplier";
        }
        break;

        case DATA_RELATIONSHIP_TYPE_UML_TRACE:
        {
            /* Type: UML Dependency, Usage, Deployment, Realization, Manifestation, Abstraction */
            result = from_end ? XMI_TYPE_CONVERTER_NS_UML "Dependency.client" : XMI_TYPE_CONVERTER_NS_UML "Dependency.supplier";
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
