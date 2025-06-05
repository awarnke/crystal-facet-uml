/* File: pencil_rules.inl; Copyright and License: see below */

static inline void pencil_rules_init ( pencil_rules_t *this_ )
{
    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */
}

static inline void pencil_rules_destroy ( pencil_rules_t *this_ )
{
}

static inline bool pencil_rules_feature_is_implicit_proxy ( const pencil_rules_t *this_,
                                                            data_feature_type_t feature_type,
                                                            data_classifier_type_t classifier_type,
                                                            data_diagram_type_t diagram_type )
{
    const bool classifier_can_have_lifeline
        = ! (( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_COMMENT )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_INTERACTION )
        || ( classifier_type == DATA_CLASSIFIER_TYPE_INTERACTION_USE ));
    const bool diagram_can_show_lifeline
        = ( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
        || ( diagram_type == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM );
    const bool feature_is_lifeline = ( feature_type == DATA_FEATURE_TYPE_LIFELINE );
    return feature_is_lifeline && (( ! diagram_can_show_lifeline )||( ! classifier_can_have_lifeline ));
}


/*
Copyright 2023-2025 Andreas Warnke

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
