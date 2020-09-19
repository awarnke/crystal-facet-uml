/* File: xmi_element_is_a.h; Copyright and License: see below */

#ifndef XMI_ELEMENT_IS_A_H
#define XMI_ELEMENT_IS_A_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Enumerates the uml/sysml types in a way that the bit-pattern represents the generalization-hierarchy
 */

/*!
 *  \brief defines the xmi-export relevant part of the hierarchy of uml/sysml elements
 */
enum xmi_element_is_a_enum {
    XMI_ELEMENT_IS_A_ELEMENT                = 0x00000001,  /*!< the type is a decendant of uml:Element */

    XMI_ELEMENT_IS_A_COMMENT                = 0x00000002 | XMI_ELEMENT_IS_A_ELEMENT,  /*!< the type is a decendant of uml:Comment */
    XMI_ELEMENT_IS_A_NAMED_ELEMENT          = 0x00000004 | XMI_ELEMENT_IS_A_ELEMENT,  /*!< the type is a decendant of uml:NamedElement */
    XMI_ELEMENT_IS_A_RELATIONSHIP           = 0x00000008 | XMI_ELEMENT_IS_A_ELEMENT,  /*!< the type is a decendant of uml:Relationship */

    XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT    = 0x00000100 | XMI_ELEMENT_IS_A_NAMED_ELEMENT,  /*!< the type is a decendant of uml:PackageableElement */
    XMI_ELEMENT_IS_A_PACKAGE                = 0x00000200 | XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT,  /*!< the type is a decendant of uml:Package */
    XMI_ELEMENT_IS_A_CLASSIFIER             = 0x00000400 | XMI_ELEMENT_IS_A_NAMED_ELEMENT | XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT,  /*!< the type is a decendant of uml:Classifier */
    XMI_ELEMENT_IS_A_VERTEX                 = 0x00000800 | XMI_ELEMENT_IS_A_NAMED_ELEMENT,  /*!< the type is a decendant of uml:Vertex */
    XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER  = 0x00001000 | XMI_ELEMENT_IS_A_CLASSIFIER,  /*!< the type is a decendant of uml:BehavioredClassifier */
    XMI_ELEMENT_IS_A_STRUCTURED_CLASSIFIER  = 0x00002000 | XMI_ELEMENT_IS_A_CLASSIFIER,  /*!< the type is a decendant of uml:StructuredClassifier */
    XMI_ELEMENT_IS_A_ENCAPSULATED_CLASSIFIER= 0x00004000 | XMI_ELEMENT_IS_A_STRUCTURED_CLASSIFIER,  /*!< the type is a decendant of uml:EncapsulatedClassifier */
    XMI_ELEMENT_IS_A_CLASS                  = 0x00008000 | XMI_ELEMENT_IS_A_ENCAPSULATED_CLASSIFIER | XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER,  /*!< the type is a decendant of uml:EncapsulatedClassifier */
    XMI_ELEMENT_IS_A_BEHAVIOR               = 0x00010000 | XMI_ELEMENT_IS_A_ENCAPSULATED_CLASSIFIER | XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER,  /*!< the type is a decendant of uml:Behavior */

    XMI_ELEMENT_IS_A_ASSOCIATION            = 0x01000000 | XMI_ELEMENT_IS_A_RELATIONSHIP | XMI_ELEMENT_IS_A_CLASSIFIER,  /*!< the type is a decendant of uml:Association */
    XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP  = 0x02000000 | XMI_ELEMENT_IS_A_RELATIONSHIP,  /*!< the type is a decendant of uml:DirectedRelationship */
    XMI_ELEMENT_IS_A_DEPENDENCY             = 0x04000000 | XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP | XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT,  /*!< the type is a decendant of uml:Dependency */
    XMI_ELEMENT_IS_A_ABSTRACTION            = 0x08000000 | XMI_ELEMENT_IS_A_DEPENDENCY,  /*!< the type is a decendant of uml:Abstraction */
};

typedef enum xmi_element_is_a_enum xmi_element_is_a_t;

#endif  /* XMI_ELEMENT_IS_A_H */


/*
Copyright 2020-2020 Andreas Warnke

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
