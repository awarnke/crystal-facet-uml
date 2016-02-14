/* File: data_classifier_type.h; Copyright and License: see below */

#ifndef DATA_CLASSIFIER_TYPE_H
#define DATA_CLASSIFIER_TYPE_H

/* public file for the doxygen documentation: */
/*! \file */

/*!
 *  \brief enumeration of all classifier types
 */
enum data_classifier_type_enum {
    DATA_CLASSIFIER_TYPE_UNSPECIFIED = 0,
    DATA_CLASSIFIER_TYPE_BLOCK = 90,
    DATA_CLASSIFIER_TYPE_NOTE = 91,
    DATA_CLASSIFIER_TYPE_FEATURE = 98,
    DATA_CLASSIFIER_TYPE_REQUIREMENT = 99,
    DATA_CLASSIFIER_TYPE_UML_ACTOR = 100,
    DATA_CLASSIFIER_TYPE_UML_USE_CASE = 101,
    DATA_CLASSIFIER_TYPE_UML_ACTIVITY = 110,
    DATA_CLASSIFIER_TYPE_UML_STATE = 111,
    DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE = 112,
    DATA_CLASSIFIER_TYPE_UML_NODE = 120,
    DATA_CLASSIFIER_TYPE_UML_COMPONENT = 121,
    DATA_CLASSIFIER_TYPE_UML_PART = 122,
    DATA_CLASSIFIER_TYPE_UML_INTERFACE = 123,
    DATA_CLASSIFIER_TYPE_UML_PACKAGE = 124,
    DATA_CLASSIFIER_TYPE_UML_CLASS = 125,
    DATA_CLASSIFIER_TYPE_UML_OBJECT = 126,
};

typedef enum data_classifier_type_enum data_classifier_type_t;


#endif  /* DATA_CLASSIFIER_TYPE_H */


/*
Copyright 2016-2016 Andreas Warnke

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
