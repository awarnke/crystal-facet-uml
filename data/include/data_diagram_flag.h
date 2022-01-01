/* File: data_diagram_flag.h; Copyright and License: see below */

#ifndef DATA_DIAGRAM_FLAG_H
#define DATA_DIAGRAM_FLAG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines layout (like emphasis) of diagrams
 */

/*!
 *  \brief constants which define display options for a classifier in a diagram
 */
enum data_diagram_flag_enum {
    DATA_DIAGRAM_FLAG_NONE = 0x0,  /*!< no flags set */
    DATA_DIAGRAM_FLAG_EMPHASIS = 0x1000000,  /*!< the diagram shall be marked by a color or other appropriate means */
    DATA_DIAGRAM_FLAG_GRAY_OUT = 0x2000000,  /*!< the diagram shall be greyed out */

    DATA_DIAGRAM_FLAG_MASK_SEMANTICS = 0x0f,  /*!< mask for semantics bits */
    DATA_DIAGRAM_FLAG_MASK_RELATIONS = 0xff00,  /*!< mask for related objects */
    DATA_DIAGRAM_FLAG_MASK_POSITION = 0xff0000,  /*!< mask for positioning hints */
    DATA_DIAGRAM_FLAG_MASK_EMPHASIS = 0x7f000000,  /*!< mask for emphasis and grey-out information */
};

typedef enum data_diagram_flag_enum data_diagram_flag_t;

#endif  /* DATA_DIAGRAM_FLAG_H */


/*
Copyright 2021-2022 Andreas Warnke

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
