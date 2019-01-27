/* File: data_classifier_type.h; Copyright and License: see below */

#ifndef DATA_CLASSIFIER_TYPE_H
#define DATA_CLASSIFIER_TYPE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Defines types of classifiers
 */

/*!
 *  \brief enumeration of all classifier types
 *
 *  \see https://en.wikipedia.org/wiki/Classifier_%28UML%29
 *  \see http://www.uml-diagrams.org/classifier.html
 *  \see https://sysml.org/docs/specs/OMGSysML-v1.5-17-05-01.pdf
 */
enum data_classifier_type_enum {
    /* Block + Internal Block Diagrams: */
    DATA_CLASSIFIER_TYPE_BLOCK = 90,  /* SysML */
    /* Parametric Diagram: */
    DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY = 96,  /* SysML, this is an equation */
    /* Requirements Diagram: */
    DATA_CLASSIFIER_TYPE_FEATURE = 98,
    DATA_CLASSIFIER_TYPE_REQUIREMENT = 99,  /* SysML */
    /* Use Case Diagram */
    DATA_CLASSIFIER_TYPE_UML_ACTOR = 100,  /* SysML */
    DATA_CLASSIFIER_TYPE_UML_USE_CASE = 101,  /* SysML */
    DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY = 102,  /* SysML */
    DATA_CLASSIFIER_TYPE_UML_ACTIVITY = 110,  /* SysML */
    DATA_CLASSIFIER_TYPE_UML_STATE = 111,
    DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE = 112,
    DATA_CLASSIFIER_TYPE_UML_NODE = 120,
    DATA_CLASSIFIER_TYPE_UML_COMPONENT = 121,
    DATA_CLASSIFIER_TYPE_UML_PART = 122,
    DATA_CLASSIFIER_TYPE_UML_INTERFACE = 123,
    DATA_CLASSIFIER_TYPE_UML_PACKAGE = 124,
    DATA_CLASSIFIER_TYPE_UML_CLASS = 125,
    DATA_CLASSIFIER_TYPE_UML_OBJECT = 126,  /* SysML */
    DATA_CLASSIFIER_TYPE_UML_ARTIFACT = 127,
    DATA_CLASSIFIER_TYPE_UML_COMMENT = 130, /* this is a note-text */
    DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION = 150,  /* interruptable activity region, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE = 151,  /* start control node, entry point, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE = 152,  /* end control node, exit point, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_FORK_NODE = 154,  /* fork control node, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE = 155,  /* join control node, UML and SYSML */
    // /*DATA_CLASSIFIER_TYPE_DYN_MERGE_NODE = 156, */  /* merge control node=junction - we do not need this maybe, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE = 157,  /* decision control node=choice, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY = 158,  /* history pseudo state, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY = 159,  /* history pseudo state, UML and SYSML */
    // /*DATA_CLASSIFIER_TYPE_DYN_PARTITION = 164, */  /* activity partition, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT = 167,  /* receive signal, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT = 168,  /* receive time signal, UML and SYSML */
    DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL = 170,  /* sends a signal, UML and SYSML */
    // missing: repeat-blocks/sections in sequence diagrams
    // missing: activity-bars and life-end-cross on lifelines - maybe these can be auto-generated?
};

typedef enum data_classifier_type_enum data_classifier_type_t;


#endif  /* DATA_CLASSIFIER_TYPE_H */


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
