/* File: xmi_element_info.inl; Copyright and License: see below */

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

static inline bool xmi_element_info_is_a_activity ( const xmi_element_info_t *this_ )
{
    return ( ((*this_).is_a) & XMI_ELEMENT_IS_A_ACTIVITY ) == XMI_ELEMENT_IS_A_ACTIVITY;
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
