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
    DATA_FEATURE_TYPE_PROPERTY = 0,
    DATA_FEATURE_TYPE_OPERATION = 1,
    DATA_FEATURE_TYPE_PORT = 2,
    DATA_FEATURE_TYPE_LIFELINE = 3,  /* for sequence diagrams and timing diagrams and communication diagrams */
};

typedef enum data_feature_type_enum data_feature_type_t;

#endif  /* DATA_FEATURE_TYPE_H */


/*
Copyright 2016-2019 Andreas Warnke

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
