/* File: data_feature_type.h; Copyright and License: see below */

#ifndef DATA_FEATURE_TYPE_H
#define DATA_FEATURE_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines types of features
 */

#include <stdbool.h>

/*!
 *  \brief enumeration of all feature types
 */
enum data_feature_type_enum {
    DATA_FEATURE_TYPE_VOID = -1,  /*!< invalid feature, used when relationships link from and/or to classifiers without specifying features */
    DATA_FEATURE_TYPE_PROPERTY = 0,
    DATA_FEATURE_TYPE_OPERATION = 1,
    DATA_FEATURE_TYPE_PORT = 2,
    DATA_FEATURE_TYPE_LIFELINE = 3,  /* for sequence, timing, communication and interaction-overview diagrams */
    DATA_FEATURE_TYPE_PROVIDED_INTERFACE = 4,  /* service */
    DATA_FEATURE_TYPE_REQUIRED_INTERFACE = 5,  /* client */
    DATA_FEATURE_TYPE_IN_PORT_PIN = 6,  /* port with SysML-ItemFlow, activity input pin */
    DATA_FEATURE_TYPE_OUT_PORT_PIN = 7,  /* port with SysML-ItemFlow, activity output pin */
    DATA_FEATURE_TYPE_ENTRY = 8,  /* control flow pseudostate for state entry */
    DATA_FEATURE_TYPE_EXIT = 9,  /* control flow pseudostate for state exit */
    DATA_FEATURE_TYPE_TAGGED_VALUE = 10,  /* tagged value, since 1.47.0 */
    /* DATA_FEATURE_TYPE_DIAG_LINK = 11, */  /* link to diagram, e.g. for DATA_CLASSIFIER_TYPE_INTERACTION_USE */
};

typedef enum data_feature_type_enum data_feature_type_t;

#define DATA_FEATURE_TYPE_COUNT (11)

/*!
 *  \brief a modulo for a hash function for unique hashing
 */
#define DATA_FEATURE_TYPE_HASH_MOD (12)

/*!
 *  \brief lists all values of data_feature_type_t, excluding DATA_FEATURE_TYPE_VOID
 */
extern data_feature_type_t const DATA_FEATURE_TYPE_ARRAY [DATA_FEATURE_TYPE_COUNT];

/*!
 *  \brief checks if the classifier type is displayed outside of classifier compartments
 *
 *  \param this_ enumeration value
 *  \return true if the feature is displayed outside a classifiers compartment like ports or lifelines,
 *          false otherwise (or if the feature type is unknown)
 */
static inline bool data_feature_type_outside_compartment( data_feature_type_t this_ );

/*!
 *  \brief checks if the classifier type is displayed inside of classifier compartments
 *
 *  \param this_ enumeration value
 *  \return false if the feature is displayed outside a classifiers compartment like ports or lifelines,
 *          true otherwise (or if the feature type is unknown)
 */
static inline bool data_feature_type_inside_compartment( data_feature_type_t this_ );

/*!
 *  \brief returns a short name for the feature type
 *
 *  \param this_ enumeration value
 *  \return name of the type, "" if unknown or void type
 */
const char * data_feature_type_get_name( data_feature_type_t this_ );

#include "entity/data_feature_type.inl"

#endif  /* DATA_FEATURE_TYPE_H */


/*
Copyright 2016-2025 Andreas Warnke

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
