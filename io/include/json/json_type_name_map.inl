/* File: json_type_name_map.inl; Copyright and License: see below */

#include "xmi/xmi_element_info.h"
#include <assert.h>

static inline void json_type_name_map_init( json_type_name_map_t *this_ )
{
    (*this_).dummy = 0;
}

static inline void json_type_name_map_destroy( json_type_name_map_t *this_ )
{
}

static inline const char * json_type_name_map_get_classifier_type( json_type_name_map_t *this_, data_classifier_type_t c_type )
{
    return "dummy";
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
