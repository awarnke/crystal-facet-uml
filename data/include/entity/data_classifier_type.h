/* File: data_classifier_type.h; Copyright and License: see below */

#ifndef DATA_CLASSIFIER_TYPE_H
#define DATA_CLASSIFIER_TYPE_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Defines types of classifiers
 */

#include <stdbool.h>

/*!
 *  \brief enumeration of all classifier types
 *
 *  \see https://en.wikipedia.org/wiki/Classifier_%28UML%29
 *  \see http://www.uml-diagrams.org/classifier.html
 *  \see https://sysml.org/docs/specs/OMGSysML-v1.5-17-05-01.pdf
 */
enum data_classifier_type_enum {
    DATA_CLASSIFIER_TYPE_VOID = -1,  /* ! < invalid classifier or unknown type */
    /* Block + Internal Block Diagrams: */
    DATA_CLASSIFIER_TYPE_BLOCK = 90,  /* SysML */
    /* Parametric Diagram: */
    DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK = 96,  /* SysML, this is an equation */
    /* Requirements Diagram: */
    /* DATA_CLASSIFIER_TYPE_TESTCASE = 97, */  /* SysML, never activated */
    /* DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE = 98, */  /* deprecated since version 1.25.0 */
    DATA_CLASSIFIER_TYPE_REQUIREMENT = 99,  /* SysML */
    /* Use Case Diagram */
    DATA_CLASSIFIER_TYPE_ACTOR = 100,
    DATA_CLASSIFIER_TYPE_USE_CASE = 101,
    DATA_CLASSIFIER_TYPE_SUBSYSTEM = 102,  /* a component of stereotype subsystem */
    DATA_CLASSIFIER_TYPE_ACTIVITY = 110,
    DATA_CLASSIFIER_TYPE_STATE = 111,
    DATA_CLASSIFIER_TYPE_INTERACTION_USE = 112,  /* a diagram reference */
    DATA_CLASSIFIER_TYPE_NODE = 120,
    DATA_CLASSIFIER_TYPE_COMPONENT = 121,
    DATA_CLASSIFIER_TYPE_PART = 122,
    DATA_CLASSIFIER_TYPE_INTERFACE = 123,
    DATA_CLASSIFIER_TYPE_PACKAGE = 124,
    DATA_CLASSIFIER_TYPE_CLASS = 125,
    DATA_CLASSIFIER_TYPE_OBJECT = 126,
    DATA_CLASSIFIER_TYPE_ARTIFACT = 127,
    DATA_CLASSIFIER_TYPE_COMMENT = 130, /* this is a note-text */
    DATA_CLASSIFIER_TYPE_IMAGE = 138, /* since version 1.48.0, an image object */
    DATA_CLASSIFIER_TYPE_STEREOTYPE = 139, /* since version 1.47.0, a stereotype, including its image */
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
    DATA_CLASSIFIER_TYPE_INTERACTION = 190,  /* a uml interaction. Currently this does not exist in the data model but is faked at xmi export */
};
typedef enum data_classifier_type_enum data_classifier_type_t;

#define DATA_CLASSIFIER_TYPE_COUNT (32)

/*!
 *  \brief a modulo for a hash function for unique hashing
 */
#define DATA_CLASSIFIER_TYPE_HASH_MOD (75)

/*!
 *  \brief lists all values of data_classifier_type_t, excluding DATA_CLASSIFIER_TYPE_VOID
 */
extern data_classifier_type_t const DATA_CLASSIFIER_TYPE_ARRAY [DATA_CLASSIFIER_TYPE_COUNT];

/*!
 *  \brief checks if the classifier type is intended for use in a behavioral context
 *
 *  \param this_ enumeration value
 *  \return true if intended use is solely for state, activity, interaction, use-case contexts
 *          false otherwise, especially excluded are requirements, actors, constraint-blocks, comments
 */
static inline bool data_classifier_type_is_behavioral( data_classifier_type_t this_ );

/*!
 *  \brief returns a short name for the classifier type
 *
 *  \param this_ enumeration value
 *  \return name of the type, "" if unknown or void type
 */
const char * data_classifier_type_get_name( data_classifier_type_t this_ );

#include "entity/data_classifier_type.inl"

#endif  /* DATA_CLASSIFIER_TYPE_H */


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
