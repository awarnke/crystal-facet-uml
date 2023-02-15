/* File: xmi_writer_pass.h; Copyright and License: see below */

#ifndef XMI_WRITER_PASS_H
#define XMI_WRITER_PASS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the semantics of multiple model-traversal-passes
 */

/*!
 *  \brief constants for output mode: basic mode or extended profile mode.
 */
enum xmi_writer_pass_enum {
    XMI_WRITER_PASS_BASE = 0,  /*!< mode for basic type output: xmi, uml and mof */
    XMI_WRITER_PASS_PROFILE = 1,  /*!< mode for profile type output: StandardProfile, mofext or SysML */
};

typedef enum xmi_writer_pass_enum xmi_writer_pass_t;

#endif  /* XMI_WRITER_PASS_H */


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
