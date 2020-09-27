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
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, c_type, false /*this parameter does not matter for this use case*/ );
    assert ( e_info != NULL );
    const xmi_spec_t result
        = (*e_info).specification;

    TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t parent_type,
                                                            data_classifier_type_t c_type,
                                                            xmi_spec_t spec )
{
    TRACE_BEGIN();

    const xmi_element_info_t *e_info
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, c_type, (parent_type==DATA_CLASSIFIER_TYPE_UML_STATE) );
    assert ( e_info != NULL );
    const char* result
        = (( (spec & (XMI_SPEC_SYSML|XMI_SPEC_STANDARD)) != 0 )&&( (*e_info).profile_name != NULL ))
        ? (*e_info).profile_name
        : (*e_info).base_name;
    assert ( result != NULL );

    TRACE_END();
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
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, parent_type, false /*guess*/ );
    assert ( parent_info != NULL );
    const xmi_element_info_t *child_info
        = xmi_element_info_map_static_get_classifier ( &xmi_element_info_map_standard, child_type, (parent_type==DATA_CLASSIFIER_TYPE_UML_STATE) );
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
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 19.5.1.6 */
        result = "nestedArtifact";
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
            result = "Property";
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
            result = "Operation";
        }
        break;

        case DATA_FEATURE_TYPE_PORT:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
            result = "Port";
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Interactions */
            result = "Lifeline";
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            result = "Interface";
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
            result = "Interface";
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
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.3.6 */
            result = "ownedAttribute";
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.3.6 */
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

    const xmi_element_info_t *e_info
        = xmi_element_info_map_static_get_relationship ( &xmi_element_info_map_standard, r_type );
    assert ( e_info != NULL );
    const xmi_spec_t result
        = (*e_info).specification;

    TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_relationship ( xmi_type_converter_t *this_,
                                                              data_relationship_type_t r_type,
                                                              xmi_spec_t spec  )
{
    TRACE_BEGIN();

    const xmi_element_info_t *e_info
        = xmi_element_info_map_static_get_relationship ( &xmi_element_info_map_standard, r_type );
    assert ( e_info != NULL );
    const char* result
        = (( (spec & (XMI_SPEC_SYSML|XMI_SPEC_STANDARD)) != 0 )&&( (*e_info).profile_name != NULL ))
        ? (*e_info).profile_name
        : (*e_info).base_name;
    assert ( result != NULL );

    TRACE_END();
    return result;
}

const char* xmi_type_converter_private_get_xmi_end_property_of_relationship ( xmi_type_converter_t *this_,
                                                                              data_relationship_type_t r_type,
                                                                              bool from_end )
{
    TRACE_BEGIN();

    const xmi_element_info_t *e_info
        = xmi_element_info_map_static_get_relationship ( &xmi_element_info_map_standard, r_type );
    assert ( e_info != NULL );
    const char* result
        = ( from_end )
        ? (*e_info).property_from
        : (*e_info).property_to;
    assert ( result != NULL );

    TRACE_END();
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
