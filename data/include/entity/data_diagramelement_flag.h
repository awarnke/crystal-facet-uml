/* File: data_diagramelement_flag.h; Copyright and License: see below */

#ifndef DATA_DIAGRAMELEMENT_FLAG_H
#define DATA_DIAGRAMELEMENT_FLAG_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines semantics (like instance-of) and layout (like emphasis) of classifiers in diagrams
 */

#include "entity/data_enum_def.h"

/*!
 *  \brief constants which define display options for a classifier in a diagram
 */
enum data_diagramelement_flag_enum {
    DATA_DIAGRAMELEMENT_FLAG_NONE = 0x0,  /*!< no flags set */
    DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE = 0x01,  /*!< the classifier shall be drawn as a named instance (underline) */
    DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE = 0x02,  /*!< the classifier shall be drawn as anonymous instance (colon-prefix and underline) */
    DATA_DIAGRAMELEMENT_FLAG_EMPHASIS = 0x1000000,  /*!< the classifier shall be marked by a color or other appropriate means */
    DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT = 0x2000000,  /*!< the classifier shall be greyed out */

    DATA_DIAGRAMELEMENT_FLAG_MASK_SEMANTICS = 0x0f,  /*!< mask for semantics bits */
    DATA_DIAGRAMELEMENT_FLAG_MASK_RELATIONS = 0xff00,  /*!< mask for related objects, like notes, relationships, parent-classifiers */
    DATA_DIAGRAMELEMENT_FLAG_MASK_POSITION = 0xff0000,  /*!< mask for positioning hints */
    DATA_DIAGRAMELEMENT_FLAG_MASK_EMPHASIS = 0x7f000000,  /*!< mask for emphasis and grey-out information */
};

typedef enum data_diagramelement_flag_enum data_diagramelement_flag_t;

/*!
 *  \brief total number of flags, taking into account combinations if applicable
 */
#define DATA_DIAGRAMELEMENT_FLAG_COUNT (3*3)

/*!
 *  \brief lists all values of data_diagramelement_flag_t
 */
extern const data_enum_def_t DATA_DIAGRAMELEMENT_FLAG_ENUM_ARRAY [ DATA_DIAGRAMELEMENT_FLAG_COUNT ];

/*!
 *  \brief returns a name for the data_diagramelement_flag_t
 *
 *  \param this_ enumeration value
 *  \return name(s) of the flag(s), "" if unknown or none
 */
const char * data_diagramelement_flag_get_name( data_diagramelement_flag_t this_ );

#endif  /* DATA_DIAGRAMELEMENT_FLAG_H */


/*
Copyright 2016-2026 Andreas Warnke

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
