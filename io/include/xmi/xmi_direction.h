/* File: xmi_direction.h; Copyright and License: see below */

#ifndef XMI_DIRECTION_H
#define XMI_DIRECTION_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the flow direction of ports and pins
 */

/*!
 *  \brief control flow and data flow directions
 */
enum xmi_direction_enum {
    XMI_DIRECTION_UNSPECIFIED,
    XMI_DIRECTION_IN,
    XMI_DIRECTION_OUT,
    XMI_DIRECTION_IN_OUT,
};

typedef enum xmi_direction_enum xmi_direction_t;

#endif  /* XMI_DIRECTION_H */


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
