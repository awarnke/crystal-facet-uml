/* File: xmi_type_converter.inl; Copyright and License: see below */

#include <assert.h>

/* ================================ CLASSIFIER ================================ */

static inline bool xmi_type_converter_can_classifier_nest_classifier ( xmi_type_converter_t *this_,
                                                                       data_classifier_type_t parent_type,
                                                                       data_classifier_type_t child_type )
{
    char const * xmi_name;
    return ( 0 == xmi_type_converter_get_xmi_nesting_property_of_classifier ( this_,
                                                                              parent_type,
                                                                              child_type,
                                                                              &xmi_name
                                                                            ) );
}

/* ================================ FEATURE ================================ */

/* ================================ RELATIONSHIP ================================ */

static inline const char* xmi_type_converter_get_xmi_from_property_of_relationship ( xmi_type_converter_t *this_,
                                                                                     data_relationship_type_t r_type )
{
    return xmi_type_converter_private_get_xmi_end_property_of_relationship( this_, r_type, true );
}

static inline const char* xmi_type_converter_get_xmi_to_property_of_relationship ( xmi_type_converter_t *this_,
                                                                                   data_relationship_type_t r_type )
{
    return xmi_type_converter_private_get_xmi_end_property_of_relationship( this_, r_type, false );
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
