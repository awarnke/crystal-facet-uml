/* File: xmi_diagram_info.inl; Copyright and License: see below */

#include <stddef.h>

static inline int xmi_diagram_info_get_data_type_checksum ( const xmi_diagram_info_t *this_ )
{
    return (*this_).data_type_checksum;
}

static inline xmi_spec_t xmi_diagram_info_get_specification ( const xmi_diagram_info_t *this_ )
{
    return (*this_).specification;
}

static inline const char * xmi_diagram_info_get_profile_name ( const xmi_diagram_info_t *this_ )
{
    return (*this_).profile_name;
}

static inline const char * xmi_diagram_info_get_base_name ( const xmi_diagram_info_t *this_ )
{
    return (*this_).base_name;
}

static inline const char * xmi_diagram_info_get_name ( const xmi_diagram_info_t *this_ )
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


/*
Copyright 2022-2022 Andreas Warnke

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
