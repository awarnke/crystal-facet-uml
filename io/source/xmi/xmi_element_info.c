/* File: xmi_element_info.c; Copyright and License: see below */

#include "xmi/xmi_element_info.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

const xmi_element_info_t xmi_element_info_standard_list[XMI_ELEMENT_INFO_MAX_ENTRIES]
= {
    [XMI_ELEMENT_INFO_INDEX_BLOCK] = {
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_BLOCK,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = XMI_ELEMENT_INFO_NS_SYSML "Block",
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
    },
};


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
