/* File: xmi_spec.h; Copyright and License: see below */

#ifndef XMI_SPEC_H
#define XMI_SPEC_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the xmi specifications for crystal facet uml types
 */

/*!
 *  \brief bit set of xmi specifications for crystal facet uml types
 */
enum xmi_spec_enum {
    XMI_SPEC_VOID      = 0x0000,  /*!< not specified anywhere */
    XMI_SPEC_MOF       = 0x0001,  /*!< specified in MOF specification */
    XMI_SPEC_XMI       = 0x0002,  /*!< specified in XMI file format specification */
    XMI_SPEC_UML       = 0x0004,  /*!< specified in UML specification */
    XMI_SPEC_PRIMITIVE = 0x0008,  /*!< specified in Primitive Types profile specification */
    XMI_SPEC_STANDARD  = 0x0010,  /*!< specified in StandardProfile specification */
    XMI_SPEC_SYSML     = 0x0020,  /*!< specified in SYSML profile specification */
};

typedef enum xmi_spec_enum xmi_spec_t;

#endif  /* XMI_SPEC_H */


/*
Copyright 2020-2024 Andreas Warnke

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
