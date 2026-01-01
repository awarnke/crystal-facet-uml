/* File: data_feature_type.inl; Copyright and License: see below */

static inline bool data_feature_type_outside_compartment( data_feature_type_t this_ )
{
    const bool result
        = ( this_ == DATA_FEATURE_TYPE_PORT )
        ||( this_ == DATA_FEATURE_TYPE_LIFELINE )
        ||( this_ == DATA_FEATURE_TYPE_PROVIDED_INTERFACE )
        ||( this_ == DATA_FEATURE_TYPE_REQUIRED_INTERFACE )
        ||( this_ == DATA_FEATURE_TYPE_IN_PORT_PIN )
        ||( this_ == DATA_FEATURE_TYPE_OUT_PORT_PIN )
        ||( this_ == DATA_FEATURE_TYPE_ENTRY )
        ||( this_ == DATA_FEATURE_TYPE_EXIT );
    return result;
}

static inline bool data_feature_type_inside_compartment( data_feature_type_t this_ )
{
    return ( ! data_feature_type_outside_compartment( this_ ) );
}

/*
Copyright 2023-2026 Andreas Warnke

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
