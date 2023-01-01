/* File: data_classifier_type.inl; Copyright and License: see below */

static inline bool data_classifier_type_is_behavioral( data_classifier_type_t this_ )
{
    const bool result
        = ( this_ == DATA_CLASSIFIER_TYPE_USE_CASE )
        ||( this_ == DATA_CLASSIFIER_TYPE_ACTIVITY )
        ||( this_ == DATA_CLASSIFIER_TYPE_STATE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_FORK_NODE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT )
        ||( this_ == DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL )
        ||( this_ == DATA_CLASSIFIER_TYPE_INTERACTION );
    return result;
}


/*
Copyright 2020-2023 Andreas Warnke

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
