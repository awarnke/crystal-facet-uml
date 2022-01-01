/* File: data_relationship_type.h; Copyright and License: see below */

#ifndef DATA_RELATIONSHIP_TYPE_H
#define DATA_RELATIONSHIP_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines types of relationships
 */

/*!
 *  \brief enumeration of all relationship types
 */
enum data_relationship_type_enum {
    /*DATA_RELATIONSHIP_TYPE_VOID = -1,*/  /* ! < invalid relationship, reserved for future use */
    /* any diagram: */
    DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY = 0,
    /* class and use-case diagram: */
    DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION = 200,
    /* class diagram: */
    DATA_RELATIONSHIP_TYPE_UML_AGGREGATION = 201,
    DATA_RELATIONSHIP_TYPE_UML_COMPOSITION = 202,
    DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION = 210,  /*!< equals inherits from */
    DATA_RELATIONSHIP_TYPE_UML_REALIZATION = 211,  /*!< equals abstraction, use this also for interface realization */
    /* sequence diagram: */
    DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL = 220,  /*!< equals message or signal */
    DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL = 221,  /*!< equals function call */
    DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL = 222,  /*!< equals function result */
    /* composition diagram, block diagram, deployent diagram: */
    DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH = 230,
    /* activity and state and INTERACTION_OVERVIEW_DIAGRAM diagrams: */
    DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW = 240,
    /* activity diagrams: */
    DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW = 241,
    /* DATA_RELATIONSHIP_TYPE_UML_INTERRUPTING_EDGE = 242, */  /* one can alternatively model this using a signal */
    /* deployment diagram: */
    DATA_RELATIONSHIP_TYPE_UML_DEPLOY = 250,
    DATA_RELATIONSHIP_TYPE_UML_MANIFEST = 251,
    /* use case diagram: a use case has the standard direct relations of a classifier plus include and extend */
    DATA_RELATIONSHIP_TYPE_UML_EXTEND = 260,
    DATA_RELATIONSHIP_TYPE_UML_INCLUDE = 261,
    /* package diagram, use-case diagram (for system boundary): */
    DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT = 300,
    /* requirement diagram, SysML: */
    DATA_RELATIONSHIP_TYPE_UML_REFINE = 400,  /*!< adapts the original requirement */
    DATA_RELATIONSHIP_TYPE_UML_TRACE = 401,   /*!< linkt to the unmodified requirement */
    /* DATA_RELATIONSHIP_TYPE_UML_COPY = 402, */  /* is a specialization of DATA_RELATIONSHIP_TYPE_UML_TRACE */
    /* DATA_RELATIONSHIP_TYPE_UML_VERIFY = 403, */  /* is a specialization of DATA_RELATIONSHIP_TYPE_UML_TRACE */
    /* DATA_RELATIONSHIP_TYPE_UML_SATISFY = 404, */  /* is a specialization of DATA_RELATIONSHIP_TYPE_UML_TRACE */
    /* DATA_RELATIONSHIP_TYPE_UML_DERIVE = 405, */  /* is a specialization of DATA_RELATIONSHIP_TYPE_UML_TRACE */
};

typedef enum data_relationship_type_enum data_relationship_type_t;

#define DATA_RELATIONSHIP_TYPE_COUNT (19)

/*!
 *  \brief lists all values of data_relationship_type_t, excluding DATA_RELATIONSHIP_TYPE_VOID
 */
extern data_relationship_type_t const DATA_RELATIONSHIP_TYPE_ARRAY [DATA_RELATIONSHIP_TYPE_COUNT];

#endif  /* DATA_RELATIONSHIP_TYPE_H */


/*
Copyright 2016-2022 Andreas Warnke

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
