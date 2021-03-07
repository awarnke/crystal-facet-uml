/* File: data_diagram_type.inl; Copyright and License: see below */

static inline bool data_diagram_type_is_interaction( data_diagram_type_t this_ )
{
    const bool result
        = ( this_ == DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM )
        ||( this_ == DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM )
        ||( this_ == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
        ||( this_ == DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM );
    return result;
}

    
/*
Copyright 2021-2021 Andreas Warnke

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
