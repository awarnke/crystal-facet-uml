/* File: xmi_type_converter.c; Copyright and License: see below */

#include "xmi/xmi_type_converter.h"
#include "xmi/xmi_element_info_map.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

void xmi_type_converter_init ( xmi_type_converter_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */

    U8_TRACE_END();
}

void xmi_type_converter_destroy( xmi_type_converter_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

/* ================================ CLASSIFIER ================================ */

xmi_spec_t xmi_type_converter_get_xmi_spec_of_classifier ( xmi_type_converter_t *this_, data_classifier_type_t c_type )
{
    U8_TRACE_BEGIN();

    const xmi_element_info_t *classifier_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                              DATA_CLASSIFIER_TYPE_PACKAGE, /*this parameter does not matter for this use case*/
                                                              c_type,
                                                              &classifier_info
                                                            );
    if ( map_err != 0 )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", c_type );
    }
    assert ( classifier_info != NULL );

    const xmi_spec_t result
        = (*classifier_info).specification;

    U8_TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t parent_type,
                                                            data_classifier_type_t c_type,
                                                            xmi_spec_t spec )
{
    U8_TRACE_BEGIN();

    const xmi_element_info_t *classifier_info = NULL;
    const u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                                    parent_type,
                                                                    c_type,
                                                                    &classifier_info
                                                                  );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", c_type );
    }
    assert ( classifier_info != NULL );

    const char* result
        = (( (spec & (XMI_SPEC_SYSML|XMI_SPEC_STANDARD)) != 0 )&&( (*classifier_info).profile_name != NULL ))
        ? (*classifier_info).profile_name
        : (*classifier_info).base_name;
    assert ( result != NULL );

    U8_TRACE_END();
    return result;
}

u8_error_t xmi_type_converter_get_xmi_nesting_property_of_classifier ( xmi_type_converter_t *this_,
                                                                       data_classifier_type_t parent_type,
                                                                       data_classifier_type_t child_type,
                                                                       char const * *out_xmi_name )
{
    U8_TRACE_BEGIN();
    assert( out_xmi_name != NULL );
    const char* result = NULL;

    const xmi_element_info_t *parent_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                              DATA_CLASSIFIER_TYPE_PACKAGE, /*TODO: fix guess*/
                                                              parent_type,
                                                              &parent_info
                                                            );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", parent_type );
    }
    assert ( parent_info != NULL );

    const bool p_is_state = ( parent_type == DATA_CLASSIFIER_TYPE_STATE );
    const bool p_is_activity = ( parent_type == DATA_CLASSIFIER_TYPE_ACTIVITY );
    const bool p_is_interruptable_region = ( parent_type == DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION );
    const bool p_is_interaction = ( parent_type == DATA_CLASSIFIER_TYPE_INTERACTION );
    const xmi_element_info_t *child_info = NULL;
    map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                   parent_type,
                                                   child_type,
                                                   &child_info
                                                 );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", child_type );
    }
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
    else if ( xmi_element_info_is_a_classifier(parent_info) && (child_type==DATA_CLASSIFIER_TYPE_USE_CASE) )
    {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classifier */
        result = "ownedUseCase";
    }
    else if ( xmi_element_info_is_a_node(parent_info) && xmi_element_info_is_a_node(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 19.5.10.5 */
        result = "nestedNode";
    }
    else if ( p_is_activity && xmi_element_info_is_a_activity_node(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.1.5 */
        result = "node";
    }
    else if ( p_is_activity && xmi_element_info_is_a_activity_group(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.1.5 */
        result = "group";
    }
    else if ( p_is_interruptable_region && xmi_element_info_is_a_activity_node(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.19.4 */
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
    else if ( p_is_state && xmi_element_info_is_a_vertex(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.8 (note: states have an implicit Region) */
        result = "subvertex";
    }
    else if ( p_is_interaction && xmi_element_info_is_a_interaction_fragment(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.11.4 */
        result = "fragment";
    }

    *out_xmi_name = (result==NULL) ? "" : result;
    const u8_error_t result_err = (result==NULL) ? U8_ERROR_NOT_FOUND : U8_ERROR_NONE;
    U8_TRACE_END_ERR( result_err );
    return result_err;
}

u8_error_t xmi_type_converter_get_xmi_owning_property_of_feature ( xmi_type_converter_t *this_,
                                                                   data_classifier_type_t parent_type,
                                                                   data_feature_type_t feature_type,
                                                                   char const * *out_xmi_name )
{
    U8_TRACE_BEGIN();
    assert( out_xmi_name != NULL );
    const char* result = NULL;
    u8_error_t result_err = U8_ERROR_NOT_FOUND;

    const xmi_element_info_t *parent_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                              DATA_CLASSIFIER_TYPE_PACKAGE, /*TODO: fix guess*/
                                                              parent_type,
                                                              &parent_info
                                                            );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", parent_type );
    }
    assert ( parent_info != NULL );

    switch ( feature_type )
    {
        case DATA_FEATURE_TYPE_PROPERTY:
        {
            const bool p_is_interface = ( parent_type == DATA_CLASSIFIER_TYPE_INTERFACE );
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.3.6 */
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.5.4 */
            result = "ownedAttribute";
            result_err = ( xmi_element_info_is_a_class(parent_info) || p_is_interface ) ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
        }
        break;

        case DATA_FEATURE_TYPE_OPERATION:
        {
            const bool p_is_interface = ( parent_type == DATA_CLASSIFIER_TYPE_INTERFACE );
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.3.6 */
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.5.4 */
            result = "ownedOperation";
            result_err = ( xmi_element_info_is_a_class(parent_info) || p_is_interface ) ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
        }
        break;

        case DATA_FEATURE_TYPE_PORT:  /* or */
        case DATA_FEATURE_TYPE_IN_PORT_PIN:  /* or */
        case DATA_FEATURE_TYPE_OUT_PORT_PIN:
        {
            const bool is_behavioral_parent = data_classifier_type_is_behavioral( parent_type );
            if ( is_behavioral_parent )
            {
                const bool p_is_activity_or_group
                    = ( parent_type == DATA_CLASSIFIER_TYPE_ACTIVITY )||( parent_type == DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION );
                /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.1.5 */
                result = "node";
                result_err = p_is_activity_or_group ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
            }
            else
            {
                /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 11.8.13.5 */
                result = "ownedPort";
                result_err = xmi_element_info_is_a_encapsulated_classifier(parent_info) ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
                /*TODO ownedPort is a derived value*/
            }
        }
        break;

        case DATA_FEATURE_TYPE_LIFELINE:
        {
            const bool p_is_interaction = ( parent_type == DATA_CLASSIFIER_TYPE_INTERACTION );
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF 17.3.2 */
            result = "lifeline";
            result_err = p_is_interaction ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND; /* note, DATA_CLASSIFIER_TYPE_INTERACTION is a fake type - only here, lifelines are valid. */
        }
        break;

        case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:
        {
            const bool p_is_component
                = ( parent_type == DATA_CLASSIFIER_TYPE_COMPONENT )
                  ||( parent_type == DATA_CLASSIFIER_TYPE_PART );
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 11.6.2 */
            result = "provided";
            result_err = p_is_component ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
            /*TODO provided is a derived value*/
        }
        break;

        case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
        {
            const bool p_is_component
                = ( parent_type == DATA_CLASSIFIER_TYPE_COMPONENT )
                  ||( parent_type == DATA_CLASSIFIER_TYPE_PART );
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF ch 11.6.2 */
            result = "required";
            result_err = p_is_component ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
            /*TODO required is a derived value*/
        }
        break;

        case DATA_FEATURE_TYPE_ENTRY:  /* or */
        case DATA_FEATURE_TYPE_EXIT:
        {
            const bool p_is_state = ( parent_type == DATA_CLASSIFIER_TYPE_STATE );
            /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.8 (note: states have an implicit Region) */
            result = "subvertex";
            result_err = ( p_is_state ) ? U8_ERROR_NONE : U8_ERROR_NOT_FOUND;
        }
        break;

        case DATA_FEATURE_TYPE_TAGGED_VALUE:
        {
            /* tagges values belong to the profile section, not to the parent classifier */
            result = NULL;
            result_err = U8_ERROR_PARAM_OUT_OF_RANGE;
        }
        break;

        default:
        {
            U8_LOG_ERROR_INT( "switch case statement for data_relationship_type_t incomplete", feature_type );
            /* this is a possible error case that can happen when a database created with a newer version of the program is opened with this version */
            result = NULL;
        }
        break;
    }

    *out_xmi_name = (result==NULL) ? "" : result;
    U8_TRACE_END_ERR( result_err );
    return result_err;
}

u8_error_t xmi_type_converter_get_xmi_nesting_property_of_relationship ( xmi_type_converter_t *this_,
                                                                         data_classifier_type_t hosting_type,
                                                                         data_relationship_type_t child_type,
                                                                         char const * *out_xmi_name )
{
    U8_TRACE_BEGIN();
    assert( out_xmi_name != NULL );
    const char* result = NULL;

    const xmi_element_info_t *host_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                              DATA_CLASSIFIER_TYPE_PACKAGE, /*TODO: fix guess*/
                                                              hosting_type,
                                                              &host_info
                                                            );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", hosting_type );
    }
    assert ( host_info != NULL );

    const bool host_is_activity = ( hosting_type == DATA_CLASSIFIER_TYPE_ACTIVITY );
    /*const bool host_is_interface = ( hosting_type == DATA_CLASSIFIER_TYPE_INTERFACE );*/
    const bool host_is_implicit_region = ( hosting_type == DATA_CLASSIFIER_TYPE_STATE );
    const bool host_is_usecase = ( hosting_type == DATA_CLASSIFIER_TYPE_USE_CASE );
    const bool host_is_state = ( hosting_type == DATA_CLASSIFIER_TYPE_STATE );
    const bool host_is_interaction = ( hosting_type == DATA_CLASSIFIER_TYPE_INTERACTION );
    const bool host_is_comment = ( hosting_type == DATA_CLASSIFIER_TYPE_COMMENT );
    const xmi_element_info_t *child_info = NULL;
    map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                     host_is_state,
                                                     child_type,
                                                     &child_info
                                                   );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_relationship could not map unknown type", child_type );
    }
    assert ( child_info != NULL );

    const bool c_is_generalization = ( child_type == DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION );

    if ( xmi_element_info_is_a_package(host_info) && xmi_element_info_is_a_packageable_element(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.4.5.6 */
        result = "packagedElement";
    }
    else if ( xmi_element_info_is_a_classifier(host_info) && c_is_generalization )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.9.4 */
        result = "generalization";
    }
    else if ( host_is_activity && xmi_element_info_is_a_activity_edge(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.1.5 */
        result = "edge";
    }
    /*
    else if ( host_is_interface && xmi_element_info_is_a_reception(child_info) )
    {
        / * spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.5.4 * /
        result = "ownedReception";
    }
    */
    /*
    else if ( xmi_element_info_is_a_class(host_info) && xmi_element_info_is_a_reception(child_info) )
    {
        / * spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.3.6 * /
        result = "ownedReception";
    }
    */
    else if ( host_is_implicit_region && xmi_element_info_is_a_transition(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.8.4 */
        result = "transition";
    }
    else if ( host_is_usecase && (child_type==DATA_RELATIONSHIP_TYPE_UML_EXTEND) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 18.2.5.4 */
        result = "extend";
    }
    else if ( host_is_usecase && (child_type==DATA_RELATIONSHIP_TYPE_UML_INCLUDE) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 18.2.5.4 */
        result = "include";
    }
    else if ( host_is_interaction && xmi_element_info_is_a_message(child_info) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.4 */
        result = "message";
    }
    else if ( host_is_comment && (child_type==DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY) )
    {
        /* spec: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.2 */
        result = "annotatedElement";
    }

    *out_xmi_name = (result==NULL) ? "" : result;
    const u8_error_t result_err = (result==NULL) ? U8_ERROR_NOT_FOUND : U8_ERROR_NONE;
    U8_TRACE_END_ERR( result_err );
    return result_err;
}

/* ================================ FEATURE ================================ */

xmi_spec_t xmi_type_converter_get_xmi_spec_of_feature ( xmi_type_converter_t *this_, data_feature_type_t feature_type )
{
    U8_TRACE_BEGIN();
    xmi_spec_t result = XMI_SPEC_UML;  /* all currently known features are defined in the uml specification */

    U8_TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_feature ( xmi_type_converter_t *this_,
                                                         data_classifier_type_t parent_type,
                                                         data_feature_type_t feature_type,
                                                         xmi_spec_t spec )
{
    U8_TRACE_BEGIN();

    const xmi_element_info_t *feature_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                           parent_type,
                                                           feature_type,
                                                           &feature_info
                                                         );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_feature could not map unknown type", feature_type );
    }
    assert ( feature_info != NULL );

    const char* result
        = (( (spec & (XMI_SPEC_SYSML|XMI_SPEC_STANDARD|XMI_SPEC_MOF)) != 0 )&&( (*feature_info).profile_name != NULL ))
        ? (*feature_info).profile_name
        : (*feature_info).base_name;
    assert ( result != NULL );

    U8_TRACE_END();
    return result;
}

/* ================================ RELATIONSHIP ================================ */

xmi_spec_t xmi_type_converter_get_xmi_spec_of_relationship ( xmi_type_converter_t *this_, data_relationship_type_t r_type )
{
    U8_TRACE_BEGIN();

    const xmi_element_info_t *rel_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                false, /*this parameter does not matter for this use case*/
                                                                r_type,
                                                                &rel_info
                                                              );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_relationship could not map unknown type", r_type );
    }
    assert ( rel_info != NULL );

    const xmi_spec_t result
        = (*rel_info).specification;

    U8_TRACE_END();
    return result;
}

const char* xmi_type_converter_get_xmi_type_of_relationship ( xmi_type_converter_t *this_,
                                                              data_classifier_type_t hosting_type,
                                                              data_relationship_type_t r_type,
                                                              xmi_spec_t spec  )
{
    U8_TRACE_BEGIN();

    const bool host_is_state = ( hosting_type == DATA_CLASSIFIER_TYPE_STATE );
    const xmi_element_info_t *rel_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                host_is_state,
                                                                r_type,
                                                                &rel_info
                                                              );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_relationship could not map unknown type", r_type );
    }
    assert ( rel_info != NULL );

    const char* result
        = (( (spec & (XMI_SPEC_SYSML|XMI_SPEC_STANDARD)) != 0 )&&( (*rel_info).profile_name != NULL ))
        ? (*rel_info).profile_name
        : (*rel_info).base_name;
    assert ( result != NULL );

    U8_TRACE_END();
    return result;
}

u8_error_t xmi_type_converter_private_get_xmi_end_property_of_relationship ( xmi_type_converter_t *this_,
                                                                             data_classifier_type_t hosting_type,
                                                                             data_relationship_type_t rel_type,
                                                                             bool from_end,
                                                                             data_classifier_type_t end_classifier_type,
                                                                             data_feature_type_t end_feature_type,
                                                                             char const * *out_xmi_name )
{
    U8_TRACE_BEGIN();
    assert( out_xmi_name != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const bool host_is_state = ( hosting_type == DATA_CLASSIFIER_TYPE_STATE );
    const xmi_element_info_t *rel_info = NULL;
    u8_error_t map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                host_is_state,
                                                                rel_type,
                                                                &rel_info
                                                              );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_relationship could not map unknown type", rel_type );
    }
    assert ( rel_info != NULL );

    const char* result
        = ( from_end )
        ? (*rel_info).property_from
        : (*rel_info).property_to;
    assert ( result != NULL );

    *out_xmi_name = ( result == NULL ) ? "" : result;
    err = ( result == NULL ) ? U8_ERROR_NOT_FOUND : U8_ERROR_NONE;

    const xmi_element_info_t *classifier_info = NULL;
    map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                   hosting_type,
                                                   end_classifier_type,
                                                   &classifier_info
                                                 );
    if ( map_err != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_INT("xmi_element_info_map_get_classifier could not map unknown type", end_classifier_type );
    }
    assert ( classifier_info != NULL );

    if ( end_feature_type == DATA_FEATURE_TYPE_VOID )
    {
        /* relationship end is at a classifier */

        /* DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW and DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW */
        if ( xmi_element_info_is_a_transition( rel_info ) && xmi_element_info_is_a_vertex( classifier_info ) )
        {
            /* valid relationship according to uml 2.5.1 spec, chapter 14.5.11 */
        }
        else if ( xmi_element_info_is_a_activity_edge( rel_info ) && xmi_element_info_is_a_activity_node( classifier_info ) )
        {
            /* valid relationship according to uml 2.5.1 spec, chapter 15.7.2 */
        }
        //else if ( xmi_element_info_is_a_activity_edge( rel_info ) && ( end_classifier_type == DATA_CLASSIFIER_TYPE_ACTIVITY ))
        //{
            /* valid relationship according to uml 2.5.1 spec, chapter 16.14.55 */
            // this is duplicate for XMI_ELEMENT_INFO_MAP_INDEX_STRUCTURED_ACTIVITY_NODE
            // this is not true for XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY
            // see https://issues.omg.org/issues/lists/uml2-rtf#issue-47324
        //}

        /* DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, */
        /* DATA_RELATIONSHIP_TYPE_UML_DEPLOY, DATA_RELATIONSHIP_TYPE_UML_REFINE,  DATA_RELATIONSHIP_TYPE_UML_TRACE */
        else if ( xmi_element_info_is_a_dependency( rel_info ) && xmi_element_info_is_a_named_element( classifier_info ) )
        {
            /* a dependency can connect any named elements according to uml 2.5.1 spec, chapter 7.8.4  */
        }

        /* DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, */
        /* DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT */
        else if ( xmi_element_info_is_a_association( rel_info ) && xmi_element_info_is_a_class( classifier_info ) )
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a property can connect a class according to uml 2.5.1 spec, chapter 9.9.17  */
        }
        else if ( xmi_element_info_is_a_association( rel_info ) && ( end_classifier_type == DATA_CLASSIFIER_TYPE_INTERFACE ))
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a property can connect an interface according to uml 2.5.1 spec, chapter 9.9.17  */
        }
        else if ( xmi_element_info_is_a_association( rel_info ) && xmi_element_info_is_a_classifier( classifier_info ) )
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a property is a typedelelemnt and can therefore connect a classifier(type) according to uml 2.5.1 spec, chapter 7.8.22  */
        }

        /* DATA_RELATIONSHIP_TYPE_UML_EXTEND and DATA_RELATIONSHIP_TYPE_UML_INCLUDE */
        else if (( rel_type == DATA_RELATIONSHIP_TYPE_UML_EXTEND ) && ( end_classifier_type == DATA_CLASSIFIER_TYPE_USE_CASE ))
        {
            /* an extend directed relationship can connect to a use case according to uml 2.5.1 spec, chapter 18.2.2  */
        }
        else if (( rel_type == DATA_RELATIONSHIP_TYPE_UML_INCLUDE ) && ( end_classifier_type == DATA_CLASSIFIER_TYPE_USE_CASE ))
        {
            /* an include directed relationship can connect to a use case according to uml 2.5.1 spec, chapter 18.2.4  */
        }

        /* DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION */
        else if (( rel_type == DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION ) && xmi_element_info_is_a_classifier( classifier_info ) )
        {
            /* a generalization can connect any classifiers according to uml 2.5.1 spec, chapter 9.9.7  */
        }

        else
        {
            /* no valid end type for given relationship type */
            err = U8_ERROR_NOT_FOUND;
        }
    }
    else
    {
        /* relationship end is at feature */
        const xmi_element_info_t *feature_info = NULL;
        map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                    end_classifier_type,
                                                    end_feature_type,
                                                    &feature_info
                                                  );
        if ( map_err != U8_ERROR_NONE )
        {
            U8_LOG_WARNING_INT("xmi_element_info_map_get_feature could not map unknown type", end_feature_type );
        }
        assert ( feature_info != NULL );

        /* DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW and DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW */
        if ( xmi_element_info_is_a_transition( rel_info )
            && (( end_feature_type == DATA_FEATURE_TYPE_ENTRY )||( end_feature_type == DATA_FEATURE_TYPE_EXIT )))
        {
            /* valid relationship according to uml 2.5.1 spec, chapter 14.5.11 */
        }
        else if ( xmi_element_info_is_a_activity_edge( rel_info ) && xmi_element_info_is_a_activity_node( feature_info ) )
        {
            /* valid relationship according to uml 2.5.1 spec, chapter 15.7.2 */
        }

        /* DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL or DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL or DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL */
        else if ( xmi_element_info_is_a_message ( rel_info ) && ( end_feature_type == DATA_FEATURE_TYPE_LIFELINE ))
        {
            /* valid relationship according to uml 2.5.1 spec, chapter 17.12.23 */
        }

        /* DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, */
        /* DATA_RELATIONSHIP_TYPE_UML_DEPLOY, DATA_RELATIONSHIP_TYPE_UML_REFINE,  DATA_RELATIONSHIP_TYPE_UML_TRACE */
        else if ( xmi_element_info_is_a_dependency( rel_info ) && xmi_element_info_is_a_named_element( feature_info ) )
        {
            /* a dependency can connect any named elements according to uml 2.5.1 spec, chapter 7.8.4  */
        }

        /* DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, */
        /* DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT */
        else if ( xmi_element_info_is_a_association( rel_info ) && xmi_element_info_is_a_class( feature_info ) )
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a property can connect a class according to uml 2.5.1 spec, chapter 9.9.17  */
        }
        else if ( xmi_element_info_is_a_association( rel_info )
            && (( end_feature_type ==  DATA_FEATURE_TYPE_PROVIDED_INTERFACE )||( end_feature_type == DATA_FEATURE_TYPE_REQUIRED_INTERFACE )))
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a property can connect an interface according to uml 2.5.1 spec, chapter 9.9.17  */
        }
        else if ( xmi_element_info_is_a_association( rel_info ) && xmi_element_info_is_a_classifier( feature_info ) )
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a property is a typedelelemnt and can therefore connect a classifier(type) according to uml 2.5.1 spec, chapter 7.8.22  */
        }
        else if ( xmi_element_info_is_a_association( rel_info ) && xmi_element_info_is_a_property( feature_info ) )
        {
            /* an association can connect to a propoerty according to uml 2.5.1 spec, chapter 11.8.1  */
            /* a port is a property */
        }

        else
        {
            /* no valid end type for given relationship type */
            err = U8_ERROR_NOT_FOUND;
        }

    }

    U8_TRACE_END_ERR( err );
    return err;
}


/*
Copyright 2020-2024 Andreas Warnke

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
