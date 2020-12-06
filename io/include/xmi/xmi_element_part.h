/* File: xmi_element_part.h; Copyright and License: see below */

#ifndef XMI_ELEMENT_PART_H
#define XMI_ELEMENT_PART_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines string parts of elements to be exported at xmi export
 */
  
/*!
 *  \brief fallback nesting type 
 * 
 * for the case that the outer model shall nest an element that is illegal according to uml/sysml spec
 */
extern const char * const XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;

/*!
 *  \brief ownedEnd element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_ELEMENT_OWNED_END;

/*!
 *  \brief Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_TYPE_PROPERTY;

/*!
 *  \brief id fragment of source end property of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_ID_FRAGMENT_SOURCE_END;

/*!
 *  \brief id fragment of target end property of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_ID_FRAGMENT_TARGET_END;

/*!
 *  \brief Association attribute of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_ASSOCIATION_ATTRIBUTE;

/*!
 *  \brief Type element of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_TYPE_ELEMENT;

/*!
 *  \brief Class element of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_CLASS_ELEMENT;

/*!
 *  \brief Interface element of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_INTERFACE_ELEMENT;

/*!
 *  \brief Aggregation attribute of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_AGGREGATION_ATTRIBUTE;

/*!
 *  \brief Shared value for aggregation of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_AGGREGATION_SHARED;

/*!
 *  \brief Composite value for aggregation of Property uml element of CommunicationPath and Association
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_AGGREGATION_COMPOSITE;

/*!
 *  \brief MessageEnd is a MessageOccurrenceSpecification
 * 
 *  see uml spec 2.5.1 chapter 17.12.21
 */
extern const char * const XMI_ELEMENT_PART_TYPE_MSG_OCCURRENCE_SPEC;

/*!
 *  \brief covered property denotes the lifeline to which an OccurrenceSpecification belongs
 * 
 *  see uml spec 2.5.1 chapter 17.12.23
 */
extern const char * const XMI_ELEMENT_PART_PROPERTY_OCCURRENCE_SPEC_COVERED;

#endif  /* XMI_ELEMENT_PART_H */


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
