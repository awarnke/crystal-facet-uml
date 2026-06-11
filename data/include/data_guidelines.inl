/* File: data_guidelines.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void data_guidelines_init ( data_guidelines_t *this_ )
{
    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */
}

static inline void data_guidelines_destroy ( data_guidelines_t *this_ )
{
}

/* ================================ DIAGRAM ================================ */

/* ================================ CLASSIFIER ================================ */

static inline bool data_guidelines_classifier_has_scenario_semantics ( const data_guidelines_t *this_,
                                                                       data_diagram_type_t diagram_type,
                                                                       data_classifier_type_t classifier_type )
{
    const bool lifeline_has_no_semantics
    = ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
    || ( classifier_type == DATA_CLASSIFIER_TYPE_COMMENT )
    || ( classifier_type == DATA_CLASSIFIER_TYPE_IMAGE )
    || ( classifier_type == DATA_CLASSIFIER_TYPE_STEREOTYPE )
    || (( diagram_type == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
    && (( classifier_type == DATA_CLASSIFIER_TYPE_INTERACTION )
    || ( classifier_type == DATA_CLASSIFIER_TYPE_INTERACTION_USE )));
    return ( ! lifeline_has_no_semantics);
}

/* ================================ FEATURE ================================ */

static inline bool data_guidelines_feature_value_is_stereotype ( const data_guidelines_t *this_, data_feature_type_t feature_type )
{
    bool has_value;
    has_value = (( feature_type == DATA_FEATURE_TYPE_PROPERTY )
           || ( feature_type == DATA_FEATURE_TYPE_OPERATION )
           || ( feature_type == DATA_FEATURE_TYPE_TAGGED_VALUE ));
    return ( ! has_value );
}

/* ================================ RELATIONSHIP ================================ */

/*
Copyright 2017-2026 Andreas Warnke

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
