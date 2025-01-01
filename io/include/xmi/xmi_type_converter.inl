/* File: xmi_type_converter.inl; Copyright and License: see below */

#include <assert.h>

/* ================================ CLASSIFIER ================================ */

static inline bool xmi_type_converter_can_nest_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t parent_type,
                                                            data_classifier_type_t child_type )
{
    char const * xmi_name;
    const int err_code
        = xmi_type_converter_get_xmi_nesting_property_of_classifier ( this_,
                                                                      parent_type,
                                                                      child_type,
                                                                      &xmi_name
                                                                    );
    return ( 0 == err_code );
}

static inline bool xmi_type_converter_can_nest_relationship ( xmi_type_converter_t *this_,
                                                              data_classifier_type_t hosting_type,
                                                              data_relationship_type_t child_type )
{
    char const * xmi_name;
    const int err_code
        = xmi_type_converter_get_xmi_nesting_property_of_relationship ( this_,
                                                                        hosting_type,
                                                                        child_type,
                                                                        &xmi_name
                                                                      );
    return ( 0 == err_code );
}

/* ================================ FEATURE ================================ */

/* ================================ RELATIONSHIP ================================ */

static inline u8_error_t xmi_type_converter_get_xmi_from_property_of_relationship ( xmi_type_converter_t *this_,
                                                                                    data_classifier_type_t hosting_type,
                                                                                    data_relationship_type_t rel_type,
                                                                                    data_classifier_type_t from_classifier_type,
                                                                                    data_feature_type_t from_feature_type,
                                                                                    char const * *out_xmi_name )
{
    return xmi_type_converter_private_get_xmi_end_property_of_relationship( this_, 
                                                                            hosting_type, 
                                                                            rel_type, 
                                                                            true /* = from */,
                                                                            from_classifier_type,
                                                                            from_feature_type,
                                                                            out_xmi_name
                                                                          );
}

static inline u8_error_t xmi_type_converter_get_xmi_to_property_of_relationship ( xmi_type_converter_t *this_,
                                                                                  data_classifier_type_t hosting_type,
                                                                                  data_relationship_type_t rel_type,
                                                                                  data_classifier_type_t to_classifier_type,
                                                                                  data_feature_type_t to_feature_type,
                                                                                  char const * *out_xmi_name )
{
    return xmi_type_converter_private_get_xmi_end_property_of_relationship( this_, 
                                                                            hosting_type, 
                                                                            rel_type, 
                                                                            false /* = to */,
                                                                            to_classifier_type,
                                                                            to_feature_type,
                                                                            out_xmi_name
                                                                          );
}


/*
Copyright 2020-2025 Andreas Warnke

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
