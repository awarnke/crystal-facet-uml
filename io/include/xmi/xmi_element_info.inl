/* File: xmi_element_info.inl; Copyright and License: see below */

#include <stddef.h>

static inline int xmi_element_info_get_data_type_checksum ( const xmi_element_info_t *this_ )
{
    return (*this_).data_type_checksum;
}

static inline xmi_spec_t xmi_element_info_get_specification ( const xmi_element_info_t *this_ )
{
    return (*this_).specification;
}

static inline const char * xmi_element_info_get_profile_name ( const xmi_element_info_t *this_ )
{
    return (*this_).profile_name;
}

static inline const char * xmi_element_info_get_base_name ( const xmi_element_info_t *this_ )
{
    return (*this_).base_name;
}

static inline const char * xmi_element_info_get_name ( const xmi_element_info_t *this_ )
{
    const char* result;
    if ( (*this_).profile_name == NULL )
    {
        result = (*this_).base_name;
    }
    else
    {
        result = (*this_).profile_name;
    }
    return result;
}

static inline const char * xmi_element_info_get_property_from ( const xmi_element_info_t *this_ )
{
    return (*this_).property_from;
}

static inline const char * xmi_element_info_get_property_to ( const xmi_element_info_t *this_ )
{
    return (*this_).property_to;
}

static inline const char * xmi_element_info_get_additional_properties ( const xmi_element_info_t *this_ )
{
    return (*this_).additional_properties;
}

static inline bool xmi_element_info_is_a_encapsulated_classifier ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ENCAPSULATED_CLASSIFIER ) == XMI_ELEMENT_IS_A_ENCAPSULATED_CLASSIFIER;
}

static inline bool xmi_element_info_is_a_classifier ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_CLASSIFIER ) == XMI_ELEMENT_IS_A_CLASSIFIER;
}

static inline bool xmi_element_info_is_a_comment ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_COMMENT ) == XMI_ELEMENT_IS_A_COMMENT;
}

static inline bool xmi_element_info_is_a_behavior ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_BEHAVIOR ) == XMI_ELEMENT_IS_A_BEHAVIOR;
}

static inline bool xmi_element_info_is_a_named_element ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_NAMED_ELEMENT ) == XMI_ELEMENT_IS_A_NAMED_ELEMENT;
}

static inline bool xmi_element_info_is_a_typed_element ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_TYPED_ELEMENT ) == XMI_ELEMENT_IS_A_TYPED_ELEMENT;
}

static inline bool xmi_element_info_is_a_package ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_PACKAGE ) == XMI_ELEMENT_IS_A_PACKAGE;
}

static inline bool xmi_element_info_is_a_packageable_element ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT ) == XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT;
}

static inline bool xmi_element_info_is_a_node ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_NODE ) == XMI_ELEMENT_IS_A_NODE;
}

static inline bool xmi_element_info_is_a_activity_group ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ACTIVITY_GROUP ) == XMI_ELEMENT_IS_A_ACTIVITY_GROUP;
}

static inline bool xmi_element_info_is_a_activity_edge ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ACTIVITY_EDGE ) == XMI_ELEMENT_IS_A_ACTIVITY_EDGE;
}

static inline bool xmi_element_info_is_a_activity_node ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ACTIVITY_NODE ) == XMI_ELEMENT_IS_A_ACTIVITY_NODE;
}

static inline bool xmi_element_info_is_a_artifact ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ARTIFACT ) == XMI_ELEMENT_IS_A_ARTIFACT;
}

static inline bool xmi_element_info_is_a_behaviored_classifier ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER ) == XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER;
}

static inline bool xmi_element_info_is_a_class ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_CLASS ) == XMI_ELEMENT_IS_A_CLASS;
}

static inline bool xmi_element_info_is_a_vertex ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_VERTEX ) == XMI_ELEMENT_IS_A_VERTEX;
}

static inline bool xmi_element_info_is_a_transition ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_TRANSITION ) == XMI_ELEMENT_IS_A_TRANSITION;
}

static inline bool xmi_element_info_is_a_message ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_MESSAGE ) == XMI_ELEMENT_IS_A_MESSAGE;
}

static inline bool xmi_element_info_is_a_interaction_fragment ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_INTERACTION_FRAGMENT ) == XMI_ELEMENT_IS_A_INTERACTION_FRAGMENT;
}

static inline bool xmi_element_info_is_a_association ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ASSOCIATION ) == XMI_ELEMENT_IS_A_ASSOCIATION;
}

static inline bool xmi_element_info_is_a_dependency ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_DEPENDENCY ) == XMI_ELEMENT_IS_A_DEPENDENCY;
}

static inline bool xmi_element_info_is_a_property ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_PROPERTY ) == XMI_ELEMENT_IS_A_PROPERTY;
}

static inline bool xmi_element_info_is_a_tagged_value ( const xmi_element_info_t *this_ )
{
    return (*this_).is_a == XMI_ELEMENT_IS_A_TAGGED_VALUE;
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
