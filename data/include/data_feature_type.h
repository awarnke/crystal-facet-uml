/* File: data_feature_type.h; Copyright and License: see below */

#ifndef DATA_FEATURE_TYPE_H
#define DATA_FEATURE_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines types of features
 */

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
};

typedef enum data_feature_type_enum data_feature_type_t;

#endif  /* DATA_FEATURE_TYPE_H */


/*
Copyright 2016-2020 Andreas Warnke

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
