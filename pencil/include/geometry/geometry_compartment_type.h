/* File: geometry_compartment_type.h; Copyright and License: see below */

#ifndef GEOMETRY_COMPARTMENT_TYPE_H
#define GEOMETRY_COMPARTMENT_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores dimensions of several feature compartments
 */

#include "entity/data_feature_type.h"
#include <stdint.h>

/*!
 *  \brief constants identifying compartments
 */
enum geometry_compartment_type_type_enum {
    GEOMETRY_COMPARTMENT_TYPE_VOID,  /*!< invalid compartment type, e.g. derived from a lifeline */
    GEOMETRY_COMPARTMENT_TYPE_PROPERTIES,
    GEOMETRY_COMPARTMENT_TYPE_OPERATIONS,
    GEOMETRY_COMPARTMENT_TYPE_TAGGED_VALUES,
    GEOMETRY_COMPARTMENT_TYPE_PORT_ON_LEFT,
    GEOMETRY_COMPARTMENT_TYPE_PORT_ON_RIGHT,
    GEOMETRY_COMPARTMENT_TYPE_PORT_ON_TOP,
    GEOMETRY_COMPARTMENT_TYPE_PORT_ON_BOTTOM,
    GEOMETRY_COMPARTMENT_TYPE_IF_ON_LEFT,
    GEOMETRY_COMPARTMENT_TYPE_IF_ON_RIGHT,
    GEOMETRY_COMPARTMENT_TYPE_IF_ON_TOP,
    GEOMETRY_COMPARTMENT_TYPE_IF_ON_BOTTOM
};

typedef enum geometry_compartment_type_type_enum geometry_compartment_type_t;

/*!
 *  \brief initializes the geometry_compartment_type_t struct
 *
 *  \param feature_type the feature type for which to create a geometry_compartment_type_t
 *  \param list_order the list order of the feature - to derive the top/bottom/left/right sides of the feature
 *  \return initialized object, GEOMETRY_COMPARTMENT_TYPE_VOID if unsuitable feature_type like LIFELINE
 */
static inline geometry_compartment_type_t geometry_compartment_type_new ( data_feature_type_t feature_type,
                                                                          uint32_t list_order );

#include "geometry/geometry_compartment_type.inl"

#endif  /* GEOMETRY_COMPARTMENT_TYPE_H */


/*
Copyright 2026-2026 Andreas Warnke

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
