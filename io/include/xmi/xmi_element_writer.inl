/* File: xmi_element_writer.inl; Copyright and License: see below */

#include "xmi/xmi_element_part.h"
#include "xmi/xmi_xml.h"

static inline io_writer_pass_t xmi_element_writer_get_mode( xmi_element_writer_t *this_ )
{
    return (*this_).mode;
}

static inline void xmi_element_writer_set_mode( xmi_element_writer_t *this_, io_writer_pass_t mode )
{
    (*this_).mode = mode;
}

static inline xml_writer_t *xmi_element_writer_get_xml_writer_ptr( xmi_element_writer_t *this_ )
{
    return &((*this_).xml_writer);
}


/*
Copyright 2020-2021 Andreas Warnke

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
